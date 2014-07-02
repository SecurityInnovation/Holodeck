//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		NetCorruptLibrary.h
//
// DESCRIPTION: Main include file for Holodeck network corruption.
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
//*************************************************************************

#ifndef __NETCORRUPTLIBRARY_H__
#define __NETCORRUPTLIBRARY_H__

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#include <mswsock.h>
#include <windows.h>
#include <shlwapi.h>
#include <time.h>
#include <map>
#include <list>
#include <string>
#include <regex>
#include "SettingsManager.h"

using namespace std;
using namespace jm; // regular expression parser


SOCKET PASCAL NetCorrupt_accept(SOCKET s, struct sockaddr* addr, int* addrlen);
int PASCAL NetCorrupt_bind(SOCKET s, const struct sockaddr* name, int namelen);
int PASCAL NetCorrupt_closesocket(SOCKET s); // !!! BUGBUG: not intercepted
int PASCAL NetCorrupt_connect(SOCKET s, const struct sockaddr* name, int namelen);
int PASCAL NetCorrupt_ioctlsocket(SOCKET s, long cmd, u_long* argp);
int PASCAL NetCorrupt_recv(SOCKET s, char* buf, int len, int flags);
int PASCAL NetCorrupt_recvfrom(SOCKET s, char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
int PASCAL NetCorrupt_select(int nfds, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, const struct timeval* timeout);
int PASCAL NetCorrupt_send(SOCKET s, const char* buf, int len, int flags);
int PASCAL NetCorrupt_sendto(SOCKET s, const char* buf, int len, int flags, const struct sockaddr* to, int tolen);
int PASCAL NetCorrupt_shutdown(SOCKET s, int how);
SOCKET PASCAL NetCorrupt_socket(int af, int type, int protocol);
SOCKET PASCAL NetCorrupt_WSAAccept(SOCKET s, struct sockaddr* addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition,
							   DWORD dwCallbackData);
int PASCAL NetCorrupt_WSAConnect(SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData,
							 LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS);
BOOL PASCAL NetCorrupt_WSAGetOverlappedResult(SOCKET s, LPWSAOVERLAPPED lpOverlapped, LPDWORD lpcbTransfer, BOOL fWait,
										  LPDWORD lpdwFlags); // !!! BUGBUG: not intercepted
int PASCAL NetCorrupt_WSAIoctl(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer,
						   DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPWSAOVERLAPPED lpOverlapped,
						   LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
SOCKET PASCAL NetCorrupt_WSAJoinLeaf(SOCKET s, const struct sockaddr* name, int namelen, LPWSABUF lpCallerData,
								 LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS, DWORD dwFlags);
int PASCAL NetCorrupt_WSARecv(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags,
						  LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int PASCAL NetCorrupt_WSARecvFrom(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd,
							  LPDWORD lpFlags, struct sockaddr* lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped,
							  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int PASCAL NetCorrupt_WSASend(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
						  LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
int PASCAL NetCorrupt_WSASendTo(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags,
							const struct sockaddr* lpTo, int iToLen, LPWSAOVERLAPPED lpOverlapped,
							LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
SOCKET PASCAL NetCorrupt_WSASocketA(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags);
SOCKET PASCAL NetCorrupt_WSASocketW(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags);
BOOL WINAPI NetCorrupt_ReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead,
							LPOVERLAPPED lpOverlapped);
BOOL WINAPI NetCorrupt_ReadFileEx(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped,
							  LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
BOOL WINAPI NetCorrupt_ReadFileScatter(HANDLE hFile, FILE_SEGMENT_ELEMENT aSegmentArray[], DWORD nNumberOfBytesToRead,
								   LPDWORD lpReserved, LPOVERLAPPED lpOverlapped);
BOOL WINAPI NetCorrupt_WriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite,LPDWORD lpNumberOfBytesWritten,
							 LPOVERLAPPED lpOverlapped);
BOOL WINAPI NetCorrupt_WriteFileEx(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPOVERLAPPED lpOverlapped,
							   LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
BOOL WINAPI NetCorrupt_WriteFileGather(HANDLE hFile, FILE_SEGMENT_ELEMENT aSegmentArray[], DWORD nNumberOfBytesToWrite,
								   LPDWORD lpReserved, LPOVERLAPPED lpOverlapped);
BOOL WINAPI NetCorrupt_GetOverlappedResult(HANDLE hFile, LPOVERLAPPED lpOverlapped, LPDWORD lpNumberOfBytesTransferred,
									   BOOL bWait); // !!! BUGBUG: not intercepted
BOOL WINAPI NetCorrupt_CancelIo(HANDLE hFile);
BOOL WINAPI NetCorrupt_CloseHandle(HANDLE hObject);
BOOL WINAPI NetCorrupt_GetQueuedCompletionStatus(HANDLE CompletionPort, LPDWORD lpNumberOfBytes,
												 PULONG_PTR lpCompletionKey, LPOVERLAPPED* lpOverlapped,
												 DWORD dwMilliseconds);
HANDLE WINAPI NetCorrupt_CreateIoCompletionPort(HANDLE FileHandle, HANDLE ExistingCompletionPort,
												ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads);
HANDLE WINAPI FileCorrupt_CreateFileA(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
									  LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
									  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE WINAPI FileCorrupt_CreateFileW(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode,
									  LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition,
									  DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);


// Original function pointer types for network corruption
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *acceptFunction) (SOCKET, struct sockaddr FAR *, int FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *bindFunction)(SOCKET, const struct sockaddr FAR*, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *closesocketFunction)(SOCKET);
typedef WINSOCK_API_LINKAGE int (WSAAPI *connectFunction)(SOCKET, const struct sockaddr FAR*, int);
typedef WINSOCK_API_LINKAGE struct hostent FAR * (WSAAPI *gethostbyaddrFunction) (const char FAR *, int, int);
typedef WINSOCK_API_LINKAGE struct hostent FAR * (WSAAPI *gethostbynameFunction) (const char FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *getsockoptFunction) (SOCKET, int, int, char FAR *, int FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *ioctlsocketFunction)(SOCKET, long, u_long FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *listenFunction) (SOCKET, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *recvFunction)(SOCKET, char FAR*, int, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *recvfromFunction) (SOCKET, char FAR *, int, int, struct sockaddr FAR *, int FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *selectFunction)(int, fd_set FAR*, fd_set FAR *, fd_set FAR *, const struct timeval FAR *);
typedef WINSOCK_API_LINKAGE int (WSAAPI *sendFunction)(SOCKET, const char FAR*, int, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *sendtoFunction) (SOCKET, const char FAR *, int, int, const struct sockaddr FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *setsockoptFunction) (SOCKET, int, int, const char FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *shutdownFunction) (SOCKET, int);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *socketFunction)(int, int, int);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSAAcceptFunction) (SOCKET, struct sockaddr FAR *, LPINT, LPCONDITIONPROC, DWORD_PTR);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetHostByAddrFunction) (HWND, u_int, const char FAR *, int, int, char FAR *, int);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetHostByNameFunction) (HWND, u_int, const char FAR *, char FAR *, int);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetServByNameFunction) (HWND, u_int, const char FAR *, const char FAR *, char FAR *, int);
typedef WINSOCK_API_LINKAGE HANDLE (WSAAPI *WSAAsyncGetServByPortFunction) (HWND, u_int, int, const char FAR *, char FAR *, int);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAAsyncSelectFunction) (SOCKET, HWND, u_int, long);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSACancelAsyncRequestFunction) (HANDLE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAConnectFunction)(SOCKET, const struct sockaddr FAR *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSADuplicateSocketAFunction) (SOCKET, DWORD, LPWSAPROTOCOL_INFOA);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSADuplicateSocketWFunction) (SOCKET, DWORD, LPWSAPROTOCOL_INFOW);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAEventSelectFunction)(SOCKET, WSAEVENT, long);
typedef WINSOCK_API_LINKAGE BOOL (WSAAPI *WSAGetOverlappedResultFunction)(SOCKET, LPWSAOVERLAPPED, LPDWORD, BOOL, LPDWORD);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSAIoctlFunction)(SOCKET, DWORD, LPVOID, DWORD, LPVOID, DWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSAJoinLeafFunction) (SOCKET, const struct sockaddr FAR *, int, LPWSABUF, LPWSABUF, LPQOS, LPQOS, DWORD);
typedef WINSOCK_API_LINKAGE INT (WSAAPI *WSAProviderConfigChangeFunction) (LPHANDLE, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSARecvFunction)(SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSARecvDisconnectFunction) (SOCKET, LPWSABUF);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSARecvFromFunction) (SOCKET, LPWSABUF, DWORD, LPDWORD, LPDWORD, struct sockaddr FAR *, LPINT, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSASendFunction)(SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSASendDisconnectFunction) (SOCKET, LPWSABUF);
typedef WINSOCK_API_LINKAGE int (WSAAPI *WSASendToFunction) (SOCKET, LPWSABUF, DWORD, LPDWORD, DWORD, const struct sockaddr FAR *, int, LPWSAOVERLAPPED, LPWSAOVERLAPPED_COMPLETION_ROUTINE);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSASocketAFunction)(int, int, int, LPWSAPROTOCOL_INFOA, GROUP, DWORD);
typedef WINSOCK_API_LINKAGE SOCKET (WSAAPI *WSASocketWFunction)(int, int, int, LPWSAPROTOCOL_INFOW, GROUP, DWORD);
typedef WINBASEAPI BOOL (WINAPI *ReadFileFunction)(HANDLE, LPVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef WINBASEAPI BOOL (WINAPI *ReadFileExFunction)(HANDLE, LPVOID, DWORD, LPOVERLAPPED, LPOVERLAPPED_COMPLETION_ROUTINE);
typedef WINBASEAPI BOOL (WINAPI *ReadFileScatterFunction)(HANDLE, FILE_SEGMENT_ELEMENT[], DWORD, LPDWORD, LPOVERLAPPED);
typedef WINBASEAPI BOOL (WINAPI *WriteFileFunction)(HANDLE, LPCVOID, DWORD, LPDWORD, LPOVERLAPPED);
typedef WINBASEAPI BOOL (WINAPI *WriteFileExFunction)(HANDLE, LPCVOID, DWORD, LPOVERLAPPED, LPOVERLAPPED_COMPLETION_ROUTINE);
typedef WINBASEAPI BOOL (WINAPI *WriteFileGatherFunction)(HANDLE, FILE_SEGMENT_ELEMENT[], DWORD, LPDWORD, LPOVERLAPPED);
typedef WINBASEAPI BOOL (WINAPI *CloseHandleFunction)(HANDLE);
typedef WINBASEAPI BOOL (WINAPI *CancelIoFunction)(HANDLE);
typedef WINBASEAPI BOOL (WINAPI *GetOverlappedResultFunction)(HANDLE, LPOVERLAPPED, LPDWORD, BOOL);
typedef WINBASEAPI HANDLE (WINAPI *CreateFileAFunction)(LPCSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef WINBASEAPI HANDLE (WINAPI *CreateFileWFunction)(LPCWSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef WINBASEAPI BOOL (WINAPI *GetQueuedCompletionStatusFunction)(HANDLE CompletionPort, LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED* lpOverlapped, DWORD dwMilliseconds);
typedef WINBASEAPI HANDLE (WINAPI *CreateIoCompletionPortFunction)(HANDLE FileHandle, HANDLE ExistingCompletionPort, ULONG_PTR CompletionKey, DWORD NumberOfConcurrentThreads);


// Original function pointers for network corruption
extern acceptFunction realaccept;
extern bindFunction realbind;
extern closesocketFunction realclosesocket;
extern connectFunction realconnect;
extern gethostbyaddrFunction realgethostbyaddr;
extern gethostbynameFunction realgethostbyname;
extern getsockoptFunction realgetsockopt;
extern ioctlsocketFunction realioctlsocket;
extern listenFunction reallisten;
extern recvFunction realrecv;
extern recvfromFunction realrecvfrom;
extern selectFunction realselect;
extern sendFunction realsend;
extern sendtoFunction realsendto;
extern setsockoptFunction realsetsockopt;
extern shutdownFunction realshutdown;
extern socketFunction realsocket;
extern WSAAcceptFunction realWSAAccept;
extern WSAAsyncGetHostByAddrFunction realWSAAsyncGetHostByAddr;
extern WSAAsyncGetHostByNameFunction realWSAAsyncGetHostByName;
extern WSAAsyncGetServByNameFunction realWSAAsyncGetServByName;
extern WSAAsyncGetServByPortFunction realWSAAsyncGetServByPort;
extern WSAAsyncSelectFunction realWSAAsyncSelect;
extern WSACancelAsyncRequestFunction realWSACancelAsyncRequest;
extern WSAConnectFunction realWSAConnect;
extern WSADuplicateSocketAFunction realWSADuplicateSocketA;
extern WSADuplicateSocketWFunction realWSADuplicateSocketW;
extern WSAEventSelectFunction realWSAEventSelect;
extern WSAGetOverlappedResultFunction realWSAGetOverlappedResult;
extern WSAIoctlFunction realWSAIoctl;
extern WSAJoinLeafFunction realWSAJoinLeaf;
extern WSAProviderConfigChangeFunction realWSAProviderConfigChange;
extern WSARecvFunction realWSARecv;
extern WSARecvDisconnectFunction realWSARecvDisconnect;
extern WSARecvFromFunction realWSARecvFrom;
extern WSASendFunction realWSASend;
extern WSASendDisconnectFunction realWSASendDisconnect;
extern WSASendToFunction realWSASendTo;
extern WSASocketAFunction realWSASocketA;
extern WSASocketWFunction realWSASocketW;
extern ReadFileFunction realReadFile;
extern ReadFileExFunction realReadFileEx;
extern ReadFileScatterFunction realReadFileScatter;
extern WriteFileFunction realWriteFile;
extern WriteFileExFunction realWriteFileEx;
extern WriteFileGatherFunction realWriteFileGather;
extern CloseHandleFunction realCloseHandle;
extern CancelIoFunction realCancelIo;
extern GetOverlappedResultFunction realGetOverlappedResult;
extern CreateFileAFunction realCreateFileA;
extern CreateFileWFunction realCreateFileW;
extern GetQueuedCompletionStatusFunction realGetQueuedCompletionStatus;
extern CreateIoCompletionPortFunction realCreateIoCompletionPort;

extern Replacement::SettingsManager * settingsManager;


void InitNetworkCorruption();
void CloseNetworkCorruption();
void InitFileCorruption();
void CloseFileCorruption();
void* GetOriginalFunctionCaller(char* szFuncName, char* szDLLName);


#endif
