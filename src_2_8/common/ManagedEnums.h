//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ManagedEnums.h
//
// DESCRIPTION: Contains common enumerations for managed code
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>

using namespace System;

namespace Holodeck
{
	public __value enum HolodeckVersion
	{
		V2_0 = 0,
		V2_1,
		V2_5
	};

	public __value enum LogEventType
	{
		FileCorruptionEvent = 1,
		PauseAUTEvent,
		NetworkLogEvent
	};

	#define CurrentHolodeckVersion HolodeckVersion::V2_5
}