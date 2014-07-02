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

string CreateString (string tempDir, bool dirName)
{
	string path = tempDir + "\\";
	size_t pathLen = tempDir.length () + 1 + 1;
	size_t maxPathLen = MAX_PATH - (tempDir.length () + 1  + 1) - 5;
	char* chars[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
	int i = 0;

	if (dirName) {
		maxPathLen -= (8+1+3);
	}

	while (pathLen < maxPathLen) {
		path += chars[i];
		i++;
		if (i >= 10) {
			i = 0;
		}
		pathLen++;
	}

	if (dirName) {
		path += ".dir_";
	} else {
		path += ".file";
	}

	return path;
}

wstring CreateWString (wstring tempDir, wstring machineName, bool dirName)
{
	wstring path = L"";
	size_t pathLen = 1;
	size_t maxPathLen = 255 - 5;
	wchar_t* chars[] = {L"1", L"2", L"3", L"4", L"5", L"6", L"7", L"8", L"9", L"0"};
	int i = 0;

	if (machineName == L"") {
		path += L"\\\\?\\" + tempDir + L"\\";
		pathLen += tempDir.length () + 1;
	} else {
		path += L"\\\\?\\UNC\\" + machineName + L"\\";
		pathLen += machineName.length () + 1;
	}

	while (pathLen < maxPathLen) {
		path += chars[i];
		i++;
		if (i >= 10) {
			i = 0;
		}
		pathLen++;
	}

	if (dirName) {
		path += L".dir_";
	} else {
		path += L".file";
	}

	return path;
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

	BOOL bRes = false;
	OFSTRUCT ofStruct;

	string tempPath = "..\\..";
	wstring LtempPath = L"..\\..";
	string machineName = "";
	string longDirName = CreateString (tempPath, true);
	string longFileName = CreateString (tempPath, false);
	wstring LlongDirName = CreateWString (LtempPath, L"", true);
	wstring LlongFileName = CreateWString (LtempPath, L"", false);
	wstring LlongFarDirName = CreateWString (L"", L"127.0.0.1\\c$", true);
	wstring LlongFarFileName = CreateWString (L"", L"127.0.0.1\\c$", false);
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

		cout << (tempPath + "\\resources9file.txt").c_str ();
		outFile = new ofstream ((tempPath + "\\resources9file.txt").c_str ());
	}

	HANDLE fHandle = NULL;

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileW (LlongFileName.c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "CreateFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (DWORD) fHandle << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileW (LlongFarFileName.c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "CreateFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (DWORD) fHandle << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/fHandle = CreateFileA (longFileName.c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "CreateFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (DWORD) fHandle << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileA (longFileName.c_str (), (tempPath + "\\dummyFile1.txt").c_str (), false);
	(*outFile) << "Name: " << longFileName << "\n";
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
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileW (LlongFileName.c_str (), LlongFarFileName.c_str (), false);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "CopyFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "CopyFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW (LlongFarFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileExA (longFileName.c_str (), (tempPath + "\\dummyFile1.txt").c_str (), NULL, NULL, false, 0);
	(*outFile) << "Name: " << longFileName << "\n";
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
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CopyFileExW (LlongFileName.c_str (), LlongFarFileName.c_str (), NULL, NULL, false, 0);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "CopyFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "CopyFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW (LlongFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryA (longDirName.c_str (), NULL);
	(*outFile) << "Name: " << longDirName << "\n";
	(*outFile) << "Func: " << "CreateDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryA (longDirName.c_str ());
	(*outFile) << "Name: " << longDirName << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryW (LlongDirName.c_str (), NULL);
	(*outFile) << "Name: " << ConvertToString (LlongDirName) << "\n";
	(*outFile) << "Func: " << "CreateDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW (LlongDirName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongDirName) << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryW (LlongFarDirName.c_str (), NULL);
	(*outFile) << "Name: " << ConvertToString (LlongFarDirName) << "\n";
	(*outFile) << "Func: " << "CreateDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW (LlongFarDirName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFarDirName) << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExA (tempPath.c_str (), longDirName.c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << longDirName << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryA (longDirName.c_str ());
	(*outFile) << "Name: " << longDirName << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExW (LtempPath.c_str (), (LlongDirName).c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongDirName) << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW (LlongDirName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongDirName) << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateDirectoryExW (LtempPath.c_str (), (LlongFarDirName).c_str (), NULL);
	(*outFile) << "Name: " << tempPath << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFarDirName) << "\n";
	(*outFile) << "Func: " << "CreateDirectoryExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = RemoveDirectoryW (LlongFarDirName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFarDirName) << "\n";
	(*outFile) << "Func: " << "RemoveDirectoryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FlushFileBuffers (fHandle);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "FlushFileBuffers" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	DWORD appPathSize = DefSize;
	char appPath[DefSize];
	DWORD temp = 0;


	WIN32_FILE_ATTRIBUTE_DATA fInfo;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileAttributesA (longFileName.c_str ());
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileAttributesA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileAttributesExA (longFileName.c_str (), GetFileExInfoStandard, &fInfo);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileAttributesExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileAttributesW (LlongFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "GetFileAttributesW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileAttributesExW (LlongFileName.c_str (), GetFileExInfoStandard, &fInfo);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "GetFileAttributesExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileAttributesW (LlongFarFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "GetFileAttributesW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileAttributesExW (LlongFarFileName.c_str (), GetFileExInfoStandard, &fInfo);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "GetFileAttributesExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	LARGE_INTEGER fileSize;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileSize (fHandle, NULL);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileSize" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileSizeEx (fHandle, &fileSize);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileSizeEx" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/temp = GetFileType (fHandle);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileType" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	HMODULE hModule = NULL;


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileA (longFileName.c_str (), (tempPath + "\\dummyFile1.txt").c_str ());
	(*outFile) << "Name: " << longFileName << "\n";
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
	/*<Resource>*/bRes = MoveFileExA ((tempPath + "\\dummyFile1.txt").c_str (), longFileName.c_str (), MOVEFILE_REPLACE_EXISTING);
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "MoveFileExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileW (LlongFileName.c_str (), LlongFarFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileExW (LlongFarFileName.c_str (), LlongFileName.c_str (), MOVEFILE_REPLACE_EXISTING);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressA (longFileName.c_str (), (tempPath + "\\dummyFile1.txt").c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << longFileName << "\n";
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
	/*<Resource>*/bRes = MoveFileWithProgressA ((tempPath + "\\dummyFile1.txt").c_str (), longFileName.c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << tempPath << "\\dummyFile1.txt" << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW (LlongFileName.c_str (), LlongFarFileName.c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = MoveFileWithProgressW (LlongFarFileName.c_str (), LlongFileName.c_str (), NULL, NULL, MOVEFILE_WRITE_THROUGH);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "MoveFileWithProgressW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
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
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "ReadFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = ReadFileScatter (fHandle, NULL, 0, NULL, NULL);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "ReadFileScatter" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileAttributesA (longFileName.c_str (), FILE_ATTRIBUTE_NORMAL);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "SetFileAttributesA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileAttributesW (LlongFileName.c_str (), FILE_ATTRIBUTE_NORMAL);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "SetFileAttributesW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileAttributesW (LlongFarFileName.c_str (), FILE_ATTRIBUTE_NORMAL);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "SetFileAttributesW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = WriteFile (fHandle, NULL, 0, &temp, NULL);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "WriteFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = WriteFileGather (fHandle, NULL, 0, NULL, NULL);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "WriteFileGather" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileShortNameA (fHandle, "short.txt");
	(*outFile) << "Name: " << longFileName << "\n";
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
	(*outFile) << "Name: " << longFileName << "\n";
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
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "SetFileValidData" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetEndOfFile (fHandle);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "SetEndOfFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	overlapped.hEvent = 0;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = LockFile (fHandle, 0, 0, 0, 0);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "LockFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = UnlockFile (fHandle, 0, 0, 0, 0);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "UnlockFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = LockFileEx (fHandle, 0, 0, 0, 0, &overlapped);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "LockFileEx" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = UnlockFileEx (fHandle, 0, 0, 0, &overlapped);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "UnlockFileEx" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = EncryptFileA (longFileName.c_str ());
	(*outFile) << "Name: " << longFileName;
	(*outFile) << "Func: " << "EncryptFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DecryptFileA (longFileName.c_str (), 0);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "DecryptFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = EncryptFileW (LlongFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "EncryptFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DecryptFileW (LlongFileName.c_str (), 0);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "DecryptFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = EncryptFileW (LlongFarFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "EncryptFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DecryptFileW (LlongFarFileName.c_str (), 0);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "DecryptFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FileEncryptionStatusA (longFileName.c_str (), &temp);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "FileEncryptionStatusA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FileEncryptionStatusW (LlongFileName.c_str (), &temp);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "FileEncryptionStatusW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = FileEncryptionStatusW (LlongFarFileName.c_str (), &temp);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "FileEncryptionStatusW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	PENCRYPTION_CERTIFICATE_HASH_LIST pUsers = NULL;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = QueryUsersOnEncryptedFile (LlongFileName.c_str (), &pUsers);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "QueryUsersOnEncryptedFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	FreeEncryptionCertificateHashList (pUsers);

	pUsers = NULL;
	GetLocalTime (&systemTime);
	/*<Resource>*/temp = QueryUsersOnEncryptedFile (LlongFarFileName.c_str (), &pUsers);
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "QueryUsersOnEncryptedFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << temp << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	FreeEncryptionCertificateHashList (pUsers);

	SECURITY_DESCRIPTOR secDesc;
	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = GetFileSecurity (longFileName.c_str (), OWNER_SECURITY_INFORMATION, &secDesc, sizeof (secDesc), &temp);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileSecurity" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileSecurity (longFileName.c_str (), OWNER_SECURITY_INFORMATION, &secDesc);
	(*outFile) << "Name: " << longFileName << "\n";
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
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "GetFileTime" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = SetFileTime (fHandle, &creaTime, &accTime, &writTime);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "SetFileTime" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateHardLinkA (longFileName.c_str (), (tempPath + "\\testFile.txt").c_str (), NULL);
	(*outFile) << "Name: " << longFileName << "\n";
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
	/*<Resource>*/bRes = DeleteFileA ((tempPath + "\\testFile.txt").c_str ());
	(*outFile) << "Name: " << tempPath << "\\testFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = CreateHardLinkW (LlongFileName.c_str (), LlongFarFileName.c_str (), NULL);
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "CreateHardLinkW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	//---
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
	(*outFile) << "Func: " << "CreateHardLinkW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW (LlongFarFileName.c_str ());
	(*outFile) << "Name: " << tempPath << "\\testFile1.txt" << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	HFILE hFile = 0;

	GetLocalTime (&systemTime);
	/*<Resource>*/hFile = OpenFile (longFileName.c_str (), &ofStruct, OF_EXIST);
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "OpenFile" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << hFile << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	bRes = CloseHandle (fHandle);

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileA (longFileName.c_str ());
	(*outFile) << "Name: " << longFileName << "\n";
	(*outFile) << "Func: " << "DeleteFileA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW (LlongFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFileName) << "\n";
	(*outFile) << "Func: " << "DeleteFileW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << (bRes?"TRUE":"FALSE") << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/bRes = DeleteFileW (LlongFarFileName.c_str ());
	(*outFile) << "Name: " << ConvertToString (LlongFarFileName) << "\n";
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