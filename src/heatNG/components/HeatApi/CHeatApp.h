//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CHeatApp.h
//
// DESCRIPTION: Contains implementation for the class CHeatApp, part of the
//	HEAT API
//
//*************************************************************************
#ifndef __HEATIIAPI_H__
#define __HEATIIAPI_H__


#include <windows.h>
#include "CIntercepts.h"

#ifdef  HEATAPI_EXPORTS
#define HEATAPI_API __declspec(dllexport)
#else
#define HEATAPI_API __declspec(dllimport)
#endif

// Error codes
#define HEAT_SUCCESS					0
#define HEAT_INVALID_PARAMETERS			1
#define HEAT_ALREADY_ATTACHED			2
#define HEAT_FAILED_CREATE_PROCESS		3
#define HEAT_NOT_READY					4
#define HEAT_ALREADY_RUNNING			5
#define HEAT_FAILED_RESUME_PROCESS		6
#define HEAT_FAILED_TERMINATE_PROCESS	7
#define HEAT_ALREADY_PAUSED				8
#define HEAT_ALREADY_INJECTED			9
#define HEAT_FAILED_INJECT_DLL			10
#define HEAT_ALREADY_INTERCEPTED		11
#define HEAT_FAILED_INTERCEPT			12
#define HEAT_NOT_INJECTED				13
#define HEAT_FAILED_PAUSE_PROCESS		14
#define HEAT_FAILED_UNINTERCEPT			15
#define HEAT_FAILED_OPEN_PROCESS		16
#define HEAT_FAILED_CREATE_EVENT		17
#define HEAT_FAILED_SET_EVENT			18
#define HEAT_FAILED_SEND_DEINIT			19

#define HEAT_NOT_IMPLEMENTED    -1

DWORD WINAPI MonitorThreadProc(LPVOID lpParameter);

class HEATAPI_API CHeatApp
{
	bool m_bReady, m_bRunning, m_bInjected;
	bool m_bUseRemoteThread, m_bUseAPC;
	CIntercepts m_ciIntercepts;
	PROCESS_INFORMATION m_piProcInfo;
	HANDLE m_hProcess;

	PVOID allocDataInProc(HANDLE hProcess, PVOID pData, DWORD dwLen);
	DWORD injectModule(LPSTR pszDLLName);
	DWORD sendIntercepts();
	DWORD sendUnintercept(void *function);
	void  startMonitorThread();

    void	*m_heatNGapp;
    wchar_t *m_pwszHeatServerPath;


public:
	CHeatApp(unsigned short *pwszHeatServerPath = NULL);
	~CHeatApp();

	DWORD initializeApp(LPSTR pszAppPath, LPSTR pszAppName);// Creates a suspended application
	DWORD attachToApp(const PROCESS_INFORMATION& procInfo);	// Attach to a suspended process
	DWORD attachToApp(DWORD dwProcessId);					// Attach to a suspended process
	DWORD attachToRunningApp(const PROCESS_INFORMATION& procInfo);	// Attach to an already running app/service
	DWORD attachToRunningApp(DWORD dwProcessId);					// Attach to an already running app/service
	DWORD detachFromApp();									// Detach from a process
	DWORD deinitializeApp();								// Terminates the process and clears all interceptions

	DWORD forceInject();									// Injects the HEAT DLL into an initialized process

	DWORD runApp();											// Beings execution of the process
	DWORD pauseApp();										// Suspends execution of the process
	DWORD setPauseState(bool value);						// For externally setting the pause state of the application
	DWORD resumeApp();										// Continues execution of the process
	DWORD terminateApp();									// Terminates execution of the process

	DWORD interceptFunc(LPSTR pszFuncToIntercept, LPSTR pszFuncDLL, LPSTR pszHandlerFunc, LPSTR pszHandlerDLL);
															// Begins intercepting the function given by pszFuncToIntercept,
															// rerouting calls to the function given by pszHandlerFunc
	DWORD deInitOnProcessTerminate(DWORD pid);				// Calls deinit if the process specified terminates

	bool isReady();											// Returns true if the application is ready to run/intercept
	bool isRunning();										// Returns true if the application is running
	void setIsRunning(bool value) {m_bRunning = value;}		// allows thread proc to set running state
	void setIsReady(bool value) {m_bReady = value;}		// allows thread proc to set ready state
	void setIsInjected(bool value) {m_bInjected = value;}		// allows thread proc to set injected state
	DWORD getProcessId();									// Returns the process ID of the current process
	HANDLE getProcessHandle() {return m_hProcess;}			// Returns the handle of the current process
	DWORD getThreadId();									// Returns the ID of the current thread
	DWORD getThreadsInCurrentProcess(DWORD *threadList);	// Returns an array of thread IDs being used by the process
};

#endif
