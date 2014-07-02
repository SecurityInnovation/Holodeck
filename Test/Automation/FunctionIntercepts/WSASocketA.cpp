#include "common.h"


BOOL My_WSASocketA()
{
	int af=NULL;
	int type=NULL;
	int protocol=NULL;
	LPWSAPROTOCOL_INFOA lpProtocolInfo=NULL;
	GROUP g=NULL;
	DWORD dwFlags=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = WSASocketA (af,type,protocol,lpProtocolInfo,g,dwFlags);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = WSASocketA (af,type,protocol,lpProtocolInfo,g,dwFlags);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
