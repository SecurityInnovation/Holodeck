//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEvent.cpp
//
// DESCRIPTION: Contains definition for the log event class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#define LogDll_File

#include "LogEntry.h"
#include "defines.h"

using namespace SiUtils;

namespace Log
{
	HANDLE LogEvent::heap = NULL;

	//*************************************************************************
	// Method:		LogEvent
	// Description: Constructor for the LogEvent class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEvent::LogEvent()
	{
		dataLength = 0;
		data = NULL;
	}

	//*************************************************************************
	// Method:		LogEvent
	// Description: Copy constructor for the LogEvent class
	//
	// Parameters:
	//	event - the event to copy
	//
	// Return Value: None
	//*************************************************************************
	LogEvent::LogEvent(const LogEvent& event)
	{
		type = event.type;
		data = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * event.GetDataLength());
		dataLength = event.GetDataLength();
		event.GetData(data, dataLength);
	}

	//*************************************************************************
	// Method:		~LogEvent
	// Description: Destructor for the LogEvent class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEvent::~LogEvent()
	{
		if (data != NULL)
		{
			HeapFree(heap, 0, data);
			data = NULL;
		}
	}

	//*************************************************************************
	// Method:		operator=
	// Description: Assignment operator for the LogEvent class
	//
	// Parameters:
	//	event - the event to assign
	//
	// Return Value: Self reference
	//*************************************************************************
	LogEvent& LogEvent::operator=(const LogEvent& event)
	{
		type = event.type;
		if (event.GetDataLength() > 0)
		{
			if (data == NULL)
				data = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * event.GetDataLength());
			else
				data = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, data, sizeof(char) * event.GetDataLength());
			dataLength = event.GetDataLength();
			event.GetData(data, dataLength);
		}
		else
		{
			if (data != NULL)
			{
				HeapFree(heap, 0, data);
				data = NULL;
			}
			dataLength = 0;
		}
		return *this;
	}

	LogEventType LogEvent::GetType() const
	{
		return type;
	}

	void LogEvent::SetType(LogEventType t)
	{
		type = t;
	}

	unsigned int LogEvent::GetDataLength() const
	{
		return dataLength;
	}

	void LogEvent::GetData(char* buf, unsigned int& len) const
	{
		if (len < dataLength)
		{
			len = 0;
			return;
		}

		len = dataLength;
		CopyMemory(buf, data, dataLength);
	}

	void LogEvent::SetData(char* buf, unsigned int len)
	{
		dataLength = len;

		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		if (data == NULL)
			data = (char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(char) * dataLength);
		else
			data = (char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, data, sizeof(char) * dataLength);

		CopyMemory(data, buf, len);
	}
}
