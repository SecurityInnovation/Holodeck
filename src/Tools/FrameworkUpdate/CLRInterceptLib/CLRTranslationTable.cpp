//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTranslationTable.cpp
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

#include "CLRTranslationTable.h"


//*************************************************************************
// Method:	 CLRTranslationTable::CLRTranslationTable
// Description: Default constructor for the CLRTranslationTable type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRTranslationTable::CLRTranslationTable()
{
	pChainedTable = NULL;
}


//*************************************************************************
// Method:	 CLRTranslationTable::Add
// Description: Adds a translation to the table
// Parameters:
//	dwOldToken - Token value to replace
//  dwNewToken - New token value
//
// Return Value: None
//*************************************************************************
void CLRTranslationTable::Add(DWORD dwOldToken, DWORD dwNewToken)
{
	// Check to see if a tranlsation has already been added for this token
	for (DWORD i=0; i<list.GetLength(); i++)
	{
		if (dwOldToken == list[i].dwOldToken)
		{
			list[i].dwNewToken = dwNewToken;
			return;
		}
	}

	CLRTranslation trans;
	trans.dwOldToken = dwOldToken;
	trans.dwNewToken = dwNewToken;
	list.Add(trans);
}


//*************************************************************************
// Method:	 CLRTranslationTable::Translate
// Description: Performs a translation lookup on a token
// Parameters:
//	dwToken - Token to look up in translation table
//
// Return Value: Translated token
//*************************************************************************
DWORD CLRTranslationTable::Translate(DWORD dwToken)
{
	for (DWORD i=0; i<list.GetLength(); i++)
	{
		if (dwToken == list[i].dwOldToken)
		{
			if (pChainedTable)
				return pChainedTable->Translate(list[i].dwNewToken);
			return list[i].dwNewToken;
		}
	}
	if (pChainedTable)
		return pChainedTable->Translate(dwToken);
	return dwToken;
}


//*************************************************************************
// Method:	 CLRTranslationTable::ReverseLookup
// Description: Performs a reverse translation lookup on a token
// Parameters:
//	dwToken - Token to look up in translation table
//
// Return Value: Translated token
//*************************************************************************
DWORD CLRTranslationTable::ReverseLookup(DWORD dwToken)
{
	if (pChainedTable)
		dwToken = pChainedTable->ReverseLookup(dwToken);

	for (DWORD i=0; i<list.GetLength(); i++)
	{
		if (dwToken == list[i].dwNewToken)
			return list[i].dwOldToken;
	}
	return dwToken;
}


//*************************************************************************
// Method:	 CLRTranslationTable::SetChainedTable
// Description: Sets up a second translation table.  This second table
//				will be applied after the current table.
// Parameters:
//	pTable - Second translation table (NULL for none)
//
// Return Value: None
//*************************************************************************
void CLRTranslationTable::SetChainedTable(CLRTranslationTable* pTable)
{
	pChainedTable = pTable;
}
