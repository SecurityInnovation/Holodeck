//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		AsyncOp.h
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

#ifndef __ASYNCOP_H__
#define __ASYNCOP_H__

#include "NetCorruptLibrary.h"


// WSCompletionAPCData contains data used to queue up a call to a Winsock asynchronous completion routine
struct WSCompletionAPCData
{
	DWORD dwError;
	DWORD cbTransferred;
	LPWSAOVERLAPPED lpOverlapped;
	DWORD dwFlags;
};

// KernelCompletionAPCData contains data used to queue up a call to a ReadFile asynchronous completion routine
struct KernelCompletionAPCData
{
	DWORD dwErrorCode;
	DWORD dwNumberOfBytesTransferred;
	LPOVERLAPPED lpOverlapped;
};


// AsyncOpInfo class contains information about an asynchronous operation.  See AsyncOpList class
// for a detailed description of what is required.  To make sure a AsyncOpInfo structure that
// needs to be maintained while the AsyncOpList class is not locked, call the AddRef/Release
// members of AsyncOpInfo.
class AsyncOpInfo
{
	static HANDLE heap;

	SOCKET m_socket;
	DWORD m_dwThreadId;
	int m_iOldTransferLen;	// Original transfer length (which will be passed back to the application)
	char *m_lpBuf;			// buffer sent to the async api.  Store here so we can delete when the operation completes
	bool m_fWinsockOp;
	union
	{
		LPOVERLAPPED m_lpOrigKOp;		// Original kernel managed async op (ReadFile/WriteFile)
		LPWSAOVERLAPPED m_lpOrigWSOp;	// Original Winsock managed async op
	};
	union
	{
		LPOVERLAPPED_COMPLETION_ROUTINE m_lpKCompletion;		// Kernel managed completion routine (ReadFile/WriteFile)
		LPWSAOVERLAPPED_COMPLETION_ROUTINE m_lpWSCompletion;	// Winsock managed completion routine
	};
	LPWSABUF m_lpWSBuffers;
	DWORD m_dwWSBufferCount;
	BYTE* m_lpKernelBuffer;
	DWORD m_dwKernelBufferLen;
	int m_iRefCount;

public:
	AsyncOpInfo(HANDLE s, LPOVERLAPPED op, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletion, char *buf, int iTransferLen);
	AsyncOpInfo(SOCKET s, LPWSAOVERLAPPED op, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletion, char *buf, int iTransferLen);
	~AsyncOpInfo();

	void AddRef();
	void Release();

	SOCKET GetSocket();
	bool IsWSAyncOp();
	LPOVERLAPPED GetKernelAsyncOp();
	LPWSAOVERLAPPED GetWSAsyncOp();
	LPOVERLAPPED_COMPLETION_ROUTINE GetKernelCompletionRoutine();
	LPWSAOVERLAPPED_COMPLETION_ROUTINE GetWSCompetionRoutine();
	int GetTransferLength();
	LPWSABUF GetWSBuffers();
	DWORD GetWSBufferCount();
	BYTE* GetKernelBuffer();
	DWORD GetKernelBufferLen();
	DWORD GetThreadId();

	void SetWSBuffers(LPWSABUF lpBuffers, DWORD dwBufferCount);
	void SetKernelBuffer(BYTE* lpBuffer, DWORD dwBufferLen);
	void SetTransferLength(int iTransferLen);
};


// AsyncOpList class holds a list of the currently executing asynchronous receive or
// send operations using either the Winsock functions or ReadFile/WriteFile.  Since
// the data sent/received by the application is different from the actual data going
// across the network, we may have to change the length of an asynchronous operation.
// To do this, we have to use a different LPOVERLAPPED structure, but we need to keep
// track of the application provided LPOVERLAPPED structure such that we can give
// the callback functions and GetOverlappedResult calls their expected behavior.
// These functions are based on the LPOVERLAPPED structure passed when starting
// the operation, which may not equal the one used by the application, thus requiring
// a form of mapping.
class AsyncOpList
{
	map<LPOVERLAPPED, AsyncOpInfo*> m_mKList;		// List of kernel managed async ops (ReadFile/WriteFile)
	map<LPWSAOVERLAPPED, AsyncOpInfo*> m_mWSList;	// List of Winsock managed async ops
	CRITICAL_SECTION m_csListCriticalSection;

public:
	AsyncOpList();
	~AsyncOpList();

	AsyncOpInfo* AddKernelOp(HANDLE s, LPOVERLAPPED lpOrigOp, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletion,
		char *buf, int iTransferLen);
	AsyncOpInfo* AddWSOp(SOCKET s, LPWSAOVERLAPPED lpOrigOp, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletion,
		char *buf, int iTransferLen);

	AsyncOpInfo* GetKernelOpInfo(LPOVERLAPPED op);
	AsyncOpInfo* GetWSOpInfo(LPWSAOVERLAPPED op);

	void CloseKernelOp(LPOVERLAPPED op);
	void CloseWSOp(LPWSAOVERLAPPED op);

	void CancelIo(HANDLE hFile);

	void Lock();
	void Unlock();
};


void CALLBACK NetCorrupt_WSRecvCompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped,
											 DWORD dwFlags);
void CALLBACK NetCorrupt_KernelRecvCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred,
												 LPOVERLAPPED lpOverlapped);
void CALLBACK WSCompletionAPCHandler(ULONG_PTR dwParam);
void CALLBACK KernelCompletionAPCHandler(ULONG_PTR dwParam);
void CALLBACK NetCorrupt_WSSendCompletionRoutine(DWORD dwError, DWORD cbTransferred, LPWSAOVERLAPPED lpOverlapped,
											 DWORD dwFlags);
void CALLBACK NetCorrupt_KernelSendCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransferred,
												 LPOVERLAPPED lpOverlapped);


extern AsyncOpList g_asyncList;


#endif
