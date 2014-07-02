#define _WIN32_WINNT 0x0500

#pragma warning(disable : 4311 4267 4244)

#include <windows.h>
#include <string>
#include <fstream>
#include <iostream>
#include <process.h>
#include <stdio.h>

using namespace std;

wstring GetTime (SYSTEMTIME systemTime) {
	wchar_t tempStr[5];
	return wstring (L"") + (systemTime.wMonth < 10 ? L" " : L"") + wstring (_ultow (systemTime.wMonth, tempStr, 10)) + L"/" +
						   (systemTime.wDay < 10 ? L" " : L"") + wstring (_ultow (systemTime.wDay, tempStr, 10)) + L"/" +
						   wstring (_ultow (systemTime.wYear, tempStr, 10)) + L"  " +
						   (systemTime.wHour < 10 ? L" " : L"") + wstring (_ultow (systemTime.wHour, tempStr, 10)) + L":" +
						   (systemTime.wMinute < 10 ? L" " : L"") + wstring (_ultow (systemTime.wMinute, tempStr, 10)) + L":" +
						   (systemTime.wSecond < 10 ? L" " : L"") + wstring (_ultow (systemTime.wSecond, tempStr, 10)) + L":" +
						   (systemTime.wMilliseconds < 10 ? L"  " : (systemTime.wMilliseconds < 100 ? L" " : L"")) + wstring (_ultow (systemTime.wMilliseconds, tempStr, 10));
}

string ConvertToString (wstring str) {
	string result;
	char * temp = (char *) calloc (1, str.length () + 1);
	sprintf (temp, "%S", str);
	result = temp;
	free (temp);
	temp = NULL;

	return result;
}

void main (void) {
	const DefSize = 1024;
	DWORD dw = 0;

	BOOL bRes = false;
	OFSTRUCT ofStruct;

	string tempPath = "..\\..";
	wstring LtempPath = L"..\\..";
	wstring LspecDirName = L"C:\\ώдΘžҖשچ€∞╛⅞♪♂אַﺅﻅﻵŊΔ";
	wstring LspecFileName = L"C:\\ώдΘžҖשچ€∞╛⅞♪♂אַﺅﻅﻵŊΔ";
	string machineName = "";
	HANDLE outFile = NULL;

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

	
		outFile = CreateFileA ((tempPath + "\\resources13file.txt").c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		cout << tempPath << "\\resources13file.txt";
	}

	wchar_t tempStr[100];
	wstring outString = L"";
	HANDLE fHandle = NULL;

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileW (LspecFileName.c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CreateFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow ((DWORD) fHandle, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileW (LspecFileName.c_str (), (LtempPath + L"\\testFile1.txt").c_str (), false);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CopyFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile1.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CopyFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\testFile1.txt").c_str ());
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile1.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"DeleteFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileExW (LspecFileName.c_str (), (LtempPath + L"\\testFile2.txt").c_str (), NULL, NULL, false, 0);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CopyFileExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile2.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CopyFileExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\testFile2.txt").c_str ());
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile2.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"DeleteFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryW (LspecDirName.c_str (), NULL);
	outString = wstring (L"Name: ") + LspecDirName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CreateDirectoryW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW (LspecDirName.c_str ());
	outString = wstring (L"Name: ") + LspecDirName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RemoveDirectoryW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExW (LtempPath.c_str (), (LspecDirName).c_str (), NULL);
	outString = wstring (L"Name: ") + LtempPath + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CreateDirectoryExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LspecDirName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CreateDirectoryExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW (LspecDirName.c_str ());
	outString = wstring (L"Name: ") + LspecDirName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RemoveDirectoryW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FlushFileBuffers (fHandle);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"FlushFileBuffers" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	DWORD appPathSize = DefSize;
	char appPath[DefSize];
	DWORD temp = 0;


	WIN32_FILE_ATTRIBUTE_DATA fInfo;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileAttributesW (LspecFileName.c_str ());
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"GetFileAttributesW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (temp, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileAttributesExW (LspecFileName.c_str (), GetFileExInfoStandard, &fInfo);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"GetFileAttributesExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	LARGE_INTEGER fileSize;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileSize (fHandle, NULL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"GetFileSize" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (temp, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileSizeEx (fHandle, &fileSize);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"GetFileSizeEx" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileType (fHandle);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"GetFileType" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (temp, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	HMODULE hModule = NULL;


	GetLocalTime (&systemTime);
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileW (LspecFileName.c_str (), (LtempPath + L"\\testFile3.txt").c_str ());
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile3.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileExW ((LtempPath + L"\\testFile3.txt").c_str (), LspecFileName.c_str (), MOVEFILE_REPLACE_EXISTING);
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile3.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileExW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW (LspecFileName.c_str (), (LtempPath + L"\\testFile4.txt").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileWithProgressW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile4.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileWithProgressW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW ((LtempPath + L"\\testFile4.txt").c_str (), LspecFileName.c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile4.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileWithProgressW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"MoveFileWithProgressW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	BYTE buffer[DefSize];
	OVERLAPPED overlapped;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = ReadFile (fHandle, buffer, 0, &temp, &overlapped);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"ReadFile" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = ReadFileScatter (fHandle, NULL, 0, NULL, NULL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"ReadFileScatter" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileAttributesW (LspecFileName.c_str (), FILE_ATTRIBUTE_NORMAL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileAttributesW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = WriteFile (fHandle, NULL, 0, &temp, NULL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"WriteFile" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = WriteFileGather (fHandle, NULL, 0, NULL, NULL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"WriteFileGather" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileShortNameA (fHandle, "short.txt");
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileShortNameA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + L"c:" + L"\\short.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileShortNameA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileShortNameW (fHandle, L"short.txt");
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileShortNameW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + L"c:" + L"\\short.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileShortNameW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileValidData (fHandle, 1);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileValidData" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetEndOfFile (fHandle);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetEndOfFile" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	overlapped.hEvent = 0;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = LockFile (fHandle, 0, 0, 0, 0);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"LockFile" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = UnlockFile (fHandle, 0, 0, 0, 0);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"UnlockFile" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = LockFileEx (fHandle, 0, 0, 0, 0, &overlapped);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"LockFileEx" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = UnlockFileEx (fHandle, 0, 0, 0, &overlapped);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"UnlockFileEx" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = EncryptFileW (LspecFileName.c_str ());
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"EncryptFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DecryptFileW (LspecFileName.c_str (), 0);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"DecryptFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FileEncryptionStatusW (LspecFileName.c_str (), &temp);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"FileEncryptionStatusW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	PENCRYPTION_CERTIFICATE_HASH_LIST pUsers = NULL;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = QueryUsersOnEncryptedFile (LspecFileName.c_str (), &pUsers);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"QueryUsersOnEncryptedFile" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (temp, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	FreeEncryptionCertificateHashList (pUsers);


	SECURITY_DESCRIPTOR secDesc;


	FILETIME creaTime;
	FILETIME accTime;
	FILETIME writTime;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileTime (fHandle, &creaTime, &accTime, &writTime);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"GetFileTime" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileTime (fHandle, &creaTime, &accTime, &writTime);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"SetFileTime" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateHardLinkW (LspecFileName.c_str (), (LtempPath + L"\\testFile5.txt").c_str (), NULL);
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CreateHardLinkW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	//---
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile5.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"CreateHardLinkW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW ((LtempPath + L"\\testFile5.txt").c_str ());
	outString = wstring (L"Name: ") + LtempPath + L"\\testFile5.txt" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"DeleteFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	HFILE hFile = 0;


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW (LspecFileName.c_str ());
	outString = wstring (L"Name: ") + LspecFileName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"DeleteFileW" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + (bRes?L"TRUE":L"FALSE") + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	CloseHandle (outFile);

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