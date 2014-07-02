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
#define LogDll_File

#include "LogEntry.h"
#include "defines.h"

namespace Log
{
	HANDLE LogEntry::heap = NULL;

	//*************************************************************************
	// Method:		LogEntry
	// Description: Constructor for the LogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEntry::LogEntry()
	{
		Initialize();
	}

	//*************************************************************************
	// Method:		~LogEntry
	// Description: Destructor for the LogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEntry::~LogEntry()
	{
		Cleanup();
	}

	//*************************************************************************
	// Method:		Initialize
	// Description: Initializes memory for the LogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::Initialize()
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		timeStampLength = sizeof(SYSTEMTIME);

		logEntrySize = 0;
		callDepth = 0;
		numParameters = 0;
		numEvents = 0;
		threadIDLength = 0;
		categoryLength = 0;
		dllNameLength = 0;
		functionLength = 0;
		returnValueLength = 0;
		errorCodeLength = 0;
		exceptionLength = 0;
		parameterLengths = NULL;
		eventLengths = NULL;
		threadID = NULL;
		category = NULL;
		dllName = NULL;
		function = NULL;
		returnValue = NULL;
		errorCode = NULL;
		exception = NULL;
		parameters = NULL;
		events = NULL;
	}

	//*************************************************************************
	// Method:		Cleanup
	// Description: Cleans up memory for the LogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::Cleanup()
	{
		if (parameterLengths != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, parameterLengths);

		if (eventLengths != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, eventLengths);

		if (threadID != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, threadID);

		if (category != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, category);

		if (dllName != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, dllName);

		if (function != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, function);

		if (returnValue != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, returnValue);

		if (errorCode != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, errorCode);

		if (exception != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, exception);

		if (parameters != NULL)
		{
			for (unsigned int i = 0; i < numParameters; i++)
			{
				if (parameters[i] != NULL)
					HeapFree(heap, HEAP_ZERO_MEMORY, parameters[i]);
			}
			HeapFree(heap, HEAP_ZERO_MEMORY, parameters);
		}

		if (events != NULL)
		{
			for (unsigned int i = 0; i < numEvents; i++)
			{
				if (events[i] != NULL)
					HeapFree(heap, HEAP_ZERO_MEMORY, events[i]);
			}
			HeapFree(heap, HEAP_ZERO_MEMORY, events);
		}
	}

	//*************************************************************************
	// Method:		RecomputeLogEntrySize
	// Description: Sets the logEntrySize member to the correct value
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::RecomputeLogEntrySize()
	{
		logEntrySize = 0;
		logEntrySize += (sizeof(unsigned int));	// for call depth
		logEntrySize += (sizeof(unsigned int) * 11); // for all unsigned int members
		logEntrySize += (sizeof(unsigned int) * numParameters); // for the parameterLengths member
		logEntrySize += (sizeof(unsigned int) * numEvents); // for the eventLengths member
		logEntrySize += timeStampLength; // for the timeStamp member
		logEntrySize += (sizeof(char) * threadIDLength); // for the threadID member
		logEntrySize += (sizeof(char) * categoryLength); // for the category member
		logEntrySize += (sizeof(char) * dllNameLength); // for the dllName member
		logEntrySize += (sizeof(char) * functionLength); // for the function member
		logEntrySize += (sizeof(char) * returnValueLength); // for the returnValue member
		logEntrySize += (sizeof(char) * errorCodeLength); // for the errorCode member
		logEntrySize += (sizeof(char) * exceptionLength); // for the exception member

		// for each of the parameter strings
		for (unsigned int i = 0; i < numParameters; i++)
			logEntrySize += (sizeof(char) * parameterLengths[i]);

		// for each of the event strings
		for (unsigned int i = 0; i < numEvents; i++)
			logEntrySize += (sizeof(char) * eventLengths[i]);
	}

	//*************************************************************************
	// Method:		GetParameterLength
	// Description: Gets the string length of a particular parameter
	//
	// Parameters:
	//	parameterIndex - the zero-based index of the parameter to get the length for
	//
	// Return Value: the length of the parameter, 0 if unsuccessful
	//*************************************************************************
	unsigned int LogEntry::GetParameterLength(unsigned int parameterIndex)
	{
		if ((parameterIndex < 0) || (parameterIndex >= numParameters))
			return 0;

		return parameterLengths[parameterIndex];
	}

	//*************************************************************************
	// Method:		GetEventLength
	// Description: Gets the string length of a particular event
	//
	// Parameters:
	//	parameterIndex - the zero-based index of the event to get the length for
	//
	// Return Value: the length of the event, 0 if unsuccessful
	//*************************************************************************
	unsigned int LogEntry::GetEventLength(unsigned int eventIndex)
	{
		if ((eventIndex < 0) || (eventIndex >= numEvents))
			return 0;

		return eventLengths[eventIndex];
	}

	//*************************************************************************
	// Method:		SetTimeStamp
	// Description: Sets the time stamp for the log entry
	//
	// Parameters:
	//	ts - the SYSTEMTIME representing the timestamp to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetTimeStamp(SYSTEMTIME ts)
	{
		CopyMemory(&timeStamp, &ts, timeStampLength);
	}

	//*************************************************************************
	// Method:		SetThreadID
	// Description: Sets the threadID for the log entry
	//
	// Parameters:
	//	tID - the string representing the thread ID to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetThreadID(char *tID)
	{
		threadIDLength = (unsigned int)strlen(tID);
		if (threadID == NULL)
			threadID = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (threadIDLength + 1));
		else
			threadID = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, threadID, sizeof(char) * (threadIDLength + 1));

		CopyMemory(threadID, tID, threadIDLength);
		threadID[threadIDLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetThreadID
	// Description: Gets the thread id for the log entry
	//
	// Parameters:
	//	tID - the buffer to put the thread-id into.  Must be allocated by caller.
	//	tIDLength - on entering the function, must be the maximum number of chars
	//		the tID buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into tID.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetThreadID(char *tID, unsigned int &tIDLength)
	{
		if ((tID == NULL) || (tIDLength < (threadIDLength + 1)))
		{
			tIDLength = 0;
			return;
		}

		tIDLength = threadIDLength + 1;
		CopyMemory(tID, threadID, threadIDLength);
		tID[threadIDLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetCategory
	// Description: Sets the category for the log entry
	//
	// Parameters:
	//	cat - the string representing the category to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetCategory(char *cat)
	{
		categoryLength = (unsigned int)strlen(cat);
		if (category == NULL)
			category = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (categoryLength + 1));
		else
			category = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, category, sizeof(char) * (categoryLength + 1));

		CopyMemory(category, cat, categoryLength);
		category[categoryLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetCategory
	// Description: Gets the category for the log entry
	//
	// Parameters:
	//	cat - the buffer to put the category into.  Must be allocated by caller.
	//	catLength - on entering the function, must be the maximum number of chars
	//		the cat buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into cat.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetCategory(char *cat, unsigned int &catLength)
	{
		if ((cat == NULL) || (catLength < (categoryLength + 1)))
		{
			catLength = 0;
			return;
		}

		catLength = categoryLength + 1;
		CopyMemory(cat, category, categoryLength);
		cat[categoryLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetDllName
	// Description: Sets the dll name for the log entry
	//
	// Parameters:
	//	dll - the string representing the dll to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetDllName(char *dll)
	{
		dllNameLength = (unsigned int)strlen(dll);
		if (dllName == NULL)
			dllName = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (dllNameLength + 1));
		else
			dllName = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, dllName, sizeof(char) * (dllNameLength + 1));

		CopyMemory(dllName, dll, dllNameLength);
		dllName[dllNameLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetDllName
	// Description: Gets the dll name for the log entry
	//
	// Parameters:
	//	dll - the buffer to put the dll name into.  Must be allocated by caller.
	//	dllLength - on entering the function, must be the maximum number of chars
	//		the dll buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into dll.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetDllName(char *dll, unsigned int &dllLength)
	{
		if ((dll == NULL) || (dllLength < (dllNameLength + 1)))
		{
			dllLength = 0;
			return;
		}

		dllLength = dllNameLength + 1;
		CopyMemory(dll, dllName, dllNameLength);
		dll[dllNameLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetFunction
	// Description: Sets the function for the log entry
	//
	// Parameters:
	//	func - the string representing the function to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetFunction(char *func)
	{
		functionLength = (unsigned int)strlen(func);
		if (function == NULL)
			function = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (functionLength + 1));
		else
			function = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, function, sizeof(char) * (functionLength + 1));

		CopyMemory(function, func, functionLength);
		function[functionLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetFunction
	// Description: Gets the function for the log entry
	//
	// Parameters:
	//	func - the buffer to put the function into.  Must be allocated by caller.
	//	funcLength - on entering the function, must be the maximum number of chars
	//		the func buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into func.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetFunction(char *func, unsigned int &funcLength)
	{
		if ((func == NULL) || (funcLength < (functionLength + 1)))
		{
			funcLength = 0;
			return;
		}

		funcLength = functionLength + 1;
		CopyMemory(func, function, functionLength);
		func[functionLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetReturnValue
	// Description: Sets the return value for the log entry
	//
	// Parameters:
	//	rv - the string representing the return value to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetReturnValue(char *rv)
	{
		returnValueLength = (unsigned int)strlen(rv);
		if (returnValue == NULL)
			returnValue = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (returnValueLength + 1));
		else
			returnValue = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, returnValue, sizeof(char) * (returnValueLength + 1));

		CopyMemory(returnValue, rv, returnValueLength);
		returnValue[returnValueLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetReturnValue
	// Description: Gets the time stamp for the log entry
	//
	// Parameters:
	//	rv - the buffer to put the return value into.  Must be allocated by caller.
	//	rvLength - on entering the function, must be the maximum number of chars
	//		the rv buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into rv.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetReturnValue(char *rv, unsigned int &rvLength)
	{
		if ((rv == NULL) || (rvLength < (returnValueLength + 1)))
		{
			rvLength = 0;
			return;
		}

		rvLength = returnValueLength + 1;
		CopyMemory(rv, returnValue, returnValueLength);
		rv[returnValueLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetErrorCode
	// Description: Sets the error code for the log entry
	//
	// Parameters:
	//	ec - the string representing the error code to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetErrorCode(char *ec)
	{
		errorCodeLength = (unsigned int)strlen(ec);
		if (errorCode == NULL)
			errorCode = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (errorCodeLength + 1));
		else
			errorCode = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, errorCode, sizeof(char) * (errorCodeLength + 1));

		CopyMemory(errorCode, ec, errorCodeLength);
		errorCode[errorCodeLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetErrorCode
	// Description: Gets the error code for the log entry
	//
	// Parameters:
	//	ec - the buffer to put the error code into.  Must be allocated by caller.
	//	ecLength - on entering the function, must be the maximum number of chars
	//		the ec buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into ec.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetErrorCode(char *ec, unsigned int &ecLength)
	{
		if ((ec == NULL) || (ecLength < (errorCodeLength + 1)))
		{
			ecLength = 0;
			return;
		}

		ecLength = errorCodeLength + 1;
		CopyMemory(ec, errorCode, errorCodeLength);
		ec[errorCodeLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetException
	// Description: Sets the exception for the log entry
	//
	// Parameters:
	//	except - the string representing the exception to set for the log entry
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetException(char *except)
	{
		exceptionLength = (unsigned int)strlen(except);
		if (exception == NULL)
			exception = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (exceptionLength + 1));
		else
			exception = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, exception, sizeof(char) * (exceptionLength + 1));

		CopyMemory(exception, except, exceptionLength);
		exception[exceptionLength] = '\0';
	}

	//*************************************************************************
	// Method:		GetException
	// Description: Gets the exception for the log entry
	//
	// Parameters:
	//	except - the buffer to put the exception into.  Must be allocated by caller.
	//	exceptLength - on entering the function, must be the maximum number of chars
	//		the except buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into except.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetException(char *except, unsigned int &exceptLength)
	{
		if ((except == NULL) || (exceptLength < (exceptionLength + 1)))
		{
			exceptLength = 0;
			return;
		}

		exceptLength = exceptionLength + 1;
		CopyMemory(except, exception, exceptionLength);
		except[exceptionLength] = '\0';
	}

	//*************************************************************************
	// Method:		SetParameter
	// Description: Sets a parameter for the log entry
	//
	// Parameters:
	//	parameterIndex - the index of the parameter to set
	//	param - the string representing the parameter value to set for the log entry
	//	paramLength - the length of param above
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetParameter(unsigned int parameterIndex, char *param, unsigned int paramLength)
	{
		if (parameterIndex > (MAX_PARAMETER_COLUMNS - 1))
			parameterIndex = MAX_PARAMETER_COLUMNS - 1;

		if (parameterIndex >= numParameters)
		{
			numParameters = parameterIndex + 1;

			// allocate the right amount of memory for the parameters
			if (parameters == NULL)
				parameters = (char **)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char *) * numParameters);
			else
				parameters = (char **)HeapReAlloc(heap, HEAP_ZERO_MEMORY, parameters, sizeof(char *) * numParameters);

			if (parameterLengths == NULL)
				parameterLengths = (unsigned int *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned int) * numParameters);
			else
				parameterLengths = (unsigned int *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, parameterLengths, sizeof(unsigned int) * numParameters);
		}

		if (param == NULL)
		{
			parameters[parameterIndex] = NULL;
			parameterLengths[parameterIndex] = 0;
			RecomputeLogEntrySize();
			return;
		}

		parameterLengths[parameterIndex] = (unsigned int)lstrlenA(param);
		if (parameters[parameterIndex] == NULL)
			parameters[parameterIndex] = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (paramLength + 1));
		else
			parameters[parameterIndex] = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, parameters[parameterIndex], sizeof(char) * (paramLength + 1));

		CopyMemory(parameters[parameterIndex], param, paramLength);
		parameters[parameterIndex][paramLength] = '\0';
		RecomputeLogEntrySize();
	}

	//*************************************************************************
	// Method:		GetParameter
	// Description: Gets the parameter for the log entry
	//
	// Parameters:
	//	parameterIndex - the index of the parameter to get
	//	param - the buffer to put the parameter into.  Must be allocated by caller.
	//	paramLength - on entering the function, must be the maximum number of chars
	//		the param buffer will hold (include room for the null terminator).
	//		on leaving the function it will be the number of characters written 
	//		into param.
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetParameter(unsigned int parameterIndex, char *param, unsigned int &paramLength)
	{
		// keep the logic in this order, relying on short circuit of the second statement
		if ((param == NULL) || (parameterIndex >= numParameters) || ((paramLength < parameterLengths[parameterIndex] + 1)))
		{
			paramLength = 0;
			return;
		}

		paramLength = parameterLengths[parameterIndex] + 1;
		CopyMemory(param, parameters[parameterIndex], paramLength);
		param[parameterLengths[parameterIndex]] = '\0';
	}

	//*************************************************************************
	// Method:		SetEvent
	// Description: Sets an event for the log entry
	//
	// Parameters:
	//	eventIndex - the index of the event to set
	//  event - the event to set
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::SetEvent(unsigned int eventIndex, const LogEvent& event)
	{
		if (eventIndex >= numEvents)
		{
			numEvents = eventIndex + 1;

			// allocate the right amount of memory for the parameters
			if (events == NULL)
				events = (char **)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char *) * numEvents);
			else
				events = (char **)HeapReAlloc(heap, HEAP_ZERO_MEMORY, events, sizeof(char *) * numEvents);

			if (eventLengths == NULL)
				eventLengths = (unsigned int *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned int) * numEvents);
			else
				eventLengths = (unsigned int *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, eventLengths, sizeof(unsigned int) * numEvents);
		}

		unsigned int eventLength = event.GetDataLength();
		eventLengths[eventIndex] = eventLength + 1;
		if (events[eventIndex] == NULL)
			events[eventIndex] = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * (eventLength + 2));
		else
			events[eventIndex] = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, events[eventIndex], sizeof(char) * (eventLength + 2));

		events[eventIndex][0] = (char)event.GetType(); // First byte contains event type
		event.GetData(&events[eventIndex][1], eventLength);
		events[eventIndex][eventLength + 1] = '\0';
		RecomputeLogEntrySize();
	}

	//*************************************************************************
	// Method:		GetEvent
	// Description: Gets the event for the log entry
	//
	// Parameters:
	//	eventIndex - the index of the event to get
	//	event - the buffer to put the event into
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::GetEvent(unsigned int eventIndex, LogEvent &event)
	{
		// keep the logic in this order, relying on short circuit of the second statement
		if ((eventIndex >= numEvents) || (eventLengths[eventIndex] < 1))
			return;

		event.SetType((LogEventType)events[eventIndex][0]);
		// Copy over event data (skip first char, which contains event type)
		event.SetData(&events[eventIndex][1], eventLengths[eventIndex] - 1);
	}

	//*************************************************************************
	// Method:		ConvertToByteSequence
	// Description: Serializes the data in the log entry for sending across
	//	processes through a pipe or other method
	//
	// Parameters:
	//	buffer - the buffer to hold the byte sequence
	//	bufferSize - on entering the function, must be the maximum number of chars
	//		the param buffer will hold.  On exiting will hold the number of chars
	//		written to
	//
	// Return Value: None
	//*************************************************************************
	void LogEntry::ConvertToByteSequence(unsigned char *buffer, unsigned int &bufferSize)
	{
		if (bufferSize < logEntrySize)
		{
			bufferSize = 0;
			return;
		}

		unsigned int offset = 0;
		unsigned int i = 0;

		// copy the log size in
		CopyMemory((buffer + offset), &logEntrySize, sizeof(logEntrySize));
		offset += sizeof(logEntrySize);

		// copy the call depth in
		CopyMemory((buffer + offset), &callDepth, sizeof(callDepth));
		offset += sizeof(callDepth);

		// copy the number of parameters in
		CopyMemory((buffer + offset), &numParameters, sizeof(numParameters));
		offset += sizeof(numParameters);

		// copy the number of events in
		CopyMemory((buffer + offset), &numEvents, sizeof(numEvents));
		offset += sizeof(numEvents);

		// copy the timestamp length in
		CopyMemory((buffer + offset), &timeStampLength, sizeof(unsigned int));
		offset += sizeof(unsigned int);

		// copy the threadID length in
		CopyMemory((buffer + offset), &threadIDLength, sizeof(threadIDLength));
		offset += sizeof(threadIDLength);

		// copy the category length in
		CopyMemory((buffer + offset), &categoryLength, sizeof(categoryLength));
		offset += sizeof(categoryLength);

		// copy the dll name length in
		CopyMemory((buffer + offset), &dllNameLength, sizeof(dllNameLength));
		offset += sizeof(dllNameLength);

		// copy the function length in
		CopyMemory((buffer + offset), &functionLength, sizeof(functionLength));
		offset += sizeof(functionLength);

		// copy the return value length in
		CopyMemory((buffer + offset), &returnValueLength, sizeof(returnValueLength));
		offset += sizeof(returnValueLength);

		// copy the error code length in
		CopyMemory((buffer + offset), &errorCodeLength, sizeof(errorCodeLength));
		offset += sizeof(errorCodeLength);

		// copy the exception length in
		CopyMemory((buffer + offset), &exceptionLength, sizeof(exceptionLength));
		offset += sizeof(exceptionLength);

		// copy the parameter lengths in
		for (i = 0; i < numParameters; i++)
		{
			CopyMemory((buffer + offset), &(parameterLengths[i]), sizeof(parameterLengths[i]));
			offset += sizeof(parameterLengths[i]);
		}

		// copy the event lengths in
		for (i = 0; i < numEvents; i++)
		{
			CopyMemory((buffer + offset), &(eventLengths[i]), sizeof(eventLengths[i]));
			offset += sizeof(eventLengths[i]);
		}

		// copy the timestamp in
		CopyMemory((buffer + offset), &timeStamp, timeStampLength);
		offset += timeStampLength;

		// copy the threadID in
		CopyMemory((buffer + offset), threadID, threadIDLength);
		offset += threadIDLength;

		// copy the category in
		CopyMemory((buffer + offset), category, categoryLength);
		offset += categoryLength;

		// copy the dllName in
		CopyMemory((buffer + offset), dllName, dllNameLength);
		offset += dllNameLength;

		// copy the function in
		CopyMemory((buffer + offset), function, functionLength);
		offset += functionLength;

		// copy the returnValue in
		CopyMemory((buffer + offset), returnValue, returnValueLength);
		offset += returnValueLength;

		// copy the errorCode in
		CopyMemory((buffer + offset), errorCode, errorCodeLength);
		offset += errorCodeLength;

		// copy the exception in
		CopyMemory((buffer + offset), exception, exceptionLength);
		offset += exceptionLength;

		// copy the parameters in
		for (i = 0; i < numParameters; i++)
		{
			CopyMemory((buffer + offset), parameters[i], parameterLengths[i]);
			offset += parameterLengths[i];
		}

		// copy the events in
		for (i = 0; i < numEvents; i++)
		{
			CopyMemory((buffer + offset), events[i], eventLengths[i]);
			offset += eventLengths[i];
		}

		bufferSize = offset;
	}
}