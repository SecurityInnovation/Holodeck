//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004  All Rights Reserved.
//
// FILE:		WS2_32ReplacementLibrary.cpp
//
// DESCRIPTION: Contains replacement library functions for ws2_32.dll
//
//=========================================================================
// Modification History
//
// Generated 04/20/2004 02:48:48 PM
//*************************************************************************
#include "ws2_32replacementlibrary.h"
#include "NetCorruptLibrary.h"

//*************************************************************************
// Method:		ReplacementLibraryAttach
// Description: Called when HEAT is attaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryAttach()
{
	ReplacementLibrary::DisableInterception();
	if (library == NULL)
	{
		library = new ReplacementLibrary("ws2_32.dll");
		logSender = &commonLogSender;

		InitNetworkCorruption();

		if (!logSender->GetIsSendLogThreadRunning())
			logSender->StartSendLogThread();
	}
	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		ReplacementLibraryDetach
// Description: Called when HEAT is detaching
// Parameters: None
// Return Value: None
//*************************************************************************
void ReplacementLibraryDetach()
{
	ReplacementLibrary::DisableInterception();
	if (library != NULL)
	{
		if ((logSender != NULL) && (logSender->GetIsSendLogThreadRunning()))
		{
			logSender->StopSendLogThread();
			logSender = NULL;
		}

		CloseNetworkCorruption();
	}
	ReplacementLibrary::EnableInterception();
}

//*************************************************************************
// Method:		DllMain
// Description: Entry point to this dll
// Parameters: See MSDN DllMain function parameters
// Return Value: See MSDN DllMain return value
//*************************************************************************
BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	ReplacementLibrary::DisableInterception();
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		ReplacementLibraryDetach();
		delete library;
		library = NULL;
		return TRUE; // Don't re-enable intercepts, as we are detaching
	default:
		break;
	}
	ReplacementLibrary::EnableInterception();

	return TRUE;
}

//*************************************************************************
// START OF ORIGINAL FUNCTION CALLER FUNCTIONS
// For all functions in this section the following applies
// Description:	Calls the original function for a replacement function
// Parameters:
//	numParams - the number of parameters in the params array
//	params - the parameters to pass to the original function
//	retVal - the return value from the original function
//	errCode - the error code from the original function
// Return Value: true if the correct number of parameters were passed in,
//	false otherwise
//*************************************************************************
//*************************************************************************
// Method: bindCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool bindCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	int tempRetValue = NetCorrupt_bind(*((SOCKET *)params[0]), *((const struct sockaddr FAR * *)params[1]), *((int *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: connectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool connectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	int tempRetValue = NetCorrupt_connect(*((SOCKET *)params[0]), *((const struct sockaddr FAR * *)params[1]), *((int *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: ioctlsocketCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool ioctlsocketCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	int tempRetValue = NetCorrupt_ioctlsocket(*((SOCKET *)params[0]), *((long *)params[1]), *((u_long FAR * *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: recvCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool recvCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	int tempRetValue = NetCorrupt_recv(*((SOCKET *)params[0]), *((char FAR * *)params[1]), *((int *)params[2]), *((int *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: selectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool selectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	int tempRetValue = NetCorrupt_select(*((int *)params[0]), *((fd_set FAR * *)params[1]), *((fd_set FAR * *)params[2]), *((fd_set FAR * *)params[3]), *((const struct timeval FAR * *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: sendCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool sendCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	int tempRetValue = NetCorrupt_send(*((SOCKET *)params[0]), *((const char FAR * *)params[1]), *((int *)params[2]), *((int *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: socketCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool socketCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	SOCKET tempRetValue = NetCorrupt_socket(*((int *)params[0]), *((int *)params[1]), *((int *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAConnectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAConnectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	int tempRetValue = NetCorrupt_WSAConnect(*((SOCKET *)params[0]), *((const struct sockaddr FAR * *)params[1]), *((int *)params[2]), *((LPWSABUF *)params[3]), *((LPWSABUF *)params[4]), *((LPQOS *)params[5]), *((LPQOS *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAEventSelectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAEventSelectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	int tempRetValue = realWSAEventSelect(*((SOCKET *)params[0]), *((WSAEVENT *)params[1]), *((long *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAIoctlCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAIoctlCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 9) return false;
	int tempRetValue = NetCorrupt_WSAIoctl(*((SOCKET *)params[0]), *((DWORD *)params[1]), *((LPVOID *)params[2]), *((DWORD *)params[3]), *((LPVOID *)params[4]), *((DWORD *)params[5]), *((LPDWORD *)params[6]), *((LPOVERLAPPED *)params[7]), *((LPWSAOVERLAPPED_COMPLETION_ROUTINE *)params[8]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSARecvCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSARecvCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	int tempRetValue = NetCorrupt_WSARecv(*((SOCKET *)params[0]), *((LPWSABUF *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((LPOVERLAPPED *)params[5]), *((LPWSAOVERLAPPED_COMPLETION_ROUTINE *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSASendCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSASendCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	int tempRetValue = NetCorrupt_WSASend(*((SOCKET *)params[0]), *((LPWSABUF *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((DWORD *)params[4]), *((LPOVERLAPPED *)params[5]), *((LPWSAOVERLAPPED_COMPLETION_ROUTINE *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSASocketACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSASocketACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	SOCKET tempRetValue = NetCorrupt_WSASocketA(*((int *)params[0]), *((int *)params[1]), *((int *)params[2]), *((LPWSAPROTOCOL_INFOA *)params[3]), *((GROUP *)params[4]), *((DWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSASocketWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSASocketWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	SOCKET tempRetValue = NetCorrupt_WSASocketW(*((int *)params[0]), *((int *)params[1]), *((int *)params[2]), *((LPWSAPROTOCOL_INFOW *)params[3]), *((GROUP *)params[4]), *((DWORD *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAStartupCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAStartupCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	int tempRetValue = realWSAStartup(*((WORD *)params[0]), *((LPWSADATA *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: recvfromCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool recvfromCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	int tempRetValue = NetCorrupt_recvfrom(*((SOCKET *)params[0]), *((char FAR * *)params[1]), *((int *)params[2]), *((int *)params[3]), *((struct sockaddr FAR * *)params[4]), *((int FAR * *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: sendtoCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool sendtoCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	int tempRetValue = NetCorrupt_sendto(*((SOCKET *)params[0]), *((const char FAR * *)params[1]), *((int *)params[2]), *((int *)params[3]), *((const struct sockaddr FAR * *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: setsockoptCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool setsockoptCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	int tempRetValue = realsetsockopt(*((SOCKET *)params[0]), *((int *)params[1]), *((int *)params[2]), *((const char FAR * *)params[3]), *((int *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: getsockoptCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool getsockoptCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	int tempRetValue = realgetsockopt(*((SOCKET *)params[0]), *((int *)params[1]), *((int *)params[2]), *((char FAR * *)params[3]), *((int FAR * *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: listenCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool listenCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	int tempRetValue = reallisten(*((SOCKET *)params[0]), *((int *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: gethostbyaddrCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool gethostbyaddrCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	struct hostent FAR * tempRetValue = realgethostbyaddr(*((const char FAR * *)params[0]), *((int *)params[1]), *((int *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: gethostbynameCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool gethostbynameCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	struct hostent FAR * tempRetValue = realgethostbyname(*((const char FAR * *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: acceptCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool acceptCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	SOCKET tempRetValue = NetCorrupt_accept(*((SOCKET *)params[0]), *((struct sockaddr FAR * *)params[1]), *((int FAR * *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSARecvFromCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSARecvFromCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 9) return false;
	int tempRetValue = NetCorrupt_WSARecvFrom(*((SOCKET *)params[0]), *((LPWSABUF *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((LPDWORD *)params[4]), *((struct sockaddr FAR * *)params[5]), *((LPINT *)params[6]), *((LPWSAOVERLAPPED *)params[7]), *((LPWSAOVERLAPPED_COMPLETION_ROUTINE *)params[8]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSASendDisconnectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSASendDisconnectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	int tempRetValue = realWSASendDisconnect(*((SOCKET *)params[0]), *((LPWSABUF *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAJoinLeafCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAJoinLeafCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 8) return false;
	SOCKET tempRetValue = NetCorrupt_WSAJoinLeaf(*((SOCKET *)params[0]), *((const struct sockaddr FAR * *)params[1]), *((int *)params[2]), *((LPWSABUF *)params[3]), *((LPWSABUF *)params[4]), *((LPQOS *)params[5]), *((LPQOS *)params[6]), *((DWORD *)params[7]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSARecvDisconnectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSARecvDisconnectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	int tempRetValue = realWSARecvDisconnect(*((SOCKET *)params[0]), *((LPWSABUF *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAProviderConfigChangeCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAProviderConfigChangeCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	INT tempRetValue = realWSAProviderConfigChange(*((LPHANDLE *)params[0]), *((LPWSAOVERLAPPED *)params[1]), *((LPWSAOVERLAPPED_COMPLETION_ROUTINE *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSADuplicateSocketACaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSADuplicateSocketACaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	int tempRetValue = realWSADuplicateSocketA(*((SOCKET *)params[0]), *((DWORD *)params[1]), *((LPWSAPROTOCOL_INFOA *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSADuplicateSocketWCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSADuplicateSocketWCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 3) return false;
	int tempRetValue = realWSADuplicateSocketW(*((SOCKET *)params[0]), *((DWORD *)params[1]), *((LPWSAPROTOCOL_INFOW *)params[2]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAAsyncSelectCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAAsyncSelectCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 4) return false;
	int tempRetValue = realWSAAsyncSelect(*((SOCKET *)params[0]), *((HWND *)params[1]), *((u_int *)params[2]), *((long *)params[3]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSACancelAsyncRequestCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSACancelAsyncRequestCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	int tempRetValue = realWSACancelAsyncRequest(*((HANDLE *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSACleanupCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSACleanupCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 0) return false;
	int tempRetValue = realWSACleanup();
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAAcceptCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAAcceptCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	SOCKET tempRetValue = NetCorrupt_WSAAccept(*((SOCKET *)params[0]), *((struct sockaddr FAR * *)params[1]), *((LPINT *)params[2]), *((LPCONDITIONPROC *)params[3]), *((DWORD *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAAsyncGetHostByAddrCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAAsyncGetHostByAddrCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 7) return false;
	HANDLE tempRetValue = realWSAAsyncGetHostByAddr(*((HWND *)params[0]), *((u_int *)params[1]), *((const char FAR * *)params[2]), *((int *)params[3]), *((int *)params[4]), *((char FAR * *)params[5]), *((int *)params[6]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAAsyncGetHostByNameCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAAsyncGetHostByNameCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 5) return false;
	HANDLE tempRetValue = realWSAAsyncGetHostByName(*((HWND *)params[0]), *((u_int *)params[1]), *((const char FAR * *)params[2]), *((char FAR * *)params[3]), *((int *)params[4]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAAsyncGetServByNameCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAAsyncGetServByNameCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realWSAAsyncGetServByName(*((HWND *)params[0]), *((u_int *)params[1]), *((const char FAR * *)params[2]), *((const char FAR * *)params[3]), *((char FAR * *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSAAsyncGetServByPortCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSAAsyncGetServByPortCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 6) return false;
	HANDLE tempRetValue = realWSAAsyncGetServByPort(*((HWND *)params[0]), *((u_int *)params[1]), *((int *)params[2]), *((const char FAR * *)params[3]), *((char FAR * *)params[4]), *((int *)params[5]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: shutdownCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool shutdownCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 2) return false;
	int tempRetValue = NetCorrupt_shutdown(*((SOCKET *)params[0]), *((int *)params[1]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: WSASendToCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool WSASendToCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 9) return false;
	int tempRetValue = NetCorrupt_WSASendTo(*((SOCKET *)params[0]), *((LPWSABUF *)params[1]), *((DWORD *)params[2]), *((LPDWORD *)params[3]), *((DWORD *)params[4]), *((const struct sockaddr FAR * *)params[5]), *((int *)params[6]), *((LPWSAOVERLAPPED *)params[7]), *((LPWSAOVERLAPPED_COMPLETION_ROUTINE *)params[8]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// Method: closesocketCaller - See START OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************
bool closesocketCaller(int numParams, void **params, DWORD *retValue, DWORD *errCode)
{
	if (numParams != 1) return false;
	int tempRetValue = NetCorrupt_closesocket(*((SOCKET *)params[0]));
	memcpy(retValue, &tempRetValue, sizeof(DWORD));
	*errCode = WSAGetLastError();
	return true;
}
//*************************************************************************
// END OF ORIGINAL FUNCTION CALLER FUNCTIONS
//*************************************************************************

//*************************************************************************
// START OF REPLACEMENT FUNCTIONS
//*************************************************************************
//*************************************************************************
// Method:		bindReplacement
// Description: See MSDN bind function
// Parameters:	See MSDN bind parameters
// Return Value: See MSDN bind return value
//*************************************************************************
int WSAAPI bindReplacement(SOCKET s, const struct sockaddr FAR * name, int namelen)
{
	const int numParams = 3;
	char *functionName = "bind";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &name, &namelen };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType };
	
	if (realbind == NULL)
		realbind = (bindFunction)library->GetOriginalFunction(functionName);
		
	if (realbind != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, bindCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		connectReplacement
// Description: See MSDN connect function
// Parameters:	See MSDN connect parameters
// Return Value: See MSDN connect return value
//*************************************************************************
int WSAAPI connectReplacement(SOCKET s, const struct sockaddr FAR * name, int namelen)
{
	const int numParams = 3;
	char *functionName = "connect";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &name, &namelen };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType };
	
	if (realconnect == NULL)
		realconnect = (connectFunction)library->GetOriginalFunction(functionName);
		
	if (realconnect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, connectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		ioctlsocketReplacement
// Description: See MSDN ioctlsocket function
// Parameters:	See MSDN ioctlsocket parameters
// Return Value: See MSDN ioctlsocket return value
//*************************************************************************
int WSAAPI ioctlsocketReplacement(SOCKET s, long cmd, u_long FAR * argp)
{
	const int numParams = 3;
	char *functionName = "ioctlsocket";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &cmd, &argp };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, PointerType };
	
	if (realioctlsocket == NULL)
		realioctlsocket = (ioctlsocketFunction)library->GetOriginalFunction(functionName);
		
	if (realioctlsocket != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, ioctlsocketCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		recvReplacement
// Description: See MSDN recv function
// Parameters:	See MSDN recv parameters
// Return Value: See MSDN recv return value
//*************************************************************************
int WSAAPI recvReplacement(SOCKET s, char FAR * buf, int len, int flags)
{
	const int numParams = 4;
	char *functionName = "recv";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &buf, &len, &flags };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, IntegerType };
	
	if (realrecv == NULL)
		realrecv = (recvFunction)library->GetOriginalFunction(functionName);
		
	if (realrecv != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, recvCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		selectReplacement
// Description: See MSDN select function
// Parameters:	See MSDN select parameters
// Return Value: See MSDN select return value
//*************************************************************************
int WSAAPI selectReplacement(int nfds, fd_set FAR * readfds, fd_set FAR * writefds, fd_set FAR * exceptfds, const struct timeval FAR * timeout)
{
	const int numParams = 5;
	char *functionName = "select";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &nfds, &readfds, &writefds, &exceptfds, &timeout };
	ParameterType paramTypes[numParams] = { IntegerType, PointerType, PointerType, PointerType, PointerType };
	
	if (realselect == NULL)
		realselect = (selectFunction)library->GetOriginalFunction(functionName);
		
	if (realselect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, selectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		sendReplacement
// Description: See MSDN send function
// Parameters:	See MSDN send parameters
// Return Value: See MSDN send return value
//*************************************************************************
int WSAAPI sendReplacement(SOCKET s, const char FAR * buf, int len, int flags)
{
	const int numParams = 4;
	char *functionName = "send";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &buf, &len, &flags };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, IntegerType };
	
	if (realsend == NULL)
		realsend = (sendFunction)library->GetOriginalFunction(functionName);
		
	if (realsend != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, sendCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		socketReplacement
// Description: See MSDN socket function
// Parameters:	See MSDN socket parameters
// Return Value: See MSDN socket return value
//*************************************************************************
SOCKET WSAAPI socketReplacement(int af, int type, int protocol)
{
	const int numParams = 3;
	char *functionName = "socket";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &af, &type, &protocol };
	ParameterType paramTypes[numParams] = { IntegerType, IntegerType, IntegerType };
	
	if (realsocket == NULL)
		realsocket = (socketFunction)library->GetOriginalFunction(functionName);
		
	if (realsocket != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SOCKET returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, socketCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SOCKET", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAConnectReplacement
// Description: See MSDN WSAConnect function
// Parameters:	See MSDN WSAConnect parameters
// Return Value: See MSDN WSAConnect return value
//*************************************************************************
int WSAAPI WSAConnectReplacement(SOCKET s, const struct sockaddr FAR * name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS)
{
	const int numParams = 7;
	char *functionName = "WSAConnect";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &name, &namelen, &lpCallerData, &lpCalleeData, &lpSQOS, &lpGQOS };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, PointerType, PointerType, PointerType, PointerType };
	
	if (realWSAConnect == NULL)
		realWSAConnect = (WSAConnectFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAConnect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAConnectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAEventSelectReplacement
// Description: See MSDN WSAEventSelect function
// Parameters:	See MSDN WSAEventSelect parameters
// Return Value: See MSDN WSAEventSelect return value
//*************************************************************************
int WSAAPI WSAEventSelectReplacement(SOCKET s, WSAEVENT hEventObject, long lNetworkEvents)
{
	const int numParams = 3;
	char *functionName = "WSAEventSelect";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &hEventObject, &lNetworkEvents };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType };
	
	if (realWSAEventSelect == NULL)
		realWSAEventSelect = (WSAEventSelectFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAEventSelect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAEventSelectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAIoctlReplacement
// Description: See MSDN WSAIoctl function
// Parameters:	See MSDN WSAIoctl parameters
// Return Value: See MSDN WSAIoctl return value
//*************************************************************************
int WSAAPI WSAIoctlReplacement(SOCKET s, DWORD dwIoControlCode, LPVOID lpvInBuffer, DWORD cbInBuffer, LPVOID lpvOutBuffer, DWORD cbOutBuffer, LPDWORD lpcbBytesReturned, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 9;
	char *functionName = "WSAIoctl";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &dwIoControlCode, &lpvInBuffer, &cbInBuffer, &lpvOutBuffer, &cbOutBuffer, &lpcbBytesReturned, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType, PointerType, PointerType };
	
	if (realWSAIoctl == NULL)
		realWSAIoctl = (WSAIoctlFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAIoctl != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAIoctlCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSARecvReplacement
// Description: See MSDN WSARecv function
// Parameters:	See MSDN WSARecv parameters
// Return Value: See MSDN WSARecv return value
//*************************************************************************
int WSAAPI WSARecvReplacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 7;
	char *functionName = "WSARecv";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &lpBuffers, &dwBufferCount, &lpNumberOfBytesRecvd, &lpFlags, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType, PointerType, PointerType };
	
	if (realWSARecv == NULL)
		realWSARecv = (WSARecvFunction)library->GetOriginalFunction(functionName);
		
	if (realWSARecv != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSARecvCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSASendReplacement
// Description: See MSDN WSASend function
// Parameters:	See MSDN WSASend parameters
// Return Value: See MSDN WSASend return value
//*************************************************************************
int WSAAPI WSASendReplacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, LPOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 7;
	char *functionName = "WSASend";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &lpBuffers, &dwBufferCount, &lpNumberOfBytesSent, &dwFlags, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType, PointerType };
	
	if (realWSASend == NULL)
		realWSASend = (WSASendFunction)library->GetOriginalFunction(functionName);
		
	if (realWSASend != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSASendCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSASocketAReplacement
// Description: See MSDN WSASocketA function
// Parameters:	See MSDN WSASocketA parameters
// Return Value: See MSDN WSASocketA return value
//*************************************************************************
SOCKET WSAAPI WSASocketAReplacement(int af, int type, int protocol, LPWSAPROTOCOL_INFOA lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	const int numParams = 6;
	char *functionName = "WSASocketA";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &af, &type, &protocol, &lpProtocolInfo, &g, &dwFlags };
	ParameterType paramTypes[numParams] = { IntegerType, IntegerType, IntegerType, PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realWSASocketA == NULL)
		realWSASocketA = (WSASocketAFunction)library->GetOriginalFunction(functionName);
		
	if (realWSASocketA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SOCKET returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSASocketACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SOCKET", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSASocketWReplacement
// Description: See MSDN WSASocketW function
// Parameters:	See MSDN WSASocketW parameters
// Return Value: See MSDN WSASocketW return value
//*************************************************************************
SOCKET WSAAPI WSASocketWReplacement(int af, int type, int protocol, LPWSAPROTOCOL_INFOW lpProtocolInfo, GROUP g, DWORD dwFlags)
{
	const int numParams = 6;
	char *functionName = "WSASocketW";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &af, &type, &protocol, &lpProtocolInfo, &g, &dwFlags };
	ParameterType paramTypes[numParams] = { IntegerType, IntegerType, IntegerType, PointerType, UnsignedLongType, UnsignedLongType };
	
	if (realWSASocketW == NULL)
		realWSASocketW = (WSASocketWFunction)library->GetOriginalFunction(functionName);
		
	if (realWSASocketW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SOCKET returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSASocketWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SOCKET", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAStartupReplacement
// Description: See MSDN WSAStartup function
// Parameters:	See MSDN WSAStartup parameters
// Return Value: See MSDN WSAStartup return value
//*************************************************************************
int WSAAPI WSAStartupReplacement(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	const int numParams = 2;
	char *functionName = "WSAStartup";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &wVersionRequested, &lpWSAData };
	ParameterType paramTypes[numParams] = { IntegerType, PointerType };
	
	if (realWSAStartup == NULL)
		realWSAStartup = (WSAStartupFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAStartup != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAStartupCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		recvfromReplacement
// Description: See MSDN recvfrom function
// Parameters:	See MSDN recvfrom parameters
// Return Value: See MSDN recvfrom return value
//*************************************************************************
int WSAAPI recvfromReplacement(SOCKET s, char FAR * buf, int len, int flags, struct sockaddr FAR * from, int FAR * fromlen)
{
	const int numParams = 6;
	char *functionName = "recvfrom";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &buf, &len, &flags, &from, &fromlen };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, IntegerType, PointerType, PointerType };
	
	if (realrecvfrom == NULL)
		realrecvfrom = (recvfromFunction)library->GetOriginalFunction(functionName);
		
	if (realrecvfrom != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, recvfromCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		sendtoReplacement
// Description: See MSDN sendto function
// Parameters:	See MSDN sendto parameters
// Return Value: See MSDN sendto return value
//*************************************************************************
int WSAAPI sendtoReplacement(SOCKET s, const char FAR * buf, int len, int flags, const struct sockaddr FAR * to, int tolen)
{
	const int numParams = 6;
	char *functionName = "sendto";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &buf, &len, &flags, &to, &tolen };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, IntegerType, PointerType, IntegerType };
	
	if (realsendto == NULL)
		realsendto = (sendtoFunction)library->GetOriginalFunction(functionName);
		
	if (realsendto != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, sendtoCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		setsockoptReplacement
// Description: See MSDN setsockopt function
// Parameters:	See MSDN setsockopt parameters
// Return Value: See MSDN setsockopt return value
//*************************************************************************
int WSAAPI setsockoptReplacement(SOCKET s, int level, int optname, const char FAR * optval, int optlen)
{
	const int numParams = 5;
	char *functionName = "setsockopt";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &level, &optname, &optval, &optlen };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, IntegerType, PointerType, IntegerType };
	
	if (realsetsockopt == NULL)
		realsetsockopt = (setsockoptFunction)library->GetOriginalFunction(functionName);
		
	if (realsetsockopt != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, setsockoptCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		getsockoptReplacement
// Description: See MSDN getsockopt function
// Parameters:	See MSDN getsockopt parameters
// Return Value: See MSDN getsockopt return value
//*************************************************************************
int WSAAPI getsockoptReplacement(SOCKET s, int level, int optname, char FAR * optval, int FAR * optlen)
{
	const int numParams = 5;
	char *functionName = "getsockopt";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &level, &optname, &optval, &optlen };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, IntegerType, PointerType, PointerType };
	
	if (realgetsockopt == NULL)
		realgetsockopt = (getsockoptFunction)library->GetOriginalFunction(functionName);
		
	if (realgetsockopt != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, getsockoptCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		listenReplacement
// Description: See MSDN listen function
// Parameters:	See MSDN listen parameters
// Return Value: See MSDN listen return value
//*************************************************************************
int WSAAPI listenReplacement(SOCKET s, int backlog)
{
	const int numParams = 2;
	char *functionName = "listen";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &backlog };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (reallisten == NULL)
		reallisten = (listenFunction)library->GetOriginalFunction(functionName);
		
	if (reallisten != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, listenCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		gethostbyaddrReplacement
// Description: See MSDN gethostbyaddr function
// Parameters:	See MSDN gethostbyaddr parameters
// Return Value: See MSDN gethostbyaddr return value
//*************************************************************************
struct hostent FAR * WSAAPI gethostbyaddrReplacement(const char FAR * addr, int len, int type)
{
	const int numParams = 3;
	char *functionName = "gethostbyaddr";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &addr, &len, &type };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, IntegerType };
	
	if (realgethostbyaddr == NULL)
		realgethostbyaddr = (gethostbyaddrFunction)library->GetOriginalFunction(functionName);
		
	if (realgethostbyaddr != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		struct hostent FAR * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, gethostbyaddrCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "struct hostent FAR *", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		gethostbynameReplacement
// Description: See MSDN gethostbyname function
// Parameters:	See MSDN gethostbyname parameters
// Return Value: See MSDN gethostbyname return value
//*************************************************************************
struct hostent FAR * WSAAPI gethostbynameReplacement(const char FAR * name)
{
	const int numParams = 1;
	char *functionName = "gethostbyname";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &name };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realgethostbyname == NULL)
		realgethostbyname = (gethostbynameFunction)library->GetOriginalFunction(functionName);
		
	if (realgethostbyname != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		struct hostent FAR * returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, gethostbynameCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "struct hostent FAR *", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		acceptReplacement
// Description: See MSDN accept function
// Parameters:	See MSDN accept parameters
// Return Value: See MSDN accept return value
//*************************************************************************
SOCKET WSAAPI acceptReplacement(SOCKET s, struct sockaddr FAR * addr, int FAR * addrlen)
{
	const int numParams = 3;
	char *functionName = "accept";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &addr, &addrlen };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realaccept == NULL)
		realaccept = (acceptFunction)library->GetOriginalFunction(functionName);
		
	if (realaccept != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SOCKET returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, acceptCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SOCKET", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSARecvFromReplacement
// Description: See MSDN WSARecvFrom function
// Parameters:	See MSDN WSARecvFrom parameters
// Return Value: See MSDN WSARecvFrom return value
//*************************************************************************
int WSAAPI WSARecvFromReplacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesRecvd, LPDWORD lpFlags, struct sockaddr FAR * lpFrom, LPINT lpFromlen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 9;
	char *functionName = "WSARecvFrom";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &lpBuffers, &dwBufferCount, &lpNumberOfBytesRecvd, &lpFlags, &lpFrom, &lpFromlen, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, PointerType, PointerType, PointerType, PointerType, PointerType };
	
	if (realWSARecvFrom == NULL)
		realWSARecvFrom = (WSARecvFromFunction)library->GetOriginalFunction(functionName);
		
	if (realWSARecvFrom != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSARecvFromCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSASendDisconnectReplacement
// Description: See MSDN WSASendDisconnect function
// Parameters:	See MSDN WSASendDisconnect parameters
// Return Value: See MSDN WSASendDisconnect return value
//*************************************************************************
int WSAAPI WSASendDisconnectReplacement(SOCKET s, LPWSABUF lpOutboundDisconnectData)
{
	const int numParams = 2;
	char *functionName = "WSASendDisconnect";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &lpOutboundDisconnectData };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realWSASendDisconnect == NULL)
		realWSASendDisconnect = (WSASendDisconnectFunction)library->GetOriginalFunction(functionName);
		
	if (realWSASendDisconnect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSASendDisconnectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAJoinLeafReplacement
// Description: See MSDN WSAJoinLeaf function
// Parameters:	See MSDN WSAJoinLeaf parameters
// Return Value: See MSDN WSAJoinLeaf return value
//*************************************************************************
SOCKET WSAAPI WSAJoinLeafReplacement(SOCKET s, const struct sockaddr FAR * name, int namelen, LPWSABUF lpCallerData, LPWSABUF lpCalleeData, LPQOS lpSQOS, LPQOS lpGQOS, DWORD dwFlags)
{
	const int numParams = 8;
	char *functionName = "WSAJoinLeaf";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &name, &namelen, &lpCallerData, &lpCalleeData, &lpSQOS, &lpGQOS, &dwFlags };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, PointerType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realWSAJoinLeaf == NULL)
		realWSAJoinLeaf = (WSAJoinLeafFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAJoinLeaf != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SOCKET returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAJoinLeafCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SOCKET", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSARecvDisconnectReplacement
// Description: See MSDN WSARecvDisconnect function
// Parameters:	See MSDN WSARecvDisconnect parameters
// Return Value: See MSDN WSARecvDisconnect return value
//*************************************************************************
int WSAAPI WSARecvDisconnectReplacement(SOCKET s, LPWSABUF lpInboundDisconnectData)
{
	const int numParams = 2;
	char *functionName = "WSARecvDisconnect";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &lpInboundDisconnectData };
	ParameterType paramTypes[numParams] = { PointerType, PointerType };
	
	if (realWSARecvDisconnect == NULL)
		realWSARecvDisconnect = (WSARecvDisconnectFunction)library->GetOriginalFunction(functionName);
		
	if (realWSARecvDisconnect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSARecvDisconnectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAProviderConfigChangeReplacement
// Description: See MSDN WSAProviderConfigChange function
// Parameters:	See MSDN WSAProviderConfigChange parameters
// Return Value: See MSDN WSAProviderConfigChange return value
//*************************************************************************
INT WSAAPI WSAProviderConfigChangeReplacement(LPHANDLE lpNotificationHandle, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 3;
	char *functionName = "WSAProviderConfigChange";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &lpNotificationHandle, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType };
	
	if (realWSAProviderConfigChange == NULL)
		realWSAProviderConfigChange = (WSAProviderConfigChangeFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAProviderConfigChange != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		INT returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAProviderConfigChangeCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "INT", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSADuplicateSocketAReplacement
// Description: See MSDN WSADuplicateSocketA function
// Parameters:	See MSDN WSADuplicateSocketA parameters
// Return Value: See MSDN WSADuplicateSocketA return value
//*************************************************************************
int WSAAPI WSADuplicateSocketAReplacement(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFOA lpProtocolInfo)
{
	const int numParams = 3;
	char *functionName = "WSADuplicateSocketA";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &dwProcessId, &lpProtocolInfo };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType };
	
	if (realWSADuplicateSocketA == NULL)
		realWSADuplicateSocketA = (WSADuplicateSocketAFunction)library->GetOriginalFunction(functionName);
		
	if (realWSADuplicateSocketA != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSADuplicateSocketACaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSADuplicateSocketWReplacement
// Description: See MSDN WSADuplicateSocketW function
// Parameters:	See MSDN WSADuplicateSocketW parameters
// Return Value: See MSDN WSADuplicateSocketW return value
//*************************************************************************
int WSAAPI WSADuplicateSocketWReplacement(SOCKET s, DWORD dwProcessId, LPWSAPROTOCOL_INFOW lpProtocolInfo)
{
	const int numParams = 3;
	char *functionName = "WSADuplicateSocketW";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &dwProcessId, &lpProtocolInfo };
	ParameterType paramTypes[numParams] = { PointerType, UnsignedLongType, PointerType };
	
	if (realWSADuplicateSocketW == NULL)
		realWSADuplicateSocketW = (WSADuplicateSocketWFunction)library->GetOriginalFunction(functionName);
		
	if (realWSADuplicateSocketW != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSADuplicateSocketWCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAAsyncSelectReplacement
// Description: See MSDN WSAAsyncSelect function
// Parameters:	See MSDN WSAAsyncSelect parameters
// Return Value: See MSDN WSAAsyncSelect return value
//*************************************************************************
int WSAAPI WSAAsyncSelectReplacement(SOCKET s, HWND hWnd, u_int wMsg, long lEvent)
{
	const int numParams = 4;
	char *functionName = "WSAAsyncSelect";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &hWnd, &wMsg, &lEvent };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, IntegerType, IntegerType };
	
	if (realWSAAsyncSelect == NULL)
		realWSAAsyncSelect = (WSAAsyncSelectFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAAsyncSelect != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAAsyncSelectCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSACancelAsyncRequestReplacement
// Description: See MSDN WSACancelAsyncRequest function
// Parameters:	See MSDN WSACancelAsyncRequest parameters
// Return Value: See MSDN WSACancelAsyncRequest return value
//*************************************************************************
int WSAAPI WSACancelAsyncRequestReplacement(HANDLE hAsyncTaskHandle)
{
	const int numParams = 1;
	char *functionName = "WSACancelAsyncRequest";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &hAsyncTaskHandle };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realWSACancelAsyncRequest == NULL)
		realWSACancelAsyncRequest = (WSACancelAsyncRequestFunction)library->GetOriginalFunction(functionName);
		
	if (realWSACancelAsyncRequest != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSACancelAsyncRequestCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSACleanupReplacement
// Description: See MSDN WSACleanup function
// Parameters:	See MSDN WSACleanup parameters
// Return Value: See MSDN WSACleanup return value
//*************************************************************************
int WSAAPI WSACleanupReplacement()
{
	const int numParams = 0;
	char *functionName = "WSACleanup";
	char *categoryName = "NETWORK";
	
	void **params = NULL;
	ParameterType *paramTypes = NULL;
	
	if (realWSACleanup == NULL)
		realWSACleanup = (WSACleanupFunction)library->GetOriginalFunction(functionName);
		
	if (realWSACleanup != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSACleanupCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAAcceptReplacement
// Description: See MSDN WSAAccept function
// Parameters:	See MSDN WSAAccept parameters
// Return Value: See MSDN WSAAccept return value
//*************************************************************************
SOCKET WSAAPI WSAAcceptReplacement(SOCKET s, struct sockaddr FAR * addr, LPINT addrlen, LPCONDITIONPROC lpfnCondition, DWORD dwCallbackData)
{
	const int numParams = 5;
	char *functionName = "WSAAccept";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &addr, &addrlen, &lpfnCondition, &dwCallbackData };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, PointerType, PointerType, UnsignedLongType };
	
	if (realWSAAccept == NULL)
		realWSAAccept = (WSAAcceptFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAAccept != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		SOCKET returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAAcceptCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "SOCKET", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAAsyncGetHostByAddrReplacement
// Description: See MSDN WSAAsyncGetHostByAddr function
// Parameters:	See MSDN WSAAsyncGetHostByAddr parameters
// Return Value: See MSDN WSAAsyncGetHostByAddr return value
//*************************************************************************
HANDLE WSAAPI WSAAsyncGetHostByAddrReplacement(HWND hWnd, u_int wMsg, const char FAR * addr, int len, int type, char FAR * buf, int buflen)
{
	const int numParams = 7;
	char *functionName = "WSAAsyncGetHostByAddr";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &hWnd, &wMsg, &addr, &len, &type, &buf, &buflen };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, PointerType, IntegerType, IntegerType, PointerType, IntegerType };
	
	if (realWSAAsyncGetHostByAddr == NULL)
		realWSAAsyncGetHostByAddr = (WSAAsyncGetHostByAddrFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAAsyncGetHostByAddr != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAAsyncGetHostByAddrCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAAsyncGetHostByNameReplacement
// Description: See MSDN WSAAsyncGetHostByName function
// Parameters:	See MSDN WSAAsyncGetHostByName parameters
// Return Value: See MSDN WSAAsyncGetHostByName return value
//*************************************************************************
HANDLE WSAAPI WSAAsyncGetHostByNameReplacement(HWND hWnd, u_int wMsg, const char FAR * name, char FAR * buf, int buflen)
{
	const int numParams = 5;
	char *functionName = "WSAAsyncGetHostByName";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &hWnd, &wMsg, &name, &buf, &buflen };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, PointerType, PointerType, IntegerType };
	
	if (realWSAAsyncGetHostByName == NULL)
		realWSAAsyncGetHostByName = (WSAAsyncGetHostByNameFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAAsyncGetHostByName != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAAsyncGetHostByNameCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAAsyncGetServByNameReplacement
// Description: See MSDN WSAAsyncGetServByName function
// Parameters:	See MSDN WSAAsyncGetServByName parameters
// Return Value: See MSDN WSAAsyncGetServByName return value
//*************************************************************************
HANDLE WSAAPI WSAAsyncGetServByNameReplacement(HWND hWnd, u_int wMsg, const char FAR * name, const char FAR * proto, char FAR * buf, int buflen)
{
	const int numParams = 6;
	char *functionName = "WSAAsyncGetServByName";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &hWnd, &wMsg, &name, &proto, &buf, &buflen };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, PointerType, PointerType, PointerType, IntegerType };
	
	if (realWSAAsyncGetServByName == NULL)
		realWSAAsyncGetServByName = (WSAAsyncGetServByNameFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAAsyncGetServByName != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAAsyncGetServByNameCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSAAsyncGetServByPortReplacement
// Description: See MSDN WSAAsyncGetServByPort function
// Parameters:	See MSDN WSAAsyncGetServByPort parameters
// Return Value: See MSDN WSAAsyncGetServByPort return value
//*************************************************************************
HANDLE WSAAPI WSAAsyncGetServByPortReplacement(HWND hWnd, u_int wMsg, int port, const char FAR * proto, char FAR * buf, int buflen)
{
	const int numParams = 6;
	char *functionName = "WSAAsyncGetServByPort";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &hWnd, &wMsg, &port, &proto, &buf, &buflen };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType, IntegerType, PointerType, PointerType, IntegerType };
	
	if (realWSAAsyncGetServByPort == NULL)
		realWSAAsyncGetServByPort = (WSAAsyncGetServByPortFunction)library->GetOriginalFunction(functionName);
		
	if (realWSAAsyncGetServByPort != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		HANDLE returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSAAsyncGetServByPortCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "HANDLE", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		shutdownReplacement
// Description: See MSDN shutdown function
// Parameters:	See MSDN shutdown parameters
// Return Value: See MSDN shutdown return value
//*************************************************************************
int WSAAPI shutdownReplacement(SOCKET s, int how)
{
	const int numParams = 2;
	char *functionName = "shutdown";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &how };
	ParameterType paramTypes[numParams] = { PointerType, IntegerType };
	
	if (realshutdown == NULL)
		realshutdown = (shutdownFunction)library->GetOriginalFunction(functionName);
		
	if (realshutdown != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, shutdownCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		WSASendToReplacement
// Description: See MSDN WSASendTo function
// Parameters:	See MSDN WSASendTo parameters
// Return Value: See MSDN WSASendTo return value
//*************************************************************************
int WSAAPI WSASendToReplacement(SOCKET s, LPWSABUF lpBuffers, DWORD dwBufferCount, LPDWORD lpNumberOfBytesSent, DWORD dwFlags, const struct sockaddr FAR * lpTo, int iTolen, LPWSAOVERLAPPED lpOverlapped, LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine)
{
	const int numParams = 9;
	char *functionName = "WSASendTo";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s, &lpBuffers, &dwBufferCount, &lpNumberOfBytesSent, &dwFlags, &lpTo, &iTolen, &lpOverlapped, &lpCompletionRoutine };
	ParameterType paramTypes[numParams] = { PointerType, PointerType, UnsignedLongType, PointerType, UnsignedLongType, PointerType, IntegerType, PointerType, PointerType };
	
	if (realWSASendTo == NULL)
		realWSASendTo = (WSASendToFunction)library->GetOriginalFunction(functionName);
		
	if (realWSASendTo != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, WSASendToCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// Method:		closesocketReplacement
// Description: See MSDN closesocket function
// Parameters:	See MSDN closesocket parameters
// Return Value: See MSDN closesocket return value
//*************************************************************************
int WSAAPI closesocketReplacement(SOCKET s)
{
	const int numParams = 1;
	char *functionName = "closesocket";
	char *categoryName = "NETWORK";
	
	void *params[numParams] = { &s };
	ParameterType paramTypes[numParams] = { PointerType };
	
	if (realclosesocket == NULL)
		realclosesocket = (closesocketFunction)library->GetOriginalFunction(functionName);
		
	if (realclosesocket != NULL)
	{
		DWORD errorCode, tempReturnValue = 0;
		int returnValue;
		
		
		if (library->RunStandardTestsAndGetResults(logSender, closesocketCaller, categoryName, functionName, numParams, params, paramTypes, &tempReturnValue, "int", &errorCode, 0, true))
		{
			
		}
		else
		{
			
		}
		
		memcpy(&returnValue, &tempReturnValue, sizeof(DWORD));
		WSASetLastError(errorCode);
		return returnValue;
	}
	
	return 0;
}
//*************************************************************************
// END OF REPLACEMENT FUNCTIONS
//*************************************************************************
