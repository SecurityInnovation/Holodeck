// fault_tester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>

extern FARPROC disableInterception = NULL;
extern FARPROC enableInterception = NULL;

BOOL bErrorVerbose = false;

void getInterceptionDisablers()
{
	HMODULE hHeatHandle;

	// Get the handle to the HEAT module...
	hHeatHandle = GetModuleHandle("HEATDLL.DLL");

	if (hHeatHandle == NULL)
		printf("ERROR Loading Module HEATDLL.DLL!\n");

	// Get function pointers...
	disableInterception = GetProcAddress(hHeatHandle, "disableInterceptionInCurrentThread");
	enableInterception = GetProcAddress(hHeatHandle, "enableInterceptionInCurrentThread");

	return;
}

void uninterceptedPrint(char * pszString)
{
	if (disableInterception != NULL)
		disableInterception();

	printf("%s", pszString);

	if (enableInterception != NULL)
		enableInterception();

	return;
}

void uninterceptedGetChar()
{

	if (disableInterception != NULL)
		disableInterception();

	getchar();

	if (enableInterception != NULL)
		enableInterception();

	return;
}

void promptForStarting()
{
	uninterceptedPrint("\n-->PRESS ENTER<--\n");
	
	_flushall();
	uninterceptedGetChar();
}

// checks to ensure error code is correct...
bool checkErrorCode(DWORD iErrorCode)
{
	if (disableInterception != NULL)
			disableInterception();

	DWORD dwLocalError = 20;
	dwLocalError = GetLastError();

	if (bErrorVerbose)	
		printf("ERROR CODE: %d --", dwLocalError);

	if (enableInterception != NULL)
			enableInterception();

	if (dwLocalError == iErrorCode)
		return TRUE;
	else
		return FALSE;
}

// checks to ensure return value is correct...
bool checkReturnValue(DWORD dwReturnExpected, DWORD dwReturnActual)
{
	if (dwReturnExpected == dwReturnActual)
		return TRUE;
	else
		return FALSE;
}

int main(int argc, char* argv[])
{
	int iTestCase = 0;

	bErrorVerbose = true;

	char pszDecision[20];

	for (;;)
	{
		// Get the pointers to the functions for disabling and enabling interception...
		getInterceptionDisablers();

		uninterceptedPrint("=== HOLODECK FAULT TESTER 1.0 by TERRY LENTZ ===\n\n");
		uninterceptedPrint("*** FILE FAULTS ***\n");
		uninterceptedPrint("0 - File Not Found\t\t10 - File Cannot Be Created\n");
		uninterceptedPrint("1 - File Cannot Be Accessed\t11 - Filename Too Long\n");
		uninterceptedPrint("2 - Access Denied\t\t12 - Directory Read Only\n");
		uninterceptedPrint("3 - Write Protected\t\t13 - Path Not Found\n");
		uninterceptedPrint("4 - Crc Data Error\t\t14 - Directory Cannot Be Removed\n");
		uninterceptedPrint("5 - Drive Cannot Seek Disk\t15 - Directory Not Empty\n");
		uninterceptedPrint("6 - File In Use\t\t\t16 - Path Invalid\n");
		uninterceptedPrint("7 - File Locked\t\t\t17 - Corrupt Structure\n");
		uninterceptedPrint("8 - Disk Full\t\t\n");
		uninterceptedPrint("9 - File Already Exists\t\t\n\n");
		uninterceptedPrint("*** MEMORY FAULTS ***\n");
		uninterceptedPrint("18 - Insufficient Memory\t\t21 - Invalid Address\n");
		uninterceptedPrint("19 - Invalid Access To Location\t\t22 - Paging File Too Small\n");
		uninterceptedPrint("20 - Segment Locked\t\t\n\n");
		uninterceptedPrint("*** NETWORK FAULTS ***\n");
		uninterceptedPrint("23 - Disconnected \t\t\t26 - Winsock Task Limit Reached\n");
		uninterceptedPrint("24 - Network Not Installed\t\t27 - All Ports Unavailable\n");
		uninterceptedPrint("25 - Wrong Winsock Version\t\t28 - Network is Down\n");

		
		uninterceptedPrint("\nPlease enter the test that you would like to run: ");

		if (disableInterception != NULL)
			disableInterception();
		
		scanf("%d", &iTestCase);

		if (enableInterception != NULL)
			enableInterception();
		
		uninterceptedPrint("\n");

		switch (iTestCase)
		{
			// FileNotFound
			case 0:
				uninterceptedPrint("=== TEST CASE: FileNotFound ===\n");
				
				if (FileNotFound())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");

				break;

			// FileCannotBeAccessed
			case 1:
				uninterceptedPrint("=== TEST CASE: FileCannotBeAccessed ===\n");

				if (FileCannotBeAccessed())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// AccessDenied
			case 2:
				uninterceptedPrint("=== TEST CASE: AccessDenied ===\n");

				if (AccessDenied())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// WriteProtected
			case 3:
				uninterceptedPrint("=== TEST CASE: WriteProtected ===\n");
				
				if (WriteProtected())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// CrcDataError
			case 4:
				uninterceptedPrint("=== TEST CASE: CrcDataError ===\n");

				if (CrcDataError())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// DriveCannotSeekDisk
			case 5:
				uninterceptedPrint("=== TEST CASE: DriveCannotSeekDisk ===\n");

				if (DriveCannotSeekDisk())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// FileInUse
			case 6:
				uninterceptedPrint("=== TEST CASE: FileInUse ===\n");

				if (FileInUse())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// FileLocked
			case 7:
				uninterceptedPrint("=== TEST CASE: FileLocked ===\n");

				if (FileLocked())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// DiskFull
			case 8:
				uninterceptedPrint("=== TEST CASE: DiskFull ===\n");

				if (DiskFull())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// FileAlreadyExists
			case 9:
				uninterceptedPrint("=== TEST CASE: FileAlreadyExists ===\n");

				if (FileAlreadyExists())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;
				
			// FileCannotBeCreated
			case 10:
				uninterceptedPrint("=== TEST CASE: FileCannotBeCreated ===\n");

				if (FileCannotBeCreated())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// FileNameTooLong
			case 11:
				uninterceptedPrint("=== TEST CASE: FileNameTooLong ===\n");

				if (FileNameTooLong())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// DirectoryReadOnly
			case 12:
				uninterceptedPrint("=== TEST CASE: DirectoryReadOnly ===\n");
				
				if (DirectoryReadOnly())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// PathNotFound
			case 13:
				uninterceptedPrint("=== TEST CASE: PathNotFound ===\n");

				if (PathNotFound())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// DirectoryCannotBeRemoved
			case 14:
				uninterceptedPrint("=== TEST CASE: DirectoryCannotBeRemoved ===\n");

				if (DirectoryCannotBeRemoved())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// DirectoryNotEmpty
			case 15:
				uninterceptedPrint("=== TEST CASE: DirectoryNotEmpty ===\n");

				if (DirectoryNotEmpty())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// PathInvalid
			case 16:
				uninterceptedPrint("=== TEST CASE: PathInvalid ===\n");

				if (PathInvalid())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// CorruptStructure
			case 17:
				uninterceptedPrint("=== TEST CASE: CorruptStructure ===\n");

				if (CorruptStructure())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// InsufficientMemory
			case 18:
				uninterceptedPrint("=== TEST CASE: InsufficientMemory ===\n");

				if (InsufficientMemory())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");
				
				
				break;

			// InvalidAccessToLocation
			case 19:
				uninterceptedPrint("=== TEST CASE: InvalidAccessToLocation ===\n");

				if (InvalidAccessToLocation())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");
				

				break;

			// SegmentLocked
			case 20:
				uninterceptedPrint("=== TEST CASE: SegmentLocked ===\n");

				if (SegmentLocked())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");
				

				break;

			// InvalidAddress
			case 21:
				uninterceptedPrint("=== TEST CASE: InvalidAddress ===\n");

				if (InvalidAddress())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");
				

				break;

			// PagingFileTooSmall
			case 22:
				uninterceptedPrint("=== TEST CASE: PagingFileTooSmall ===\n");

				if (PagingFileTooSmall())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");
				

				break;

			// Disconnected
			case 23:
				uninterceptedPrint("=== TEST CASE: Disconnected ===\n");

				if (Disconnected())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// NetworkNotInstalled
			case 24:
				uninterceptedPrint("=== TEST CASE: NetworkNotInstalled ===\n");

				if (NetworkNotInstalled())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// WrongWinsockVersion
			case 25:
				uninterceptedPrint("=== TEST CASE: WrongWinsockVersion ===\n");

				if (WrongWinsockVersion())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// WinsockTaskLimitReached
			case 26:
				uninterceptedPrint("=== TEST CASE: WinsockTaskLimitReached ===\n");

				if (WinsockTaskLimitReached())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// AllPortsUnavailable
			case 27:
				uninterceptedPrint("=== TEST CASE: AllPortsUnavailable ===\n");

				if (AllPortsUnavailable())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;

			// NetworkIsDown
			case 28:
				uninterceptedPrint("=== TEST CASE: NetworkIsDown ===\n");

				if (NetworkIsDown())
					uninterceptedPrint("\nTEST Passed!\n");
				else
					uninterceptedPrint("\nTEST Failed!\n");


				break;
		}	

		uninterceptedPrint("\nWould You Like to Run Another Test (Y/N) ? ");
		
		if (disableInterception != NULL)
			disableInterception();

		scanf("%s", pszDecision);

		if (enableInterception != NULL)
			enableInterception();

		if ((_stricmp(pszDecision, "y")))
			break;
	}


	return 0;
}