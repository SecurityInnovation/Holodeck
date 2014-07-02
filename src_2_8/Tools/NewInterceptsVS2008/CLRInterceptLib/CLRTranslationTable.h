//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTranslationTable.h
//
// DESCRIPTION: Contains classes to manage IL token translation tables
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 6 Feb 2004	   	 R. Wagner	 File created.
//*************************************************************************

#ifndef __CLRTRANSLATIONTABLE_H__
#define __CLRTRANSLATIONTABLE_H__

#include "SiArray.h"


struct CLRTranslation
{
	DWORD dwOldToken;
	DWORD dwNewToken;
};


class CLRTranslationTable
{
	SiUtils::SiArray<CLRTranslation> list;
	CLRTranslationTable* pChainedTable;

public:
	CLRTranslationTable();

	void Add(DWORD dwOldToken, DWORD dwNewToken);
	DWORD Translate(DWORD dwToken);
	DWORD ReverseLookup(DWORD dwToken);
	void SetChainedTable(CLRTranslationTable *pTable);
};


#endif
