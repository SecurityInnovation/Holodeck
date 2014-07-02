#define _WIN32_WINNT 0x0500

#pragma warning(disable : 4311)

#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <process.h>
#include <stdio.h>

using namespace std;

string GetTime (SYSTEMTIME systemTime) {
	char temp[5];
	return string ("") + (systemTime.wMonth < 10 ? " " : "") + string (ultoa (systemTime.wMonth, temp, 10)) + "/" +
		                 (systemTime.wDay < 10 ? " " : "") + string (ultoa (systemTime.wDay, temp, 10)) + "/" +
						 string (ultoa (systemTime.wYear, temp, 10)) + "  " +
						 (systemTime.wHour < 10 ? " " : "") + string (ultoa (systemTime.wHour, temp, 10)) + ":" +
						 (systemTime.wMinute < 10 ? " " : "") + string (ultoa (systemTime.wMinute, temp, 10)) + ":" +
						 (systemTime.wSecond < 10 ? " " : "") + string (ultoa (systemTime.wSecond, temp, 10)) + ":" +
						 (systemTime.wMilliseconds < 10 ? "  " : (systemTime.wMilliseconds < 100 ? " " : "")) + string (ultoa (systemTime.wMilliseconds, temp, 10));
}

void main (void) {
	const DefSize = 1024;

	BOOL bRes = false;
	OFSTRUCT ofStruct;

	string tempPath = "..\\..";
	wstring LtempPath = L"..\\..";
	string baseTemp = "";
	wstring LbaseTemp = L"";
	string machineName = "";
	ofstream *outFile = NULL;

	SYSTEMTIME systemTime;

	{
		char *envTempPath = getenv ("TEMP");
		if (envTempPath == NULL) {
			envTempPath = getenv ("TMP");
		}
		if (envTempPath != NULL) {
			tempPath = envTempPath;
			wchar_t *buff = (wchar_t *) calloc (strlen (envTempPath), sizeof (wchar_t));
			wsprintfW (buff, L"%S", envTempPath);
			LtempPath = buff;
		}

		baseTemp = tempPath.substr (tempPath.find_last_of ("\\") + 1);
		LbaseTemp = LtempPath.substr (LtempPath.find_last_of (L"\\") + 1);

		cout << (tempPath + "\\resources21file.txt").c_str ();
		outFile = new ofstream ((tempPath + "\\resources21file.txt").c_str ());
	}

	HANDLE fHandle = NULL;

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "CreateFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (DWORD) fHandle << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "CreateFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (DWORD) fHandle << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileA ((tempPath + "\\..\\" + baseTemp + "\\testFile.txt").c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "CreateFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (DWORD) fHandle << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str (), false);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str (), false);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileExA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str (), NULL, NULL, false, 0);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileExW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str (), NULL, NULL, false, 0);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "CopyFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryA ((tempPath + "\\..\\" + baseTemp + "\\TestDir").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryA ((tempPath + "\\..\\" + baseTemp + "\\TestDir").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\TestDir").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\TestDir").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExA ((tempPath+ "\\..\\" + baseTemp).c_str (), (tempPath + "\\..\\" + baseTemp + "\\TestDir").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryA ((tempPath + "\\..\\" + baseTemp + "\\TestDir").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExW ((LtempPath + L"\\..\\" + LbaseTemp).c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\TestDir").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\TestDir").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FlushFileBuffers (fHandle);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "FlushFileBuffers" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	DWORD appPathSize = DefSize;
	char appPath[DefSize];
	DWORD temp = 0;

	char *filePart = NULL;


	WIN32_FILE_ATTRIBUTE_DATA fInfo;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileAttributesA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileAttributesA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileAttributesExA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), GetFileExInfoStandard, &fInfo);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileAttributesExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileAttributesW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileAttributesW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileAttributesExW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), GetFileExInfoStandard, &fInfo);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileAttributesExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	LARGE_INTEGER fileSize;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileSize (fHandle, NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileSize" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileSizeEx (fHandle, &fileSize);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileSizeEx" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileType (fHandle);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileType" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	HMODULE hModule = NULL;


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileExA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), MOVEFILE_REPLACE_EXISTING);
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileExW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), MOVEFILE_REPLACE_EXISTING);
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile1.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile1.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	BYTE buffer[DefSize];
	OVERLAPPED overlapped;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = ReadFile (fHandle, buffer, 0, &temp, &overlapped);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "ReadFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = ReadFileScatter (fHandle, NULL, 0, NULL, NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "ReadFileScatter" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileAttributesA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), FILE_ATTRIBUTE_NORMAL);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileAttributesA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileAttributesW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), FILE_ATTRIBUTE_NORMAL);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileAttributesW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = WriteFile (fHandle, NULL, 0, &temp, NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "WriteFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = WriteFileGather (fHandle, NULL, 0, NULL, NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "WriteFileGather" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileShortNameA (fHandle, "short.txt");
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileShortNameA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\short.txt" << "\n";
	(*outFile) << "Func: " << "SetFileShortNameA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileShortNameW (fHandle, L"short.txt");
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileShortNameW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\short.txt" << "\n";
	(*outFile) << "Func: " << "SetFileShortNameW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileValidData (fHandle, 1);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileValidData" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetEndOfFile (fHandle);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "SetEndOfFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	overlapped.hEvent = 0;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = LockFile (fHandle, 0, 0, 0, 0);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "LockFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = UnlockFile (fHandle, 0, 0, 0, 0);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "UnlockFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = LockFileEx (fHandle, 0, 0, 0, 0, &overlapped);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "LockFileEx" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = UnlockFileEx (fHandle, 0, 0, 0, &overlapped);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "UnlockFileEx" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = EncryptFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "EncryptFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DecryptFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), 0);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "DecryptFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = EncryptFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "EncryptFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DecryptFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), 0);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "DecryptFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FileEncryptionStatusA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), &temp);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "FileEncryptionStatusA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FileEncryptionStatusW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), &temp);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "FileEncryptionStatusW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	PENCRYPTION_CERTIFICATE_HASH_LIST pUsers = NULL;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = QueryUsersOnEncryptedFile ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str (), &pUsers);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "QueryUsersOnEncryptedFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	FreeEncryptionCertificateHashList (pUsers);

	SECURITY_DESCRIPTOR secDesc;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileSecurity ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), OWNER_SECURITY_INFORMATION, &secDesc, sizeof (secDesc), &temp);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileSecurity" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileSecurity ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str (), OWNER_SECURITY_INFORMATION, &secDesc);
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileSecurity" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	FILETIME creaTime;
	FILETIME accTime;
	FILETIME writTime;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileTime (fHandle, &creaTime, &accTime, &writTime);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "GetFileTime" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileTime (fHandle, &creaTime, &accTime, &writTime);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "SetFileTime" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateHardLinkA ((tempPath + "\\..\\" + baseTemp + "\\testFile1.txt").c_str (), (tempPath + "\\..\\" + baseTemp + "\\testFile.txt").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile1.txt" << "\n";
	(*outFile) << "Func: " << "CreateHardLinkA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "CreateHardLinkA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

    GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\..\\" + baseTemp + "\\testFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\testFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateHardLinkW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\testFile1.txt").c_str (), (LtempPath + L"\\..\\" + LbaseTemp + L"\\testFile.txt").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\testFile1.txt" << "\n";
	(*outFile) << "Func: " << "CreateHardLinkW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "CreateHardLinkW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\testFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\testFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	HFILE hFile = 0;

	GetLocalTime (&systemTime);
	/*<Resource>*/hFile = OpenFile ((tempPath + "\\..\\" + baseTemp + "\\testFile.txt").c_str (), &ofStruct, OF_EXIST);
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "OpenFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << hFile << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	bRes = CloseHandle (fHandle);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\..\\" + baseTemp + "\\dummyFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\dummyFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\..\\" + baseTemp + "\\testFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\..\\" + LbaseTemp + L"\\testFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\testFile.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	outFile->close ();

/*
	MapViewOfFile
	MapViewOfFileEx
	ReadFileEx
	RegLoadKey
	RegReplaceKey
	RegRestoreKey
	RegSaveKey
	RegSaveKeyEx
	LZCopy
	LZClose
	LZDone
	LZOpenFile
	LZSeek
	_lcreat
	_lclose
	_lopen
	_llseek
	_hread
	_lread
	_hwrite
	_lwrite
	CreateFileMapping
	CreateFileMoniker
*/
}