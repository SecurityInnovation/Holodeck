//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEventList.h
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
#pragma once

#include "LogEvent.h"
#include "SiArray.h"

namespace Log
{
	class HolodeckLog_Api LogEventList
	{
	private:
		static DWORD currentEventArrayTlsSlot;

		SiUtils::SiArray<LogEvent> list;

	public:
		LogEventList();
		~LogEventList();

		unsigned int GetLength() const;
		const LogEvent& GetEvent(unsigned int index) const;

		void AddEvent(const LogEvent& event);

		static LogEventList* GetListForCurrentThread();
		static LogEventList* SetListForCurrentThread(LogEventList* list);
	};
}
