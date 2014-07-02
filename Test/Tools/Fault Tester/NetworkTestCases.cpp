#include "stdafx.h"

bool Disconnected()
{	
	bool bSuccess = TRUE;

	if (!My_WSAConnect(-1, 10050))
		bSuccess = FALSE;

	if (!My_connect(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSARecv(-1, 10050))
		bSuccess = FALSE;

	if (!My_recv(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSASend(-1, 10050))
		bSuccess = FALSE;

	if (!My_send(-1, 10050))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}

bool NetworkNotInstalled()
{	
	bool bSuccess = TRUE;

	if (!My_WSAStartup(10091, 0))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}

bool WrongWinsockVersion()
{	
	bool bSuccess = TRUE;

	if (!My_WSAStartup(10091, 0))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}

bool WinsockTaskLimitReached()
{	
	bool bSuccess = TRUE;

	if (!My_WSAStartup(10067, 0))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}

bool AllPortsUnavailable()
{	
	bool bSuccess = TRUE;

	if (!My_WSAConnect(-1, 10048))
		bSuccess = FALSE;

	if (!My_connect(-1, 10048))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}

bool NetworkIsDown()
{	
	bool bSuccess = TRUE;


	if (!My_WSAConnect(-1, 10050))
		bSuccess = FALSE;

	if (!My_connect(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSARecv(-1, 10050))
		bSuccess = FALSE;

	if (!My_recv(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSASend(-1, 10050))
		bSuccess = FALSE;

	if (!My_send(-1, 10050))
		bSuccess = FALSE;

	if (!My_ioctlsocket(-1, 10050))
		bSuccess = FALSE;

	if (!My_select(-1, 10050))
		bSuccess = FALSE;

	if (!My_socket(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSAEventSelect(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSAIoctl(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSASocketA(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSASocketW(-1, 10050))
		bSuccess = FALSE;

	if (!My_WSAStartup(-1, 10050))
		bSuccess = FALSE;

// return true if we make it through all function calls...
return bSuccess;
}

