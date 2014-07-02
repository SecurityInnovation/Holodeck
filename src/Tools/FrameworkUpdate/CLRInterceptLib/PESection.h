//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		PESection.h
//
// DESCRIPTION: Contains classes to manage sections in a PE executable
//				file.
//
//				See http://msdn.microsoft.com/library/en-us/dndebug/html/msdn_peeringpe.asp
//				for details on the PE format.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Mar 2003	103	 R. Wagner	 File created.
//*************************************************************************

#ifndef __PESECTION_H__
#define __PESECTION_H__

#include <windows.h>


class PESection
{
	IMAGE_SECTION_HEADER m_shHeader;
	PBYTE m_pbData;
	DWORD m_dwAllocatedDataSize;

public:
	PESection();
	~PESection();

	bool loadNext(HANDLE hFile);
	void create(char* pszName, DWORD dwSectionFlags);

	bool isAddressValid();

	DWORD getStartVirtualAddress();
	DWORD getEndVirtualAddress();
	DWORD getVirtualSize();

	DWORD getStartRawDataOffset();
	DWORD getEndRawDataOffset();
	DWORD getRawDataSize();

	const IMAGE_SECTION_HEADER& getSectionHeader();

	void setVirtualAddress(DWORD dwStartVirtualAddr);
	void setRawDataOffset(DWORD dwStartRawDataOffset);

	const BYTE* getData();
	bool appendData(const void* pbData, DWORD dwLen);
	bool modifyData(DWORD dwOffset, const void* pbData, DWORD dwLen);

	DWORD getChecksum();
};


#endif
