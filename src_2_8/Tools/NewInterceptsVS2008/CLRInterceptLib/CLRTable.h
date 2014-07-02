//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTable.h
//
// DESCRIPTION: Contains classes to manage tables in the CLR metadata.
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

#ifndef __CLRTABLE_H__
#define __CLRTABLE_H__

#include <windows.h>
#include "CLRMetadataDefs.h"
#include "CLRTableRow.h"
#include "SiArray.h"


#define CLR_METADATA_TABLE_COUNT 64


struct CLRCodedTokenTableList
{
	BYTE bTableCount;
	BYTE bBitCount;
	BYTE pbTables[20];
};


struct CLRTableColDef
{
	BYTE bType;
	BYTE bColOffset;
	BYTE bColSize;
};


struct CLRTableDef
{
	CLRTableColDef* ptcCols;
	BYTE bColCount;
	BYTE bKeyIndex;
};


class CLRMetadata;

class CLRTable
{
	SiUtils::SiArray<CLRTableRow> m_vtrRows;
	CLRMetadata* m_mdMetadata;
	CLRTableColDef* m_ptcCols;
	DWORD m_dwColCount, m_dwIndex;

public:
	CLRTable();
	~CLRTable();

	CLRTableRow& operator[](const int index);

	DWORD add(CLRTableRow& trRow);
	void remove(DWORD index);
	void sizeHint(const DWORD dwSize);
	DWORD getRowCount();

	CLRMetadata* getMetadata();
	void setMetadata(CLRMetadata* mdMetadata);

	CLRTableColDef* getColDefs();
	DWORD getColCount();
	void setColDefs(const CLRTableColDef* ptcCols, const DWORD dwColCount, DWORD dwIndex);
	DWORD getIndex() { return m_dwIndex; }

	void printTable(FILE* fp);

	void sort();
};


// Global variables containing metadata schema
extern CLRCodedTokenTableList* g_pcttCodedTokenTables[];
extern CLRTableDef g_ptdTableFormat[CLR_METADATA_TABLE_COUNT];
extern char* g_pszTableNames[CLR_METADATA_TABLE_COUNT];

#endif
