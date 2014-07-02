//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		AsyncOp.cpp
//
// DESCRIPTION: AsyncOpInfo class contains information about an
//				asynchronous operation.  See AsyncOpList class for a
//				detailed description of what is required.  To make sure
//				a AsyncOpInfo structure that needs to be maintained while
//				the AsyncOpList class is not locked, call the
//				AddRef/Release members of AsyncOpInfo.
//
//				AsyncOpList class holds a list of the currently executing
//				asynchronous receive or send operations using either the
//				Winsock functions or ReadFile/WriteFile.  Since the data
//				sent/received by the application is different from the
//				actual data going across the network, we may have to
//				change the length of an asynchronous operation.  To do
//				this, we have to use a different LPOVERLAPPED structure,
//				but we need to keep track of the application provided
//				LPOVERLAPPED structure such that we can give the callback
//				functions and GetOverlappedResult calls their expected
//				behavior.  These functions are based on the LPOVERLAPPED
//				structure passed when starting the operation, which may
//				not equal the one used by the application, thus requiring
//				a form of mapping.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Feb 2003		 R. Wagner	 File created.
//*************************************************************************

#include "AsyncOp.h"
#include "SocketList.h"
#include "SocketInfo.h"

HANDLE AsyncOpInfo::heap = NULL;

// AsyncOpInfo constructor, ReadFile/WriteFile version
//
// Parameters:
//   s            - Socket handle
//   op           - OVERLAPPED structure for asynchronous operation
//   lpCompletion - Routine to be called when the operation completes
//	 buf		  - Buffer that will be sent via asynchronous operation
//   iTransferLen - Number of bytes to be transferred
//
// Return value:
//   None
AsyncOpInfo::AsyncOpInfo(HANDLE s, LPOVERLAPPED op, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletion, char *buf, int iTransferLen)
{
	if (!heap)
		heap = HeapCreate(0, 0, 0);

	m_socket = (SOCKET)s;
	m_fWinsockOp = false;
	m_lpOrigKOp = op;
	m_lpKCompletion = lpCompletion;
	m_lpWSCompletion = NULL;
	m_lpBuf = buf;
	m_iOldTransferLen = iTransferLen;
	m_iRefCount = 1;
	m_dwThreadId = GetCurrentThreadId();
	m_lpWSBuffers = NULL;
	m_lpKernelBuffer = NULL;
}


// AsyncOpInfo constructor, Winsock version
//
// Parameters:
//   s            - Socket handle
//   op           - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletion - Routine to be called when the operation completes
//	 buf		  - Buffer that will be sent via asynchronous operation
//   iTransferLen - Number of bytes to be transferred
//
// Return value:
//   None
AsyncOpInfo::AsyncOpInfo(SOCKET s, LPWSAOVERLAPPED op, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletion, char *buf, int iTransferLen)
{
	if (!heap)
		heap = HeapCreate(0, 0, 0);

	m_socket = s;
	m_fWinsockOp = true;
	m_lpOrigWSOp = op;
	m_lpWSCompletion = lpCompletion;
	m_lpKCompletion = NULL;
	m_lpBuf = buf;
	m_iOldTransferLen = iTransferLen;
	m_iRefCount = 1;
	m_dwThreadId = GetCurrentThreadId();
	m_lpWSBuffers = NULL;
	m_lpKernelBuffer = NULL;
}


AsyncOpInfo::~AsyncOpInfo()
{
	if (m_lpWSBuffers)
	{
		HeapFree(heap, 0, m_lpWSBuffers);
		m_lpWSBuffers = NULL;
	}

	if (m_lpBuf)
	{
		delete[] m_lpBuf;
		m_lpBuf = NULL;
	}
}


// AddRef increases the reference count of the AsyncOpInfo object
//
// Parameters:
//   None
//
// Return value:
//   None
void AsyncOpInfo::AddRef()
{
	m_iRefCount++;
}


// Release decreases the reference count of the AsyncOpInfo object,
// and deletes it if the reference count reaches zero.
//
// Parameters:
//   None
//
// Return value:
//   None
void AsyncOpInfo::Release()
{
	m_iRefCount--;
	if (m_iRefCount == 0)
		delete this;
}


// GetSocket retrieves the socket handle for this operation
//
// Parameters:
//   None
//
// Return value:
//   Socket handle for this operation
SOCKET AsyncOpInfo::GetSocket()
{
	return m_socket;
}


// IsWSAyncOp determines if this operation is a Winsock operation
//
// Parameters:
//   None
//
// Return value:
//   true if this operation is a Winsock operation, false if this
//   operation is a ReadFile/WriteFile operation.
bool AsyncOpInfo::IsWSAyncOp()
{
	return m_fWinsockOp;
}


// GetKernelAsyncOp retrieves the OVERLAPPED structure for a ReadFile/WriteFile operation
//
// Parameters:
//   None
//
// Return value:
//   OVERLAPPED structure for this operation
LPOVERLAPPED AsyncOpInfo::GetKernelAsyncOp()
{
	return m_lpOrigKOp;
}


// GetKernelAsyncOp retrieves the WSAOVERLAPPED structure for a Winsock operation
//
// Parameters:
//   None
//
// Return value:
//   WSAOVERLAPPED structure for this operation
LPWSAOVERLAPPED AsyncOpInfo::GetWSAsyncOp()
{
	return m_lpOrigWSOp;
}


// GetKernelCompletionRoutine retrieves the completion routine for a ReadFile/WriteFile operation
//
// Parameters:
//   None
//
// Return value:
//   Completion routine for this operation
LPOVERLAPPED_COMPLETION_ROUTINE AsyncOpInfo::GetKernelCompletionRoutine()
{
	return m_lpKCompletion;
}


// GetWSCompletionRoutine retrieves the completion routine for a Winsock operation
//
// Parameters:
//   None
//
// Return value:
//   Completion routine for this operation
LPWSAOVERLAPPED_COMPLETION_ROUTINE AsyncOpInfo::GetWSCompetionRoutine()
{
	return m_lpWSCompletion;
}


// GetTransferLength retrieves the original transfer length for this operation
//
// Parameters:
//   None
//
// Return value:
//   Original transfer length for this operation
int AsyncOpInfo::GetTransferLength()
{
	return m_iOldTransferLen;
}


// GetWSBuffers retrieves the data buffers used for a Winsock operation
//
// Parameters:
//   None
//
// Return value:
//   Data buffers used for this operation
LPWSABUF AsyncOpInfo::GetWSBuffers()
{
	return m_lpWSBuffers;
}


// GetWSBufferCount retrieves the number of data buffers used for a Winsock operation
//
// Parameters:
//   None
//
// Return value:
//   Number of data buffers used for this operation
DWORD AsyncOpInfo::GetWSBufferCount()
{
	return m_dwWSBufferCount;
}


// GetKernelBuffer retrieves the data buffer used for a ReadFile/WriteFile operation
//
// Parameters:
//   None
//
// Return value:
//   Data buffer used for this operation
BYTE* AsyncOpInfo::GetKernelBuffer()
{
	return m_lpKernelBuffer;
}


// GetKernelBufferLen retrieves the length of the data buffer used for a ReadFile/WriteFile operation
//
// Parameters:
//   None
//
// Return value:
//   Length of the data buffer used for this operation
DWORD AsyncOpInfo::GetKernelBufferLen()
{
	return m_dwKernelBufferLen;
}


// GetThreadId returns the thread that started this operation
//
// Parameters:
//   None
//
// Return value:
//   Thread ID that started this operation
DWORD AsyncOpInfo::GetThreadId()
{
	return m_dwThreadId;
}


// SetWSBuffers sets the data buffers that are to be used for this operation
//
// Parameters:
//   lpBuffers     - Data buffers to be used
//   dwBufferCount - Number of data buffers
//
// Return value:
//   None
void AsyncOpInfo::SetWSBuffers(LPWSABUF lpBuffers, DWORD dwBufferCount)
{
	if (m_lpWSBuffers)
		m_lpWSBuffers = (LPWSABUF)HeapReAlloc(heap, HEAP_ZERO_MEMORY, m_lpWSBuffers, sizeof(WSABUF) * dwBufferCount);
	else
		m_lpWSBuffers = (LPWSABUF)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(WSABUF) * dwBufferCount);

	CopyMemory(m_lpWSBuffers, lpBuffers, sizeof(WSABUF) * dwBufferCount);
	m_dwWSBufferCount = dwBufferCount;
}


// SetKernelBuffer sets the data buffer that is to be used for this operation
//
// Parameters:
//   lpBuffer    - Data buffer to be used
//   dwBufferLen - Length of data buffer
//
// Return value:
//   None
void AsyncOpInfo::SetKernelBuffer(BYTE* lpBuffer, DWORD dwBufferLen)
{
	m_lpKernelBuffer = lpBuffer;
	m_dwKernelBufferLen = dwBufferLen;
}


// SetTransferLength will set the transfer length for this operation
//
// Parameters:
//   iTransferLen - Number of bytes transferred
//
// Return value:
//   None
void AsyncOpInfo::SetTransferLength(int iTransferLen)
{
	m_iOldTransferLen = iTransferLen;
}


// AsyncOpList constructor
//
// Parameters:
//   None
//
// Return value:
//   None
AsyncOpList::AsyncOpList()
{
	InitializeCriticalSection(&m_csListCriticalSection);
}


// AsyncOpList destructor
//
// Parameters:
//   None
//
// Return value:
//   None
AsyncOpList::~AsyncOpList()
{
	DeleteCriticalSection(&m_csListCriticalSection);
}


// AddKernelOp adds an asynchronous operation to the operation list (ReadFile/WriteFile version)
//
// Parameters:
//   s            - Socket handle
//   lpOrigOp     - OVERLAPPED structure for the asynchronous operation
//   lpCompletion - Routine to be called when the operation completes
//	 buf		  - Buffer that will be sent via asynchronous operation
//   iTransferLen - Number of bytes to be transferred
//
// Return value:
//   Pointer to the new AsyncOpInfo object for this operation
AsyncOpInfo* AsyncOpList::AddKernelOp(HANDLE s, LPOVERLAPPED lpOrigOp, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletion,
	char *buf, int iTransferLen)
{
	AsyncOpInfo* info = new AsyncOpInfo(s, lpOrigOp, lpCompletion, buf, iTransferLen);

	m_mKList[lpOrigOp] = info;
	return info;
}


// AddWSOp adds an asynchronous operation to the operation list (Winsock version)
//
// Parameters:
//   s            - Socket handle
//   lpOrigOp     - WSAOVERLAPPED structure for the asynchronous operation
//   lpCompletion - Routine to be called when the operation completes
//	 buf		  - Buffer that will be sent via asynchronous operation
//   iTransferLen - Number of bytes to be transferred
//
// Return value:
//   Pointer to the new AsyncOpInfo object for this operation
AsyncOpInfo* AsyncOpList::AddWSOp(SOCKET s, LPWSAOVERLAPPED lpOrigOp, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletion,
	char *buf, int iTransferLen)
{
	AsyncOpInfo* info = new AsyncOpInfo(s, lpOrigOp, lpCompletion, buf, iTransferLen);

	m_mWSList[lpOrigOp] = info;
	return info;
}


// GetKernelOpInfo retrieves the AsyncOpInfo instance describing a given asynchronous operation.  Returns NULL
// if the operation does not exist. (ReadFile/WriteFile version)
//
// Parameters:
//   op - OVERLAPPED structure used when starting the operation
//
// Return value:
//   Pointer to the AsyncOpInfo object for this operation
AsyncOpInfo* AsyncOpList::GetKernelOpInfo(LPOVERLAPPED op)
{
	if (m_mKList.find(op) == m_mKList.end())
		return NULL;

	AsyncOpInfo* info = m_mKList[op];
	return info;
}


// GetWSOpInfo retrieves the AsyncOpInfo instance describing a given asynchronous operation.  Returns NULL
// if the operation does not exist. (Winsock version)
//
// Parameters:
//   op - WSAOVERLAPPED structure used when starting the operation
//
// Return value:
//   Pointer to the AsyncOpInfo object for this operation
AsyncOpInfo* AsyncOpList::GetWSOpInfo(LPWSAOVERLAPPED op)
{
	if (m_mWSList.find(op) == m_mWSList.end())
		return NULL;

	AsyncOpInfo* info = m_mWSList[op];
	return info;
}


// CancelIo removes any ReadFile/WriteFile asynchronous operations in the
// current thread for a specific socket handle
//
// Parameters:
//   hFile - Socket handle to cancel operations for
//
// Return value:
//   None
void AsyncOpList::CancelIo(HANDLE hFile)
{
	map<LPOVERLAPPED, AsyncOpInfo*>::iterator opIter;

	// Loop through all active operations and remove the ones for the
	// specified handle and the current thread
	for (opIter=m_mKList.begin(); opIter!=m_mKList.end(); )
	{
		if ((opIter->second->GetThreadId() == GetCurrentThreadId()) &&
			(opIter->second->GetSocket() == (SOCKET)hFile))
		{
			m_mKList.erase(opIter);
			// Iterator may no longer be valid, so obtain a valid iterator
			opIter = m_mKList.begin();
		}
		else
			opIter++;
	}
}


// CloseKernelOp removes an asynchronous operation from the operation list and frees the memory used by
// the associated AsyncOpInfo instance (ReadFile/WriteFile version)
//
// Parameters:
//   op - OVERLAPPED structure used when starting the operation
//
// Return value:
//   None
void AsyncOpList::CloseKernelOp(LPOVERLAPPED op)
{
	map<LPOVERLAPPED, AsyncOpInfo*>::iterator operation = m_mKList.find(op);
		
	if (operation != m_mKList.end())
	{
		AsyncOpInfo* info = m_mKList[op];
		if (info)
			info->Release();

		m_mKList.erase(operation);
	}
}


// CloseWSOp removes an asynchronous operation from the operation list and frees the memory used by
// the associated AsyncOpInfo instance (Winsock version)
//
// Parameters:
//   op - WSAOVERLAPPED structure used when starting the operation
//
// Return value:
//   None
void AsyncOpList::CloseWSOp(LPWSAOVERLAPPED op)
{
	map<LPWSAOVERLAPPED, AsyncOpInfo*>::iterator operation = m_mWSList.find(op);
		
	if (operation != m_mWSList.end())
	{
		AsyncOpInfo* info = m_mWSList[op];
		if (info)
			info->Release();

		m_mWSList.erase(operation);
	}
}


// Gives the current thread exclusive access to the operation list
//
// Parameters:
//   None
//
// Return value:
//   None
void AsyncOpList::Lock()
{
	EnterCriticalSection(&m_csListCriticalSection);
}


// Yields exclusive access to the operation list
//
// Parameters:
//   None
//
// Return value:
//   None
void AsyncOpList::Unlock()
{
	LeaveCriticalSection(&m_csListCriticalSection);
}


// NetCorrupt_WSRecvCompletionRoutine is called by Winsock when an asynchronous receive has completed
//
// Parameters:
//   dwError       - Error code
//   cbTransferred - Number of bytes transferred
//   lpOverlapped  - WSAOVERLAPPED structure used when starting the operation
//   dwFlags       - Completion flags
//
// Return value:
//   None
void CALLBACK NetCorrupt_WSRecvCompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped,
											 DWORD dwFlags)
{
	// Find the AsyncInfo structure for this operation
	g_asyncList.Lock();
	AsyncOpInfo* opInfo = g_asyncList.GetWSOpInfo(lpOverlapped);
	if (!opInfo)
	{
		g_asyncList.Unlock();
		return;
	}

	// Find the SocketInfo structure for the socket
	g_socketList.Lock();
	SocketInfo* socketInfo = g_socketList.GetSocketInfo(opInfo->GetSocket());
	if (!socketInfo)
	{
		g_socketList.Unlock();
		g_asyncList.Unlock();
		return;
	}

	// Process the received data
	socketInfo->ReceiveWSCompletion(opInfo->GetWSBuffers(), opInfo->GetWSBufferCount(), cbTransferred);

	// Save the transfer length for any calls to WSAGetOverlappedResult
	opInfo->SetTransferLength(cbTransferred);

	opInfo->AddRef();
	socketInfo->AddRef();
	g_socketList.Unlock();
	g_asyncList.Unlock();

	// Call the original routine, if it exists.  Use the original number of bytes sent instead of
	// the filtered value so that the application does not get confused.
	if (opInfo->GetWSCompetionRoutine())
		opInfo->GetWSCompetionRoutine()(dwError, cbTransferred, lpOverlapped, dwFlags);

	// Remove the operation from the list
	g_asyncList.CloseWSOp(lpOverlapped);

	opInfo->Release();
	socketInfo->Release();
}


// NetCorrupt_KernelRecvCompletionRoutine is called by the kernel when an asynchronous
// receive has completed
//
// Parameters:
//   dwErrorCode                - Error code
//   dwNumberOfBytesTransferred - Number of bytes transferred
//   lpOverlapped               - OVERLAPPED structure used when starting the operation
//
// Return value:
//   None
void CALLBACK NetCorrupt_KernelRecvCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred,
												 LPOVERLAPPED lpOverlapped)
{
	// Find the AsyncInfo structure for this operation
	g_asyncList.Lock();
	AsyncOpInfo* opInfo = g_asyncList.GetKernelOpInfo(lpOverlapped);
	if (!opInfo)
	{
		g_asyncList.Unlock();
		return;
	}

	// Find the SocketInfo structure for the socket
	g_socketList.Lock();
	SocketInfo* socketInfo = g_socketList.GetSocketInfo(opInfo->GetSocket());
	if (!socketInfo)
	{
		g_socketList.Unlock();
		g_asyncList.Unlock();
		return;
	}

	// Process the received data
	socketInfo->ReceiveReadFileCompletion(opInfo->GetKernelBuffer(), opInfo->GetKernelBufferLen(), dwNumberOfBytesTransferred);

	// Save the transfer length for any calls to GetOverlappedResult
	opInfo->SetTransferLength(dwNumberOfBytesTransferred);

	opInfo->AddRef();
	socketInfo->AddRef();
	g_socketList.Unlock();
	g_asyncList.Unlock();

	// Call the original routine, if it exists.  Use the original number of bytes sent instead of
	// the filtered value so that the application does not get confused.
	if (opInfo->GetKernelCompletionRoutine())
		opInfo->GetKernelCompletionRoutine()(dwErrorCode, dwNumberOfBytesTransferred, lpOverlapped);

	// Remove the operation from the list
	g_asyncList.CloseKernelOp(lpOverlapped);

	opInfo->Release();
	socketInfo->Release();
}


// WSCompletionAPCHandler is called on an alertable wait state after the SocketInfo
// class has queued up a simulated Winsock completion routine call.
//
// Parameters:
//   dwParam - Parameter passed to QueueUserAPC
//
// Return value:
//   None
void CALLBACK WSCompletionAPCHandler(ULONG_PTR dwParam)
{
	WSCompletionAPCData* data = (WSCompletionAPCData*)dwParam;
	NetCorrupt_WSRecvCompletionRoutine(data->dwError, data->cbTransferred, data->lpOverlapped, data->dwFlags);
	delete data;
}


// KernelCompletionAPCHandler is called on an alertable wait state after the SocketInfo
// class has queued up a simulated ReadFile/WriteFile completion routine call.
//
// Parameters:
//   dwParam - Parameter passed to QueueUserAPC
//
// Return value:
//   None
void CALLBACK KernelCompletionAPCHandler(ULONG_PTR dwParam)
{
	KernelCompletionAPCData* data = (KernelCompletionAPCData*)dwParam;
	NetCorrupt_KernelRecvCompletionRoutine(data->dwErrorCode, data->dwNumberOfBytesTransferred, data->lpOverlapped);
	delete data;
}


// NetCorrupt_WSSendCompletionRoutine is called by Winsock when an asynchronous send has completed
//
// Parameters:
//   dwError       - Error code
//   cbTransferred - Number of bytes transferred
//   lpOverlapped  - WSAOVERLAPPED structure used when starting the operation
//   dwFlags       - Completion flags
//
// Return value:
//   None
void CALLBACK NetCorrupt_WSSendCompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped,
											 DWORD dwFlags)
{
	// Find the AsyncInfo structure for this operation
	g_asyncList.Lock();
	AsyncOpInfo* info = g_asyncList.GetWSOpInfo(lpOverlapped);
	if (!info)
	{
		g_asyncList.Unlock();
		return;
	}

	info->AddRef();
	g_asyncList.Unlock();

	// Call the original routine, if it exists.  Use the original number of bytes sent instead of
	// the filtered value so that the application does not get confused.
	if (info->GetWSCompetionRoutine())
		info->GetWSCompetionRoutine()(dwError, info->GetTransferLength(), lpOverlapped, dwFlags);

	// Remove the operation from the list
	g_asyncList.CloseWSOp(lpOverlapped);

	info->Release();
}


// NetCorrupt_KernelSendCompletionRoutine is called by the kernel when an asynchronous WriteFile with a
// socket handle has completed
//
// Parameters:
//   dwErrorCode                - Error code
//   dwNumberOfBytesTransferred - Number of bytes transferred
//   lpOverlapped               - OVERLAPPED structure used when starting the operation
//
// Return value:
//   None
void CALLBACK NetCorrupt_KernelSendCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred,
												 LPOVERLAPPED lpOverlapped)
{
	// Find the AsyncInfo structure for this operation
	g_asyncList.Lock();
	AsyncOpInfo* info = g_asyncList.GetKernelOpInfo(lpOverlapped);
	if (!info)
	{
		g_asyncList.Unlock();
		return;
	}

	info->AddRef();
	g_asyncList.Unlock();

	// Call the original routine, if it exists.  Use the original number of bytes sent instead of
	// the filtered value so that the application does not get confused.
	if (info->GetKernelCompletionRoutine())
		info->GetKernelCompletionRoutine()(dwErrorCode, (DWORD)info->GetTransferLength(), lpOverlapped);

	// Remove the operation from the list
	g_asyncList.CloseKernelOp(lpOverlapped);

	info->Release();
}
