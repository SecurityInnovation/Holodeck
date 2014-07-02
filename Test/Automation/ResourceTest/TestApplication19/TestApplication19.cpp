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
		}

		cout << (tempPath + "\\resources19reg.txt").c_str ();
		outFile = new ofstream ((tempPath + "\\resources19reg.txt").c_str ());
	}

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_CLASSES_ROOT, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_CLASSES_ROOT" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey0);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_CURRENT_CONFIG, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_CURRENT_CONFIG" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_CURRENT_USER, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_CURRENT_USER" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_DYN_DATA, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_DYN_DATA" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_LOCAL_MACHINE, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_PERFORMANCE_DATA, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_PERFORMANCE_DATA" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_USERS, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_USERS" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";
	res = RegCloseKey (rKey0);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegConnectRegistryA ("", HKEY_LOCAL_MACHINE, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE" << "\n";
	(*outFile) << "Func: " << "RegConnectRegistryA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegOpenKeyExA (rKey0, "SOFTWARE\\HolodeckEE\\", 0, KEY_ALL_ACCESS, &rKey0);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE" << "\n";
	(*outFile) << "Func: " << "RegOpenKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyA (rKey0, "ResourcePaneTestKey1\\", &rKey1);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey1\\");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey1" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, "ResourcePaneTestKey3\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey3" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey3\\");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey3" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, L"ResourcePaneTestKey4\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey4" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, L"ResourcePaneTestKey4\\");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey4" << "\n";
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

	keySize = DefSize/2;

	keySize = DefSize;
	classSize = DefSize;

	keySize = DefSize/2;
	classSize = DefSize/2;

	valueSize = DefSize;

	valueSize = DefSize/2;


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegOpenKeyA (HKEY_LOCAL_MACHINE, "SOFTWARE\\", &rKey1);
	(*outFile) << "Name: " << "HKEY_LOCAL_MACHINE\\SOTWARE" << "\n";
	(*outFile) << "Func: " << "RegOpenKeyA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegOpenKeyExA (HKEY_LOCAL_MACHINE, "SOFTWARE\\", 0, KEY_ALL_ACCESS, &rKey1);
	(*outFile) << "Name: " << "HKEY_LOCAL_MACHINE\\SOTWARE" << "\n";
	(*outFile) << "Func: " << "RegOpenKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	res = RegCloseKey (rKey1);


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, "ResourcePaneTestKey5\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey5" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey5\\");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey5" << "\n";
	(*outFile) << "Func: " << "RegDeleteKey" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExW (rKey0, L"ResourcePaneTestKey6\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey6" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExW" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyW (rKey0, L"ResourcePaneTestKey6\\");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey6" << "\n";
	(*outFile) << "Func: " << "RegDeleteKeyW" << "\n";
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

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegQueryValueA (HKEY_LOCAL_MACHINE, "SOFTWARE\\", data, &dataSize);
	(*outFile) << "Name: " << "HKEY_LOCAL_MACHINE\\SOFTWARE" << data << "\n";
	(*outFile) << "Func: " << "RegQueryValueA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	dataSize = DefSize;


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegQueryValueExA (rKey0, "InstallPath", NULL, NULL, NULL, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\InstallPath" << "\n";
	(*outFile) << "Func: " << "RegQueryValueExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegCreateKeyExA (rKey0, "ResourcePaneTestKey7\\", 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7" << "\n";
	(*outFile) << "Func: " << "RegCreateKeyExA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegSetValueA (rKey0, "ResourcePaneTestKey7\\", REG_SZ, "newData0", 8);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegSetValueA" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";

	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteValueA (rKey1, NULL);
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7\\(Default)" << "\n";
	(*outFile) << "Func: " << "RegDeleteValue" << "\n";
	(*outFile) << "Proc: " << _getpid () << "\n";
	(*outFile) << "Time: " << GetTime (systemTime).c_str () << "\n";
	(*outFile) << "Retv: " << res << "\n";
	(*outFile) << "Errc: " << GetLastError () << "\n";
	(*outFile) << "Exce: " << "" << "\n";


	GetLocalTime (&systemTime);
	/*<Resource>*/res = RegDeleteKeyA (rKey0, "ResourcePaneTestKey7\\");
	(*outFile) << "Name: " << (machineName != "" ? machineName + "\\\\" : "") << "HKEY_LOCAL_MACHINE\\SOFTWARE\\HolodeckEE\\ResourcePaneTestKey7" << "\n";
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