//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestGenerationDataArray.h
//
// DESCRIPTION: Contains definition for the class TestGenerationDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Xml.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>
#using <HoloScript.dll>

#include "DisplayableDataArray.h"
#include "TestGenerationDataNode.h"
#include "FaultPaneDataArray.h"
#include "ResourcePaneDataArray.h"
#include "ResourceTestDataNode.h"
#include "LimitPaneDataNode.h"
#include "ScheduledTestDataNode.h"
#include "FileCorruptionDataNode.h"
#include "NetworkCorruptionDataNode.h"
#include "Defines.h"
#include "TestGenStatusBox.h"
#include "UIEventRegistry.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Threading;
using namespace System::Xml;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace FunctionDatabase;
using namespace HoloScript;
using namespace HolodeckGui;

namespace UIData
{
	public __value enum TestType
	{
		diskFault,
		memoryFault,
		networkFault,
		registryFault,
		processFault,
		customFault,
		diskLimit,
		memoryLimit,
		networkUploadLimit,
		networkDownloadLimit,
		networkCorruption
	};

	public __value enum LimitSet
	{
		AtCurrentUsage,
		OverCurrentUsage
	};

	public __gc class TestGenerationDataArray : public DisplayableDataArray
	{
	public:
		__property int get_TestInterval() { return testInterval; }
		__property int get_TestDuration() { return testDuration; }
		__property bool get_AllowDiskLimit() { return allowDiskLimit; }
		__property bool get_AllowMemoryLimit() { return allowMemoryLimit; }
		__property bool get_AllowNetworkLimit() { return allowNetworkLimit; }
		__property bool get_AllowDiskFaults() { return allowDiskFaults; }
		__property bool get_AllowMemoryFaults() { return allowMemoryFaults; }
		__property bool get_AllowNetworkFaults() { return allowNetworkFaults; }
		__property bool get_AllowRegistryFaults() { return allowRegistryFaults; }
		__property bool get_AllowProcessFaults() { return allowProcessFaults; }
		__property bool get_AllowCustomFaults() { return allowCustomFaults; }
		__property bool get_AllowNetworkCorruption() { return allowNetworkCorruption; }
		__property bool get_AllowTests() { return allowTests; }
		__property bool get_AllowResourceTests() { return allowResourceTests; }
		__property bool get_AllowFileCorruption() { return allowFileCorruption; }
		__property int get_NumOverlappingTests() { return numOverlappingTests; }
		__property UInt64 get_NetworkLowRange() { return networkLowRange; }
		__property UInt64 get_NetworkHighRange() { return networkHighRange; }
		__property int get_DiskLimitMax() { return diskLimitMax; }
		__property int get_MemoryLimitMax() { return memoryLimitMax; }
		__property int get_MaxCorruptAmount() { return maxCorruptAmount; }
		__property int get_MaxLongStringLen() { return maxLongStringLen; }

		TestGenerationDataArray(HoloScriptRegistry *HoloScriptRegistry, SortedList *dataArrayList);
		~TestGenerationDataArray();
		void CleanUp() { DisplayableDataArray::CleanUp(); }

		void StressTestGenerationStart(GenerationType GenerationType, Intensity intensity);
		void StressTestGenerationStop();
		void CodeCoverageTestGenerationStart(Intensity intensity, bool restartProject, int timeout);
		void CodeCoverageTestGenerationStop();
		bool LoadSettings(GenerationType generationType, Intensity intensity);

	private:
		LogsGeneratedDelegate *logHandler;
		ProjectRestartedDelegate *projectRestartedHandler;
		ProcessTerminatedDelegate *processTerminatedHandler;
		UIEventRegistry *uiEventRegistry;
		static Mutex *restartMutex = new Mutex(false);
		bool projectRestartStarted;

		int testInterval, testDuration;								//how often to enable a test and how long it will stay enabled.
		bool allowDiskLimit, allowMemoryLimit, allowNetworkLimit;
		bool allowDiskFaults, allowMemoryFaults, allowNetworkFaults, allowRegistryFaults, allowProcessFaults, allowCustomFaults;
		bool allowNetworkCorruption, allowFileCorruption;
		bool allowTests, allowResourceTests;						//only used by code coverage tests
		bool recording;												//true if in code coverage recording mode
		bool stressTestRunning;
		bool codeCoverageTestRunning;
		int numOverlappingTests;									//number of faults and/or tests can be set at once.  Limit overlap is set by the flags above
		UInt64 networkLowRange, networkHighRange;					//high and low settings for the network limit
		UInt64 maxDiskUsed, maxMemoryUsed;							//used to calculate max resource usage in cc testing
		UInt64 avgDiskUsed, avgMemoryUsed, dataPoints;				//used to calculate average resource usage in cc testing
		LimitSet diskLimitSet, memoryLimitSet;
		int diskLimitMax, memoryLimitMax;							//represents how many bytes over current usage the limit can be set
		int maxCorruptAmount, maxLongStringLen;						//options for file/network corruption
		int codeCoverTestTimeout;									//test timeout in milliseconds in cc testing
		HoloScriptRegistry *holoScriptRegistry;
		SortedList *dataArrayList;
		System::Threading::Timer *startRandomStressTimer, *startIntelligentStressTimer, *stopTimer, *restartTimer;
		Random *random;
		ArrayList *stressInjectList;
		SortedList *apiList;
		String *settingsFile;
		IList *diskFaults;
		IList *memoryFaults;
		IList *networkFaults;
		IList *registryFaults;
		IList *processFaults;
		IList *customFaults;
		TestGenerationDataNode *currentCodeCoverageTest;
		HolodeckGui::TestGenStatusBox *testGenStatusBox;

		void onStartRandomStressTimer(Object *state);
		void onStartIntelligentStressTimer(Object *state);
		void onStopTimer(Object *state);
		void onRestartTimer(Object *state);
		void BuildArray(GenerationType generationType);
		String *GetErrorCode(String *function);
		String *GetReturnValue(String *function);
		void onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID, unsigned int threadID);
		void UpdateArrayWithLogIndexes(long logEntryCount);
		void ProjectRestarted();
		void ProcessTerminated(unsigned int processID);
		String *GetCodeCoverageDescriptionText(DisplayableDataNode *dataNode);
	};
}