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

		cout << (tempPath + "\\resources17file.txt").c_str ();
		outFile = new ofstream ((tempPath + "\\resources17file.txt").c_str ());
	}

	HANDLE fHandle = NULL;


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryA ((tempPath + "\\TestDir\\").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryA ((tempPath + "\\TestDir\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryW ((LtempPath + L"\\TestDir\\").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW ((LtempPath + L"\\TestDir\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExA ((tempPath + "\\").c_str (), (tempPath + "\\TestDir\\").c_str (), NULL);
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
	/*<Resource>*/bRes = RemoveDirectoryA ((tempPath + "\\TestDir\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExW ((LtempPath + L"\\").c_str (), (LtempPath + L"\\TestDir\\").c_str (), NULL);
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
	/*<Resource>*/bRes = RemoveDirectoryW ((LtempPath + L"\\TestDir\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryA ((tempPath + "\\TestDir1\\").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\TestDir" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileA ((tempPath + "\\TestDir1\\").c_str (), (tempPath + "\\TestDir2\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir1" << "\n";
	(*outFile) << "Func: " << "MoveFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir2" << "\n";
	(*outFile) << "Func: " << "MoveFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileExA ((tempPath + "\\TestDir2\\").c_str (), (tempPath + "\\TestDir3\\").c_str (), MOVEFILE_REPLACE_EXISTING);
	(*outFile) << "Name: " << tempPath << "\\TestDir2" << "\n";
	(*outFile) << "Func: " << "MoveFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir3" << "\n";
	(*outFile) << "Func: " << "MoveFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressA ((tempPath + "\\TestDir3\\").c_str (), (tempPath + "\\TestDir4\\").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\TestDir3" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir4" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryA ((tempPath + "\\TestDir4\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir4" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryW ((LtempPath + L"\\TestDir1\\").c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\\TestDir1" << "\n";
	(*outFile) << "Func: " << "CreateDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileW ((LtempPath + L"\\TestDir1\\").c_str (), (LtempPath + L"\\TestDir2\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir1" << "\n";
	(*outFile) << "Func: " << "MoveFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir2" << "\n";
	(*outFile) << "Func: " << "MoveFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileExW ((LtempPath + L"\\TestDir2\\").c_str (), (LtempPath + L"\\TestDir3\\").c_str (), MOVEFILE_REPLACE_EXISTING);
	(*outFile) << "Name: " << tempPath << "\\TestDir2" << "\n";
	(*outFile) << "Func: " << "MoveFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath << "\\TestDir3" << "\n";
	(*outFile) << "Func: " << "MoveFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW ((LtempPath + L"\\TestDir3\\").c_str (), (LtempPath + L"\\TestDir4\\").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\TestDir3" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << tempPath + "\\TestDir4" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW ((LtempPath + L"\\TestDir4\\").c_str ());
	(*outFile) << "Name: " << tempPath << "\\TestDir4" << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	DWORD appPathSize = DefSize;
	char appPath[DefSize];
	DWORD temp = 0;


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