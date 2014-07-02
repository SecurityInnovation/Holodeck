//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTableRow.cpp
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

#include <windows.h>
#include "CLRMetadata.h"
#include "CLRTable.h"
#include "CLRTableRow.h"
#include "CLRSignature.h"

using namespace SiUtils;


//*************************************************************************
// Method:	 CLRTableRow::CLRTableRow
// Description: Default constructor for the CLRTableRow type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRTableRow::CLRTableRow()
{
	m_ptTable = NULL;
}


//*************************************************************************
// Method:	 CLRTableRow::CLRTableRow
// Description: Constructor for the CLRTableRow type that takes the table
//				containing this row as a parameter.
// Parameters:
//	ptTable - Pointer to table that contains this row
//
// Return Value: None
//*************************************************************************
CLRTableRow::CLRTableRow(CLRTable* ptTable)
{
	m_ptTable = ptTable;
}


//*************************************************************************
// Method:	 CLRTableRow::CLRTableRow
// Description: Copy constructor for the CLRTableRow type.
// Parameters:
//	trRow - Table row to copy
//
// Return Value: None
//*************************************************************************
CLRTableRow::CLRTableRow(const CLRTableRow& trRow)
{
	m_ptTable = trRow.m_ptTable;
	memcpy(m_pdwColData, trRow.m_pdwColData, CLR_METADATA_TABLE_MAX_COLUMNS * sizeof(DWORD));
}


//*************************************************************************
// Method:	 CLRTableRow::CLRTableRow
// Description: Assignment operator for the CLRTableRow type.
// Parameters:
//	trRow - New table row
//
// Return Value: Reference to the value assigned
//*************************************************************************
CLRTableRow& CLRTableRow::operator=(const CLRTableRow& trRow)
{
	m_ptTable = trRow.m_ptTable;
	memcpy(m_pdwColData, trRow.m_pdwColData, CLR_METADATA_TABLE_MAX_COLUMNS * sizeof(DWORD));
	return *this;
}


//*************************************************************************
// Method:	 CLRTableRow::getTable
// Description: Gets the table that contains this row
// Parameters:
//	None
//
// Return Value: Reference to the table that contains this row
//*************************************************************************
CLRTable& CLRTableRow::getTable()
{
	return *m_ptTable;
}


//*************************************************************************
// Method:	 CLRTableRow::operator[]
// Description: Gets the integer value at a given column index
// Parameters:
//	index - Column index
//
// Return Value: Integer value at the column
//*************************************************************************
DWORD& CLRTableRow::operator[](const int index)
{
	return m_pdwColData[index];
}


//*************************************************************************
// Method:	 CLRTableRow::getString
// Description: Interprets the given column as a string
// Parameters:
//	index - Column index
//
// Return Value: String value at the column
//*************************************************************************
SiString CLRTableRow::getString(int index)
{
	return m_ptTable->getMetadata()->getString(m_pdwColData[index]);
}


//*************************************************************************
// Method:	 CLRTableRow::getRefTable
// Description: Interprets the given column as a coded token and returns
//				the referenced table
// Parameters:
//	index - Column index
//
// Return Value: Pointer to the table referenced by the coded token, or
//		NULL if it is not a valid coded token
//*************************************************************************
CLRTable* CLRTableRow::getRefTable(int index)
{
	int iTableIndex = getRefTableIndex(index);
	if (iTableIndex == -1)
		return NULL;
	return &m_ptTable->getMetadata()->getTable(iTableIndex);
}


//*************************************************************************
// Method:	 CLRTableRow::getRefTableIndex
// Description: Interprets the given column as a coded token and returns
//				the referenced table index
// Parameters:
//	index - Column index
//
// Return Value: Index of the table referenced by the coded token, or
//		-1 if it is not a valid coded token
//*************************************************************************
int CLRTableRow::getRefTableIndex(int index)
{
	BYTE bColType = m_ptTable->getColDefs()[index].bType;
	DWORD dwColValue = m_pdwColData[index];

	// If the type is a reference to a specific table, we just have an RID
	if (bColType < CLR_CODED_TOKEN_BASE)
		return bColType;

	// Should be a coded token
	if (bColType > CLR_COL_CODED_TOKEN_MAX)
		return -1;

	// The type of the coded token is in the least significant bits.  Extract them.
	DWORD dwCodedTokenIndex = bColType - CLR_CODED_TOKEN_BASE;
	DWORD dwTokenTypeMask = (1 << g_pcttCodedTokenTables[dwCodedTokenIndex]->bBitCount) - 1;
	DWORD dwTokenType = dwColValue & dwTokenTypeMask;

	// Validate the type of the coded token
	if (dwTokenType >= g_pcttCodedTokenTables[dwCodedTokenIndex]->bTableCount)
		return -1;

	// Return the index of the table for this type
	return g_pcttCodedTokenTables[dwCodedTokenIndex]->pbTables[dwTokenType];
}


//*************************************************************************
// Method:	 CLRTableRow::getRefRow
// Description: Interprets the given column as a coded token and returns
//				the referenced table row
// Parameters:
//	index - Column index
//
// Return Value: Pointer to the table row referenced by the coded token, or
//		NULL if it is not a valid coded token
//*************************************************************************
CLRTableRow* CLRTableRow::getRefRow(int index)
{
	CLRTable* ptTable = getRefTable(index);
	int iRowIndex = getRefRowIndex(index);
	if ((!ptTable) || (iRowIndex == -1))
		return NULL;
	return &((*ptTable)[iRowIndex]);
}


//*************************************************************************
// Method:	 CLRTableRow::getRefRowIndex
// Description: Interprets the given column as a coded token and returns
//				the referenced table row index
// Parameters:
//	index - Column index
//
// Return Value: Index of the table row referenced by the coded token, or
//		-1 if it is not a valid coded token
//*************************************************************************
int CLRTableRow::getRefRowIndex(int index)
{
	BYTE bColType = m_ptTable->getColDefs()[index].bType;
	DWORD dwColValue = m_pdwColData[index];

	if (bColType < CLR_CODED_TOKEN_BASE)
	{
		// Type is a reference to a specific table, we just have an RID.  Since
		// RIDs are one-based, subtract one to make it a zero-based index.
		return (int)dwColValue - 1;
	}

	// Should be a coded token
	if (bColType > CLR_COL_CODED_TOKEN_MAX)
		return -1;

	// The type of the coded token is in the least significant bits.  To get the RID of the referenced
	// row, the coded token needs to be shifted right past the type information.  The number of bits
	// to shift is equal to the number of bits of the type information, which is stored as bBitCount in
	// the coded token schema tables.
	DWORD dwCodedTokenIndex = bColType - CLR_CODED_TOKEN_BASE;
	DWORD dwTokenRID = dwColValue >> g_pcttCodedTokenTables[dwCodedTokenIndex]->bBitCount;

	// RID is a one-based index.  Subtract one and return to give a zero-based index.  If the RID
	// was zero (a null value), this will return -1, our error code.
	return (int)dwTokenRID - 1;
}


//*************************************************************************
// Method:	 CLRTableRow::getBlobData
// Description: Interprets the given column as a blob and returns the data
// Parameters:
//	index - Column index
//
// Return Value: Blob data
//*************************************************************************
const BYTE* CLRTableRow::getBlobData(int index)
{
	const CLRBlobHeapEntry* blob = m_ptTable->getMetadata()->getBlob(m_pdwColData[index]);
	return blob->pbData;
}


//*************************************************************************
// Method:	 CLRTableRow::getBlobLength
// Description: Interprets the given column as a blob and returns the length
// Parameters:
//	index - Column index
//
// Return Value: Blob length
//*************************************************************************
DWORD CLRTableRow::getBlobLength(int index)
{
	const CLRBlobHeapEntry* blob = m_ptTable->getMetadata()->getBlob(m_pdwColData[index]);
	return blob->dwSize;
}


//*************************************************************************
// Method:	 CLRTableRow::getGuid
// Description: Interprets the given column as a GUID
// Parameters:
//	index - Column index
//
// Return Value: GUID data
//*************************************************************************
const BYTE* CLRTableRow::getGuid(int index)
{
	const CLRGuidHeapEntry* guid = m_ptTable->getMetadata()->getGuid(m_pdwColData[index]);
	return guid->pbGuid;
}


//*************************************************************************
// Method:	 CLRTableRow::getToken
// Description: Interprets the given column as a token
// Parameters:
//	index - Column index
//
// Return Value: Token
//*************************************************************************
DWORD CLRTableRow::getToken(int index)
{
	return (getRefTableIndex(index) << 24) | (getRefRowIndex(index) + 1); // RIDs are one indexed
}


//*************************************************************************
// Method:	 CLRTableRow::setString
// Description: Sets a column as a reference to a new string value
// Parameters:
//	index - Column index
//  sStr - String value for column
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTableRow::setString(int index, const SiString& sStr)
{
	if (sStr == "")
	{
		// Use zero for an empty string
		m_pdwColData[index] = 0;
		return false;
	}

	DWORD dwStrIndex = m_ptTable->getMetadata()->addString(sStr);
	if (dwStrIndex == 0)
		return false;

	m_pdwColData[index] = dwStrIndex;
	return true;
}


//*************************************************************************
// Method:	 CLRTableRow::setBlob
// Description: Sets a column as a reference to a new blob object
// Parameters:
//	index - Column index
//  pbData - Data for the column's blob object
//  dwSize - Size of data in bytes for the column's blob object
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTableRow::setBlob(int index, const BYTE* pbData, DWORD dwSize)
{
	DWORD dwBlobIndex = m_ptTable->getMetadata()->addBlob(pbData, dwSize);
	if (dwBlobIndex == 0)
		return false;

	m_pdwColData[index] = dwBlobIndex;
	return true;
}


//*************************************************************************
// Method:	 CLRTableRow::setGuid
// Description: Sets a column as a reference to a new GUID object
// Parameters:
//	index - Column index
//  guid - Data for the column's GUID object
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTableRow::setGuid(int index, const BYTE* guid)
{
	DWORD dwGuidIndex = m_ptTable->getMetadata()->addGuid(guid);
	if (dwGuidIndex == 0)
		return false;

	m_pdwColData[index] = dwGuidIndex;
	return true;
}


//*************************************************************************
// Method:	 CLRTableRow::setRefTable
// Description: Sets a column as a table row reference
// Parameters:
//	iColIndex - Column index
//  dwTableIndex - Index of table for column
//  dwRowIndex - Index of row within the table for column
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTableRow::setRefTable(int iColIndex, int dwTableIndex, int dwRowIndex)
{
	BYTE bColType = m_ptTable->getColDefs()[iColIndex].bType;

	if (bColType < CLR_CODED_TOKEN_BASE)
	{
		// Type is a reference to a specific table, we just have an RID.
		if (bColType != dwTableIndex)
		{
			// Wrong table
			return false;
		}
		m_pdwColData[iColIndex] = dwRowIndex + 1; // RIDs are one-indexed
		return true;
	}

	// Should be a coded token
	if (bColType > CLR_COL_CODED_TOKEN_MAX)
		return false;

	// Try to find out which token ID goes with this table
	DWORD dwCodedTokenIndex = bColType - CLR_CODED_TOKEN_BASE;
	for (DWORD i=0; i<g_pcttCodedTokenTables[dwCodedTokenIndex]->bTableCount; i++)
	{
		if (g_pcttCodedTokenTables[dwCodedTokenIndex]->pbTables[i] == dwTableIndex)
		{
			// Found the table.  Coded token is made up of an RID in the high bits (one-indexed), and
			// the token type (i) in the lower bits.
			DWORD dwCodedToken = ((dwRowIndex + 1) << g_pcttCodedTokenTables[dwCodedTokenIndex]->bBitCount) | i;
			m_pdwColData[iColIndex] = dwCodedToken;
			return true;
		}
	}
	// Invalid table
	return false;
}


//*************************************************************************
// Method:	 CLRTableRow::setToken
// Description: Sets a column as a table row reference
// Parameters:
//	iColIndex - Column index
//  dwToken - Token
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTableRow::setToken(int iColIndex, DWORD dwToken)
{
	return setRefTable(iColIndex, CLR_TOKEN_TYPE(dwToken), CLR_TOKEN_RID(dwToken) - 1);
}


//*************************************************************************
// Method:	 CLRTableRow::copy
// Description: Copies a row into a new metadata stream
// Parameters:
//	metadata - Destination metadata
//  tableIndex - Table to copy into
//
// Return Value: Table row
//*************************************************************************
CLRTableRow CLRTableRow::copy(CLRMetadata& metadata, int tableIndex)
{
	CLRTableRow row(&metadata.getTable(tableIndex));
	for (DWORD i=0; i<m_ptTable->getColCount(); i++)
	{
		if (m_ptTable->getColDefs()[i].bType == CLR_COL_STRING)
			row.setString(i, getString(i));
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_GUID)
			row.setGuid(i, getGuid(i));
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_BLOB)
			row.setBlob(i, getBlobData(i), getBlobLength(i));
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_SIGNATURE)
			row.setBlob(i, getBlobData(i), getBlobLength(i));
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_SIGNATURE_NOCALLCONV)
			row.setBlob(i, getBlobData(i), getBlobLength(i));
		else
			row[i] = (*this)[i];
	}
	return row;
}


//*************************************************************************
// Method:	 CLRTableRow::replaceTokens
// Description: Replaces tokens in a table row
// Parameters:
//	table - Translation table to use
//
// Return Value: None
//*************************************************************************
void CLRTableRow::replaceTokens(CLRTranslationTable& table)
{
	for (DWORD i=0; i<m_ptTable->getColCount(); i++)
	{
		if (m_ptTable->getColDefs()[i].bType <= CLR_COL_CODED_TOKEN_MAX)
		{
			// Column is a reference to a table, so run it through the translation table
			setToken(i, table.Translate(getToken(i)));
		}
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_SIGNATURE)
		{
			// Column refers to a signature.  Replace tokens in the signature.
			CLRSignature sig;
			const BYTE* pOldData = getBlobData(i);
			sig.replaceTokens(pOldData, table);
			if ((sig.getSize() != getBlobLength(i)) || (memcmp(pOldData, sig.getData(), sig.getSize()) != 0))
				setBlob(i, sig.getData(), sig.getSize());
		}
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_SIGNATURE_NOCALLCONV)
		{
			// Column refers to a signature.  Replace tokens in the signature.
			CLRTypeSignature sig;
			const BYTE* pOldData = getBlobData(i);
			BYTE* pbSigData = NULL;
			DWORD dwSigLen = 0;
			DWORD dwSigMaxLen = 0;
			sig.replaceTokens(pOldData, pbSigData, dwSigLen, dwSigMaxLen, table);
			if ((dwSigLen != getBlobLength(i)) || (memcmp(pOldData, pbSigData, dwSigLen) != 0))
				setBlob(i, pbSigData, dwSigLen);
			if (pbSigData)
				delete[] pbSigData;
		}
		else if (m_ptTable->getColDefs()[i].bType == CLR_COL_TOKEN)
			m_pdwColData[i] = table.Translate(m_pdwColData[i]);
	}
}


void CLRTableRow::printTableRow(FILE* fp, int index)
{
	if (!m_ptTable)
		return;

	fprintf(fp, "%d: ", index);

	for (int i=0; i<(int)m_ptTable->getColCount(); i++)
	{
		if (m_ptTable->getColDefs()[i].bType == CLR_COL_STRING)
			fprintf(fp, "%s", (char*)getString(i));
		else if (getRefTableIndex(i) != -1)
			fprintf(fp, "%s.%d", g_pszTableNames[getRefTableIndex(i)], getRefRowIndex(i));
		else
			fprintf(fp, "%d", m_pdwColData[i]);

		if ((i + 1) < (int)m_ptTable->getColCount())
			fprintf(fp, ", ");
	}
	fprintf(fp, "\n");
}
