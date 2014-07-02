//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Symbols.cpp
//
// DESCRIPTION: Contains implementation of the class Symbols.
//				Class is not thread safe.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 APR 2004		 Ady K		 File created.
//*************************************************************************
#include "Symbols.h"
#include <imagehlp.h>
#include <psapi.h>

typedef BOOL (__stdcall *EnumSymbolsFunc)(HANDLE, ULONG64, PCSTR, PSYM_ENUMERATESYMBOLS_CALLBACK, PVOID);

namespace SiUtils
{
	SiArray<SiString> * processSymbolList;
	char enumModName[1024];
	HANDLE hEnumerateProcess;

	//*************************************************************************
	// Method:		Symbols
	// Description: Constructor for the symbols class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Symbols::Symbols()
	{
		pathToDbgHelp = "dbghelp.dll";
	}
	
	//*************************************************************************
	// Method:		EnumerateSymbolsProc
	// Description: Callback method used by SymEnumSymbols
	//
	// Parameters:
	//	SEE MSDN link for SymEnumSymbols 4th parameter
	//
	// Return Value: SEE MSDN link for SymEnumSymbols 4th parameter
	//*************************************************************************
	BOOL CALLBACK EnumerateSymbolsProc(PSYMBOL_INFO pSymInfo, ULONG SymbolSize, PVOID UserContext)
	{
		if ((pSymInfo->Flags | SYMFLAG_FUNCTION) == SYMFLAG_FUNCTION)
		{
			if (processSymbolList == NULL)
				processSymbolList = new SiArray<SiString>();

			SiString item = SiString (enumModName);
			item += " ";
			item += SiString (pSymInfo->Name);
			processSymbolList->Add (SiString (item));
		}
		return TRUE;
	}

	//*************************************************************************
	// Method:		SetPathToDbgHelpDll
	// Description: Sets the path to the dbghelp.dll file to use
	//
	// Parameters:
	//	path - the path to the file.  If the path includes 'dbghelp.dll' at the
	//		   end it will use it as is.  If it does not have 'dbghelp.dll' at
	//		   the end it will append it
	//
	// Return Value: None
	//*************************************************************************
	void Symbols::SetPathToDbgHelpDll(const SiString &path)
	{
		SiString lowerPath = path;
		lowerPath.MakeLower();

		SiString dllName = "dbghelp.dll";

		// see if it already ends with 
		if (lowerPath.FindLast(dllName) == lowerPath.GetLength() - dllName.GetLength())
		{
			pathToDbgHelp = path;
		}
		// we need to append it.  See if we already end with a path char
		else if (lowerPath.FindLast('\\') == lowerPath.GetLength() - 1)
		{
			pathToDbgHelp = path;
			pathToDbgHelp += dllName;
		}
		else
		{
			pathToDbgHelp = path;
			pathToDbgHelp += "\\";
			pathToDbgHelp += dllName;
		}
	}

	//*************************************************************************
	// Method:		GetSymbolsForProcess
	// Description: Gets the symbols for a module in a process
	//
	// Parameters:
	//	hProcess - handle to the process for which to get symbols (process must
	//			   NOT be suspended)
	//	moduleName - the name of the module for which the symbols are needed
	//
	// Return Value: An array of symbols, NULL if none
	//*************************************************************************
	SiArray<SiString>* Symbols::GetSymbolsForProcess(HANDLE hProcess, char* moduleName)
	{
		if (processSymbolList != NULL)
			processSymbolList->Clear();

		hEnumerateProcess = hProcess;
		strcpy (enumModName, moduleName);

		SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);

		if (!SymInitialize(hEnumerateProcess, NULL, TRUE))
			return NULL;
		
		HMODULE dbgHelpModule = LoadLibrary(pathToDbgHelp);
		if (!dbgHelpModule)
			return NULL;

		EnumSymbolsFunc symEnumSymbolsFunc = (EnumSymbolsFunc)GetProcAddress(dbgHelpModule, "SymEnumSymbols");
		if (!symEnumSymbolsFunc)
			return NULL;

		symEnumSymbolsFunc(hEnumerateProcess, 0, moduleName, EnumerateSymbolsProc, NULL);

		SymCleanup (hProcess);
		
		if (processSymbolList != NULL && processSymbolList->GetLength() > 0)
			return processSymbolList;
		else
			return NULL;
	}


}
