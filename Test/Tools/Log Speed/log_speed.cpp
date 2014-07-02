#include <windows.h>
#include <stdio.h>

// this program simply calls HeapAlloc and HeapFree several times...

void logFunction(char pszName[ ], FILE * fHandle)
{
	LPSYSTEMTIME pSystemTime = NULL;

	char pszDate[100];
	char pszTime[100];
	char pszTemp[100];

	// format function call timer and log...
	pSystemTime = new SYSTEMTIME;
	GetLocalTime(pSystemTime);
	
	ltoa(pSystemTime->wMonth, pszDate, 10);
	
	ltoa(pSystemTime->wDay, pszTemp, 10);
	strcat(pszDate, "/");
	strcat(pszDate, pszTemp);

	ltoa(pSystemTime->wYear, pszTemp, 10);
	strcat(pszDate, "/");
	strcat(pszDate, pszTemp);

	ltoa(pSystemTime->wHour, pszTime, 10);
	
	ltoa(pSystemTime->wMinute, pszTemp, 10);
	strcat(pszTime, ":");
	strcat(pszTime, pszTemp);

	ltoa(pSystemTime->wSecond, pszTemp, 10);
	strcat(pszTime, ":");
	strcat(pszTime, pszTemp);

	ltoa(pSystemTime->wMilliseconds, pszTemp, 10);
	strcat(pszTime, ":");
	strcat(pszTime, pszTemp);

	fprintf(fHandle, "%s %s %s\n", pszDate, pszTime, pszName);
}

int main()
{
	int nTimes = 1000;
	int * iAlloc = NULL;

	CHAR pszCount[20];
	
	// Get handle to the heap...
	HANDLE hHeap = GetProcessHeap( );

	// Open a file handle...
	FILE * fHandle;

	system("del log.txt /q");
	system("type nul > log.txt");

	fHandle = fopen("log.txt", "w");
	
	for (int iCount = 0; iCount <= nTimes; iCount++)
	{
		iAlloc = (int *)HeapAlloc(hHeap, NULL, sizeof(int));
		CreateFile(pszCount, 0, 0, 0, 0, 0, 0);

		ltoa(iCount, pszCount, 10);
		strcat(pszCount, "VAL");		
		logFunction(pszCount, fHandle);
		
		HeapFree(hHeap, NULL, iAlloc);
		logFunction("HeapFree(hHeap, NULL, sizeof(int))", fHandle);
	}

	getchar();
	fclose(fHandle);

	return 0;
}