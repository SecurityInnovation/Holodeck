#define _WIN32_WINNT 0x400

#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>
#include <dbghelp.h>
#include "CHeatApp.h"

#define CMD_INTERCEPT 1
#define CMD_UNINTERCEPT 2
#define CMD_DEINIT_ON_PROCESS_TERMINATE 3

#define TIMEOUT_LENGTH 10000

//********************************************************************
//
// |Routine Name              modifyProcessState
//
// |Description               Suspends or Resumes a process
//                            if bSuspend is true, it suspends
//                            else it resumes
//
// |Return Value              success or failuire
//
// |PreCondition(s)           If called with bSuspend false, the process
//                            should be already paused
//
// |PostCondition(s)          Process is suspended or resumed
//
// |Developed by   Rahul Chaturvedi
//                            Team Lead - HEAT Base Development Team
//
// |Date of Creation          03-17-2001
//
// |Date of Last Update       03-17-2001
//
// |Version                   1.0.0
//
//********************************************************************

BOOL modifyProcessState(DWORD dwProcessId, BOOL bSuspend)
{
	DWORD dwProcPriority = GetPriorityClass(GetCurrentProcess());
	DWORD dwThreadPriority = GetThreadPriority(GetCurrentThread());

	SetPriorityClass(GetCurrentProcess, REALTIME_PRIORITY_CLASS);
	SetThreadPriority(GetCurrentThread, THREAD_PRIORITY_TIME_CRITICAL);

	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	if (!hSnap)
		return FALSE;

	THREADENTRY32 first = {0};
	first.dwSize = sizeof(first);

	if (!Thread32First(hSnap, &first))
	{
		CloseHandle(hSnap);
		return FALSE;
	}

	do
	{
		if (first.th32OwnerProcessID == dwProcessId)
		{
			HANDLE hThread =
			OpenThread(THREAD_SUSPEND_RESUME, false, first.th32ThreadID);

			if (!hThread)
			{
				CloseHandle(hSnap);
				return FALSE;
			}

			if (bSuspend)
				SuspendThread(hThread);
			else
				ResumeThread(hThread);
			CloseHandle(hThread);

		}
		ZeroMemory(&first, sizeof(first));
		first.dwSize = sizeof(first);
	} while (Thread32Next(hSnap, &first));

	CloseHandle(hSnap);

	SetPriorityClass(GetCurrentProcess(), dwProcPriority);
	SetThreadPriority(GetCurrentThread(), dwThreadPriority);

	return TRUE;
}

DWORD getThreadIdFromProcessId(DWORD dwProcessId)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
	if (!hSnap)
		return NULL;

	THREADENTRY32 first = {0};
	first.dwSize = sizeof(first);

	if (!Thread32First(hSnap, &first))
	{
		CloseHandle(hSnap);
		return 0;
	}

	do
	{
		if (first.th32OwnerProcessID == dwProcessId)
		{
			CloseHandle(hSnap);
			return first.th32ThreadID;
		}
		ZeroMemory(&first, sizeof(first));
		first.dwSize = sizeof(first);
	} while (Thread32Next(hSnap, &first));

	CloseHandle(hSnap);
	return 0;
}

BOOL CALLBACK GetFirstWindowEnumProc(HWND hWnd, LPARAM pRet)
{
	*((HWND*)pRet) = hWnd;
	return FALSE;
}

//*************************************************************************
// Function:	findMangledNameInModule
// Description: Looks for a function by name in a module and returns
//				the mangled form of it.
//
// Parameters:
//	hMod - module to search in
//  szFuncName - unmangled function name on entry, mangled name on exit
//  maxStrLen - size in characters of szFuncName buffer
//
// Return Value: true if the function was found
//*************************************************************************
bool findMangledNameInModule(HMODULE hMod, char* szFuncName, int maxStrLen)
{
	// Find the export directory
	const IMAGE_DOS_HEADER* dosHeader = (const IMAGE_DOS_HEADER*)hMod;
	const IMAGE_NT_HEADERS32* ntHeader = (const IMAGE_NT_HEADERS32*)&((BYTE*)hMod)[dosHeader->e_lfanew];

	if (ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == 0)
		return false;

	const IMAGE_EXPORT_DIRECTORY* pExportTable = (const IMAGE_EXPORT_DIRECTORY*)((UINT_PTR)hMod +
		ntHeader->OptionalHeader. DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	const DWORD* pFunctionNames = (const DWORD*)((UINT_PTR)hMod + pExportTable->AddressOfNames);

	for (DWORD i=0; i<pExportTable->NumberOfNames; i++)
	{
		if (!pFunctionNames[i])
			continue;

		const char* szCurName = (const char*)((UINT_PTR)hMod + pFunctionNames[i]);
		char undecoratedName[MAX_PATH];

		if (!UnDecorateSymbolName(szCurName, undecoratedName, MAX_PATH, UNDNAME_NAME_ONLY))
			continue;

		if (!strcmp(undecoratedName, szFuncName))
		{
			strncpy(szFuncName, szCurName, maxStrLen);
			szFuncName[maxStrLen - 1] = 0; // ensure it is NULL terminated
			return true;
		}
	}

	return false;
}

inline void writeByte(PBYTE& dest,BYTE n)
{
	if (dest)
		*(dest++)=n;
}

inline void writeWord(PBYTE& dest,WORD n)
{
	writeByte(dest,n&0xff);
	writeByte(dest,(n>>8)&0xff);
}

inline void writeDWord(PBYTE& dest,DWORD n)
{
	writeWord(dest,(WORD)(n&0xffff));
	writeWord(dest,(WORD)((n>>16)&0xffff));
}


DWORD WINAPI delayedInjectThread(LPVOID pCHeatApp)
{
	CHeatApp* pApp = (CHeatApp*)pCHeatApp;

	// Initialize a security descriptor.
	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
	// Initialize a security attributes structure.
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	char *pszPipeReadyEventName = new char[MAX_PATH];
	sprintf(pszPipeReadyEventName, "HEATPipeReady%d", pApp->m_piProcInfo.dwProcessId);
	HANDLE hPipeReadyEvent = CreateEvent(&sa, FALSE, FALSE, pszPipeReadyEventName);
	if (!hPipeReadyEvent)
		return 1;

	char *pszPipeCompleteEventName = new char[MAX_PATH];
	sprintf(pszPipeCompleteEventName, "HEATPipeComplete%d", pApp->m_piProcInfo.dwProcessId);
	HANDLE hPipeCompleteEvent = CreateEvent(&sa, FALSE, FALSE, pszPipeCompleteEventName);
	if (!hPipeCompleteEvent)
		return 1;

	LocalFree(pSD);

	WaitForSingleObject(hPipeReadyEvent, INFINITE);
	pApp->sendIntercepts();
	SetEvent(hPipeCompleteEvent);

	CloseHandle(hPipeReadyEvent);
	CloseHandle(hPipeCompleteEvent);
	delete[] pszPipeReadyEventName;
	delete[] pszPipeCompleteEventName;
	return 0;
}


CHeatApp::CHeatApp()
{
	m_bReady = false;
	m_bRunning = false;
	m_bInjected = false;
	m_bUseRemoteThread = false;
	m_hProcess = NULL;

	// Use QueueUserAPC on Longhorn or higher (SetThreadContext does not work properly)
	OSVERSIONINFO osVersion;
	osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osVersion);
	if (osVersion.dwMajorVersion >= 6)
		m_bUseAPC = true;
	else
		m_bUseAPC = false;

	HANDLE Token;

    TOKEN_PRIVILEGES privs; 
    privs.PrivilegeCount = 1; 

    // Get our access token 
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &Token)) 
            printf ("Couldn't get access to process access token\n"); 

    // Get LUID of SeDebugName privilege 
    if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &privs.Privileges[0].Luid)) 
            printf("Error getting LUID of SeDebugPrivilege privilege\n"); 
    
    // Find this privilege in the array and mark it as enabled 
    privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
    AdjustTokenPrivileges(Token, FALSE, &privs, sizeof(privs), NULL, NULL);

	CloseHandle(Token);
}

CHeatApp::~CHeatApp()
{
	try
	{
		CloseHandle(m_hProcess);
		//make sure we don't leave the app in a paused state.
		resumeApp();
	}
	catch(...)
	{
	}
}

// Creates a suspended application
DWORD CHeatApp::initializeApp(LPSTR pszAppPath, LPSTR pszAppName)
{
	if (m_bReady)
		return HEAT_ALREADY_ATTACHED;

	if (!pszAppPath)
		return HEAT_INVALID_PARAMETERS;
	if (!pszAppName)
		return HEAT_INVALID_PARAMETERS;

	// Construct the full path to the application that is being created
	char* pszFullPath = new char[strlen(pszAppPath) + strlen(pszAppName) + 2];
	strcpy(pszFullPath, pszAppPath);
	if ((strlen(pszAppPath) > 0) && (pszAppPath[strlen(pszAppPath) - 1] != '\\'))
		strcat(pszFullPath, "\\");		// If '\' is not last char of path, append a '\'
	strcat(pszFullPath, pszAppName);

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;

	if (!CreateProcess(NULL, pszFullPath, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, pszAppPath,
		&si, &m_piProcInfo))
	{
		delete[] pszFullPath;
		return HEAT_FAILED_CREATE_PROCESS;
	}

	m_hProcess = m_piProcInfo.hProcess;

	m_bUseRemoteThread = false;
	m_bReady = true;

	delete[] pszFullPath;
	return HEAT_SUCCESS;
}

// Attach to a process
DWORD CHeatApp::attachToApp(const PROCESS_INFORMATION& procInfo)
{
	if (m_bReady)
		return HEAT_ALREADY_ATTACHED;

	if (procInfo.dwProcessId == GetCurrentProcessId())
		return HEAT_FAILED_INJECT_DLL;

	if (!(m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, procInfo.dwProcessId)))
		return HEAT_FAILED_OPEN_PROCESS;
	else
		startMonitorThread();

	m_piProcInfo = procInfo;

	m_bUseRemoteThread = false;
	m_bReady = true;

	return HEAT_SUCCESS;
}

// Attach to a process
DWORD CHeatApp::attachToApp(DWORD dwProcessId)
{
	if (m_bReady)
		return HEAT_ALREADY_ATTACHED;

	if (dwProcessId == GetCurrentProcessId())
		return HEAT_FAILED_INJECT_DLL;

	if (!(m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessId)))
		return HEAT_FAILED_OPEN_PROCESS;
	else
		startMonitorThread();

	m_piProcInfo.dwProcessId = dwProcessId;
	m_piProcInfo.dwThreadId = getThreadIdFromProcessId(dwProcessId);

	if (!m_piProcInfo.dwThreadId)
		return HEAT_FAILED_OPEN_PROCESS;

	m_bUseRemoteThread = false;
	m_bReady = true;

	return HEAT_SUCCESS;
}

// Attach to an already running app/service
DWORD CHeatApp::attachToRunningApp(const PROCESS_INFORMATION& procInfo)
{
	if (m_bReady)
		return HEAT_ALREADY_ATTACHED;

	if (procInfo.dwProcessId == GetCurrentProcessId())
		return HEAT_FAILED_INJECT_DLL;

	if (!(m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, procInfo.dwProcessId)))
		return HEAT_FAILED_OPEN_PROCESS;
	else
		startMonitorThread();

	m_piProcInfo = procInfo;

	m_bUseRemoteThread = true;
	m_bReady = true;
	m_bRunning = true;

	return HEAT_SUCCESS;
}

// Attach to an already running app/service
DWORD CHeatApp::attachToRunningApp(DWORD dwProcessId)
{
	if (m_bReady)
		return HEAT_ALREADY_ATTACHED;

	if (dwProcessId == GetCurrentProcessId())
		return HEAT_FAILED_INJECT_DLL;

	if (!(m_hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, dwProcessId)))
		return HEAT_FAILED_OPEN_PROCESS;
	else
		startMonitorThread();

	m_piProcInfo.dwProcessId = dwProcessId;
	m_piProcInfo.dwThreadId = getThreadIdFromProcessId(dwProcessId);

	if (!m_piProcInfo.dwThreadId)
		return HEAT_FAILED_OPEN_PROCESS;

	m_bUseRemoteThread = true;
	m_bReady = true;
	m_bRunning = true;

	return HEAT_SUCCESS;
}

// Detach from a process
DWORD CHeatApp::detachFromApp()
{
	if (!m_bReady)
		return HEAT_NOT_READY;

	if (!m_bInjected)
	{
		// Wasn't injected, so we're done
		m_bReady = false;
		return HEAT_SUCCESS;
	}

	DWORD dwErr;

	// Make sure there are no intercepted functions
	CNode *pNode = m_ciIntercepts.getInterceptList();
	while (pNode)
	{
		if (pNode->getSent())
		{
			if ((dwErr = sendUnintercept(pNode->getFuncName(), pNode->getFuncDLL())) != HEAT_SUCCESS)
				return dwErr;
		}
		pNode = pNode->getNext();
	}

	m_ciIntercepts.markAllUnsent();

	bool bResumeWhenDone = false;

	if (m_bRunning && (!m_bUseRemoteThread))
	{
		bResumeWhenDone = true;
		if ((dwErr = pauseApp()) != HEAT_SUCCESS)
			return dwErr;
	}

	// Unload the DLL from the process
	if ((dwErr = ejectModule("Heat4Dll.dll")) != HEAT_SUCCESS)
		return dwErr;

	m_bInjected = false;

	if (bResumeWhenDone)
	{
		if ((dwErr = resumeApp()) != HEAT_SUCCESS)
			return dwErr;
	}

	m_bReady = false;

	return HEAT_SUCCESS;
}

// Terminates the process and clears all interceptions
DWORD CHeatApp::deinitializeApp()
{
	if (!m_bReady)
		return HEAT_NOT_READY;

	DWORD dwErr;
	if ((dwErr = terminateApp()) != HEAT_SUCCESS)
		return dwErr;

	m_ciIntercepts.removeAllIntercepts();

	return HEAT_SUCCESS;
}

DWORD CHeatApp::sendIntercepts()
{
	if (!m_bReady)
		return HEAT_NOT_READY;

	CNode *pNode = m_ciIntercepts.getInterceptList();
	while (pNode)
	{
		if (!pNode->getSent())
		{
			char pszPipeName[MAX_PATH];
			sprintf(pszPipeName, "\\\\.\\pipe\\HEATDataPipe%d", m_piProcInfo.dwProcessId);
			if (!WaitNamedPipe(pszPipeName, TIMEOUT_LENGTH))
				return HEAT_FAILED_INTERCEPT;
			HANDLE hFile = CreateFile(pszPipeName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
			if (hFile == INVALID_HANDLE_VALUE)
				return HEAT_FAILED_INTERCEPT;

			BYTE ucCommand = CMD_INTERCEPT;
			DWORD dwBytes, dwStrLen;

			WriteFile(hFile, &ucCommand, 1, &dwBytes, NULL);
			if (dwBytes != 1)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}

			dwStrLen = strlen(pNode->getFuncName());
			WriteFile(hFile, &dwStrLen, 4, &dwBytes, NULL);
			if (dwBytes != 4)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}
			WriteFile(hFile, pNode->getFuncName(), dwStrLen, &dwBytes, NULL);
			if (dwBytes != dwStrLen)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}

			dwStrLen = strlen(pNode->getFuncDLL());
			WriteFile(hFile, &dwStrLen, 4, &dwBytes, NULL);
			if (dwBytes != 4)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}
			WriteFile(hFile, pNode->getFuncDLL(), dwStrLen, &dwBytes, NULL);
			if (dwBytes != dwStrLen)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}

			dwStrLen = strlen(pNode->getHandlerFunc());
			WriteFile(hFile, &dwStrLen, 4, &dwBytes, NULL);
			if (dwBytes != 4)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}
			WriteFile(hFile, pNode->getHandlerFunc(), dwStrLen, &dwBytes, NULL);
			if (dwBytes != dwStrLen)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}

			dwStrLen = strlen(pNode->getHandlerDLL());
			WriteFile(hFile, &dwStrLen, 4, &dwBytes, NULL);
			if (dwBytes != 4)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}
			WriteFile(hFile, pNode->getHandlerDLL(), dwStrLen, &dwBytes, NULL);
			if (dwBytes != dwStrLen)
			{
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}

			char pszProcessedEventName[MAX_PATH];
			sprintf(pszProcessedEventName, "HEATCmdProcessed%d", m_piProcInfo.dwProcessId);
			HANDLE hProcessedEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, pszProcessedEventName);

			if (WaitForSingleObject(hProcessedEvent, TIMEOUT_LENGTH) == WAIT_TIMEOUT)
			{
				CloseHandle(hProcessedEvent);
				CloseHandle(hFile);
				return HEAT_FAILED_INTERCEPT;
			}

			CloseHandle(hProcessedEvent);

			FlushFileBuffers(hFile);
			CloseHandle(hFile);

			pNode->setSent(true);
		}
		else if (pNode->getDeleted())
		{
			sendUnintercept(pNode->getFuncName(), pNode->getFuncDLL());
			CNode *pNextNode = pNode->getNext();
			m_ciIntercepts.removeIntercept(pNode->getFuncName(), pNode->getFuncDLL());
			pNode = pNextNode;
			continue;
		}

		pNode = pNode->getNext();
	}
	return HEAT_SUCCESS;
}

// Allocates memory in another process and copies the data pointed to by pData into that memory.
// If pData is NULL, no data is copied.
PVOID CHeatApp::allocDataInProc(HANDLE hProcess, PVOID pData, DWORD dwLen)
{
	PVOID pDataOther = VirtualAllocEx(hProcess, NULL, dwLen, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pDataOther)
		return NULL;

	if (pData)
	{
		if (!WriteProcessMemory(hProcess, pDataOther, pData, dwLen, NULL))
		{
			VirtualFreeEx(hProcess, pDataOther, dwLen, MEM_RELEASE);
			return NULL;
		}
	}
	return pDataOther;
}

DWORD CHeatApp::injectModule(LPSTR pszDLLName)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	char pszCompleteEventName[MAX_PATH];
	char pszPipeReadyEventName[MAX_PATH];
	char pszPipeCompleteEventName[MAX_PATH];
	char* pszInitFuncName = "init";
	PBYTE pdwCode = NULL;
	PBYTE pdwOutPtr = NULL;
	PVOID pvCompleteEventName = NULL;
	PVOID pvPipeCompleteEventName = NULL;
	PVOID pvDLLName = NULL;
	PVOID pvInitFuncName = NULL;
	PVOID pvCode = NULL;
	CONTEXT cxt;
	HANDLE hCompleteEvent = NULL;
	HANDLE hPipeReadyEvent = NULL;
	HANDLE hPipeCompleteEvent = NULL;
	DWORD dwSuspendCount = 0;
	DWORD dwErr;
	DWORD dwFailureCode = HEAT_FAILED_INJECT_DLL;
	HWND hWnd;
	PSECURITY_DESCRIPTOR pSD = NULL;
	SECURITY_ATTRIBUTES sa;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, m_piProcInfo.dwProcessId);
	if (!hProcess)
		goto failed;

	if (!m_bUseRemoteThread)
	{
		hThread = OpenThread(THREAD_ALL_ACCESS, false, m_piProcInfo.dwThreadId);
		if (!hThread)
			goto failed;

		if (!m_bUseAPC)
		{
			ZeroMemory(&cxt, sizeof(cxt));
			cxt.ContextFlags = CONTEXT_FULL;
			if (!GetThreadContext(hThread, &cxt))
				goto failed;
		}
	}

	sprintf(pszCompleteEventName, "HEATInjectComplete%d", m_piProcInfo.dwProcessId);
	pvCompleteEventName = allocDataInProc(hProcess, pszCompleteEventName, MAX_PATH);

	sprintf(pszPipeCompleteEventName, "HEATPipeComplete%d", m_piProcInfo.dwProcessId);
	pvPipeCompleteEventName = allocDataInProc(hProcess, pszPipeCompleteEventName, MAX_PATH);

	pvDLLName = allocDataInProc(hProcess, pszDLLName, strlen(pszDLLName) + 1);

	pvInitFuncName = allocDataInProc(hProcess, pszInitFuncName, strlen(pszInitFuncName) + 1);

	pdwCode = new BYTE[256];
	pvCode = allocDataInProc(hProcess, NULL, 256);

	// Code for injecting DLL:
	//  pushad
	//  pushfd
	//  push pvDLLName
	//  call GetModuleHandle
	//  test eax, eax
	//  jnz AlreadyLoaded
	//  push pvDLLName
	//  call LoadLibrary
	// AlreadyLoaded:
	//  push pvInitFuncName
	//	push eax
	//	call GetProcAddress
	//	call eax
	//  push pvPipeCompleteEventName
	//  push 0
	//  push SYNCHRONIZE
	//  call OpenEvent
	//  push eax
	//  push INFINITE
	//  push eax
	//  call WaitForSingleObject
	//  call CloseHandle
	//  push pvCompleteEventName
	//  push 0
	//  push EVENT_MODIFY_STATE
	//  call OpenEvent
	//  push eax
	//  push eax
	//  call SetEvent
	//  call CloseHandle
	//  push 0xfe  ; current thread
	//  call SuspendThread
	//  popfd
	//  popad
	//  jmp OriginalEIP

	pdwOutPtr = pdwCode;

	writeByte(pdwOutPtr, 0x60);		// pushad
	writeByte(pdwOutPtr, 0x9c);		// pushfd
	
	writeByte(pdwOutPtr, 0x68);		// push pvDLLName
	writeDWord(pdwOutPtr, (DWORD)pvDLLName);

	writeByte(pdwOutPtr, 0xe8);		// call GetModuleHandle
	writeDWord(pdwOutPtr, (DWORD)GetModuleHandleA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x85);		// test eax, eax
	writeByte(pdwOutPtr, 0xc0);

	writeByte(pdwOutPtr, 0x75);		// jnz AlreadyLoaded
	writeByte(pdwOutPtr, 10);

	writeByte(pdwOutPtr, 0x68);		// push pvDLLName
	writeDWord(pdwOutPtr, (DWORD)pvDLLName);

	writeByte(pdwOutPtr, 0xe8);		// call LoadLibrary
	writeDWord(pdwOutPtr, (DWORD)LoadLibraryA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x68);		// push pvInitFuncName
	writeDWord(pdwOutPtr, (DWORD)pvInitFuncName);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call GetProcAddress
	writeDWord(pdwOutPtr, (DWORD)GetProcAddress - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xff);		// call eax
	writeByte(pdwOutPtr, 0xd0);

	writeByte(pdwOutPtr, 0x68);		// push pvPipeCompleteEventName
	writeDWord(pdwOutPtr, (DWORD)pvPipeCompleteEventName);

	writeByte(pdwOutPtr, 0x6a);		// push 0
	writeByte(pdwOutPtr, 0);

	writeByte(pdwOutPtr, 0x68);		// push SYNCHRONIZE
	writeDWord(pdwOutPtr, (DWORD)SYNCHRONIZE);

	writeByte(pdwOutPtr, 0xe8);		// call OpenEvent
	writeDWord(pdwOutPtr, (DWORD)OpenEventA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0x68);		// push INFINITE
	writeDWord(pdwOutPtr, (DWORD)INFINITE);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call WaitForSingleObject
	writeDWord(pdwOutPtr, (DWORD)WaitForSingleObject - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xe8);		// call CloseHandle
	writeDWord(pdwOutPtr, (DWORD)CloseHandle - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x68);		// push pvCompleteEventName
	writeDWord(pdwOutPtr, (DWORD)pvCompleteEventName);

	writeByte(pdwOutPtr, 0x6a);		// push 0
	writeByte(pdwOutPtr, 0);

	writeByte(pdwOutPtr, 0x6a);		// push EVENT_MODIFY_STATE
	writeByte(pdwOutPtr, (BYTE)EVENT_MODIFY_STATE);

	writeByte(pdwOutPtr, 0xe8);		// call OpenEvent
	writeDWord(pdwOutPtr, (DWORD)OpenEventA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x50);		// push eax
	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call SetEvent
	writeDWord(pdwOutPtr, (DWORD)SetEvent - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xe8);		// call CloseHandle
	writeDWord(pdwOutPtr, (DWORD)CloseHandle - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	if (!m_bUseRemoteThread)
	{
		writeByte(pdwOutPtr, 0x6a);		// push 0xfe
		writeByte(pdwOutPtr, 0xfe);

		writeByte(pdwOutPtr, 0xe8);		// call SuspendThread
		writeDWord(pdwOutPtr, (DWORD)SuspendThread - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);
	}

	writeByte(pdwOutPtr, 0x9d);		// popfd
	writeByte(pdwOutPtr, 0x61);		// popad

	if ((!m_bUseRemoteThread) && (!m_bUseAPC))
	{
		writeByte(pdwOutPtr, 0xe9);		// jmp OriginalEIP
		writeDWord(pdwOutPtr, cxt.Eip - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);
	}
	else
	{
		writeByte(pdwOutPtr, 0xc2);		// ret 0x4
		writeWord(pdwOutPtr, 4);
	}

	if (!WriteProcessMemory(hProcess, pvCode, pdwCode, 256, NULL))
		goto failed;

	if (!m_bUseRemoteThread)
	{
		if (m_bUseAPC)
		{
			if (!QueueUserAPC((PAPCFUNC)pvCode, hThread, 0))
				goto failed;
		}
		else
		{
			cxt.Eip = (DWORD)pvCode;
			if (!SetThreadContext(hThread, &cxt))
				goto failed;
		}
	}

	// Initialize a security descriptor.
	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
	// Initialize a security attributes structure.
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	hCompleteEvent = CreateEvent(&sa, FALSE, FALSE, pszCompleteEventName);
	if (!hCompleteEvent)
		goto failed;

	sprintf(pszPipeReadyEventName, "HEATPipeReady%d", m_piProcInfo.dwProcessId);
	hPipeReadyEvent = CreateEvent(&sa, FALSE, FALSE, pszPipeReadyEventName);
	if (!hPipeReadyEvent)
		goto failed;

	hPipeCompleteEvent = CreateEvent(&sa, FALSE, FALSE, pszPipeCompleteEventName);
	if (!hPipeCompleteEvent)
		goto failed;

	if (!m_bUseRemoteThread)
	{
		while (ResumeThread(hThread) > 1)
			dwSuspendCount++;
	}
	else
	{
		hThread = CreateRemoteThread(hProcess, &sa, 0, (LPTHREAD_START_ROUTINE)pvCode, NULL, 0, NULL);
		if (!hThread)
			goto failed;
	}

	if (!m_bUseRemoteThread)
	{
		// Send a message to a window in the thread so it won't get stuck waiting for a message
		hWnd = NULL;
		EnumThreadWindows(m_piProcInfo.dwThreadId, GetFirstWindowEnumProc, (LPARAM)&hWnd);
		if (hWnd)
			PostMessage(hWnd, WM_PAINT, NULL, NULL);
	}

	// Wait for the command pipe to be created
	if (WaitForSingleObject(hPipeReadyEvent, TIMEOUT_LENGTH) == WAIT_TIMEOUT)
	{
		MessageBox(NULL, "Attach to application failed.", "Error", MB_OK | MB_ICONSTOP);
		TerminateProcess(hProcess, -1);
		goto failed;
	}
	CloseHandle(hPipeReadyEvent);  hPipeReadyEvent = NULL;

	// Send all interception commands
	if ((dwErr = sendIntercepts()) != HEAT_SUCCESS)
	{
		SetEvent(hPipeCompleteEvent);
		dwFailureCode = dwErr;
		goto failed;
	}

	// Notify the DLL that we are done sending commands
	SetEvent(hPipeCompleteEvent);
	CloseHandle(hPipeCompleteEvent);  hPipeCompleteEvent = NULL;

	// Wait for injection to complete
	if (WaitForSingleObject(hCompleteEvent, TIMEOUT_LENGTH) == WAIT_TIMEOUT)
	{
		MessageBox(NULL, "Attach to application failed.", "Error", MB_OK | MB_ICONSTOP);
		TerminateProcess(hProcess, -1);
		goto failed;
	}
	CloseHandle(hCompleteEvent);  hCompleteEvent = NULL;

	if (!m_bUseRemoteThread)
	{
		Sleep(500);
		for (; dwSuspendCount; dwSuspendCount--)
			SuspendThread(hThread);
	}
	else
		WaitForSingleObject(hThread, TIMEOUT_LENGTH);

	dwFailureCode = HEAT_SUCCESS;

failed:
	if (pvCompleteEventName) VirtualFreeEx(hProcess, pvCompleteEventName, MAX_PATH, MEM_RELEASE);
	if (pvPipeCompleteEventName) VirtualFreeEx(hProcess, pvPipeCompleteEventName, MAX_PATH, MEM_RELEASE);
	if (pvDLLName) VirtualFreeEx(hProcess, pvDLLName, strlen(pszDLLName) + 1, MEM_RELEASE);
	if (pvInitFuncName) VirtualFreeEx(hProcess, pvInitFuncName, strlen(pszInitFuncName) + 1, MEM_RELEASE);
	if (pvCode) VirtualFreeEx(hProcess, pvCode, 256, MEM_RELEASE);
	if (pdwCode) delete[] pdwCode;
	if (hProcess) CloseHandle(hProcess);
	if (hThread) CloseHandle(hThread);
	if (hCompleteEvent) CloseHandle(hCompleteEvent);
	if (pSD) LocalFree(pSD);
	return dwFailureCode;
}

DWORD CHeatApp::delayedInjectModule(LPSTR pszDLLName)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	char* pszPipeCompleteEventName = NULL;
	char* pszInitFuncName = "init";
	PBYTE pdwCode = NULL;
	PBYTE pdwOutPtr = NULL;
	PVOID pvDLLName = NULL;
	PVOID pvPipeCompleteEventName = NULL;
	PVOID pvInitFuncName = NULL;
	PVOID pvCode = NULL;
	CONTEXT cxt;
	DWORD dwFailureCode = HEAT_FAILED_INJECT_DLL;

	if (m_bUseRemoteThread)
	{
		// Delayed inject not allowed when using CreateRemoteThread method
		goto failed;
	}

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, m_piProcInfo.dwProcessId);
	if (!hProcess)
		goto failed;

	hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, false,
		m_piProcInfo.dwThreadId);
	if (!hThread)
		goto failed;

	ZeroMemory(&cxt, sizeof(cxt));
	cxt.ContextFlags = CONTEXT_FULL;
	if (!GetThreadContext(hThread, &cxt))
		goto failed;

	pvDLLName = allocDataInProc(hProcess, pszDLLName, strlen(pszDLLName) + 1);

	pvInitFuncName = allocDataInProc(hProcess, pszInitFuncName, strlen(pszInitFuncName) + 1);

	pszPipeCompleteEventName = new char[MAX_PATH];
	sprintf(pszPipeCompleteEventName, "HEATPipeComplete%d", m_piProcInfo.dwProcessId);
	pvPipeCompleteEventName = allocDataInProc(hProcess, pszPipeCompleteEventName, MAX_PATH);

	pdwCode = new BYTE[256];
	pvCode = allocDataInProc(hProcess, NULL, 256);

	// Code for injecting DLL:
	//  pushad
	//  pushfd
	//  push pvDLLName
	//  call GetModuleHandle
	//  test eax, eax
	//  jnz AlreadyLoaded
	//  push pvDLLName
	//  call LoadLibrary
	// AlreadyLoaded:
	//  push pvInitFuncName
	//	push eax
	//	call GetProcAddress
	//	call eax
	//  push pvPipeCompleteEventName
	//  push 0
	//  push SYNCHRONIZE
	//  call OpenEvent
	//  push eax
	//  push INFINITE
	//  push eax
	//  call WaitForSingleObject
	//  call CloseHandle
	//  popfd
	//  popad
	//  jmp OriginalEIP

	pdwOutPtr = pdwCode;

	writeByte(pdwOutPtr, 0x60);		// pushad
	writeByte(pdwOutPtr, 0x9c);		// pushfd
	
	writeByte(pdwOutPtr, 0x68);		// push pvDLLName
	writeDWord(pdwOutPtr, (DWORD)pvDLLName);

	writeByte(pdwOutPtr, 0xe8);		// call GetModuleHandle
	writeDWord(pdwOutPtr, (DWORD)GetModuleHandleA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x85);		// test eax, eax
	writeByte(pdwOutPtr, 0xc0);

	writeByte(pdwOutPtr, 0x75);		// jnz AlreadyLoaded
	writeByte(pdwOutPtr, 10);

	writeByte(pdwOutPtr, 0x68);		// push pvDLLName
	writeDWord(pdwOutPtr, (DWORD)pvDLLName);

	writeByte(pdwOutPtr, 0xe8);		// call LoadLibrary
	writeDWord(pdwOutPtr, (DWORD)LoadLibraryA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x68);		// push pvInitFuncName
	writeDWord(pdwOutPtr, (DWORD)pvInitFuncName);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call GetProcAddress
	writeDWord(pdwOutPtr, (DWORD)GetProcAddress - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xff);		// call eax
	writeByte(pdwOutPtr, 0xd0);

	writeByte(pdwOutPtr, 0x68);		// push pvPipeCompleteEventName
	writeDWord(pdwOutPtr, (DWORD)pvPipeCompleteEventName);

	writeByte(pdwOutPtr, 0x6a);		// push 0
	writeByte(pdwOutPtr, 0);

	writeByte(pdwOutPtr, 0x68);		// push SYNCHRONIZE
	writeDWord(pdwOutPtr, (DWORD)SYNCHRONIZE);

	writeByte(pdwOutPtr, 0xe8);		// call OpenEvent
	writeDWord(pdwOutPtr, (DWORD)OpenEventA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0x68);		// push INFINITE
	writeDWord(pdwOutPtr, (DWORD)INFINITE);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call WaitForSingleObject
	writeDWord(pdwOutPtr, (DWORD)WaitForSingleObject - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xe8);		// call CloseHandle
	writeDWord(pdwOutPtr, (DWORD)CloseHandle - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x9d);		// popfd
	writeByte(pdwOutPtr, 0x61);		// popad

	writeByte(pdwOutPtr, 0xe9);		// jmp OriginalEIP
	writeDWord(pdwOutPtr, cxt.Eip - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	if (!WriteProcessMemory(hProcess, pvCode, pdwCode, 256, NULL))
		goto failed;

	CreateThread(NULL, 0, delayedInjectThread, (LPVOID)this, 0, NULL);

	cxt.Eip = (DWORD)pvCode;
	if (!SetThreadContext(hThread, &cxt))
		goto failed;

	dwFailureCode = HEAT_SUCCESS;

failed:
	if (pvDLLName) VirtualFreeEx(hProcess, pvDLLName, strlen(pszDLLName) + 1, MEM_RELEASE);
	if (pvPipeCompleteEventName) VirtualFreeEx(hProcess, pvPipeCompleteEventName, MAX_PATH, MEM_RELEASE);
	if (pvInitFuncName) VirtualFreeEx(hProcess, pvInitFuncName, strlen(pszInitFuncName) + 1, MEM_RELEASE);
	if (pvCode) VirtualFreeEx(hProcess, pvCode, 256, MEM_RELEASE);
	if (pdwCode) delete[] pdwCode;
	if (hProcess) CloseHandle(hProcess);
	if (hThread) CloseHandle(hThread);
	if (pszPipeCompleteEventName) delete[] pszPipeCompleteEventName;
	return dwFailureCode;
}

// Removes a DLL from a process
DWORD CHeatApp::ejectModule(LPSTR pszDLLName)
{
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	char pszCompleteEventName[MAX_PATH];
	char* pszDeinitFuncName = "deinit";
	PBYTE pdwCode = NULL;
	PBYTE pdwOutPtr = NULL;
	PVOID pvCompleteEventName = NULL;
	PVOID pvDLLName = NULL;
	PVOID pvDeinitFuncName = NULL;
	PVOID pvCode = NULL;
	CONTEXT cxt;
	HANDLE hCompleteEvent = NULL;
	DWORD dwSuspendCount = 0;
	DWORD dwFailureCode = HEAT_FAILED_INJECT_DLL;
	DWORD dwThreadId;
	HWND hWnd;
	PSECURITY_DESCRIPTOR pSD = NULL;
	SECURITY_ATTRIBUTES sa;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, false, m_piProcInfo.dwProcessId);
	if (!hProcess)
		goto failed;

	if (!m_bUseRemoteThread)
	{
		dwThreadId = getThreadIdFromProcessId(m_piProcInfo.dwProcessId);
		if (!dwThreadId)
			goto failed;

		hThread = OpenThread(THREAD_GET_CONTEXT | THREAD_SET_CONTEXT | THREAD_SUSPEND_RESUME, false, dwThreadId);
		if (!hThread)
			goto failed;

		ZeroMemory(&cxt, sizeof(cxt));
		cxt.ContextFlags = CONTEXT_FULL;
		if (!GetThreadContext(hThread, &cxt))
			goto failed;
	}

	sprintf(pszCompleteEventName, "HEATEjectComplete%d", m_piProcInfo.dwProcessId);
	pvCompleteEventName = allocDataInProc(hProcess, pszCompleteEventName, MAX_PATH);

	pvDLLName = allocDataInProc(hProcess, pszDLLName, strlen(pszDLLName) + 1);

	pvDeinitFuncName = allocDataInProc(hProcess, pszDeinitFuncName, strlen(pszDeinitFuncName) + 1);

	pdwCode = new BYTE[256];
	pvCode = allocDataInProc(hProcess, NULL, 256);

	// Code for ejecting DLL:
	//  pushad
	//  pushfd
	//  push pvDLLName
	//  call GetModuleHandle
	//  test eax, eax
	//  jz AlreadyUnloaded
	//	push pvDeinitFuncName
	//  push eax
	//	call GetProcAddress
	//  call eax
	// AlreadyUnloaded:
	//  push pvCompleteEventName
	//  push 0
	//  push EVENT_MODIFY_STATE
	//  call OpenEvent
	//  push eax
	//  push eax
	//  call SetEvent
	//  call CloseHandle
	//  push 0xfe  ; current thread
	//  call SuspendThread
	//  popfd
	//  popad
	//  jmp OriginalEIP

	pdwOutPtr = pdwCode;

	writeByte(pdwOutPtr, 0x60);		// pushad
	writeByte(pdwOutPtr, 0x9c);		// pushfd
	
	writeByte(pdwOutPtr, 0x68);		// push pvDLLName
	writeDWord(pdwOutPtr, (DWORD)pvDLLName);

	writeByte(pdwOutPtr, 0xe8);		// call GetModuleHandle
	writeDWord(pdwOutPtr, (DWORD)GetModuleHandleA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x85);		// test eax, eax
	writeByte(pdwOutPtr, 0xc0);

	writeByte(pdwOutPtr, 0x74);		// jz AlreadyLoaded
	writeByte(pdwOutPtr, 13);

	writeByte(pdwOutPtr, 0x68);		// push pvDeinitFuncName
	writeDWord(pdwOutPtr, (DWORD)pvDeinitFuncName);

	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call GetProcAddress
	writeDWord(pdwOutPtr, (DWORD)GetProcAddress - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xff);		// call eax
	writeByte(pdwOutPtr, 0xd0);

	writeByte(pdwOutPtr, 0x68);		// push pvCompleteEventName
	writeDWord(pdwOutPtr, (DWORD)pvCompleteEventName);

	writeByte(pdwOutPtr, 0x6a);		// push 0
	writeByte(pdwOutPtr, 0);

	writeByte(pdwOutPtr, 0x6a);		// push EVENT_MODIFY_STATE
	writeByte(pdwOutPtr, (BYTE)EVENT_MODIFY_STATE);

	writeByte(pdwOutPtr, 0xe8);		// call OpenEvent
	writeDWord(pdwOutPtr, (DWORD)OpenEventA - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0x50);		// push eax
	writeByte(pdwOutPtr, 0x50);		// push eax

	writeByte(pdwOutPtr, 0xe8);		// call SetEvent
	writeDWord(pdwOutPtr, (DWORD)SetEvent - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	writeByte(pdwOutPtr, 0xe8);		// call CloseHandle
	writeDWord(pdwOutPtr, (DWORD)CloseHandle - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);

	if (!m_bUseRemoteThread)
	{
		writeByte(pdwOutPtr, 0x6a);		// push 0xfe
		writeByte(pdwOutPtr, 0xfe);

		writeByte(pdwOutPtr, 0xe8);		// call SuspendThread
		writeDWord(pdwOutPtr, (DWORD)SuspendThread - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);
	}

	writeByte(pdwOutPtr, 0x9d);		// popfd
	writeByte(pdwOutPtr, 0x61);		// popad

	if (!m_bUseRemoteThread)
	{
		writeByte(pdwOutPtr, 0xe9);		// jmp OriginalEIP
		writeDWord(pdwOutPtr, cxt.Eip - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);
	}
	else
	{
		writeByte(pdwOutPtr, 0x6a);		// push 0
		writeByte(pdwOutPtr, 0x00);

		writeByte(pdwOutPtr, 0xe8);		// call ExitThread
		writeDWord(pdwOutPtr, (DWORD)ExitThread - (DWORD)pvCode - (pdwOutPtr-pdwCode) - 4);
	}

	if (!WriteProcessMemory(hProcess, pvCode, pdwCode, 256, NULL))
		goto failed;

	if (!m_bUseRemoteThread)
	{
		cxt.Eip = (DWORD)pvCode;
		if (!SetThreadContext(hThread, &cxt))
			goto failed;
	}

	// Initialize a security descriptor.
	pSD = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
	// Initialize a security attributes structure.
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	hCompleteEvent = CreateEvent(&sa, FALSE, FALSE, pszCompleteEventName);
	if (!hCompleteEvent)
		goto failed;

	if (!m_bUseRemoteThread)
	{
		while (ResumeThread(hThread) > 1)
			dwSuspendCount++;
	}
	else
	{
		hThread = CreateRemoteThread(hProcess, &sa, 0, (LPTHREAD_START_ROUTINE)pvCode, NULL, 0, NULL);
		if (!hThread)
			goto failed;
	}

	if (!m_bUseRemoteThread)
	{
		// Send a message to a window in the thread so it won't get stuck waiting for a message
		hWnd = NULL;
		EnumThreadWindows(dwThreadId, GetFirstWindowEnumProc, (LPARAM)&hWnd);
		if (hWnd)
			PostMessage(hWnd, WM_PAINT, NULL, NULL);
	}

	// Wait for ejection to complete
	if (WaitForSingleObject(hCompleteEvent, TIMEOUT_LENGTH) == WAIT_TIMEOUT)
	{
		MessageBox(NULL, "Detach from application failed.  Application will be terminated.", "Error", MB_OK | MB_ICONSTOP);
		TerminateProcess(hProcess, -1);
		goto failed;
	}

	CloseHandle(hCompleteEvent);  hCompleteEvent = NULL;

	if (!m_bUseRemoteThread)
	{
		Sleep(500);
		for (; dwSuspendCount; dwSuspendCount--)
			SuspendThread(hThread);
	}
	else
		WaitForSingleObject(hThread, TIMEOUT_LENGTH);

	dwFailureCode = HEAT_SUCCESS;

failed:
	if (pvCompleteEventName) VirtualFreeEx(hProcess, pvCompleteEventName, MAX_PATH, MEM_RELEASE);
	if (pvDLLName) VirtualFreeEx(hProcess, pvDLLName, strlen(pszDLLName) + 1, MEM_RELEASE);
	if (pvDeinitFuncName) VirtualFreeEx(hProcess, pvDeinitFuncName, strlen(pszDeinitFuncName) + 1, MEM_RELEASE);
	if (pvCode) VirtualFreeEx(hProcess, pvCode, 256, MEM_RELEASE);
	if (pdwCode) delete[] pdwCode;
	if (hProcess) CloseHandle(hProcess);
	if (hThread) CloseHandle(hThread);
	if (hCompleteEvent) CloseHandle(hCompleteEvent);
	if (pSD) LocalFree(pSD);
	return dwFailureCode;
}

// Injects the HEAT DLL into the process
DWORD CHeatApp::forceInject()
{
	if (!m_bReady)
		return HEAT_NOT_READY;
	if (m_bInjected)
		return HEAT_ALREADY_INJECTED;

	PSECURITY_DESCRIPTOR pSD;
	SECURITY_ATTRIBUTES sa;

	pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
	// Initialize a security attributes structure.
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	char eventName[30];
	_snprintf(eventName, 30, "HEATInjectionDone_%d", this->m_piProcInfo.dwProcessId);
	HANDLE injectionCompleteEvent = CreateEvent(&sa, TRUE, FALSE, eventName);

	LocalFree(pSD);

	if (injectionCompleteEvent == INVALID_HANDLE_VALUE)
		return HEAT_FAILED_CREATE_EVENT;

	DWORD dwErr;
	bool bResumeWhenDone = false;

	if ((dwErr = injectModule("Heat4Dll.dll")) != HEAT_SUCCESS)
		return dwErr;

	m_bInjected = true;
	if (!SetEvent(injectionCompleteEvent))
		return HEAT_FAILED_SET_EVENT;

	return HEAT_SUCCESS;
}

// Injects the HEAT DLL into an uninitialized process
DWORD CHeatApp::delayedInject()
{
	if (!m_bReady)
		return HEAT_NOT_READY;
	if (m_bInjected)
		return HEAT_ALREADY_INJECTED;

	if (m_bUseRemoteThread)
	{
		// Delayed inject not allowed when using CreateRemoteThread method
		return forceInject();
	}

	DWORD dwErr;
	bool bResumeWhenDone = false;

	if ((dwErr = delayedInjectModule("Heat4Dll.dll")) != HEAT_SUCCESS)
		return dwErr;

	m_bInjected = true;

	return HEAT_SUCCESS;
}


// Beings execution of the process
DWORD CHeatApp::runApp()
{
	if (!m_bReady)
		return HEAT_NOT_READY;
	if (m_bRunning)
		return HEAT_ALREADY_RUNNING;

	DWORD dwErr;

	if ((dwErr = resumeApp()) != HEAT_SUCCESS)
		return dwErr;

	return HEAT_SUCCESS;
}

// Suspends execution of the process
DWORD CHeatApp::pauseApp()
{
	if (!m_bReady)
		return HEAT_NOT_READY;
	if (!m_bRunning)
		return HEAT_ALREADY_PAUSED;

	if (modifyProcessState(m_piProcInfo.dwProcessId, true))
	{
		m_bRunning = false;
		char *pszEventName = new char[MAX_PATH];
		sprintf(pszEventName, "ProcessPausedEvent%d", getProcessId());
		HANDLE eventHandle = CreateEvent(NULL, false, false, pszEventName);
		SetEvent(eventHandle);
		delete[] pszEventName;
		return HEAT_SUCCESS;
	}
	return HEAT_FAILED_PAUSE_PROCESS;
}

// For externally setting the pause state of the application
DWORD CHeatApp::setPauseState(bool value)
{
	if (m_bRunning != value)
		return HEAT_SUCCESS;

	m_bRunning = !value;
	char *pszEventName = new char[MAX_PATH];
	if (m_bRunning)
		sprintf(pszEventName, "ProcessResumedEvent%d", getProcessId());
	else
		sprintf(pszEventName, "ProcessPausedEvent%d", getProcessId());

	HANDLE eventHandle = CreateEvent(NULL, false, false, pszEventName);
	SetEvent(eventHandle);
	delete[] pszEventName;

	return HEAT_SUCCESS;
}

// Continues execution of the process
DWORD CHeatApp::resumeApp()
{
	if (!m_bReady)
		return HEAT_NOT_READY;
	if (m_bRunning)
		return HEAT_ALREADY_RUNNING;

	if (modifyProcessState(m_piProcInfo.dwProcessId, false))
	{
		m_bRunning = true;
		char *pszEventName = new char[MAX_PATH];
		sprintf(pszEventName, "ProcessResumedEvent%d", getProcessId());
		HANDLE eventHandle = CreateEvent(NULL, false, false, pszEventName);
		SetEvent(eventHandle);
		delete[] pszEventName;
		return HEAT_SUCCESS;
	}
	return HEAT_FAILED_RESUME_PROCESS;
}

// Terminates execution of the process
DWORD CHeatApp::terminateApp()
{
	if (!m_bReady)
		return HEAT_NOT_READY;

	HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, false, m_piProcInfo.dwProcessId);
	if (!hProcess)
		return HEAT_FAILED_TERMINATE_PROCESS;

	if (!TerminateProcess(hProcess, 0))
	{
		CloseHandle(hProcess);
		return HEAT_FAILED_TERMINATE_PROCESS;
	}

	CloseHandle(hProcess);

	m_ciIntercepts.markAllUnsent();

	m_bReady = false;
	m_bRunning = false;
	m_bInjected = false;

	return HEAT_SUCCESS;
}


// Begins intercepting the function given by pszFuncToIntercept, rerouting calls to the function given by pszHandlerFunc
DWORD CHeatApp::interceptFunc(LPSTR pszFuncToIntercept, LPSTR pszFuncDLL, LPSTR pszHandlerFunc, LPSTR pszHandlerDLL)
{
	if (!pszFuncToIntercept)
		return HEAT_INVALID_PARAMETERS;
	if (!pszFuncDLL)
		return HEAT_INVALID_PARAMETERS;
	if (!pszHandlerFunc)
		return HEAT_INVALID_PARAMETERS;
	if (!pszHandlerDLL)
		return HEAT_INVALID_PARAMETERS;

#ifndef _DEBUG
	if (strncmp(pszFuncToIntercept, "0x", 2) != 0) // ignore direct addresses
	{
		HMODULE hFuncMod = LoadLibraryEx(pszFuncDLL, NULL, DONT_RESOLVE_DLL_REFERENCES);
		if (!hFuncMod)
			return HEAT_FAILED_INTERCEPT;

		if (pszFuncToIntercept[0] != '+') // ignore offsets from base
		{
			PVOID pFunc = GetProcAddress(hFuncMod, pszFuncToIntercept);
			FreeLibrary(hFuncMod);
			if (!pFunc)
				return HEAT_FAILED_INTERCEPT;
		}
		else
			FreeLibrary(hFuncMod);
	}

	char szHandlerFuncName[MAX_PATH];
	strncpy(szHandlerFuncName, pszHandlerFunc, MAX_PATH);
	szHandlerFuncName[MAX_PATH - 1] = 0; // ensure NULL terminator

	HMODULE hHandlerMod = LoadLibraryEx(pszHandlerDLL, NULL, DONT_RESOLVE_DLL_REFERENCES);
	if (!hHandlerMod)
		return HEAT_FAILED_INTERCEPT;
	PVOID pHandler = GetProcAddress(hHandlerMod, szHandlerFuncName);

	if (!pHandler)
	{
		if (findMangledNameInModule(hHandlerMod, szHandlerFuncName, MAX_PATH))
			pHandler = GetProcAddress(hHandlerMod, szHandlerFuncName);
	}

	FreeLibrary(hHandlerMod);
	if (!pHandler)
		return HEAT_FAILED_INTERCEPT;
#endif

	if (m_ciIntercepts.getIntercept(pszFuncToIntercept, pszFuncDLL))
	{
		CNode *pNode = m_ciIntercepts.getIntercept(pszFuncToIntercept, pszFuncDLL);
		if (pNode->getDeleted())
			m_ciIntercepts.removeIntercept(pszFuncToIntercept, pszFuncDLL);	// go ahead and delete the old entry
		else
			return HEAT_ALREADY_INTERCEPTED;
	}

#ifndef _DEBUG
	if (!m_ciIntercepts.addIntercept(pszFuncToIntercept, pszFuncDLL, szHandlerFuncName, pszHandlerDLL))
		return HEAT_FAILED_INTERCEPT;
#else
	if (!m_ciIntercepts.addIntercept(pszFuncToIntercept, pszFuncDLL, pszHandlerFunc, pszHandlerDLL))
		return HEAT_FAILED_INTERCEPT;
#endif

	if ((m_bReady) && (m_bInjected))
	{
		DWORD dwErr;
		if ((dwErr = sendIntercepts()) != HEAT_SUCCESS)
			return dwErr;
	}

	return HEAT_SUCCESS;
}

DWORD CHeatApp::interceptFunc(PVOID pFuncToIntercept, LPSTR pszHandlerFunc, LPSTR pszHandlerDLL)
{
	char pszAddrString[32];

	sprintf(pszAddrString, "0x%X", pFuncToIntercept);
	return interceptFunc(pszAddrString, "", pszHandlerFunc, pszHandlerDLL);
}

// Stops intercepting the function given by pszFuncName
DWORD CHeatApp::unInterceptFunc(LPSTR pszFuncName, LPSTR pszFuncDLL)
{
	if (!pszFuncName)
		return HEAT_INVALID_PARAMETERS;
	if (!pszFuncDLL)
		return HEAT_INVALID_PARAMETERS;

	CNode *pNode = m_ciIntercepts.getIntercept(pszFuncName, pszFuncDLL);
	if (!pNode)
		return HEAT_FAILED_UNINTERCEPT;

	if ((m_bReady) && (m_bInjected))
	{
		if (m_bRunning)
		{
			if (pNode->getSent())
			{
				DWORD dwErr;
				if ((dwErr = sendUnintercept(pszFuncName, pszFuncDLL)) != HEAT_SUCCESS)
					return dwErr;
			}
		}
		else
		{
			if (pNode->getSent())
				pNode->setDeleted(true);
			else
				m_ciIntercepts.removeIntercept(pszFuncName, pszFuncDLL);

			return HEAT_SUCCESS;
		}
	}

	m_ciIntercepts.removeIntercept(pszFuncName, pszFuncDLL);

	return HEAT_SUCCESS;
}

DWORD CHeatApp::unInterceptFunc(PVOID pFuncToIntercept)
{
	char pszAddrString[32];

	sprintf(pszAddrString, "0x%X", pFuncToIntercept);
	return unInterceptFunc(pszAddrString, "");
}

DWORD CHeatApp::sendUnintercept(LPSTR pszFuncName, LPSTR pszFuncDLL)
{
	char pszPipeName[MAX_PATH];
	sprintf(pszPipeName, "\\\\.\\pipe\\HEATDataPipe%d", m_piProcInfo.dwProcessId);
	if (!WaitNamedPipe(pszPipeName, NMPWAIT_WAIT_FOREVER))
		return HEAT_FAILED_INTERCEPT;
	HANDLE hFile = CreateFile(pszPipeName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return HEAT_FAILED_INTERCEPT;

	BYTE ucCommand = CMD_UNINTERCEPT;
	DWORD dwBytes, dwStrLen;

	WriteFile(hFile, &ucCommand, 1, &dwBytes, NULL);
	if (dwBytes != 1)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_UNINTERCEPT;
	}

	dwStrLen = strlen(pszFuncName);
	WriteFile(hFile, &dwStrLen, 4, &dwBytes, NULL);
	if (dwBytes != 4)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_INTERCEPT;
	}
	WriteFile(hFile, pszFuncName, dwStrLen, &dwBytes, NULL);
	if (dwBytes != dwStrLen)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_INTERCEPT;
	}

	dwStrLen = strlen(pszFuncDLL);
	WriteFile(hFile, &dwStrLen, 4, &dwBytes, NULL);
	if (dwBytes != 4)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_INTERCEPT;
	}
	WriteFile(hFile, pszFuncDLL, dwStrLen, &dwBytes, NULL);
	if (dwBytes != dwStrLen)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_INTERCEPT;
	}

	char pszProcessedEventName[MAX_PATH];
	sprintf(pszProcessedEventName, "HEATCmdProcessed%d", m_piProcInfo.dwProcessId);
	HANDLE hProcessedEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, pszProcessedEventName);
	WaitForSingleObject(hProcessedEvent, INFINITE);
	CloseHandle(hProcessedEvent);

	CloseHandle(hFile);

	return HEAT_SUCCESS;
}

// Calls deinit if the process specified terminates
DWORD CHeatApp::deInitOnProcessTerminate(DWORD pid)
{
	char pszPipeName[MAX_PATH];
	sprintf(pszPipeName, "\\\\.\\pipe\\HEATDataPipe%d", m_piProcInfo.dwProcessId);
	if (!WaitNamedPipe(pszPipeName, NMPWAIT_WAIT_FOREVER))
		return HEAT_FAILED_SEND_DEINIT;
	HANDLE hFile = CreateFile(pszPipeName, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return HEAT_FAILED_SEND_DEINIT;

	BYTE ucCommand = CMD_DEINIT_ON_PROCESS_TERMINATE;
	DWORD dwBytes;

	WriteFile(hFile, &ucCommand, 1, &dwBytes, NULL);
	if (dwBytes != 1)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_SEND_DEINIT;
	}

	WriteFile(hFile, &pid, 4, &dwBytes, NULL);
	if (dwBytes != 4)
	{
		CloseHandle(hFile);
		return HEAT_FAILED_SEND_DEINIT;
	}

	char pszProcessedEventName[MAX_PATH];
	sprintf(pszProcessedEventName, "HEATCmdProcessed%d", m_piProcInfo.dwProcessId);
	HANDLE hProcessedEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, pszProcessedEventName);
	WaitForSingleObject(hProcessedEvent, INFINITE);
	CloseHandle(hProcessedEvent);

	CloseHandle(hFile);

	return HEAT_SUCCESS;
}

// Returns true if the application is ready to run/intercept
bool CHeatApp::isReady()
{
	return m_bReady;
}

// Returns true if the application is currently running, false otherwise
bool CHeatApp::isRunning()
{
	return m_bRunning;
}

// Returns the process ID of the current process
DWORD CHeatApp::getProcessId()
{
	return m_piProcInfo.dwProcessId;
}

// Returns the thread ID of the current process
DWORD CHeatApp::getThreadId()
{
	return m_piProcInfo.dwThreadId;
}

DWORD CHeatApp::getThreadsInCurrentProcess(DWORD *threadList)
{
	int threadCount = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);	
	if (!hSnap)
		return 0;

	THREADENTRY32 threadInfo = {0};
	threadInfo.dwSize = sizeof(threadInfo);

	if (!Thread32First(hSnap, &threadInfo))
	{
		CloseHandle(hSnap);
		return 0;
	}

	do
	{
		if (threadInfo.th32OwnerProcessID == m_piProcInfo.dwProcessId)
		{
			if (threadList)
				threadList[threadCount] = threadInfo.th32ThreadID;
			threadCount++;
		}
		
		ZeroMemory(&threadInfo, sizeof(threadInfo));
		threadInfo.dwSize = sizeof(threadInfo);
	} while (Thread32Next(hSnap, &threadInfo));

	CloseHandle(hSnap);
	return threadCount;

}

void CHeatApp::startMonitorThread()
{
	CreateThread(NULL, 0, MonitorThreadProc, this, 0, NULL);
}

DWORD WINAPI MonitorThreadProc(LPVOID lpParameter)
{
	CHeatApp *pHeatApi = (CHeatApp*) lpParameter;
	WaitForSingleObject(pHeatApi->getProcessHandle(), INFINITE);
	
	char *pszEventName = new char[MAX_PATH];
	sprintf(pszEventName, "ProcessTerminatedEvent%d", pHeatApi->getProcessId());
	HANDLE eventHandle = CreateEvent(NULL, false, false, pszEventName);
	SetEvent(eventHandle);
	pHeatApi->setIsRunning(false);
	pHeatApi->setIsReady(false);
	pHeatApi->setIsInjected(false);

	delete[] pszEventName;
	return 0;
}
