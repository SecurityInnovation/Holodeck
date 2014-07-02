//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		NetCorruptLibrary.cpp
//
// DESCRIPTION: Contains the hook functions for Winsock APIs as well
//              as the Modification structure, which contains the
//				description of each data modification filter.
//
//				The free regular expression library used in this
//				DLL is Copyright (C) Dr. John Maddock, 1998-1999.
//				See regex-readme.txt for full details.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Feb 2003		 R. Wagner	 File created.
// 25 Feb 2003       R. Wagner   Fixed Outlook data capturing, cleaned up
//                               interception functions
//*************************************************************************

#include "NetCorruptLibrary.h"
#include "SocketInfo.h"
#include "AsyncOp.h"
#include "DataQueue.h"
#include "SocketList.h"
#include "SiString.h"
#include "Registry.h"
#include "Event.h"
#include "defines.h"

using namespace SiUtils;


Replacement::SettingsManager * settingsManager = NULL;

AsyncOpList g_asyncList;
SocketList g_socketList;


acceptFunction realaccept = NULL;
bindFunction realbind = NULL;
closesocketFunction realclosesocket = NULL;
connectFunction realconnect = NULL;
gethostbyaddrFunction realgethostbyaddr = NULL;
gethostbynameFunction realgethostbyname = NULL;
getsockoptFunction realgetsockopt = NULL;
ioctlsocketFunction realioctlsocket = NULL;
listenFunction reallisten = NULL;
recvFunction realrecv = NULL;
recvfromFunction realrecvfrom = NULL;
selectFunction realselect = NULL;
sendFunction realsend = NULL;
sendtoFunction realsendto = NULL;
setsockoptFunction realsetsockopt = NULL;
shutdownFunction realshutdown = NULL;
socketFunction realsocket = NULL;
WSAAcceptFunction realWSAAccept = NULL;
WSAAsyncGetHostByAddrFunction realWSAAsyncGetHostByAddr = NULL;
WSAAsyncGetHostByNameFunction realWSAAsyncGetHostByName = NULL;
WSAAsyncGetServByNameFunction realWSAAsyncGetServByName = NULL;
WSAAsyncGetServByPortFunction realWSAAsyncGetServByPort = NULL;
WSAAsyncSelectFunction realWSAAsyncSelect = NULL;
WSACancelAsyncRequestFunction realWSACancelAsyncRequest = NULL;
WSAConnectFunction realWSAConnect = NULL;
WSADuplicateSocketAFunction realWSADuplicateSocketA = NULL;
WSADuplicateSocketWFunction realWSADuplicateSocketW = NULL;
WSAEventSelectFunction realWSAEventSelect = NULL;
WSAGetOverlappedResultFunction realWSAGetOverlappedResult = NULL;
WSAIoctlFunction realWSAIoctl = NULL;
WSAJoinLeafFunction realWSAJoinLeaf = NULL;
WSAProviderConfigChangeFunction realWSAProviderConfigChange = NULL;
WSARecvFunction realWSARecv = NULL;
WSARecvDisconnectFunction realWSARecvDisconnect = NULL;
WSARecvFromFunction realWSARecvFrom = NULL;
WSASendFunction realWSASend = NULL;
WSASendDisconnectFunction realWSASendDisconnect = NULL;
WSASendToFunction realWSASendTo = NULL;
WSASocketAFunction realWSASocketA = NULL;
WSASocketWFunction realWSASocketW = NULL;
ReadFileFunction realReadFile = NULL;
ReadFileExFunction realReadFileEx = NULL;
ReadFileScatterFunction realReadFileScatter = NULL;
WriteFileFunction realWriteFile = NULL;
WriteFileExFunction realWriteFileEx = NULL;
WriteFileGatherFunction realWriteFileGather = NULL;
CloseHandleFunction realCloseHandle = NULL;
CancelIoFunction realCancelIo = NULL;
GetOverlappedResultFunction realGetOverlappedResult = NULL;
CreateFileAFunction realCreateFileA = NULL;
CreateFileWFunction realCreateFileW = NULL;
GetQueuedCompletionStatusFunction realGetQueuedCompletionStatus = NULL;
CreateIoCompletionPortFunction realCreateIoCompletionPort = NULL;


//*************************************************************************
// Interception function for 'accept' in ws2_32.dll.
// The accept function permits an incoming connection attempt on a socket.
//
// Parameters:
//   s       - Socket handle to accept on.
//   addr    - Optional.  Receives address of connecting entity.
//   addrlen - Optional.  Receives length of address.
//
// Return value:
//   New socket handle, or INVALID_SOCKET on error.
//*************************************************************************
SOCKET PASCAL NetCorrupt_accept(SOCKET s, struct sockaddr* addr, int* addrlen)
{
	if (!realaccept)
		realaccept = (acceptFunction)GetOriginalFunctionCaller("accept", "ws2_32.dll");

	SOCKET retVal = realaccept(s, addr, addrlen);
	if (retVal == INVALID_SOCKET)
	{
		// accept returned error
		return retVal;
	}

	// Put the new connection's socket handle in the socket list
	g_socketList.Lock();
	SocketInfo* oldSocketInfo = g_socketList.GetSocketInfo(s);
	SocketInfo* newSocketInfo = g_socketList.AddSocket(retVal);
	if (oldSocketInfo && newSocketInfo)
	{
		newSocketInfo->SetPacketType(oldSocketInfo->GetPacketType());
		newSocketInfo->SetProtocol(oldSocketInfo->GetProtocol());
		newSocketInfo->SetRemoteIPAddress(oldSocketInfo->GetRemoteIPAddress());
		newSocketInfo->SetPort(oldSocketInfo->GetPort());
	}
	g_socketList.Unlock();

	return retVal;
}


//*************************************************************************
// Interception function for 'bind' in ws2_32.dll
// The bind function associates a local address with a socket.
//
// Parameters:
//   s       - Unbound socket handle
//   name    - Address to assign to the socket
//   namelen - Length of address
//
// Return value:
//   Zero on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_bind(SOCKET s, const struct sockaddr* name, int namelen)
{
	if (!realbind)
		realbind = (bindFunction)GetOriginalFunctionCaller("bind", "ws2_32.dll");

	int retVal = realbind(s, name, namelen);
	if (retVal != 0)
	{
		// bind returned error
		return retVal;
	}

	// Save last error before calling other Winsock functions
	int lastError = WSAGetLastError();

	// Use getsockname to find the socket address
	struct sockaddr sockName;
	int sockNameLen = sizeof(sockName);
	getsockname(s, &sockName, &sockNameLen);

	// Get the port number from the socket address
	u_short port = ntohs(((sockaddr_in*)name)->sin_port);

	// Save the port number in the socket information structure for this socket
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if (info)
	{
		info->SetRemoteIPAddress(((sockaddr_in*)name)->sin_addr.S_un.S_addr);
		info->SetPort(port);
	}
	g_socketList.Unlock();

	// Restore last error and return
	WSASetLastError(lastError);
	return retVal;
}


//*************************************************************************
// Interception function for 'closesocket' in ws2_32.dll
// The closesocket function closes an existing socket.
//
// Parameters:
//   s - Socket handle to close
//
// Return value:
//   Zero on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_closesocket(SOCKET s)
{
	if (!realclosesocket)
		realclosesocket = (closesocketFunction)GetOriginalFunctionCaller("closesocket", "ws2_32.dll");

	// Remove the socket from the socket list
	g_socketList.Lock();
	g_socketList.CloseSocket(s);
	g_socketList.Unlock();

	return realclosesocket(s);
}


//*************************************************************************
// Interception function for 'connect' in ws2_32.dll
// The connect function establishes a connection to a specified socket.
//
// Parameters:
//   s       - Unconnected socket handle
//   name    - Address to connect to
//   namelen - Length of address
//
// Return value:
//   Zero on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_connect(SOCKET s, const struct sockaddr* name, int namelen)
{
	if (!realconnect)
		realconnect = (connectFunction)GetOriginalFunctionCaller("connect", "ws2_32.dll");

	int retVal = realconnect(s, name, namelen);
	if ((retVal != 0) && (WSAGetLastError() != WSAEWOULDBLOCK))
	{
		// connect returned failure
		return retVal;
	}

	// Save last error before calling other Winsock functions
	int lastError = WSAGetLastError();

	// Get the port number from the socket address
	u_short port = ntohs(((sockaddr_in*)name)->sin_port);

	// Save the port number in the socket information structure for this socket
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if (info)
	{
		info->SetRemoteIPAddress(((sockaddr_in*)name)->sin_addr.S_un.S_addr);
		info->SetPort(port);
	}
	g_socketList.Unlock();

	// Restore last error and return
	WSASetLastError(lastError);
	return retVal;
}


//*************************************************************************
// Interception function for 'ioctlsocket' in ws2_32.dll
// The ioctlsocket function controls the I/O mode of a socket.
//
// Parameters:
//   s    - Socket handle
//   cmd  - Command to perform
//   argp - Parameter for the command
//
// Return value:
//   Zero on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_ioctlsocket(SOCKET s, long cmd, u_long* argp)
{
	unsigned long originalDataLength = -1;

	if (!realioctlsocket)
		realioctlsocket = (ioctlsocketFunction)GetOriginalFunctionCaller("ioctlsocket", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if (!info)
	{
		// Unknown socket
		g_socketList.Unlock();
		return realioctlsocket(s, cmd, argp);
	}

	info->AddRef();
	g_socketList.Unlock();

	if (cmd == FIONBIO)
		info->SetBlocking((*argp) == 0);

	// Application is trying to find out how much data is waiting
	// to be read.  Get the real value first.
	int result = realioctlsocket(s, cmd, argp);

	if (result != 0)
	{
		// ioctlsocket failed
		info->Release();
		return result;
	}

	if (cmd == FIONREAD)
	{
		if (((*argp) > 0) && (!info->HasMoreDataThanLimit()))
		{
			// There is data waiting.  Get it into our data queue now.
			BYTE* buf = new BYTE[*argp];
			info->ReceiveDataIntoQueue(buf, *argp, NULL, originalDataLength);
			delete[] buf;
		}

		// Get the length of our data queue and return this value to
		// application
		*argp = info->GetReceiveLengthAfterLimit();
		if (*argp < 0)
			*argp = 0;
	}

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'recv' in ws2_32.dll
// The recv function receives data from a connected or bound socket.
//
// Parameters:
//   s     - Socket handle to receive on
//   buf   - Data buffer that receives the data
//   len   - Length of data buffer
//   flags - Flags specifying options for the receive
//
// Return value:
//   Number of bytes received on success, or SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_recv(SOCKET s, char* buf, int len, int flags)
{
	if (!realrecv)
		realrecv = (recvFunction)GetOriginalFunctionCaller("recv", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	if ((!info) || (info->IsReceieveDisabled()))
	{
		// Unknown socket or receive disabled
		g_socketList.Unlock();
		return realrecv(s, buf, len, flags);
	}

	// Add one to the reference count of info before unlocking the
	// socket list to prevent info from getting deleted by another
	// thread closing the socket.
	info->AddRef();
	g_socketList.Unlock();

	// Receive or peek at the data in the input queue, according to the flags
	int retVal;
	if (flags & MSG_PEEK)
		retVal = info->PeekData((BYTE*)buf, len);
	else
		retVal = info->ReceiveData((BYTE*)buf, len);
	info->Release();
	return retVal;
}


//*************************************************************************
// Interception function for 'recvfrom' in ws2_32.dll
// The recvfrom function receives a datagram and stores the source address.
// (Normally used for connectionless, non-stream sockets)
//
// Parameters:
//   s       - Socket handle to receive on
//   buf     - Data buffer that receives the data
//   len     - Length of data buffer
//   flags   - Flags specifying options for the receive
//   from    - Receives address of where the data came from
//   fromlen - Receives length of address
//
// Return value:
//   Number of bytes received on success, or SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
{
	if (!realrecvfrom)
		realrecvfrom = (recvfromFunction)GetOriginalFunctionCaller("recvfrom", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	// For stream-based sockets, recvfrom is the same as recv
	if ((info) && (info->IsStreamBased()))
	{
		g_socketList.Unlock();
		return NetCorrupt_recv(s, buf, len, flags);
	}

	if ((!info) || (info->IsReceieveDisabled()))
	{
		// Unknown socket or receive disabled
		g_socketList.Unlock();
		return realrecvfrom(s, buf, len, flags, from, fromlen);
	}

	// Add one to the reference count of info before unlocking the
	// socket list to prevent info from getting deleted by another
	// thread closing the socket.
	info->AddRef();
	g_socketList.Unlock();

	// Receive or peek at the data in the input queue, according to the flags
	int retVal;
	if (flags & MSG_PEEK)
		retVal = info->PeekDataFrom((BYTE*)buf, len, from, fromlen);
	else
		retVal = info->ReceiveDataFrom((BYTE*)buf, len, from, fromlen);
	info->Release();
	return retVal;
}


//*************************************************************************
// Interception function for 'select' in ws2_32.dll
// The select function determines the status of one or more sockets,
// waiting if necessary, to perform synchronous I/O.
//
// Parameters:
//   nfds      - Ignored
//   readfds   - Set of sockets to be checked for readability
//   writefds  - Set of sockets to be checked for writability
//   exceptfds - Set of sockets to be checked for errors
//   timeout   - Maximum time to wait
//
// Return value:
//   Number of socket handles that are ready, zero if time limit expires,
//   or SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout)
{
	if (!realselect)
		realselect = (selectFunction)GetOriginalFunctionCaller("select", "ws2_32.dll");

	// BUGBUG: This needs to also include sockets that have data in
	// our queue but not the "real" queue.
	int count = realselect(nfds, readfds, writefds, exceptfds, timeout);
	if (count == SOCKET_ERROR)
		return count;

	if (readfds)
	{
		g_socketList.Lock();
		// Ensure nonblocking sockets that are marked as readable aren't restricted by limits
		for (DWORD i=0; i<readfds->fd_count; i++)
		{
			SocketInfo* info = g_socketList.GetSocketInfo(readfds->fd_array[i]);
			if (info && (!info->IsBlocking()))
			{
				if (info->GetReceiveLengthAfterLimit() == -1)
				{
					// Nonblocking socket with data held back due to limits, remove it from the list
					for (DWORD j=i+1; j<readfds->fd_count; j++)
						readfds->fd_array[j - 1] = readfds->fd_array[j];
					readfds->fd_count--;
					count--;
					i--; // So that i is same value in next iteration
				}
				else
					info->ForceNextMessage();
			}
		}
		g_socketList.Unlock();
	}

	return count;
}


//*************************************************************************
// Interception function for 'send' in ws2_32.dll
// The send function sends data on a connected socket.
//
// Parameters:
//   s     - Socket handle to send on
//   buf   - Data buffer containing data to be sent
//   len   - Length of data buffer
//   flags - Flags specifying options for the send
//
// Return value:
//   Total number of bytes sent on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_send(SOCKET s, const char* buf, int len, int flags)
{
	if (!realsend)
		realsend = (sendFunction)GetOriginalFunctionCaller("send", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	if ((!info) || (info->IsSendDisabled()))
	{
		// Unknown socket or send disabled
		g_socketList.Unlock();
		return realsend(s, buf, len, flags);
	}

	// Add one to the reference count of info before unlocking the
	// socket list to prevent info from getting deleted by another
	// thread closing the socket.
	info->AddRef();
	g_socketList.Unlock();

	// Send the data
	int retVal = info->SendData((const BYTE*)buf, len);
	info->Release();
	return retVal;
}


//*************************************************************************
// Interception function for 'sendto' in ws2_32.dll
// The sendto function sends data to a specific destination.
// (Behaves like send for stream-based sockets)
//
// Parameters:
//   s     - Socket handle to send on
//   buf   - Data buffer containing data to be sent
//   len   - Length of data buffer
//   flags - Flags specifying options for the send
//   to    - Address to send the data to
//   tolen - Length of address
//
// Return value:
//   Total number of bytes sent on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
{
	if (!realsendto)
		realsendto = (sendtoFunction)GetOriginalFunctionCaller("sendto", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	// For stream-based sockets, sendto is the same as send
	if ((info) && (info->IsStreamBased()))
	{
		g_socketList.Unlock();
		return NetCorrupt_send(s, buf, len, flags);
	}

	if ((!info) || (info->IsSendDisabled()))
	{
		// Unknown socket or send disabled
		g_socketList.Unlock();
		return realsendto(s, buf, len, flags, to, tolen);
	}

	// Add one to the reference count of info before unlocking the
	// socket list to prevent info from getting deleted by another
	// thread closing the socket.
	info->AddRef();
	g_socketList.Unlock();

	// Send the data
	int retVal = info->SendDataWithAddr((const BYTE*)buf, len, to, &tolen);
	info->Release();
	return retVal;
}


//*************************************************************************
// Interception function for 'shutdown' in ws2_32.dll
// The shutdown function disables sends or receives on a socket.
//
// Parameters:
//   s   - Socket handle to shutdown
//   how - Flag that describes what types of operation will no longer be allowed
//
// Return value:
//   Zero on success, SOCKET_ERROR on error.
//*************************************************************************
int PASCAL NetCorrupt_shutdown(SOCKET s, int how)
{
	if (!realshutdown)
		realshutdown = (shutdownFunction)GetOriginalFunctionCaller("shutdown", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	if (!info)
	{
		// Unknown socket handle
		g_socketList.Unlock();
		return realshutdown(s, how);
	}

	// Unlock the socket list in case shutdown blocks
	info->AddRef();
	g_socketList.Unlock();

	// Execute the shutdown and check the result
	int result = shutdown(s, how);
	if (result != 0)
	{
		// Shutdown failed
		info->Release();
		return result;
	}

	// Shutdown succeeded, mark the disable send/receive flags as required
	if (how == SD_RECEIVE)
		info->DisableReceive();
	else if (how == SD_SEND)
		info->DisableSend();
	else if (how == SD_BOTH)
	{
		info->DisableReceive();
		info->DisableSend();
	}

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'socket' in ws2_32.dll
// The socket function creates a socket that is bound to a specific service provider.
//
// Parameters:
//   af       - Address family
//   type     - Type of socket to be created
//   protocol - Protocol to be used
//
// Return value:
//   New socket on success, INVALID_SOCKET on error.
//*************************************************************************
SOCKET PASCAL NetCorrupt_socket(int af, int type, int protocol)
{
	if (!realsocket)
		realsocket = (socketFunction)GetOriginalFunctionCaller("socket", "ws2_32.dll");

	SOCKET ret = realsocket(af, type, protocol);

	if (ret == INVALID_SOCKET)
		return ret;

	g_socketList.Lock();
	SocketInfo *info = g_socketList.AddSocket(ret);
	info->SetPacketType(type);
	info->SetProtocol(protocol);
	g_socketList.Unlock();
	return ret;
}


//*************************************************************************
// Interception function for 'WSAAccept' in ws2_32.dll
// The WSAAccept function conditionally accepts a connection based on the
// return value of a condition function, provides quality of service flow
// specifications, and allows the transfer of connection data.
//
// Parameters:
//   s              - Socket handle to accept on
//   addr           - Receives address of connecting entity
//   addrlen        - Receives length of address
//   lpfnCondition  - Optional condition function that makes an accept/reject decision
//   dwCallbackData - Data passed as a parameter to the condition function
//
// Return value:
//   New socket handle on success, INVALID_SOCKET on failure.
//*************************************************************************
SOCKET PASCAL NetCorrupt_WSAAccept(SOCKET s, struct sockaddr* addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition,
							   DWORD dwCallbackData)
{
	if (!realWSAAccept)
		realWSAAccept = (WSAAcceptFunction)GetOriginalFunctionCaller("WSAAccept", "ws2_32.dll");

	SOCKET retVal = realWSAAccept(s, addr, addrlen, lpfnCondition, dwCallbackData);
	if (retVal == INVALID_SOCKET)
	{
		// WSAAccept returned failure
		return retVal;
	}

	// Put the new connection's socket handle in the socket list
	g_socketList.Lock();
	SocketInfo* oldSocketInfo = g_socketList.GetSocketInfo(s);
	SocketInfo* newSocketInfo = g_socketList.AddSocket(retVal);
	if (oldSocketInfo && newSocketInfo)
	{
		newSocketInfo->SetPacketType(oldSocketInfo->GetPacketType());
		newSocketInfo->SetProtocol(oldSocketInfo->GetProtocol());
		newSocketInfo->SetRemoteIPAddress(oldSocketInfo->GetRemoteIPAddress());
		newSocketInfo->SetPort(oldSocketInfo->GetPort());
	}
	g_socketList.Unlock();
	return retVal;
}


//*************************************************************************
// Interception function for 'WSAConnect' in ws2_32.dll
// The WSAConnect function establishes a connection to another socket
// application, exchanges connect data, and specifies needed quality
// of service based on the specified FLOWSPEC structure.
//
// Parameters:
//   s            - Unconnected socket handle
//   name         - Address to connect to
//   namelen      - Length of address
//   lpCallerData - Data to be sent during connection establishment
//   lpCalleeData - Data to be received during connection establishment
//   lpSQOS       - Pointer to the FLOWSPEC structures for socket s, one for each direction.
//   lpGQOS       - Reserved
//
// Return value:
//   Zero on success, SOCKET_ERROR on failure.
//*************************************************************************
int PASCAL NetCorrupt_WSAConnect(SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData,
							 LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS)
{
	if (!realWSAConnect)
		realWSAConnect = (WSAConnectFunction)GetOriginalFunctionCaller("WSAConnect", "ws2_32.dll");

	int retVal = realWSAConnect(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS);
	if ((retVal != 0) && (WSAGetLastError() != WSAEWOULDBLOCK))
	{
		// WSAConnect returned error
		return retVal;
	}

	// Save last error before calling other Winsock functions
	int lastError = WSAGetLastError();

	// Get the port number from the socket address
	u_short port = ntohs(((sockaddr_in*)name)->sin_port);

	// Save the port number in the socket information structure for this socket
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if (info)
	{
		info->SetRemoteIPAddress(((sockaddr_in*)name)->sin_addr.S_un.S_addr);
		info->SetPort(port);
	}
	g_socketList.Unlock();

	// Restore last error and return
	WSASetLastError(lastError);
	return retVal;
}


//*************************************************************************
// Interception function for 'WSAGetOverlappedResult' in ws2_32.dll
// The WSAGetOverlappedResult function retrieves the results of an overlapped
// operation on the specified socket.
//
// Parameters:
//   s            - Socket handle
//   lpOverlapped - OVERLAPPED structure used to start the operation
//   lpcbTransfer - Receives number of bytes transferred
//   fWait        - If TRUE, does not return until the operation is completed
//   lpdwFlags    - Receives flags that supplement the completion status
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL PASCAL NetCorrupt_WSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait,
										  LPDWORD lpdwFlags)
{
	if (!realWSAGetOverlappedResult)
		realWSAGetOverlappedResult = (WSAGetOverlappedResultFunction)GetOriginalFunctionCaller("WSAGetOverlappedResult", "ws2_32.dll");

	// Find the operation in our asynchronous operation list
	g_asyncList.Lock();
	AsyncOpInfo* op = g_asyncList.GetKernelOpInfo(lpOverlapped);

	if (!op)
	{
		// Not an operation we care about
		g_asyncList.Unlock();
		return realWSAGetOverlappedResult(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);
	}

	// Unlock the list in case we end up waiting for completion
	op->AddRef();
	g_asyncList.Unlock();

	// Call the original function to get the status
	BOOL result = realWSAGetOverlappedResult(s, lpOverlapped, lpcbTransfer, fWait, lpdwFlags);

	if (!result)
	{
		// Not completed
		op->Release();
		return result;
	}

	// Completed, report the number of bytes transferred that the application will expect
	if (lpcbTransfer)
		*lpcbTransfer = op->GetTransferLength();

	op->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WSAIoctl' in ws2_32.dll
// The WSAIoctl function controls the mode of a socket.
//
// Parameters:
//   s                   - Socket handle
//   dwIoControlCode     - Control code of operation to perform.
//   lpvInBuffer         - Pointer to the input buffer.
//   cbInBuffer          - Size of the input buffer.
//   lpvOutBuffer        - Pointer to the output buffer.
//   cbOutBuffer         - Size of the output buffer.
//   lpcbBytesReturned   - Receives number of bytes placed into the output buffer.
//   lpOverlapped        - OVERLAPPED structure for asynchronous operation.
//   lpCompletionRoutine - Routine to be called when operation is completed.
//
// Return value:
//   Zero on success, SOCKET_ERROR on failure.
//*************************************************************************
int PASCAL NetCorrupt_WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer,
						   DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped,
						   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	unsigned long originalDataLength = -1;

	if (!realWSAIoctl)
		realWSAIoctl = (WSAIoctlFunction)GetOriginalFunctionCaller("WSAIoctl", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if (!info)
	{
		// Unknown socket
		g_socketList.Unlock();
		return realWSAIoctl(s, dwIoControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, lpcbBytesReturned,
			lpOverlapped, lpCompletionRoutine);
	}

	info->AddRef();
	g_socketList.Unlock();

	if (dwIoControlCode == FIONBIO)
		info->SetBlocking(*((DWORD*)lpvInBuffer) == 0);

	// Application is trying to find out how much data is waiting
	// to be read.  Get the real value first.
	int result = realWSAIoctl(s, dwIoControlCode, lpvInBuffer, cbInBuffer, lpvOutBuffer, cbOutBuffer, lpcbBytesReturned,
		lpOverlapped, lpCompletionRoutine);

	if (result != 0)
	{
		// ioctlsocket failed
		info->Release();
		return result;
	}

	if (dwIoControlCode == FIONREAD)
	{
		if (((*((DWORD*)lpvOutBuffer)) > 0) && (!info->HasMoreDataThanLimit()))
		{
			// There is data waiting.  Get it into our data queue now.
			BYTE* buf = new BYTE[*((DWORD*)lpvOutBuffer)];
			info->ReceiveDataIntoQueue(buf, *((DWORD*)lpvOutBuffer), NULL, originalDataLength);
			delete[] buf;
		}

		// Get the length of our data queue and return this value to
		// application
		*((DWORD*)lpvOutBuffer) = info->GetReceiveLengthAfterLimit();
		if (*((DWORD*)lpvOutBuffer) == -1)
			*((DWORD*)lpvOutBuffer) = 0;
	}

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WSAJoinLeaf' in ws2_32.dll
// The WSAJoinLeaf function joins a leaf node into a multipoint session,
// exchanges connect data, and specifies needed quality of service based
// on the specified FLOWSPEC structures.
//
// Parameters:
//   s            - Socket handle
//   name         - Name of the peer to which the socket is to be joined.
//   namelen      - Length of name
//   lpCallerData - User data to be sent during connection establishment
//   lpCalleeData - User data to be received during connection establishment
//   lpSQOS       - Pointer to the FLOWSPEC structures for socket s, one for each direction.
//   lpGQOS       - Reserved
//   dwFlags      - Flags to indicate that the socket is acting as a sender, receiver, or both.
//
// Return value:
//   New socket handle on success, INVALID_SOCKET on error.
//*************************************************************************
SOCKET PASCAL NetCorrupt_WSAJoinLeaf(SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData,
								 LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS, DWORD dwFlags)
{
	if (!realWSAJoinLeaf)
		realWSAJoinLeaf = (WSAJoinLeafFunction)GetOriginalFunctionCaller("WSAJoinLeaf", "ws2_32.dll");

	SOCKET retVal = realWSAJoinLeaf(s, name, namelen, lpCallerData, lpCalleeData, lpSQOS, lpGQOS, dwFlags);
	if (retVal == INVALID_SOCKET)
	{
		// WSAAccept returned failure
		return retVal;
	}

	// Put the new connection's socket handle in the socket list
	g_socketList.Lock();
	SocketInfo* oldSocketInfo = g_socketList.GetSocketInfo(s);
	SocketInfo* newSocketInfo = g_socketList.AddSocket(retVal);
	if (oldSocketInfo && newSocketInfo)
	{
		newSocketInfo->SetPacketType(oldSocketInfo->GetPacketType());
		newSocketInfo->SetProtocol(oldSocketInfo->GetProtocol());
		newSocketInfo->SetRemoteIPAddress(oldSocketInfo->GetRemoteIPAddress());
		newSocketInfo->SetPort(oldSocketInfo->GetPort());
	}
	g_socketList.Unlock();
	return retVal;
}


//*************************************************************************
// Interception function for 'WSARecv' in ws2_32.dll
// The WSARecv function receives data from a connected socket.
//
// Parameters:
//   s                    - Socket handle to receive on
//   lpBuffers            - Buffers to use for receiving
//   dwBufferCount        - Number of buffers to use
//   lpNumberOfBytesRecvd - Receives number of bytes received
//   lpFlags              - Pointer to flags
//   lpOverlapped         - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine  - Routine to be called when operation is complete
//
// Return value:
//   Zero on immediate completion, SOCKET_ERROR otherwise.  WSAGetLastError
//   returns WSA_IO_PENDING on a pending asynchronous request.
//*************************************************************************
int PASCAL NetCorrupt_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
						  LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!realWSARecv)
		realWSARecv = (WSARecvFunction)GetOriginalFunctionCaller("WSARecv", "ws2_32.dll");

	g_socketList.Lock();
	// Find the socket info
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if ((!info) || (info->IsReceieveDisabled()))
	{
		// Unknown socket or receive disabled
		g_socketList.Unlock();
		return realWSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpOverlapped, lpCompletionRoutine);
	}

	// Determine if the socket is capable of asynchronous operations
	bool fOverlapped = info->IsOverlapped();
	// Keep the SocketInfo structure from being deleted and unlock the socket list
	info->AddRef();
	g_socketList.Unlock();

	if (((!lpOverlapped) && (!lpCompletionRoutine)) || (!fOverlapped))
	{
		// Operation is synchronous.  Just use the normal recv functionality
		DWORD dwByteTotal = 0;

		if (dwBufferCount > 0)
		{
			if (lpFlags && ((*lpFlags) & MSG_PEEK))
				dwByteTotal = info->PeekData((BYTE*)lpBuffers[0].buf, lpBuffers[0].len);
			else
				dwByteTotal = info->ReceiveData((BYTE*)lpBuffers[0].buf, lpBuffers[0].len);
		}
		if (dwByteTotal == -1)
			return SOCKET_ERROR;

		if (lpNumberOfBytesRecvd)
			*lpNumberOfBytesRecvd = dwByteTotal;
		if (lpFlags)
			*lpFlags = 0;

		info->Release();
		return 0;
	}

	// Receive the data asynchronously, passing it through the data modification filters first
	DWORD dwByteTotal;
	int result = info->ReceiveDataWSAsync(lpBuffers, dwBufferCount, lpFlags, lpOverlapped, lpCompletionRoutine, dwByteTotal);

	if ((result == 0) && (lpNumberOfBytesRecvd))
		*lpNumberOfBytesRecvd = dwByteTotal;

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WSARecvFrom' in ws2_32.dll
// The WSARecvFrom function receives a datagram and stores the source address.
//
// Parameters:
//   s                    - Socket handle to receive on
//   lpBuffers            - Buffers to use for receiving
//   dwBufferCount        - Number of buffers to use
//   lpNumberOfBytesRecvd - Receives number of bytes received
//   lpFlags              - Pointer to flags
//   lpFrom               - Receives address of sending entity
//   lpFromlen            - Receives length of address
//   lpOverlapped         - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine  - Routine to be called when operation is complete
//
// Return value:
//   Zero on immediate completion, SOCKET_ERROR otherwise.  WSAGetLastError
//   returns WSA_IO_PENDING on a pending asynchronous request.
//*************************************************************************
int PASCAL NetCorrupt_WSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd,
							  LPDWORD lpFlags, struct sockaddr* lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped,
							  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!realWSARecvFrom)
		realWSARecvFrom = (WSARecvFromFunction)GetOriginalFunctionCaller("WSARecvFrom", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	// For stream-based sockets, WSARecvFrom is the same as WSARecv
	if ((info) && (info->IsStreamBased()))
	{
		g_socketList.Unlock();
		return NetCorrupt_WSARecv(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd,
			lpFlags, lpOverlapped, lpCompletionRoutine);
	}

	if ((!info) || (info->IsReceieveDisabled()))
	{
		// Unknown socket or receive disabled
		g_socketList.Unlock();
		return realWSARecvFrom(s, lpBuffers, dwBufferCount, lpNumberOfBytesRecvd, lpFlags, lpFrom, lpFromlen, lpOverlapped,
			lpCompletionRoutine);
	}

	// Determine if the socket is capable of asynchronous operations
	bool fOverlapped = info->IsOverlapped();
	// Keep the SocketInfo structure from being deleted and unlock the socket list
	info->AddRef();
	g_socketList.Unlock();

	if (((!lpOverlapped) && (!lpCompletionRoutine)) || (!fOverlapped))
	{
		// Operation is synchronous.  Just use the normal recv functionality
		DWORD dwByteTotal = 0;

		if (dwBufferCount > 0)
		{
			if (lpFlags && ((*lpFlags) & MSG_PEEK))
				dwByteTotal = info->PeekDataFrom((BYTE*)lpBuffers[0].buf, lpBuffers[0].len, lpFrom, lpFromlen);
			else
				dwByteTotal = info->ReceiveDataFrom((BYTE*)lpBuffers[0].buf, lpBuffers[0].len, lpFrom, lpFromlen);
		}
		if (dwByteTotal == -1)
			return SOCKET_ERROR;

		if (lpNumberOfBytesRecvd)
			*lpNumberOfBytesRecvd = dwByteTotal;
		if (lpFlags)
			*lpFlags = 0;

		info->Release();
		return 0;
	}

	// Receive the data asynchronously, passing it through the data modification filters first
	DWORD dwByteTotal;
	int result = info->ReceiveDataWSAsync(lpBuffers, dwBufferCount, lpFlags, lpOverlapped, lpCompletionRoutine, dwByteTotal);

	if ((result == 0) && (lpNumberOfBytesRecvd))
		*lpNumberOfBytesRecvd = dwByteTotal;

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WSASend' in ws2_32.dll
// The WSASend function sends data on a connected socket.
//
// Parameters:
//   s                   - Socket handle to send on
//   lpBuffers           - Buffers to use for sending
//   dwBufferCount       - Number of buffers to use
//   lpNumberOfBytesSent - Receives number of bytes sent
//   dwFlags             - Options for send
//   lpOverlapped        - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine - Routine to be called when operation is complete
//
// Return value:
//   Zero on immediate completion, SOCKET_ERROR otherwise.  WSAGetLastError
//   returns WSA_IO_PENDING on a pending asynchronous request.
//*************************************************************************
int PASCAL NetCorrupt_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
						  LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!realWSASend)
		realWSASend = (WSASendFunction)GetOriginalFunctionCaller("WSASend", "ws2_32.dll");

	g_socketList.Lock();
	// Find the socket info
	SocketInfo* info = g_socketList.GetSocketInfo(s);
	if ((!info) || (info->IsSendDisabled()))
	{
		// Unknown socket or send disabled
		g_socketList.Unlock();
		return realWSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpOverlapped, lpCompletionRoutine);
	}
	// Determine if the socket is capable of asynchronous operations
	bool fOverlapped = info->IsOverlapped();
	// Keep the SocketInfo structure from being deleted and unlock the socket list
	info->AddRef();
	g_socketList.Unlock();

	if (((!lpOverlapped) && (!lpCompletionRoutine)) || (!fOverlapped))
	{
		// Operation is synchronous.  Just use the normal send functionality
		DWORD dwByteTotal = 0;
		for (DWORD i=0; i<dwBufferCount; i++)
			dwByteTotal += info->SendData((BYTE*)lpBuffers[i].buf, lpBuffers[i].len);
		if (lpNumberOfBytesSent)
			*lpNumberOfBytesSent = dwByteTotal;
		info->Release();
		return 0;
	}

	// Send the data asynchronously, passing it through the data modification filters first
	DWORD dwByteTotal;
	int result = info->SendDataWSAsync(lpBuffers, dwBufferCount, dwFlags, lpOverlapped, lpCompletionRoutine, dwByteTotal);

	if ((result == 0) && (lpNumberOfBytesSent))
		*lpNumberOfBytesSent = dwByteTotal;

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WSASendTo' in ws2_32.dll
// The WSASendTo function sends data to a specific destination, using overlapped
// I/O where applicable.
//
// Parameters:
//   s                   - Socket handle to send on
//   lpBuffers           - Buffers to use for sending
//   dwBufferCount       - Number of buffers to use
//   lpNumberOfBytesSent - Receives number of bytes sent
//   dwFlags             - Options for send
//   lpTo                - Address to send to
//   iToLen              - Length of address
//   lpOverlapped        - WSAOVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine - Routine to be called when operation is complete
//
// Return value:
//   Zero on immediate completion, SOCKET_ERROR otherwise.  WSAGetLastError
//   returns WSA_IO_PENDING on a pending asynchronous request.
//*************************************************************************
int PASCAL NetCorrupt_WSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
							const struct sockaddr* lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped,
							LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!realWSASendTo)
		realWSASendTo = (WSASendToFunction)GetOriginalFunctionCaller("WSASendTo", "ws2_32.dll");

	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo(s);

	// For stream-based sockets, WSASendTo is the same as WSASend
	if ((info) && (info->IsStreamBased()))
	{
		g_socketList.Unlock();
		return NetCorrupt_WSASend(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags,
			lpOverlapped, lpCompletionRoutine);
	}

	if ((!info) || (info->IsSendDisabled()))
	{
		// Unknown socket or send disabled
		g_socketList.Unlock();
		return realWSASendTo(s, lpBuffers, dwBufferCount, lpNumberOfBytesSent, dwFlags, lpTo, iToLen, lpOverlapped,
			lpCompletionRoutine);
	}
	// Determine if the socket is capable of asynchronous operations
	bool fOverlapped = info->IsOverlapped();
	// Keep the SocketInfo structure from being deleted and unlock the socket list
	info->AddRef();
	g_socketList.Unlock();

	if (((!lpOverlapped) && (!lpCompletionRoutine)) || (!fOverlapped))
	{
		// Operation is synchronous.  Just use the normal send functionality
		DWORD dwByteTotal = 0;
		for (DWORD i=0; i<dwBufferCount; i++)
			dwByteTotal += info->SendDataWithAddr((BYTE*)lpBuffers[i].buf, lpBuffers[i].len, lpTo, &iToLen);
		if (lpNumberOfBytesSent)
			*lpNumberOfBytesSent = dwByteTotal;
		info->Release();
		return 0;
	}

	// Send the data asynchronously, passing it through the data modification filters first
	DWORD dwByteTotal;
	int result = info->SendDataWSAsync(lpBuffers, dwBufferCount, dwFlags, lpOverlapped, lpCompletionRoutine, dwByteTotal);

	if ((result == 0) && (lpNumberOfBytesSent))
		*lpNumberOfBytesSent = dwByteTotal;

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for ASCII version of 'WSASocket' in ws2_32.dll
// The WSASocket function creates a socket that is bound to a specific
// transport-service provider.
//
// Parameters:
//   af             - Address family
//   type           - Type of new socket
//   protocol       - Protocol to be used
//   lpProtocolInfo - Defines characteristics of socket being created
//   g              - Reserved
//   dwFlags        - Flag that specifies the socket attribute.
//
// Return value:
//   New socket handle on success, INVALID_SOCKET on error.
//*************************************************************************
SOCKET PASCAL NetCorrupt_WSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	if (!realWSASocketA)
		realWSASocketA = (WSASocketAFunction)GetOriginalFunctionCaller("WSASocketA", "ws2_32.dll");

	SOCKET retVal = realWSASocketA(af, type, protocol, lpProtocolInfo, g, dwFlags);
	if (retVal == INVALID_SOCKET)
	{
		// WSASocket returned failure
		return retVal;
	}

	// Add the new socket to the socket list
	g_socketList.Lock();
	SocketInfo* info = g_socketList.AddSocket(retVal);
	info->SetPacketType(type);
	info->SetProtocol(protocol);
	info->SetOverlapped((dwFlags & WSA_FLAG_OVERLAPPED) != 0);
	g_socketList.Unlock();
	return retVal;
}


//*************************************************************************
// Interception function for Unicode version of 'WSASocket' in ws2_32.dll
// The WSASocket function creates a socket that is bound to a specific
// transport-service provider.
//
// Parameters:
//   af             - Address family
//   type           - Type of new socket
//   protocol       - Protocol to be used
//   lpProtocolInfo - Defines characteristics of socket being created
//   g              - Reserved
//   dwFlags        - Flag that specifies the socket attribute.
//
// Return value:
//   New socket handle on success, INVALID_SOCKET on error.
//*************************************************************************
SOCKET PASCAL NetCorrupt_WSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	if (!realWSASocketW)
		realWSASocketW = (WSASocketWFunction)GetOriginalFunctionCaller("WSASocketW", "ws2_32.dll");

	SOCKET retVal = realWSASocketW(af, type, protocol, lpProtocolInfo, g, dwFlags);
	if (retVal == INVALID_SOCKET)
	{
		// WSASocket returned failure
		return retVal;
	}

	// Add the new socket to the socket list
	g_socketList.Lock();
	SocketInfo* info = g_socketList.AddSocket(retVal);
	info->SetPacketType(type);
	info->SetProtocol(protocol);
	info->SetOverlapped((dwFlags & WSA_FLAG_OVERLAPPED) != 0);
	g_socketList.Unlock();
	return retVal;
}


//*************************************************************************
// Interception function for 'ReadFile' in kernel32.dll
// The ReadFile function reads data from a file.  This function is designed
// for both synchronous and asynchronous operation.
//
// Parameters:
//   hFile                - File or socket handle to read from
//   lpBuffer             - Data buffer that receives the data being read
//   nNumberOfBytesToRead - Length of data buffer
//   lpNumberOfBytesRead  - Receives number of bytes read
//   lpOverlapped         - OVERLAPPED structure for asynchronous operation
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
							LPOVERLAPPED lpOverlapped)
{
	if (!realReadFile)
		realReadFile = (ReadFileFunction)GetOriginalFunctionCaller("ReadFile", "kernel32.dll");

	// Is this a socket handle?
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo((SOCKET)hFile);

	if ((!info) || (info->IsReceieveDisabled()))
	{
		// Not a socket or receive disabled
		g_socketList.Unlock();
		return realReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	}

	// Determine if the socket is capable of asynchronous operations
	bool fOverlapped = info->IsOverlapped();

	info->AddRef();
	g_socketList.Unlock();

	BOOL result;

	DWORD dwByteTotal;
	if ((!lpOverlapped) || (!fOverlapped))
	{
		// Synchronous operation
		result = info->ReceiveDataReadFileSync((BYTE*)lpBuffer, nNumberOfBytesToRead, dwByteTotal);
	}
	else
	{
		// Asynchronous operation
		result = info->ReceiveDataReadFileAsync((BYTE*)lpBuffer, nNumberOfBytesToRead, lpOverlapped, NULL, dwByteTotal);
	}

	if (result && lpNumberOfBytesRead)
		*lpNumberOfBytesRead = dwByteTotal;

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'ReadFileEx' in kernel32.dll
// The ReadFileEx function reads data from a file asynchronously.
//
// Parameters:
//   hFile                - File or socket handle to read from
//   lpBuffer             - Data buffer that receives the data being read
//   nNumberOfBytesToRead - Length of data buffer
//   lpOverlapped         - OVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine  - Routine to be called when the operation completes
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_ReadFileEx(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped,
							  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!realReadFileEx)
		realReadFileEx = (ReadFileExFunction)GetOriginalFunctionCaller("ReadFileEx", "kernel32.dll");

	// Is this a socket handle?
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo((SOCKET)hFile);

	if ((!info) || (info->IsReceieveDisabled()) ||
		((!lpOverlapped) || (!info->IsOverlapped())))
	{
		// Not a socket, receive disabled, or not asynchronous
		g_socketList.Unlock();
		return realReadFileEx(hFile, lpBuffer, nNumberOfBytesToRead, lpOverlapped, lpCompletionRoutine);
	}

	info->AddRef();
	g_socketList.Unlock();

	// Asynchronous operation
	DWORD dwBytesRead;
	BOOL result = info->ReceiveDataReadFileAsync((BYTE*)lpBuffer, nNumberOfBytesToRead, lpOverlapped,
		lpCompletionRoutine, dwBytesRead);

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'ReadFileScatter' in kernel32.dll
// The ReadFileScatter function reads data from a file and stores it in an array of buffers.
//
// Parameters:
//   hFile                - File or socket handle to read from
//   aSegmentArray        - Buffers to receive data into
//   nNumberOfBytesToRead - Number of bytes to read
//   lpReserved           - Reserved
//   lpOverlapped         - OVERLAPPED structure for asynchronous operation
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_ReadFileScatter(HANDLE hFile, FILE_SEGMENT_ELEMENT aSegmentArray[], DWORD nNumberOfBytesToRead,
								   LPDWORD lpReserved, LPOVERLAPPED lpOverlapped)
{
	if (!realReadFileScatter)
		realReadFileScatter = (ReadFileScatterFunction)GetOriginalFunctionCaller("ReadFileScatter", "kernel32.dll");
	// BUGBUG: Want to inject faults into data received with a scattered read
	return realReadFileScatter(hFile, aSegmentArray, nNumberOfBytesToRead, lpReserved, lpOverlapped);
}


//*************************************************************************
// Interception function for 'WriteFile' in kernel32.dll
// The WriteFile function writes data to a file and is designed for both
// synchronous and asynchronous operation.
//
// Parameters:
//   hFile                  - File or socket handle
//   lpBuffer               - Data buffer containing data to write
//   nNumberOfBytesToWrite  - Length of data buffer
//   lpNumberOfBytesWritten - Receives number of bytes written
//   lpOverlapped           - OVERLAPPED structure for asynchronous operation
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,
							 LPOVERLAPPED lpOverlapped)
{
	if (!realWriteFile)
		realWriteFile = (WriteFileFunction)GetOriginalFunctionCaller("WriteFile", "kernel32.dll");

	// Is this a socket handle?
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo((SOCKET)hFile);

	if ((!info) || (info->IsSendDisabled()))
	{
		// Not a socket, or send disabled
		g_socketList.Unlock();
		return realWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	}

	// Determine if the socket is capable of asynchronous operations
	bool fOverlapped = info->IsOverlapped();

	info->AddRef();
	g_socketList.Unlock();

	BOOL result;

	if ((!lpOverlapped) || (!fOverlapped))
	{
		// Synchronous operation
		result = info->SendDataWriteFileSync((const BYTE*)lpBuffer, nNumberOfBytesToWrite);
	}
	else
	{
		// Asynchronous operation
		result = info->SendDataWriteFileAsync((const BYTE*)lpBuffer, nNumberOfBytesToWrite, lpOverlapped, NULL);
	}

	if (result && lpNumberOfBytesWritten)
	{
		// Actual number of bytes sent will be the number after modification.  Return the original number
		// of bytes so that the application doesn't get confused.
		*lpNumberOfBytesWritten = nNumberOfBytesToWrite;
	}

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WriteFileEx' in kernel32.dll
// The WriteFileEx function writes data to a file. It is designed solely for
// asynchronous operation.
//
// Parameters:
//   hFile                 - File or socket handle
//   lpBuffer              - Data buffer containing data to write
//   nNumberOfBytesToWrite - Length of data buffer
//   lpOverlapped          - OVERLAPPED structure for asynchronous operation
//   lpCompletionRoutine   - Routine to be called when the operation completes
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_WriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped,
							   LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	if (!realWriteFileEx)
		realWriteFileEx = (WriteFileExFunction)GetOriginalFunctionCaller("WriteFileEx", "kernel32.dll");

	// Is this a socket handle?
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo((SOCKET)hFile);

	if ((!info) || (info->IsSendDisabled()) ||
		((!lpOverlapped) || (!info->IsOverlapped())))
	{
		// Not a socket, send disabled, or not asynchronous
		g_socketList.Unlock();
		return realWriteFileEx(hFile, lpBuffer, nNumberOfBytesToWrite, lpOverlapped, lpCompletionRoutine);
	}

	info->AddRef();
	g_socketList.Unlock();

	// Asynchronous operation
	BOOL result = info->SendDataWriteFileAsync((const BYTE*)lpBuffer, nNumberOfBytesToWrite, lpOverlapped,
		lpCompletionRoutine);

	info->Release();
	return result;
}


//*************************************************************************
// Interception function for 'WriteFileGather' in kernel32.dll
// The WriteFileGather function gathers data from a set of buffers and writes it to a file.
//
// Parameters:
//   hFile                 - File or socket handle to write to
//   aSegmentArray         - Buffers containing data to write
//   nNumberOfBytesToWrite - Number of bytes to write
//   lpReserved            - Reserved
//   lpOverlapped          - OVERLAPPED structure for asynchronous operation
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_WriteFileGather(HANDLE hFile, FILE_SEGMENT_ELEMENT aSegmentArray[], DWORD nNumberOfBytesToWrite,
								   LPDWORD lpReserved, LPOVERLAPPED lpOverlapped)
{
	if (!realWriteFileGather)
		realWriteFileGather = (WriteFileGatherFunction)GetOriginalFunctionCaller("WriteFileGather", "kernel32.dll");
	// BUGBUG: Want to inject faults into data sent with a gathered write
	return realWriteFileGather(hFile, aSegmentArray, nNumberOfBytesToWrite, lpReserved, lpOverlapped);
}


//*************************************************************************
// Interception function for 'GetOverlappedResult' in kernel32.dll
// The GetOverlappedResult function retrieves the results of an overlapped operation
// on the specified file, named pipe, or communications device.
//
// Parameters:
//   hFile                      - File or socket handle
//   lpOverlapped               - OVERLAPPED structure used to start the operation
//   lpNumberOfBytesTransferred - Receives number of bytes transferred
//   bWait                      - If TRUE, does not return until operation completes
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_GetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred,
									   BOOL bWait)
{
	if (!realGetOverlappedResult)
		realGetOverlappedResult = (GetOverlappedResultFunction)GetOriginalFunctionCaller("GetOverlappedResult", "kernel32.dll");

	// Find the operation in our asynchronous operation list
	g_asyncList.Lock();
	AsyncOpInfo* op = g_asyncList.GetKernelOpInfo(lpOverlapped);

	if (!op)
	{
		// Not an operation we care about
		g_asyncList.Unlock();
		return realGetOverlappedResult(hFile, lpOverlapped, lpNumberOfBytesTransferred, bWait);
	}

	// Unlock the list in case we end up waiting for completion
	op->AddRef();
	g_asyncList.Unlock();

	// Call the original function to get the status
	BOOL result = realGetOverlappedResult(hFile, lpOverlapped, lpNumberOfBytesTransferred, bWait);

	if (!result)
	{
		// Not completed
		op->Release();
		return result;
	}

	// Completed, report the number of bytes transferred that the application will expect
	if (lpNumberOfBytesTransferred)
		*lpNumberOfBytesTransferred = op->GetTransferLength();

	op->Release();
	return result;
}


//*************************************************************************
// Interception function for 'CancelIo' in kernel32.dll
// The CancelIo function cancels all pending input and output (I/O) operations that
// were issued by the calling thread for the specified file handle. 
//
// Parameters:
//   hFile - File or socket handle to cancel operations for
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_CancelIo(HANDLE hFile)
{
	if (!realCancelIo)
		realCancelIo = (CancelIoFunction)GetOriginalFunctionCaller("CancelIo", "kernel32.dll");

	BOOL result = realCancelIo(hFile);

	if (result)
	{
		// CancelIo succeeded, go ahead and remove the affected operations
		// from the asynchronous operation list
		g_asyncList.Lock();
		g_asyncList.CancelIo(hFile);
		g_asyncList.Unlock();
	}

	return result;
}


//*************************************************************************
// Interception function for 'CloseHandle' in kernel32.dll
// The CloseHandle function closes an open object handle.
//
// Parameters:
//   hObject - Handle to close
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_CloseHandle(HANDLE hObject)
{
	if (!realCloseHandle)
		realCloseHandle = (CloseHandleFunction)GetOriginalFunctionCaller("CloseHandle", "kernel32.dll");

	// Sockets are kernel object handles in NT.  If CloseHandle is called on a socket object, close
	// it in the socket list.
	g_socketList.Lock();
	if (g_socketList.GetSocketInfo((SOCKET)hObject) != NULL)
		g_socketList.CloseSocket((SOCKET)hObject);
	g_socketList.Unlock();
	return realCloseHandle(hObject);
}


//*************************************************************************
// Interception function for 'GetQueuedCompletionStatus' in kernel32.dll
// The GetQueuedCompletionStatus function attempts to dequeue an I/O
// completion packet from the specified I/O completion port.
//
// Parameters:
//   CompletionPort - Handle to the completion port of interest
//   lpNumberOfBytes - Receives number of bytes transferred
//   lpCompletionKey - Receives completion key for the operation
//   lpOverlapped - Receives OVERLAPPED structure for the operation
//   dwMilliseconds - Number of milliseconds to wait
//
// Return value:
//   TRUE on success, FALSE on failure.
//*************************************************************************
BOOL WINAPI NetCorrupt_GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytes,
												 PULONG_PTR lpCompletionKey, LPOVERLAPPED* lpOverlapped,
												 DWORD dwMilliseconds)
{
	if (!realGetQueuedCompletionStatus)
		realGetQueuedCompletionStatus = (GetQueuedCompletionStatusFunction)GetOriginalFunctionCaller("GetQueuedCompletionStatus", "kernel32.dll");

	// Call the original function to get the status
	BOOL result = realGetQueuedCompletionStatus(CompletionPort, lpNumberOfBytes, lpCompletionKey, lpOverlapped, dwMilliseconds);
	if (!result)
	{
		// Not completed
		return result;
	}

	// Find the operation in our asynchronous operation list
	g_asyncList.Lock();
	AsyncOpInfo* op = g_asyncList.GetWSOpInfo((LPWSAOVERLAPPED)*lpOverlapped);

	if (!op)
	{
		// Not a Winsock operation, try kernel
		op = g_asyncList.GetKernelOpInfo(*lpOverlapped);
		if (!op)
		{
			// Can't find the operation
			g_asyncList.Unlock();
			return result;
		}
	}

	// Unlock the list in case we end up waiting for completion
	op->AddRef();
	g_asyncList.Unlock();

	// Trigger the completion routine manually to run the data through the corruption filters.
	if (op->IsWSAyncOp())
		NetCorrupt_WSRecvCompletionRoutine(0, *lpNumberOfBytes, (LPWSAOVERLAPPED)*lpOverlapped, 0);
	else
		NetCorrupt_KernelRecvCompletionRoutine(0, *lpNumberOfBytes, *lpOverlapped);

	// Completed, report the number of bytes transferred that the application will expect
	if (lpNumberOfBytes)
		*lpNumberOfBytes = op->GetTransferLength();

	op->Release();
	return result;
}


//*************************************************************************
// Interception function for 'CreateIoCompletionPort' in kernel32.dll
// The CreateIoCompletionPort function associates an I/O completion port
// with one or more file handles
//
// Parameters:
//   CompletionPort - Handle to the file
//   ExistingCompletionPort - Existing completion port, or NULL to create new
//   CompletionKey - Per-file completion key
//   NumberOfConcurrentThreads - Maximum number of threads
//
// Return value:
//   I/O completion port handle, or NULL on failure
//*************************************************************************
HANDLE WINAPI NetCorrupt_CreateIoCompletionPort(HANDLE FileHandle, HANDLE ExistingCompletionPort,
												ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads)
{
	if (!realCreateIoCompletionPort)
		realCreateIoCompletionPort = (CreateIoCompletionPortFunction)GetOriginalFunctionCaller("CreateIoCompletionPort", "kernel32.dll");

	// Is this a socket handle?
	g_socketList.Lock();
	SocketInfo* info = g_socketList.GetSocketInfo((SOCKET)FileHandle);

	if (!info)
	{
		// Not a socket
		g_socketList.Unlock();
		return realCreateIoCompletionPort(FileHandle, ExistingCompletionPort, CompletionKey, NumberOfConcurrentThreads);
	}

	info->AddRef();
	g_socketList.Unlock();

	// Setting completion port on a socket
	HANDLE result = realCreateIoCompletionPort(FileHandle, ExistingCompletionPort, CompletionKey, NumberOfConcurrentThreads);

	if (!result)
	{
		info->Release();
		return result;
	}

	// New completion port created, attach it to the socket
	info->SetCompletionPort(result, CompletionKey);

	info->Release();
	return result;
}


//*************************************************************************
// Method:		InitNetworkCorruption
// Description: Initializes the network corruption backend
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void InitNetworkCorruption()
{
	if (settingsManager == NULL)
		settingsManager = new Replacement::SettingsManager(GetCurrentProcessId(), false);
	InitFileCorruption();
}


//*************************************************************************
// Method:		CloseNetworkCorruption
// Description: Closes down the network corruption backend
//
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void CloseNetworkCorruption()
{
	CloseFileCorruption();
	if (settingsManager != NULL)
	{
		delete settingsManager;
		settingsManager = NULL;
	}
}


void* GetOriginalFunctionCaller(char* szFuncName, char* szDLLName)
{
	HMODULE module;
	static void* (WINAPI *getOriginalCallerFunc)(char*, char*) = NULL;
	
	if (getOriginalCallerFunc == NULL)
	{
		module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
		if (module == NULL)
			module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

		if (module == NULL)
			return NULL;

		getOriginalCallerFunc = (void* (WINAPI *)(char*, char*))::GetProcAddress(module, ORIGINAL_CALLER_FUNC_NAME);
	}

	return getOriginalCallerFunc(szFuncName, szDLLName);
}
