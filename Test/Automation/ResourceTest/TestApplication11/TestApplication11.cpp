#pragma warning(disable : 4311)

#include <windows.h>
#include <winreg.h>
#include <string>
#include <fstream>
#include <iostream>
#include <process.h>

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

string ConvertToString (wstring str) {
	string result;
	char * temp = (char *) calloc (2, str.length () + 1);
	sprintf (temp, "%S", str.c_str ());
	result = temp;
	free (temp);
	temp = NULL;

	return result;
}

void main (void) {
	HKEY rKey0 = 0;
	HKEY rKey1 = 0;
	long res = 0;

	string tempPath = "..\\..";
	string machineName = "";
	char * longKeyName = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890KEY__";
	char * longValName = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890VALUE";
	wchar_t * LlongKeyName = L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890KEY__";
	wchar_t * LlongValName = L"1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890VALUE";
	ofstream *outFile = NULL;

	SYSTEMTIME systemTime;

	{
		char *envTempPath = getenv ("TEMP");
		if (envTempPath == NULL) {
			envTempPath = getenv ("TMP");
		}
		if (envTempPath != NULL) {
			tempPath = envTempPath;
		}

		cout << (tempPath + "\\resources11reg.txt").c_str ();
		outFile = new ofstream ((tempPath + "\\resources11reg.txt").c_str ());
	}

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_LOCAL_MACHINE, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CLASSES_ROOT" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegOpenKeyExA (rKey0, "SOFTWARE\\HolodeckEE", 0, KEY_ALL_ACCESS, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\n";
	(*outFile) << "Func: " << "RegOpenKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyA (rKey0, longKeyName, &rKey1);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longKeyName << "\n";
	(*outFile) << "Func: " << "RegCreateKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, longKeyName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longKeyName << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyW (rKey0, LlongKeyName, &rKey1);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongKeyName) << "\n";
	(*outFile) << "Func: " << "RegCreateKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, LlongKeyName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongKeyName) << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, longKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longKeyName << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, longKeyName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longKeyName << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, LlongKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongKeyName) << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, LlongKeyName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongKeyName) << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


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
	/*<Resource>*/res = RegCreateKeyExA (rKey0, longKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longKeyName << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, longKeyName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longKeyName << "\n";
	(*outFile) << "Func: " << "RegDeleteKey" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, LlongKeyName, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongKeyName) << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, LlongKeyName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongKeyName) << "\n";
	(*outFile) << "Func: " << "RegDeleteKey" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";



	WORD temp = 0;
	classSize = DefSize;


	char data[DefSize];
	long dataSize = DefSize;
	dataSize = DefSize;

	dataSize = DefSize;

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExA (rKey1, longValName, 0, REG_SZ, (BYTE *) "newData2", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longValName << "\n";
	(*outFile) << "Func: " << "RegSetValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, longValName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << longValName << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExW (rKey1, LlongValName, 0, REG_SZ, (BYTE *) "newData2", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongValName) << "\n";
	(*outFile) << "Func: " << "RegSetValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, LlongValName);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << ConvertToString (LlongValName) << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	outFile->close ();

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