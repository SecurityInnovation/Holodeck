//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReceiver.cpp
//
// DESCRIPTION: Contains the implementation for the class LogReceiver
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogReceiver.h"
#include <vcclr.h>

namespace HoloScript
{
	//*************************************************************************
	// Method:		LogReceiver
	// Description: Constructor for the LogReceiver class
	//
	// Parameters:
	//	procID - The id of the process to receive from
	//
	// Return Value: None
	//*************************************************************************
	LogReceiver::LogReceiver(int procID) : logReceivedThread(NULL), 
		receiveLogThreadRunning(false), listeners(NULL), logsSentEvent(NULL),
		logsReceivedEvent(NULL)
	{
		listeners = new ArrayList();

		char pidStr[10];
		_itoa(procID, pidStr, 10);

		SiString mappedFileName = LOG_SENT_MAP_FILE_PREFIX;
		mappedFileName += pidStr;

		entryCountFile = new MemoryMappedFile();
		entryCountFile->Create(mappedFileName, 256);

		String *logsSentEventName = String::Concat(LOG_SENT_EVENT_NAME_PREFIX, pidStr);
		String *logsReceivedEventName = String::Concat(NOTIFIER_DONE_EVENT_PREFIX, pidStr);

		const __wchar_t __pin *sentEventName = PtrToStringChars(logsSentEventName);
		const __wchar_t __pin *receivedEventName = PtrToStringChars(logsReceivedEventName);

		logsSentEvent = new Event();
		logsSentEvent->Create(sentEventName);

		logsReceivedEvent = new Event();
		logsReceivedEvent->Create(receivedEventName);
	}

	//*************************************************************************
	// Method:		~LogReceiver
	// Description: Destructor for the LogReceiver class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReceiver::~LogReceiver()
	{
		// stop the thread and wait for it
		if (receiveLogThreadRunning)
		{
			receiveLogThreadRunning = false;
			logsSentEvent->Signal();
			logReceivedThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
			logReceivedThread = NULL;
		}

		if (entryCountFile != NULL)
		{
			entryCountFile->Close();
			delete entryCountFile;
			entryCountFile = NULL;
		}

		if (logsSentEvent)
		{
			logsSentEvent->Close();
			delete logsSentEvent;
			logsSentEvent = NULL;
		}

		if (logsReceivedEvent)
		{
			logsReceivedEvent->Close();
			delete logsReceivedEvent;
			logsReceivedEvent = NULL;
		}

		if (listeners)
		{
			listeners->Clear();
			listeners = NULL;
		}
	}

	//*************************************************************************
	// Method:		ReceiveLogThreadFunction
	// Description: Function that runs in another thread to receive logs as needed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogReceiver::ReceiveLogThreadFunction()
	{
		DWORD result;
		DWORD timeWithoutLog = 0;

		while (receiveLogThreadRunning)
		{
			if (logsSentEvent)
				result = logsSentEvent->Acquire(RECEIVE_LOG_WAIT_TIME);

			if (receiveLogThreadRunning)
			{
				if (result == WAIT_TIMEOUT)
				{
					timeWithoutLog += RECEIVE_LOG_WAIT_TIME;
					if (timeWithoutLog >= 1000)
					{
						// 1 second without a log.  Update the monitors
						timeWithoutLog = 0;
						ReceivedLogInfo *info = new ReceivedLogInfo();
						info->LogEntryCount = 0;
						info->FilePosition = 0;
						NotifyLogReceivedListeners(info);
						delete info;
					}
				}
				else
				{
					//event was acquired.  Send the logs
					timeWithoutLog = 0;

					int numLogEntries = 0;
					unsigned long filePos;

					unsigned int size = entryCountFile->GetNumberOfBytesInFile();
					char *msg = new char[size + 1];
					entryCountFile->Read(msg);

					// find the space character
					char *spacePos = strchr(msg, ' ');
					if (spacePos == NULL)
					{
						delete[] msg;
						continue;
					}

					char *posStr = spacePos + 1;
					*spacePos = '\0';

					numLogEntries = atoi(msg);
					char *stopChars;
					filePos = strtoul(posStr, &stopChars, 10);

					delete[] msg;

					ReceivedLogInfo *info = new ReceivedLogInfo();
					info->FilePosition = filePos;
					info->LogEntryCount = numLogEntries;
					info->Version = CurrentHolodeckVersion;

					try
					{
						// call all of the listeners
						NotifyLogReceivedListeners(info);
					}
					catch (System::Exception* e)
					{
						const wchar_t __pin* wszMessage = PtrToStringChars(System::String::Concat(
							"Unhandled exception in log listener:\n", e->ToString()));
						MessageBoxW(NULL, wszMessage, L"Error", MB_OK | MB_ICONSTOP | MB_TOPMOST | MB_TASKMODAL);
					}

					if (receiveLogThreadRunning)
						logsReceivedEvent->Signal();
					delete info;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		AddLogReceivedListener
	// Description: registers a callback function to be called on log received
	//
	// Parameters:
	//	callback - the function to call on log received
	//
	// Return Value: None
	//*************************************************************************
	void LogReceiver::StartReceivingLogs()
	{
		receiveLogThreadRunning = true;
		logReceivedThread = new Thread(new ThreadStart(this, ReceiveLogThreadFunction));
		logReceivedThread->IsBackground = true;
		logReceivedThread->Start();
	}

	//*************************************************************************
	// Method:		NotifyLogReceivedListeners
	// Description: Calls the callback function for all registered log received
	//	listeners
	//
	// Parameters:
	//	numNewReceivedLogs - The number of new log entries generated
	//
	// Return Value: None
	//*************************************************************************
	void LogReceiver::NotifyLogReceivedListeners(ReceivedLogInfo *info)
	{
		if (!listeners)
			return;

		IEnumerator *enumerator = listeners->GetEnumerator();
		try
		{
			while (enumerator->MoveNext())
			{
				NotifiableLogReceiver *receiver = dynamic_cast<NotifiableLogReceiver *>(enumerator->Current);
				if (receiver != NULL)
					receiver->NewLogsReceived(info);
			}
		}
		catch (InvalidOperationException *)
		{
			//Lost a reciever.  Start over but don't lose the log info.
			NotifyLogReceivedListeners(info);
		}

	}

	//*************************************************************************
	// Method:		AddLogReceivedListener
	// Description: registers a callback function to be called on log received
	//
	// Parameters:
	//	receiver - the object to call on log received
	//
	// Return Value: None
	//*************************************************************************
	void LogReceiver::AddLogReceivedListener(NotifiableLogReceiver *receiver)
	{
		if (receiver != NULL)
			listeners->Add(receiver);
	}

	//*************************************************************************
	// Method:		RemoveLogReceivedListener
	// Description: unregisters a callback function to be called on log received
	//
	// Parameters:
	//	receiver - the object to no longer call on log received
	//
	// Return Value: None
	//*************************************************************************
	void LogReceiver::RemoveLogReceivedListener(NotifiableLogReceiver *receiver)
	{
		if (receiver != NULL)
			listeners->Remove(receiver);	
	}
}
