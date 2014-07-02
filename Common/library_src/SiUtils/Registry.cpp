//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Registry.cpp
//
// DESCRIPTION: Contains implementation of the class Registry.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 DEC 2002		 B. Shirey	 File created.
// 05 FEB 2003       J. Taylor   Added FindKey function
//*************************************************************************
#include "registry.h"
#include <stdio.h>

namespace SiUtils
{
	//*************************************************************************
	// Method:		Registry
	// Description: Constructor for the Registry class
	//
	// Parameters:
	//	rootKey - The root key for this instance of the registry
	//		Assumptions - the key is already open
	//
	// Return Value: None
	//*************************************************************************
	Registry::Registry(HKEY rootKey)
	{
		this->rootKey = rootKey;
		currentKey = NULL;
	}

	//*************************************************************************
	// Method:		~Registry
	// Description: Destructor for the Registry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Registry::~Registry(void)
	{
		rootKey = NULL;
		
		Close();
	}

	//*************************************************************************
	// Method:		KeyExists
	// Description: Determines whether or not a key exists
	//
	// Parameters:
	//	keyPath - the path under the root key to check for existence
	//
	// Return Value: true if the key exists, false otherwise
	//*************************************************************************
	bool Registry::KeyExists(const SiString &keyPath)
	{
		if (currentKey != NULL)
		{
			RegCloseKey (currentKey);
			currentKey = NULL;
		}

		LONG returnValue = RegOpenKeyExW (rootKey, keyPath, 0L, KEY_ALL_ACCESS, &currentKey);
		
		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		ValueExists
	// Description: Determines whether or not a value exists
	//
	// Parameters:
	//	valuePath - the path under the currently open key to check for existence
	//
	// Return Value: true if the value exists, false otherwise
	//*************************************************************************
	bool Registry::ValueExists(const SiString &valuePath)
	{
		LONG returnValue = RegQueryValueExW(currentKey, valuePath, NULL, NULL, NULL, NULL);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		CreateKey
	// Description: Creates a new key if it does not exist, otherwise it opens
	//		the specified key
	//
	// Parameters:
	//	rootKey - An open key to create the key under
	//	keyPath - the path under rootKey to create
	//
	// Return Value: true if the create was successful, false otherwise
	//*************************************************************************
	bool Registry::CreateKey(HKEY rootKey, const SiString &keyPath)
	{
		DWORD disposition;

		if (currentKey != NULL)
		{
			RegCloseKey(currentKey);
			currentKey = NULL;
		}

		// Initialize a security descriptor.
		PSECURITY_DESCRIPTOR pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pSD, TRUE, NULL, FALSE);
		// Initialize a security attributes structure.
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = FALSE;

		LONG returnValue = RegCreateKeyExW(rootKey, keyPath, 0, NULL, REG_OPTION_NON_VOLATILE, 
			KEY_ALL_ACCESS, &sa, &currentKey, &disposition);

		LocalFree(pSD);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		OpenKey
	// Description: Tries to open an existing key
	//
	// Parameters:
	//	rootKey - An already open key to which contains the key to open
	//	keyPath - the path under rootKey to open
	//
	// Return Value: true if the open was successful, false otherwise
	//*************************************************************************
	bool Registry::OpenKey(HKEY rootKey, const SiString &keyPath)
	{
		if (currentKey != NULL)
		{
			RegCloseKey(currentKey);
			currentKey = NULL;
		}

		LONG returnValue = RegOpenKeyExW(rootKey, keyPath, 0, KEY_ALL_ACCESS, &currentKey);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		OpenKey
	// Description: Tries to open an existing key
	//
	// Parameters:
	//	rootKey - An already open key to which contains the key to open
	//	keyPath - the path under rootKey to open
	//	permissions - the permissions to open the key with
	//
	// Return Value: true if the open was successful, false otherwise
	//*************************************************************************
	bool Registry::OpenKey(HKEY rootKey, const SiString &keyPath, unsigned long permissions)
	{
		if (currentKey != NULL)
		{
			RegCloseKey(currentKey);
			currentKey = NULL;
		}

		LONG returnValue = RegOpenKeyExW(rootKey, keyPath, 0, permissions, &currentKey);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Close
	// Description: Closes the current key
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void Registry::Close()
	{
		if (currentKey != NULL)
		{
			RegCloseKey(currentKey);
			currentKey = NULL;
		}
	}

	//*************************************************************************
	// Method:		DeleteKey
	// Description: Tries to delete an existing key
	//
	// Parameters:
	//	rootKey - An already open key to which contains the key to delete
	//	keyPath - the path under rootKey to delete
	//
	// Return Value: true if the delete was successful, false otherwise
	//*************************************************************************
	bool Registry::DeleteKey(HKEY rootKey, const SiString &keyPath)
	{
		LONG returnValue = RegDeleteKeyW(rootKey, keyPath);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		DeleteValue
	// Description: Tries to delete an existing value
	//
	// Parameters:
	//	valuePath - the path under the current key to delete
	//
	// Return Value: true if the delete was successful, false otherwise
	//*************************************************************************
	bool Registry::DeleteValue(const SiString &valuePath)
	{
		LONG returnValue = RegDeleteValueW(currentKey, valuePath);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		DeleteKeyRecursively
	// Description: Tries to delete an existing key and all its subkeys and values
	//
	// Parameters:
	//	rootKey - An already open key to which contains the key to delete
	//	keyPath - the path under rootKey to delete
	//
	// Return Value: true if the delete was successful, false otherwise
	//*************************************************************************
	bool Registry::DeleteKeyRecursively(HKEY rootKey, const SiString &keyPath)
	{
		DWORD numSubkeys;
		DWORD maxSubkeyLength;
		DWORD numValues;
		DWORD maxValueLength;
		unsigned int i;
		unsigned int j;
		HKEY thisKey;

		// open the key
		if (RegOpenKeyExW(rootKey, keyPath, 0, KEY_ALL_ACCESS, &thisKey) != ERROR_SUCCESS)
		{
			return false;
		}

		// get the number of keys and values under this key
		if (RegQueryInfoKeyW(thisKey, NULL, NULL, NULL, &numSubkeys, &maxSubkeyLength,
			NULL, &numValues, &maxValueLength, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			RegCloseKey(thisKey);
			return false;
		}

		// get room for the value names
		wchar_t **valueList = new wchar_t *[numValues];
		if (valueList == NULL)
		{
			RegCloseKey(thisKey);
			return false;
		}
		FillMemory(valueList, numValues * sizeof(wchar_t *), 0);

		// make a list this key's values
		for (i = 0; i < numValues; i++)
		{
			valueList[i] = new wchar_t [maxValueLength + 1];
			if (valueList[i] == NULL)
			{
				for (j = 0; j < numValues; j++)
					if (valueList[j] != NULL) delete[] valueList[j];
				delete[] valueList;

				RegCloseKey(thisKey);
				return false;
			}

			DWORD size = maxValueLength + 1;
			if (RegEnumValueW(thisKey, i, valueList[i], &size, 0, NULL, NULL, NULL) != ERROR_SUCCESS)
			{
				for (j = 0; j < numValues; j++)
					if (valueList[j] != NULL) delete[] valueList[j];
				delete[] valueList;
				RegCloseKey(thisKey);
				return false;
			}
		}

		// now delete the values
		for (i = 0; i < numValues; i++)
		{
			if (RegDeleteValueW(thisKey, valueList[i]) != ERROR_SUCCESS)
			{
				for (j = 0; j < numValues; j++)
					if (valueList[j] != NULL) delete[] valueList[j];
				delete[] valueList;

				RegCloseKey(thisKey);
				return false;
			}
		}

		// clean up the value list
		for (j = 0; j < numValues; j++)
			if (valueList[j] != NULL) delete[] valueList[j];
		delete[] valueList;

		// get room for the key names
		wchar_t **subkeyList = new wchar_t *[numSubkeys];
		if (subkeyList == NULL)
		{
			RegCloseKey(thisKey);
			return false;
		}
		FillMemory(subkeyList, sizeof(wchar_t *) * numSubkeys, 0);

		// Delete the subkeys
		for (i = 0; i < numSubkeys; i++)
		{
			subkeyList[i] = new wchar_t[maxSubkeyLength + 1];
			if (subkeyList[i] == NULL)
			{
				for (j = 0; j < numSubkeys; j++)
					if (subkeyList[j] != NULL) delete[] subkeyList[j];
				delete[] subkeyList;
				RegCloseKey(thisKey);
				return false;
			}

			if (RegEnumKeyW(thisKey, i, subkeyList[i], maxSubkeyLength + 1) != ERROR_SUCCESS)
			{
				for (j = 0; j < numSubkeys; j++)
					if (subkeyList[j] != NULL) delete[] subkeyList[j];
				delete[] subkeyList;
				RegCloseKey(thisKey);
				return false;
			}
		}

		for (i = 0; i < numSubkeys; i++)
		{
			SiString subPath = keyPath;
			subPath += "\\";
			subPath += subkeyList[i];

			// now delete the subkey's subkeys and values
			if (!DeleteKeyRecursively(rootKey, subPath))
			{
				for (j = 0; j < numSubkeys; j++)
					if (subkeyList[j] != NULL) delete[] subkeyList[j];
				delete[] subkeyList;
				RegCloseKey(thisKey);
				return false;
			}
		}

		// clean up
		for (j = 0; j < numSubkeys; j++)
			if (subkeyList[j] != NULL) delete[] subkeyList[j];
		delete[] subkeyList;
		RegCloseKey(thisKey);

		// now actually delete this key
		if (RegDeleteKeyW(rootKey, keyPath) != ERROR_SUCCESS)
		{
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		Write
	// Description: Sets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to set
	//	value - what to set the value to
	//
	// Return Value: true if the write was successful, false otherwise
	//*************************************************************************
	bool Registry::Write (const SiString &valuePath, int value)
	{
		LONG returnValue = RegSetValueExW(currentKey, valuePath, 0, REG_DWORD,
			(const BYTE *) &value, sizeof(DWORD));

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Write
	// Description: Sets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to set
	//	value - what to set the value to
	//
	// Return Value: true if the write was successful, false otherwise
	//*************************************************************************
	bool Registry::Write (const SiString &valuePath, DWORD64 value)
	{
		LONG returnValue = RegSetValueExW(currentKey, valuePath, 0, REG_QWORD,
			(const BYTE *) &value, sizeof(DWORD64));

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Write
	// Description: Sets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to set
	//	value - what to set the value to
	//
	// Return Value: true if the write was successful, false otherwise
	//*************************************************************************
	bool Registry::Write (const SiString &valuePath, unsigned long value)
	{
		LONG returnValue = RegSetValueExW(currentKey, valuePath, 0, REG_DWORD,
			(const BYTE *) &value, sizeof(DWORD));

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Write
	// Description: Sets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to set
	//	value - what to set the value to
	//
	// Return Value: true if the write was successful, false otherwise
	//*************************************************************************
	bool Registry::Write (const SiString &valuePath, const SiString &value)
	{
		wchar_t *valueStr = value;
		LONG returnValue = RegSetValueExW(currentKey, valuePath, 0, REG_SZ,
			(const BYTE *)valueStr, sizeof(wchar_t) * (value.GetLength() + 1));

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Write
	// Description: Sets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to set
	//	value - what to set the value to
	//	byteCount - the number of characters in value
	//
	// Return Value: true if the write was successful, false otherwise
	//*************************************************************************
	bool Registry::Write (const SiString &valuePath, const char *value, long byteCount)
	{
		LONG returnValue = RegSetValueExW(currentKey, valuePath, 0, REG_BINARY,
			(const BYTE *) value, byteCount + 1);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Write
	// Description: Sets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to set
	//	value - what to set the value to
	//	byteCount - the size in bytes of the value parameter	
	//
	// Return Value: true if the write was successful, false otherwise
	//*************************************************************************
	bool Registry::Write (const SiString &valuePath, void *value, unsigned long byteCount)
	{
		LONG returnValue = RegSetValueExW(currentKey, valuePath, 0, REG_BINARY,
			(const BYTE *)value, byteCount);

		return (returnValue == ERROR_SUCCESS);
	}

	//*************************************************************************
	// Method:		Read
	// Description: Gets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to read
	//	value - what the value was set to
	//
	// Return Value: true if the read was successful, false otherwise
	//*************************************************************************
	bool Registry::Read (const SiString &valuePath, int &value)
	{
		DWORD type;
		DWORD size = sizeof(DWORD);
		DWORD dest;

		LONG returnValue = RegQueryValueExW(currentKey, valuePath, NULL, &type, (BYTE *) &dest, &size);

		if (returnValue == ERROR_SUCCESS)
		{
			value = dest;
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		Read
	// Description: Gets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to read
	//	value - what the value was set to
	//
	// Return Value: true if the read was successful, false otherwise
	//*************************************************************************
	bool Registry::Read (const SiString &valuePath, DWORD64 &value)
	{
		DWORD type;
		DWORD size = sizeof(DWORD64);
		DWORD64 dest;

		LONG returnValue = RegQueryValueExW(currentKey, valuePath, NULL, &type, (BYTE *) &dest, &size);

		if (returnValue == ERROR_SUCCESS)
		{
			value = dest;
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		Read
	// Description: Gets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to read
	//	value - what the value was set to
	//
	// Return Value: true if the read was successful, false otherwise
	//*************************************************************************
	bool Registry::Read (const SiString &valuePath, unsigned long &value)
	{
		DWORD type;
		DWORD size = sizeof(DWORD);
		DWORD dest;

		LONG returnValue = RegQueryValueExW(currentKey, valuePath, NULL, &type, (BYTE *) &dest, &size);

		if (returnValue == ERROR_SUCCESS)
		{
			value = dest;
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		Read
	// Description: Gets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to read
	//	value - what the value was set to
	//
	// Return Value: true if the read was successful, false otherwise
	//*************************************************************************
	bool Registry::Read (const SiString &valuePath, SiString &value)
	{
		DWORD type;
		DWORD size = 1;
		wchar_t *buffer;

		LONG returnValue = RegQueryValueExW(currentKey, valuePath, NULL, &type, NULL, &size);
		if (returnValue != ERROR_SUCCESS)
			return false;

		int wsize = size / sizeof(wchar_t);
		buffer = new wchar_t[wsize + 1];
		if (buffer == NULL)
			return false;
		FillMemory(buffer, ((wsize + 1) * sizeof(wchar_t)), '\0');

		returnValue = RegQueryValueExW(currentKey, valuePath, NULL, &type, (BYTE *)buffer, &size);

		if (returnValue == ERROR_SUCCESS)
		{
			value = buffer;
			delete[] buffer;
			return true;
		}

		delete[] buffer;
		return false;
	}

	//*************************************************************************
	// Method:		Read
	// Description: Gets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to read
	//	value - what the value was set to
	//	bytesRead - the number of bytes read in on exit, should contain size
	//		of the buffer on entry
	//
	// Return Value: true if the read was successful, false otherwise
	//*************************************************************************
	bool Registry::Read (const SiString &valuePath, void *value, unsigned long &bytesRead)
	{
		DWORD type;

		FillMemory(value, bytesRead, 0);
		LONG returnValue = RegQueryValueExW(currentKey, valuePath, NULL, &type, (BYTE *)value, &bytesRead);

		if (returnValue == ERROR_SUCCESS)
			return true;

		bytesRead = 0;
		return false;
	}

	//*************************************************************************
	// Method:		Read
	// Description: Gets the specified value under the currently open key
	//
	// Parameters:
	//	valuePath - the path to the value to read
	//	value - what the value was set to
	//
	// Return Value: true if the read was successful, false otherwise
	//*************************************************************************
	bool Registry::Read(const SiString &valuePath, bool &value)
	{
		DWORD intValue;

		if (!this->Read(valuePath, intValue))
			return false;

		value = (intValue != 0);
		return true;
	}

	//*************************************************************************
	// Method:		GetSubKeyCount
	// Description: Gets the number of subkeys of a given key
	//
	// Parameters:
	//	rootKey - An already open key to which contains the keys to count
	//	keyPath - the path under rootKey to count
	//
	// Return Value: -1 if an error occurred, otherwise the number of 
	//	subkeys of the keys
	//*************************************************************************
	int Registry::GetSubKeyCount(HKEY rootKey, const SiString &keyPath)
	{
		HKEY thisKey;
		DWORD numSubkeys;

		// open the key
		if (RegOpenKeyExW(rootKey, keyPath, 0, KEY_ALL_ACCESS, &thisKey) != ERROR_SUCCESS)
			return -1;

		// get the number of keys under this key
		if (RegQueryInfoKeyW(thisKey, NULL, NULL, NULL, &numSubkeys, NULL,
			NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			RegCloseKey(thisKey);
			return -1;
		}

		// close the key
		RegCloseKey(thisKey);
		return numSubkeys;
	}

	//*************************************************************************
	// Method:		GetSubKeyNameAt
	// Description: Gets the name of the subkey of the given key at the specified
	//	position
	//
	// Parameters:
	//	rootKey - An already open key to which contains the keys to count
	//	keyPath - the path under rootKey to count
	//	index - the position of the subkey to get
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Registry::GetSubKeyNameAt(HKEY rootKey, const SiString &keyPath, int index, SiString &subKeyName)
	{
		subKeyName = "";
		HKEY thisKey;

		if (RegOpenKeyExW(rootKey, keyPath, 0, KEY_ALL_ACCESS, &thisKey) != ERROR_SUCCESS)
			return false;

		// get the size of the name
		DWORD size = 0;
		if (RegQueryInfoKeyW(thisKey, NULL, NULL, NULL, NULL, &size,
			NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			RegCloseKey(thisKey);
			return false;
		}

		wchar_t *subKey = new wchar_t[size + 1];
		if (subKey == NULL)
		{
			RegCloseKey(thisKey);
			return false;
		}

		// you have to cycle through starting at 0 until you get the one you want
		// according to the docs for RegEnumKey
		for (int i = 0; i <= index; i++)
		{
			// get the key name
			if (RegEnumKeyW(thisKey, i, subKey, size + 1) != ERROR_SUCCESS)
			{
				RegCloseKey(thisKey);
				int j = GetLastError();
				delete[] subKey;
				return false;
			}
		}

		// assign it to the parameter to return
		subKeyName = subKey;

		// clean up
		RegCloseKey(thisKey);
		delete[] subKey;
		return true;
	}

	//*************************************************************************
	// Method:		GetValueNames
	// Description: Gets an array of the names of the values within the specified
	//				key
	//
	// Parameters:
	//	rootKey - An already open key to which contains the keys to count
	//	keyPath - the path under rootKey to count
	//
	// Return Value: Array of value names
	//*************************************************************************
	SiArray<SiString> Registry::GetValueNames(HKEY rootKey, const SiString &keyPath)
	{
		SiArray<SiString> values;

		HKEY thisKey;

		if (RegOpenKeyExW(rootKey, keyPath, 0, KEY_ALL_ACCESS, &thisKey) != ERROR_SUCCESS)
			return values;

		// get the size of the keys largest name
		DWORD size = 0;
		if (RegQueryInfoKeyW(thisKey, NULL, NULL, NULL, NULL, NULL,
			NULL, NULL, &size, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			RegCloseKey(thisKey);
			return values;
		}

		wchar_t *value = new wchar_t[size + 1];

		// Keep enumerating until RegEnumValue fails
		for (int i = 0; ; i++)
		{
			DWORD valsize = size + 1;
			// get the value name
			if (RegEnumValueW (thisKey, i, value, &valsize, NULL, NULL,
				NULL, NULL) != ERROR_SUCCESS)
				break;

			values.Add (value);
		}

		// clean up
		RegCloseKey(thisKey);
		delete[] value;
		return values;
	}

	//*************************************************************************
	// Method:		GetSubKeys
	// Description: Gets an array of subkeys of the given key
	//
	// Parameters:
	//	rootKey - An already open key to which contains the keys to count
	//	keyPath - the path under rootKey to count
	//
	// Return Value: Array of subkeys
	//*************************************************************************
	SiArray<SiString> Registry::GetSubKeys(HKEY rootKey, const SiString &keyPath)
	{
		SiArray<SiString> subkeys;

		HKEY thisKey;

		if (RegOpenKeyExW(rootKey, keyPath, 0, KEY_ALL_ACCESS, &thisKey) != ERROR_SUCCESS)
			return subkeys;

		// get the size of the name
		DWORD size = 0;
		if (RegQueryInfoKeyW(thisKey, NULL, NULL, NULL, NULL, &size,
			NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			RegCloseKey(thisKey);
			return subkeys;
		}

		wchar_t *subKey = new wchar_t[size + 1];
		if (subKey == NULL)
		{
			RegCloseKey(thisKey);
			return subkeys;
		}

		// Keep enumerating until RegEnumKey fails
		for (int i = 0; ; i++)
		{
			// get the key name
			if (RegEnumKeyW(thisKey, i, subKey, size + 1) != ERROR_SUCCESS)
				break;

			subkeys.Add(subKey);
		}

		// clean up
		RegCloseKey(thisKey);
		delete[] subKey;
		return subkeys;
	}

	//*************************************************************************
	// Method:		GetValueSizeAndType
	// Description: Gets the size and type of a value under the current key
	//
	// Parameters:
	//	valueName - The name of the value to get the size and type of
	//	size - the number of bytes in the value
	//	type - the type of the value
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Registry::GetValueSizeAndType(const SiString &valueName, int &size, int &type)
	{
		// get the size of the name
		DWORD tempSize;
		DWORD tempType;
		if (RegQueryValueExW(currentKey, valueName, 0, &tempType, NULL, &tempSize) != ERROR_SUCCESS)
			return false;

		size = tempSize;
		type = tempType;
		return true;
	}

	//*************************************************************************
	// Method:		FindKey
	// Description: Searches for a key name
	//
	// Parameters:
	//	rootKey - An already open key to which contains the keys to count
	//	keySearchPath - the path under the root key to check for the search key
	//	keyName - the key to look for, if found contains the full path to the key on return
	//
	// Return Value: true if the key exists, false otherwise
	//*************************************************************************
	bool Registry::FindKey(HKEY rootKey, const SiString &keySearchPath, SiString &keyName)
	{
		DWORD numSubkeys;
		DWORD maxSubkeyLength;
		DWORD numValues = 0;
		DWORD maxValueLength;
		unsigned int i;
		unsigned int j;
		HKEY thisKey;
		bool bFoundIt = false;

		// open the key
		if (RegOpenKeyExW(rootKey, keySearchPath, 0, KEY_ALL_ACCESS, &thisKey) != ERROR_SUCCESS)
		{
			return false;
		}

		// get the number of keys under this key
		if (RegQueryInfoKeyW(thisKey, NULL, NULL, NULL, &numSubkeys, &maxSubkeyLength,
			NULL, NULL, &maxValueLength, NULL, NULL, NULL) != ERROR_SUCCESS)
		{
			RegCloseKey(thisKey);
			return false;
		}


		// get room for the key names
		wchar_t **subkeyList = new wchar_t *[numSubkeys];
		if (subkeyList == NULL)
		{
			RegCloseKey(thisKey);
			return false;
		}
		FillMemory(subkeyList, sizeof(wchar_t *) * numSubkeys, 0);

		// Look at the subkeys
		for (i = 0; i < numSubkeys; i++)
		{
			subkeyList[i] = new wchar_t[maxSubkeyLength + 1];
			if (subkeyList[i] == NULL)
			{
				for (j = 0; j < numSubkeys; j++)
					if (subkeyList[j] != NULL) delete[] subkeyList[j];
				delete[] subkeyList;
				RegCloseKey(thisKey);
				return false;
			}

			if (RegEnumKeyW(thisKey, i, subkeyList[i], maxSubkeyLength + 1) != ERROR_SUCCESS)
			{
				for (j = 0; j < numSubkeys; j++)
					if (subkeyList[j] != NULL) delete[] subkeyList[j];
				delete[] subkeyList;
				RegCloseKey(thisKey);
				return false;
			}
			if (keyName == subkeyList[i])
			{
				SiString tmp = keySearchPath;
				tmp += "\\";
				tmp += keyName;
				keyName = tmp;
				bFoundIt = true;
				break;
			}

		}

		for (i = 0; !bFoundIt && i < numSubkeys; i++)
		{
			SiString subPath = keySearchPath;
			subPath += "\\";
			subPath += subkeyList[i];

			// now look at the subkey's subkeys
			if (FindKey(rootKey, subPath, keyName))
			{
				//found it, let's bail.
				for (j = 0; j < numSubkeys; j++)
					if (subkeyList[j] != NULL) delete[] subkeyList[j];
				delete[] subkeyList;
				RegCloseKey(thisKey);
				return true;
			}
		}

		// clean up
		for (j = 0; j < numSubkeys; j++)
			if (subkeyList[j] != NULL) delete[] subkeyList[j];
		delete[] subkeyList;
		RegCloseKey(thisKey);

		return bFoundIt;
	}
}; // namespace