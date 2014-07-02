#include "common.h"


BOOL My_select()
{
	int nfds=NULL;
	fd_set FAR * readfds=NULL;
	fd_set FAR * writefds=NULL;
	fd_set FAR * exceptfds=NULL;
	const struct timeval FAR * timeout=NULL;
	int returnVal_Real = NULL;
	int returnVal_Intercepted = NULL;

	DWORD error_Real = 0;
	DWORD error_Intercepted = 0;
	__try{
	disableInterception();
	returnVal_Real = select (nfds,readfds,writefds,exceptfds,timeout);
	error_Real = GetLastError();
	enableInterception();
	returnVal_Intercepted = select (nfds,readfds,writefds,exceptfds,timeout);
	error_Intercepted = GetLastError();
	}__except(puts("in filter"), 1){puts("exception caught");}
	return ((returnVal_Real == returnVal_Intercepted) && (error_Real == error_Intercepted));
}
