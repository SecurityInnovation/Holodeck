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

void main (void) {
	HKEY rKey0 = 0;
	HKEY rKey1 = 0;
	long res = 0;

	string tempPath = "..\\..";
	string machineName = "\\\\127.0.0.1";
	wstring LmachineName = L"\\\\127.0.0.1";
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

		cout << (tempPath + "\\resources7reg.txt").c_str ();
		outFile = new ofstream ((tempPath + "\\resources7reg.txt").c_str ());
	}

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_CLASSES_ROOT, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CLASSES_ROOT" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey0);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_CURRENT_CONFIG, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CURRENT_CONFIG" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_CURRENT_USER, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CURRENT_USER" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_DYN_DATA, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_DYN_DATA" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_LOCAL_MACHINE, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_PERFORMANCE_DATA, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_PERFORMANCE_DATA" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_USERS, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_USERS" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_CLASSES_ROOT, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CLASSES_ROOT" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_CURRENT_CONFIG, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CURRENT_CONFIG" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_CURRENT_USER, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_CURRENT_USER" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_DYN_DATA, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_DYN_DATA" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_LOCAL_MACHINE, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_PERFORMANCE_DATA, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_PERFORMANCE_DATA" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryW (LmachineName.c_str (), HKEY_USERS, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_USERS" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA (machineName.c_str (), HKEY_LOCAL_MACHINE, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegOpenKeyExA (rKey0, "SOFTWARE\\HolodeckEE", 0, KEY_ALL_ACCESS, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE" << "\n";
	(*outFile) << "Func: " << "RegOpenKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyA (rKey0, "ResourcePaneTestKey1", &rKey1);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey1");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyW (rKey0, L"ResourcePaneTestKey2", &rKey1);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey2" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, L"ResourcePaneTestKey2");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey2" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, "ResourcePaneTestKey3", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey3" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey3");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey3" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, L"ResourcePaneTestKey4", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey4" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, L"ResourcePaneTestKey4");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey4" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyW" << "\n";
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
	/*<Resource>*/res = RegEnumKeyA (rKey0, 0, keyName, keySize);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << keyName << "\n";
	(*outFile) << "Func: " << "RegEnumKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	keySize = DefSize/2;

	GetLocalTime (&systemTime);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << keyName << "\n";
	/*<Resource>*/res = RegEnumKeyW (rKey0, 0, (LPWSTR) keyName, keySize);
	(*outFile) << "Func: " << "RegEnumKeyW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	keySize = DefSize;
	classSize = DefSize;

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegEnumKeyExA (rKey0, 0, keyName, &keySize, 0, className, &classSize, &fileTime);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << keyName << "\n";
	(*outFile) << "Func: " << "RegEnumKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	keySize = DefSize/2;
	classSize = DefSize/2;

	GetLocalTime (&systemTime);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << keyName << "\n";
	/*<Resource>*/res = RegEnumKeyExW (rKey0, 0, (LPWSTR) keyName, &keySize, 0, (LPWSTR) className, &classSize, &fileTime);
	(*outFile) << "Func: " << "RegEnumKeyExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	valueSize = DefSize;

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegEnumValueA (rKey0, 0, valueName, &valueSize, 0, NULL, NULL, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << valueName << "\n";
	(*outFile) << "Func: " << "RegEnumValueA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	valueSize = DefSize/2;

	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\" << valueName << "\n";
	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegEnumValueW (rKey0, 0, (LPWSTR) valueName, &valueSize, 0, NULL, NULL, NULL);
	(*outFile) << "Func: " << "RegEnumValueW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, "ResourcePaneTestKey5", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey5" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey5");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey5" << "\n";
	(*outFile) << "Func: " << "RegDeleteKey" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, L"ResourcePaneTestKey6", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey6" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, L"ResourcePaneTestKey6");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey6" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	WORD temp = 0;
	classSize = DefSize;

	GetLocalTime (&systemTime);
	res = RegQueryInfoKey (rKey0, className, &classSize, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE" << "\n";
	(*outFile) << "Func: " << "RegQueryInfoKey" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	char data[DefSize];
	long dataSize = DefSize;
	dataSize = DefSize;


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegQueryValueExA (rKey0, "InstallPath", NULL, NULL, NULL, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\InstallPath" << "\n";
	(*outFile) << "Func: " << "RegQueryValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegQueryValueExW (rKey0, L"InstallPath", NULL, NULL, NULL, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\InstallPath" << "\n";
	(*outFile) << "Func: " << "RegQueryValueExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, "ResourcePaneTestKey7", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueA (rKey0, "ResourcePaneTestKey7", REG_SZ, "newData0", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueW (rKey0, L"ResourcePaneTestKey7", REG_MULTI_SZ, L"newData1", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExA (rKey1, "newValue2", 0, REG_SZ, (BYTE *) "newData2", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\newValue2" << "\n";
	(*outFile) << "Func: " << "RegSetValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, "newValue2");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\newValue2" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExW (rKey1, L"newValue3", 0, REG_MULTI_SZ, (BYTE *) L"newData3", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\newValue3" << "\n";
	(*outFile) << "Func: " << "RegSetValueExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, L"newValue3");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\newValue3" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueA (rKey1, NULL, REG_SZ, "newData4", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExA (rKey1, NULL, 0, REG_SZ, (BYTE *) "newData5", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueA (rKey1, "", REG_SZ, "newData6", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExA (rKey1, "", 0, REG_SZ, (BYTE *) "newData7", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueW (rKey1, NULL, REG_SZ, L"newData8", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExW (rKey1, NULL, 0, REG_SZ, (BYTE *) "newData9", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueW (rKey1, L"", REG_SZ, L"newData10", 9);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueExW (rKey1, L"", 0, REG_SZ, (BYTE *) "newData11", 9);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueW (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey7");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
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