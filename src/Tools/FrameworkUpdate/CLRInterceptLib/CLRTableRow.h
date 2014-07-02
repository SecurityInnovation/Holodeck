//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTableRow.h
//
// DESCRIPTION: Contains classes to manage single table rows in the
//				CLR metadata.
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 31 Mar 2003	103	 R. Wagner	 File created.
//*************************************************************************

#ifndef __CLRTABLEROW_H__
#define __CLRTABLEROW_H__

#include <windows.h>
#include <stdio.h>
#include "CLRTranslationTable.h"
#include "SiString.h"

#define CLR_METADATA_TABLE_MAX_COLUMNS 9


class CLRTable;
class CLRMetadata;

class CLRTableRow
{
	CLRTable* m_ptTable;
	DWORD m_pdwColData[CLR_METADATA_TABLE_MAX_COLUMNS];

public:
	CLRTableRow();
	CLRTableRow(CLRTable* ptTable);
	CLRTableRow(const CLRTableRow& trRow);

	CLRTableRow& operator=(const CLRTableRow& trRow);

	CLRTable& getTable();

	DWORD& operator[](const int index);
	SiUtils::SiString getString(int index);

	CLRTable* getRefTable(int index);
	int getRefTableIndex(int index);
	CLRTableRow* getRefRow(int index);
	int getRefRowIndex(int index);
	DWORD getToken(int index);
	const BYTE* getBlobData(int index);
	DWORD getBlobLength(int index);
	const BYTE* getGuid(int index);

	bool setString(int index, const SiUtils::SiString& sStr);
	bool setBlob(int index, const BYTE* pbData, DWORD dwSize);
	bool setRefTable(int iColIndex, int dwTableIndex, int dwRowIndex);
	bool setToken(int iColIndex, DWORD dwToken);
	bool setGuid(int index, const BYTE* guid);

	CLRTableRow copy(CLRMetadata& metadata, int tableIndex);
	void replaceTokens(CLRTranslationTable& table);

	void printTableRow(FILE* fp, int index);
};


#endif
