//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRMetadata.cpp
//
// DESCRIPTION: Contains classes to manage the CLR (Common Language Runtime)
//				metadata for an executable based on the .NET runtime.
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Mar 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include "CLRMetadata.h"
#include "PEFile.h"

using namespace SiUtils;


//*************************************************************************
// Method:	 CLRMetadata::CLRMetadata
// Description: Default constructor for the CLRMetadata type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRMetadata::CLRMetadata()
{
	m_pszVersionString = NULL;
	m_pFile = NULL;

	// Associate each table with this CLRMetadata instance
	for (DWORD i=0; i<CLR_METADATA_TABLE_COUNT; i++)
		m_ptTable[i].setMetadata(this);
}


//*************************************************************************
// Method:	 CLRMetadata::~CLRMetadata
// Description: Destructor for the CLRMetadata type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRMetadata::~CLRMetadata()
{
	// Free the version string if it has been allocated
	if (m_pszVersionString)
	{
		delete[] m_pszVersionString;
		m_pszVersionString = NULL;
	}

	// Free blob heap entries
	for (DWORD i=0; i<m_bhBlobHeap.GetLength(); i++)
	{
		if (m_bhBlobHeap[i].pbData != NULL)
			delete[] m_bhBlobHeap[i].pbData;
	}
	m_bhBlobHeap.Clear();

	// Free user string heap entries
	for (DWORD i=0; i<m_bhUserStringHeap.GetLength(); i++)
	{
		if (m_bhUserStringHeap[i].pbData != NULL)
			delete[] m_bhUserStringHeap[i].pbData;
	}
	m_bhUserStringHeap.Clear();
}


//*************************************************************************
// Method:	 CLRMetadata::readCompressedInt
// Description: Reads a compressed 32-bit integer out of the data stream.
//				The most significant 2 bits specify how many bytes to use
//				to encode the integer, with the data in big endian format.
// Parameters:
//	pbData - Data stream to read from
//  dwOffset - Offset in the data stream to read from.  Offset will be
//		updated according to how many bytes are read
//
// Return Value: Integer that was decoded
//*************************************************************************
DWORD CLRMetadata::readCompressedInt(const BYTE* pbData, DWORD& dwOffset)
{
	DWORD dwValue = pbData[dwOffset++];
	if (dwValue < 0x80)
		return dwValue;
	else if (dwValue < 0xc0)
	{
		// 16-bit value
		dwValue = (dwValue << 8) | pbData[dwOffset++];
		dwValue &= 0x3fff;
	}
	else
	{
		// 32-bit value
		dwValue = (dwValue << 8) | pbData[dwOffset++];
		dwValue = (dwValue << 8) | pbData[dwOffset++];
		dwValue = (dwValue << 8) | pbData[dwOffset++];
		dwValue &= 0x1fffffff;
	}
	return dwValue;
}


//*************************************************************************
// Method:	 CLRMetadata::writeCompressedInt
// Description: Writes a compressed 32-bit integer to a PE section using
//				the same format as that used in readCompressedInt.
// Parameters:
//	pSection - PE section to write to
//  dwValue - Integer to encode
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::writeCompressedInt(PESection* pSection, DWORD dwValue)
{
	if (dwValue < 0x80)
	{
		if (!pSection->appendData((PBYTE)&dwValue, 1))
			return false;
		return true;
	}

	if (dwValue < 0x4000)
	{
		dwValue |= 0x8000;
		// Write out in big endian format
		if (!pSection->appendData(((PBYTE)&dwValue) + 1, 1))
			return false;
		if (!pSection->appendData((PBYTE)&dwValue, 1))
			return false;
		return true;
	}

	dwValue |= 0xc0000000;
	// Write out in big endian format
	if (!pSection->appendData(((PBYTE)&dwValue) + 3, 1))
		return false;
	if (!pSection->appendData(((PBYTE)&dwValue) + 2, 1))
		return false;
	if (!pSection->appendData(((PBYTE)&dwValue) + 1, 1))
		return false;
	if (!pSection->appendData((PBYTE)&dwValue, 1))
		return false;
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::getCompressedIntSize
// Description: Gets the number of bytes required to encode a value
//				in the compression format used in readCompressedInt.
// Parameters:
//	dwValue - Value to encode
//
// Return Value: Number of bytes required to encode the value
//*************************************************************************
DWORD CLRMetadata::getCompressedIntSize(DWORD dwValue)
{
	if (dwValue < 0x80)
		return 1;
	if (dwValue < 0x4000)
		return 2;
	return 4;
}


//*************************************************************************
// Method:	 CLRMetadata::readStringHeap
// Description: Reads a string heap from the metadata stream.  String heaps
//				are made up of a series of null-terminated UTF8 strings.
// Parameters:
//	shHeap - String heap to read into
//  pbData - Pointer to the string heap's data
//  dwSize - Length in bytes of the string heap
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::readStringHeap(CLRStringHeap& shHeap, const BYTE* pbData, DWORD dwSize)
{
	if (!pbData)
		return false;

	DWORD i;
	for (i=0; i<dwSize; )
	{
		CLRStringHeapEntry entry;
		entry.dwOffset = i;

		// Read in the string
		entry.sStr = SiString((char*)&pbData[i]);
		i += (DWORD)entry.sStr.GetLength() + 1; // Include null terminator

		// Add the string to the list
		shHeap.Add(entry);
	}

	if (shHeap.GetLength() == 0)
		return true;

	if (shHeap[shHeap.GetLength() - 1].sStr != "")
	{
		// Last string should be an empty string
		CLRStringHeapEntry entry;
		entry.dwOffset = i;
		entry.sStr = "";
		shHeap.Add(entry);
	}

	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::readGuidHeap
// Description: Reads a GUID heap from the metadata stream.  GUID heaps
//				are made up of a sequence of 16-byte GUIDs.
// Parameters:
//	ghHeap - GUID heap to read into
//  pbData - Pointer to the GUID heap's data
//  dwSize - Length in bytes of the GUID heap
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::readGuidHeap(CLRGuidHeap& ghHeap, const BYTE* pbData, DWORD dwSize)
{
	if (!pbData)
		return false;

	for (DWORD i=0; i<dwSize; i+=16)
	{
		CLRGuidHeapEntry entry;

		// Read in the next GUID (16 bytes)
		memcpy(entry.pbGuid, pbData + i, 16);
		entry.dwOffset = i;

		// Add the GUID to the list
		ghHeap.Add(entry);
	}
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::readBlobHeap
// Description: Reads a blob heap from the metadata stream.  A blob heap
//				is made up of a sequence of binary objects.  Each binary
//				object is preceded by a compressed integer representing
//				the object's size in bytes.
// Parameters:
//	bhHeap - Blob heap to read into
//  pbData - Pointer to the blob heap's data
//  dwSize - Length in bytes of the blob heap
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::readBlobHeap(CLRBlobHeap& bhHeap, const BYTE* pbData, DWORD dwSize)
{
	if (!pbData)
		return false;

	for (DWORD i=0; i<dwSize; )
	{
		// Initialize offset and size of data
		const BYTE* pbStartOfObject = pbData;
		CLRBlobHeapEntry entry;
		entry.dwOffset = i;
		entry.dwSize = readCompressedInt(pbData, i);

		// Calculate size of entire object, including compressed length
		entry.dwTotalSize = getCompressedIntSize(entry.dwSize) + entry.dwSize;

		// Allocate and read in the data
		entry.pbData = new BYTE[entry.dwSize];
		memcpy(entry.pbData, &pbData[i], entry.dwSize);

		// Update offset
		i += entry.dwSize;

		// Add blob to the list
		bhHeap.Add(entry);
	}
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::minBitCount
// Description: Calculates the minimum number of bits needed to encode
//				a given integer
// Parameters:
//	dwValue - Integer to calculate minimum bit count for
//
// Return Value: Minimum number of bits needed to encode the integer
//*************************************************************************
DWORD CLRMetadata::minBitCount(DWORD dwValue)
{
	DWORD i = 1;
	DWORD pow2 = 2;

	while (dwValue >= pow2)
	{
		i++;
		pow2 <<= 1;
	}
	return i;
}


//*************************************************************************
// Method:	 CLRMetadata::readSingleTable
// Description: Reads one metadata table from the metadata stream
// Parameters:
//	pbData - Pointer to the tables section of the metadata stream
//  pdwTableRecordCount - Pointer to an array containing the number of
//		records in each of the tables
//  dwTableIndex - Index of the table to read
//  dwTableOffset - Offset in the tables section where this table begins.
//		After this function finishes, this variable will hold the offset
//		where the next table begins.
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::readSingleTable(const BYTE* pbData, DWORD* pdwTableRecordCount,
	DWORD dwTableIndex, DWORD& dwTableOffset)
{
	DWORD i, j, dwOffset, dwMaxRecords, dwRecords;

	DWORD dwColCount = g_ptdTableFormat[dwTableIndex].bColCount;
	if ((dwTableIndex == CLR_TABLE_GENERICPARAM) && (!isAtLeastVersion(2, 0, 40209)))
	{
		// CLR builds prior to 2.0.40209 use a deprecated column in the GenericParam table
		dwColCount = 6;
	}

	CLRTableColDef* ptcCols = new CLRTableColDef[dwColCount];
	if (!ptcCols)
		return false;

	// Initiailize columns for the current table
	for (i=0, dwOffset=0; i<dwColCount; i++)
	{
		ptcCols[i].bType = g_ptdTableFormat[dwTableIndex].ptcCols[i].bType;
		ptcCols[i].bColOffset = (BYTE)dwOffset;
		if (ptcCols[i].bType < CLR_CODED_TOKEN_BASE)
		{
			// Table RID.  If there are less than 65536 rows in the table, the RID takes only 2 bytes
			if (pdwTableRecordCount[ptcCols[i].bType] < 65536)
				ptcCols[i].bColSize = 2;
			else
				ptcCols[i].bColSize = 4;
		}
		else if (ptcCols[i].bType <= CLR_COL_CODED_TOKEN_MAX)
		{
			// Coded token
			dwMaxRecords = 0;
			// Token size is computed from the maximum number of records in the tables that can be
			// referenced from this coded token (referenced tables are stored in the codedTokenTables list).
			// If the token type bits plus the number of bits needed to store the RID is less than or
			// equal to 16 bits, only 2 bytes are needed for the coded token.  Otherwise 4 bytes are used.
			for (j=0; j<g_pcttCodedTokenTables[ptcCols[i].bType - CLR_CODED_TOKEN_BASE]->bTableCount; j++)
			{
				if (g_pcttCodedTokenTables[ptcCols[i].bType - CLR_CODED_TOKEN_BASE]->pbTables[j] == CLR_COL_STRING)
				{
					// String columns in a coded token appear to be deprecated.  This needs to be always zero.
					dwRecords = 0;
				}
				else
				{
					dwRecords = pdwTableRecordCount[g_pcttCodedTokenTables[ptcCols[i].bType -
						CLR_CODED_TOKEN_BASE]->pbTables[j]];
				}
				if (dwRecords > dwMaxRecords)
					dwMaxRecords = dwRecords;
			}
			if ((minBitCount(dwMaxRecords) + g_pcttCodedTokenTables[ptcCols[i].bType -
				CLR_CODED_TOKEN_BASE]->bBitCount) <= 16)
			{
				ptcCols[i].bColSize = 2;
			}
			else
				ptcCols[i].bColSize = 4;
		}
		else
		{
			// Simple type.  Size is based on type.
			switch (ptcCols[i].bType)
			{
			case CLR_COL_SHORT:
			case CLR_COL_USHORT:
				ptcCols[i].bColSize = 2;
				break;
			case CLR_COL_LONG:
			case CLR_COL_ULONG:
			case CLR_COL_TOKEN:
				ptcCols[i].bColSize = 4;
				break;
			case CLR_COL_BYTE:
				ptcCols[i].bColSize = 2;
				break;
			case CLR_COL_STRING:
				ptcCols[i].bColSize = (m_mthTableHeader.bHeapFlags & CLR_LARGE_STRING_HEAP_FLAG) ? 4 : 2;
				break;
			case CLR_COL_GUID:
				ptcCols[i].bColSize = (m_mthTableHeader.bHeapFlags & CLR_LARGE_GUID_HEAP_FLAG) ? 4 : 2;
				break;
			case CLR_COL_BLOB:
			case CLR_COL_SIGNATURE:
			case CLR_COL_SIGNATURE_NOCALLCONV:
				ptcCols[i].bColSize = (m_mthTableHeader.bHeapFlags & CLR_LARGE_BLOB_HEAP_FLAG) ? 4 : 2;
				break;
			default:
				// Invalid type
				delete[] ptcCols;
				return false;
			}
		}
		dwOffset += ptcCols[i].bColSize;
	}

	m_ptTable[dwTableIndex].setColDefs(ptcCols, dwColCount, dwTableIndex);

	DWORD dwRecordSize = (WORD)dwOffset;

	// Read in each row
	m_ptTable[dwTableIndex].sizeHint(pdwTableRecordCount[dwTableIndex]);
	for (DWORD i=0; i<pdwTableRecordCount[dwTableIndex]; i++)
	{
		CLRTableRow row(&m_ptTable[dwTableIndex]);
		// Set data for each column in this row
		for (DWORD j=0; j<dwColCount; j++)
		{
			switch (ptcCols[j].bColSize)
			{
			case 1:
				row[j] = *(pbData + dwTableOffset + ptcCols[j].bColOffset);
				break;
			case 2:
				row[j] = *((WORD*)(pbData + dwTableOffset + ptcCols[j].bColOffset));
				break;
			case 4:
				row[j] = *((DWORD*)(pbData + dwTableOffset + ptcCols[j].bColOffset));
				break;
			default:
				// Invalid column size
				delete[] ptcCols;
				return false;
			}
		}

		// Add row and place offset past this row
		m_ptTable[dwTableIndex].add(row);
		dwTableOffset += dwRecordSize;
	}

	delete[] ptcCols;
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::readTables
// Description: Reads the metadata tables from the metadata stream.
// Parameters:
//	pbData - Pointer to the data in the tables section
//  pdwTableRecordCount - Pointer to an array containing the number of
//		records in each of the tables
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::readTables(const BYTE* pbData, DWORD* pdwTableRecordCount)
{
	DWORD i, dwOffset;

	// Go through each table and prepare the table definition structures
	dwOffset = 0;
	for (i=0; i<CLR_METADATA_TABLE_COUNT; i++)
	{
		// Only get table definition if the table is valid.  Check the ith bit of the
		// pfTableValid bit field in the metadata header.
		if (m_mthTableHeader.pfTableValid[i / 8] & (1 << (i % 8)))
		{
			// Table is valid.  Read in the table data.
			if (!readSingleTable(pbData, pdwTableRecordCount, i, dwOffset))
			{
				// Error in the metadata
				return false;
			}
		}
		else
		{
			// No table in file, but set the column definitions just in case
			// we want to add a row to the table later
			DWORD dwColCount = g_ptdTableFormat[i].bColCount;
			if ((i == CLR_TABLE_GENERICPARAM) && (!isAtLeastVersion(2, 0, 40209)))
			{
				// CLR builds prior to 2.0.40209 use a deprecated column in the GenericParam table
				dwColCount = 6;
			}
			m_ptTable[i].setColDefs(g_ptdTableFormat[i].ptcCols, dwColCount, i);
		}
	}
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::saveTable
// Description: Writes one table to the metadata stream in a PE section.
// Parameters:
//	dwTableIndex - Index of table to write
//  pSection - PE section to write to
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::saveTable(DWORD dwTableIndex, PESection* pSection)
{
	DWORD i, j, dwOffset, dwMaxRecords, dwRecords;

	// Create new column definitions for the modified data
	DWORD dwColCount = g_ptdTableFormat[dwTableIndex].bColCount;
	if ((dwTableIndex == CLR_TABLE_GENERICPARAM) && (!isAtLeastVersion(2, 0, 40209)))
	{
		// CLR builds prior to 2.0.40209 use a deprecated column in the GenericParam table
		dwColCount = 6;
	}
	CLRTableColDef* ptcCols = new CLRTableColDef[dwColCount];
	if (!ptcCols)
		return false;

	// Initiailize column definitions for the current table
	for (i=0, dwOffset=0; i<dwColCount; i++)
	{
		ptcCols[i].bType = g_ptdTableFormat[dwTableIndex].ptcCols[i].bType;
		ptcCols[i].bColOffset = (BYTE)dwOffset;
		if (ptcCols[i].bType < CLR_CODED_TOKEN_BASE)
		{
			// Table RID.  If there are less than 65536 rows in the table, the RID takes only 2 bytes
			if (m_ptTable[ptcCols[i].bType].getRowCount() < 65536)
				ptcCols[i].bColSize = 2;
			else
				ptcCols[i].bColSize = 4;
		}
		else if (ptcCols[i].bType <= CLR_COL_CODED_TOKEN_MAX)
		{
			// Coded token
			dwMaxRecords = 0;
			// Token size is computed from the maximum number of records in the tables that can be
			// referenced from this coded token (referenced tables are stored in the codedTokenTables list).
			// If the token type bits plus the number of bits needed to store the RID is less than or
			// equal to 16 bits, only 2 bytes are needed for the coded token.  Otherwise 4 bytes are used.
			for (j=0; j<g_pcttCodedTokenTables[ptcCols[i].bType - CLR_CODED_TOKEN_BASE]->bTableCount; j++)
			{
				if (g_pcttCodedTokenTables[ptcCols[i].bType - CLR_CODED_TOKEN_BASE]->pbTables[j] == CLR_COL_STRING)
				{
					// String columns in a coded token appear to be deprecated.  This needs to be always zero.
					dwRecords = 0;
				}
				else
				{
					dwRecords = m_ptTable[g_pcttCodedTokenTables[ptcCols[i].bType -
						CLR_CODED_TOKEN_BASE]->pbTables[j]].getRowCount();
				}
				if (dwRecords > dwMaxRecords)
					dwMaxRecords = dwRecords;
			}
			if ((minBitCount(dwMaxRecords) + g_pcttCodedTokenTables[ptcCols[i].bType -
				CLR_CODED_TOKEN_BASE]->bBitCount) <= 16)
			{
				ptcCols[i].bColSize = 2;
			}
			else
				ptcCols[i].bColSize = 4;
		}
		else
		{
			// Simple type.  Size is based on type.
			switch (ptcCols[i].bType)
			{
			case CLR_COL_SHORT:
			case CLR_COL_USHORT:
				ptcCols[i].bColSize = 2;
				break;
			case CLR_COL_LONG:
			case CLR_COL_ULONG:
			case CLR_COL_TOKEN:
				ptcCols[i].bColSize = 4;
				break;
			case CLR_COL_BYTE:
				ptcCols[i].bColSize = 2;
				break;
			case CLR_COL_STRING:
				ptcCols[i].bColSize = (m_mthTableHeader.bHeapFlags & CLR_LARGE_STRING_HEAP_FLAG) ? 4 : 2;
				break;
			case CLR_COL_GUID:
				ptcCols[i].bColSize = (m_mthTableHeader.bHeapFlags & CLR_LARGE_GUID_HEAP_FLAG) ? 4 : 2;
				break;
			case CLR_COL_BLOB:
			case CLR_COL_SIGNATURE:
			case CLR_COL_SIGNATURE_NOCALLCONV:
				ptcCols[i].bColSize = (m_mthTableHeader.bHeapFlags & CLR_LARGE_BLOB_HEAP_FLAG) ? 4 : 2;
				break;
			default:
				// Invalid type
				delete[] ptcCols;
				return false;
			}
		}
		dwOffset += ptcCols[i].bColSize;
	}

	// Use the column definitions to output the data for this table
	for (DWORD i=0; i<m_ptTable[dwTableIndex].getRowCount(); i++)
	{
		// Output data for each column in this row
		for (DWORD j=0; j<dwColCount; j++)
		{
			DWORD dwColValue = m_ptTable[dwTableIndex][i][j];
			switch (ptcCols[j].bColSize)
			{
			case 1:
				pSection->appendData(&dwColValue, 1);
				break;
			case 2:
				pSection->appendData(&dwColValue, 2);
				break;
			case 4:
				pSection->appendData(&dwColValue, 4);
				break;
			default:
				// Invalid column size
				delete[] ptcCols;
				return false;
			}
		}
	}

	// Table output complete
	delete[] ptcCols;
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::parse
// Description: Parses the metadata and maintains the tables and heaps
//				in ata structures accessible to the caller.
// Parameters:
//	pFile - PE file containing the metadata to parse
//  dwMetadataRVA - RVA to the metadata stream
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::parse(PEFile* pFile, DWORD dwMetadataRVA)
{
	m_pFile = pFile;

	// Find a pointer to the metadata
	const BYTE* pbMetadata = pFile->rvaToPointer(dwMetadataRVA);
	if (!pbMetadata)
		return false;

	// Copy header information
	memcpy(&m_mhHeader, pbMetadata, sizeof(CLRMetadataHeader));

	// Check header signature
	if (m_mhHeader.dwSignature != CLR_METADATA_HEADER_SIGNATURE)
		return false;

	// Allocate memory for the version string
	m_pszVersionString = new char[m_mhHeader.dwLength + 1];
	if (!m_pszVersionString)
		return false;

	// Copy the version string, ensuring that there is a NULL terminator byte
	memcpy(m_pszVersionString, pbMetadata + sizeof(CLRMetadataHeader), m_mhHeader.dwLength);
	m_pszVersionString[m_mhHeader.dwLength] = 0;

	// Copy storage header
	memcpy(&m_mshStorageHeader, pbMetadata + sizeof(CLRMetadataHeader) + m_mhHeader.dwLength,
		sizeof(CLRMetadataStorageHeader));

	// Parse stream information
	DWORD dwStreamHeaderOffset = sizeof(CLRMetadataHeader) + m_mhHeader.dwLength +
		sizeof(CLRMetadataStorageHeader);
	for (DWORD i=0; i<m_mshStorageHeader.wStreams; i++)
	{
		CLRMetadataStreamHeader mshStreamHeader;

		// Get next stream header
		memcpy(&mshStreamHeader, pbMetadata + dwStreamHeaderOffset, sizeof(CLRMetadataStreamHeader));
		dwStreamHeaderOffset += sizeof(CLRMetadataStreamHeader);

		// Read stream name
		char pszStreamName[16];
		DWORD j;
		for (j=0; pbMetadata[dwStreamHeaderOffset]; j++)
		{
			pszStreamName[j] = pbMetadata[dwStreamHeaderOffset];
			dwStreamHeaderOffset++;
		}
		// Copy null terminator
		pszStreamName[j] = pbMetadata[dwStreamHeaderOffset];
		dwStreamHeaderOffset++;

		// Make sure offset is on a 4-byte boundary
		dwStreamHeaderOffset = (dwStreamHeaderOffset + 3) & (~3);

		if (!strcmp(pszStreamName, "#Strings"))
		{
			// String heap
			if (!readStringHeap(m_shStringHeap, pbMetadata + mshStreamHeader.dwOffset,
				mshStreamHeader.dwSize))
				return false;
		}
		else if (!strcmp(pszStreamName, "#Blob"))
		{
			// Blob heap
			if (!readBlobHeap(m_bhBlobHeap, pbMetadata + mshStreamHeader.dwOffset,
				mshStreamHeader.dwSize))
				return false;
		}
		else if (!strcmp(pszStreamName, "#GUID"))
		{
			// GUID heap
			if (!readGuidHeap(m_ghGuidHeap, pbMetadata + mshStreamHeader.dwOffset,
				mshStreamHeader.dwSize))
				return false;
		}
		else if (!strcmp(pszStreamName, "#US"))
		{
			// User string heap (a blob heap)
			if (!readBlobHeap(m_bhUserStringHeap, pbMetadata + mshStreamHeader.dwOffset,
				mshStreamHeader.dwSize))
				return false;
		}
		else if (!strcmp(pszStreamName, "#~"))
		{
			// Metadata tables, compressed
			m_pbOrigMetadataTables = pbMetadata + mshStreamHeader.dwOffset;
			m_fCompressedTables = true;
		}
		else if (!strcmp(pszStreamName, "#-"))
		{
			// Metadata tables, uncompressed
			m_pbOrigMetadataTables = pbMetadata + mshStreamHeader.dwOffset;
			m_fCompressedTables = false;
		}
	}

	if (!m_pbOrigMetadataTables)
	{
		// Didn't find any valid metadata tables
		return false;
	}

	// Copy the metadata table header
	memcpy(&m_mthTableHeader, m_pbOrigMetadataTables, sizeof(CLRMetadataTableHeader));

	// Record counts follow the metadata header
	DWORD* pdwRecordCountPtr = (DWORD*)((size_t)m_pbOrigMetadataTables + sizeof(CLRMetadataTableHeader));
	DWORD pdwTableRecordCount[CLR_METADATA_TABLE_COUNT];
	memset(pdwTableRecordCount, 0, sizeof(DWORD) * CLR_METADATA_TABLE_COUNT);
	// Find the record count for each metadata table
	for (DWORD i=0; i<CLR_METADATA_TABLE_COUNT; i++)
	{
		// Check the ith bit of the pfTableValid bit field in the metadata header.  If it
		// is set, the table is present and a record count for the table follows the header.
		if (m_mthTableHeader.pfTableValid[i / 8] & (1 << (i % 8)))
			pdwTableRecordCount[i] = *(pdwRecordCountPtr++);
	}

	// Metadata tables themselves follow record counts.  Read the tables.
	if (!readTables((const BYTE*)pdwRecordCountPtr, pdwTableRecordCount))
		return false;

	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::save
// Description: Saves the possibly modified metadata as a new metadata
//				stream in a PE section.
// Parameters:
//	pSection - PE section to write the metadata to
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMetadata::save(PESection* pSection)
{
	DWORD dwStartOffset = pSection->getRawDataSize();

	// Create a temporary section to hold the table data
	PESection* pTableSection = new PESection();
	pTableSection->create("Tables", 0);

	// Set large string heap flag if the string heap needs more than 16 bits
	// to represent any offset in the heap
	if (getStringHeapSize() >= 65536)
		m_mthTableHeader.bHeapFlags |= CLR_LARGE_STRING_HEAP_FLAG;
	else
		m_mthTableHeader.bHeapFlags &= ~CLR_LARGE_STRING_HEAP_FLAG;

	// Set large GUID heap flag if the GUID heap needs more than 16 bits
	// to represent any offset in the heap
	if (getGuidHeapSize() >= 65536)
		m_mthTableHeader.bHeapFlags |= CLR_LARGE_GUID_HEAP_FLAG;
	else
		m_mthTableHeader.bHeapFlags &= ~CLR_LARGE_GUID_HEAP_FLAG;

	// Set large blob heap flag if the blob heap needs more than 16 bits
	// to represent any offset in the heap
	if (getBlobHeapSize() >= 65536)
		m_mthTableHeader.bHeapFlags |= CLR_LARGE_BLOB_HEAP_FLAG;
	else
		m_mthTableHeader.bHeapFlags &= ~CLR_LARGE_BLOB_HEAP_FLAG;

	// Recompute which tables are valid
	for (DWORD i=0; i<CLR_METADATA_TABLE_COUNT; i++)
	{
		if (m_ptTable[i].getRowCount() > 0)
		{
			// Table has rows.  Mark it as valid.
			m_mthTableHeader.pfTableValid[i / 8] |= (1 << (i % 8));
		}
	}

	// Write out the metadata table header to the temporary table section
	if (!pTableSection->appendData(&m_mthTableHeader, sizeof(CLRMetadataTableHeader)))
		return false;

	// Write out record counts for each valid table to the temporary table section
	for (DWORD i=0; i<CLR_METADATA_TABLE_COUNT; i++)
	{
		// Only write out a record count if the table was valid.  Check the
		// valid bit in the pfTableValid bit field in the header.
		if (m_mthTableHeader.pfTableValid[i / 8] & (1 << (i % 8)))
		{
			DWORD dwRowCount = m_ptTable[i].getRowCount();
			if (!pTableSection->appendData(&dwRowCount, 4))
				return false;
		}
	}

	// Output rows for each table that has its valid bit set
	for (DWORD i=0; i<CLR_METADATA_TABLE_COUNT; i++)
	{
		// Only write out rows if the table was valid.  Check the
		// valid bit in the pfTableValid bit field in the header.
		if (m_mthTableHeader.pfTableValid[i / 8] & (1 << (i % 8)))
		{
			if (!saveTable(i, pTableSection))
				return false;
		}
	}

	// Align the table section to a 4 byte boundary
	while (pTableSection->getRawDataSize() & 3)
	{
		BYTE bZero = 0;
		pTableSection->appendData(&bZero, 1);
	}

	// Temporary section containing metadata tables is complete.  Start
	// outputting the actual metadata by writing the metadata header to
	// the real metadata section.
	if (!pSection->appendData(&m_mhHeader, sizeof(CLRMetadataHeader)))
		return false;
	if (!pSection->appendData(m_pszVersionString, m_mhHeader.dwLength))
		return false;

	// Always output 5 streams (#Strings, #Blob, #GUID, #US, and either #~ or #-)
	m_mshStorageHeader.wStreams = 5;

	// Save storage header
	if (!pSection->appendData(&m_mshStorageHeader, sizeof(CLRMetadataStorageHeader)))
		return false;

	// Calculate size of stream headers.  Each stream header is the size of a CLRMetadataStreamHeader
	// structure plus the length of the name string including null terminator.  Stream headers must
	// be aligned on a four byte boundary, so round the string length up to the next four byte boundary.
	DWORD dwCurOffset = pSection->getRawDataSize();
	dwCurOffset += sizeof(CLRMetadataStreamHeader) + 4; // "#-" or "#~", 3 chars with null, 4 rounded
	dwCurOffset += sizeof(CLRMetadataStreamHeader) + 12; // "#Strings", 9 chars with null, 12 rounded
	dwCurOffset += sizeof(CLRMetadataStreamHeader) + 4; // "#US", 4 chars with null, 4 rounded
	dwCurOffset += sizeof(CLRMetadataStreamHeader) + 8; // "#GUID", 6 chars with null, 8 rounded
	dwCurOffset += sizeof(CLRMetadataStreamHeader) + 8; // "#Blob", 6 chars with null, 8 rounded

	// Metadata table stream follows stream headers.  Save offset and calculate offset of the next stream
	CLRMetadataStreamHeader mshMetadataStreamHeader;
	mshMetadataStreamHeader.dwOffset = dwCurOffset;
	mshMetadataStreamHeader.dwSize = pTableSection->getRawDataSize();
	dwCurOffset += (mshMetadataStreamHeader.dwSize + 3) & (~3); // Align size to a 4 byte boundary

	// String heap follows stream headers.  Save offset and calculate offset of the next stream
	CLRMetadataStreamHeader mshStringStreamHeader;
	mshStringStreamHeader.dwOffset = dwCurOffset;
	mshStringStreamHeader.dwSize = getStringHeapSize();
	dwCurOffset += (mshStringStreamHeader.dwSize + 3) & (~3); // Align size to a 4 byte boundary

	// User string heap follows.  Save offset and calculate offset of the next stream
	CLRMetadataStreamHeader mshUserStringStreamHeader;
	mshUserStringStreamHeader.dwOffset = dwCurOffset;
	mshUserStringStreamHeader.dwSize = getUserStringHeapSize();
	dwCurOffset += (mshUserStringStreamHeader.dwSize + 3) & (~3); // Align size to a 4 byte boundary

	// GUID heap follows.  Save offset and calculate offset of the next stream
	CLRMetadataStreamHeader mshGuidStreamHeader;
	mshGuidStreamHeader.dwOffset = dwCurOffset;
	mshGuidStreamHeader.dwSize = getGuidHeapSize();
	dwCurOffset += (mshGuidStreamHeader.dwSize + 3) & (~3); // Align size to a 4 byte boundary

	// Blob heap is last.  Save offset
	CLRMetadataStreamHeader mshBlobStreamHeader;
	mshBlobStreamHeader.dwOffset = dwCurOffset;
	mshBlobStreamHeader.dwSize = getBlobHeapSize();

	// Save table stream header to section, padding name to 4 byte boundary with null bytes
	if (!pSection->appendData(&mshMetadataStreamHeader, sizeof(CLRMetadataStreamHeader)))
		return false;
	if (m_fCompressedTables)
	{
		if (!pSection->appendData("#~\0\0", 4))
			return false;
	}
	else
	{
		if (!pSection->appendData("#-\0\0", 4))
			return false;
	}

	// Save other stream headers to section, padding names to 4 byte boundaries with null bytes
	if (!pSection->appendData(&mshStringStreamHeader, sizeof(CLRMetadataStreamHeader)))
		return false;
	if (!pSection->appendData("#Strings\0\0\0\0", 12))
		return false;
	if (!pSection->appendData(&mshUserStringStreamHeader, sizeof(CLRMetadataStreamHeader)))
		return false;
	if (!pSection->appendData("#US\0", 4))
		return false;
	if (!pSection->appendData(&mshGuidStreamHeader, sizeof(CLRMetadataStreamHeader)))
		return false;
	if (!pSection->appendData("#GUID\0\0\0", 8))
		return false;
	if (!pSection->appendData(&mshBlobStreamHeader, sizeof(CLRMetadataStreamHeader)))
		return false;
	if (!pSection->appendData("#Blob\0\0\0", 8))
		return false;

	// Save metadata tables to section
	pSection->appendData(pTableSection->getData(), pTableSection->getRawDataSize());
	delete pTableSection; // Done with the temporary section

	// Save string heap to section
	for (DWORD i=0; i<m_shStringHeap.GetLength(); i++)
	{
		// Write out string, including null terminator
		if (!pSection->appendData((char*)m_shStringHeap[i].sStr, (DWORD)m_shStringHeap[i].sStr.GetLength() + 1))
			return false;
	}

	// Align the section to a 4 byte boundary
	while (pSection->getRawDataSize() & 3)
	{
		BYTE bZero = 0;
		pSection->appendData(&bZero, 1);
	}

	// Save user string heap to section
	for (DWORD i=0; i<m_bhUserStringHeap.GetLength(); i++)
	{
		// Write out data size in compressed form
		if (!writeCompressedInt(pSection, m_bhUserStringHeap[i].dwSize))
			return false;
		// Write out the actual data
		if (!pSection->appendData(m_bhUserStringHeap[i].pbData, m_bhUserStringHeap[i].dwSize))
			return false;
	}

	// Align the section to a 4 byte boundary
	while (pSection->getRawDataSize() & 3)
	{
		BYTE bZero = 0;
		pSection->appendData(&bZero, 1);
	}

	// Save GUID heap to section
	for (DWORD i=0; i<m_ghGuidHeap.GetLength(); i++)
	{
		// Write out each GUID
		if (!pSection->appendData(m_ghGuidHeap[i].pbGuid, 16))
			return false;
	}

	// Align the section to a 4 byte boundary
	while (pSection->getRawDataSize() & 3)
	{
		BYTE bZero = 0;
		pSection->appendData(&bZero, 1);
	}

	// Save blob heap to section
	for (DWORD i=0; i<m_bhBlobHeap.GetLength(); i++)
	{
		// Write out data size in compressed form
		if (!writeCompressedInt(pSection, m_bhBlobHeap[i].dwSize))
			return false;
		// Write out the actual data
		if (!pSection->appendData(m_bhBlobHeap[i].pbData, m_bhBlobHeap[i].dwSize))
			return false;
	}

	// Metadata written out sucessfully
	return true;
}


//*************************************************************************
// Method:	 CLRMetadata::getString
// Description: Returns a string from the string heap based on offset.
// Parameters:
//	dwOffset - Offset within string heap of the string to be retrieved
//
// Return Value: String at the given offset, or empty string if not found
//*************************************************************************
SiString CLRMetadata::getString(const DWORD dwOffset)
{
	// Find and return the string at this offset in the string heap, using
	// a binary search (offsets are always sorted in the array)
	DWORD dwMin = 0;
	DWORD dwMax = (DWORD)m_shStringHeap.GetLength();
	for (DWORD i=(dwMin + dwMax) / 2; dwMin <= dwMax; i = (dwMin + dwMax) / 2)
	{
		if (m_shStringHeap[i].dwOffset == dwOffset)
			return m_shStringHeap[i].sStr;
		if (dwMin == dwMax)
		{
			// Done searching and didn't find the offset
			break;
		}

		// Split the search space according to which side we know the offset should be on
		if (m_shStringHeap[i].dwOffset < dwOffset)
			dwMin = i + 1;
		else
			dwMax = i - 1;
	}
	return "";
}


//*************************************************************************
// Method:	 CLRMetadata::getBlob
// Description: Returns a blob object from the blob heap based on offset
// Parameters:
//	dwOffset - Offset of the blob object to retrieve
//
// Return Value: CLRBlobHeapEntry object containing the blob object
//		retrieved.  NULL if the blob could not be found.
//*************************************************************************
const CLRBlobHeapEntry* CLRMetadata::getBlob(const DWORD dwOffset)
{
	// Find and return the blob at this offset in the blob heap, using
	// a binary search (offsets are always sorted in the array)
	DWORD dwMin = 0;
	DWORD dwMax = (DWORD)m_bhBlobHeap.GetLength();
	for (DWORD i=(dwMin + dwMax) / 2; dwMin <= dwMax; i = (dwMin + dwMax) / 2)
	{
		if (m_bhBlobHeap[i].dwOffset == dwOffset)
			return &m_bhBlobHeap[i];
		if (dwMin == dwMax)
		{
			// Done searching and didn't find the offset
			break;
		}

		// Split the search space according to which side we know the offset should be on
		if (m_bhBlobHeap[i].dwOffset < dwOffset)
			dwMin = i + 1;
		else
			dwMax = i - 1;
	}
	return NULL;
}


//*************************************************************************
// Method:	 CLRMetadata::getGuid
// Description: Returns a GUID object from the GUID heap based on offset
// Parameters:
//	dwOffset - Offset of the GUID object to retrieve
//
// Return Value: CLRGuidHeapEntry object containing the GUID object
//		retrieved.  NULL if the GUID could not be found.
//*************************************************************************
const CLRGuidHeapEntry* CLRMetadata::getGuid(const DWORD dwOffset)
{
	// Find and return the GUID at this offset in the GUID heap, using
	// a binary search (offsets are always sorted in the array)
	DWORD dwMin = 0;
	DWORD dwMax = (DWORD)m_ghGuidHeap.GetLength();
	for (DWORD i=(dwMin + dwMax) / 2; dwMin <= dwMax; i = (dwMin + dwMax) / 2)
	{
		if (m_ghGuidHeap[i].dwOffset == dwOffset)
			return &m_ghGuidHeap[i];
		if (dwMin == dwMax)
		{
			// Done searching and didn't find the offset
			break;
		}

		// Split the search space according to which side we know the offset should be on
		if (m_ghGuidHeap[i].dwOffset < dwOffset)
			dwMin = i + 1;
		else
			dwMax = i - 1;
	}
	return NULL;
}


//*************************************************************************
// Method:	 CLRMetadata::getTable
// Description: Accesses a table at the given table index
// Parameters:
//	index - Table index
//
// Return Value: Reference to the CLRTable object representing the table
//		at the requested index.
//*************************************************************************
CLRTable& CLRMetadata::getTable(const int index)
{
	return m_ptTable[index];
}


//*************************************************************************
// Method:	 CLRMetadata::getStringHeapSize
// Description: Calculates the current size in bytes of the string heap
// Parameters:
//	None
//
// Return Value: Size in bytes of the string heap
//*************************************************************************
DWORD CLRMetadata::getStringHeapSize()
{
	if (m_shStringHeap.GetLength() == 0)
		return 0;
	return m_shStringHeap[m_shStringHeap.GetLength() - 1].dwOffset +
		m_shStringHeap[m_shStringHeap.GetLength() - 1].sStr.GetLength() + 1;
}


//*************************************************************************
// Method:	 CLRMetadata::getGuidHeapSize
// Description: Calculates the current size in bytes of the GUID heap
// Parameters:
//	None
//
// Return Value: Size in bytes of the GUID heap
//*************************************************************************
DWORD CLRMetadata::getGuidHeapSize()
{
	return (DWORD)m_ghGuidHeap.GetLength() * 16;
}


//*************************************************************************
// Method:	 CLRMetadata::getBlobHeapSize
// Description: Calculates the current size in bytes of the blob heap
// Parameters:
//	None
//
// Return Value: Size in bytes of the blob heap
//*************************************************************************
DWORD CLRMetadata::getBlobHeapSize()
{
	if (m_bhBlobHeap.GetLength() == 0)
		return 0;
	return m_bhBlobHeap[m_bhBlobHeap.GetLength() - 1].dwOffset +
		m_bhBlobHeap[m_bhBlobHeap.GetLength() - 1].dwTotalSize;
}


//*************************************************************************
// Method:	 CLRMetadata::getUserStringHeapSize
// Description: Calculates the current size in bytes of the user string heap
// Parameters:
//	None
//
// Return Value: Size in bytes of the user string heap
//*************************************************************************
DWORD CLRMetadata::getUserStringHeapSize()
{
	if (m_bhUserStringHeap.GetLength() == 0)
		return 0;
	return m_bhUserStringHeap[m_bhUserStringHeap.GetLength() - 1].dwOffset +
		m_bhUserStringHeap[m_bhUserStringHeap.GetLength() - 1].dwTotalSize;
}


//*************************************************************************
// Method:	 CLRMetadata::getUserStringCount
// Description: Gets the number of entries in the user string heap
// Parameters:
//	None
//
// Return Value: Number of entries in the user string heap
//*************************************************************************
DWORD CLRMetadata::getUserStringCount()
{
	return m_bhUserStringHeap.GetLength();
}


//*************************************************************************
// Method:	 CLRMetadata::getUserStringFromIndex
// Description: Gets a user string heap entry from an index
// Parameters:
//	dwIndex - Index of entry
//
// Return Value: User string heap entry
//*************************************************************************
const CLRBlobHeapEntry* CLRMetadata::getUserStringFromIndex(const DWORD dwIndex)
{
	return &m_bhUserStringHeap[dwIndex];
}


//*************************************************************************
// Method:	 CLRMetadata::addString
// Description: Adds a new string to the string heap
// Parameters:
//	sStr - String to add
//
// Return Value: Offset within the string heap of the new string
//*************************************************************************
DWORD CLRMetadata::addString(const SiString& sStr)
{
	// Create a string heap entry for this string
	CLRStringHeapEntry sheEntry;
	sheEntry.dwOffset = getStringHeapSize() - 1; // Insert before last null string
	sheEntry.sStr = sStr;

	// Insert the string into the heap.  Be sure to insert before the last null string
	m_shStringHeap.Insert(m_shStringHeap.GetLength() - 1, sheEntry);

	// Move the offset of the last null string to be after the newly inserted string
	m_shStringHeap[m_shStringHeap.GetLength() - 1].dwOffset += (DWORD)sStr.GetLength() + 1;

	return sheEntry.dwOffset;
}


//*************************************************************************
// Method:	 CLRMetadata::addUserString
// Description: Adds a new string to the user string heap
// Parameters:
//	sStr - String to add
//
// Return Value: Offset in the user string heap of the new string
//*************************************************************************
DWORD CLRMetadata::addUserString(const SiString& sStr)
{
	wchar_t* wszStrBuffer;

	// Copy the string into the new buffer that will be saved in the user string heap
	DWORD dwStrLen = (DWORD)wcslen((wchar_t*)sStr) + 1;
	wszStrBuffer = new wchar_t[dwStrLen];
	wcscpy(wszStrBuffer, (wchar_t*)sStr);

	// Create a user string heap entry for this string
	CLRBlobHeapEntry bheEntry;
	bheEntry.dwOffset = getUserStringHeapSize();
	bheEntry.dwSize = (dwStrLen * sizeof(wchar_t)) - 1; // Only output the first byte of the terminating null
	bheEntry.pbData = (BYTE*)wszStrBuffer;
	bheEntry.dwTotalSize = bheEntry.dwSize + getCompressedIntSize(bheEntry.dwSize);

	// Add the entry to the user string heap
	m_bhUserStringHeap.Add(bheEntry);

	return bheEntry.dwOffset;
}


//*************************************************************************
// Method:	 CLRMetadata::addUserStringBlob
// Description: Adds a new string to the user string heap
// Parameters:
//	pbData - Data to add
//  dwSize - Length of data to add
//
// Return Value: Offset in the user string heap of the new string
//*************************************************************************
DWORD CLRMetadata::addUserString(const BYTE* pbData, DWORD dwSize)
{
	// Create a new blob heap entry
	CLRBlobHeapEntry bheEntry;
	bheEntry.dwOffset = getUserStringHeapSize();
	bheEntry.dwSize = dwSize;
	bheEntry.pbData = new BYTE[dwSize];
	memcpy(bheEntry.pbData, pbData, dwSize);
	bheEntry.dwTotalSize = bheEntry.dwSize + getCompressedIntSize(bheEntry.dwSize);

	// Add the entry to the blob heap
	m_bhUserStringHeap.Add(bheEntry);

	return bheEntry.dwOffset;
}


//*************************************************************************
// Method:	 CLRMetadata::addBlob
// Description: Adds a new blob object to the blob heap.
// Parameters:
//	pbData - Data contained in the blob object
//  dwSize - Size of the data in the blob object
//
// Return Value: Offset of the new blob object in the blob heap
//*************************************************************************
DWORD CLRMetadata::addBlob(const BYTE* pbData, DWORD dwSize)
{
	// Create a new blob heap entry
	CLRBlobHeapEntry bheEntry;
	bheEntry.dwOffset = getBlobHeapSize();
	bheEntry.dwSize = dwSize;
	bheEntry.pbData = new BYTE[dwSize];
	memcpy(bheEntry.pbData, pbData, dwSize);
	bheEntry.dwTotalSize = bheEntry.dwSize + getCompressedIntSize(bheEntry.dwSize);

	// Add the entry to the blob heap
	m_bhBlobHeap.Add(bheEntry);

	return bheEntry.dwOffset;
}


//*************************************************************************
// Method:	 CLRMetadata::addGuid
// Description: Adds a new GUID object to the GUID heap.
// Parameters:
//	guid - Data contained in the GUID object
//
// Return Value: Offset of the new GUID object in the GUID heap
//*************************************************************************
DWORD CLRMetadata::addGuid(const BYTE* guid)
{
	// Create a new GUID heap entry
	CLRGuidHeapEntry gheEntry;
	gheEntry.dwOffset = getGuidHeapSize();
	memcpy(gheEntry.pbGuid, guid, 16);

	// Add the entry to the blob heap
	m_ghGuidHeap.Add(gheEntry);

	return gheEntry.dwOffset;
}


//*************************************************************************
// Method:	 CLRMetadata::getFile
// Description: Gets the PE file where this metadata came from
// Parameters:
//	None
//
// Return Value: PEFile object that contains this metadata
//*************************************************************************
PEFile& CLRMetadata::getFile()
{
	return *m_pFile;
}


//*************************************************************************
// Method:	 CLRMetadata::printTables
// Description: Prints the metadata tables to a file for debugging
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void CLRMetadata::printTables()
{
	FILE *fp = fopen("c:\\tables.txt", "w");
	for (int i=0; i<CLR_METADATA_TABLE_COUNT; i++)
		m_ptTable[i].printTable(fp);
	fclose(fp);
}


//*************************************************************************
// Method:	 CLRMetadata::isAtLeastVersion
// Description: Determines if the metadata is a given version or higher
// Parameters:
//	dwMajor - Major version number
//  dwMinor - Minor version number
//  dwBuild - Build number
//
// Return Value: true if given version or later, false if earlier version
//*************************************************************************
bool CLRMetadata::isAtLeastVersion(DWORD dwMajor, DWORD dwMinor, DWORD dwBuild)
{
	// Version info has the format "v[Major].[Minor].[Build]"
	SiString versionStr = m_pszVersionString;
	SiString majorStr, minorStr, buildStr;

	if (versionStr.FindFirst('.') != -1)
	{
		versionStr.SubString(majorStr, 1, versionStr.FindFirst('.') - 1);
		versionStr.SubString(versionStr, versionStr.FindFirst('.') + 1);
	}
	else
		return false;

	if (versionStr.FindFirst('.') != -1)
	{
		versionStr.SubString(minorStr, 0, versionStr.FindFirst('.') - 1);
		versionStr.SubString(buildStr, versionStr.FindFirst('.') + 1);
	}
	else
		return false;

	if (atoi((char*)majorStr) < (int)dwMajor)
		return false;
	if (atoi((char*)majorStr) > (int)dwMajor)
		return true;

	if (atoi((char*)minorStr) < (int)dwMinor)
		return false;
	if (atoi((char*)minorStr) > (int)dwMinor)
		return true;

	if (atoi((char*)buildStr) < (int)dwBuild)
		return false;
	return true;
}
