//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NotifiableLogReceiver.h
//
// DESCRIPTION: Contains the definition for the interface NotifiableLogReceiver
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "ReceivedLogInfo.h"

namespace HoloScript
{
	//*************************************************************************
	// interface that must be implemented to be notified of received logs
	//*************************************************************************
	public __gc __interface NotifiableLogReceiver
	{
	public:
		void NewLogsReceived(ReceivedLogInfo *info);
	};
}