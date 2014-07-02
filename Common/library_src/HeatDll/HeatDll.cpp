//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HeatDll.cpp
//
// DESCRIPTION: Contains the AUT side implementation of HEAT technology
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Sep 2003		 R. Wagner	 HEAT revision 4 (Heat4Dll)
//*************************************************************************

#include <windows.h>
#include <Tlhelp32.h>
#include <stdio.h>
#include <psapi.h>
#include "CopyInstr.h"

#ifdef _DEBUG
// Debug mode causes problems with code generation on some versions
#error DO NOT compile Debug.  Please compile Release.
#endif

// Pipe processing commands
#define CMD_INTERCEPT 1
#define CMD_UNINTERCEPT 2
#define CMD_DEINIT_ON_PROCESS_TERMINATE 3

// Structure representing a replacement library
struct Module
{
	HMODULE hMod;
	bool bInit;
	Module* pNext;
};


// Structure representing an intercepted function
struct Intercept
{
	PVOID pOrigFunc;		// pointer to original function being intercepted
	PVOID pHandlerFunc;		// pointer to handler for the function

	PBYTE pOrigHeader;		// pointer to the first 5+ bytes of the original function
	DWORD dwHeaderBytes;

	PBYTE pRelocHeader;		// pointer to the relocated first 5+ bytes of the function, call this to execute the original
	PBYTE pJumpTable;		// pointer to push pIntercept / jmp impFunc instructions for this intercept

	PBYTE pCallerFunc;		// pointer to reentrant-compatible caller function

	Intercept* pNext;
};


// Represents a stack of return addresses for reentrant interception
struct RetAddrStack
{
	DWORD dwCurIndex, dwMaxIndex;
	PVOID* pStack;
};

// a node in the linked list of threads created in heat
struct CreatedThread
{
	DWORD dwThreadID;
	CreatedThread *pNext;
};


// Global variables
Intercept* g_pFirstIntercept = NULL;
Module* g_pFirstModule = NULL;
CreatedThread *g_pFirstCreatedThread = NULL;
CreatedThread *g_pLastCreatedThread = NULL;
DWORD g_dwCreatedThreadCount = 0;

CRITICAL_SECTION g_csThreadCreated;

HANDLE g_hDataPipe = INVALID_HANDLE_VALUE;
HANDLE g_hProcessedEvent = INVALID_HANDLE_VALUE;
HANDLE g_hPipeThread = NULL;
HANDLE g_hHeap = NULL;
HMODULE g_hHeatModule = NULL;

BYTE* g_pFreeHeatDllCode = NULL; // MUST be allocated on process heap, not HeatDll local heap

DWORD g_dwTlsIsCalled;
DWORD g_dwCallLevel;
DWORD g_dwOrigRetAddrStack;
DWORD g_dwCallerRetAddrStack;
DWORD g_dwCalledAddr;

DWORD g_dwReplacementFunctionRefCount = 0;
BOOL g_bIsUninjecting = FALSE;

BOOL g_bInit = FALSE;




// Use caller functions for calling system functions in case the user intercepts them

// Remove the HeapAllocCaller fptr; instead replace it with a stub that
// calls the 'real' HeapAllocCaller fptr
//LPVOID (__stdcall *HeapAllocCaller)(HANDLE, DWORD, SIZE_T) = HeapAlloc;

LPVOID (__stdcall *HeapAllocCaller_real)(HANDLE, DWORD, SIZE_T) = HeapAlloc;
BOOL (__stdcall *HeapFreeCaller)(HANDLE, DWORD, LPVOID) = HeapFree;
LPVOID (__stdcall *TlsGetValueCaller)(DWORD) = TlsGetValue;
BOOL (__stdcall *TlsSetValueCaller)(DWORD, LPVOID) = TlsSetValue;
DWORD (__stdcall *GetLastErrorCaller)() = GetLastError;
void (__stdcall *SetLastErrorCaller)(DWORD) = SetLastError;


// HACK: Make all memory allocated execute and readwrite; need this in place for now
//       to get rid of the DEP bug. Eventually we need to figure out exactly which allocations
//       are for code and which for data.
LPVOID __stdcall HeapAllocCaller(HANDLE hHeap, DWORD dwFlags, SIZE_T size)
{
	LPVOID lpMem = HeapAllocCaller_real(hHeap, dwFlags, size);
	DWORD dwOld;
	VirtualProtect(lpMem, size, PAGE_EXECUTE_READWRITE, &dwOld);
	return lpMem;
}


bool deInitOnProcessTerminate(DWORD pid);


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

//*************************************************************************
// Method:		TlsGetValueWrapper
// Description: Performs same task as TlsGetValue but safely
//
// Parameters:
//	index - storage index to return
//
// Return Value: value that was in the index
//*************************************************************************
LPVOID __stdcall TlsGetValueWrapper(DWORD index)
{
	LPVOID retVal;
	if (index >= 64)
	{
		retVal = TlsGetValueCaller(index);
	}
	else
	{
		_asm
		{
//			mov eax, index
//			mov eax, fs:[0xe10 + eax*4]

			mov	eax, fs:[0x18]
			mov	ecx, index
			mov	eax, [eax+ecx*4+0xE10]
			//and	fs:[0x34], 0
			mov	[retVal], eax
		}
	}
	return retVal;
}

//*************************************************************************
// Method:		IsHeatCreatedThread
// Description: determines whether a thread was created by heat
//
// Parameters:
//	dwThreadID - the thread id to check
//
// Return Value: true if the thread was created by heat, false otherwise
//*************************************************************************
bool __stdcall IsHeatCreatedThread(DWORD dwThreadID)
{
	EnterCriticalSection(&g_csThreadCreated);

	CreatedThread *node = g_pFirstCreatedThread;
	while (node)
	{
		if (node->dwThreadID == dwThreadID)
		{
			LeaveCriticalSection(&g_csThreadCreated);
			return true;
		}
		node = node->pNext;
	}

	LeaveCriticalSection(&g_csThreadCreated);
	return false;
}

//*************************************************************************
// Method:		HeatCreateThread
// Description: creates a thread and logs it as created by heat
//
// Parameters:
//	routine - the thread routine to execute
//	param - the parameter to pass to the thread function
//	pdwThreadID (out) - the thread id of the new thread
//
// Return Value: the handle to the created thread
//*************************************************************************
HANDLE __stdcall HeatCreateThread(LPTHREAD_START_ROUTINE routine, LPVOID param, DWORD *pdwThreadID)
{
	EnterCriticalSection(&g_csThreadCreated);

	// Initialize a security descriptor.
	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
	// Initialize a security attributes structure.
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	DWORD threadID;
	HANDLE thread = CreateThread(&sa, 0, routine, param, CREATE_SUSPENDED, &threadID);

	CreatedThread *newThread = (CreatedThread *)HeapAllocCaller(g_hHeap, HEAP_ZERO_MEMORY, sizeof(CreatedThread));
	if (!newThread)
	{
		ResumeThread(thread);
		LeaveCriticalSection(&g_csThreadCreated);
		return thread;
	}

	newThread->dwThreadID = threadID;
	newThread->pNext = NULL;

	if (pdwThreadID)
		*pdwThreadID = threadID;

	if (!g_pFirstCreatedThread)
		g_pFirstCreatedThread = newThread;

	if (g_pLastCreatedThread)
		g_pLastCreatedThread->pNext = newThread;
	g_pLastCreatedThread = newThread;
	g_dwCreatedThreadCount++;

	ResumeThread(thread);
	LeaveCriticalSection(&g_csThreadCreated);
	return thread;
}

//*************************************************************************
// Method:		unloadHandlerModules
// Description: Unloads the replacement library DLLs.  Do not call until
//				g_dwReplacementFunctionRefCount becomes zero.
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void unloadHandlerModules()
{
	// Free all handler modules
	Module* pModule = g_pFirstModule;
	while (pModule)
	{
		FreeLibrary(pModule->hMod);
		Module* pNext = pModule->pNext;
		HeapFree(g_hHeap, 0, pModule);
		pModule = pNext;
	}
	g_pFirstModule = NULL;
}


//*************************************************************************
// Method:		pushOntoRetAddrStack
// Description: Push a return address onto the return address stack
//
// Parameters:
//	dwStackTlsIndex - TLS index pointing to the stack for this thread
//  pAddr - Address to push
//
// Return Value: None
//*************************************************************************
void __stdcall pushOntoRetAddrStack(DWORD dwStackTlsIndex, PVOID pAddr)
{
	RetAddrStack* pStack = (RetAddrStack*)TlsGetValueWrapper(dwStackTlsIndex);
	if (!pStack)
	{
		// No stack allocated yet, so create one
		pStack = (RetAddrStack*)HeapAllocCaller(g_hHeap, 0, sizeof(RetAddrStack));
		pStack->dwCurIndex = 0;
		pStack->dwMaxIndex = 64;
		pStack->pStack = (PVOID*)HeapAllocCaller(g_hHeap, 0, pStack->dwMaxIndex * sizeof(PVOID));
		TlsSetValueCaller(dwStackTlsIndex, pStack);
	}

	if (pStack->dwCurIndex >= pStack->dwMaxIndex)
	{
		// Not enough space, so allocate more
		DWORD dwNewSize = pStack->dwMaxIndex + 64;
		PVOID* pNewStack = (PVOID*)HeapAllocCaller(g_hHeap, 0, dwNewSize * sizeof(PVOID));
		memcpy(pNewStack, pStack->pStack, pStack->dwCurIndex * sizeof(PVOID));
		HeapFreeCaller(g_hHeap, 0, pStack->pStack);
		pStack->pStack = pNewStack;
		pStack->dwMaxIndex = dwNewSize;
	}

	pStack->pStack[pStack->dwCurIndex++] = pAddr;
}


//*************************************************************************
// Method:		popOffRetAddrStack
// Description: Pop a return address off the return address stack
//
// Parameters:
//	dwStackTlsIndex - TLS index pointing to the stack for this thread
//
// Return Value: Return address popped off
//*************************************************************************
PVOID __stdcall popOffRetAddrStack(DWORD dwStackTlsIndex)
{
	RetAddrStack* pStack = (RetAddrStack*)TlsGetValueWrapper(dwStackTlsIndex);
	return pStack->pStack[--pStack->dwCurIndex];
}


//*************************************************************************
// Method:		peekRetAddrStack
// Description: Returns the return address on the top of the return
//				address stack, but does not pop it off
//
// Parameters:
//	dwStackTlsIndex - TLS index pointing to the stack for this thread
//
// Return Value: Return address popped off
//*************************************************************************
PVOID __stdcall peekRetAddrStack(DWORD dwStackTlsIndex)
{
	RetAddrStack* pStack = (RetAddrStack*)TlsGetValue(dwStackTlsIndex);
	return pStack->pStack[pStack->dwCurIndex - 1];
}


//*************************************************************************
// Method:		impFunc
// Description: The "imposter function".  This function is called in
//				place of the original, intercepted function with a
//				pointer to its associated Intercept structure on the
//				top of the stack.
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void __declspec(naked) impFunc()
{
	__asm
	{
		pushfd
		pushad

		// Preserve last error
		call dword ptr [GetLastErrorCaller]
		push eax

		push g_dwTlsIsCalled
		call TlsGetValueWrapper
		
		test eax, eax
		jnz returnOut

		lock inc g_dwReplacementFunctionRefCount

		// Increment IsCalled
		push g_dwTlsIsCalled
		call TlsGetValueWrapper
		inc eax
		push eax
		push g_dwTlsIsCalled
		call dword ptr [TlsSetValueCaller]

		// Increment CallLevel
		push g_dwCallLevel
		call TlsGetValueWrapper
		inc eax
		push eax
		push g_dwCallLevel
		call dword ptr [TlsSetValueCaller]

		// Save old return address in thread local storage
		mov eax, [esp+44]	// old return address
		push eax
		push g_dwOrigRetAddrStack
		call pushOntoRetAddrStack

		// Save called address in thread local storage
		mov eax, [esp+40]	// retrieve pIntercepts pointer off stack
		mov eax, [eax]Intercept.pOrigFunc
		push eax
		push g_dwCalledAddr
		call dword ptr [TlsSetValueCaller]

		mov eax, offset returnFromHandlerFunc
		mov [esp+44], eax		// set return address to return here after handler function is done

		mov eax, [esp+40]	// retrieve pIntercepts pointer off stack
		mov eax, [eax]Intercept.pHandlerFunc
		mov [esp+40], eax	// put contents of pHandlerFunc on top of stack

		// saved last error is on top of stack
		pop dword ptr fs:[0x34]
		popad
		popfd
		ret					// go to handler function, will return to returnFromHandlerFunc label

	returnFromHandlerFunc:
		pushfd
		pushfd				// push twice, first will be used for space on stack for return address

		pushad

		// Preserve last error
		call dword ptr [GetLastErrorCaller]
		push eax

		// Decrement CallLevel
		push g_dwCallLevel
		call TlsGetValueWrapper
		dec eax
		push eax
		push g_dwCallLevel
		call dword ptr [TlsSetValueCaller]

		// Decrement IsCalled
		push g_dwTlsIsCalled
		call TlsGetValueWrapper
		dec eax
		push eax
		push g_dwTlsIsCalled
		call dword ptr [TlsSetValueCaller]
		
		// Restore original return address
		push g_dwOrigRetAddrStack
		call popOffRetAddrStack
		mov [esp+40], eax	// put original return address on top of stack

		lock dec g_dwReplacementFunctionRefCount
		jnz notUninjecting

		cmp g_bIsUninjecting, 0
		jz notUninjecting

		call unloadHandlerModules
		push g_hHeap
		call dword ptr [HeapDestroy]

		// saved last error is on top of stack
		pop dword ptr fs:[0x34]
		popad
		popfd

		// Need to free HeatDll (make sure we return to caller, not HeatDll, when done)
		push g_pFreeHeatDllCode
		ret

	notUninjecting:
		// saved last error is on top of stack
		pop dword ptr fs:[0x34]
		popad
		popfd

		ret					// return to original caller

	returnOut:
		mov eax, [esp+40]	// retrieve pIntercepts pointer off stack
		mov eax, [eax+16]	// access pRelocHeader member
		mov [esp+40], eax	// put contents of pRelocHeader on top of stack

		// saved last error is on top of stack
		pop dword ptr fs:[0x34]
		popad
		popfd
		ret					// go to original function
	}
}


//*************************************************************************
// Method:		callerFunc
// Description: The "original function caller".  This will decrement
//				the "is called" counter to allow other functions to be
//				intercepted, but execute the original, unintercepted
//				form of the current function.  This allows for
//				reentrant interception.  The pointer to the Intercept
//				structure for this function will be on the top of
//				the stack.
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void __declspec(naked) callerFunc()
{
	__asm
	{
		pushfd
		pushad

		// Preserve last error
		call dword ptr [GetLastErrorCaller]
		push eax

		// Save old return address in thread local storage
		mov eax, [esp+44]	// old return address
		push eax
		push g_dwCallerRetAddrStack
		call pushOntoRetAddrStack

		// Decrement IsCalled
		push g_dwTlsIsCalled
		call TlsGetValueWrapper
		dec eax
		push eax
		push g_dwTlsIsCalled
		call dword ptr [TlsSetValueCaller]

		mov eax, offset returnFromOriginalFunc
		mov [esp+44], eax		// set return address to return here after handler function is done

		mov eax, [esp+40]	// retrieve pIntercepts pointer off stack
		mov eax, [eax]Intercept.pRelocHeader // original function without instrumentation
		mov [esp+40], eax	// put contents of pRelocHeader on top of stack

		// saved last error is on top of stack
		pop dword ptr fs:[0x34]
		popad
		popfd
		ret					// go to original function, will return to returnFromOriginalFunc label

	returnFromOriginalFunc:
		pushfd
		pushfd				// push twice, first will be used for space on stack for return address

		pushad

		// Preserve last error
		call dword ptr [GetLastErrorCaller]
		push eax

		// Increment IsCalled (we are going back to the replacement library)
		push g_dwTlsIsCalled
		call TlsGetValueWrapper
		inc eax
		push eax
		push g_dwTlsIsCalled
		call dword ptr [TlsSetValueCaller]
		
		// Restore original return address
		push g_dwCallerRetAddrStack
		call popOffRetAddrStack
		mov [esp+40], eax	// put original return address on top of stack

		// saved last error is on top of stack
		pop dword ptr fs:[0x34]
		popad
		popfd

		ret					// return to replacement method
	}
}


//*************************************************************************
// Method:		getHeaderSize
// Description: Returns the size of a function's "header" (first few
//				instructions).  This must be greater than or equal to
//				five for interception to be allowed on the function.
//				This will always return a size aligned on an
//				instruction boundary.
//
// Parameters:
//	pFunc - Pointer to function
//
// Return Value: Number of bytes in the header code
//*************************************************************************
DWORD getHeaderSize(PBYTE pFunc)
{
	DWORD dwSize = 0;
	PBYTE pNewSrc;
	while (dwSize < 5)
	{
		copyInstruction(NULL, pFunc, &pNewSrc);
		dwSize += pNewSrc - pFunc;
		if (((*pFunc) == 0xc2) || ((*pFunc) == 0xc3))
			break;
		pFunc = pNewSrc;
	}
	return dwSize;
}


//*************************************************************************
// Method:		updateHeader
// Description: Updates a function's first five bytes as an atomic
//				operation.  This prevents the need to suspend all other
//				threads when updating the function's code.
//
// Parameters:
//	pFunc - Pointer to function
//  pNewHeader - New header, five bytes are copied to pFunc
//
// Return Value: None
//*************************************************************************
void updateHeader(PVOID pFunc, PVOID pNewHeader)
{
	// Atomic change of a function's first 5 bytes
	__asm
	{
		mov		edi, pFunc
		mov		esi, pNewHeader

		mov		eax, [edi]
		mov		edx, [edi + 4]

		mov		ebx, [esi]
		mov		ecx, edx
		mov		cl, [esi + 4]

		lock	cmpxchg8b [edi]
	}
}


//*************************************************************************
// Method:		interceptFunc
// Description: Intercepts a function.
//
// Parameters:
//	pszFuncName - Name of function to intercept
//  pszFuncDLL - DLL function to intercept resides in
//  pszHandlerFunc - Name of replacement function
//  pszHandlerDLL - Name of replacement library DLL
//
// Return Value: true on success
//*************************************************************************
bool interceptFunc(LPSTR pszFuncName, LPSTR pszFuncDLL, LPSTR pszHandlerFunc, LPSTR pszHandlerDLL)
{
	PVOID pFunc;

	if (pszFuncDLL[0] != 0)
	{
		HMODULE hFuncMod = GetModuleHandle(pszFuncDLL);
		if (!hFuncMod)
		{
			hFuncMod = LoadLibrary(pszFuncDLL);
			if (!hFuncMod)
				return false;
		}

		if (pszFuncName[0] == '+')
		{
			MODULEINFO moduleInfo;

			int offset = strtol(pszFuncName, NULL, 0);
			GetModuleInformation(GetCurrentProcess(), hFuncMod, &moduleInfo, sizeof(moduleInfo));
			pFunc = (PVOID)(((DWORD)(moduleInfo.lpBaseOfDll)) + offset);
		}
		else
			pFunc = GetProcAddress(hFuncMod, pszFuncName);
	}
	else
		pFunc = (PVOID)strtoul(pszFuncName, NULL, 0);

	if (!pFunc)
		return false;

	Intercept* pIntercept = g_pFirstIntercept;
	while (pIntercept)
	{
		if (pIntercept->pOrigFunc == pFunc)
		{
			// Already intercepted
			return false;
		}

		pIntercept = pIntercept->pNext;
	}

	DWORD dwHeaderSize = getHeaderSize((PBYTE)pFunc);
	if (dwHeaderSize < 5)
		return false;

	HMODULE hMod = GetModuleHandle(pszHandlerDLL);
	if (!hMod)
	{
		hMod = LoadLibrary(pszHandlerDLL);

		if (!hMod)
			return false;
	}

	// Find/add the handler module in the existing module list and increment its reference count
	Module *pModule = g_pFirstModule;
	while (pModule)
	{
		if (pModule->hMod == hMod)
		{
			// Found the module
			break;
		}

		pModule = pModule->pNext;
	}

	if (pModule == NULL)
	{
		// Handler module is referenced for the first time
		pModule = (Module*)HeapAllocCaller(g_hHeap, 0, sizeof(Module));
		pModule->hMod = hMod;
		pModule->bInit = false;
		pModule->pNext = g_pFirstModule;
		g_pFirstModule = pModule;
	}

	if (!pModule->bInit)
	{
		// Call the replacement library's init function to let it initialize
		void (*init)() = (void(*)())GetProcAddress(pModule->hMod, "ReplacementLibraryAttach");
		if (init)
			init();
		pModule->bInit = true;
	}

	PVOID pHandlerFunc = GetProcAddress(hMod, pszHandlerFunc);
	if (!pHandlerFunc)
		return false;

	pIntercept = (Intercept*)HeapAllocCaller(g_hHeap, 0, sizeof(Intercept));
	pIntercept->pNext = g_pFirstIntercept;
	pIntercept->pOrigFunc = pFunc;
	pIntercept->pHandlerFunc = pHandlerFunc;
	pIntercept->dwHeaderBytes = dwHeaderSize;

	pIntercept->pOrigHeader = (PBYTE)HeapAllocCaller(g_hHeap, 0, dwHeaderSize);
	memcpy(pIntercept->pOrigHeader, pFunc, dwHeaderSize);

	// Relocate the header instructions so that the jump can be placed there
	pIntercept->pRelocHeader = (PBYTE)HeapAllocCaller(g_hHeap, 0, 32);
	
	//Mark the memory as executable
	DWORD dwTempOldProt;
	if (! VirtualProtect(pIntercept->pRelocHeader, 32, PAGE_EXECUTE_READWRITE, &dwTempOldProt))
	{
		goto cleanup;
	}

	DWORD dwBytes = 0;
	PBYTE pDest = pIntercept->pRelocHeader;
	PBYTE pUpdatedSrc, pSrc = (PBYTE)pFunc;
	while (dwBytes < 5)
	{
		pDest = copyInstruction(pDest, pSrc, &pUpdatedSrc);
		dwBytes += pUpdatedSrc - pSrc;
		pSrc = pUpdatedSrc;
	}
	// Add jump to remainder of original function
	writeByte(pDest, 0xe9);
	writeDWord(pDest, ((PBYTE)pFunc + dwBytes) - (pDest + 4));
	while (pDest < (pIntercept->pRelocHeader + 32))
		writeByte(pDest, 0xcc);

	if (pFunc == HeapAlloc)
		HeapAllocCaller_real = (LPVOID (__stdcall *)(HANDLE, DWORD, SIZE_T))pIntercept->pRelocHeader;
	else if (pFunc == HeapFree)
		HeapFreeCaller = (BOOL (__stdcall *)(HANDLE, DWORD, LPVOID))pIntercept->pRelocHeader;
	else if (pFunc == TlsGetValue)
		TlsGetValueCaller = (LPVOID (__stdcall *)(DWORD))pIntercept->pRelocHeader;
	else if (pFunc == TlsSetValue)
		TlsSetValueCaller = (BOOL (__stdcall *)(DWORD, LPVOID))pIntercept->pRelocHeader;
	else if (pFunc == GetLastError)
		GetLastErrorCaller = (DWORD (__stdcall *)())pIntercept->pRelocHeader;
	else if (pFunc == SetLastError)
		SetLastErrorCaller = (void (__stdcall *)(DWORD))pIntercept->pRelocHeader;

	// Create original function caller entry point
	DWORD dwOldProt;
	pIntercept->pCallerFunc = (PBYTE)HeapAllocCaller(g_hHeap, 0, 10);
	
	//Mark the memory as executable (new feature on some processors)
	if (! VirtualProtect(pIntercept->pCallerFunc, 10, PAGE_EXECUTE_READWRITE, &dwOldProt))
	{
		goto cleanup;
	}

	PBYTE pJumpCode = pIntercept->pCallerFunc;
	writeByte(pJumpCode, 0x68); // push pIntercept
	writeDWord(pJumpCode, (DWORD)pIntercept);
	writeByte(pJumpCode, 0xe9); // jmp callerFunc
	writeDWord(pJumpCode, (PBYTE)callerFunc - (pJumpCode + 4));

	// Create jump table entry
	pIntercept->pJumpTable = (PBYTE)HeapAllocCaller(g_hHeap, 0, 10);
	
	//Mark the memory as executable (new feature on some processors)
	if (!VirtualProtect(pIntercept->pJumpTable, 10, PAGE_EXECUTE_READWRITE, &dwOldProt))
	{
		goto cleanup;
	}

	pJumpCode = pIntercept->pJumpTable;
	writeByte(pJumpCode, 0x68); // push pIntercept
	writeDWord(pJumpCode, (DWORD)pIntercept);
	writeByte(pJumpCode, 0xe9); // jmp impFunc
	writeDWord(pJumpCode, (PBYTE)impFunc - (pJumpCode + 4));

	// Place a jump at the original function's start so that when the function is called, our
	// jump table function gets called instead
	if (!VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &dwOldProt))
	{
		goto cleanup;
	}
	BYTE pNewHeader[5];
	pJumpCode = pNewHeader;
	writeByte(pJumpCode, 0xe9);
	writeDWord(pJumpCode, pIntercept->pJumpTable - ((PBYTE)pFunc + 5));

	updateHeader(pFunc, pNewHeader);

	if (!VirtualProtect(pFunc, 8, dwOldProt, &dwOldProt))
	{
		goto cleanup;
	}

	g_pFirstIntercept = pIntercept; // add the intercept to the list

	return true;

cleanup:

	HeapFree(g_hHeap, 0, pIntercept->pOrigHeader);
	HeapFree(g_hHeap, 0, pIntercept->pRelocHeader);
	HeapFree(g_hHeap, 0, pIntercept->pJumpTable);
	HeapFree(g_hHeap, 0, pIntercept);
	return false;
}


//*************************************************************************
// Method:		unInterceptFunc
// Description: Unintercepts a function.
//
// Parameters:
//	pFunc - Address of function to unintercept
//
// Return Value: true on success
//*************************************************************************
bool unInterceptFunc(LPVOID pFunc)
{
	if (!pFunc)
		return false;

	Intercept* pIntercept = g_pFirstIntercept;
	Intercept* pPrevIntercept = NULL;
	while (pIntercept)
	{
		if (pIntercept->pOrigFunc == pFunc)
			break;
		pPrevIntercept = pIntercept;
		pIntercept = pIntercept->pNext;
	}

	if (pIntercept == NULL)
		return false;

	// Replace the bytes originally at the header of the function
	DWORD dwOldProt;
	if (!VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &dwOldProt))
		return false;
	updateHeader(pFunc, pIntercept->pOrigHeader);
	VirtualProtect(pFunc, 8, dwOldProt, &dwOldProt);

	if (pFunc == HeapAlloc)
		HeapAllocCaller_real = HeapAlloc;
	else if (pFunc == HeapFree)
		HeapFreeCaller = HeapFree;
	else if (pFunc == TlsGetValue)
		TlsGetValueCaller = TlsGetValue;
	else if (pFunc == TlsSetValue)
		TlsSetValueCaller = TlsSetValue;
	else if (pFunc == GetLastError)
		GetLastErrorCaller = GetLastError;
	else if (pFunc == SetLastError)
		SetLastErrorCaller = SetLastError;

	// Remove intercept from the list
	if (pPrevIntercept)
		pPrevIntercept->pNext = pIntercept->pNext;
	else
		g_pFirstIntercept = pIntercept->pNext;

	HeapFree(g_hHeap, 0, pIntercept->pOrigHeader);
	HeapFree(g_hHeap, 0, pIntercept->pJumpTable);
	HeapFree(g_hHeap, 0, pIntercept);
	return true;
}


//*************************************************************************
// Method:		unInterceptFunc
// Description: Unintercepts a function.
//
// Parameters:
//	pszFuncName - Name of function to unintercept
//  pszFuncDLL - DLL the function resides in
//
// Return Value: true on success
//*************************************************************************
bool unInterceptFunc(LPSTR pszFuncName, LPSTR pszFuncDLL)
{
	if (pszFuncDLL[0] != 0)
	{
		HMODULE hFuncMod = GetModuleHandle(pszFuncDLL);
		if (!hFuncMod)
		{
			hFuncMod = LoadLibrary(pszFuncDLL);
			if (!hFuncMod)
				return false;
		}

		PVOID pFunc;

		if (pszFuncName[0] == '+')
		{
			MODULEINFO moduleInfo;

			int offset = strtol(pszFuncName, NULL, 0);
			GetModuleInformation(GetCurrentProcess(), hFuncMod, &moduleInfo, sizeof(moduleInfo));
			pFunc = (PVOID)(((DWORD)(moduleInfo.lpBaseOfDll)) + offset);
		}
		else
			pFunc = GetProcAddress(hFuncMod, pszFuncName);

		return unInterceptFunc(pFunc);
	}
	return unInterceptFunc((PVOID)strtoul(pszFuncName, NULL, 0));
}

//*************************************************************************
// Method:		pipeThreadProc
// Description: Thread procedure that handles incoming commands from
//				HEAT API.
//
// Parameters:
//	None
//
// Return Value: Thread exit code
//*************************************************************************
DWORD WINAPI pipeThreadProc(LPVOID)
{
	// Don't allow intercepting in this thread
	TlsSetValue(g_dwTlsIsCalled, (LPVOID)1);

	char pszFuncName[MAX_PATH];
	char pszFuncDLL[MAX_PATH];
	char pszHandlerFunc[MAX_PATH];
	char pszHandlerDLL[MAX_PATH];

	while (true)
	{
		DWORD dwBytes;
		DWORD dwStrLen;
		BYTE ucCommand;

		if ((!ConnectNamedPipe(g_hDataPipe, NULL)) && (GetLastError() != ERROR_PIPE_CONNECTED))
			goto PipeError;

		if (!ReadFile(g_hDataPipe, &ucCommand, 1, &dwBytes, NULL))
			continue;
		if (dwBytes != 1)
			continue;
		switch (ucCommand)
		{
			case CMD_INTERCEPT:
				if (!ReadFile(g_hDataPipe, &dwStrLen, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;
				if (dwStrLen >= MAX_PATH)
					goto PipeError;

				if (!ReadFile(g_hDataPipe, pszFuncName, dwStrLen, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != dwStrLen)
					goto PipeError;
				pszFuncName[dwStrLen] = 0;

				if (!ReadFile(g_hDataPipe, &dwStrLen, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;
				if (dwStrLen >= MAX_PATH)
					goto PipeError;

				if (!ReadFile(g_hDataPipe, pszFuncDLL, dwStrLen, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != dwStrLen)
					goto PipeError;
				pszFuncDLL[dwStrLen] = 0;

				if (!ReadFile(g_hDataPipe, &dwStrLen, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;
				if (dwStrLen >= MAX_PATH)
					goto PipeError;

				if (!ReadFile(g_hDataPipe, pszHandlerFunc, dwStrLen, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != dwStrLen)
					goto PipeError;
				pszHandlerFunc[dwStrLen] = 0;

				if (!ReadFile(g_hDataPipe, &dwStrLen, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;
				if (dwStrLen >= MAX_PATH)
					goto PipeError;

				if (!ReadFile(g_hDataPipe, pszHandlerDLL, dwStrLen, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != dwStrLen)
					goto PipeError;
				pszHandlerDLL[dwStrLen] = 0;

				if (!interceptFunc(pszFuncName, pszFuncDLL, pszHandlerFunc, pszHandlerDLL))
				{
					// BUGBUG: Error handler here
				}
				break;

			case CMD_UNINTERCEPT:
				if (!ReadFile(g_hDataPipe, &dwStrLen, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;
				if (dwStrLen >= MAX_PATH)
					goto PipeError;

				if (!ReadFile(g_hDataPipe, pszFuncName, dwStrLen, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != dwStrLen)
					goto PipeError;
				pszFuncName[dwStrLen] = 0;

				if (!ReadFile(g_hDataPipe, &dwStrLen, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;
				if (dwStrLen >= MAX_PATH)
					goto PipeError;

				if (!ReadFile(g_hDataPipe, pszFuncDLL, dwStrLen, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != dwStrLen)
					goto PipeError;
				pszFuncDLL[dwStrLen] = 0;

				if (!unInterceptFunc(pszFuncName, pszFuncDLL))
				{
					// BUGBUG: Error handler here
				}
				break;
			case CMD_DEINIT_ON_PROCESS_TERMINATE:
				DWORD pid;

				if (!ReadFile(g_hDataPipe, &pid, 4, &dwBytes, NULL))
					goto PipeError;
				if (dwBytes != 4)
					goto PipeError;

				if (!deInitOnProcessTerminate(pid))
				{
					// Error handler here
				}
				break;

			default:
				goto PipeError;
				break;
		}

		SetEvent(g_hProcessedEvent);

		if (!DisconnectNamedPipe(g_hDataPipe))
			goto PipeError;
	}

PipeError:
	MessageBox(NULL, "Wrong version of HeatDll.dll", "Terminating process", MB_OK | MB_TOPMOST | MB_TASKMODAL);
	CloseHandle(g_hDataPipe);
	ExitProcess(-1);
}


//*************************************************************************
// Method:		disableInterceptionInCurrentThread
// Description: Disallows interception for the current thread.  To
//				reenable interception, there must be exactly one call to
//				enableInterceptionInCurrentThread for each call to
//				this function.
//
// Parameters:
//	None
//
// Return Value: Counter representing how many times this function
//		has been called for this thread.
//*************************************************************************
DWORD __declspec(dllexport) WINAPI disableInterceptionInCurrentThread()
{
	DWORD dwNewValue = (DWORD)TlsGetValue(g_dwTlsIsCalled) + 1;
	TlsSetValue(g_dwTlsIsCalled, (PVOID)dwNewValue);
	return dwNewValue;
}

//*************************************************************************
// Method:		enableInterceptionInCurrentThread
// Description: Reenables interception for the current thread.  To
//				reenable interception, there must be exactly one call to
//				this function for each call to disableInterceptionInCurrentThread
//
// Parameters:
//	None
//
// Return Value: Counter representing how many more times
//		disableInterceptionInCurrentThread has been called for this thread.
//*************************************************************************
DWORD __declspec(dllexport) WINAPI enableInterceptionInCurrentThread()
{
	DWORD dwNewValue = (DWORD)TlsGetValue(g_dwTlsIsCalled) - 1;
	TlsSetValue(g_dwTlsIsCalled, (PVOID)dwNewValue);
	return dwNewValue;
}


//*************************************************************************
// Method:		incrementCallLevel
// Description: Increments the call level for this thread.
//
// Parameters:
//	None
//
// Return Value: New call level
//*************************************************************************
DWORD __declspec(dllexport) WINAPI incrementCallLevel()
{
	DWORD dwNewValue = (DWORD)TlsGetValue(g_dwCallLevel) + 1;
	TlsSetValue(g_dwCallLevel, (PVOID)dwNewValue);
	return dwNewValue;
}

//*************************************************************************
// Method:		decrementCallLevel
// Description: Decerements the call level for this thread
//
// Parameters:
//	None
//
// Return Value: New call level
//*************************************************************************
DWORD __declspec(dllexport) WINAPI decrementCallLevel()
{
	DWORD dwNewValue = (DWORD)TlsGetValue(g_dwCallLevel) - 1;
	TlsSetValue(g_dwCallLevel, (PVOID)dwNewValue);
	return dwNewValue;
}


//*************************************************************************
// Method:		getCalledFunc
// Description: Returns the original function's address.
//
// Parameters:
//	None
//
// Return Value: Original function's address
//*************************************************************************
PVOID __declspec(dllexport) WINAPI getCalledFunc()
{
	return TlsGetValue(g_dwCalledAddr);
}

//*************************************************************************
// Method:		getCallLevel
// Description: Returns the current call level (how many intercepted
//				functions are currently on the call stack)
//
// Parameters:
//	None
//
// Return Value: Current call level
//*************************************************************************
DWORD __declspec(dllexport) WINAPI getCallLevel()
{
	return (DWORD)TlsGetValue(g_dwCallLevel);
}

//*************************************************************************
// Method:		getReturnAddr
// Description: Returns the return address of the original function.
//
// Parameters:
//	None
//
// Return Value: Return address
//*************************************************************************
PVOID __declspec(dllexport) WINAPI getReturnAddr()
{
	return peekRetAddrStack(g_dwOrigRetAddrStack);
}


//*************************************************************************
// Method:		getOriginalFunctionCaller
// Description: Obtains a function pointer that can be used to call
//				the original method with reentrant interception enabled.
//
// Parameters:
//	pszFuncName - Name of function
//  pszFuncDLL - Name of DLL function resides in
//
// Return Value: Function pointer to be used for calling original function
//*************************************************************************
PVOID __declspec(dllexport) WINAPI getOriginalFunctionCaller(LPSTR pszFuncName, LPSTR pszFuncDLL)
{
	PVOID pFunc;

	// Get the function address out of the name strings
	if (pszFuncDLL[0] != 0)
	{
		HMODULE hFuncMod = GetModuleHandle(pszFuncDLL);
		if (!hFuncMod)
			return NULL; // DLL will already be loaded if the function is intercepted

		if (pszFuncName[0] == '+')
		{
			MODULEINFO moduleInfo;

			int offset = strtol(pszFuncName, NULL, 0);
			GetModuleInformation(GetCurrentProcess(), hFuncMod, &moduleInfo, sizeof(moduleInfo));
			pFunc = (PVOID)(((DWORD)(moduleInfo.lpBaseOfDll)) + offset);
		}
		else
			pFunc = GetProcAddress(hFuncMod, pszFuncName);
	}
	else
		pFunc = (PVOID)strtoul(pszFuncName, NULL, 0);

	if (!pFunc)
		return NULL;

	// Find the Intercept structure for this function and return the caller function
	Intercept* pIntercept = g_pFirstIntercept;
	while (pIntercept)
	{
		if (pIntercept->pOrigFunc == pFunc)
			return pIntercept->pCallerFunc;
		pIntercept = pIntercept->pNext;
	}
	return pFunc; // not found, return real function
}


//*************************************************************************
// Method:		initCommandPipe
// Description: Initializes the command pipe used for HEAT API communication
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void initCommandPipe()
{
	char pszPipeReadyEventName[MAX_PATH];
	char pszPipeName[MAX_PATH];
	char pszProcessedEventName[MAX_PATH];
	HANDLE hPipeReadyEvent;
	
	// Initialize a security descriptor.
	PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
	// Initialize a security attributes structure.
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof (SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = pSD;
	sa.bInheritHandle = FALSE;

	// Create the pipe for incoming commands
	sprintf(pszPipeName, "\\\\.\\pipe\\HEATDataPipe%d", GetCurrentProcessId());
	g_hDataPipe = CreateNamedPipe(pszPipeName, PIPE_ACCESS_INBOUND, PIPE_TYPE_BYTE | PIPE_WAIT, 2, 4096, 4096, INFINITE, &sa);
	if (g_hDataPipe == INVALID_HANDLE_VALUE)
	{
		MessageBox(NULL, "Could not create pipe for interception", "Terminating process", MB_ICONSTOP | MB_OK | MB_TOPMOST | MB_TASKMODAL);
		ExitProcess(-1);
	}

	sprintf(pszProcessedEventName, "HEATCmdProcessed%d", GetCurrentProcessId());
	g_hProcessedEvent = CreateEvent(NULL, FALSE, FALSE, pszProcessedEventName);

	g_hPipeThread = HeatCreateThread(pipeThreadProc, NULL, NULL);

	// Notify the API that we are ready for commands
	sprintf(pszPipeReadyEventName, "HEATPipeReady%d", GetCurrentProcessId());
	hPipeReadyEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, pszPipeReadyEventName);
	SetEvent(hPipeReadyEvent);
	CloseHandle(hPipeReadyEvent);
}


//*************************************************************************
// Method:		init
// Description: Called by the DLL injection code to initialize HEAT.
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void __declspec(dllexport) init()
{
	if (g_bInit)
	{
		if (g_bIsUninjecting)
		{
			g_bIsUninjecting = FALSE;
			initCommandPipe();
		}
		return;
	}

	g_dwTlsIsCalled = TlsAlloc();
	g_dwCallLevel = TlsAlloc();
	g_dwOrigRetAddrStack = TlsAlloc();
	g_dwCallerRetAddrStack = TlsAlloc();
	g_dwCalledAddr = TlsAlloc();

	if ((g_dwTlsIsCalled == TLS_OUT_OF_INDEXES) || (g_dwCallLevel == TLS_OUT_OF_INDEXES) ||
		(g_dwOrigRetAddrStack == TLS_OUT_OF_INDEXES) || (g_dwCallerRetAddrStack == TLS_OUT_OF_INDEXES) ||
		(g_dwCalledAddr == TLS_OUT_OF_INDEXES))
	{
		MessageBox(NULL, "Insufficient thread local storage to continue", "Terminating process", MB_ICONSTOP | MB_OK | MB_TOPMOST | MB_TASKMODAL);
		ExitProcess(-1);
	}

	disableInterceptionInCurrentThread();

	// Create code to free HeatDll on uninject
	g_pFreeHeatDllCode = (BYTE*)HeapAllocCaller(GetProcessHeap(), 0, 64);
	if (!g_pFreeHeatDllCode)
	{
		MessageBox(NULL, "Insufficient memory to inject", "Terminating process", MB_ICONSTOP | MB_OK | MB_TOPMOST | MB_TASKMODAL);
		ExitProcess(-1);
	}
	BYTE* pCodePtr = g_pFreeHeatDllCode;
	// pushad
	writeByte(pCodePtr, 0x60);
	// pushfd
	writeByte(pCodePtr, 0x9c);
	// push g_hHeatModule
	writeByte(pCodePtr, 0x68);
	writeDWord(pCodePtr, (DWORD)g_hHeatModule);
	// call FreeLibrary
	writeByte(pCodePtr, 0xe8);
	writeDWord(pCodePtr, (DWORD)((PBYTE)FreeLibrary - (pCodePtr + 4)));
	// popfd
	writeByte(pCodePtr, 0x9d);
	// popad
	writeByte(pCodePtr, 0x61);
	// ret
	writeByte(pCodePtr, 0xc3);

	g_hHeap = HeapCreate(0, 16384, 0);

	g_bIsUninjecting = FALSE;

	initCommandPipe();

	g_bInit = TRUE;
	enableInterceptionInCurrentThread();  // OK to intercept now
}


//*************************************************************************
// Method:		processDeinit
// Description: Deinitializes HEAT by unintercepting all functions and
//				flagging that HEAT should be unloaded as soon as all
//				replacement functions are exited.
//
// Parameters:
//	None
//
// Return Value: true if HEAT is fully deinitialized and the DLL can
//		be unloaded.
//*************************************************************************
bool processDeinit()
{
	try
	{
		if (!g_bInit)
			return false;

		TlsSetValue(g_dwTlsIsCalled, (LPVOID)1);  // Stop intercepting in this thread

		if (g_hPipeThread)
		{
			TerminateThread(g_hPipeThread, 0);
			g_hPipeThread = NULL;
		}

		if (g_hDataPipe != INVALID_HANDLE_VALUE)
		{
			CloseHandle(g_hDataPipe);
			g_hDataPipe = INVALID_HANDLE_VALUE;
		}
		if (g_hProcessedEvent != INVALID_HANDLE_VALUE)
		{
			CloseHandle(g_hProcessedEvent);
			g_hProcessedEvent = INVALID_HANDLE_VALUE;
		}

		// Unintercept all intercepted functions
		while (g_pFirstIntercept)
			unInterceptFunc(g_pFirstIntercept->pOrigFunc);

		// Call the deinit function for all loaded handler modules
		Module* pModule = g_pFirstModule;
		while (pModule)
		{
			void (*deinit)() = (void(*)())GetProcAddress(pModule->hMod, "ReplacementLibraryDetach");
			if (deinit)
				deinit();
			pModule->bInit = false;
			pModule = pModule->pNext;
		}

		if (g_dwReplacementFunctionRefCount == 0)
		{
			unloadHandlerModules();
			if (g_hHeap != NULL)
			{
				HeapDestroy(g_hHeap);
				g_hHeap = NULL;
			}
			g_pFirstModule = NULL;
			if (g_pFreeHeatDllCode != NULL)
			{
				HeapFree(GetProcessHeap(), 0, g_pFreeHeatDllCode);
				g_pFreeHeatDllCode = NULL;
			}
			g_bInit = FALSE;
			return true;
		}

		// BUGBUG: Free TLS entries
		g_bIsUninjecting = TRUE;
		g_bInit = FALSE;
	}
	catch(...)
	{
	}
	return false;
}


//*************************************************************************
// Method:		deinit
// Description: Called by the uninjection code to deinitialize HEAT
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void __declspec(naked) __declspec(dllexport) deinit()
{
	__asm
	{
		call processDeinit
		test al, al
		jz noDLLFree

		// Free HeatDll (must make sure we return to caller, not HeatDll, when done)
		pop eax // Pop off return address
		push g_hHeatModule
		push eax // Push return address back
		jmp dword ptr [FreeLibrary]

noDLLFree:
		ret
	}
}

//*************************************************************************
// Method:		deInitOnProcessTerminate
// Description: calls deinit if the specified process terminates
//
// Parameters:
//	pid - the pid to wait on
//
// Return Value: true on success
//*************************************************************************
bool deInitOnProcessTerminate(DWORD pid)
{
	// The thread in memory will look like this
	//
	// disableInterceptionInCurrentThread();
	// HANDLE h = OpenProcess(SYNCHRONIZE, FALSE, pid);
	// WaitForSingleObject(h, INFINITE);
	// deinit();
	// ExitThread();
	//

	PBYTE pdwCode = (PBYTE)VirtualAlloc(NULL, 256, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if (!pdwCode)
		return false;

	PBYTE pdwOutPtr = pdwCode;

	writeByte(pdwOutPtr, 0xe8);
	writeDWord(pdwOutPtr, (DWORD)disableInterceptionInCurrentThread - (DWORD)pdwOutPtr - 4);

	writeByte(pdwOutPtr, 0x8b);  // mov eax, DWORD PTR [esp+4]
	writeByte(pdwOutPtr, 0x44);
	writeByte(pdwOutPtr, 0x24);
	writeByte(pdwOutPtr, 0x04);

	writeByte(pdwOutPtr, 0x56);  // push esi
	writeByte(pdwOutPtr, 0x50);  // push eax

	writeByte(pdwOutPtr, 0x6a);  // push 0
	writeByte(pdwOutPtr, 0x00);

	writeByte(pdwOutPtr, 0x68);  // push 00100000H
	writeByte(pdwOutPtr, 0x00);
	writeByte(pdwOutPtr, 0x00);
	writeByte(pdwOutPtr, 0x10);
	writeByte(pdwOutPtr, 0x00);

	writeByte(pdwOutPtr, 0xe8);  // call DWORD PTR __imp__OpenProcess@12
	writeDWord(pdwOutPtr, (DWORD)OpenProcess - (DWORD)pdwOutPtr - 4);

	writeByte(pdwOutPtr, 0x8b);  // mov esi,eax
	writeByte(pdwOutPtr, 0xf0);

	writeByte(pdwOutPtr, 0x6a);  // push -1
	writeByte(pdwOutPtr, 0xff);

	writeByte(pdwOutPtr, 0x56);  // push esi

	writeByte(pdwOutPtr, 0xe8);  // call DWORD PTR __imp__WaitForSingleObject@8
	writeDWord(pdwOutPtr, (DWORD)WaitForSingleObject - (DWORD)pdwOutPtr - 4);

	writeByte(pdwOutPtr, 0xe8);  // call deinit
	writeDWord(pdwOutPtr, (DWORD)deinit - (DWORD)pdwOutPtr - 4);

	writeByte(pdwOutPtr, 0xe8);  // call ExitThread
	writeDWord(pdwOutPtr, (DWORD)ExitThread - (DWORD)pdwOutPtr - 4);

	HeatCreateThread((LPTHREAD_START_ROUTINE)pdwCode, (LPVOID)(size_t)pid, NULL);

	return true;
}

//*************************************************************************
// Method:		DllMain
// Description: Called by Windows when the DLL loads/unloads
//
// Parameters:
//	See MSDN documentation
//
// Return Value: See MSDN documentation
//*************************************************************************
BOOL APIENTRY DllMain( HMODULE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			g_hHeatModule = hModule;
			InitializeCriticalSection(&g_csThreadCreated);
			break;
		case DLL_PROCESS_DETACH:
			DeleteCriticalSection(&g_csThreadCreated);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			// Don't intercept on thread exit (return address stack will be invalid)
			disableInterceptionInCurrentThread();
			if (g_hHeap)
			{
				// Free the return address stacks if they exist
				if (g_dwOrigRetAddrStack != TLS_OUT_OF_INDEXES)
				{
					RetAddrStack* pStack = (RetAddrStack*)TlsGetValueWrapper(g_dwOrigRetAddrStack);
					if (pStack)
					{
						if (pStack->pStack)
							HeapFreeCaller(g_hHeap, 0, pStack->pStack);
						HeapFreeCaller(g_hHeap, 0, pStack);
					}
				}
				if (g_dwCallerRetAddrStack != TLS_OUT_OF_INDEXES)
				{
					RetAddrStack* pStack = (RetAddrStack*)TlsGetValueWrapper(g_dwCallerRetAddrStack);
					if (pStack)
					{
						if (pStack->pStack)
							HeapFreeCaller(g_hHeap, 0, pStack->pStack);
						HeapFreeCaller(g_hHeap, 0, pStack);
					}
				}
			}
			break;
	}
    return TRUE;
}
