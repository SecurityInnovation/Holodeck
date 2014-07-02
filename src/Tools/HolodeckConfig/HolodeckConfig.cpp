// HolodeckConfig.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

//get current system time and record in the registry
int _tmain(int argc, _TCHAR* argv[])
{
	//WARNING: This code is replicated in RegisterationMgr.cpp::ConvertToFeatureDisabledTrial
	//If config generation method changes, it must be reflected in the method above as well

	HKEY configKey;
	FILETIME fileTime;
	bool keyExists = false;

	GetSystemTimeAsFileTime(&fileTime);

	if (RegCreateKey(HKEY_LOCAL_MACHINE, "Software\\Security Innovation\\Holodeck Enterprise Edition", &configKey) != ERROR_SUCCESS)
	{
		printf("Create key failed\n");
		return 0;
	}

	//WARNING: This code is replicated in RegisterationMgr.cpp::ConvertToFeatureDisabledTrial
	//If config generation method changes, it must be reflected in the method above as well
	if (RegQueryValueEx(configKey, "Config3", 0, 0, 0, 0) == ERROR_SUCCESS)
	{
		keyExists = true;
	}

	if ( keyExists && (RegQueryValueEx(configKey, "Config4", 0, 0, 0, 0) == ERROR_SUCCESS) )
	{
		//both keys already exist, don't overwrite
		printf("both keys exist\n");
		return 0;
	}

	if (RegSetValueEx(configKey, "Config3", 0, REG_DWORD, (const BYTE*) &fileTime.dwHighDateTime, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		printf("Set value failed\n");
		return 0;
	}

	if (RegSetValueEx(configKey, "Config4", 0, REG_DWORD, (const BYTE*) &fileTime.dwLowDateTime, sizeof(DWORD)) != ERROR_SUCCESS)
	{
		printf("Set value failed\n");
		return 0;
	}

	return 1;
}

