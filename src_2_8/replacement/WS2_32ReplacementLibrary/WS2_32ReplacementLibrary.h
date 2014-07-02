//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		WS2_32ReplacementLibrary.h
//
// DESCRIPTION: Contains replacement library prototypes for ws2_32.dll
//
//=========================================================================
// Modification History
//
// Generated 04/13/2004 01:51:17 AM
//*************************************************************************
#pragma once

#include <winsock2.h>
#include <windows.h>
#include "ReplacementLibrary.h"
#include "Log.h"

using namespace Replacement;
using namespace Log;

// function pointer definitions
typedef WINSOCK_API_LINKAGE int (WSAAPI *bindFunction)(SOCKET, const struct sockaddr FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *connectFunction)(SOCKET, const struct sockaddr FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *ioctlsocketFunction)(SOCKET, long, u_long FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *recvFunction)(SOCKET, char FAR *, int, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *selectFunction)(int, fd_set FAR *, fd_set FAR *, fd_set FAR *, const struct timeval FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *sendFunction)(SOCKET, const char FAR *, int, int);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *socketFunction)(int, int, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAConnectFunction)(SOCKET, const struct sockaddr FAR *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAEventSelectFunction)(SOCKET, WSAEVENT, long);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAIoctlFunction)(SOCKET, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSARecvFunction)(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSASendFunction)(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, LPOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSASocketAFunction)(int, int, int, LPWSAPROTOCOL_INFOA, GROUP, DWORD);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSASocketWFunction)(int, int, int, LPWSAPROTOCOL_INFOW, GROUP, DWORD);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAStartupFunction)(WORD, LPWSADATA);
typedef WINSOCK_API_LINKAGE int (WSAAPI *recvfromFunction)(SOCKET, char FAR *, int, int, struct sockaddr FAR *, int FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *sendtoFunction)(SOCKET, const char FAR *, int, int, const struct sockaddr FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *setsockoptFunction)(SOCKET, int, int, const char FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *getsockoptFunction)(SOCKET, int, int, char FAR *, int FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *listenFunction)(SOCKET, int);
typedef WINSOCK_API_LINKAGE struct hostent FAR * (WSAAPI *gethostbyaddrFunction)(const char FAR *, int, int);
typedef WINSOCK_API_LINKAGE struct hostent FAR * (WSAAPI *gethostbynameFunction)(const char FAR *);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *acceptFunction)(SOCKET, struct sockaddr FAR *, int FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSARecvFromFunction)(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, struct sockaddr FAR *, LPINT, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSASendDisconnectFunction)(SOCKET, LPWSABUF);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSAJoinLeafFunction)(SOCKET, const struct sockaddr FAR *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS, DWORD);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSARecvDisconnectFunction)(SOCKET, LPWSABUF);
typedef WINSOCK_API_LINKAGE INT (WSAAPI *WSAProviderConfigChangeFunction)(LPHANDLE, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSADuplicateSocketAFunction)(SOCKET, DWORD, LPWSAPROTOCOL_INFOA);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSADuplicateSocketWFunction)(SOCKET, DWORD, LPWSAPROTOCOL_INFOW);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAAsyncSelectFunction)(SOCKET, HWND, u_int, long);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSACancelAsyncRequestFunction)(HANDLE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSACleanupFunction)();
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSAAcceptFunction)(SOCKET, struct sockaddr FAR *, LPINT, LPCONDITIONPROC, DWORD);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetHostByAddrFunction)(HWND, u_int, const char FAR *, int, int, char FAR *, int);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetHostByNameFunction)(HWND, u_int, const char FAR *, char FAR *, int);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetServByNameFunction)(HWND, u_int, const char FAR *, const char FAR *, char FAR *, int);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetServByPortFunction)(HWND, u_int, int, const char FAR *, char FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *shutdownFunction)(SOCKET, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSASendToFunction)(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, const struct sockaddr FAR *, int, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *closesocketFunction)(SOCKET);

// original function pointers
bindFunction realbind = NULL;
connectFunction realconnect = NULL;
ioctlsocketFunction realioctlsocket = NULL;
recvFunction realrecv = NULL;
selectFunction realselect = NULL;
sendFunction realsend = NULL;
socketFunction realsocket = NULL;
WSAConnectFunction realWSAConnect = NULL;
WSAEventSelectFunction realWSAEventSelect = NULL;
WSAIoctlFunction realWSAIoctl = NULL;
WSARecvFunction realWSARecv = NULL;
WSASendFunction realWSASend = NULL;
WSASocketAFunction realWSASocketA = NULL;
WSASocketWFunction realWSASocketW = NULL;
WSAStartupFunction realWSAStartup = NULL;
recvfromFunction realrecvfrom = NULL;
sendtoFunction realsendto = NULL;
setsockoptFunction realsetsockopt = NULL;
getsockoptFunction realgetsockopt = NULL;
listenFunction reallisten = NULL;
gethostbyaddrFunction realgethostbyaddr = NULL;
gethostbynameFunction realgethostbyname = NULL;
acceptFunction realaccept = NULL;
WSARecvFromFunction realWSARecvFrom = NULL;
WSASendDisconnectFunction realWSASendDisconnect = NULL;
WSAJoinLeafFunction realWSAJoinLeaf = NULL;
WSARecvDisconnectFunction realWSARecvDisconnect = NULL;
WSAProviderConfigChangeFunction realWSAProviderConfigChange = NULL;
WSADuplicateSocketAFunction realWSADuplicateSocketA = NULL;
WSADuplicateSocketWFunction realWSADuplicateSocketW = NULL;
WSAAsyncSelectFunction realWSAAsyncSelect = NULL;
WSACancelAsyncRequestFunction realWSACancelAsyncRequest = NULL;
WSACleanupFunction realWSACleanup = NULL;
WSAAcceptFunction realWSAAccept = NULL;
WSAAsyncGetHostByAddrFunction realWSAAsyncGetHostByAddr = NULL;
WSAAsyncGetHostByNameFunction realWSAAsyncGetHostByName = NULL;
WSAAsyncGetServByNameFunction realWSAAsyncGetServByName = NULL;
WSAAsyncGetServByPortFunction realWSAAsyncGetServByPort = NULL;
shutdownFunction realshutdown = NULL;
WSASendToFunction realWSASendTo = NULL;
closesocketFunction realclosesocket = NULL;

ReplacementLibrary *library = NULL;
LogSender *logSender = NULL;
