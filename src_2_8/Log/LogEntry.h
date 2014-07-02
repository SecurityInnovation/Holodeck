//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEntry.h
//
// DESCRIPTION: Contains definition for the log entry class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <stdio.h>
#include "LogEvent.h"

namespace Log
{
	//*************************************************************************
	// class to hold a log entry
	//*************************************************************************
	class LogEntry
	{
	private:
		// static members
		static HANDLE heap;

		// non string members
		unsigned int logEntrySize;
		unsigned int callDepth;			// Holodeck 2.5 and later
		unsigned int numParameters;
		unsigned int numEvents;			// Holodeck 2.5 and later
		SYSTEMTIME timeStamp;
		unsigned int timeStampLength;

		// string lengths
		unsigned int categoryLength;
		unsigned int threadIDLength;
		unsigned int dllNameLength;
		unsigned int functionLength;
		unsigned int returnValueLength;
		unsigned int errorCodeLength;
		unsigned int exceptionLength;
		unsigned int *parameterLengths;
		unsigned int *eventLengths;

		// strings
		char *category;
		char *threadID;
		char *dllName;
		char *function;
		char *returnValue;
		char *errorCode;
		char *exception;
		char **parameters;
		char **events;

		void Cleanup();
		void RecomputeLogEntrySize();
	public:
		LogEntry();
		~LogEntry();

		void Initialize();

		void ConvertToByteSequence(unsigned char *buffer, unsigned int &bufferSize);

		unsigned int GetLogSize() 
		{ 
			if (logEntrySize == 0)
				RecomputeLogEntrySize();
			return logEntrySize; 
		}
		unsigned int GetParameterCount() { return numParameters; }
		unsigned int GetEventCount() { return numEvents; }

		unsigned int GetTheadIDLength() { return this->threadIDLength; }
		unsigned int GetCategoryLength() { return this->categoryLength; }
		unsigned int GetDllNameLength() { return this->dllNameLength; }
		unsigned int GetFunctionLength() { return this->functionLength; }
		unsigned int GetReturnValueLength() { return this->returnValueLength; }
		unsigned int GetErrorCodeLength() { return this->errorCodeLength; }
		unsigned int GetExceptionLength() { return this->exceptionLength; }

		unsigned int GetCallDepth() { return this->callDepth; }
		void SetCallDepth(unsigned int value) { this->callDepth = value; }

		unsigned int GetParameterLength(unsigned int parameterIndex);
		unsigned int GetEventLength(unsigned int eventIndex);

		void SetTimeStamp(SYSTEMTIME ts);
		SYSTEMTIME GetTimeStamp() { return this->timeStamp; }

		void SetThreadID(char *tID);
		void GetThreadID(char *tID, unsigned int &tIDLength);

		void SetCategory(char *cat);
		void GetCategory(char *cat, unsigned int &catLength);

		void SetDllName(char *dll);
		void GetDllName(char *dll, unsigned int &dllLength);

		void SetFunction(char *func);
		void GetFunction(char *func, unsigned int &funcLength);

		void SetReturnValue(char *rv);
		void GetReturnValue(char *rv, unsigned int &rvLength);

		void SetErrorCode(char *ec);
		void GetErrorCode(char *ec, unsigned int &ecLength);

		void SetException(char *except);
		void GetException(char *except, unsigned int &exceptLength);

		void SetParameter(unsigned int parameterIndex, char *param, unsigned int paramLength);
		void GetParameter(unsigned int parameterIndex, char *param, unsigned int &paramLength);

		void SetEvent(unsigned int eventIndex, const LogEvent& event);
		void GetEvent(unsigned int eventIndex, LogEvent& event);
	};
}