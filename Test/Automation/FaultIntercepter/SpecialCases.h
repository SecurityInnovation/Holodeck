///*************************************************************************
/// Center for Information Assurance - Florida Institute Of Technology
///
/// FILE:		SpecialCases.h
///
/// DESCRIPTION: Contains definitions for hard coded values for functions 
///              that cause TestFaults to crash. 
///=========================================================================
/// Modification History
///
/// Date				Name			Purpose
/// -----------    ----------- ------------------------------------------
/// 03/10/2003     Gavin Menezes	File Created.
///*************************************************************************

#pragma once

#include <windows.h>
using namespace System;

__gc class SpecialCases
	{
	public:
		

		SpecialCases(void);
		~SpecialCases(void);
		//list of special case functions (currently 7)
		DWORD Sp_HeapAlloc();
		DWORD Sp_ReadFileEx();
		DWORD Sp_WriteFileEx();
		DWORD Sp_UnlockFileEx();
		DWORD Sp_FindNextFileA();
		DWORD Sp_FindNextFileW();
		DWORD Sp_FindFirstFileW();
		DWORD Sp_LoadModule();
		
		DWORD getSpecialCaseFunctionReturnValue(String *SpecialFunctionName);
	
	};
