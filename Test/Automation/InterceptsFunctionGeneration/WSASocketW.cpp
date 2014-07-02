#include "common.h"


BOOL My_WSASocketW()
{
	int af=NULL;
	int type=NULL;
	int protocol=NULL;
	LPWSAPROTOCOL_INFOW lpProtocolInfo=NULL;
	GROUP g=NULL;
	DWORD dwFlags=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	disableInterception();
	returnVal_Real = WSASocketW (af,type,protocol,lpProtocolInfo,g,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSASocketW (af,type,protocol,lpProtocolInfo,g,dwFlags);
	error_Intercepted = GetLastError();
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
