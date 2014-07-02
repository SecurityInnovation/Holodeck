//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReaderV2_5.cpp
//
// DESCRIPTION: Contains implementation for the class LogReaderV2_5
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogReaderV2_5.h"

#using <mscorlib.dll>
using namespace System::IO;

namespace HoloScript
{
	
	//int MAX_PARAMETER_COLUMNS = 12;

	//*************************************************************************
	// Method:		LogReaderV2_5
	// Description: Constructor for the LogReaderV2_5 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReaderV2_5::LogReaderV2_5()
	{
		//version = HolodeckVersion::V2_5;
	}

	//*************************************************************************
	// Method:		~LogReaderV2_5
	// Description: Destructor for the LogReaderV2_1 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogReaderV2_5::~LogReaderV2_5()
	{
	}

	//*************************************************************************
	// Method:		OpenLogFile
	// Description: sets the file name to use and re-opens the file
	//
	// Parameters:
	//	numNewEntries - the number of log entries received
	//
	// Return Value: None
	//*************************************************************************
	void LogReaderV2_5::OpenLogFile(String *newLogFileName, String *exportLogFileName)
	{
		//CloseHandle(logFile);

		// Open the new file to write...
		StreamWriter *sw = new StreamWriter(exportLogFileName);
		if (!sw)
			return;

		//write column headers
		sw->Write("TimeStamp");
		sw->Write("\t");
		sw->Write("CallDepth");
		sw->Write("\t");
		sw->Write("ThreadID");
		sw->Write("\t");
		sw->Write("Category");
		sw->Write("\t");
		sw->Write("DllName");
		sw->Write("\t");
		sw->Write("Function");
		sw->Write("\t");
		sw->Write("ReturnValue");
		sw->Write("\t");
		sw->Write("ErrorCode");
		sw->Write("\t");
		sw->Write("Exception");
	
		// Write parameter name column headers...
		for (int i = 0; i < MAX_PARAMETER_COLUMNS; i++)
		{
			sw->Write("\t");
			sw->Write("Parameter ");
			sw->Write(Convert::ToString(i));
		}

		sw->Write("\n");

		logFileName = newLogFileName;
		const __wchar_t __pin *fullLogFileName = PtrToStringChars(logFileName);
		logFile = CreateFileW(fullLogFileName, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
			OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_RANDOM_ACCESS, NULL);

		this->logFile = logFile;

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
				

				ReceivedLogInfo *info = new ReceivedLogInfo();
				info->FilePosition = currPos;
				info->LogEntryCount = numEntries;
				
				unsigned long offset = sizeof(unsigned int) + (info->LogEntryCount * sizeof(unsigned int));

				// Convert the logitem from the  byte sequence...
				LogItem *item = new LogItem( );
				item->ConvertFromByteSequence( buffer + offset );

				//LogItem *item = this->GetLogEntryAtFilePosition( info->FilePosition );

				if (!item)
					continue;

				sw->Write(item->TimeStamp);
				sw->Write("\t");
				sw->Write(item->CallDepth);
				sw->Write("\t");
				sw->Write(item->ThreadID);
				sw->Write("\t");
				sw->Write(item->Category);
				sw->Write("\t");
				sw->Write(item->DllName);
				sw->Write("\t");
				sw->Write(item->Function);
				sw->Write("\t");
				sw->Write(item->ReturnValue);
				sw->Write("\t");
				sw->Write(item->ErrorCode);
				sw->Write("\t");
				sw->Write(item->Exception);
			
				for (int i = 0; (i < item->Parameters->Count) && (i < MAX_PARAMETER_COLUMNS); i++)
				{
					sw->Write("\t");
					sw->Write(item->Parameters->get_Item(i));
				}

				sw->Write("\n");

				//NewLogsReceived(info);
				delete info;
				delete[] buffer;
			}
		}

		// Close the exported file to flush.
		sw->Close( );
	}
}