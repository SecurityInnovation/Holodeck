//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		SocketInfo.h
//
// DESCRIPTION: SocketInfo class holds information about a single open
//				socket.  Reference counted to prevent socket close
//				operations from deleting SocketInfo structures being used
//				in other threads.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Feb 2003		 R. Wagner	 File created.
//*************************************************************************

#ifndef __SOCKETINFO_H__
#define __SOCKETINFO_H__

#include "NetCorruptLibrary.h"
#include "DataQueue.h"

#include "SettingsManager.h"

using namespace Replacement;

// SocketInfo class holds information about a single open socket.  Reference counted
// to prevent socket close operations from deleting SocketInfo structures being used
// in other threads.
class SocketInfo
{
	static HANDLE heap;

	SOCKET m_socket;
	WORD m_wPort;
	DWORD m_dwRemoteIP;
	int m_iPacketType;
	int m_iProtocol;
	int m_iRefCount;
	bool m_fOverlapped;
	DataQueue m_recvQueue;
	DataQueue m_sendQueue;
	DWORD m_dwSendBytes, m_dwRecvBytes;
	DWORD m_dwRecvReplacePosition, m_dwSendReplacePosition;
	BOOL m_fRecvPositionReplaced, m_fSendPositionReplaced;
	BOOL m_fDisableSend, m_fDisableRecv;
	bool m_fBlocking, m_fForceNext;
	HANDLE m_hCompletionPort;
	ULONG_PTR m_dwCompletionPortKey;

	int AddDataToQueue(DataQueue& queue, const BYTE* buf, int len, bool send, DWORD& dwByteCount,
		DWORD& dwReplacePosition, BOOL& fPositionReplaced);
	int AddDataToQueueWithAddr(DataQueue& queue, const BYTE* buf, int len, bool send, DWORD& dwByteCount,
		DWORD& dwReplacePosition, BOOL& fPositionReplaced, const struct sockaddr* addr, const int* addrlen);
	int SendDataIntoQueue(const BYTE* buf, int len);
	int SendDataIntoQueueWithAddr(const BYTE* buf, int len, const struct sockaddr* addr, const int* addrlen);
	void AddSendingNetworkLog(const BYTE *buf, int len, const BYTE* oBuf, int oLen);
	void AddReceivingNetworkLog(const BYTE *buf, int len, const BYTE *oBuf, int oLen);

	static DWORD64	g_NetLimit_LastSendLen;
	static DWORD64	g_NetLimit_LastRecvLen;
	static DWORD	g_NetLimit_LastRecvStamp;
	static DWORD	g_NetLimit_LastSendStamp;
	static DWORD	g_NetLimit_SendMonitorStamp;
	static DWORD	g_NetLimit_SendMonitorBytes;
	static DWORD	g_NetLimit_RecvMonitorStamp;
	static DWORD	g_NetLimit_RecvMonitorBytes;
	static int		g_NetLimit_Check_Interval_ms;

public:
	SocketInfo(SOCKET s);

	void AddRef();
	void Release();

	WORD GetPort();
	DWORD GetRemoteIPAddress();
	int GetPacketType();
	int GetProtocol();
	bool IsOverlapped();
	int GetReceiveDataQueueLen();

	void SetPort(WORD p);
	void SetRemoteIPAddress(DWORD addr);
	void SetPacketType(int p);
	void SetProtocol(int p);
	void SetOverlapped(bool o);

	void DisableSend();
	void DisableReceive();
	bool IsSendDisabled();
	bool IsReceieveDisabled();

	void SetBlocking(bool blocking);
	bool IsBlocking();

	bool IsStreamBased();
	int PeekData(BYTE* buf, int len);
	int PeekDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen);

	int ReceiveData(BYTE* buf, int len);
	int ReceiveDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen);
	int ReceiveDataIntoQueue(BYTE* buf, int len, BYTE** originalData, unsigned long &originalDataLength);
	int ReceiveDataWSAsync(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, DWORD& dwByteTotal);
	BOOL ReceiveDataReadFileSync(BYTE* buf, DWORD len, DWORD& dwByteTotal);
	BOOL ReceiveDataReadFileAsync(BYTE* lpBuffer, DWORD dwBufferLen, LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, DWORD& dwByteTotal);

	void ReceiveWSCompletion(LPWSABUF lpBuffers, DWORD dwBufferCount, DWORD& dwByteTotal);
	void ReceiveReadFileCompletion(BYTE* lpBuffer, DWORD dwBufferLen, DWORD& dwByteTotal);

	int SendData(const BYTE* buf, int len);
	int SendDataWithAddr(const BYTE* buf, int len, const struct sockaddr* addr, const int* addrlen);
	BOOL SendDataWriteFileSync(const BYTE* buf, int len);
	int SendDataWSAsync(LPWSABUF lpBuffers, DWORD dwBufferCount, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped,
		LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, DWORD& dwByteTotal);
	BOOL SendDataWriteFileAsync(const BYTE* buf, int len, LPOVERLAPPED lpOverlapped,
		LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

	int GetReceiveLengthAfterLimit();
	bool HasMoreDataThanLimit();

	void ForceNextMessage();

	void SetCompletionPort(HANDLE hPort, ULONG_PTR dwKey);
};


#endif
