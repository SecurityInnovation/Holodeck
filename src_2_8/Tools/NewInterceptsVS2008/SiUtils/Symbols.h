//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Symbols.h
//
// DESCRIPTION: Contains definition for the utility class Symbols
//				Class is not thread safe.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 APR 2004		 Ady K		 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "SiString.h"
#include "SiArray.h"

namespace SiUtils
{
	extern SiArray<SiString> * processSymbolList;
	extern char enumModName[1024];
	extern HANDLE hEnumerateProcess;

	class Symbols
	{
	public:
		Symbols();
		~Symbols();

		void SetPathToDbgHelpDll(const SiString &path);

		SiArray<SiString>* GetSymbolsForProcess(HANDLE hProcess, char* moduleName);
		SiArray<SiString> * processSymbolListPart;
	private:
		SiString pathToDbgHelp;
	};
}
