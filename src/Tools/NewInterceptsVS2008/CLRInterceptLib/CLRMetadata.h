//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRMetadata.h
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

#ifndef __CLRMETADATA_H__
#define __CLRMETADATA_H__

#include <windows.h>
#include <corhdr.h>
#include "CLRMetadataDefs.h"
#include "CLRTable.h"
#include "SiArray.h"
#include "SiString.h"


#define CLR_METADATA_HEADER_SIGNATURE 0x424a5342


struct CLRMetadataHeader
{
	DWORD dwSignature;
	WORD wMajorVersion;
	WORD wMinorVersion;
	DWORD dwExtraDelta;
	DWORD dwLength;
};


struct CLRMetadataStorageHeader
{
	BYTE bFlags;
	BYTE bPadding;
	WORD wStreams;
};


struct CLRMetadataStreamHeader
{
	DWORD dwOffset;
	DWORD dwSize;
};


struct CLRStringHeapEntry
{
	SiUtils::SiString sStr;
	DWORD dwOffset;
};

typedef SiUtils::SiArray<CLRStringHeapEntry> CLRStringHeap;


struct CLRGuidHeapEntry
{
	BYTE pbGuid[16];
	DWORD dwOffset;
};

typedef SiUtils::SiArray<CLRGuidHeapEntry> CLRGuidHeap;


struct CLRBlobHeapEntry
{
	DWORD dwSize;
	BYTE* pbData;
	DWORD dwOffset;
	DWORD dwTotalSize;
};

typedef SiUtils::SiArray<CLRBlobHeapEntry> CLRBlobHeap;


struct CLRMetadataTableHeader
{
	DWORD dwReserved;
	BYTE bMajorVersion;
	BYTE bMinorVersion;
	BYTE bHeapFlags;
	BYTE bMaxRecordBitCount;
	BYTE pfTableValid[8];
	BYTE pfTableSorted[8];
};


class PEFile;
class PESection;

class CLRMetadata
{
	PEFile* m_pFile;

	CLRMetadataHeader m_mhHeader;
	char* m_pszVersionString;
	CLRMetadataStorageHeader m_mshStorageHeader;
	CLRMetadataTableHeader m_mthTableHeader;

	CLRStringHeap m_shStringHeap;
	CLRGuidHeap m_ghGuidHeap;
	CLRBlobHeap m_bhBlobHeap, m_bhUserStringHeap;
	const BYTE* m_pbOrigMetadataTables;
	bool m_fCompressedTables;

	CLRTable m_ptTable[CLR_METADATA_TABLE_COUNT];

	DWORD readCompressedInt(const BYTE* pbData, DWORD& dwOffset);
	bool writeCompressedInt(PESection* pSection, DWORD dwValue);
	DWORD getCompressedIntSize(DWORD dwValue);

	bool readStringHeap(CLRStringHeap& shHeap, const BYTE* pbData, DWORD dwSize);
	bool readGuidHeap(CLRGuidHeap& ghHeap, const BYTE* pbData, DWORD dwSize);
	bool readBlobHeap(CLRBlobHeap& bhHeap, const BYTE* pbData, DWORD dwSize);

	DWORD minBitCount(DWORD dwValue);
	bool readSingleTable(const BYTE* pbData, DWORD* pdwTableRecordCount, DWORD dwTableIndex,
		DWORD& dwTableOffset);
	bool readTables(const BYTE* pbData, DWORD* pdwTableRecordCount);
	bool saveTable(DWORD dwTableIndex, PESection* pSection);

public:
	CLRMetadata();
	~CLRMetadata();

	bool parse(PEFile* pFile, DWORD dwMetadataRVA);
	bool save(PESection* pSection);

	SiUtils::SiString getString(const DWORD dwOffset);
	const CLRBlobHeapEntry* getBlob(const DWORD dwOffset);
	const CLRGuidHeapEntry* getGuid(const DWORD dwOffset);

	DWORD getUserStringCount();
	const CLRBlobHeapEntry* getUserStringFromIndex(const DWORD dwIndex);

	DWORD addString(const SiUtils::SiString& sStr);
	DWORD addUserString(const SiUtils::SiString& sStr);
	DWORD addUserString(const BYTE* pbData, DWORD dwSize);
	DWORD addBlob(const BYTE* pbData, DWORD dwSize);
	DWORD addGuid(const BYTE* guid);

	PEFile& getFile();
	CLRTable& getTable(const int index);

	DWORD getStringHeapSize();
	DWORD getGuidHeapSize();
	DWORD getBlobHeapSize();
	DWORD getUserStringHeapSize();

	bool isAtLeastVersion(DWORD dwMajor, DWORD dwMinor, DWORD dwBuild);

	void printTables();
};


#endif
