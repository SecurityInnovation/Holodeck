//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReplacementLibrary.h
//
// DESCRIPTION: Contains definition for the class ReplacementLibrary.
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
#include <psapi.h>
#include "SettingsManager.h"
#include "FunctionTestList.h"
#include "Log.h"
#include "LogEvent.h"
#include <math.h>
#include "StackWalker.h"

using namespace Log;

namespace Replacement
{
	typedef bool (*OriginalFunctionCaller)(int numParams, void **params, DWORD *retValue, DWORD *errCode);

	//*************************************************************************
	// Contains common functionality for all replacement libraries in Holodeck.
	// All replacement libraries should have an instance of this class.
	//*************************************************************************
	class ReplacementLibrary
	{
	protected:
		SiString originalDllName;
		SettingsManager *settingsMgr;
		SiArray<SiString> createdFileList;
		SiHashTable<SiString> resourceMappings;		//key is handle value, value is resource name
		SiHashTable<HANDLE> handleMappings;			//use to map handle values together
		SiUtils::Mutex *resourceMappingMutex;		//Synchronizes use of resource and handle mappings
		SiUtils::Mutex *testPauseMutex;				//Ensures only one test can pause the AUT at a time
		DWORD currentTlsSlot;				//thread local storage slot

		void ModifyChangeParameters(int numParams, void **paramValues, ParameterType *paramTypes, FunctionTestList *testList, int matchingTest);
		void FlushLog(LogSender *sender);
		void SuspendProcess();
		SiArray<SiString> *GetResourceListForCurrentThread();
		void TestSpecialFunctionsAndCallOriginal(OriginalFunctionCaller ofCaller, char *category, char *functionName, void **paramValues, int numParams, DWORD *returnValue, DWORD *errorCode, DWORD allocationInfo, bool positiveAllocation);
		void TestFunctionForOutStringLogging(char *functionName, void **paramValues, ParameterType *paramTypes, int numParams, DWORD *returnValue, DWORD *errorCode);
		
	public:

		ReplacementLibrary(char *dllName);
		virtual ~ReplacementLibrary();

		SiString ConcatResourcePath(SiString str0, SiString str1);
		SiString ConcatResourcePath(HANDLE handle, SiString string);
		void CreateHandleMapping(HANDLE mapFromHandle, HANDLE mapToHandle);
		HANDLE GetHandleMapping(HANDLE handle);
		void CreateResourceMapping(HANDLE handle, SiString resourcePath);
		SiString GetResourceMapping(HANDLE handle);
		void AddToPathList(unsigned int index, SiString resourcePath);
		SiString GetPathFromList(unsigned int pos);
		void ClearPathList();
		void *GetOriginalFunction(const SiString &functionName);
		bool RunStandardTestsAndGetResults(LogSender *sender, OriginalFunctionCaller ofCaller, char *category, char *functionName, int numParams, void **paramValues, ParameterType *paramTypes, DWORD *returnValue, char *returnType, DWORD *errorCode, DWORD allocationInfo, bool positiveAllocation);
		bool RunLimitTests(LogSender *sender, OriginalFunctionCaller ofCaller, char *category, char *functionName, int numParams, void **paramValues, ParameterType *paramTypes, DWORD *returnValue, char *returnType, DWORD *errorCode, unsigned long limitInfo);

		bool GetFunctionTests(const char *category, const char *functionName, FunctionTestList &testList);
		bool DoesTestMatch(void **parameters, int numParameters, bool considerOutParams, SiArray<SiString> *resourcePathList, FunctionTestList &testList, DWORD *matchingErrorCode, DWORD *matchingReturnValue, unsigned long allocInfo, bool positiveAllocation, int *matchingTest);
		bool CheckFiringSpecification(FunctionTest *testList);
		bool IsCategoryLoggingEnabled(const char *category);
		bool IsFunctionLoggingEnabled(const char *category, const char *functionName);

		void *CallLoggingDisabledFunction(OriginalFunctionCaller ofCaller, unsigned int numParams, void **parameters);
		void SendLog(LogSender *sender, long threadID, const SiString &category, const SiString &functionName, unsigned int numParams, void **parameters, ParameterType *paramTypes, DWORD *errorCode, DWORD *returnValue, char *returnType, const LogEventList& eventList);

		void UpdateDiskUsage (unsigned long allocInfo, bool positiveAllocation);
		void UpdateMemoryUsage (unsigned long totalMemUsed);

		bool GetMaxDiskInfo(DWORD64 &maxDiskUsage);
		bool GetMaxMemInfo(DWORD64 &maxMemUsage);
		bool GetCurrentDiskUsage(DWORD64 &diskUsed);
		bool GetCurrentMemUsage(DWORD64 &memUsed);

		static void AddEventToCurrentEntry(const LogEvent& event);

		static void EnableInterception();
		static void DisableInterception();

		static HANDLE CreateAutThread(LPTHREAD_START_ROUTINE routine, LPVOID param, DWORD *pdwThreadID);
		static bool IsAutCreatedThread(DWORD dwThreadID);

		static unsigned int GetFileSizeInfoA (LPCTSTR lpFileName);
		static unsigned int GetFileSizeInfoW (LPCWSTR lpFileName);
		static unsigned long GetProcessMemory();
	};
}