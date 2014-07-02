//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogSender.h
//
// DESCRIPTION: Contains the definition for the class LogSender
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 09 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#pragma warning(disable: 4251)

#include <stdio.h>
#include "LogEntry.h"
#include "LogEventList.h"
#include "ParameterType.h"
#include "SiString.h"
#include "Event.h"
#include "Mutex.h"
#include "MemoryMappedFile.h"
#include "LogQueue.h"
#include "defines.h"


#ifdef LogDll_File 
#define HolodeckLog_Api __declspec(dllexport)
#else
#define HolodeckLog_Api __declspec(dllimport)
#endif

using namespace SiUtils;

namespace Log
{
	//*************************************************************************
	// class to provide a mechanism to send logs to a LogReceiver object
	//*************************************************************************
	class HolodeckLog_Api LogSender
	{
	private:
        static HANDLE heap;

		CRITICAL_SECTION logGuard;
		Mutex forceWriteGuard;
		HANDLE logFile;

		bool heatInjectionDone;
		Event heatInjectionDoneEvent;

		LogQueue queue;
		bool sendLogThreadRunning;
		bool canSendLogs;
		HANDLE sendLogThread;
		Event logsNeedToBeSentEvent;
		Event logsCanBeAddedEvent;
		Event logsSentEvent;
		Event logsReceivedEvent;
		Event cantCreateLogFileEvent;
		Mutex logFileMutex;

		SiString *mappedFileName;
		MemoryMappedFile entryCountFile;

//*************************************************************************
// NOTE:
// Restrict log entries to 100 in a trial version. UI filtering in holoscriptapplication is not disabled, this is for restricting file size
// Log entries in the file are actually restricted to 200, not 100.
//*************************************************************************
		static int numLogEntriesSent;	

		static DWORD parentLogDisabledTlsSlot;

		static unsigned long WINAPI SendLogThreadFunction(void *param);
		static unsigned int GetCurrentCallLevel();
	public:
		LogSender();
		~LogSender();

		static HANDLE CreateAutThread(LPTHREAD_START_ROUTINE routine, LPVOID param, DWORD *pdwThreadID);
		static bool IsAutCreatedThread(DWORD dwThreadID);

		void ForceWrite();
		void StartSendLogThread();
		void StopSendLogThread();
		bool GetIsSendLogThreadRunning() { return sendLogThreadRunning; }
		void SendLog(DWORD tID, const SiString &threadID, const SiString &category, const SiString &functionName, const SiString &dllName, unsigned int numParams, void **parameters, ParameterType *paramTypes, DWORD errorCode, const SiString& exception, const SiString& returnValue, const LogEventList& eventList);

		static bool IsParentLogged();
		static void SetParentLogged(bool value);
	};

	extern "C" HolodeckLog_Api LogSender commonLogSender;
}
