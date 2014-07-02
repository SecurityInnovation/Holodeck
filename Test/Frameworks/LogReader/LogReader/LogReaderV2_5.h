//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogReaderV2_5.h
//
// DESCRIPTION: Contains definition for the class LogReaderV2_5
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "LogItem.h"
#include "ReceivedLogInfo.h"
#include "LogReader.h"

namespace HoloScript
{
	//*************************************************************************
	// class that reads HD v2_5 logs
	//*************************************************************************
	public __gc class LogReaderV2_5 : public LogReader
	{
	public:
		LogReaderV2_5();
		~LogReaderV2_5();
		
		void OpenLogFile(String *newLogFileName, String *exportLogFileName);
				 
	};
}