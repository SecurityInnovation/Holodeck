//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReader.cpp
//
// DESCRIPTION: Contains implementation for the class LogReader
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogReader.h"

namespace HoloScript
{
	//*************************************************************************
	// Method:		LogReader
	// Description: Constructor for the LogReader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReader::LogReader() : logFileMutex(NULL), totalLogCount(0), version(CurrentHolodeckVersion)
	{
		LogsCache = new ArrayList();
		grabbedLogEntries = new Queue();
		logInfoList = new ArrayList();
		logIndexInfoTable = new Hashtable();
		logFileName = new String("");

		logEventMutex = new SiUtils::Mutex();
		logEventMutex->Create();

		// Initialize GetLogEntryFromFile cache
		cachedLogOffsets = NULL;
		cachedLogInfoPtr = NULL;
	}

	//*************************************************************************
	// Method:		~LogReader
	// Description: Destructor for the LogReader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReader::~LogReader()
	{
		if (logFileMutex != NULL)
		{
			logFileMutex->Close();
			delete logFileMutex;
			logFileMutex = NULL;
		}
		if (logEventMutex != NULL)
		{
			logEventMutex->Close();
			delete logEventMutex;
			logEventMutex = NULL;
		}

		//if handle 'logFile' has already been closed; then CloseHandle
		//will throw an exception (in dbg mode only) which we need to handle
		try
		{
			CloseHandle(logFile);
			logFile = NULL;
		}
		catch (...)
		{
		}
	}

	//*************************************************************************
	// Method:		GetLogEntryAt
	// Description: Get a given log entry from the aut
	//
	// Parameters:
	//	index - The log entry number to get
	//  filePosition - The position of the entry in the file
	//
	// Return Value: The log entry at the index given
	//*************************************************************************
	LogItem *LogReader::GetLogEntryAt(int index, unsigned long &filePosition)
	{
		if (index < 0)
			return NULL;

		// Check to see if we are reading from the already cached ReceivedLogInfo
		if (cachedLogInfoPtr && (index >= cachedLogInfoPtr->StartIndex) &&
			(index < (cachedLogInfoPtr->StartIndex + cachedLogInfoPtr->LogEntryCount)))
			return this->GetLogEntryFromFile(cachedLogInfoPtr, index - cachedLogInfoPtr->StartIndex, filePosition);

		// Look for the correct ReceivedLogInfo with a binary search
		int min = 0;
		int max = logInfoList->Count - 1;
		ReceivedLogInfo *info = NULL;
		for (int i = (min + max) / 2; min <= max; i = (min + max) / 2)
		{
			info = dynamic_cast<ReceivedLogInfo *>(logInfoList->Item[i]);
			if (index < info->StartIndex)
			{
				// Our entry is earlier in the log
				max = i - 1;
			}
			else if (index >= (info->StartIndex + info->LogEntryCount))
			{
				// Our entry is later in the log
				min = i + 1;
			}
			else
			{
				// Found the correct ReceivedLogInfo
				break;
			}
		}

		if (info)
			return this->GetLogEntryFromFile(info, index - info->StartIndex, filePosition);
		return NULL;
	}

	//*************************************************************************
	// Method:		GetLogEntryFromFile
	// Description: Get a given log entry from the log file
	//
	// Parameters:
	//	info - The info of the log block to get it from
	//	index - The log entry number in the info block to get
	//  filePosition - The position of the file of the entry
	//
	// Return Value: The log entry at the given location
	//*************************************************************************
	LogItem *LogReader::GetLogEntryFromFile(ReceivedLogInfo *info, int index, unsigned long &filePosition)
	{
		if (info == NULL)
			return NULL;

		unsigned long numRead = 0;

		if (logFileMutex != NULL)
			logFileMutex->Acquire(INFINITE);

		// Only re-read the log entry offset information if we don't have the current ReceivedLogInfo cached already
		if ((cachedLogInfoPtr != info) || (cachedLogOffsets == NULL))
		{
			if (cachedLogOffsets)
			{
				// Free any previously cached entries
				delete[] cachedLogOffsets;
			}

			// get the total size of the sent packet
			unsigned long filePos = info->FilePosition;
			SetFilePointer(logFile, filePos, 0, FILE_BEGIN);
			unsigned int totalSize = 0;
			ReadFile(logFile, &totalSize, sizeof(unsigned int), &numRead, NULL);

			// get the number of entries
			unsigned int numEntries = 0;
			ReadFile(logFile, &numEntries, sizeof(unsigned int), &numRead, NULL);

			cachedLogInfoPtr = info;
			cachedLogOffsets = new unsigned int[numEntries];

			// get the offsets of each log entry in the sent packet
			ReadFile(logFile, cachedLogOffsets, numEntries * sizeof(unsigned int), &numRead, NULL);
		}

		filePosition = info->FilePosition + cachedLogOffsets[index] + sizeof(unsigned int);

		// Read the size of the log entry
		unsigned int size;
		SetFilePointer(logFile, filePosition, 0, FILE_BEGIN);
		ReadFile(logFile, &size, sizeof(unsigned int), &numRead, NULL);

		// Read the log entry itself
		unsigned char *temp = new unsigned char [size];
		SetFilePointer(logFile, filePosition, 0, FILE_BEGIN);
		ReadFile(logFile, temp, size, &numRead, NULL);

		if (logFileMutex != NULL)
			logFileMutex->Release();

		// Parse the log entry data
		LogItem *item = new LogItem(CurrentHolodeckVersion);
		item->ConvertFromByteSequence(temp);
		delete[] temp;
		return item;
	}

	//*************************************************************************
	// Method:		GetLogEntryAtFilePosition
	// Description: Get a given log entry from the aut at a specified location
	//	in the file
	//
	// Parameters:
	//	filePos - The file position of the log entry to get
	//
	// Return Value: The log entry at the location given
	//*************************************************************************
	LogItem *LogReader::GetLogEntryAtFilePosition(unsigned long filePos)
	{
		if (logFileMutex != NULL)
			logFileMutex->Acquire(INFINITE);

		SetFilePointer(logFile, filePos, 0, FILE_BEGIN);

		unsigned int totalSize = 0;
		unsigned long numRead = 0;
		ReadFile(logFile, &totalSize, sizeof(unsigned int), &numRead, NULL);

		if ((numRead == 0) || (totalSize == 0))
			return NULL;

		unsigned char *buffer = new unsigned char[totalSize];
		memcpy(buffer, &totalSize, sizeof(unsigned int));

		unsigned long totalNumRead = sizeof(unsigned int);
		unsigned long totalRemaining = totalSize - sizeof(unsigned int);
		while (totalNumRead != totalSize)
		{
			ReadFile(logFile, buffer + totalNumRead, sizeof(unsigned char) * totalRemaining, &numRead, NULL);
			totalNumRead += numRead;
			totalRemaining -= numRead;
		}

		if (logFileMutex != NULL)
			logFileMutex->Release();
		
		LogItem *item = new LogItem(CurrentHolodeckVersion);
		if (item)
			item->ConvertFromByteSequence(buffer);

		delete[] buffer;
		return item;
	}

	//*************************************************************************
	// Method:		NewLogsReceived
	// Description: called when a log is received from the aut.
	//
	// Parameters:
	//	numNewEntries - the number of log entries received
	//
	// Return Value: None
	//*************************************************************************
	void LogReader::NewLogsReceived(ReceivedLogInfo *info)
	{
		if (info == NULL || logInfoList == NULL || LogsCache == NULL)
			return;

		try
		{
			unsigned int threadID;

			//don't add to list if this is just a timeout with no data.
			info->StartIndex = this->totalLogCount;
			if (info->LogEntryCount > 0)
				logInfoList->Add(info);
			this->totalLogCount += info->LogEntryCount;

			if (logEventMutex != NULL)
				logEventMutex->Acquire();

			if ( (OnLogGenerated != NULL) && (logFile != NULL) )
			{
				//bugbug - should we even be sending this?  You can get from logitem.
				threadID = 0;

				if (info->LogEntryCount == 0)
				{
					OnLogGenerated->Invoke(info->LogEntryCount, NULL, processID, threadID);
					if (logEventMutex != NULL)
						logEventMutex->Release();
					return;
				}

				int numCachedLogs = LogsCache->Count;
				LogsCache->Add(info);

				for (int infoNum = numCachedLogs; infoNum >= 0; infoNum--)
				{
					ReceivedLogInfo *logInfo = dynamic_cast <ReceivedLogInfo *> (LogsCache->get_Item(infoNum));
					if (logInfo == NULL)
						continue;

					// Read the new log entries if there were any
					LogItem* logItems __gc[] = NULL;
					if (logInfo->LogEntryCount > 0)
					{
						if (logFileMutex != NULL)
							logFileMutex->Acquire(INFINITE);

						// seek to the log packet and read the size
						unsigned long filePos = logInfo->FilePosition;
						SetFilePointer(logFile, filePos, 0, FILE_BEGIN);
						unsigned int totalSize = 0;
						unsigned long numRead = 0;
						ReadFile(logFile, &totalSize, sizeof(unsigned int), &numRead, NULL);

						// read in the data
						unsigned char *buffer = new unsigned char[totalSize];
						if (buffer == NULL)
							continue;

						unsigned long totalNumRead = 0;
						unsigned long totalRemaining = totalSize;
						while (totalNumRead != totalSize)
						{
							ReadFile(logFile, buffer + totalNumRead, sizeof(unsigned char) * totalRemaining, &numRead, NULL);
							totalNumRead += numRead;
							totalRemaining -= numRead;
						}

						if (logFileMutex != NULL)
							logFileMutex->Release();

						// log entries start past the entry count and entry offsets
						unsigned long offset = sizeof(unsigned int) + (logInfo->LogEntryCount * sizeof(unsigned int));

						// Create a new array to contain the log items
						logItems = new LogItem* __gc[logInfo->LogEntryCount];
						if (logItems != NULL)
						{
							for (int i=0; i<logInfo->LogEntryCount; i++)
							{
								// Create the LogItem structure
								logItems[i] = new LogItem(info->Version);
								if (logItems[i] != NULL)
								{
									logItems[i]->ConvertFromByteSequence(buffer + offset);
									logItems[i]->FilePosition = logInfo->FilePosition + offset + sizeof(unsigned int);
								}
								// First unsigned int is the size of the log entry.  Adjust offset to next entry using this.
								offset += *((unsigned int*)(buffer + offset));
							}
						}
						delete[] buffer;
					}

					OnLogGenerated->Invoke(logInfo->LogEntryCount, logItems, processID, threadID);
				}
				LogsCache->Clear();
			}
			else
			{
				LogsCache->Add(info);
			}
		}
		catch (...)
		{
			//clear the logs cache, since some bad data from this cache might have been causing the exception
			if (LogsCache)
				LogsCache->Clear();
		}
		
		if (logEventMutex != NULL)
			logEventMutex->Release();
	}

	//*************************************************************************
	// Method:		CreateLogFile
	// Description: Creates the log file to log to
	//
	// Parameters:
	//	fileName - the name of the log file
	//	pidStr - a string containing the current process id
	//
	// Return Value: None
	//*************************************************************************
	void LogReader::CreateLogFile(String *fileName, String *pidStr)
	{
		PSECURITY_DESCRIPTOR pSD;
		SECURITY_ATTRIBUTES sa;

		pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
		// Initialize a security attributes structure.
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = FALSE;

		const __wchar_t __pin *fullLogFileName = PtrToStringChars(fileName);
		logFile = CreateFileW(fullLogFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, &sa);
		if (logFile == INVALID_HANDLE_VALUE)
			return;

		String *mutexName = String::Concat(LOG_FILE_MUTEX_PREFIX, "\\", pidStr);
		logFileMutex = new SiUtils::Mutex();
		logFileMutex->Create();
	}

	//*************************************************************************
	// Method:		ExportToDiffFile
	// Description: Exports all log entries to a diff file in plain text format
	//
	// Parameters:
	//	diffFileName - the filename to store the logs in
	//	filePositionsToLog - A list of file positions to export.  If NULL the
	//		entire log file will be exported
	//
	// Return Value: None
	//*************************************************************************
	void LogReader::ExportToDiffFile(String *diffFileName, ArrayList *filePositionsToLog)
	{
		switch(version)
		{
		case HolodeckVersion::V2_0:
		case HolodeckVersion::V2_1:
			ExportToDiffFileV2_1(diffFileName, filePositionsToLog);
			break;
		case HolodeckVersion::V2_5:
			ExportToDiffFileV2_5(diffFileName, filePositionsToLog);
			break;
		}
	}
	
	//*************************************************************************
	// Method:		OpenLogFile
	// Description: sets the file name to use and re-opens the file
	//
	// Parameters:
	//	numNewEntries - the number of log entries received
	//
	// Return Value: true on success, false on failure
	//*************************************************************************
	bool LogReader::OpenLogFile(String *newLogFileName)
	{
		switch(version)
		{
		case HolodeckVersion::V2_0:
		case HolodeckVersion::V2_1:
			return OpenLogFileV2_1(newLogFileName);
		case HolodeckVersion::V2_5:
			return OpenLogFileV2_5(newLogFileName);
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		ExportToDiffFileV2_1
	// Description: Exports all log entries to a diff file in plain text format
	//
	// Parameters:
	//	diffFileName - the filename to store the logs in
	//	filePositionsToLog - A list of file positions to export.  If NULL the
	//		entire log file will be exported
	//
	// Return Value: None
	//*************************************************************************
	void LogReader::ExportToDiffFileV2_1(String *diffFileName, ArrayList *filePositionsToLog)
	{
		StreamWriter *sw = NULL;

		try
		{
			sw = new StreamWriter(diffFileName, false);
			if (!sw)
				return;

			//write column headers
			sw->Write("TimeStamp");
			sw->Write(",");
			sw->Write("ThreadID");
			sw->Write(",");
			sw->Write("Category");
			sw->Write(",");
			sw->Write("DllName");
			sw->Write(",");
			sw->Write("Function");
			sw->Write(",");
			sw->Write("ReturnValue");
			sw->Write(",");
			sw->Write("ErrorCode");
			sw->Write(",");
			sw->Write("Exception");
			sw->Write(",");
			sw->Write("Parameter 1");
			sw->Write(",");
			sw->Write("Parameter 2");
			sw->Write(",");
			sw->Write("Parameter 3");
			sw->Write(",");
			sw->Write("Parameter 4");
			sw->Write(",");
			sw->Write("Parameter 5");
			sw->Write(",");
			sw->Write("Parameter 6");
			sw->Write(",");
			sw->Write("Parameter 7");
			sw->Write(",");
			sw->Write("Parameter 8");
			sw->Write(",");
			sw->Write("Parameter 9");
			sw->Write(",");
			sw->Write("Parameter 10");
			sw->Write(",");
			sw->Write("Parameter 11");
			sw->Write(",");
			sw->Write("Parameter 12");

			sw->Write("\r\n");

			unsigned long count = 0;
			if (filePositionsToLog)
				count = filePositionsToLog->Count;
			else
				count = totalLogCount;

			for (unsigned long i = 0; i < count; i++)
			{
				LogItem *item = NULL;
				if (filePositionsToLog)
				{
					unsigned long filePos = *dynamic_cast<__box unsigned long *>(filePositionsToLog->get_Item(i));
					if (filePos == 0)
						item = NULL;
					else
						item = this->GetLogEntryAtFilePosition(filePos);
				}
				else 
				{
					unsigned long filePos;
					item = this->GetLogEntryAt(i, filePos);
				}

				if (!item)
					continue;

				sw->Write("\"");
				sw->Write(item->TimeStamp);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ThreadID);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->Category);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->DllName);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->Function);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ReturnValue);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ErrorCode);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->Exception);
				sw->Write("\"");
				sw->Write(",");
				for (int i = 0; (i < item->Parameters->Count) && (i < MAX_PARAMETER_COLUMNS); i++)
				{
					sw->Write("\"");
					sw->Write(item->Parameters->get_Item(i));
					sw->Write("\"");
					if (i < item->Parameters->Count - 1)
						sw->Write(",");
				}
				sw->Write("\r\n");

			}
			sw->Close();
		}
		catch (Exception *)
		{
			if (sw)
				sw->Close();

			MessageBoxA(NULL, "There was a problem exporting the API log to the specified location. Please verify that the specified location is not read-only and that you have adequate permissions to write to it.", "Holodeck", MB_OK | MB_ICONERROR);
			return;
		}
	}

	//*************************************************************************
	// Method:		ExportToDiffFileV2_5
	// Description: Exports all log entries to a diff file in plain text format
	//
	// Parameters:
	//	diffFileName - the filename to store the logs in
	//	filePositionsToLog - A list of file positions to export.  If NULL the
	//		entire log file will be exported
	//
	// Return Value: None
	//*************************************************************************
	void LogReader::ExportToDiffFileV2_5(String *diffFileName, ArrayList *filePositionsToLog)
	{
		StreamWriter *sw = NULL;

		try
		{
			sw = new StreamWriter(diffFileName, false);
			if (!sw)
				return;

			//write column headers
			sw->Write("TimeStamp");
			sw->Write(",");
			sw->Write("CallDepth");
			sw->Write(",");
			sw->Write("ThreadID");
			sw->Write(",");
			sw->Write("Category");
			sw->Write(",");
			sw->Write("DllName");
			sw->Write(",");
			sw->Write("Function");
			sw->Write(",");
			sw->Write("ReturnValue");
			sw->Write(",");
			sw->Write("ErrorCode");
			sw->Write(",");
			sw->Write("Exception");
			sw->Write(",");
			sw->Write("Parameter 1");
			sw->Write(",");
			sw->Write("Parameter 2");
			sw->Write(",");
			sw->Write("Parameter 3");
			sw->Write(",");
			sw->Write("Parameter 4");
			sw->Write(",");
			sw->Write("Parameter 5");
			sw->Write(",");
			sw->Write("Parameter 6");
			sw->Write(",");
			sw->Write("Parameter 7");
			sw->Write(",");
			sw->Write("Parameter 8");
			sw->Write(",");
			sw->Write("Parameter 9");
			sw->Write(",");
			sw->Write("Parameter 10");
			sw->Write(",");
			sw->Write("Parameter 11");
			sw->Write(",");
			sw->Write("Parameter 12");

			sw->Write("\r\n");

			unsigned long count = 0;
			if (filePositionsToLog)
				count = filePositionsToLog->Count;
			else
				count = totalLogCount;

			for (unsigned long i = 0; i < count; i++)
			{
				LogItem *item = NULL;
				if (filePositionsToLog)
				{
					unsigned long filePos = *dynamic_cast<__box unsigned long *>(filePositionsToLog->get_Item(i));
					if (filePos == 0)
						item = NULL;
					else
						item = this->GetLogEntryAtFilePosition(filePos);
				}
				else
				{
					unsigned long filePos;
					item = this->GetLogEntryAt(i, filePos);
				}

				if (!item)
					continue;

				sw->Write("\"");
				sw->Write(item->TimeStamp);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->CallDepth);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ThreadID);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->Category);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->DllName);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->Function);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ReturnValue);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ErrorCode);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->Exception);
				sw->Write("\"");
				sw->Write(",");
				for (int i = 0; (i < item->Parameters->Count) && (i < MAX_PARAMETER_COLUMNS); i++)
				{
					sw->Write("\"");
					sw->Write(item->Parameters->get_Item(i));
					sw->Write("\"");
					if (i < item->Parameters->Count - 1)
						sw->Write(",");
				}
				sw->Write("\r\n");
			}
			sw->Close();
		}
		catch (Exception *)
		{
			if (sw)
				sw->Close();

			MessageBoxA(NULL, "There was a problem exporting the API log to the specified location. Please verify that the specified location is not read-only and that you have adequate permissions to write to it.", "Holodeck", MB_OK | MB_ICONERROR);
			return;
		}
	}

	//*************************************************************************
	// Method:		OpenLogFileV2_1
	// Description: sets the file name to use and re-opens the file
	//
	// Parameters:
	//	numNewEntries - the number of log entries received
	//
	// Return Value: true on success, false on failure
	//*************************************************************************
	bool LogReader::OpenLogFileV2_1(String *newLogFileName)
	{
		CloseHandle(logFile);
		logFile = NULL;

		logFileName = newLogFileName;
		const __wchar_t __pin *fullLogFileName = PtrToStringChars(logFileName);
		logFile = CreateFileW(fullLogFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

		if (logFile == INVALID_HANDLE_VALUE)
			return false;

		logInfoList->Clear();
		
		// set all the receivedloginfo structures correctly
		DWORD numRead = 0;

		// skip the process id and app path
		int procID;
		unsigned long appPathLength;
		ReadFile(logFile, &procID, sizeof(int), &numRead, NULL);
		ReadFile(logFile, &appPathLength, sizeof(unsigned long), &numRead, NULL);
		SetFilePointer(logFile, appPathLength, 0, FILE_CURRENT);

		while (numRead > 0)
		{
			// get the current position in the file to send to the receiver
			DWORD currPos = SetFilePointer(logFile, 0, 0, FILE_CURRENT);

			// get the first size
			unsigned int totalSize = 0;
			ReadFile(logFile, &totalSize, sizeof(unsigned int), &numRead, NULL);
			if (numRead > 0)
			{
				unsigned char *buffer = new unsigned char[totalSize];
				if (!buffer)
				{
					//totalSize is garbage or out of memory
					return false;
				}

				// get the number of entries, we only read in the whole buffer to advance the
				// file pointer
				unsigned int numEntries = 0;
				unsigned long totalNumRead = 0;
				unsigned long totalRemaining = totalSize;
				while (totalNumRead != totalSize)
				{
					ReadFile(logFile, buffer, sizeof(unsigned char) * totalRemaining, &numRead, NULL);
					totalNumRead += numRead;
					totalRemaining -= numRead;
				}
				memcpy(&numEntries, buffer, sizeof(unsigned int));
				delete[] buffer;

				ReceivedLogInfo *info = new ReceivedLogInfo();
				info->FilePosition = currPos;
				info->LogEntryCount = numEntries;
				info->Version = version;
				NewLogsReceived(info);
				delete info;
			}
		}

		return true;
	}

	//*************************************************************************
	// Method:		OpenLogFileV2_5
	// Description: sets the file name to use and re-opens the file
	//
	// Parameters:
	//	numNewEntries - the number of log entries received
	//
	// Return Value: true on success, false on failure
	//*************************************************************************
	bool LogReader::OpenLogFileV2_5(String *newLogFileName)
	{
		CloseHandle(logFile);
		logFile = NULL;

		logFileName = newLogFileName;
		const __wchar_t __pin *fullLogFileName = PtrToStringChars(logFileName);
		logFile = CreateFileW(fullLogFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

		if (logFile == INVALID_HANDLE_VALUE)
			return false;

		logInfoList->Clear();
		
		// set all the receivedloginfo structures correctly
		DWORD numRead = 0;

		// skip the version info
		unsigned long versionSize;
		ReadFile(logFile, &versionSize, sizeof(unsigned long), &numRead, NULL);
		SetFilePointer(logFile, versionSize, 0, FILE_CURRENT);

		// skip the process id and app path
		int procID;
		unsigned long appPathLength;
		ReadFile(logFile, &procID, sizeof(int), &numRead, NULL);
		ReadFile(logFile, &appPathLength, sizeof(unsigned long), &numRead, NULL);
		SetFilePointer(logFile, appPathLength, 0, FILE_CURRENT);

		while (numRead > 0)
		{
			// get the current position in the file to send to the receiver
			DWORD currPos = SetFilePointer(logFile, 0, 0, FILE_CURRENT);

			// get the first size
			unsigned int totalSize = 0;
			ReadFile(logFile, &totalSize, sizeof(unsigned int), &numRead, NULL);
			if (numRead > 0)
			{
				unsigned char *buffer = new unsigned char[totalSize];
				if (!buffer)
				{
					//totalSize is garbage or out of memory
					return false;
				}

				// get the number of entries, we only read in the whole buffer to advance the
				// file pointer
				unsigned int numEntries = 0;
				unsigned long totalNumRead = 0;
				unsigned long totalRemaining = totalSize;
				while (totalNumRead != totalSize)
				{
					ReadFile(logFile, buffer, sizeof(unsigned char) * totalRemaining, &numRead, NULL);
					totalNumRead += numRead;
					totalRemaining -= numRead;
				}
				memcpy(&numEntries, buffer, sizeof(unsigned int));
				delete[] buffer;

				ReceivedLogInfo *info = new ReceivedLogInfo();
				info->FilePosition = currPos;
				info->LogEntryCount = numEntries;
				info->Version = version;
				NewLogsReceived(info);
				delete info;
			}

			currPos = SetFilePointer(logFile, totalSize + currPos + sizeof(unsigned int), 0, FILE_BEGIN);
		}

		return true;
	}

	//*************************************************************************
	// Method:		getFormattedAddressString
	// Description: Forms a string with spaces
	//
	// Parameters:
	//	str - the string
	//	minLength - the minimum length of the string
	//
	// Return Value: formatted string
	//*************************************************************************
	String * LogReader::getFormattedString(String *str, int minLength)
	{
		String * returnStr = str->Trim();
		if (returnStr->Length < minLength)
		{
			int diff = minLength - returnStr->Length;
			for (int add=0; add < diff; add++)
				returnStr = String::Concat (returnStr, S" ");
		}
		
		return returnStr;
	}
}