//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEventList.cpp
//
// DESCRIPTION: Contains definition for the log event list class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#define LogDll_File

#pragma warning(disable: 4251)

#include "LogEventList.h"
#include "defines.h"

using namespace SiUtils;

namespace Log
{
	HolodeckLog_Api DWORD LogEventList::currentEventArrayTlsSlot = TLS_OUT_OF_INDEXES;

	//*************************************************************************
	// Method:		LogEventList
	// Description: Constructor for the LogEventList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEventList::LogEventList()
	{
	}

	//*************************************************************************
	// Method:		~LogEventList
	// Description: Destructor for the LogEventList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEventList::~LogEventList()
	{
	}

	unsigned int LogEventList::GetLength() const
	{
		return list.GetLength();
	}

	const LogEvent& LogEventList::GetEvent(unsigned int index) const
	{
		return list[index];
	}

	void LogEventList::AddEvent(const LogEvent& event)
	{
		list.Add(event);
	}

	LogEventList* LogEventList::GetListForCurrentThread()
	{
		if (currentEventArrayTlsSlot == TLS_OUT_OF_INDEXES)
			currentEventArrayTlsSlot = TlsAlloc();

		return (LogEventList*)TlsGetValue(currentEventArrayTlsSlot);
	}

	LogEventList* LogEventList::SetListForCurrentThread(LogEventList* eventList)
	{
		if (currentEventArrayTlsSlot == TLS_OUT_OF_INDEXES)
			currentEventArrayTlsSlot = TlsAlloc();

		// this breaks ppt 11, but if the value is >= 64 the asm code is not executed, some other code is,
		// so we just have to deal with it.  Still trying to figure out why the and statement breaks it
		LogEventList* oldList;
		if (currentEventArrayTlsSlot >= 64)
		{
			oldList = (LogEventList*)TlsGetValue(currentEventArrayTlsSlot);
		}
		else
		{
			_asm
			{
				mov	eax, fs:[0x18]
				mov	ecx, currentEventArrayTlsSlot
				mov	eax, [eax+ecx*4+0xE10]
				//and	fs:[0x34], 0
				mov	[oldList], eax
			}
		}
		TlsSetValue(currentEventArrayTlsSlot, eventList);
		return oldList;
	}
}
