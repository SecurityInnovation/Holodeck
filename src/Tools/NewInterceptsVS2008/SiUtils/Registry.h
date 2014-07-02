//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Registry.h
//
// DESCRIPTION: Contains definition for the utility class Registry
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "SiString.h"
#include "SiArray.h"

namespace SiUtils
{
	//*************************************************************************
	// Combines common functionality of the registry into a class
	//*************************************************************************
	class Registry
	{
	protected:
		HKEY rootKey;
		HKEY currentKey;

	public:
		Registry(HKEY rootKey = HKEY_LOCAL_MACHINE);
		~Registry();

		bool KeyExists(const SiString &keyPath);
		bool ValueExists(const SiString &valuePath);

		void SetRootKey(HKEY rootKey) { this->rootKey = rootKey; };
		HKEY GetRootKey() { return this->rootKey; };
		HKEY GetCurrentKey() { return this->currentKey; };

		bool CreateKey(HKEY rootKey, const SiString &keyPath);
		bool OpenKey(HKEY rootKey, const SiString &keyPath);
		bool OpenKey(HKEY rootKey, const SiString &keyPath, unsigned long permissions);
		void Close();

		bool DeleteKey(HKEY rootKey, const SiString &keyPath);
		bool DeleteValue(const SiString &valuePath);
		bool DeleteKeyRecursively(HKEY rootKey, const SiString &keyPath);

		bool Write (const SiString &valuePath, int value);
		bool Write (const SiString &valuePath, unsigned long value);
		bool Write (const SiString &valuePath, const SiString &value);
		bool Write (const SiString &valuePath, const char *value, long byteCount);
		bool Write (const SiString &valuePath, void *value, unsigned long byteCount);
		bool Write (const SiString &valuePath, DWORD64 value);

		bool Read (const SiString &valuePath, int &value);
		bool Read (const SiString &valuePath, unsigned long &value);
		bool Read (const SiString &valuePath, SiString &value);
		bool Read (const SiString &valuePath, void *value, unsigned long &bytesRead);
		bool Read (const SiString &valuePath, bool &value);
		bool Read (const SiString &valuePath, DWORD64 &value);

		int GetSubKeyCount(HKEY rootKey, const SiString &keyPath);
		bool GetSubKeyNameAt(HKEY rootKey, const SiString &keyPath, int index, SiString &subKeyName);

		SiArray<SiString> GetValueNames(HKEY rootKey, const SiString &keyPath);
		SiArray<SiString> GetSubKeys(HKEY rootKey, const SiString &keyPath);

		bool GetValueSizeAndType(const SiString &valueName, int &size, int &type);
		bool FindKey(HKEY rootKey, const SiString &keySearchPath, SiString &keyName);
	};
}; // namespace
