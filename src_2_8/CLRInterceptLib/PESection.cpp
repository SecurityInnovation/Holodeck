//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		PESection.cpp
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

#include <windows.h>
#include "PESection.h"


//*************************************************************************
// Method:	 PESection::PESection
// Description: Default constructor for the PESection class
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
PESection::PESection()
{
	// Zero out the header information
	memset(&m_shHeader, 0, sizeof(IMAGE_SECTION_HEADER));

	// Initialize section data to NULL
	m_pbData = NULL;
	m_dwAllocatedDataSize = 0;
}


//*************************************************************************
// Method:	 PESection::~PESection
// Description: Destructor for the PESection class
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
PESection::~PESection()
{
	// Free section data if it exists
	if (m_pbData)
	{
		delete[] m_pbData;
		m_pbData = NULL;
	}
}


//*************************************************************************
// Method:	 PESection::loadNext
// Description: Reads a section header from the current location in a
//				file, and loads the raw data which that section points
//				to.  Call this function repeatedly to load all sections
//				in the file.
// Parameters:
//	hFile - Handle to the executable file
//
// Return Value: true on success, false on failure.  Current location in
//				 the file will now be the beginning of the next section
//				 header, allowing this function to be immediately called
//				 again to retrieve the next section.
//*************************************************************************
bool PESection::loadNext(HANDLE hFile)
{
	DWORD dwReadCount;

	// Read the section header from the file
	if (!ReadFile(hFile, &m_shHeader, sizeof(IMAGE_SECTION_HEADER), &dwReadCount, NULL))
		return false;
	if (dwReadCount != sizeof(IMAGE_SECTION_HEADER))
		return false;

	// Free any existing data
	if (m_pbData)
		delete[] m_pbData;

	// Allocate data storage for the loaded section
	m_dwAllocatedDataSize = m_shHeader.SizeOfRawData;
	m_pbData = new BYTE[m_dwAllocatedDataSize];
	if (!m_pbData)
		return false;

	// Obtain the current file pointer and save it so that we can return to the section
	// header area later
	DWORD dwOldOffset = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	if (dwOldOffset == INVALID_SET_FILE_POINTER)
		return false;

	// Read the section's raw data in the new buffer
	if (SetFilePointer(hFile, m_shHeader.PointerToRawData, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return false;
	if (!ReadFile(hFile, m_pbData, m_shHeader.SizeOfRawData, &dwReadCount, NULL))
		return false;
	if (dwReadCount != m_shHeader.SizeOfRawData)
		return false;

	// Seek back to the saved file pointer location.  This location points to the start
	// of the next section's header.  This allows the caller to simply call loadNext
	// again to read the next section.
	if (SetFilePointer(hFile, dwOldOffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		return false;

	return true;
}


//*************************************************************************
// Method:	 PESection::create
// Description: Creates a new section
// Parameters:
//	pszName - Name of the new section
//  dwSectionFlags - Characteristics of the new section (read, write
//					 shared, etc.).  See PE format documentation for
//					 a list of possible flags.
//
// Return Value: None.  This PESection instance now represents the new
//				 section.  Note that the virtual address and file offset
//				 of the section is not yet defined.  Call setVirtualAddress
//				 and setRawDataOffset to set up this information.
//*************************************************************************
void PESection::create(char* pszName, DWORD dwSectionFlags)
{
	strncpy((char*)m_shHeader.Name, pszName, 8);
	m_shHeader.Characteristics = dwSectionFlags;
}


//*************************************************************************
// Method:	 PESection::isAddressValid
// Description: Determines if the virtual address information is valid
// Parameters:
//	None
//
// Return Value: true if the virtual address information for this
//				 PESection instance is valid, false otherwise
//*************************************************************************
bool PESection::isAddressValid()
{
	if (m_shHeader.VirtualAddress == 0)
		return false;
	return true;
}


//*************************************************************************
// Method:	 PESection::getStartVirtualAddress
// Description: Gets the starting virtual address of this section
// Parameters:
//	None
//
// Return Value: Starting virtual address
//*************************************************************************
DWORD PESection::getStartVirtualAddress()
{
	return m_shHeader.VirtualAddress;
}


//*************************************************************************
// Method:	 PESection::getEndVirtualAddress
// Description: Gets the ending virtual address of this section
// Parameters:
//	None
//
// Return Value: Ending virtual address
//*************************************************************************
DWORD PESection::getEndVirtualAddress()
{
	return m_shHeader.VirtualAddress + m_shHeader.Misc.VirtualSize;
}


//*************************************************************************
// Method:	 PESection::getVirtualSize
// Description: Returns how much virtual address space is used by this
//				section
// Parameters:
//	None
//
// Return Value: Virtual size of this section
//*************************************************************************
DWORD PESection::getVirtualSize()
{
	return m_shHeader.Misc.VirtualSize;
}


//*************************************************************************
// Method:	 PESection::getStartRawDataOffset
// Description: Gets the starting offset within the file for this section
// Parameters:
//	None
//
// Return Value: Starting offset within the file
//*************************************************************************
DWORD PESection::getStartRawDataOffset()
{
	return m_shHeader.PointerToRawData;
}


//*************************************************************************
// Method:	 PESection::getEndRawDataOffset
// Description: Gets the ending offset within the file for this section
// Parameters:
//	None
//
// Return Value: Ending offset within the file
//*************************************************************************
DWORD PESection::getEndRawDataOffset()
{
	return m_shHeader.PointerToRawData + m_shHeader.SizeOfRawData;
}


//*************************************************************************
// Method:	 PESection::getRawDataSize
// Description: Gets the actual size in bytes of the data in this section
// Parameters:
//	None
//
// Return Value: Raw data size
//*************************************************************************
DWORD PESection::getRawDataSize()
{
	return m_shHeader.SizeOfRawData;
}


//*************************************************************************
// Method:	 PESection::setVirtualAddress
// Description: Sets the starting virtual address of this section, also
//				updating the virtual size based on the raw data size.
// Parameters:
//	dwStartVirtualAddr - New starting virtual address
//
// Return Value: None
//*************************************************************************
void PESection::setVirtualAddress(DWORD dwStartVirtualAddr)
{
	m_shHeader.VirtualAddress = dwStartVirtualAddr;
	m_shHeader.Misc.VirtualSize = m_shHeader.SizeOfRawData;
}


//*************************************************************************
// Method:	 PESection::setRawDataOffset
// Description: Sets the starting offset within the file for this section
// Parameters:
//	dwStartRawDataOffset - New starting offset within the file
//
// Return Value: None
//*************************************************************************
void PESection::setRawDataOffset(DWORD dwStartRawDataOffset)
{
	m_shHeader.PointerToRawData = dwStartRawDataOffset;
}


//*************************************************************************
// Method:	 PESection::getData
// Description: Returns the data in this section
// Parameters:
//	None
//
// Return Value: Pointer to the data in this section
//*************************************************************************
const BYTE* PESection::getData()
{
	return m_pbData;
}


//*************************************************************************
// Method:	 PESection::getSectionHeader
// Description: Returns the section header for this section
// Parameters:
//	None
//
// Return Value: Reference to the section header
//*************************************************************************
const IMAGE_SECTION_HEADER& PESection::getSectionHeader()
{
	return m_shHeader;
}


//*************************************************************************
// Method:	 PESection::appendData
// Description: Appends data to the end of a section
// Parameters:
//	pbData - Buffer containing data to append
//  dwLen - Length of data to append
//
// Return Value: true on success, false on failure
//*************************************************************************
bool PESection::appendData(const void* pbData, DWORD dwLen)
{
	if ((m_shHeader.SizeOfRawData + dwLen) > m_dwAllocatedDataSize)
	{
		// Buffer isn't large enough to hold the appended data.  Enlarge the buffer to
		// 1.5 times the necessary amount.
		DWORD dwNewSize = ((m_shHeader.SizeOfRawData + dwLen) * 3) / 2;
		PBYTE pbNewBuffer = new BYTE[dwNewSize];

		if (!pbNewBuffer)
			return false;

		// Copy the old data into the new buffer, and delete the old buffer
		memcpy(pbNewBuffer, m_pbData, m_shHeader.SizeOfRawData);
		delete[] m_pbData;

		// Update the size and buffer members
		m_dwAllocatedDataSize = dwNewSize;
		m_pbData = pbNewBuffer;
	}

	// Buffer is large enough.  Append the data.
	memcpy(m_pbData + m_shHeader.SizeOfRawData, pbData, dwLen);
	m_shHeader.SizeOfRawData += dwLen;
	m_shHeader.Misc.VirtualSize = (m_shHeader.SizeOfRawData + 4095) & (~4095); // Align to page boundary
	return true;
}


//*************************************************************************
// Method:	 PESection::modifyData
// Description: Modifies data in a section
// Parameters:
//	dwOffset - Starting offset at which to modify
//  pbData - Buffer that will replace the data currently at the offset
//			 being modified
//  dwLen - Length of data to modify
//
// Return Value: true on success, false on failure
//*************************************************************************
bool PESection::modifyData(DWORD dwOffset, const void* pbData, DWORD dwLen)
{
	if ((dwOffset > m_shHeader.SizeOfRawData) ||
		(dwLen > m_shHeader.SizeOfRawData) ||
		((dwOffset + dwLen) > m_shHeader.SizeOfRawData))
	{
		// Attempted to modify data that is past the end of the section
		return false;
	}

	// Copy the new data into the desired location
	memcpy(m_pbData + dwOffset, pbData, dwLen);
	return true;
}


//*************************************************************************
// Method:	 PESection::getChecksum
// Description: Computes the checksum for a section
// Parameters:
//	None
//
// Return Value: Checksum
//*************************************************************************
DWORD PESection::getChecksum()
{
	DWORD dwSum = 0;
	for (DWORD i=0; i<m_shHeader.SizeOfRawData; i++)
	{
		// Rotate left once and add the next byte
		dwSum = (dwSum << 1) | (dwSum >> 31);
		dwSum += (DWORD)m_pbData[i];
	}
	return dwSum;
}
