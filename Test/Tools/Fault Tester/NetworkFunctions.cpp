#include "stdafx.h"


extern BOOL BOOL_VAL;
extern DWORD INT_VAL;
extern SECURITY_ATTRIBUTES GENERIC_SECURITY;
extern FILE_SEGMENT_ELEMENT GENERIC_SEGMENT_ARRAY[];
extern HANDLE GENERIC_HANDLE;

SOCKET s = 0;
sockaddr SOCK_ADDR;
WORD WORD_VAL = 0;
WSAPROTOCOL_INFO GENERIC_PROTOCOL_INFO;
WSAPROTOCOL_INFOW GENERIC_PROTOCOL_INFOW;
GROUP GENERIC_GROUP = 0;

void CALLBACK GENERIC_NETWORK_ROUTINE(
  IN DWORD dwError, 
  IN DWORD cbTransferred, 
  IN LPWSAOVERLAPPED lpOverlapped, 
  IN DWORD dwFlags
)
{
	return;
}


bool My_WSAConnect(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = WSAConnect(s, &SOCK_ADDR, INT_VAL, (LPWSABUF)&INT_VAL, (LPWSABUF)&INT_VAL, (LPQOS)&INT_VAL, (LPQOS)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSAConnect PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSAConnect FAILED!\n");
	return FALSE;
}

bool My_connect(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = connect(s, &SOCK_ADDR, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("connect PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("connect FAILED!\n");
	return FALSE;
}

bool My_WSARecv(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = WSARecv(s, (LPWSABUF)&INT_VAL, INT_VAL, &INT_VAL, &INT_VAL, (LPWSAOVERLAPPED)&STRING_VAL, GENERIC_NETWORK_ROUTINE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSARecv PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSARecv FAILED!\n");
	return FALSE;
}

bool My_recv(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = recv(s, STRING_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("recv PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("recv FAILED!\n");
	return FALSE;
}

bool My_WSASend(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = WSASend(s, (LPWSABUF)&INT_VAL, INT_VAL, &INT_VAL, INT_VAL, (LPWSAOVERLAPPED)&STRING_VAL, GENERIC_NETWORK_ROUTINE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSASend PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSASend FAILED!\n");
	return FALSE;
}

bool My_send(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = send(s, STRING_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("send PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("send FAILED!\n");
	return FALSE;
}

bool My_WSAStartup(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = WSAStartup(WORD_VAL, (LPWSADATA)&STRING_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSAStartup PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSAStartup FAILED!\n");
	return FALSE;
}

bool My_ioctlsocket(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = ioctlsocket(s, (long)INT_VAL, (u_long*)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("ioctlsocket PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("ioctlsocket FAILED!\n");
	return FALSE;
}

bool My_select(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = select(INT_VAL, (fd_set*)&INT_VAL, (fd_set*)&INT_VAL, (fd_set*)&INT_VAL, (timeval*)&INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("select PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("select FAILED!\n");
	return FALSE;
}

bool My_socket(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = (DWORD)socket(INT_VAL, INT_VAL, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("socket PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("socket FAILED!\n");
	return FALSE;
}

bool My_WSAEventSelect(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = WSAEventSelect(s, GENERIC_HANDLE, (long)INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSAEventSelect PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSAEventSelect FAILED!\n");
	return FALSE;
}

bool My_WSAIoctl(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = WSAIoctl(s, INT_VAL, &INT_VAL, INT_VAL, &INT_VAL, INT_VAL, &INT_VAL, (LPWSAOVERLAPPED)&INT_VAL, GENERIC_NETWORK_ROUTINE);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSAIoctl PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSAIoctl FAILED!\n");
	return FALSE;
}

bool My_WSASocketA(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = (DWORD)WSASocketA(INT_VAL, INT_VAL, INT_VAL, &GENERIC_PROTOCOL_INFO, GENERIC_GROUP, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSASocketA PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSASocketA FAILED!\n");
	return FALSE;
}

bool My_WSASocketW(DWORD ret, DWORD error)
{
	DWORD dwRet = -20;

	////
	dwRet = (DWORD)WSASocketW(INT_VAL, INT_VAL, INT_VAL, &GENERIC_PROTOCOL_INFOW, GENERIC_GROUP, INT_VAL);

	if (dwRet != ret)
		goto FAILED;

	if (!checkErrorCode(error))
		goto FAILED;

	uninterceptedPrint("WSASocketW PASSED!\n");

return TRUE;

FAILED:
	uninterceptedPrint("WSASocketW FAILED!\n");
	return FALSE;
}