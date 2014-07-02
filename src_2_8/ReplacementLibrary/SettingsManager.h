//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SettingsManager.h
//
// DESCRIPTION: Contains definition for the class SettingsManager.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <rpcdce.h>
#include <stdlib.h>
#include <stdio.h>
#include <Shlwapi.h>
#include "SiString.h"
#include "SiHashTable.h"
#include "FunctionTestList.h"
#include "Modification.h"
#include "Event.h"
#include "defines.h"
using namespace SiUtils;

namespace Replacement
{
	struct SingleFunctionTestList
	{
		int threadId;
		FunctionTestList tests;
	};

	typedef SiArray<SingleFunctionTestList> FunctionTests;

	struct NetworkModification
	{
		bool send, recv;
		unsigned char portMask[8192]; // 65536 bits, one for each possible port
		Modification mod;
	};

	struct FileModification
	{
		SiString testId;
		bool regen;
		Modification mod;
	};

	//*************************************************************************
	// Manages the settings between HoloScript and all the replacement libraries
	//*************************************************************************
	class SettingsManager
	{
	protected:
		SiString programRootPath;
		SiHashTable<FunctionTests> functionTestTable;
		SiHashTable<bool> disabledLogTable;
		SiString projectPath, projectFolder, projectTitle;

		SiArray<NetworkModification> netCorruptFaults;
		SiHashTable< SiArray<FileModification> > fileCorruptFaults;

		SiHashTable<SiString> functionTestKeys;

		CRITICAL_SECTION functionTestLock;
		CRITICAL_SECTION disabledLogLock;
		CRITICAL_SECTION corruptLock;

		bool registryMonitorRunning;
		HANDLE registryMonitorThread;

		DWORD64 currentMemUsage, currentDiskUsage;
		DWORD64 maxMemUsage, maxDiskUsage, maxUploadBandwidth, maxDownloadBandwidth;
		bool maxMemUsageSet, maxDiskUsageSet, netUploadBandwidthSet, netDownloadBandwidthSet;
		static DWORD recvUpdateStamp, sendUpdateStamp;
		static DWORD64 currentUploadUsage, currentDownloadUsage;

		Event pauseEvent;

		static HANDLE heap;

		void Initialize(int processId, bool deletePreviousKey);
		bool DoesBooleanParamMatch(bool paramValue, bool testValue, ParameterOperatorType operatorType);
		bool DoesIntegerParamMatch(int paramValue, int testValue, ParameterOperatorType operatorType);
		bool DoesStringParamMatch(const SiString &paramValue, const SiString &testValue, ParameterOperatorType operatorType);
		bool DoesRealParamMatch(float paramValue, float testValue, ParameterOperatorType operatorType);
		bool DoesPointerParamMatch(unsigned long paramValue, unsigned long testValue, ParameterOperatorType operatorType);
		bool DoesInteger64ParamMatch(__int64 paramValue, __int64 testValue, ParameterOperatorType operatorType);

		static unsigned long WINAPI WaitForRegistryChangeThread(LPVOID settingsManagerPtr);
		void ReadFromRegistry();
		bool ReadFunctionTestsFromRegistry(const SiString &targetThreadID, const SiString &category, const SiString &functionName, FunctionTestList &testList);
		void ReadNetworkCorruptionFromRegistry();
		void ReadFileCorruptionFromRegistry();
		bool isResourceType(ParameterOperatorType operatorType);

	public:
		SettingsManager(int processId, bool deletePreviousKey = true);
		~SettingsManager();

		void *operator new(size_t size);
		void operator delete(void *p);

		void SetProjectPath(const SiString& path);
		bool GetProjectPath(SiString& path);
		bool GetProjectFolder(SiString& folder);
		bool GetProjectTitle(SiString& title);
		bool GetDTDFolder(SiString& folder);

		bool GetFunctionTests(int targetThreadID, const char *category, const char *functionName, FunctionTestList &testList);
		bool AddFunctionTest(const SiString &targetThreadID, const SiString &category, const SiString &functionName, FunctionTest *test, SiString *testID);
		bool DeleteFunctionTest(const SiString &targetThreadID, const SiString &testID);
		int IncrementTestExecutionCount(const SiString &testIDPath);
		int GetTestExecutionCount(const SiString &testIDPath);
		int GetTestExecutionCount(const SiString &targetThreadID, const SiString &category, const SiString &functionName, const SiString &testID);
		bool DoesTestMatch(void **parameters, int numParameters, bool considerOutParams, SiArray<SiString> *resourcePathList, FunctionTestList &testList, SiString &matchingErrorCode, SiString &matchingReturnValue, unsigned long allocInfo, bool positiveAllocation, int *matchingTest);

		int GetFunctionCallCount (const SiString &testIDPath);
		int IncrementFunctionCallCount (const SiString &testIDPath);

		bool AddNetworkCorruptionFault(bool send, bool recv, unsigned char* portMask, const SiString& searchRegExpr, const SiString& replacementStr, const SiString& testID);
		bool DeleteNetworkCorruptionFault(const SiString &testID);
		bool GetNetworkCorruptionFaults(bool send, int port, ModificationList &modList);

		bool AddFileCorruptionFault(const SiString &fileName, bool alwaysRegen, const SiString& searchRegExpr, const SiString& replacementStr, const SiString& testID);
		bool DeleteFileCorruptionFault(const SiString &testID);
		bool GetFileCorruptionFaults(const SiString &fileName, ModificationList &modList, bool& alwaysRegen);
		bool SetLastFileCorruption(const SiString &fileName, const SiString& corruptedFile, const SiString& changesXmlFile);
		bool ClearLastFileCorruption(const SiString &fileName);
		bool GetLastFileCorruption(const SiString &fileName, SiString& corruptedFile, SiString& changesXmlFile);

		bool IsCategoryLoggingEnabled(const char *category);
		bool IsFunctionLoggingEnabled(const char *category, const char *functionName);
		bool SetCategoryLoggingEnabled(const SiString &category, bool enabled);
		bool SetFunctionLoggingEnabled(const SiString &category, const SiString &functionName, bool enabled);
		bool CommitLoggingOptions();

		bool GetCachedMaximumDiskUsage(DWORD64 &maxUsage);
		bool GetMaximumDiskUsage(DWORD64 &maxUsage);
		bool GetCurrentDiskUsage(DWORD64 &currentUsage);
		bool GetCurrentCachedDiskUsage(DWORD64 &currentUsage);
		bool GetCachedMaximumMemoryUsage(DWORD64 &maxUsage);
		bool GetMaximumMemoryUsage(DWORD64 &maxUsage);
		bool GetCurrentMemoryUsage(DWORD64 &currentUsage);
		bool GetCurrentCachedMemoryUsage(DWORD64 &currentUsage);
		bool GetNetworkUploadBandwidth(DWORD64 &uploadBW);
		bool GetNetworkDownloadBandwidth(DWORD64 &downloadBW);
		bool GetCachedNetworkUploadBandwidth(DWORD64 &uploadBandwidth);
		bool GetCachedNetworkDownloadBandwidth(DWORD64 &downloadBandwidth);
		bool GetUploadBandwidthUsage (DWORD64 &bytesTransferred, DWORD &lastUpdateStamp);
		bool GetDownloadBandwidthUsage (DWORD64 &bytesTransferred, DWORD &lastUpdateStamp);
		bool GetCachedUploadBandwidthUsage (DWORD64 &bytesTransferred, DWORD &lastUpdateStamp);
		bool GetCachedDownloadBandwidthUsage (DWORD64 &bytesTransferred, DWORD &lastUpdateStamp);


		bool SetExternalDotNetLibsForInit(SiArray<SiString> * externalLibList);
		bool SetMaximumDiskUsage(DWORD64 maxUsage, bool isEnabled);
		bool SetCurrentDiskUsage(DWORD64 currentUsage);
		bool SetCurrentDiskUsageInRegistry();
		bool SetMaximumMemoryUsage(DWORD64 maxUsage, bool isEnabled);
		bool SetCurrentMemoryUsage(DWORD64 currentUsage);
		bool SetCurrentMemoryUsageInRegistry();
		bool SetNetworkUploadBandwidth (DWORD64 maxUploadSpeed, bool IsUploadControlEnabled);
		bool SetNetworkDownloadBandwidth (DWORD64 maxDownloadSpeed, bool IsDownloadControlEnabled);
		bool SetUploadBandwidthUsage (DWORD64 currentUploadRate, DWORD lastSendUpdateStamp);
		bool SetUploadBandwidthUsageInRegistry();
		bool SetDownloadBandwidthUsage (DWORD64 currentDownloadRate, DWORD lastRecvUpdateStamp);
		bool SetDownloadBandwidthUsageInRegistry();

		bool SetProcessChainingEnabled(bool enabled);
		bool GetProcessChainingEnabled(bool &enabled);

		bool GetLogPipeName(SiString &pipeName);
		void SuspendProcess();
		int CheckFiringSpecification (SiArray<SiString>* callStackList, FunctionTestList * functionTestList, int matchingTestIndex);
		int CheckFiringSpecification (SiArray<SiString>* callStackList, FunctionTest *functionTest);

		void SetTestPausedEvent();
		bool WaitForTestPausedEvent(DWORD timeout);
	};
}