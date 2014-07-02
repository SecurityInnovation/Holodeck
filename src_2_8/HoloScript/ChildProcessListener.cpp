//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ChildProcessListener.cpp
//
// DESCRIPTION: Contains implementation for the class ChildProcessListener
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "ChildProcessListener.h"

namespace HoloScript
{
	//*************************************************************************
	// Method:		ChildProcessListener
	// Description: Constructor for the ChildProcessListener class
	//
	// Parameters:
	//	procID - the process id
	//
	// Return Value: None
	//*************************************************************************
	ChildProcessListener::ChildProcessListener(unsigned long procID)
	{
		char pidStr[12];
		_ultoa(procID, pidStr, 10);

		SiString mappedFileName = CHILD_PROC_MAP_FILE_PREFIX;
		mappedFileName += pidStr;

		childProcessDataFile = new MemoryMappedFile();
		childProcessDataFile->Create(mappedFileName, 256);

		SiString childProcessCreatedEventName = CHILD_PROCESS_EVENT_PREFIX;
		childProcessCreatedEventName += pidStr;

		SiString holodeckAttachedToChildProcessEventName = HOLODECK_ATTACHED_PREFIX;
		holodeckAttachedToChildProcessEventName += pidStr;

		childProcessCreatedEvent = new Event();
		childProcessCreatedEvent->Create(childProcessCreatedEventName);

		holodeckAttachedToChildProcessEvent = new Event();
		holodeckAttachedToChildProcessEvent->Create(holodeckAttachedToChildProcessEventName);
	}

	//*************************************************************************
	// Method:		~ChildProcessListener
	// Description: Destructor for the ChildProcessListener class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ChildProcessListener::~ChildProcessListener()
	{
		// stop the thread and wait for it
		if (listenThreadRunning)
		{
			listenThreadRunning = false;
			childProcessCreatedEvent->Signal();
			childProcessCreatedListenerThread->Join(WAIT_FOR_THREAD_JOIN_TIMEOUT);
		}

		if (childProcessDataFile != NULL)
		{
			childProcessDataFile->Close();
			delete childProcessDataFile;
			childProcessDataFile = NULL;
		}

		childProcessCreatedEvent->Close();
		holodeckAttachedToChildProcessEvent->Close();
	}

	//*************************************************************************
	// Method:		StartListeningForChildProcesses
	// Description: starts the listen thread
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ChildProcessListener::StartListeningForChildProcesses()
	{
		listenThreadRunning = true;
		childProcessCreatedListenerThread = new Thread(new ThreadStart(this, ListenThreadFunction));
		childProcessCreatedListenerThread->IsBackground = true;
		childProcessCreatedListenerThread->Start();
	}

	//*************************************************************************
	// Method:		ListenThreadFunction
	// Description: runs in another thread and waits for child processes to
	//	be created
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ChildProcessListener::ListenThreadFunction()
	{
		DWORD result;

		while (listenThreadRunning)
		{
			result = childProcessCreatedEvent->Acquire(RECEIVE_LOG_WAIT_TIME);

			if (listenThreadRunning)
			{
				if (result != WAIT_TIMEOUT)
				{
					bool resume;

					unsigned int size = childProcessDataFile->GetNumberOfBytesInFile();
					char *msg = new char[size + 1];
					childProcessDataFile->Read(msg);

					// find the space character
					char *flag = strtok(msg, " ");
					if (flag == NULL)
					{
						delete[] msg;
						continue;
					}

					bool newProcess = false;
					if (strcmp(flag, NEW_PROCESS_FLAG) == 0)
						newProcess = true;

					char *idStr = strtok(NULL, " ");
					if (idStr == NULL)
					{
						delete[] msg;
						continue;
					}
					unsigned long id = atoi(idStr);

					char *resumeString = strtok(NULL, " ");
					if (resumeString == NULL)
					{
						delete[] msg;
						continue;
					}

					resume = (strcmp(resumeString, "0") == 0) ? false : true;

					delete[] msg;

					if (this->OnChildProcessCreated)
						OnChildProcessCreated->Invoke(newProcess, id, resume);

					this->holodeckAttachedToChildProcessEvent->Signal();
				}
			}
		}
	}
}
