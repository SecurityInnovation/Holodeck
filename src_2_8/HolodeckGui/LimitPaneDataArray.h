//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LimitPaneDataArray.h
//
// DESCRIPTION: Contains definition for the class LimitPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 5 Mar 2004		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "LimitPaneDataNode.h"
#include "UIEventRegistry.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Diagnostics;
using namespace HoloScript;
using namespace FunctionDatabase;
using namespace HolodeckGui;

namespace UIData
{
	public __gc class LimitPaneDataArray : public DisplayableDataArray
	{
	public:
		
		[ReadOnly(true), Description("Maximum recorded disk usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_MaxDiskUsage() { return maxRecordedDiskUsage; }
		[ReadOnly(true), Description("Average recorded disk usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_AverageDiskUsage() { return averageRecordedDiskUsage; }

		[ReadOnly(true), Description("Maximum recorded memory usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_MaxMemoryUsage() { return maxRecordedMemoryUsage; }
		[ReadOnly(true), Description("Average recorded memory usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_AverageMemoryUsage() { return averageRecordedMemoryUsage; }

		[ReadOnly(true), Description("Maximum recorded upload bandwidth usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_MaxNetUpUsage() { return maxRecordedNetUpUsage; }
		[ReadOnly(true), Description("Average recorded upload bandwidth usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_AverageNetUpUsage() { return averageRecordedNetUpUsage; }

		[ReadOnly(true), Description("Maximum recorded download bandwidth usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_MaxNetDownUsage() { return maxRecordedNetDownUsage; }
		[ReadOnly(true), Description("Average recorded download bandwidth usage"), CategoryAttribute("Process Info")]
		__property UInt64 get_AverageNetDownUsage() { return averageRecordedNetDownUsage; }


		LimitPaneDataArray(unsigned int procID);
		~LimitPaneDataArray();
		void CleanUp();
		bool GetRealMaxNetwork(UInt64 &realMaxNetworkUp, UInt64 &realMaxNetworkDown);
		bool GetMonitorInformation(UInt64 &maximumDiskUsage, UInt64 &currentDiskUsage, UInt64 &maximumMemoryUsage, UInt64 &currentMemoryUsage);
		bool GetRealSpaceInformation(UInt64 &maximumDiskUsage, UInt64 &maximumMemoryUsage);
		bool GetNetworkUploadBandwidth(UInt64 &uploadBandwidth);
		bool GetNetworkDownloadBandwidth(UInt64 &downloadBandwidth);
		bool GetNetworkMonitorInformation (UInt64 &UploadUsage, UInt64 &DownloadUsage);

	private:
		PerformanceCounter *maxUsageCounter;
		HoloScriptApplication *holoScriptApp;	
		CreateLimitDelegate *CreateLimitHandler;
		ModifyLimitDelegate *ModifyLimitHandler;
		DeleteLimitDelegate *DeleteLimitHandler;
		ProcessTerminatedDelegate *ProcessTerminatedHandler;
		bool processTerminated;

		UInt64 maxRecordedDiskUsage, averageRecordedDiskUsage, lastRecordedDiskUsage;
		UInt64 maxRecordedMemoryUsage, averageRecordedMemoryUsage, lastRecordedMemoryUsage;
		UInt64 maxRecordedNetUpUsage, averageRecordedNetUpUsage, lastRecordedNetUpUsage;
		UInt64 maxRecordedNetDownUsage, averageRecordedNetDownUsage, lastRecordedNetDownUsage;

		void onCreateLimit(DisplayableDataNode *dataNode);
		void onModifyLimit(DisplayableDataNode *originalNode, DisplayableDataNode *modifiedNode, String *propertyName);
		void onDeleteLimit(DisplayableDataNode *dataNode);
		void onProcessTerminated(unsigned int processID);
	};
}