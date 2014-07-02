//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogSender.cpp
//
// DESCRIPTION: Contains the implementation for the class LogSender
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 09 Jan 2003		 B. Shirey	 File created.
//*************************************************************************

#define LogDll_File

#include "RegistrationMgr.h"
#include "logsender.h"
#include "Registry.h"
#include <psapi.h>
#include <windows.h>

using namespace HolodeckRegistration;

namespace Log
{
	HolodeckLog_Api LogSender commonLogSender;

	HANDLE LogSender::heap = NULL;
	int LogSender::numLogEntriesSent = 0;
	DWORD LogSender::parentLogDisabledTlsSlot = TLS_OUT_OF_INDEXES;

	//*************************************************************************
	// Method:		LogSender
	// Description: Constructor for the LogSender class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogSender::LogSender() : sendLogThread(INVALID_HANDLE_VALUE), 
		sendLogThreadRunning(false), mappedFileName(NULL), canSendLogs(true)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		InitializeCriticalSection(&logGuard);

		int pid = GetCurrentProcessId();

		// get the process id in string form
		char pidStr[10];
		itoa(pid, pidStr, 10);

		SiString fileName, logFolderName;
		SiUtils::Registry registry;

		// get the install path
		registry.OpenKey(INSTALLED_ROOT_KEY, ROOT_PATH, KEY_QUERY_VALUE);
		registry.Read(INSTALL_PATH_VALUE_NAME, fileName);
		if (!registry.Read(LOG_PATH_VALUE_NAME, logFolderName))
			logFolderName = LOG_FOLDER_DEFAULT_VALUE;
		registry.Close();

		// get the mapped file name
		mappedFileName = (SiString *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(SiString));
		*mappedFileName = LOG_SENT_MAP_FILE_PREFIX;
		*mappedFileName += pidStr;

		entryCountFile.Open(*mappedFileName);

		// make the filename
		fileName += "\\";
		fileName += logFolderName;

		// create the directory if it doesn't exist
		CreateDirectory(fileName, NULL);

		fileName += "\\";
		fileName += pidStr;
		fileName += ".hdl";

		PSECURITY_DESCRIPTOR pSD;
		SECURITY_ATTRIBUTES sa;

		pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
		// Initialize a security attributes structure.
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = FALSE;

		logFile = CreateFile(fileName, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, &sa);

		SiString cantCreateLogFileEventName = CANT_CREATE_LOG_FILE_PREFIX;
		cantCreateLogFileEventName += pidStr;

		cantCreateLogFileEvent.Create(cantCreateLogFileEventName);
		if (logFile == INVALID_HANDLE_VALUE)
		{
			canSendLogs = false;
			cantCreateLogFileEvent.Signal();
		}

		// get the image name
		char *imageName = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, 1024);
		GetModuleFileName(NULL, imageName, 1024);
		SiString appPath = imageName;
		HeapFree(heap, HEAP_ZERO_MEMORY, imageName);

		if (canSendLogs)
		{
			// write the version info at the beginning of the file (2.5 and later only)
			DWORD numWritten;
			SiString version = LOG_VERSION_STRING;
			DWORD versionSize = version.GetLength();
			WriteFile(logFile, &versionSize, sizeof(unsigned long), &numWritten, NULL);
			WriteFile(logFile, (char *)version, versionSize, &numWritten, NULL);

			// write the app path and proc id		
			DWORD appPathSize = appPath.GetLength();
			WriteFile(logFile, &pid, sizeof(int), &numWritten, NULL);
			WriteFile(logFile, &appPathSize, sizeof(unsigned long), &numWritten, NULL);
			WriteFile(logFile, (char *)appPath, appPathSize, &numWritten, NULL);
		}

		SiString logsSentEventName = LOG_SENT_EVENT_NAME_PREFIX;
		logsSentEventName += pidStr;

		SiString logsReceivedEventName = NOTIFIER_DONE_EVENT_PREFIX;
		logsReceivedEventName += pidStr;

		SiString heatInjectionDoneEventName = HEAT_INJECT_DONE_PREFIX;
		heatInjectionDoneEventName += pidStr;

		logsNeedToBeSentEvent.Create();
		logsSentEvent.Create(logsSentEventName);
		logsCanBeAddedEvent.Create();
		logsReceivedEvent.Create(logsReceivedEventName);

		heatInjectionDone = false;
		heatInjectionDoneEvent.Create(heatInjectionDoneEventName);

		SiString mutexName = LOG_FILE_MUTEX_PREFIX;
		mutexName += pidStr;
		logFileMutex.Create(mutexName);

		mutexName = FORCE_WRITE_MUTEX_PREFIX;
		mutexName += pidStr;
		forceWriteGuard.Create(mutexName);
	}

	//*************************************************************************
	// Method:		~LogSender
	// Description: Destructor for the LogSender class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogSender::~LogSender()
	{
		HMODULE module;

		// disable interception
		module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
		if (module == NULL)
			module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

		DWORD (WINAPI *disableFunc)() = (DWORD (WINAPI *)())::GetProcAddress(module, DISABLE_INTERCEPT_FUNC_NAME);
		disableFunc();

		StopSendLogThread();

		if (logFile != INVALID_HANDLE_VALUE)
			CloseHandle(logFile);

		DeleteCriticalSection(&logGuard);

		logFileMutex.Close();
		forceWriteGuard.Close();

		logsSentEvent.Release();

		logsNeedToBeSentEvent.Close();
		logsSentEvent.Close();
		logsCanBeAddedEvent.Close();

		entryCountFile.Close();

		if (mappedFileName != NULL)
		{
			HeapFree(heap, HEAP_ZERO_MEMORY, mappedFileName);
			mappedFileName = NULL;
		}

		// don't reenable interception because it will crash and we don't want to
		// log anything anyway
	}

	//*************************************************************************
	// Method:		StartSendLogThread
	// Description: Starts the send log thread.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogSender::StartSendLogThread()
	{
		DWORD threadID;
		sendLogThreadRunning = true;
		sendLogThread = CreateAutThread(SendLogThreadFunction, this, &threadID);
	}

	//*************************************************************************
	// Method:		StopSendLogThread
	// Description: Stops the send log thread.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogSender::StopSendLogThread()
	{
		if (sendLogThreadRunning)
		{
			while (!queue.IsEmpty())
				ForceWrite();

			// stop the thread and wait for it
			sendLogThreadRunning = false;
			logsNeedToBeSentEvent.Signal();
			WaitForSingleObject(sendLogThread, INFINITE);
			CloseHandle(sendLogThread);
			sendLogThread = INVALID_HANDLE_VALUE;
		}
	}

	//*************************************************************************
	// Method:		SendLogThreadFunction
	// Description: Function that runs in another thread to send logs as needed
	//
	// Parameters:
	//	param - The parameter to the thread function.  Should be a pointer
	//		to the LogSender object that started the thread
	//
	// Return Value: None
	//*************************************************************************
	unsigned long WINAPI LogSender::SendLogThreadFunction(void *param)
	{
		LogSender *sender = (LogSender *)param;

		HMODULE module;
		
		module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
		if (module == NULL)
			module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

		if (module == NULL)
			return -1;

		DWORD (WINAPI *disableFunc)() = (DWORD (WINAPI *)())::GetProcAddress(module, DISABLE_INTERCEPT_FUNC_NAME);
		disableFunc();

		while (sender->sendLogThreadRunning)
		{
			unsigned int result = sender->logsNeedToBeSentEvent.Acquire(SEND_LOG_WAIT_TIME);

			// no need to put this in a critical section because only one object
			// is ever running this code and no logs will be added due to the
			// critical section object in sendLog()
			if (!sender->queue.IsEmpty())
			{
				sender->ForceWrite();
			}
		}

		TerminateThread(GetCurrentThread(), 0);
		return 0;
	}

	//*************************************************************************
	// Method:		ForceWrite
	// Description: Function that actually writes the log entries in the
	//	log queue to disk and notifies the receiver that entries have been
	//	written
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogSender::ForceWrite()
	{
		if (!sendLogThreadRunning)
			return;

		forceWriteGuard.Acquire();

		if (!queue.IsEmpty())
		{
			// acquire the logGuard critical section for the GetTotalLogSizes and GetLogCount
			// call pair.  If a thread was able to add a log entry between these two calls, the
			// values of totalSize and numEntries would be out of sync, causing problems for the
			// log receiver.
			EnterCriticalSection(&logGuard);

			// get the size of all the log data to send
			int totalSize = queue.GetTotalLogSizes();

			// get the number of log entries to send
			unsigned int numEntries = queue.GetLogCount();

			LeaveCriticalSection(&logGuard);

			// add the space for the sizes before the log data
			totalSize += numEntries * sizeof(unsigned int);

			// add the space for the number of logs we are sending
			totalSize += sizeof(unsigned int);

			// allocate the memory for the buffer
			unsigned char *data = (unsigned char *) HeapAlloc(heap, HEAP_ZERO_MEMORY, totalSize);
			if (data != NULL)
			{
				int offset = 0;
				
				// put the number of entries in the data
				CopyMemory((data + offset), (void *)&numEntries, sizeof(unsigned int));
				offset += sizeof(unsigned int);

				// reserve space for the log entry offsets
				offset += numEntries * sizeof(unsigned int);

				// put the rest of the data in
				for (unsigned int i=0; i<numEntries; i++)
				{
					unsigned int size = queue.GetNextLogSize();

					// Save the offset to this entry in the log packet header.  These offsets should
					// follow directly after the number of entries.
					*((unsigned int*)(data + (i * sizeof(unsigned int)) + sizeof(unsigned int))) = offset;

					queue.PopLog(size, (data + offset));
					offset += size;
				}

				// write the data to file
				while (true)
				{
					unsigned int result = logFileMutex.Acquire(SEND_LOG_WAIT_TIME);
					if (result == WAIT_OBJECT_0)
						break;

					if (!sendLogThreadRunning)
					{
						HeapFree(heap, HEAP_ZERO_MEMORY, data);
						forceWriteGuard.Release();
						return;
					}
				}

				SetFilePointer(logFile, 0, 0, FILE_END);
				
				// get the current position in the file to send to the receiver
				DWORD currPos = SetFilePointer(logFile, 0, 0, FILE_CURRENT);

				DWORD numWritten;
				WriteFile(logFile, &totalSize, sizeof(unsigned int), &numWritten, NULL);
				WriteFile(logFile, data, sizeof(unsigned char) * totalSize, &numWritten, NULL);
				FlushFileBuffers(logFile);
			
				char numEntriesString[12];
				_itoa(numEntries, numEntriesString, 10);

				char currPosString[12];
				wsprintf(currPosString, "%lu", currPos);

				size_t numEntriesSize = lstrlenA(numEntriesString);
				size_t currPosSize = lstrlenA(currPosString);

				// make sure this is at least as big as 2 more than the combined
				// total of numEntriesString and currPosString
				char entryCountMsg[35];
				FillMemory(entryCountMsg, 35, 0);

				CopyMemory(entryCountMsg, numEntriesString, numEntriesSize);
				lstrcatA(entryCountMsg, " ");
				CopyMemory(entryCountMsg + numEntriesSize + 1, currPosString, currPosSize);

				entryCountFile.Write(entryCountMsg, (unsigned int)(lstrlenA(entryCountMsg) + 1));

				logFileMutex.Release();

				logsReceivedEvent.Release();
				// notify the receiver
				logsSentEvent.Signal();

				HeapFree(heap, HEAP_ZERO_MEMORY, data);

				while (sendLogThreadRunning)
				{
					unsigned int result = logsReceivedEvent.Acquire(SEND_LOG_WAIT_TIME);
					if (result == WAIT_OBJECT_0)
						break;
				}
			}
		}

		forceWriteGuard.Release();
	}

	//*************************************************************************
	// Method:		CreateAutThread
	// Description: Creates an aut thread through Heat
	//
	// Parameters:
	//	routine - the thread routine to execute
	//	param - the parameter to pass to the thread function
	//	pdwThreadID (out) - the thread id of the new thread
	//
	// Return Value: the handle to the created thread
	//*************************************************************************
	HANDLE LogSender::CreateAutThread(LPTHREAD_START_ROUTINE routine, LPVOID param, DWORD *pdwThreadID)
	{
		HMODULE module;
		static HANDLE (WINAPI *createThreadFunc)(LPTHREAD_START_ROUTINE, LPVOID, DWORD *) = NULL;
		
		if (createThreadFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return INVALID_HANDLE_VALUE;

			createThreadFunc = (HANDLE (WINAPI *)(LPTHREAD_START_ROUTINE, LPVOID, DWORD *))::GetProcAddress(module, CREATE_THREAD_FUNC_NAME);
		}

		return createThreadFunc(routine, param, pdwThreadID);
	}
	
	//*************************************************************************
	// Method:		IsAutCreatedThread
	// Description: determines whether or not a thread was created by holodeck
	//	in the aut
	//
	// Parameters:
	//	dwThreadID - the thread id of the thread to check
	//
	// Return Value: true if it was created by us, false otherwise
	//*************************************************************************
	bool LogSender::IsAutCreatedThread(DWORD dwThreadID)
	{
		HMODULE module;
		static bool (WINAPI *isCreatedThreadFunc)(DWORD) = NULL;
		
		if (isCreatedThreadFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return false;

			isCreatedThreadFunc = (bool (WINAPI *)(DWORD))::GetProcAddress(module, IS_CREATED_THREAD_FUNC_NAME);
		}

		return isCreatedThreadFunc(dwThreadID);
	}

	//*************************************************************************
	// Method:		SendLog
	// Description: Sends a log of a function to the HoloScript object listening
	//	for them
	//
	// Parameters:
	//	tID - The ID of the thread sending the log
	//	threadID - the string representation of the thread id
	//	category - The category to check for logging enabled
	//	functionName - The function to check for logging enabled
	//	dllName - The name of the dll that is sending the log
	//	numParams - The number of parameters in the parameters array
	//	parameters - A list of parameter values to a function.  Note all strings
	//		must be converted from char*, wchar_t*, etc to an SiString object
	//		to be correctly interpreted.
	//	paramTypes - A list of parameter types to a function (must be same #
	//		elements as parameters has)
	//	errorCode - The error code from the function
	//  exception - Name of exception thrown by this function, empty string
	//		for none
	//	returnValue - The return value from the function
	//
	// Return Value: true if enabled, false otherwise
	//*************************************************************************
	void LogSender::SendLog(DWORD tID, const SiString &threadID, const SiString &category, const SiString &functionName, const SiString &dllName, unsigned int numParams, void **parameters, ParameterType *paramTypes, DWORD errorCode, const SiString& exception, const SiString& returnValue, const LogEventList& eventList)
	{
		if (!canSendLogs)
			return;

		// don't log our threads
		if (IsAutCreatedThread(tID))
			return;

		if (!heatInjectionDone)
		{
			if (heatInjectionDoneEvent.IsValidEvent())
			{
				unsigned int result = heatInjectionDoneEvent.Acquire(0);
				if (result != WAIT_OBJECT_0)
					return;
				else
					heatInjectionDone = true;
			}
			else
			{
				// there was a wierd error creating or opening the event handle
				// don't block logging in this case
				heatInjectionDone = true;
			}
		}

		if ((RegistrationMgr::GetInstance()->IsFeatureDisabledTrial()) && 
			(++numLogEntriesSent > MAX_TRIAL_LOG_ENTRIES))
			return;

		const int bufferSize = 30; // big enough to hold error code
		char buffer[bufferSize + 1];
		LogEntry logEntry;

		if (numParams > MAX_PARAMETER_COLUMNS)
			numParams = MAX_PARAMETER_COLUMNS;

		logEntry.SetThreadID (threadID);
		logEntry.SetCallDepth(GetCurrentCallLevel());
		logEntry.SetCategory(category);
		logEntry.SetFunction(functionName);
		logEntry.SetDllName(dllName);

		logEntry.SetReturnValue(returnValue);

		// convert the error code to a string
		ZeroMemory(buffer, bufferSize + 1);
		_ultoa(errorCode, buffer, 10);
		logEntry.SetErrorCode(buffer);

		logEntry.SetException(exception);

		for (unsigned int i = 0; i < numParams; i++)
		{
			if ((parameters == NULL) || (parameters[i] == NULL))
				logEntry.SetParameter(i, "0", 1);
			else
			{
				switch(paramTypes[i])
				{
					case BooleanType:
					{
						bool value = *((bool *)parameters[i]);

						if (value)
							logEntry.SetParameter(i, LOG_TRUE_DISPLAY_STRING, (unsigned int)lstrlenA("True"));
						else
							logEntry.SetParameter(i, LOG_FALSE_DISPLAY_STRING, (unsigned int)lstrlenA("False"));

						break;
					}
					case StringType:
					{
						SiString value = *((SiString *)parameters[i]);
						logEntry.SetParameter(i, value, value.GetLength());
						break;
					}
					case OutStringType:
					{
						LPSTR value = *((LPSTR *)parameters[i]);
						SiString valueStr = value;
						logEntry.SetParameter(i, valueStr, valueStr.GetLength());
						break;
					}
					case WideStringType:
					{
						SiString value = *((SiString *)parameters[i]);
						logEntry.SetParameter(i, value, value.GetLength());
						break;
					}
					case OutWideStringType:
					{
						LPWSTR value = *((LPWSTR *)parameters[i]);
						SiString valueStr = value;
						logEntry.SetParameter(i, valueStr, valueStr.GetLength());
						break;
					}
					case IIDRefType:
					{
						IID value = *((IID *)parameters[i]);
						LPOLESTR valueStr = NULL;
						StringFromIID(value, &valueStr);
						SiString tempStr = valueStr;
						logEntry.SetParameter(i, tempStr, tempStr.GetLength());
						CoTaskMemFree(valueStr);
						break;
					}
					case RealType:
					{
						float value = *((float *)parameters[i]);
						int decPos, sign;
						ZeroMemory(buffer, bufferSize + 1);
						CopyMemory(buffer, _fcvt(value, 4, &decPos, &sign), bufferSize);
						logEntry.SetParameter(i, buffer, bufferSize);
						break;
					}
					case Integer64Type:
					{
						ZeroMemory(buffer, bufferSize + 1);
						__int64 value = *((__int64 *)parameters[i]);
						_i64toa(value, buffer, 10);
						logEntry.SetParameter(i, buffer, bufferSize);
						break;
					}
					case PointerType:
					{
						ZeroMemory(buffer, bufferSize + 1);
						unsigned long value = *((unsigned long *)parameters[i]);
						if (value == 0)
						{
							strcpy(buffer, "NULL");
						}
						else
							_ultoa(value, buffer, 16);

						logEntry.SetParameter(i, buffer, bufferSize);
						break;
					}
					case UnsignedLongType:
					{
						ZeroMemory(buffer, bufferSize + 1);
						unsigned long value = *((unsigned long *)parameters[i]);
						_ultoa(value, buffer, 16);
						logEntry.SetParameter(i, buffer, bufferSize);
						break;
					}
					case IntegerType: // fall through intentional
					default:
					{
						ZeroMemory(buffer, bufferSize + 1);
						int value = *((int *)parameters[i]);
						_itoa(value, buffer, 10);
						logEntry.SetParameter(i, buffer, bufferSize);
						break;
					}
				}
			}
		}

		// Fill in the events
		for (unsigned int i=0; i<eventList.GetLength(); i++)
			logEntry.SetEvent(i, eventList.GetEvent(i));

		// send the data, emptying the queue
		if (queue.IsFull())
			ForceWrite();

		EnterCriticalSection(&logGuard);
	
		// get the timestamp string
		SYSTEMTIME currentTime;
		GetLocalTime(&currentTime);
		logEntry.SetTimeStamp(currentTime);

		// put the log in the queue
		unsigned int dataSize = logEntry.GetLogSize();
		unsigned char *data = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, dataSize);
		logEntry.ConvertToByteSequence(data, dataSize);
		queue.PushLog(dataSize, data);
		HeapFree(heap, HEAP_ZERO_MEMORY, data);

		LeaveCriticalSection(&logGuard);
	}

	//*************************************************************************
	// Method:		GetCurrentCallLevel
	// Description: Gets the current thread's call level from heat
	//
	// Parameters:
	//	None
	//
	// Return Value: the thread's current call level
	//*************************************************************************
	unsigned int LogSender::GetCurrentCallLevel()
	{
		HMODULE module;
		static DWORD (WINAPI *callLevelFunc)() = NULL;
		
		if (callLevelFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return 0;

			callLevelFunc = (DWORD (WINAPI *)())::GetProcAddress(module, GET_CALL_LEVEL_FUNC_NAME);
		}

		return callLevelFunc() - 1;
	}

	bool LogSender::IsParentLogged()
	{
		if (parentLogDisabledTlsSlot == TLS_OUT_OF_INDEXES)
			parentLogDisabledTlsSlot = TlsAlloc();

		PVOID value;
		if (parentLogDisabledTlsSlot >= 64)
		{
			value = TlsGetValue(parentLogDisabledTlsSlot);
		}
		else
		{
			_asm
			{
				mov	eax, fs:[0x18]
				mov	ecx, parentLogDisabledTlsSlot
				mov	eax, [eax+ecx*4+0xE10]
				//and	fs:[0x34], 0
				mov	[value], eax
			}
		}

		return (value == 0);
	}

	void LogSender::SetParentLogged(bool value)
	{
		if (parentLogDisabledTlsSlot == TLS_OUT_OF_INDEXES)
			parentLogDisabledTlsSlot = TlsAlloc();

		TlsSetValue(parentLogDisabledTlsSlot, (LPVOID)(size_t)(value ? 0 : 1));
	}
}
