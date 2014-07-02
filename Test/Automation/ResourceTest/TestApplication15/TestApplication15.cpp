#pragma warning(disable : 4311 4267 4244)

#include <windows.h>
#include <winreg.h>
#include <string>
#include <fstream>
#include <iostream>
#include <process.h>

using namespace std;

wstring GetTime (SYSTEMTIME systemTime) {
	wchar_t temp[5];
	return wstring (L"") + (systemTime.wMonth < 10 ? L" " : L"") + wstring (_ultow (systemTime.wMonth, temp, 10)) + L"/" +
		                   (systemTime.wDay < 10 ? L" " : L"") + wstring (_ultow (systemTime.wDay, temp, 10)) + L"/" +
						   wstring (_ultow (systemTime.wYear, temp, 10)) + L"  " +
						   (systemTime.wHour < 10 ? L" " : L"") + wstring (_ultow (systemTime.wHour, temp, 10)) + L":" +
						   (systemTime.wMinute < 10 ? L" " : L"") + wstring (_ultow (systemTime.wMinute, temp, 10)) + L":" +
						   (systemTime.wSecond < 10 ? L" " : L"") + wstring (_ultow (systemTime.wSecond, temp, 10)) + L":" +
						   (systemTime.wMilliseconds < 10 ? L"  " : (systemTime.wMilliseconds < 100 ? L" " : L"")) + wstring (_ultow (systemTime.wMilliseconds, temp, 10));
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
	HKEY rKey0 = 0;
	HKEY rKey1 = 0;
	long res = 0;
	DWORD dw = 0;

	string tempPath = "..\\..";
	wstring LtempPath = L"..\\..";
	string machineName = "";
	wchar_t * LspecKeyName = L"ώдΘžҖשچ€∞╛⅞♪♂אַﺅﻅﻵŊΔ";
	wchar_t * LspecValName = L"ώдΘžҖשچ€∞╛⅞♪♂אַﺅﻅﻵŊΔ";
	HANDLE outFile = NULL;

	SYSTEMTIME systemTime;

	{
		char *envTempPath = getenv ("TEMP");
		if (envTempPath == NULL) {
			envTempPath = getenv ("TMP");
		}
		if (envTempPath != NULL) {
			tempPath = envTempPath;
		}

		cout << (tempPath + "\\resources15reg.txt").c_str ();
		outFile = CreateFileA ((tempPath + "\\resources15reg.txt").c_str (), FILE_ALL_ACCESS, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	wchar_t tempStr[100];
	wstring outString = L"";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_LOCAL_MACHINE, &rKey0);
	outString = wstring (L"Name: ") + L"HKEY_LOCAL_MACHINE" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegConnectRegistryA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegOpenKeyExA (rKey0, "SOFTWARE\\HolodeckEE", 0, KEY_ALL_ACCESS, &rKey0);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegOpenKeyExA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyW (rKey0, LspecKeyName, &rKey1);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecKeyName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegCreateKeyA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, LspecKeyName);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecKeyName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegDeleteKeyA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, LspecKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecKeyName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegCreateKeyExA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, LspecKeyName);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecKeyName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegDeleteKeyA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	const		DefSize = 1024;
	DWORD		keySize = 0;
	DWORD		valueSize = 0;
	DWORD		classSize = 0;
	char		keyName[DefSize];
	char		valueName[DefSize];
	char		className[DefSize];
	FILETIME	fileTime;

	keySize = DefSize;

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, LspecKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecKeyName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegCreateKeyExA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, LspecKeyName);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecKeyName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegDeleteKey" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);



	WORD temp = 0;
	classSize = DefSize;


	char data[DefSize];
	long dataSize = DefSize;
	dataSize = DefSize;

	dataSize = DefSize;

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExW (rKey1, LspecValName, 0, REG_SZ, (BYTE *) "newData2", 8);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecValName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegSetValueExA" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, LspecValName);
	outString = wstring (L"Name: ") + L"" + L"HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" + LspecValName + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Func: ") + L"RegDeleteValue" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Proc: ") + _ultow (_getpid (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Time: ") + GetTime (systemTime).c_str () + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Retv: ") + _ultow (res, tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Errc: ") + _ultow (GetLastError (), tempStr, 10) + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);
	outString = wstring (L"Exce: ") + L"" + L"\n";
	WriteFile (outFile, outString.c_str (), 2 * outString.length (), &dw, NULL);


	CloseHandle (outFile);

/*
RegLoadKey
RegOpenUserClassesRoot
RegReplaceKey
RegRestoreKey
RegSaveKey
RegSaveKeyEx
RegSetKeySecurity
RegUnLoadKey
*/
}