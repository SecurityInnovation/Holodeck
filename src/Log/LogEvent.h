//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEvent.h
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
#pragma once

#include "SiArray.h"

#ifdef LogDll_File 
#define HolodeckLog_Api __declspec(dllexport)
#else
#define HolodeckLog_Api __declspec(dllimport)
#endif

namespace Log
{
	enum LogEventType
	{
		FileCorruptionEvent = 1,
		PauseAUTEvent,
		NetworkLogEvent
	};

	class HolodeckLog_Api LogEvent
	{
	private:
		static HANDLE heap;

		LogEventType type;
		unsigned int dataLength;
		char* data;

	public:
		LogEvent();
		LogEvent(const LogEvent& event);
		~LogEvent();
		LogEvent& operator=(const LogEvent& event);

		LogEventType GetType() const;
		void SetType(LogEventType type);

		unsigned int GetDataLength() const;

		void GetData(char* data, unsigned int& len) const;
		void SetData(char* data, unsigned int len);
	};
}
