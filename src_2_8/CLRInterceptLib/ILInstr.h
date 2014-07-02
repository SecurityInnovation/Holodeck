//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		ILInstr.h
//
// DESCRIPTION: Contains classes to manage .NET IL instructions
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the IL instruction set.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Jan 2003		 R. Wagner	 File created.
//*************************************************************************

#ifndef __ILINSTR_H__
#define __ILINSTR_H__

#include <windows.h>
#include "CLRTranslationTable.h"


bool CLRReplaceTokensInIL(BYTE* pCode, DWORD dwLen, CLRTranslationTable& table);


#endif
