//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		SocketInfo.cpp
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

#include "SocketInfo.h"
#include "AsyncOp.h"
#include "Corrupt.h"
#include "NetworkLogEntry.h"
#include "LogEvent.h"
#include "ReplacementLibrary.h"

DWORD64 SocketInfo::g_NetLimit_LastSendLen = 0;
DWORD64 SocketInfo::g_NetLimit_LastRecvLen = 0;
DWORD SocketInfo::g_NetLimit_LastSendStamp = 0;
DWORD SocketInfo::g_NetLimit_LastRecvStamp = 0;
DWORD SocketInfo::g_NetLimit_SendMonitorStamp = 0;
DWORD SocketInfo::g_NetLimit_SendMonitorBytes = 0;
DWORD SocketInfo::g_NetLimit_RecvMonitorStamp = 0;
DWORD SocketInfo::g_NetLimit_RecvMonitorBytes = 0;
HANDLE SocketInfo::heap = NULL;

int	SocketInfo::g_NetLimit_Check_Interval_ms = 200;


void WriteToDataQueue(const char* begin, const char* end, void* queuePtr)
{
	DataQueue* queue = (DataQueue*)queuePtr;
	queue->AppendData((const BYTE*)begin, (DWORD)(end - begin));
}


//*************************************************************************
// AddDataToQueue - will pass the data provided through the regular
// expression filters, and add the resulting data to the data queue.
// Returns the number of bytes added to the data queue.
//
// Parameters:
//   queue             - Data queue to add to
//   incomingBuf       - Buffer containing unmodified data to add
//   incomingLen       - Length of data in the buffer
//   modList           - Modification list to be used
//   dwByteCount       - Current count of bytes that have been sent/received on this socket
//   dwReplacePosition - Position in the stream where replacement should occur.  Ignored
//                       if the replacement type is not position replacement.
//   fPositionReplaced - true if the positional replacement has been completed, ignored
//                       if the replacement type is not position replacement.
//
// Return value:
//   Number of bytes added to the queue
//*************************************************************************
int SocketInfo::AddDataToQueue(DataQueue& queue, const BYTE* incomingBuf, int incomingLen, bool send, DWORD& dwByteCount,
	DWORD& dwReplacePosition, BOOL& fPositionReplaced)
{
	ModificationList modList;

	if (!settingsManager->GetNetworkCorruptionFaults(send, this->m_wPort, modList))
	{
		queue.AppendData(incomingBuf, incomingLen);
		return incomingLen;
	}

	DataQueue tempQueue;
	int retVal = (int)CorruptData((const char*)incomingBuf, (const char*)incomingBuf + incomingLen,
		modList, WriteToDataQueue, WriteToDataQueue, &tempQueue);

	// Place the corrupted data into the queue all at once, allowing datagream protocols to treat it
	// as a single message
	int len = tempQueue.GetRemainingDataLength();
	BYTE* data = new BYTE[len];
	tempQueue.RetrieveData(data, len);
	queue.AppendData(data, len);

	return retVal;
}

//*************************************************************************
// AddDataToQueueWithAddr - will pass the data provided through the regular
// expression filters, and add the resulting data to the data queue.
// Records the address the data was received from.  Returns the number of
// bytes added to the data queue.
//
// Parameters:
//   queue             - Data queue to add to
//   incomingBuf       - Buffer containing unmodified data to add
//   incomingLen       - Length of data in the buffer
//   modList           - Modification list to be used
//   dwByteCount       - Current count of bytes that have been sent/received on this socket
//   dwReplacePosition - Position in the stream where replacement should occur.  Ignored
//                       if the replacement type is not position replacement.
//   fPositionReplaced - true if the positional replacement has been completed, ignored
//                       if the replacement type is not position replacement.
//   addr			   - Address the data was received from
//   addrlen		   - Length of address
//
// Return value:
//   Number of bytes added to the queue
//*************************************************************************
int SocketInfo::AddDataToQueueWithAddr(DataQueue& queue, const BYTE* incomingBuf, int incomingLen, bool send,
	DWORD& dwByteCount, DWORD& dwReplacePosition, BOOL& fPositionReplaced, const struct sockaddr* addr,
	const int* addrlen)
{
	ModificationList modList;

	if (!settingsManager->GetNetworkCorruptionFaults(send, this->m_wPort, modList))
	{
		queue.AppendDataWithAddr(incomingBuf, incomingLen, addr, addrlen);
		return incomingLen;
	}

	DataQueue tempQueue;
	int retVal = (int)CorruptData((const char*)incomingBuf, (const char*)incomingBuf + incomingLen,
		modList, WriteToDataQueue, WriteToDataQueue, &tempQueue);

	// Place the corrupted data into the queue all at once, allowing datagream protocols to treat it
	// as a single message
	int len = tempQueue.GetRemainingDataLength();
	BYTE* data = new BYTE[len];
	tempQueue.RetrieveData(data, len);
	queue.AppendDataWithAddr(data, len, addr, addrlen);

	return retVal;
}

//*************************************************************************
// ReceiveDataIntoQueue will receive incoming data from the network and,
// after passing it through the regular expression filters, queue the
// data into the received data queue.  Returns the number of bytes added
// to the data queue, or -1 if there was an error.
//
// Parameters:
//   buf - Data buffer used for receving data
//   len - Length of data buffer
//
// Return value:
//   Number of bytes added to the receive queue, or -1 on error.
//*************************************************************************
int SocketInfo::ReceiveDataIntoQueue(BYTE* buf, int len, BYTE** originalData, unsigned long &originalDataLength)
{
	// We should block while waiting for data, go ahead and wait
	if (!realrecv)
		realrecv = (recvFunction)GetOriginalFunctionCaller("recv", "ws2_32.dll");
	if (!realrecvfrom)
		realrecvfrom = (recvfromFunction)GetOriginalFunctionCaller("recvfrom", "ws2_32.dll");

	int readCount;
	struct sockaddr from;
	int fromlen = sizeof(sockaddr);

	if (IsStreamBased())
		readCount = realrecv(m_socket, (char*)buf, len, 0);
	else
		readCount = realrecvfrom(m_socket, (char*)buf, len, 0, &from, &fromlen);

	if (readCount == -1)
		return -1;

	if (originalDataLength != -1)
	{
		if (*originalData == NULL)
			*originalData = (unsigned char *) HeapAlloc (heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (readCount));
		else 
			*originalData = (unsigned char *) HeapReAlloc (heap, HEAP_ZERO_MEMORY, originalData, sizeof(unsigned char) * (readCount));

		CopyMemory (*originalData, buf, readCount);
		originalDataLength = readCount;
	}

	// Add the received data to the data queue
	if (IsStreamBased())
	{
		return AddDataToQueue(m_recvQueue, buf, readCount, false, m_dwRecvBytes, m_dwRecvReplacePosition,
			m_fRecvPositionReplaced);
	}
	else
	{
		return AddDataToQueueWithAddr(m_recvQueue, buf, readCount, false, m_dwRecvBytes, m_dwRecvReplacePosition,
			m_fRecvPositionReplaced, &from, &fromlen);
	}
}


// SendDataIntoQueue will take outgoing data, pass it through the
// regular expression filters, and then queue the data into the
// outgoing data queue.  Returns the number of bytes added to the
// data queue.
//
// Parameters:
//   buf - Data buffer containing data being sent
//   len - Length of data buffer
//
// Return value:
//   Number of bytes added to the send queue
int SocketInfo::SendDataIntoQueue(const BYTE* buf, int len)
{
	// Pass along the sent data to the data queue
	return AddDataToQueue(m_sendQueue, buf, len, true, m_dwSendBytes, m_dwSendReplacePosition,
		m_fSendPositionReplaced);
}


// SendDataIntoQueueWithAddr will take outgoing data, pass it through the
// regular expression filters, and then queue the data into the
// outgoing data queue.  Records the destination address.  Returns the
// number of bytes added to the data queue.
//
// Parameters:
//   buf - Data buffer containing data being sent
//   len - Length of data buffer
//
// Return value:
//   Number of bytes added to the send queue
int SocketInfo::SendDataIntoQueueWithAddr(const BYTE* buf, int len, const struct sockaddr* addr, const int* addrlen)
{
	// Pass along the sent data to the data queue
	return AddDataToQueueWithAddr(m_sendQueue, buf, len, true, m_dwSendBytes, m_dwSendReplacePosition,
		m_fSendPositionReplaced, addr, addrlen);
}


// SocketInfo constructor
//
// Parameters:
//   s - Socket handle for this instance
//
// Return value:
//   None
SocketInfo::SocketInfo(SOCKET s)
{
	if (heap == NULL)
		heap = HeapCreate(0, 0, 0);

	m_socket = s;
	m_wPort = 0;
	m_dwRemoteIP = 0;
	m_iPacketType = -1;
	m_iProtocol = -1;
	m_fOverlapped = true;
	m_iRefCount = 1;
	m_dwSendBytes = 0;
	m_dwRecvBytes = 0;
	m_fDisableRecv = false;
	m_fDisableSend = false;
	m_fBlocking = true;
	m_fForceNext = false;
	m_hCompletionPort = NULL;
}


// AddRef increases the reference count of the SocketInfo object
//
// Parameters:
//   None
//
// Return value:
//   None
void SocketInfo::AddRef()
{
	m_iRefCount++;
}


// Release decreases the reference count of the SocketInfo object,
// and deletes the object if the reference count becomes zero.
//
// Parameters:
//   None
//
// Return value:
//   None
void SocketInfo::Release()
{
	m_iRefCount--;
	if (m_iRefCount == 0)
		delete this;
}


// GetPort retrieves the port used by this socket
//
// Parameters:
//   None
//
// Return value:
//   Port number
WORD SocketInfo::GetPort()
{
	return m_wPort;
}

// GetRemoteIPAddress retrieves the remote ip address used by this socket
//
// Parameters:
//   None
//
// Return value:
//   remote ip address in reverse network byte order
DWORD SocketInfo::GetRemoteIPAddress()
{
	return m_dwRemoteIP;
}


// GetPacketType retrieves the packet type used by this socket
//
// Parameters:
//   None
//
// Return value:
//   Packet type identification
int SocketInfo::GetPacketType()
{
	return m_iPacketType;
}

// GetProtocol retrieves the protocol used by this socket
//
// Parameters:
//   None
//
// Return value:
//   Protocol identification
int SocketInfo::GetProtocol()
{
	return m_iProtocol;
}


// IsOverlapped determines if the socket can do overlapped operations
//
// Parameters:
//   None
//
// Return value:
//   true if the socket can do overlapped operations, false otherwise.
bool SocketInfo::IsOverlapped()
{
	return m_fOverlapped;
}


// SetPort sets the port used by this socket
//
// Parameters:
//   p - Port number now used by this socket
//
// Return value:
//   None
void SocketInfo::SetPort(WORD p)
{
	m_wPort = p;
}

// SetRemoteIPAddress sets the remote ip address used by this socket
//
// Parameters:
//   addr - the ip address to set in reverse byte order
//
// Return value:
//   None
void SocketInfo::SetRemoteIPAddress(DWORD addr)
{
	m_dwRemoteIP = addr;
}

// SetPacketType sets the packet type used by this socket
//
// Parameters:
//   p - Packet type now used by this socket
//
// Return value:
//   None
void SocketInfo::SetPacketType(int p)
{
	m_iPacketType = p;

	if (IsStreamBased())
	{
		// Stream based socket (TCP, etc.), don't use message mode
		m_recvQueue.SetMessageMode(false);
		m_sendQueue.SetMessageMode(false);
	}
	else
	{
		// Non-stream based socket (UDP, etc.), use message mode
		m_recvQueue.SetMessageMode(true);
		m_sendQueue.SetMessageMode(true);
	}
}

// SetProtocol sets the protocol used by this socket
//
// Parameters:
//   p - Protocol now used by this socket
//
// Return value:
//   None
void SocketInfo::SetProtocol(int p)
{
	m_iProtocol = p;
}


// SetOverlapped sets whether the socket can do overlapped operations
//
// Parameters:
//   o - true if the socket can do overlapped operations, false otherwise
//
// Return value:
//   None
void SocketInfo::SetOverlapped(bool o)
{
	m_fOverlapped = o;
}


// IsStreamBased determines if the socket is a stream-based socket
//
// Parameters:
//   None
//
// Return value:
//   true if the socket is stream-based, false otherwise
bool SocketInfo::IsStreamBased()
{
	if (m_iPacketType == SOCK_STREAM)
		return true;
	if (m_iPacketType == IPPROTO_TCP)
		return true;
	return false;
}


// GetReceiveDataQueueLen retrieves the number of bytes waiting in
// the receive queue
//
// Parameters:
//   None
//
// Return value:
//   Number of bytes waiting in the receive queue
int SocketInfo::GetReceiveDataQueueLen()
{
	return m_recvQueue.GetRemainingDataLength();
}


// PeekData retrieves data waiting to be received on the socket, but
// does not remove it from the queue
//
// Parameters:
//   buf - Data buffer used to receive the data
//   len - Length of data buffer
//
// Return value:
//   Number of bytes in the receive queue, or -1 on error
int SocketInfo::PeekData(BYTE* buf, int len)
{
	// Apply network download limits to the length
	int maxLen = GetReceiveLengthAfterLimit();
	if (maxLen == -1)
	{
		WSASetLastError(WSAEWOULDBLOCK);
		return -1;
	}

	if (len > maxLen)
		len = maxLen;

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		// No data in the queue, so call the real recv.  Remove it
		// from Winsock's queue, but keep it in ours.
		if (ReceiveDataIntoQueue(buf, len, &originalDataBuffer, originalDataLength) == -1)
		{
			// recv error, return now
			return -1;
		}
		if (originalDataBuffer != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);
	}

	// Retrieve the data from the data queue, but don't remove it
	int readCount = m_recvQueue.PeekData(buf, len);
	// Don't write to the log yet, we will do that when we remove
	// it from the queue.  Just return the byte count.
	return readCount;
}

// PeekDataFrom retrieves data waiting to be received on the socket, but
// does not remove it from the queue.  This version will also retrieve
// the address the data was received from.
//
// Parameters:
//   buf - Data buffer used to receive the data
//   len - Length of data buffer
//   from - Receives address
//   fromlen - Address length
//
// Return value:
//   Number of bytes in the receive queue, or -1 on error
int SocketInfo::PeekDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen)
{
	// Apply network download limits to the length
	int maxLen = GetReceiveLengthAfterLimit();
	if (maxLen == -1)
	{
		WSASetLastError(WSAEWOULDBLOCK);
		return -1;
	}

	if (len > maxLen)
		len = maxLen;

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		// No data in the queue, so call the real recv.  Remove it
		// from Winsock's queue, but keep it in ours.
		if (ReceiveDataIntoQueue(buf, len, &originalDataBuffer, originalDataLength) == -1)
		{
			// recv error, return now
			return -1;
		}
		if (originalDataBuffer != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);
	}

	// Retrieve the data from the data queue, but don't remove it
	int readCount = m_recvQueue.PeekDataFrom(buf, len, from, fromlen);
	// Don't write to the log yet, we will do that when we remove
	// it from the queue.  Just return the byte count.
	return readCount;
}

//*************************************************************************
// Method:		AddSendingNetworkLog
// Description: Adds a sending network log to the current log entry
//
// Parameters:
//	buf - the buffer to hold the byte sequence
//	len - the size of the buffer
//
// Return Value: None
//*************************************************************************
void SocketInfo::AddSendingNetworkLog(const BYTE *buf, int len, const BYTE* oBuf, int oLen)
{
	Log::NetworkLogEntry networkLogEntry;
	networkLogEntry.SetData((unsigned char *)buf, len);
	networkLogEntry.SetOriginalData ((unsigned char *)oBuf, oLen);

	networkLogEntry.SetIsIncomingMessage(false);
	networkLogEntry.SetProtocol(GetProtocol());

	struct sockaddr_in address;
	int size = sizeof(address);
	getsockname(m_socket, (struct sockaddr *)&address, &size); 
	networkLogEntry.SetDestinationIPAddress(m_dwRemoteIP);
	networkLogEntry.SetDestinationPort(m_wPort);

	networkLogEntry.SetSourceIPAddress((unsigned long)address.sin_addr.S_un.S_addr);
	networkLogEntry.SetSourcePort(address.sin_port);

	unsigned int dataSize = networkLogEntry.GetLogSize();
	unsigned char *data = new unsigned char[dataSize];
	networkLogEntry.ConvertToByteSequence(data, dataSize);

	Log::LogEvent event;
	event.SetType(Log::NetworkLogEvent);
	event.SetData((char *)data, dataSize);

	ReplacementLibrary::AddEventToCurrentEntry(event);

	delete[] data;
}

//*************************************************************************
// Method:		AddReceivingNetworkLog
// Description: Adds a receiving network log to the current log entry
//
// Parameters:
//	buf - the buffer to hold the corrupted byte sequence
//	len - the size of the corrupted buffer
//	oBuf - the buffer to hold the original byte sequence
//	oLen - the size of the original buffer
//
// Return Value: None
//*************************************************************************
void SocketInfo::AddReceivingNetworkLog(const BYTE *buf, int len, const BYTE *oBuf, int oLen)
{
	Log::NetworkLogEntry networkLogEntry;
	networkLogEntry.SetData((unsigned char *)buf, len);
	networkLogEntry.SetOriginalData ((unsigned char *)oBuf, oLen);

	networkLogEntry.SetIsIncomingMessage(true);
	networkLogEntry.SetProtocol(GetProtocol());

	struct sockaddr_in address;
	int size = sizeof(address);
	getsockname(m_socket, (struct sockaddr *)&address, &size); 
	networkLogEntry.SetSourceIPAddress(m_dwRemoteIP);
	networkLogEntry.SetSourcePort(m_wPort);

	networkLogEntry.SetDestinationIPAddress(address.sin_addr.S_un.S_addr);
	networkLogEntry.SetDestinationPort(address.sin_port);

	unsigned int dataSize = networkLogEntry.GetLogSize();
	unsigned char *data = new unsigned char[dataSize];
	networkLogEntry.ConvertToByteSequence(data, dataSize);

	Log::LogEvent event;
	event.SetType(Log::NetworkLogEvent);
	event.SetData((char *)data, dataSize);

	ReplacementLibrary::AddEventToCurrentEntry(event);

	delete[] data;
}

//*************************************************************************
// ReceiveData simulates the blocking form of the recv function, and takes data
// from the data queue first before going out to the real network.
//
// Parameters:
//   buf - Data buffer used to receive the data
//   len - Length of data buffer
//
// Return value:
//   Number of bytes received, or -1 on error
//*************************************************************************
int SocketInfo::ReceiveData(BYTE* buf, int len)
{
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	DWORD64 downloadSpeed = 0;
	int uLen = len;
	bool IsDownloadControlEnabled = settingsManager->GetCachedNetworkDownloadBandwidth (downloadSpeed);
	downloadSpeed /= 8; // Speed is given in bits/sec, convert to bytes/sec

	// If datagram protocol, we don't know how big the next message is going to be, so apply
	// limit using past data only
	if (!IsStreamBased())
		uLen = 0;

	DWORD currentTick = GetTickCount();
	
	// update monitor information every 1 second
	if (currentTick - g_NetLimit_RecvMonitorStamp > 1000)
	{
		settingsManager->SetDownloadBandwidthUsage (g_NetLimit_RecvMonitorBytes * 8, currentTick);
		g_NetLimit_RecvMonitorStamp = currentTick;
	}

	if (currentTick - g_NetLimit_LastRecvStamp > (DWORD) g_NetLimit_Check_Interval_ms)
	{
		g_NetLimit_LastRecvLen -= (downloadSpeed * (DWORD64)(currentTick - g_NetLimit_LastRecvStamp)) / 1000;
		if ((__int64)g_NetLimit_LastRecvLen < 0)
			g_NetLimit_LastRecvLen = 0;
		g_NetLimit_LastRecvStamp = currentTick;
	}

	if (IsDownloadControlEnabled)
	{
		if (g_NetLimit_LastRecvLen + uLen >= (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
		{
			uLen = (int) ((downloadSpeed /(1000 / g_NetLimit_Check_Interval_ms)) - g_NetLimit_LastRecvLen);
			if (uLen <= 0)
			{
				if ((!IsBlocking()) && (!m_fForceNext))
				{
					WSASetLastError(WSAEWOULDBLOCK);
					return -1;
				}

				Sleep (g_NetLimit_Check_Interval_ms - (currentTick - g_NetLimit_LastRecvStamp));
				g_NetLimit_LastRecvLen = 0;
				g_NetLimit_LastRecvStamp = GetTickCount();

				if (len > (int) (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
					uLen = downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
				else
					uLen = len;
			}
		}
	}

	m_fForceNext = false;

	// Always receive whole messages in datagram mode
	if (!IsStreamBased())
		uLen = len;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		// Only call real recv if there isn't any data waiting in the recv queue
		if (ReceiveDataIntoQueue(buf, uLen, &originalDataBuffer, originalDataLength) == -1)
		{
			// recv error, return now
			return -1;
		}
	}

	// Retrieve the data from the data queue
	int readCount = m_recvQueue.RetrieveData(buf, uLen);
	AddReceivingNetworkLog(buf, readCount, originalDataBuffer, originalDataLength);

	if (originalDataBuffer != NULL)	
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	g_NetLimit_RecvMonitorBytes += readCount;

	if (IsDownloadControlEnabled)
		g_NetLimit_LastRecvLen += readCount;
	
	return readCount;
}

//*************************************************************************
// ReceiveDataFrom simulates the blocking form of the recv function, and takes data
// from the data queue first before going out to the real network.  This
// version also returns the address the data was received from.
//
// Parameters:
//   buf - Data buffer used to receive the data
//   len - Length of data buffer
//   from - Receives address
//   fromlen - Address length
//
// Return value:
//   Number of bytes received, or -1 on error
//*************************************************************************
int SocketInfo::ReceiveDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen)
{
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	DWORD64 downloadSpeed = 0;
	int uLen = len;
	bool IsDownloadControlEnabled = settingsManager->GetCachedNetworkDownloadBandwidth (downloadSpeed);
	downloadSpeed /= 8; // Speed is given in bits/sec, convert to bytes/sec

	// If datagram protocol, we don't know how big the next message is going to be, so apply
	// limit using past data only
	if (!IsStreamBased())
		uLen = 0;

	DWORD currentTick = GetTickCount();
	
	// update monitor information every 1 second
	if (currentTick - g_NetLimit_RecvMonitorStamp > 1000)
	{
		settingsManager->SetDownloadBandwidthUsage (g_NetLimit_RecvMonitorBytes * 8, currentTick);
		g_NetLimit_RecvMonitorStamp = currentTick;
	}

	if (currentTick - g_NetLimit_LastRecvStamp > (DWORD) g_NetLimit_Check_Interval_ms)
	{
		g_NetLimit_LastRecvLen -= (downloadSpeed * (DWORD64)(currentTick - g_NetLimit_LastRecvStamp)) / 1000;
		if ((__int64)g_NetLimit_LastRecvLen < 0)
			g_NetLimit_LastRecvLen = 0;
		g_NetLimit_LastRecvStamp = currentTick;
	}

	if (IsDownloadControlEnabled)
	{
		if (g_NetLimit_LastRecvLen + uLen >= (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
		{
			uLen = (int) ((downloadSpeed /(1000 / g_NetLimit_Check_Interval_ms)) - g_NetLimit_LastRecvLen);
			if (uLen <= 0)
			{
				if ((!IsBlocking()) && (!m_fForceNext))
				{
					WSASetLastError(WSAEWOULDBLOCK);
					return -1;
				}

				Sleep (g_NetLimit_Check_Interval_ms - (currentTick - g_NetLimit_LastRecvStamp));
				g_NetLimit_LastRecvLen = 0;
				g_NetLimit_LastRecvStamp = GetTickCount();

				if (len > (int) (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
					uLen = downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
				else
					uLen = len;
			}
		}
	}

	m_fForceNext = false;

	// Always receive whole messages in datagram mode
	if (!IsStreamBased())
		uLen = len;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		// Only call real recv if there isn't any data waiting in the recv queue
		if (ReceiveDataIntoQueue(buf, uLen, &originalDataBuffer, originalDataLength) == -1)
		{
			// recv error, return now
			return -1;
		}
	}

	// Retrieve the data from the data queue
	int readCount = m_recvQueue.RetrieveDataFrom(buf, uLen, from, fromlen);
	AddReceivingNetworkLog(buf, readCount, originalDataBuffer, originalDataLength);

	if (originalDataBuffer != NULL)	
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	g_NetLimit_RecvMonitorBytes += readCount;

	if (IsDownloadControlEnabled)
		g_NetLimit_LastRecvLen += readCount;
	
	return readCount;
}

// ReceiveDataWSAsync simulates the asynchronous form of the WSARecv function, and takes data
// from the data queue first before going out to the real network.
//
// Parameters:
//   lpBuffers           - Data buffers used for receiving data
//   dwBufferCount       - Number of data buffers
//   lpFlags             - Pointer to flags
//   lpOverlapped        - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine - Routine to be called when the opeartion completes
//   dwByteTotal         - Receives number of bytes received
//
// Return value:
//   Zero on immediate completion, SOCKET_ERROR otherwise.  WSAGetLastError
//   returns WSA_IO_PENDING on a pending asynchronous request.
int SocketInfo::ReceiveDataWSAsync(LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpFlags, LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, DWORD& dwByteTotal)
{
	if (!realWSARecv)
		realWSARecv = (WSARecvFunction)GetOriginalFunctionCaller("WSARecv", "ws2_32.dll");

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataOffset = 0;

	bool calledOriginal = false;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		calledOriginal = true;

		// Keep track of the upcoming asynchronous call
		g_asyncList.Lock();
		AsyncOpInfo* info = g_asyncList.AddWSOp(m_socket, lpOverlapped, lpCompletionRoutine, NULL, 0);
		info->SetWSBuffers(lpBuffers, dwBufferCount);

		// Call real WSARecv since there isn't any data waiting in the recv queue
		int result = realWSARecv(m_socket, lpBuffers, dwBufferCount, &dwByteTotal, lpFlags, lpOverlapped,
			NetCorrupt_WSRecvCompletionRoutine);

		if ((result != 0) && (WSAGetLastError() == WSAEINVAL))
		{
			// Error is either due to an associated completion port or an invalid parameter.  Try without
			// completion routine to cover the completion port case.  In the case of an invalid parameter,
			// the call will still fail and we can pass the error to the application.
			result = realWSARecv(m_socket, lpBuffers, dwBufferCount, &dwByteTotal, lpFlags, lpOverlapped, NULL);
		}

		if (result == 0)
		{
			// Data is already present, don't need to wait for the completion routine
			g_asyncList.CloseWSOp(lpOverlapped);

			// Add the data to the data queue
			for (DWORD i=0; (i<dwBufferCount) && (dwByteTotal > 0); i++)
			{
				DWORD len = dwByteTotal;
				if (len > lpBuffers[i].len)
					len = lpBuffers[i].len;
				dwByteTotal -= len;

				if (originalDataBuffer == NULL)
					originalDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (lpBuffers[i].len));
				else
					originalDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, originalDataBuffer, sizeof(unsigned char) * (originalDataOffset + lpBuffers[i].len));
				
				CopyMemory(originalDataBuffer + originalDataOffset, (const BYTE*)lpBuffers[i].buf, lpBuffers[i].len);
				originalDataOffset += lpBuffers[i].len;

				AddDataToQueue(m_recvQueue, (const BYTE*)lpBuffers[i].buf, len, false,
					m_dwRecvBytes, m_dwRecvReplacePosition, m_fRecvPositionReplaced);
			}
			g_asyncList.Unlock();
			// Fall through to retrieve the modified data out of the queue
		}
		else
		{
			// Data retrieval is not completed yet, just return to the application
			g_asyncList.Unlock();
			return result;
		}
	}

	// Retrieve the data from the data queue
	unsigned char *corruptedDataBuffer = NULL;
	unsigned long corruptedDataOffset = 0;
	dwByteTotal = 0;
	for (DWORD i=0; i<dwBufferCount; i++)
	{
		// Obtain one chunk of data from the data queue
		DWORD readCount = m_recvQueue.RetrieveData((BYTE*)lpBuffers[i].buf, lpBuffers[i].len);

		if (corruptedDataBuffer == NULL)
			corruptedDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (readCount));
		else
			corruptedDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer, sizeof(unsigned char) * (corruptedDataOffset + readCount));
		
		CopyMemory(corruptedDataBuffer + corruptedDataOffset, (BYTE*)lpBuffers[i].buf, readCount);
		corruptedDataOffset += readCount;

		dwByteTotal += readCount;
		// If we didn't fill the buffer, we're out of data
		if (readCount < lpBuffers[i].len)
			break;
	}

	AddReceivingNetworkLog(corruptedDataBuffer, corruptedDataOffset, originalDataBuffer, originalDataOffset);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);
	if (corruptedDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer);

	// Add the operation to the asynchronous operation table
	g_asyncList.Lock();
	g_asyncList.AddWSOp(m_socket, lpOverlapped, lpCompletionRoutine, NULL, dwByteTotal);
	g_asyncList.Unlock();

	if (!calledOriginal)
	{
		if (lpCompletionRoutine)
		{
			// Need to queue up a completion routine
			WSCompletionAPCData* data = new WSCompletionAPCData;
			data->dwError = 0;
			data->cbTransferred = dwByteTotal;
			data->lpOverlapped = lpOverlapped;
			data->dwFlags = 0;
			QueueUserAPC(WSCompletionAPCHandler, GetCurrentThread(), (ULONG_PTR)data);
		}

		if (m_hCompletionPort)
			PostQueuedCompletionStatus(m_hCompletionPort, dwByteTotal, m_dwCompletionPortKey, lpOverlapped);
	}

	return 0; // Completed
}


// ReceiveData simulates the blocking form of the recv function, and takes data
// from the data queue first before going out to the real network.
//
// Parameters:
//   buf         - Data buffer used to receive data
//   len         - Length of data buffer
//   dwByteTotal - Receives number of bytes received
//
// Return value:
//   TRUE on success, FALSE on failure
BOOL SocketInfo::ReceiveDataReadFileSync(BYTE* buf, DWORD len, DWORD& dwByteTotal)
{
	if (!realReadFile)
		realReadFile = (ReadFileFunction)GetOriginalFunctionCaller("ReadFile", "kernel32.dll");

	// Retrieve the data from the data queue
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		// Only call real ReadFile if there isn't any data waiting in the recv queue
		int readCount;
		if (!realReadFile((HANDLE)m_socket, buf, len, (LPDWORD)&readCount, NULL))
			return FALSE;

		if (originalDataBuffer == NULL)
			originalDataBuffer = (unsigned char *)HeapAlloc (heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (readCount));
		
		CopyMemory (originalDataBuffer, buf, readCount);
		originalDataLength = readCount;

		// Add the received data to the data queue
		AddDataToQueue(m_recvQueue, buf, readCount, false, m_dwRecvBytes, m_dwRecvReplacePosition,
			m_fRecvPositionReplaced);
	}

	// Retrieve the data from the data queue
	dwByteTotal = m_recvQueue.RetrieveData(buf, len);
	AddReceivingNetworkLog(buf, len, originalDataBuffer, originalDataLength);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	SetLastError(ERROR_SUCCESS);
	return TRUE;
}


// ReceiveDataReadFileAsync simulates the asynchronous form of the ReadFile function, and takes data
// from the data queue first before going out to the real network.
//
// Parameters:
//   lpBuffer            - Data buffer used to receive data
//   dwBufferLen         - Length of data buffer
//   lpOverlapped        - OVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine - Routine to be called when the operation completes
//   dwByteTotal         - Receives number of bytes received
//
// Return value:
//   TRUE on success, FALSE on failure
BOOL SocketInfo::ReceiveDataReadFileAsync(BYTE* lpBuffer, DWORD dwBufferLen, LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, DWORD& dwByteTotal)
{
	if (!realReadFileEx)
		realReadFileEx = (ReadFileExFunction)GetOriginalFunctionCaller("ReadFileEx", "kernel32.dll");
	if (!realReadFile)
		realReadFile = (ReadFileFunction)GetOriginalFunctionCaller("ReadFile", "kernel32.dll");
	if (!realGetOverlappedResult)
		realGetOverlappedResult = (GetOverlappedResultFunction)GetOriginalFunctionCaller("GetOverlappedResult", "kernel32.dll");

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	bool calledOriginal = false;

	if (m_recvQueue.GetRemainingDataLength() == 0)
	{
		calledOriginal = true;

		// Keep track of the upcoming asynchronous call
		g_asyncList.Lock();
		AsyncOpInfo* info = g_asyncList.AddKernelOp((HANDLE)m_socket, lpOverlapped, lpCompletionRoutine, NULL, 0);
		info->SetKernelBuffer(lpBuffer, dwBufferLen);

		// Call real ReadFileEx since there isn't any data waiting in the recv queue
		BOOL result = realReadFileEx((HANDLE)m_socket, lpBuffer, dwBufferLen, lpOverlapped,
			NetCorrupt_KernelRecvCompletionRoutine);

		if ((result == FALSE) && (GetLastError() == ERROR_INVALID_PARAMETER))
		{
			// Error is either due to an associated completion port or an invalid parameter.  Try without
			// completion routine to cover the completion port case.  In the case of an invalid parameter,
			// the call will still fail and we can pass the error to the application.
			result = realReadFile((HANDLE)m_socket, lpBuffer, dwBufferLen, &dwByteTotal, lpOverlapped);
		}

		if (result)
		{
			// Data is already present, don't need to wait for the completion routine
			g_asyncList.CloseWSOp(lpOverlapped);

			// Get the length of the data
			realGetOverlappedResult((HANDLE)m_socket, lpOverlapped, &dwByteTotal, TRUE);

			if (originalDataBuffer == NULL)
				originalDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (dwByteTotal));
			
			CopyMemory(originalDataBuffer, lpBuffer, dwByteTotal);
			originalDataLength += dwByteTotal;

			// Add the data to the data queue
			AddDataToQueue(m_recvQueue, lpBuffer, dwByteTotal, false,
					m_dwRecvBytes, m_dwRecvReplacePosition, m_fRecvPositionReplaced);
			g_asyncList.Unlock();

			// Fall through to retrieve the modified data out of the queue
		}
		else
		{
			// Data retrieval is not completed yet, just return to the application
			g_asyncList.Unlock();
			return result;
		}
	}

	// Retrieve the data from the data queue
	dwByteTotal = m_recvQueue.RetrieveData(lpBuffer, dwBufferLen);
	AddReceivingNetworkLog(lpBuffer, dwBufferLen, originalDataBuffer, originalDataLength);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	// Add the operation to the asynchronous operation table
	g_asyncList.Lock();
	g_asyncList.AddKernelOp((HANDLE)m_socket, lpOverlapped, lpCompletionRoutine, NULL, dwByteTotal);
	g_asyncList.Unlock();

	if (!calledOriginal)
	{
		if (lpCompletionRoutine)
		{
			// Need to queue up a completion routine
			KernelCompletionAPCData* data = new KernelCompletionAPCData;
			data->dwErrorCode = 0;
			data->dwNumberOfBytesTransferred = dwByteTotal;
			data->lpOverlapped = lpOverlapped;
			QueueUserAPC(KernelCompletionAPCHandler, GetCurrentThread(), (ULONG_PTR)data);
		}

		if (m_hCompletionPort)
			PostQueuedCompletionStatus(m_hCompletionPort, dwByteTotal, m_dwCompletionPortKey, lpOverlapped);
	}

	return TRUE; // Completed
}


// ReceiveWSCompletion is called when an asynchronous Winsock receive operation has completed
//
// Parameters:
//   lpBuffers     - Data buffers used to receive data
//   dwBufferCount - Number of data buffers
//   dwByteTotal   - Number of bytes received.  Receieves number of bytes received
//                   after data modification.
//
// Return value:
//   None
void SocketInfo::ReceiveWSCompletion(LPWSABUF lpBuffers, DWORD dwBufferCount, DWORD& dwByteTotal)
{
	if (lpBuffers == NULL)
	{
		// No buffers, data was already passed through the corruption system
		return;
	}

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataOffset = 0;

	// Add the data to the data queue
	for (DWORD i=0; (i<dwBufferCount) && (dwByteTotal > 0); i++)
	{
		DWORD len = dwByteTotal;
		if (len > lpBuffers[i].len)
			len = lpBuffers[i].len;
		dwByteTotal -= len;

		if (originalDataBuffer == NULL)
			originalDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (len));
		else
			originalDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, originalDataBuffer, sizeof(unsigned char) * (originalDataOffset + len));
		
		CopyMemory(originalDataBuffer + originalDataOffset, (const BYTE*)lpBuffers[i].buf, len);
		originalDataOffset += len;

		AddDataToQueue(m_recvQueue, (const BYTE*)lpBuffers[i].buf, len, false,
			m_dwRecvBytes, m_dwRecvReplacePosition, m_fRecvPositionReplaced);
	}

	// Retrieve the data from the data queue
	unsigned char *corruptedDataBuffer = NULL;
	unsigned long corruptedDataOffset = 0;
	dwByteTotal = 0;
	for (DWORD i=0; i<dwBufferCount; i++)
	{
		// Obtain one chunk of data from the data queue
		DWORD readCount = m_recvQueue.RetrieveData((BYTE*)lpBuffers[i].buf, lpBuffers[i].len);
		
		if (corruptedDataBuffer == NULL)
			corruptedDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (readCount));
		else
			corruptedDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer, sizeof(unsigned char) * (corruptedDataOffset + readCount));
		
		CopyMemory(corruptedDataBuffer + corruptedDataOffset, (BYTE*)lpBuffers[i].buf, readCount);
		corruptedDataOffset += readCount;

		dwByteTotal += readCount;
		// If we didn't fill the buffer, we're out of data
		if (readCount < lpBuffers[i].len)
			break;
	}
	
	AddReceivingNetworkLog(corruptedDataBuffer, corruptedDataOffset, originalDataBuffer, originalDataOffset);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);
	if (corruptedDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer);
}


// ReceiveReadFileCompletion is called when an asynchronous ReadFile receive operation has completed
//
// Parameters:
//   lpBuffer    - Data buffer used to receive data
//   dwBufferLen - Length of data buffer
//   dwByteTotal - Number of bytes received.  Receieves number of bytes received
//                 after data modification.
//
// Return value:
//   None
void SocketInfo::ReceiveReadFileCompletion(BYTE* lpBuffer, DWORD dwBufferLen, DWORD& dwByteTotal)
{
	if (lpBuffer == NULL)
	{
		// No buffer, data was already passed through the corruption system
		return;
	}

	// Retrieve the data from the data queue
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	if (originalDataBuffer == NULL)
		originalDataBuffer = (unsigned char *)HeapAlloc (heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (dwBufferLen));
	
	CopyMemory (originalDataBuffer, lpBuffer, dwBufferLen);
	originalDataLength = dwBufferLen;

	// Add the data to the data queue
	AddDataToQueue(m_recvQueue, lpBuffer, dwByteTotal, false,
		m_dwRecvBytes, m_dwRecvReplacePosition, m_fRecvPositionReplaced);

	// Retrieve the data from the data queue
	dwByteTotal = m_recvQueue.RetrieveData(lpBuffer, dwBufferLen);
	AddReceivingNetworkLog(lpBuffer, dwBufferLen, originalDataBuffer, originalDataLength);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);
}

//*************************************************************************
// SendData simulates the blocking form of the send function, passing it through
// the data queue and the data modification filters before going to the real network.
//
// Parameters:
//   buf - Data buffer containing data to be sent
//   len - Length of data buffer
//
// Return value:
//   Number of bytes sent, or -1 on failure.
//*************************************************************************
int SocketInfo::SendData(const BYTE* buf, int len)
{
	if (!realsend)
		realsend = (sendFunction)GetOriginalFunctionCaller("send", "ws2_32.dll");

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	unsigned char *corruptedDataBuffer = NULL;
	unsigned long corruptedDataOffset = 0;

	if (originalDataBuffer == NULL)
		originalDataBuffer = (unsigned char *)HeapAlloc (heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (len));
	
	CopyMemory (originalDataBuffer, buf, len);
	originalDataLength = len;

	// Enqueue the data
	int bytesToSend = SendDataIntoQueue(buf, len);
	int sendLength = 0;
	int bytesSent = 0;
	DWORD monitorTickCount = 0;

	// Get current time stamp
	DWORD currentTick = GetTickCount();

	DWORD64 uploadSpeed = 0;
	bool IsUploadControlEnabled = settingsManager->GetCachedNetworkUploadBandwidth (uploadSpeed);
	uploadSpeed /= 8; // Speed is given in bits/sec, convert to bytes/sec

	int minSize = 1;
	// Must send the entire message at once for datagram protocols
	if (!IsStreamBased())
		minSize = bytesToSend;

	if (IsUploadControlEnabled)
	{
		if (currentTick - g_NetLimit_LastSendStamp > (DWORD) g_NetLimit_Check_Interval_ms)
		{
			g_NetLimit_LastSendStamp = currentTick;
			g_NetLimit_LastSendLen = 0;
		}
		
		sendLength = (int) (uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms)) - g_NetLimit_LastSendLen;

		if (sendLength > bytesToSend)
			sendLength = bytesToSend;
		
		if (sendLength <= 0)
		{
			Sleep (g_NetLimit_Check_Interval_ms - (currentTick - g_NetLimit_LastSendStamp));

			currentTick = GetTickCount();
			if (bytesToSend > (int) (uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
				sendLength = uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
			else
				sendLength = bytesToSend;
			g_NetLimit_LastSendStamp = currentTick;
			g_NetLimit_LastSendLen = 0;
		}
	}
	
	// Always send whole message in datagram protocols
	if (!IsStreamBased())
		bytesToSend = len;

	while (bytesToSend != 0)
	{
		// update monitor information every 1 second
		monitorTickCount = GetTickCount();
		if (monitorTickCount - g_NetLimit_SendMonitorStamp > 1000)
		{
			settingsManager->SetUploadBandwidthUsage (g_NetLimit_SendMonitorBytes * 8, monitorTickCount);
			g_NetLimit_SendMonitorStamp = monitorTickCount;
		}
		
		// Retrieve the filtered data out of the queue
		if (!IsUploadControlEnabled)
			sendLength = m_sendQueue.GetRemainingDataLength();

		BYTE* tempBuf = new BYTE[sendLength];
		m_sendQueue.RetrieveData(tempBuf, sendLength);

		if (corruptedDataBuffer == NULL)
			corruptedDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (sendLength));
		else
			corruptedDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer, sizeof(unsigned char) * (corruptedDataOffset + sendLength));
		
		CopyMemory(corruptedDataBuffer + corruptedDataOffset, tempBuf, sendLength);
		corruptedDataOffset += sendLength;

		// Send the data over the network
		if ((bytesSent = realsend(m_socket, (const char*)tempBuf, sendLength, 0)) == -1)
		{
			// send failed
			return -1;
		}
		delete[] tempBuf;

		bytesToSend -= bytesSent;
		g_NetLimit_LastSendLen += bytesSent;
		g_NetLimit_SendMonitorBytes += bytesSent;
		
		if ((bytesToSend > 0)&&(IsUploadControlEnabled))
		{
			int sleepLen = (int)(g_NetLimit_Check_Interval_ms - (GetTickCount() - currentTick));
			if (sleepLen > 0)
				Sleep (sleepLen);

			currentTick = GetTickCount();
			if (bytesToSend > (int) (uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
				sendLength = uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
			else
				sendLength = bytesToSend;
			
			g_NetLimit_LastSendStamp = currentTick;
			g_NetLimit_LastSendLen = 0;
		}
	}

	AddSendingNetworkLog(corruptedDataBuffer, corruptedDataOffset, originalDataBuffer, originalDataLength);

	if (corruptedDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer);
	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	return len;
}

//*************************************************************************
// SendData simulates the blocking form of the send function, passing it through
// the data queue and the data modification filters before going to the real network.
//
// Parameters:
//   buf - Data buffer containing data to be sent
//   len - Length of data buffer
//   addr - Destination address
//   addrlen - Length of address
//
// Return value:
//   Number of bytes sent, or -1 on failure.
//*************************************************************************
int SocketInfo::SendDataWithAddr(const BYTE* buf, int len, const struct sockaddr* addr, const int* addrlen)
{
	if (!realsendto)
		realsendto = (sendtoFunction)GetOriginalFunctionCaller("sendto", "ws2_32.dll");

	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	unsigned char *corruptedDataBuffer = NULL;
	unsigned long corruptedDataOffset = 0;

	if (originalDataBuffer == NULL)
		originalDataBuffer = (unsigned char *)HeapAlloc (heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (len));
	
	CopyMemory (originalDataBuffer, buf, len);
	originalDataLength = len;

	// Enqueue the data
	int bytesToSend = SendDataIntoQueueWithAddr(buf, len, addr, addrlen);
	int sendLength = 0;
	int bytesSent = 0;
	DWORD monitorTickCount = 0;

	// Get current time stamp
	DWORD currentTick = GetTickCount();

	DWORD64 uploadSpeed = 0;
	bool IsUploadControlEnabled = settingsManager->GetCachedNetworkUploadBandwidth (uploadSpeed);
	uploadSpeed /= 8; // Speed is given in bits/sec, convert to bytes/sec

	int minSize = 1;
	// Must send the entire message at once for datagram protocols
	if (!IsStreamBased())
		minSize = bytesToSend;

	if (IsUploadControlEnabled)
	{
		if (currentTick - g_NetLimit_LastSendStamp > (DWORD) g_NetLimit_Check_Interval_ms)
		{
			g_NetLimit_LastSendStamp = currentTick;
			g_NetLimit_LastSendLen = 0;
		}
		
		sendLength = (int) (uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms)) - g_NetLimit_LastSendLen;

		if (sendLength > bytesToSend)
			sendLength = bytesToSend;
		
		if (sendLength < minSize)
		{
			Sleep (g_NetLimit_Check_Interval_ms - (currentTick - g_NetLimit_LastSendStamp));

			currentTick = GetTickCount();
			if (bytesToSend > (int) (uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
				sendLength = uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
			else
				sendLength = bytesToSend;
			g_NetLimit_LastSendStamp = currentTick;
			g_NetLimit_LastSendLen = 0;
		}
	}
	
	// Always send whole message in datagram protocols
	if (!IsStreamBased())
		bytesToSend = len;

	while (bytesToSend != 0)
	{
		// update monitor information every 1 second
		monitorTickCount = GetTickCount();
		if (monitorTickCount - g_NetLimit_SendMonitorStamp > 1000)
		{
			settingsManager->SetUploadBandwidthUsage (g_NetLimit_SendMonitorBytes * 8, monitorTickCount);
			g_NetLimit_SendMonitorStamp = monitorTickCount;
		}
		
		// Retrieve the filtered data out of the queue
		if (!IsUploadControlEnabled)
			sendLength = m_sendQueue.GetRemainingDataLength();

		struct sockaddr from;
		int fromlen = sizeof(sockaddr);

		BYTE* tempBuf = new BYTE[sendLength];
		m_sendQueue.RetrieveDataFrom(tempBuf, sendLength, &from, &fromlen);

		if (corruptedDataBuffer == NULL)
			corruptedDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (sendLength));
		else
			corruptedDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer, sizeof(unsigned char) * (corruptedDataOffset + sendLength));
		
		CopyMemory(corruptedDataBuffer + corruptedDataOffset, tempBuf, sendLength);
		corruptedDataOffset += sendLength;

		// Send the data over the network
		if ((bytesSent = realsendto(m_socket, (const char*)tempBuf, sendLength, 0, &from, fromlen)) == -1)
		{
			// send failed
			return -1;
		}
		delete[] tempBuf;

		bytesToSend -= bytesSent;
		g_NetLimit_LastSendLen += bytesSent;
		g_NetLimit_SendMonitorBytes += bytesSent;
		
		if ((bytesToSend > 0)&&(IsUploadControlEnabled))
		{
			int sleepLen = (int)(g_NetLimit_Check_Interval_ms - (GetTickCount() - currentTick));
			if (sleepLen > 0)
				Sleep (sleepLen);

			currentTick = GetTickCount();
			if (bytesToSend > (int) (uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
				sendLength = uploadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
			else
				sendLength = bytesToSend;
			
			g_NetLimit_LastSendStamp = currentTick;
			g_NetLimit_LastSendLen = 0;
		}
	}

	AddSendingNetworkLog(corruptedDataBuffer, corruptedDataOffset, originalDataBuffer, originalDataLength);

	if (corruptedDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, corruptedDataBuffer);
	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	return len;
}


// SendDataWriteFileSync simulates the blocking form of the WriteFile function with a
// socket handle, passing it through the data queue and the data modification
// filters before going to the real network.
//
// Parameters:
//   buf - Data buffer containing data to be sent
//   len - Length of data buffer
//
// Return value:
//   TRUE on success, FALSE on failure
BOOL SocketInfo::SendDataWriteFileSync(const BYTE* buf, int len)
{
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataLength = 0;

	if (!realWriteFile)
		realWriteFile = (WriteFileFunction)GetOriginalFunctionCaller("WriteFile", "kernel32.dll");

	if (originalDataBuffer == NULL)
		originalDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (len));
	
	CopyMemory(originalDataBuffer + originalDataLength, buf, len);
	originalDataLength = len;

	// Enqueue the data
	SendDataIntoQueue(buf, len);

	// Retrieve the filtered data out of the queue
	int dataLen = m_sendQueue.GetRemainingDataLength();
	BYTE* tempBuf = new BYTE[dataLen];
	m_sendQueue.RetrieveData(tempBuf, dataLen);
	AddSendingNetworkLog(tempBuf, dataLen, originalDataBuffer, originalDataLength);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	// Send the data over the network
	if (!realWriteFile((HANDLE)m_socket, tempBuf, (DWORD)dataLen, (LPDWORD)&dataLen, NULL))
	{
		// Send failed
		return FALSE;
	}

	delete[] tempBuf;
	SetLastError(ERROR_SUCCESS);
	return TRUE;
}


// SendDataWSAsync simulates the asynchronous form of the WSASend function,
// passing it through the data queue and the data modification filters
// before going to the real network.
//
// Parameters:
//   lpBuffers           - Data buffers containing data to be sent
//   dwBufferCount       - Number of data buffers
//   dwFlags             - Options for send
//   lpOverlapped        - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine - Routine to be called when the operation completes
//   dwByteTotal         - Receives number of bytes sent
//
// Return value:
//   Zero on immediate completion, SOCKET_ERROR otherwise.  WSAGetLastError
//   returns WSA_IO_PENDING on a pending asynchronous request.
int SocketInfo::SendDataWSAsync(LPWSABUF lpBuffers, DWORD dwBufferCount, DWORD dwFlags, LPWSAOVERLAPPED lpOverlapped,
	LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine, DWORD& dwByteTotal)
{
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataOffset = 0;

	if (!realWSASend)
		realWSASend = (WSASendFunction)GetOriginalFunctionCaller("WSASend", "ws2_32.dll");

	// Enqueue the data
	dwByteTotal = 0;
	for (DWORD i=0; i<dwBufferCount; i++)
	{
		if (originalDataBuffer == NULL)
			originalDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (lpBuffers[i].len));
		else
			originalDataBuffer = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, originalDataBuffer, sizeof(unsigned char) * (originalDataOffset + lpBuffers[i].len));
		
		CopyMemory(originalDataBuffer + originalDataOffset, (const BYTE*)lpBuffers[i].buf, lpBuffers[i].len);
		originalDataOffset += lpBuffers[i].len;

		SendDataIntoQueue((const BYTE*)lpBuffers[i].buf, lpBuffers[i].len);
		dwByteTotal += lpBuffers[i].len;
	}

	// Retrieve the filtered data out of the queue
	int dataLen = m_sendQueue.GetRemainingDataLength();
	BYTE* tempBuf = new BYTE[dataLen];
	m_sendQueue.RetrieveData(tempBuf, dataLen);
	
	AddSendingNetworkLog(tempBuf, dataLen, originalDataBuffer, originalDataOffset);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);


	// Send the data asynchronously.  Specify our own completion routine so that we can remove
	// the operation from the list once it has completed (the original will be called from
	// our custom routine).
	WSABUF newBuffer[1];
	newBuffer[0].buf = (char*)tempBuf;
	newBuffer[0].len = (DWORD)dataLen;

	// Add the operation to the async op list
	g_asyncList.AddWSOp(m_socket, lpOverlapped, lpCompletionRoutine, newBuffer[0].buf, dwByteTotal);

	int result = realWSASend(m_socket, newBuffer, 1, (LPDWORD)&dataLen, dwFlags, lpOverlapped,
		lpCompletionRoutine ? NetCorrupt_WSSendCompletionRoutine : NULL);

	if ((result != 0) && (WSAGetLastError() == WSAEINVAL))
	{
		// Error is either due to an associated completion port or an invalid parameter.  Try without
		// completion routine to cover the completion port case.  In the case of an invalid parameter,
		// the call will still fail and we can pass the error to the application.
		result = realWSASend(m_socket, newBuffer, 1, (LPDWORD)&dataLen, dwFlags, lpOverlapped, NULL);
	}

	return result;
}


// SendDataWriteFileAsync simulates the asynchronous form of the WriteFile function with a
// socket handle, passing it through the data queue and the data modification
// filters before going to the real network.
//
// Parameters:
//   buf                 - Data buffer containing data to be sent
//   len                 - Length of data buffer
//   lpOverlapped        - OVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine - Routine to be called when the operation completes
//
// Return value:
//   TRUE on success, FALSE on failure
BOOL SocketInfo::SendDataWriteFileAsync(const BYTE* buf, int len, LPOVERLAPPED lpOverlapped,
	LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	unsigned char *originalDataBuffer = NULL;
	unsigned long originalDataOffset = 0;

	if (!realWriteFileEx)
		realWriteFileEx = (WriteFileExFunction)GetOriginalFunctionCaller("WriteFileEx", "kernel32.dll");

	if (originalDataBuffer == NULL)
		originalDataBuffer = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (len));
	
	CopyMemory(originalDataBuffer + originalDataOffset, buf, len);
	originalDataOffset += len;

	// Enqueue the data
	SendDataIntoQueue(buf, len);

	// Retrieve the filtered data out of the queue
	int dataLen = m_sendQueue.GetRemainingDataLength();
	BYTE* tempBuf = new BYTE[dataLen];
	m_sendQueue.RetrieveData(tempBuf, dataLen);
	
	AddSendingNetworkLog(tempBuf, dataLen, originalDataBuffer, originalDataOffset);

	if (originalDataBuffer != NULL)
		HeapFree(heap, HEAP_ZERO_MEMORY, originalDataBuffer);

	// Add the operation to the async op list
	g_asyncList.AddKernelOp((HANDLE)m_socket, lpOverlapped, lpCompletionRoutine, (char*)tempBuf, len);

	// Send the data asynchronously.  Specify our own completion routine so that we can remove
	// the operation from the list once it has completed (the original will be called from
	// our custom routine).
	BOOL result = realWriteFileEx((HANDLE)m_socket, tempBuf, (DWORD)dataLen, lpOverlapped,
		lpCompletionRoutine ? NetCorrupt_KernelSendCompletionRoutine : NULL);

	if ((result == FALSE) && (GetLastError() == ERROR_INVALID_PARAMETER))
	{
		// Error is either due to an associated completion port or an invalid parameter.  Try without
		// completion routine to cover the completion port case.  In the case of an invalid parameter,
		// the call will still fail and we can pass the error to the application.
		result = realWriteFile((HANDLE)m_socket, tempBuf, (DWORD)dataLen, (LPDWORD)&dataLen, lpOverlapped);
	}

	return result;
}


// DisableSend will mark this socket so that send operations can no longer
// be completed.
//
// Parameters:
//   None
//
// Return value:
//   None
void SocketInfo::DisableSend()
{
	m_fDisableSend = true;
}


// DisableReceive will mark this socket so that receive operations can no longer
// be completed.
//
// Parameters:
//   None
//
// Return value:
//   None
void SocketInfo::DisableReceive()
{
	m_fDisableRecv = true;
}


// IsSendDisabled will check if this socket has sending disabled
//
// Parameters:
//   None
//
// Return value:
//   true if sending is disabled, false otherwise
bool SocketInfo::IsSendDisabled()
{
	return m_fDisableSend;
}


// IsReceiveDisabled will check if this socket has receiving disabled
//
// Parameters:
//   None
//
// Return value:
//   true if receiving is disabled, false otherwise
bool SocketInfo::IsReceieveDisabled()
{
	return m_fDisableRecv;
}


//*************************************************************************
// GetReceiveLengthAfterLimit determines how much data can currently be
// received, taking into account any download limits that are set
//
// Parameters:
//   None
//
// Return value:
//   Number of bytes that can be currently read from the receive queue, or
//   -1 if operation would block on a nonblocking socket
//*************************************************************************
int SocketInfo::GetReceiveLengthAfterLimit()
{
	int len = m_recvQueue.GetRemainingDataLength();

	DWORD64 downloadSpeed = 0;
	int uLen = len;

	// If datagram protocol, we don't know how big the next message is going to be, so apply
	// limit using past data only
	if (!IsStreamBased())
		uLen = 0;

	if (settingsManager->GetCachedNetworkDownloadBandwidth (downloadSpeed))
	{
		downloadSpeed /= 8; // Speed is given in bits/sec, convert to bytes/sec

		// Apply network download limits to the length
		DWORD currentTick = GetTickCount();

		if (currentTick - g_NetLimit_LastRecvStamp > (DWORD) g_NetLimit_Check_Interval_ms)
		{
			g_NetLimit_LastRecvLen -= (downloadSpeed * (DWORD64)(currentTick - g_NetLimit_LastRecvStamp)) / 1000;
			if ((__int64)g_NetLimit_LastRecvLen < 0)
				g_NetLimit_LastRecvLen = 0;
			g_NetLimit_LastRecvStamp = currentTick;
		}

		if (g_NetLimit_LastRecvLen + uLen >= (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
		{
			uLen = (int) ((downloadSpeed /(1000 / g_NetLimit_Check_Interval_ms)) - g_NetLimit_LastRecvLen);
			if (uLen <= 0)
			{
				if (IsBlocking())
				{
					Sleep (g_NetLimit_Check_Interval_ms - (currentTick - g_NetLimit_LastRecvStamp));
					g_NetLimit_LastRecvLen = 0;
					g_NetLimit_LastRecvStamp = GetTickCount();

					if (len > (int) (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
						uLen = downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms);
					else
						uLen = len;
				}
				else
					return -1;
			}
		}
	}

	// Always return an entire message for datagram protocols
	if (!IsStreamBased())
		return len;

	return uLen;
}


//*************************************************************************
// HasMoreDataThanLimit determines if more data is present in the receive
// queue than can be received with the current download limit
//
// Parameters:
//   None
//
// Return value:
//   true if more data is waiting in receive queue than can be read with
//   current download limit, false otherwise
//*************************************************************************
bool SocketInfo::HasMoreDataThanLimit()
{
	int len = m_recvQueue.GetRemainingDataLength();
	DWORD64 downloadSpeed = 0;

	if (settingsManager->GetCachedNetworkDownloadBandwidth (downloadSpeed))
	{
		downloadSpeed /= 8; // Speed is given in bits/sec, convert to bytes/sec

		DWORD currentTick = GetTickCount();

		if (currentTick - g_NetLimit_LastRecvStamp > (DWORD) g_NetLimit_Check_Interval_ms)
		{
			g_NetLimit_LastRecvLen -= (downloadSpeed * (DWORD64)(currentTick - g_NetLimit_LastRecvStamp)) / 1000;
			if ((__int64)g_NetLimit_LastRecvLen < 0)
				g_NetLimit_LastRecvLen = 0;
			g_NetLimit_LastRecvStamp = currentTick;
		}

		if (g_NetLimit_LastRecvLen + len >= (downloadSpeed / (1000 / g_NetLimit_Check_Interval_ms)))
			return true;
	}

	return false;
}


//*************************************************************************
// SetBlocking sets the blocking mode on the socket
//
// Parameters:
//   blocking - true if blocking mode is enabled
//
// Return value:
//   None
//*************************************************************************
void SocketInfo::SetBlocking(bool blocking)
{
	m_fBlocking = blocking;
}


//*************************************************************************
// IsBlocking determines if the socket is in blocking mode
//
// Parameters:
//   None
//
// Return value:
//   true if socket is in blocking mode
//*************************************************************************
bool SocketInfo::IsBlocking()
{
	return m_fBlocking;
}


//*************************************************************************
// ForceNextMessage forces the next message to be read or sent even if,
// according to the set limits, it will temporarily block on a
// nonblocking socket.
//
// Parameters:
//   None
//
// Return value:
//   true if socket is in blocking mode
//*************************************************************************
void SocketInfo::ForceNextMessage()
{
	m_fForceNext = true;
}


//*************************************************************************
// SetCompletionPort sets the completion port attached to this socket
//
// Parameters:
//   hPort - completion port, NULL for none
//   dwKey - completion key for this port
//
// Return value:
//   None
//*************************************************************************
void SocketInfo::SetCompletionPort(HANDLE hPort, ULONG_PTR dwKey)
{
	m_hCompletionPort = hPort;
	m_dwCompletionPortKey = dwKey;
}
