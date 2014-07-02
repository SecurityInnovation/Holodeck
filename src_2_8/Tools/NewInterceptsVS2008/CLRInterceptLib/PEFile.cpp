//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		PEFile.cpp
//
// DESCRIPTION: Contains classes to manage a PE executable file, with support
//				for the .NET runtime.
//
//				See http://msdn.microsoft.com/library/en-us/dndebug/html/msdn_peeringpe.asp
//				for details on the PE format.
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Mar 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include "PEFile.h"
#include "PESection.h"

using namespace SiUtils;


//*************************************************************************
// Method:	 PEFile::PEFile
// Description: Default constructor for PEFile class
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
PEFile::PEFile()
{
	m_pbDOSData = NULL;
	m_pReplacedILSection = NULL;
	m_bNeedHashTableUpdate = true;
}


//*************************************************************************
// Method:	 PEFile::~PEFile
// Description: Destructor for PEFile class
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
PEFile::~PEFile()
{
	// Delete section data
	for (DWORD i=0; i<m_vpSections.GetLength(); i++)
		delete m_vpSections[i];
	m_vpSections.Clear();

	// Delete data between DOS and NT headers
	if (m_pbDOSData)
	{
		delete[] m_pbDOSData;
		m_pbDOSData = NULL;
	}
}


//*************************************************************************
// Method:	 PEFile::load
// Description: Parses an executable file into its component sections, and
//				parses the .NET metadata if it is present.
// Parameters:
//	szFileName - Path to the executable file
//  fParseFile - true if metadata and managed type information should be parsed.
//		Pass false here if you just want to check the type of the executable.
//
// Return Value: true on success, false on failure
//*************************************************************************
bool PEFile::load(const SiString& szFileName, bool fParseFile)
{
	DWORD dwReadCount;

	// Open the file
	HANDLE hFile = CreateFile((char*)szFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// Read the MZ (DOS) header into memory
	if ((!ReadFile(hFile, &m_mzDOSHeader, sizeof(IMAGE_DOS_HEADER), &dwReadCount, NULL)) ||
		(dwReadCount != sizeof(IMAGE_DOS_HEADER)))
	{
		CloseHandle(hFile);
		return false;
	}

	// Is it a valid DOS header?
	if (m_mzDOSHeader.e_magic != IMAGE_DOS_SIGNATURE) // Should be "MZ"
	{
		CloseHandle(hFile);
		return false;
	}

	// Calculate size of data between MZ (DOS) and PE (NT) headers and read the data
	m_dwDOSDataLen = m_mzDOSHeader.e_lfanew - sizeof(IMAGE_DOS_HEADER);
	if (m_dwDOSDataLen != 0)
	{
		// Read data between the MZ (DOS) header and the PE (NT) header
		m_pbDOSData = new BYTE[m_dwDOSDataLen];
		if ((!ReadFile(hFile, m_pbDOSData, m_dwDOSDataLen, &dwReadCount, NULL)) ||
			(dwReadCount != m_dwDOSDataLen))
		{
			CloseHandle(hFile);
			return false;
		}
	}
	else
		m_pbDOSData = NULL;

	// Read the PE (NT) header
	if ((!ReadFile(hFile, &m_peNTHeader, sizeof(IMAGE_NT_HEADERS), &dwReadCount, NULL)) ||
		(dwReadCount != sizeof(IMAGE_NT_HEADERS)))
	{
		CloseHandle(hFile);
		return false;
	}

	// Is it a valid NT header?
	if (m_peNTHeader.Signature != IMAGE_NT_SIGNATURE) // Should be "PE\0\0"
	{
		CloseHandle(hFile);
		return false;
	}

	// Seek to the section list
	if (SetFilePointer(hFile, m_mzDOSHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS), NULL, FILE_BEGIN) ==
		INVALID_SET_FILE_POINTER)
	{
		CloseHandle(hFile);
		return false;
	}

	// Read in each section
	for (DWORD i=0; i<m_peNTHeader.FileHeader.NumberOfSections; i++)
	{
		PESection* pSection = new PESection();
		if (!pSection->loadNext(hFile))
		{
			CloseHandle(hFile);
			return false;
		}
		m_vpSections.Add(pSection);
	}

	// Done with the file handle
	CloseHandle(hFile);

	// Determine if a managed header exists
	m_fManaged = false;
	if ((m_peNTHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COMHEADER].
		VirtualAddress != 0) && (m_peNTHeader.OptionalHeader.DataDirectory[
		IMAGE_DIRECTORY_ENTRY_COMHEADER].Size != 0))
	{
		// Managed header found.  Get a pointer to the header.
		m_fManaged = true;
		m_pchManagedHeader = (PIMAGE_COR20_HEADER)rvaToPointer(m_peNTHeader.OptionalHeader.
			DataDirectory[IMAGE_DIRECTORY_ENTRY_COMHEADER].VirtualAddress);

		if (m_pchManagedHeader == NULL)
		{
			// RVA of managed header was invalid
			return false;
		}

		// Parse the metadata if we were asked to
		if (fParseFile)
		{
			if (!m_mdMetadata.parse(this, m_pchManagedHeader->MetaData.VirtualAddress))
			{
				// Managed metadata was invalid
				return false;
			}
		}
	}
	return true;
}


//*************************************************************************
// Function:	writePadding
// Description: Pads the file up to the specified offset with zero bytes
// Parameters:
//	hFile - File to pad.
//  dwOffset - File offset to pad up to.
//
// Return Value: true on success, false on failure
//*************************************************************************
bool writePadding(HANDLE hFile, DWORD dwOffset)
{
	DWORD dwCurOffset = SetFilePointer(hFile, 0, NULL, FILE_CURRENT);
	if (dwCurOffset == INVALID_SET_FILE_POINTER)
		return false;

	while (dwCurOffset < dwOffset)
	{
		BYTE bZero = 0;
		DWORD dwWriteCount;

		if ((!WriteFile(hFile, &bZero, 1, &dwWriteCount, NULL)) || (dwWriteCount != 1))
			return false;

		dwCurOffset++;
	}
	return true;
}


//*************************************************************************
// Method:	 PEFile::save
// Description: Saves the executable file, which may be modified, to disk.
// Parameters:
//	szFileName - New path to the executable file.  If the file already
//				 exists, it will be overwritten.
//
// Return Value: true on success, false on failure
//*************************************************************************
bool PEFile::save(const SiString& szFileName)
{
	DWORD dwWriteCount;

	// Open the file
	HANDLE hFile = CreateFile((char*)szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	// Get file offset of the end of the section headers
	DWORD dwSectionHeaderEndOffset = m_mzDOSHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS) +
		(sizeof(IMAGE_SECTION_HEADER) * (DWORD)m_vpSections.GetLength());
	if (dwSectionHeaderEndOffset == INVALID_SET_FILE_POINTER)
	{
		CloseHandle(hFile);
		return false;
	}

	// Adjust file offsets of sections such that the section headers don't overflow into the section data itself
	while (dwSectionHeaderEndOffset > getFirstSectionFileOffset())
	{
		// We're in the middle of a section, move the sections forward to make room for the section headers
		for (DWORD i=0; i<m_vpSections.GetLength(); i++)
		{
			// Move each section forward 512 bytes if it is present in the file
			if (m_vpSections[i]->getRawDataSize() > 0)
				m_vpSections[i]->setRawDataOffset(m_vpSections[i]->getStartRawDataOffset() + 512);
		}
		m_peNTHeader.OptionalHeader.SizeOfHeaders += 512;
	}

	// Write the MZ (DOS) header to the file
	if ((!WriteFile(hFile, &m_mzDOSHeader, sizeof(IMAGE_DOS_HEADER), &dwWriteCount, NULL)) ||
		(dwWriteCount != sizeof(IMAGE_DOS_HEADER)))
	{
		CloseHandle(hFile);
		return false;
	}

	// Write the data between the MZ (DOS) header and the PE (NT) header to the file
	if ((!WriteFile(hFile, m_pbDOSData, m_dwDOSDataLen, &dwWriteCount, NULL)) ||
		(dwWriteCount != m_dwDOSDataLen))
	{
		CloseHandle(hFile);
		return false;
	}

	// Write the PE (NT) header to the file
	if ((!WriteFile(hFile, &m_peNTHeader, sizeof(IMAGE_NT_HEADERS), &dwWriteCount, NULL)) ||
		(dwWriteCount != sizeof(IMAGE_NT_HEADERS)))
	{
		CloseHandle(hFile);
		return false;
	}

	// Section headers follow the NT header
	for (DWORD i=0; i<m_vpSections.GetLength(); i++)
	{
		// Write out each section header
		if ((!WriteFile(hFile, &m_vpSections[i]->getSectionHeader(), sizeof(IMAGE_SECTION_HEADER), &dwWriteCount, NULL)) ||
			(dwWriteCount != sizeof(IMAGE_SECTION_HEADER)))
		{
			CloseHandle(hFile);
			return false;
		}
	}

	// Section data follows section headers
	for (DWORD i=0; i<m_vpSections.GetLength(); i++)
	{
		// Pad the file up to the file offset found in the section header
		if (!writePadding(hFile, m_vpSections[i]->getStartRawDataOffset()))
		{
			CloseHandle(hFile);
			return false;
		}

		// Write out the section data
		if ((!WriteFile(hFile, m_vpSections[i]->getData(), m_vpSections[i]->getRawDataSize(), &dwWriteCount, NULL)) ||
			(dwWriteCount != m_vpSections[i]->getRawDataSize()))
		{
			CloseHandle(hFile);
			return false;
		}
	}

	CloseHandle(hFile);
	return true;
}


//*************************************************************************
// Method:	 PEFile::saveMetadata
// Description: Saves a new copy of the .NET metadata, possibly modified,
//				in a new section.  Updates references so that the new copy
//				of the metadata is used instead of the old.
// Parameters:
//	None
//
// Return Value: true on success, false on failure
//*************************************************************************
bool PEFile::saveMetadata()
{
	// Create a new read-only shared section to store the metadata
	PESection* pNewSection = new PESection();
	pNewSection->create(".heatmd", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA);//IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_CODE);

	// Write the metadata to the new section
	if (!m_mdMetadata.save(pNewSection))
	{
		delete pNewSection;
		return false;
	}
//	const BYTE* pOldMetadata = rvaToPointer(m_pchManagedHeader->MetaData.VirtualAddress);
//	pNewSection->appendData(pOldMetadata, m_pchManagedHeader->MetaData.Size);

	// Keep the size of the metadata
	DWORD dwMetadataSize = pNewSection->getRawDataSize();

	// Pad the section to align it to a multiple of the file alignment size
	while (pNewSection->getRawDataSize() & (m_peNTHeader.OptionalHeader.FileAlignment - 1))
	{
		BYTE bZero = 0;
		pNewSection->appendData(&bZero, 1);
	}

	// Add the section to the section list
	m_vpSections.Add(pNewSection);
	m_peNTHeader.FileHeader.NumberOfSections++;

	// If there was a modified IL section created, update data sizes now
	if (m_pReplacedILSection)
	{
		// Pad the section to align it to a multiple of the file alignment size
		while (m_pReplacedILSection->getRawDataSize() & (m_peNTHeader.OptionalHeader.FileAlignment - 1))
		{
			BYTE bZero = 0;
			m_pReplacedILSection->appendData(&bZero, 1);
		}
	
		// Force an update of the virtual size
		m_pReplacedILSection->setVirtualAddress(m_pReplacedILSection->getStartVirtualAddress());

		// Update data size
		m_peNTHeader.OptionalHeader.SizeOfInitializedData += m_pReplacedILSection->getRawDataSize();
		// Update image size.  Image size must be a multiple of the section alignment, so round up to the
		// nearest multiple of the section alignment.
		m_peNTHeader.OptionalHeader.SizeOfImage += (m_pReplacedILSection->getVirtualSize() +
			m_peNTHeader.OptionalHeader.SectionAlignment - 1) & (~(m_peNTHeader.OptionalHeader.SectionAlignment - 1));
	}

	// Layout the sections so that the new section gets a virtual address
	layoutSections();

	// Update data size
	m_peNTHeader.OptionalHeader.SizeOfInitializedData += pNewSection->getRawDataSize();
	// Update image size.  Image size must be a multiple of the section alignment, so round up to the
	// nearest multiple of the section alignment.
	m_peNTHeader.OptionalHeader.SizeOfImage += (pNewSection->getVirtualSize() +
		m_peNTHeader.OptionalHeader.SectionAlignment - 1) & (~(m_peNTHeader.OptionalHeader.SectionAlignment - 1));

	// Change the managed header to point at the new metadata
	m_pchManagedHeader->MetaData.VirtualAddress = pNewSection->getStartVirtualAddress();
	m_pchManagedHeader->MetaData.Size = dwMetadataSize;
	return true;
}


//*************************************************************************
// Method:	 PEFile::layoutSections
// Description: Gives virtual addresses and file offsets to sections that
//				don't already have them.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void PEFile::layoutSections()
{
	DWORD dwCurVirtualAddr = 0;
	DWORD dwCurFileOffset = 0;

	for (DWORD i=0; i<m_vpSections.GetLength(); i++)
	{
		if (!m_vpSections[i]->isAddressValid())
		{
			m_vpSections[i]->setVirtualAddress(dwCurVirtualAddr);
			m_vpSections[i]->setRawDataOffset(dwCurFileOffset);
		}

		// Place the next section just after this section
		dwCurVirtualAddr = m_vpSections[i]->getEndVirtualAddress();
		dwCurFileOffset = m_vpSections[i]->getEndRawDataOffset();
		// Align virtual addresses on the boundaries specified in the PE header
		dwCurVirtualAddr = (dwCurVirtualAddr + m_peNTHeader.OptionalHeader.SectionAlignment - 1) &
			(~(m_peNTHeader.OptionalHeader.SectionAlignment - 1));
		dwCurFileOffset = (dwCurFileOffset + m_peNTHeader.OptionalHeader.FileAlignment - 1) &
			(~(m_peNTHeader.OptionalHeader.FileAlignment - 1));
	}
}


//*************************************************************************
// Method:	 PEFile::isManagedExecutable
// Description: Determines if the executable loaded into this instance
//				of PEFile is based on the .NET runtime.
// Parameters:
//	None
//
// Return Value: true if the executable is based on the .NET runtime,
//				 false if it is a native executable.
//*************************************************************************
bool PEFile::isManagedExecutable()
{
	return m_fManaged;
}


CLRMetadata& PEFile::getMetadata()
{
	return m_mdMetadata;
}


//*************************************************************************
// Method:	 PEFile::rvaToPointer
// Description: Method to convert an RVA (Relative Virtual Address)
//				found in an executable file to a pointer to the correct
//				section's data.
// Parameters:
//	dwRVA - RVA of data
//
// Return Value: Pointer to the data requested, or NULL on failure
//*************************************************************************
const BYTE* PEFile::rvaToPointer(DWORD dwRVA)
{
	// Find the section containing the RVA we are looking for
	for (WORD i=0; i<m_vpSections.GetLength(); i++)
	{
		if (!m_vpSections[i]->isAddressValid())
			continue;
		if ((dwRVA >= m_vpSections[i]->getStartVirtualAddress()) && (dwRVA < m_vpSections[i]->getEndVirtualAddress()))
		{
			// Found the section.  Return a pointer to the requested address.
			return m_vpSections[i]->getData() + (dwRVA - m_vpSections[i]->getStartVirtualAddress());
		}
	}
	// RVA was not found
	return NULL;
}


//*************************************************************************
// Method:	 PEFile::fileOffsetToPointer
// Description: Method to convert a file offset within an executable file
//				to a pointer to the correct section's data.
// Parameters:
//	dwFileOffset - File offset of data
//
// Return Value: Pointer to the data requested, or NULL on failure
//*************************************************************************
const BYTE* PEFile::fileOffsetToPointer(DWORD dwFileOffset)
{
	// Find the section containing the file offset we are looking for
	for (WORD i=0; i<m_vpSections.GetLength(); i++)
	{
		if (!m_vpSections[i]->isAddressValid())
			continue;
		if ((dwFileOffset >= m_vpSections[i]->getStartRawDataOffset()) &&
			(dwFileOffset < m_vpSections[i]->getEndRawDataOffset()))
		{
			// Found the section.  Return a pointer to the requested address.
			return m_vpSections[i]->getData() + (dwFileOffset - m_vpSections[i]->getStartRawDataOffset());
		}
	}
	// Offset was not found
	return NULL;
}


//*************************************************************************
// Method:	 PEFile::getFirstSectionFileOffset
// Description: Finds the file offset of the first section present in the
//				file, skipping sections that do not have raw data present.
// Parameters:
//	None
//
// Return Value: File offset of first section present in the file, or
//	0xffffffff if there are no sections present in the file
//*************************************************************************
DWORD PEFile::getFirstSectionFileOffset()
{
	for (DWORD i=0; i<m_vpSections.GetLength(); i++)
	{
		if (m_vpSections[i]->getRawDataSize() != 0)
			return m_vpSections[i]->getStartRawDataOffset();
	}
	return 0xffffffff;
}


//*************************************************************************
// Method:	 PEFile::findMethod
// Description: Finds a method in a .NET executable and returns a pointer
//				to a CLRMethod object containing the method's code.
// Parameters:
//	mdMember - CLRMemberDescription structure defining the name and type
//		of the method to find.  The dwFlags member is ignored.
//  localOnly - If true, only locally defined methods will be searched
//
// Return Value: Pointer to a CLRMethod object if the method is found,
//	NULL otherwise.  The caller is repsonsible for freeing the CLRMethod
//  object when it is no longer needed.
//*************************************************************************
CLRMethod* PEFile::findMethod(const CLRMemberDescription& mdMember, bool localOnly)
{
	if (!m_fManaged)
		return NULL;

	updateHashTables();

	SiString fullName;

	if (mdMember.tnType.sOuterClass.GetLength() > 0)
	{
		fullName = mdMember.tnType.sOuterClass;
		fullName += "+";
		fullName += mdMember.tnType.sName;
	}
	else
	{
		fullName = mdMember.tnType.sNamespace;
		fullName += ".";
		fullName += mdMember.tnType.sName;
	}

	if (!m_htTypeDefHash.KeyExists(fullName))
		return NULL;
	DWORD i = m_htTypeDefHash[fullName];

	// Get index of first method.  Subtract one as RIDs in the tables are one indexed.
	DWORD dwMethodStartIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i][CLR_TYPEDEF_METHODLIST_COLINDEX] - 1;
	
	// Index of last method is determined by the first method of the next type in the type table.  If this
	// is the last type, all of the rest of the methods in the method table belong to this type.
	DWORD dwMethodEndIndex;
	if ((i + 1) < m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount())
	{
		// Subtract one as RIDs in the tables are one indexed.
		dwMethodEndIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i + 1][CLR_TYPEDEF_METHODLIST_COLINDEX] - 1;
	}
	else
		dwMethodEndIndex = m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount();

	// Find the method
	for (DWORD j=dwMethodStartIndex; j<dwMethodEndIndex; j++)
	{
		if (localOnly)
		{
			// Skip abstract or externally defined methods
			if (m_mdMetadata.getTable(CLR_TABLE_METHOD)[j][CLR_METHOD_RVA_COLINDEX] == 0)
				continue;
		}

		if (m_mdMetadata.getTable(CLR_TABLE_METHOD)[j].getString(CLR_METHOD_NAME_COLINDEX) == (wchar_t*)mdMember.sName)
		{
			// Found a method with the correct name.  Get the signature.
			DWORD dwSigOffset = m_mdMetadata.getTable(CLR_TABLE_METHOD)[j][CLR_METHOD_SIGNATURE_COLINDEX];
			const CLRBlobHeapEntry* pSigBlob = m_mdMetadata.getBlob(dwSigOffset);
			if (!pSigBlob)
			{
				// Not a valid signature
				continue;
			}

			// Compare the signatures to see if this is the correct member
			CLRSignature sMemberSig;
			sMemberSig.parse(&getMetadata(), pSigBlob->pbData);
			if (sMemberSig == mdMember.sSignature)
			{
				// Signatures match, this is our member.
				CLRMethod* pMethod = new CLRMethod();
				if (!pMethod->parse(this, i, j))
				{
					// Error parsing method
					delete pMethod;
					return NULL;
				}

				// Method parsed, return the CLRMethod object
				return pMethod;
			}
		}
	}
	// Member not found in the class.  This was the correct class, so the member won't be found.
	return NULL;
}


//*************************************************************************
// Method:	 PEFile::findMemberRefToken
// Description: Finds the MemberRef token value for an external member in
//				a .NET executable.  This token is used to reference the
//				member in IL code.
// Parameters:
//	mdMember - CLRMemberDescription structure defining the name and type
//		of the member to find
//  fCreateIfNotFound - If true, create a MemberRef entry for this method
//		if the method isn't found
//
// Return Value: MemberRef token value to be used in IL code, zero on fail
//*************************************************************************
DWORD PEFile::findMemberRefToken(const CLRMemberDescription& mdMember, bool fCreateIfNotFound)
{
	if (!m_fManaged)
		return 0;

	updateHashTables();

	SiString fullName;

	if (mdMember.tnType.sOuterClass.GetLength() > 0)
	{
		fullName = mdMember.tnType.sOuterClass;
		fullName += "+";
		fullName += mdMember.tnType.sName;
	}
	else
	{
		fullName = mdMember.tnType.sNamespace;
		fullName += ".";
		fullName += mdMember.tnType.sName;
	}

	if (m_htTypeRefHash.KeyExists(fullName))
	{
		DWORD i = m_htTypeRefHash[fullName];

		// Found the class.  Try and find the member.
		for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_MEMBERREF).getRowCount(); j++)
		{
			if (m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[j].getRefTableIndex(
				CLR_MEMBERREF_CLASS_COLINDEX) != CLR_TABLE_TYPEREF)
			{
				// Only look at members contained in a TypeRef type
				continue;
			}

			if (m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[j].getRefRowIndex(CLR_MEMBERREF_CLASS_COLINDEX) != i)
			{
				// Only look at members contained in the type defined by the current TypeRef entry
				continue;
			}

			if (m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[j].getString(CLR_MEMBERREF_NAME_COLINDEX) == (wchar_t*)mdMember.sName)
			{
				// Found a member with the correct name.  Get the signature
				DWORD dwSigOffset = m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[j][CLR_MEMBERREF_SIGNATURE_COLINDEX];
				const CLRBlobHeapEntry* pSigBlob = m_mdMetadata.getBlob(dwSigOffset);
				if (!pSigBlob)
				{
					// Not a valid signature
					continue;
				}

				// Compare the signatures to see if this is the correct member
				CLRSignature sMemberSig;
				sMemberSig.parse(&getMetadata(), pSigBlob->pbData);
				if (sMemberSig == mdMember.sSignature)
				{
					// Signatures match, this is our member.  Return the token.  Note that tokens
					// use a one-based index, so add one to the row index.
					return CLR_TOKEN_MEMBERREF | (j + 1);
				}
			}
		}
		// Member not found in the class.  This was the correct class, so the member won't be found.
		if (fCreateIfNotFound)
		{
			// Need to create the MemberRef.  We already have the containing TypeRef
			CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_MEMBERREF));
			row.setRefTable(CLR_MEMBERREF_CLASS_COLINDEX, CLR_TABLE_TYPEREF, i);
			row.setString(CLR_MEMBERREF_NAME_COLINDEX, mdMember.sName);
			CLRSignature sSig = mdMember.sSignature;
			if (!sSig.save(&m_mdMetadata))
				return 0;
			row.setBlob(CLR_MEMBERREF_SIGNATURE_COLINDEX, sSig.getData(), sSig.getSize());
			// Add the new row to the MemberRef table
			DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_MEMBERREF).add(row);
			// Return the new token.  Tokens use a one-based index, so add one to the row index.
			return CLR_TOKEN_MEMBERREF | (dwRowIndex + 1);
		}
		return 0;
	}
	// Neither member nor type was found
	if (fCreateIfNotFound)
	{
		// Need to create both a TypeRef and a MemberRef.  Create the TypeRef first.
		CLRTableRow typeRow(&m_mdMetadata.getTable(CLR_TABLE_TYPEREF));
		typeRow.setToken(CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX, mdMember.dwAssemblyRefToken);
		typeRow.setString(CLR_TYPEREF_NAME_COLINDEX, mdMember.tnType.sName);
		typeRow.setString(CLR_TYPEREF_NAMESPACE_COLINDEX, mdMember.tnType.sNamespace);
		// Add the TypeRef row to the table
		DWORD dwTypeIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEREF).add(typeRow);
		m_htTypeRefHash.Add(fullName, dwTypeIndex);

		// Create the MemberRef row.
		CLRTableRow memberRow(&m_mdMetadata.getTable(CLR_TABLE_MEMBERREF));
		memberRow.setRefTable(CLR_MEMBERREF_CLASS_COLINDEX, CLR_TABLE_TYPEREF, dwTypeIndex);
		memberRow.setString(CLR_MEMBERREF_NAME_COLINDEX, mdMember.sName);
		CLRSignature sSig = mdMember.sSignature;
		if (!sSig.save(&m_mdMetadata))
			return 0;
		memberRow.setBlob(CLR_MEMBERREF_SIGNATURE_COLINDEX, sSig.getData(), sSig.getSize());
		// Add the new row to the MemberRef table
		DWORD dwMemberIndex = m_mdMetadata.getTable(CLR_TABLE_MEMBERREF).add(memberRow);
		// Return the new token.  Tokens use a one-based index, so add one to the row index.
		return CLR_TOKEN_MEMBERREF | (dwMemberIndex + 1);
	}
	return 0;
}


//*************************************************************************
// Method:	 PEFile::createUserStringToken
// Description: Creates a new user string in the .NET metadata and returns
//				a token value that references this string.
// Parameters:
//	sStr - String to add to the user string heap
//
// Return Value: String token value to be used in IL code, zero on fail
//*************************************************************************
DWORD PEFile::createUserStringToken(const SiUtils::SiString& sStr)
{
	if (!m_fManaged)
		return 0;

	DWORD dwStringOffset = m_mdMetadata.addUserString(sStr);
	return CLR_TOKEN_USERSTRING | dwStringOffset;
}


//*************************************************************************
// Method:	 PEFile::replaceMethod
// Description: Replaces the original IL code for the given method with
//				the new modified IL code.
// Parameters:
//	pMethod - Method to replace
//
// Return Value: true on success, false on failure
//*************************************************************************
bool PEFile::replaceMethod(CLRMethod* pMethod)
{
	if (!m_fManaged)
		return false;

	if (!m_pReplacedILSection)
	{
		// Create a new section to contain the modified IL code
		m_pReplacedILSection = new PESection();
		m_pReplacedILSection->create(".heatil", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA);
		m_vpSections.Add(m_pReplacedILSection);
		m_peNTHeader.FileHeader.NumberOfSections++;

		// Lay out the sections so that the replaced IL code section has a valid virtual address
		layoutSections();
	}

	// Calculate RVA to the new IL code
	DWORD dwMethodRVA = m_pReplacedILSection->getRawDataSize() + m_pReplacedILSection->getStartVirtualAddress();

	// Write out the IL code
	if (!pMethod->save(this, m_pReplacedILSection))
		return false;

	// Align code on a 4 byte boundary
	while (m_pReplacedILSection->getRawDataSize() & 3)
	{
		BYTE bZero = 0;
		m_pReplacedILSection->appendData(&bZero, 1);
	}

	// Update the metadata to point to the new IL code
	m_mdMetadata.getTable(CLR_TABLE_METHOD)[pMethod->getMethodIndex()][CLR_METHOD_RVA_COLINDEX] = dwMethodRVA;

	return true;
}


//*************************************************************************
// Method:	 PEFile::enumerateTypes
// Description: Returns a list of all managed types in a .NET executable
// Parameters:
//	None
//
// Return Value: List of CLRTypeName structures representing the names
//	of all types in the executable
//*************************************************************************
SiArray<CLRTypeName> PEFile::enumerateTypes()
{
	SiArray<CLRTypeName> vTypes;

	if (!m_fManaged)
		return vTypes;

	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount(); i++)
	{
		CLRTypeName name;

		name.sNamespace = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].getString(CLR_TYPEDEF_NAMESPACE_COLINDEX);
		name.sName = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].getString(CLR_TYPEDEF_NAME_COLINDEX);
		name.sOuterClass = getOuterClassName(CLR_TOKEN_TYPEDEF | (i + 1));

		vTypes.Add(name);
	}
	return vTypes;
}


//*************************************************************************
// Method:	 PEFile::enumerateMethods
// Description: Returns a list of all methods in a .NET type
// Parameters:
//	tnType - Enumeration will be performed on the type by this name
//
// Return Value: List of CLRMemberDescription structures representing
//	the names and signatures of all methods in the type
//*************************************************************************
SiArray<CLRMemberDescription> PEFile::enumerateMethods(CLRTypeName& tnType)
{
	SiArray<CLRMemberDescription> vMethods;

	if (!m_fManaged)
		return vMethods;

	updateHashTables();

	SiString fullName;

	if (tnType.sOuterClass.GetLength() > 0)
	{
		fullName = tnType.sOuterClass;
		fullName += "+";
		fullName += tnType.sName;
	}
	else
	{
		fullName = tnType.sNamespace;
		fullName += ".";
		fullName += tnType.sName;
	}

	if (!m_htTypeDefHash.KeyExists(fullName))
		return vMethods;
	DWORD i = m_htTypeDefHash[fullName];

	// Found the type we're looking for.  Get index of first method.  Subtract one as RIDs in
	// the tables are one indexed.
	DWORD dwMethodStartIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i][CLR_TYPEDEF_METHODLIST_COLINDEX] - 1;
	
	// Index of last method is determined by the first method of the next type in the type table.  If this
	// is the last type, all of the rest of the methods in the method table belong to this type.
	DWORD dwMethodEndIndex;
	if ((i + 1) < m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount())
	{
		// Subtract one as RIDs in the tables are one indexed.
		dwMethodEndIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i + 1][CLR_TYPEDEF_METHODLIST_COLINDEX] - 1;
	}
	else
		dwMethodEndIndex = m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount();

	// Enumerate the methods
	for (DWORD j=dwMethodStartIndex; j<dwMethodEndIndex; j++)
	{
		CLRMemberDescription member;

		member.tnType = tnType;
		member.sName = m_mdMetadata.getTable(CLR_TABLE_METHOD)[j].getString(CLR_METHOD_NAME_COLINDEX);

		// Find the signature
		DWORD dwSigOffset = m_mdMetadata.getTable(CLR_TABLE_METHOD)[j][CLR_METHOD_SIGNATURE_COLINDEX];
		const CLRBlobHeapEntry* pSigBlob = m_mdMetadata.getBlob(dwSigOffset);
		if (!pSigBlob)
		{
			// Not a valid signature
			continue;
		}
		member.sSignature.parse(&m_mdMetadata, pSigBlob->pbData);

		// Copy the flags
		member.dwFlags = m_mdMetadata.getTable(CLR_TABLE_METHOD)[j][CLR_METHOD_FLAGS_COLINDEX];

		// Add the method to the list
		vMethods.Add(member);
	}

	// Done with the type, so return the list
	return vMethods;
}


//*************************************************************************
// Method:	 PEFile::createAssemblyRef
// Description: Creates a new assembly reference to another .NET binary,
//				and returns the token value for this assembly reference.
// Parameters:
//	peRefBinary - PEFile object representing the .NET binary to reference
//
// Return Value: Token value of the new assembly reference, or zero on fail
//*************************************************************************
DWORD PEFile::createAssemblyRef(PEFile& peRefBinary)
{
	if (!m_fManaged)
		return 0;
	if (!peRefBinary.isManagedExecutable())
		return 0;

	// Read in the assembly information for the assembly being referenced
	DWORD dwMajorVersion = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0][CLR_ASSEMBLY_MAJORVERSION_COLINDEX];
	DWORD dwMinorVersion = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0][CLR_ASSEMBLY_MINORVERSION_COLINDEX];
	DWORD dwBuildNumber = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0][CLR_ASSEMBLY_BUILDNUMBER_COLINDEX];
	DWORD dwRevisionNumber = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0][CLR_ASSEMBLY_REVISIONNUMBER_COLINDEX];
	DWORD dwFlags = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0][CLR_ASSEMBLY_FLAGS_COLINDEX];
	DWORD dwPublicKeyOffset = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0][CLR_ASSEMBLY_PUBLICKEY_COLINDEX];
	SiString sName = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0].getString(CLR_ASSEMBLY_NAME_COLINDEX);
	SiString sLocale = peRefBinary.getMetadata().getTable(CLR_TABLE_ASSEMBLY)[0].getString(CLR_ASSEMBLY_LOCALE_COLINDEX);

	// Create a new AssemblyRef row in the assembly doing the referencing
	CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF));
	row[CLR_ASSEMBLYREF_MAJORVERSION_COLINDEX] = dwMajorVersion;
	row[CLR_ASSEMBLYREF_MINORVERSION_COLINDEX] = dwMinorVersion;
	row[CLR_ASSEMBLYREF_BUILDNUMBER_COLINDEX] = dwBuildNumber;
	row[CLR_ASSEMBLYREF_REVISIONNUMBER_COLINDEX] = dwRevisionNumber;
	row[CLR_ASSEMBLYREF_FLAGS_COLINDEX] = 1;
	row.setString(CLR_ASSEMBLYREF_NAME_COLINDEX, sName);
	row.setString(CLR_ASSEMBLYREF_LOCALE_COLINDEX, sLocale);
	row[CLR_ASSEMBLYREF_HASHVALUE_COLINDEX] = 0;

	// Create a new blob to contain the public key
	row[CLR_ASSEMBLYREF_PUBLICKEYORTOKEN_COLINDEX] = 0;
	if (dwPublicKeyOffset != 0)
	{
		const CLRBlobHeapEntry* pBlob = peRefBinary.getMetadata().getBlob(dwPublicKeyOffset);
		if (pBlob != NULL)
			row.setBlob(CLR_ASSEMBLYREF_PUBLICKEYORTOKEN_COLINDEX, pBlob->pbData, pBlob->dwSize);
	}

	// Add the row to the AssemblyRef table
	DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).add(row);
	// Tokens are one-indexed, so add one to the row index when creating the token.
	return CLR_TOKEN_ASSEMBLYREF | (dwRowIndex + 1);
}


//*************************************************************************
// Method:	 PEFile::getAttributeValue
// Description: Gets the value of a given attribute attached to a type.
// Parameters:
//	tnType - Name of the type that has the attribute attached
//  tnAttribute - Name of the type of attribute that is attached
//
// Return Value: Blob heap entry containing the initialization data for
//		this attribute, NULL if there is no attribute data.
//*************************************************************************
const CLRBlobHeapEntry* PEFile::getAttributeValue(CLRTypeName& tnType, CLRTypeName& tnAttribute)
{
	int iAttrTypeRefIndex = -1;

	if (!m_fManaged)
		return NULL;

	updateHashTables();

	SiString fullName;

	if (tnAttribute.sOuterClass.GetLength() > 0)
	{
		fullName = tnAttribute.sOuterClass;
		fullName += "+";
		fullName += tnAttribute.sName;
	}
	else
	{
		fullName = tnAttribute.sNamespace;
		fullName += ".";
		fullName += tnAttribute.sName;
	}

	if (!m_htTypeRefHash.KeyExists(fullName))
		return NULL;
	iAttrTypeRefIndex = m_htTypeRefHash[fullName];

	if (tnType.sOuterClass.GetLength() > 0)
	{
		fullName = tnType.sOuterClass;
		fullName += "+";
		fullName += tnType.sName;
	}
	else
	{
		fullName = tnType.sNamespace;
		fullName += ".";
		fullName += tnType.sName;
	}

	if (!m_htTypeDefHash.KeyExists(fullName))
		return NULL;
	DWORD i = m_htTypeDefHash[fullName];

	// Found the type.  Go through all the CustomAttribute rows and find
	// the attribute we are looking for.
	for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE).getRowCount(); j++)
	{
		// It must be applied to a TypeDef
		if (m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)[j].getRefTableIndex(
			CLR_CUSTOMATTRIBUTE_PARENT_COLINDEX) != CLR_TABLE_TYPEDEF)
			continue;
		// It must be applied to the TypeDef of our type
		if (m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)[j].getRefRowIndex(
			CLR_CUSTOMATTRIBUTE_PARENT_COLINDEX) != i)
			continue;

		// The attribute should be a MemberRef (always a constructor)
		if (m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)[j].getRefTableIndex(
			CLR_CUSTOMATTRIBUTE_TYPE_COLINDEX) != CLR_TABLE_MEMBERREF)
			continue;
		DWORD dwAttrMemberIndex = m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)[j].getRefRowIndex(
			CLR_CUSTOMATTRIBUTE_TYPE_COLINDEX);

		// Member's parent type should be a TypeRef
		if (m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[dwAttrMemberIndex].getRefTableIndex(
			CLR_MEMBERREF_CLASS_COLINDEX) != CLR_TABLE_TYPEREF)
			continue;
		// Attribute should be of the type we're looking for.  Check method index
		if (m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[dwAttrMemberIndex].getRefRowIndex(
			CLR_MEMBERREF_CLASS_COLINDEX) != iAttrTypeRefIndex)
			continue;

		// The attribute is of the correct type and applied to the correct type.
		// Return the value of the attribute.
		return m_mdMetadata.getBlob(m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)
			[j][CLR_CUSTOMATTRIBUTE_VALUE_COLINDEX]);
	}

	return NULL;
}


//*************************************************************************
// Method:	 PEFile::findTypeToken
// Description: Finds a type by name and returns the type's token value
// Parameters:
//	tnType - Name of type to find
//
// Return Value: Token value of the type, or zero on fail
//*************************************************************************
DWORD PEFile::findTypeToken(const CLRTypeName& tnType)
{
	updateHashTables();

	SiString fullName;

	if (tnType.sOuterClass.GetLength() > 0)
	{
		fullName = tnType.sOuterClass;
		fullName += "+";
		fullName += tnType.sName;
	}
	else
	{
		fullName = tnType.sNamespace;
		fullName += ".";
		fullName += tnType.sName;
	}

	if (m_htTypeDefHash.KeyExists(fullName))
		return CLR_TOKEN_TYPEDEF | (m_htTypeDefHash[fullName] + 1); // RIDs should be one-indexed

	if (m_htTypeRefHash.KeyExists(fullName))
		return CLR_TOKEN_TYPEREF | (m_htTypeRefHash[fullName] + 1); // RIDs should be one-indexed

	return 0;
}


//*************************************************************************
// Method:	 PEFile::createType
// Description: Creates a new type in the executable
// Parameters:
//	ntdType - Structure containing name, base class, method list, and
//		field list for the new type
//
// Return Value: Token value of the new type, or zero on fail
//*************************************************************************
DWORD PEFile::createType(CLRNewTypeDescription& ntdType)
{
	CLRTableRow trTypeRow(&m_mdMetadata.getTable(CLR_TABLE_TYPEDEF));
	trTypeRow[CLR_TYPEDEF_FLAGS_COLINDEX] = ntdType.dwFlags;
	trTypeRow.setString(CLR_TYPEDEF_NAMESPACE_COLINDEX, ntdType.tnTypeName.sNamespace);
	trTypeRow.setString(CLR_TYPEDEF_NAME_COLINDEX, ntdType.tnTypeName.sName);
	// Set RID of field list (one past the current last entry)
	trTypeRow[CLR_TYPEDEF_FIELDLIST_COLINDEX] = m_mdMetadata.getTable(CLR_TABLE_FIELD).getRowCount() + 1;
	// Set RID of method list (one past the current last entry)
	trTypeRow[CLR_TYPEDEF_METHODLIST_COLINDEX] = m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount() + 1;

	// Find the base type
	DWORD dwBaseTypeToken = findTypeToken(ntdType.tnBaseName);
	if (dwBaseTypeToken == 0)
	{
		// Only System.Object doesn't have a base class, and we shouldn't be adding
		// System.Object as a new type.
		return 0;
	}

	trTypeRow.setToken(CLR_TYPEDEF_EXTENDS_COLINDEX, dwBaseTypeToken);

	// Add the new TypeDef row
	DWORD dwTypeRowIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).add(trTypeRow);

	// Create field definitions
	for (DWORD i=0; i<ntdType.vmdFields.GetLength(); i++)
	{
		CLRTableRow trFieldRow(&m_mdMetadata.getTable(CLR_TABLE_FIELD));
		trFieldRow[CLR_FIELD_FLAGS_COLINDEX] = ntdType.vmdFields[i].dwFlags;
		trFieldRow.setString(CLR_FIELD_NAME_COLINDEX, ntdType.vmdFields[i].sName);
		// Save the signature for the field
		if (!ntdType.vmdFields[i].sSignature.save(&m_mdMetadata))
			return 0;
		trFieldRow.setBlob(CLR_FIELD_SIGNATURE_COLINDEX, ntdType.vmdFields[i].sSignature.getData(),
			ntdType.vmdFields[i].sSignature.getSize());

		// Add the Field row
		m_mdMetadata.getTable(CLR_TABLE_FIELD).add(trFieldRow);
	}

	// Create method definitions
	for (DWORD i=0; i<ntdType.vmdMethods.GetLength(); i++)
	{
		CLRTableRow trMethodRow(&m_mdMetadata.getTable(CLR_TABLE_METHOD));
		trMethodRow[CLR_METHOD_IMPLFLAGS_COLINDEX] = ntdType.vmdMethods[i].dwImplFlags;
		trMethodRow[CLR_METHOD_FLAGS_COLINDEX] = ntdType.vmdMethods[i].mdMember.dwFlags;
		trMethodRow.setString(CLR_METHOD_NAME_COLINDEX, ntdType.vmdMethods[i].mdMember.sName);
		trMethodRow[CLR_METHOD_PARAMLIST_COLINDEX] = m_mdMetadata.getTable(CLR_TABLE_PARAM).getRowCount() + 1;
		trMethodRow[CLR_METHOD_RVA_COLINDEX] = 0; // Will be filled in later with replaceMethod
		// Save the signature for the field
		if (!ntdType.vmdMethods[i].mdMember.sSignature.save(&m_mdMetadata))
			return 0;
		trMethodRow.setBlob(CLR_METHOD_SIGNATURE_COLINDEX, ntdType.vmdMethods[i].mdMember.sSignature.getData(),
			ntdType.vmdMethods[i].mdMember.sSignature.getSize());

		// Add the Method row
		DWORD dwMethodRowIndex = m_mdMetadata.getTable(CLR_TABLE_METHOD).add(trMethodRow);

		// Save the method code by calling replaceMethod for this method, but only if there actually is
		// method code.
		if (ntdType.vmdMethods[i].mMethod.getILCodeSize() > 0)
		{
			ntdType.vmdMethods[i].mMethod.setTypeIndex(dwTypeRowIndex);
			ntdType.vmdMethods[i].mMethod.setMethodIndex(dwMethodRowIndex);
			replaceMethod(&ntdType.vmdMethods[i].mMethod);
		}
	}

	updateHashTables();

	SiString fullName;
	if (ntdType.tnTypeName.sOuterClass.GetLength() > 0)
	{
		fullName = ntdType.tnTypeName.sOuterClass;
		fullName += "+";
		fullName += ntdType.tnTypeName.sName;
	}
	else
	{
		fullName = ntdType.tnTypeName.sNamespace;
		fullName += ".";
		fullName += ntdType.tnTypeName.sName;
	}

	m_htTypeDefHash.Add(fullName, dwTypeRowIndex);

	// Tokens are one-indexed, so add one to the row index when creating the token.
	return CLR_TOKEN_TYPEDEF | (dwTypeRowIndex + 1);
}


//*************************************************************************
// Method:	 PEFile::getChecksum
// Description: Computes the checksum for a file
// Parameters:
//	None
//
// Return Value: Checksum
//*************************************************************************
DWORD PEFile::getChecksum()
{
	DWORD dwSum = 0;
	for (DWORD i=0; i<m_vpSections.GetLength(); i++)
		dwSum += m_vpSections[i]->getChecksum();
	return dwSum;
}


const BYTE* PEFile::getImportTable()
{
	return rvaToPointer(m_peNTHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].
		VirtualAddress);
}


const BYTE* PEFile::getExportTable()
{
	return rvaToPointer(m_peNTHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].
		VirtualAddress);
}


//*************************************************************************
// Method:	 PEFile::enumerateNativeFunctions
// Description: Gets a list of native functions defined in the PE file
// Parameters:
//	None
//
// Return Value: List of native functions in the PE file
//*************************************************************************
SiArray<SiString> PEFile::enumerateNativeFunctions()
{
	SiArray<SiString> funcList;

	const IMAGE_EXPORT_DIRECTORY* pExportTable = (const IMAGE_EXPORT_DIRECTORY*)getExportTable();
	if (!pExportTable)
		return funcList;

	const DWORD* pFunctionNames = (const DWORD*)rvaToPointer(pExportTable->AddressOfNames);
	if (!pFunctionNames)
		return funcList;

	for (DWORD i=0; i<pExportTable->NumberOfNames; i++)
	{
		if (!pFunctionNames[i])
			continue;
		const char* pszName = (const char*)rvaToPointer(pFunctionNames[i]);
		if (!pszName)
			continue;
		funcList.Add(pszName);
	}

	return funcList;
}


SiUtils::SiArray<SiUtils::SiString> PEFile::enumerateImportedModules()
{
	SiArray<SiString> moduleList;

	const IMAGE_IMPORT_DESCRIPTOR* pImportTable = (const IMAGE_IMPORT_DESCRIPTOR*)getImportTable();
	if (!pImportTable)
		return moduleList;

	while (pImportTable->Name)
	{
		const char* pszName = (const char*)rvaToPointer(pImportTable->Name);
		if (!pszName)
			continue;
		moduleList.Add(pszName);
		pImportTable++;
	}

	return moduleList;
}


SiUtils::SiArray<SiUtils::SiString> PEFile::enumerateNativeImports(SiUtils::SiString sModuleName)
{
	SiArray<SiString> funcList;

	const IMAGE_IMPORT_DESCRIPTOR* pImportTable = (const IMAGE_IMPORT_DESCRIPTOR*)getImportTable();
	if (!pImportTable)
		return funcList;

	// Enumerate modules to find the one we are looking for
	while (pImportTable->Name)
	{
		const char* pszName = (const char*)rvaToPointer(pImportTable->Name);
		if (!pszName)
			continue;
		if (!strcmp(pszName, (char*)sModuleName))
		{
			// Found the module we are looking for, get the list of functions
			const DWORD* pModuleImports = (const DWORD*)rvaToPointer(pImportTable->OriginalFirstThunk);
			if (!pModuleImports)
				pModuleImports = (const DWORD*)rvaToPointer(pImportTable->FirstThunk);
			if (pModuleImports)
			{
				// Enumerate the functions
				while (*pModuleImports)
				{
					const IMAGE_IMPORT_BY_NAME* pImport = (const IMAGE_IMPORT_BY_NAME*)rvaToPointer(*pModuleImports);
					if (pImport)
						funcList.Add(SiString((const char*)pImport->Name));
					pModuleImports++;
				}
			}
			return funcList;
		}
		pImportTable++;
	}

	return funcList;
}


//*************************************************************************
// Method:	 PEFile::findAssemblyRef
// Description: Finds an assembly reference token by name
// Parameters:
//	sAssemblyName - Name of assembly to find
//
// Return Value: Token value of the assembly reference, or zero on fail
//*************************************************************************
DWORD PEFile::findAssemblyRef(const SiUtils::SiString& sAssemblyName)
{
	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).getRowCount(); i++)
	{
		SiString sName = m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF)[i].getString(CLR_ASSEMBLYREF_NAME_COLINDEX);
		if (sName.CompareToIgnoreCase(sAssemblyName) == 0)
		{
			// Tokens are one-indexed, so add one to the row index when creating the token.
			return CLR_TOKEN_ASSEMBLYREF | (i + 1);
		}
	}
	return 0;
}


//*************************************************************************
// Method:	 PEFile::createTypeRef
// Description: Creates a new type reference to another .NET binary,
//				and returns the token value for this type reference.
// Parameters:
//	dwAssemblyRef - Assembly reference of the assembly that contains
//		the type
//	tnType - Type name
//
// Return Value: Token value of the new type reference, or zero on fail
//*************************************************************************
DWORD PEFile::createTypeRef(DWORD dwAssemblyRef, CLRTypeName& tnType)
{
	CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_TYPEREF));
	row.setToken(CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX, dwAssemblyRef);
	row.setString(CLR_TYPEREF_NAME_COLINDEX, tnType.sName);
	row.setString(CLR_TYPEREF_NAMESPACE_COLINDEX, tnType.sNamespace);

	// Add the row to the TypeRef table
	DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEREF).add(row);

	updateHashTables();

	SiString fullName;
	if (tnType.sOuterClass.GetLength() > 0)
	{
		fullName = tnType.sOuterClass;
		fullName += "+";
		fullName += tnType.sName;
	}
	else
	{
		fullName = tnType.sNamespace;
		fullName += ".";
		fullName += tnType.sName;
	}

	m_htTypeRefHash.Add(fullName, dwRowIndex);

	// Tokens are one-indexed, so add one to the row index when creating the token.
	return CLR_TOKEN_TYPEREF | (dwRowIndex + 1);
}


void PEFile::updateHashTables()
{
	if (!m_bNeedHashTableUpdate)
		return;

	m_htTypeDefHash.Clear();
	m_htTypeRefHash.Clear();

	// Cache the TypeDef tables
	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount(); i++)
	{
		SiString outerClass = getOuterClassName(CLR_TOKEN_TYPEDEF | (i + 1));
		if (outerClass.GetLength() > 0)
		{
			SiString fullName = outerClass;
			fullName += "+";
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].getString(CLR_TYPEDEF_NAME_COLINDEX);
			m_htTypeDefHash[fullName] = i;
		}
		else
		{
			SiString fullName = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].getString(CLR_TYPEDEF_NAMESPACE_COLINDEX);
			fullName += ".";
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].getString(CLR_TYPEDEF_NAME_COLINDEX);
			m_htTypeDefHash[fullName] = i;
		}
	}

	// Cache the TypeRef tables
	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_TYPEREF).getRowCount(); i++)
	{
		SiString outerClass = getOuterClassName(CLR_TOKEN_TYPEREF | (i + 1));
		if (outerClass.GetLength() > 0)
		{
			SiString fullName = outerClass;
			fullName += "+";
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAME_COLINDEX);
			m_htTypeRefHash[fullName] = i;
		}
		else
		{
			SiString fullName = m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAMESPACE_COLINDEX);
			fullName += ".";
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAME_COLINDEX);
			m_htTypeRefHash[fullName] = i;
		}
	}

	m_bNeedHashTableUpdate = false;
}


//*************************************************************************
// Method:	 PEFile::addNestedType
// Description: Creates a type nesting declaration in the assembly's
//				metadata.
// Parameters:
//	dwEnclosingToken - TypeDef token of outer class
//	dwEnclosedToken - TypeDef token of inner class
//
// Return Value: true on success
//*************************************************************************
bool PEFile::addNestedType(DWORD dwEnclosingToken, DWORD dwEnclosedToken)
{
	CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS));
	row[CLR_NESTEDCLASS_NESTEDCLASS_COLINDEX] = CLR_TOKEN_RID(dwEnclosedToken);
	row[CLR_NESTEDCLASS_ENCLOSINGCLASS_COLINDEX] = CLR_TOKEN_RID(dwEnclosingToken);
	m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS).add(row);

	if ((m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwEnclosedToken) - 1][CLR_TYPEDEF_FLAGS_COLINDEX] &
		CLR_TYPEDEF_VIS_MASK) == CLR_TYPEDEF_VIS_PRIVATE)
	{
		m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwEnclosedToken) - 1][CLR_TYPEDEF_FLAGS_COLINDEX] =
			(m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwEnclosedToken) - 1][CLR_TYPEDEF_FLAGS_COLINDEX] &
			(~CLR_TYPEDEF_VIS_MASK)) | CLR_TYPEDEF_VIS_NESTED_PRIVATE;
	}
	else if ((m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwEnclosedToken) - 1][CLR_TYPEDEF_FLAGS_COLINDEX] &
		CLR_TYPEDEF_VIS_MASK) == CLR_TYPEDEF_VIS_PUBLIC)
	{
		m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwEnclosedToken) - 1][CLR_TYPEDEF_FLAGS_COLINDEX] =
			(m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwEnclosedToken) - 1][CLR_TYPEDEF_FLAGS_COLINDEX] &
			(~CLR_TYPEDEF_VIS_MASK)) | CLR_TYPEDEF_VIS_NESTED_PUBLIC;
	}

	return true;
}


//*************************************************************************
// Method:	 PEFile::getOuterClassName
// Description: Gets the name of any enclosing class
// Parameters:
//	dwEnclosedToken - TypeDef or TypeRef token of inner class
//
// Return Value: Full name of enclosing class, or empty string if not enclosed
//*************************************************************************
SiString PEFile::getOuterClassName(DWORD dwEnclosedToken)
{
	if (CLR_TOKEN_TYPE(dwEnclosedToken) == CLR_TABLE_TYPEDEF)
	{
		// Search NestedClass table for enclosing class
		for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS).getRowCount(); i++)
		{
			if (m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS)[i].getToken(CLR_NESTEDCLASS_NESTEDCLASS_COLINDEX) ==
				dwEnclosedToken)
			{
				// Found the nested class entry, get the full name of the enclosing class
				SiString enclosingClassOuter = getOuterClassName(m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS)[i].
					getToken(CLR_NESTEDCLASS_ENCLOSINGCLASS_COLINDEX));
				if (enclosingClassOuter.GetLength() > 0)
				{
					// Enclosing class is also an inner class
					SiString fullName = enclosingClassOuter;
					fullName += "+";
					fullName += m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS)[i].getRefRow(
						CLR_NESTEDCLASS_ENCLOSINGCLASS_COLINDEX)->getString(CLR_TYPEDEF_NAME_COLINDEX);
					return fullName;
				}
				else
				{
					// Enclosing class is a top level class
					SiString fullName = m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS)[i].getRefRow(
						CLR_NESTEDCLASS_ENCLOSINGCLASS_COLINDEX)->getString(CLR_TYPEDEF_NAMESPACE_COLINDEX);
					fullName += ".";
					fullName += m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS)[i].getRefRow(
						CLR_NESTEDCLASS_ENCLOSINGCLASS_COLINDEX)->getString(CLR_TYPEDEF_NAME_COLINDEX);
					return fullName;
				}
			}
		}
	}
	else if (CLR_TOKEN_TYPE(dwEnclosedToken) == CLR_TABLE_TYPEREF)
	{
		// Class is nested if resolution scope is a TypeRef
		if (m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwEnclosedToken) - 1].getRefTableIndex(
			CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX) == CLR_TABLE_TYPEREF)
		{
			SiString enclosingClassOuter = getOuterClassName(m_mdMetadata.getTable(CLR_TABLE_TYPEREF)
				[CLR_TOKEN_RID(dwEnclosedToken) - 1].getToken(CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX));
			if (enclosingClassOuter.GetLength() > 0)
			{
				// Enclosing class is also an inner class
				SiString fullName = enclosingClassOuter;
				fullName += "+";
				fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwEnclosedToken) - 1].
					getRefRow(CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX)->getString(CLR_TYPEREF_NAME_COLINDEX);
				return fullName;
			}
			else
			{
				// Enclosing class is a top level class
				SiString fullName = m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwEnclosedToken) - 1].
					getRefRow(CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX)->getString(CLR_TYPEREF_NAMESPACE_COLINDEX);
				fullName += ".";
				fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwEnclosedToken) - 1].
					getRefRow(CLR_TYPEREF_RESOLUTIONSCOPE_COLINDEX)->getString(CLR_TYPEREF_NAME_COLINDEX);
				return fullName;
			}
		}
	}
	return "";
}


//*************************************************************************
// Method:	 PEFile::merge
// Description: Merges another binary into this binary.  The binary being
//				merged must be pure IL (no unmanaged code).
// Parameters:
//	pFile - Binary to merge
//
// Return Value: true on success
//*************************************************************************
bool PEFile::merge(PEFile& pFile)
{
	updateHashTables();

	CLRTranslationTable srcTranslation, destTranslation;
	srcTranslation.SetChainedTable(&destTranslation);

	// Merge user string heaps
	for (DWORD i=0; i<pFile.m_mdMetadata.getUserStringCount(); i++)
	{
		const CLRBlobHeapEntry* entry = pFile.m_mdMetadata.getUserStringFromIndex(i);
		DWORD dwNewOffset = m_mdMetadata.addUserString(entry->pbData, entry->dwSize);
		srcTranslation.Add(CLR_TOKEN_USERSTRING | entry->dwOffset, CLR_TOKEN_USERSTRING | dwNewOffset);
	}

	// Merge source AssemblyRefs into dest.  Ignore version numbers (this will allow us to
	// intercept both 1.0 and 1.1 apps).
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).getRowCount(); i++)
	{
		// Try to find this AssemblyRef in the dest binary
		BOOL bFound = FALSE;
		for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).getRowCount(); j++)
		{
			if (pFile.m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF)[i].getString(CLR_ASSEMBLYREF_NAME_COLINDEX).CompareTo(
				m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF)[j].getString(CLR_ASSEMBLYREF_NAME_COLINDEX)) == 0)
			{
				bFound = TRUE;
				srcTranslation.Add(CLR_TOKEN_ASSEMBLYREF | (i + 1), CLR_TOKEN_ASSEMBLYREF | (j + 1)); // RIDs are one-indexed
				break;
			}
		}

		if (pFile.m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF)[i].getString(CLR_ASSEMBLYREF_NAME_COLINDEX).CompareTo(
			m_mdMetadata.getTable(CLR_TABLE_ASSEMBLY)[0].getString(CLR_ASSEMBLY_NAME_COLINDEX)) == 0)
		{
			// Refers to current assembly
			bFound = TRUE;
			srcTranslation.Add(CLR_TOKEN_ASSEMBLYREF | (i + 1), CLR_TOKEN_ASSEMBLY | 1); // RIDs are one-indexed
		}

		if (!bFound)
		{
			// External assembly that hasn't been referenced yet
			CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF)[i].copy(m_mdMetadata, CLR_TABLE_ASSEMBLYREF);
			DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).add(row);
			srcTranslation.Add(CLR_TOKEN_ASSEMBLYREF | (i + 1), CLR_TOKEN_ASSEMBLYREF | (dwRowIndex + 1)); // RIDs are one-indexed
		}
	}

	// Merge ModuleRef tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_MODULEREF).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_MODULEREF)[i].copy(m_mdMetadata, CLR_TABLE_MODULEREF);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_MODULEREF).add(row);
		srcTranslation.Add(CLR_TOKEN_MODULEREF | (i + 1), CLR_TOKEN_MODULEREF | (dwRowIndex + 1));
	}

	// Merge source TypeRefs with dest TypeRef table
	SiArray<DWORD> typeRefWorkList;
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEREF).getRowCount(); i++)
	{
		CLRTypeName name;
		name.sNamespace = pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAMESPACE_COLINDEX);
		name.sName = pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAME_COLINDEX);
		name.sOuterClass = pFile.getOuterClassName(CLR_TOKEN_TYPEREF | (i + 1));
		DWORD dwExistingType = findTypeToken(name);

		if (dwExistingType)
		{
			// Type already exists in the dest binary
			srcTranslation.Add(CLR_TOKEN_TYPEREF | (i + 1), dwExistingType);
		}
		else
		{
			// Copy TypeRef to dest binary
			CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].copy(m_mdMetadata, CLR_TABLE_TYPEREF);
			DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEREF).add(row);
			srcTranslation.Add(CLR_TOKEN_TYPEREF | (i + 1), CLR_TOKEN_TYPEREF | (dwRowIndex + 1));
			typeRefWorkList.Add(dwRowIndex);
		}
	}
	for (DWORD i=0; i<typeRefWorkList.GetLength(); i++)
		m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[typeRefWorkList[i]].replaceTokens(srcTranslation);

	// Merge source TypeDefs with dest TypeDef table, but not filling in dest of FieldList and MethodList
	// (just use the count and make the RIDs valid)
	SiArray<DWORD> typeDefWorkList;
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount(); i++)
	{
		if (pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].getString(CLR_TYPEDEF_NAME_COLINDEX).CompareTo(
			"<Module>") == 0)
		{
			// Module TypeDef, don't copy it over.  Instead, create a translation
			for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount(); j++)
			{
				if (m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[j].getString(CLR_TYPEDEF_NAME_COLINDEX).CompareTo(
					"<Module>") == 0)
				{
					srcTranslation.Add(CLR_TOKEN_TYPEDEF | (i + 1), CLR_TOKEN_TYPEDEF | (j + 1));
					break;
				}
			}
			continue;
		}

		// Copy TypeDef to dest binary
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[i].copy(m_mdMetadata, CLR_TABLE_TYPEDEF);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).add(row);
		srcTranslation.Add(CLR_TOKEN_TYPEDEF | (i + 1), CLR_TOKEN_TYPEDEF | (dwRowIndex + 1));
		typeDefWorkList.Add(dwRowIndex);
	}
	DWORD dwFieldOffset = m_mdMetadata.getTable(CLR_TABLE_FIELD).getRowCount();
	DWORD dwMethodOffset = m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount();
	DWORD dwParamOffset = m_mdMetadata.getTable(CLR_TABLE_PARAM).getRowCount();
	for (DWORD i=0; i<typeDefWorkList.GetLength(); i++)
	{
		m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[typeDefWorkList[i]].replaceTokens(srcTranslation);
		m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[typeDefWorkList[i]][CLR_TYPEDEF_FIELDLIST_COLINDEX] += dwFieldOffset;
		m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[typeDefWorkList[i]][CLR_TYPEDEF_METHODLIST_COLINDEX] += dwMethodOffset;
	}

	// Create translation table entries for Field, Method, and Param rows
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_FIELD).getRowCount(); i++)
		srcTranslation.Add(CLR_TOKEN_FIELD | (i + 1), CLR_TOKEN_FIELD | (dwFieldOffset + i + 1));
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount(); i++)
		srcTranslation.Add(CLR_TOKEN_METHOD | (i + 1), CLR_TOKEN_METHOD | (dwMethodOffset + i + 1));
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_PARAM).getRowCount(); i++)
		srcTranslation.Add(CLR_TOKEN_PARAM | (i + 1), CLR_TOKEN_PARAM | (dwParamOffset + i + 1));

	// Fill in Field table with fields referenced by FieldList in the added TypeDefs
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_FIELD).getRowCount(); i++)
	{
		// Copy Field to dest binary
		CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_FIELD));
		row[CLR_FIELD_FLAGS_COLINDEX] = pFile.m_mdMetadata.getTable(CLR_TABLE_FIELD)[i][CLR_FIELD_FLAGS_COLINDEX];
		row.setString(CLR_FIELD_NAME_COLINDEX, pFile.m_mdMetadata.getTable(CLR_TABLE_FIELD)[i].getString(CLR_FIELD_NAME_COLINDEX));

		CLRSignature sig;
		sig.replaceTokens(pFile.m_mdMetadata.getTable(CLR_TABLE_FIELD)[i].getBlobData(CLR_FIELD_SIGNATURE_COLINDEX),
			srcTranslation);
		row.setBlob(CLR_FIELD_SIGNATURE_COLINDEX, sig.getData(), sig.getSize());

		m_mdMetadata.getTable(CLR_TABLE_FIELD).add(row);
	}

	// Merge source MemberRefs into the dest binary.
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_MEMBERREF).getRowCount(); i++)
	{
		CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_MEMBERREF));
		row.setToken(CLR_MEMBERREF_CLASS_COLINDEX, srcTranslation.Translate(pFile.m_mdMetadata.getTable(
			CLR_TABLE_MEMBERREF)[i].getToken(CLR_MEMBERREF_CLASS_COLINDEX)));
		row.setString(CLR_MEMBERREF_NAME_COLINDEX, pFile.m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[i].
			getString(CLR_MEMBERREF_NAME_COLINDEX));

		CLRSignature sig;
		sig.replaceTokens(pFile.m_mdMetadata.getTable(CLR_TABLE_MEMBERREF)[i].getBlobData(
			CLR_MEMBERREF_SIGNATURE_COLINDEX), srcTranslation);
		row.setBlob(CLR_MEMBERREF_SIGNATURE_COLINDEX, sig.getData(), sig.getSize());

		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_MEMBERREF).add(row);
		srcTranslation.Add(CLR_TOKEN_MEMBERREF | (i + 1), CLR_TOKEN_MEMBERREF | (dwRowIndex + 1));
	}

	// Merge InterfaceImpl tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_INTERFACEIMPL).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_INTERFACEIMPL)[i].copy(m_mdMetadata, CLR_TABLE_INTERFACEIMPL);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_INTERFACEIMPL).add(row);
		srcTranslation.Add(CLR_TOKEN_INTERFACEIMPL | (i + 1), CLR_TOKEN_INTERFACEIMPL | (dwRowIndex + 1));
	}

	// Merge FieldMarshal tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_FIELDMARSHAL).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_FIELDMARSHAL)[i].copy(m_mdMetadata, CLR_TABLE_FIELDMARSHAL);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_FIELDMARSHAL).add(row);
		srcTranslation.Add(CLR_TOKEN_FIELDMARSHAL | (i + 1), CLR_TOKEN_FIELDMARSHAL | (dwRowIndex + 1));
	}

	// Merge ClassLayout tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_CLASSLAYOUT).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_CLASSLAYOUT)[i].copy(m_mdMetadata, CLR_TABLE_CLASSLAYOUT);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_CLASSLAYOUT).add(row);
		srcTranslation.Add(CLR_TOKEN_CLASSLAYOUT | (i + 1), CLR_TOKEN_CLASSLAYOUT | (dwRowIndex + 1));
	}

	// Merge FieldLayout tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_FIELDLAYOUT).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_FIELDLAYOUT)[i].copy(m_mdMetadata, CLR_TABLE_FIELDLAYOUT);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_FIELDLAYOUT).add(row);
		srcTranslation.Add(CLR_TOKEN_FIELDLAYOUT | (i + 1), CLR_TOKEN_FIELDLAYOUT | (dwRowIndex + 1));
	}

	// Merge Event tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_EVENT).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_EVENT)[i].copy(m_mdMetadata, CLR_TABLE_EVENT);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_EVENT).add(row);
		srcTranslation.Add(CLR_TOKEN_EVENT | (i + 1), CLR_TOKEN_EVENT | (dwRowIndex + 1));
	}

	// Merge EventMap tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_EVENTMAP).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_EVENTMAP)[i].copy(m_mdMetadata, CLR_TABLE_EVENTMAP);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_EVENTMAP).add(row);
		srcTranslation.Add(CLR_TOKEN_EVENTMAP | (i + 1), CLR_TOKEN_EVENTMAP | (dwRowIndex + 1));
	}

	// Merge Property tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_PROPERTY).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_PROPERTY)[i].copy(m_mdMetadata, CLR_TABLE_PROPERTY);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_PROPERTY).add(row);
		srcTranslation.Add(CLR_TOKEN_PROPERTY | (i + 1), CLR_TOKEN_PROPERTY | (dwRowIndex + 1));
	}

	// Merge PropertyMap tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_PROPERTYMAP).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_PROPERTYMAP)[i].copy(m_mdMetadata, CLR_TABLE_PROPERTYMAP);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_PROPERTYMAP).add(row);
		srcTranslation.Add(CLR_TOKEN_PROPERTYMAP | (i + 1), CLR_TOKEN_PROPERTYMAP | (dwRowIndex + 1));
	}

	// Merge Constant tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_CONSTANT).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_CONSTANT)[i].copy(m_mdMetadata, CLR_TABLE_CONSTANT);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_CONSTANT).add(row);
		srcTranslation.Add(CLR_TOKEN_CONSTANT | (i + 1), CLR_TOKEN_CONSTANT | (dwRowIndex + 1));
	}

	// Merge MethodSemantics tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_METHODSEMANTICS).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_METHODSEMANTICS)[i].copy(m_mdMetadata, CLR_TABLE_METHODSEMANTICS);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_METHODSEMANTICS).add(row);
		srcTranslation.Add(CLR_TOKEN_METHODSEMANTICS | (i + 1), CLR_TOKEN_METHODSEMANTICS | (dwRowIndex + 1));
	}

	// Merge MethodImpl tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_METHODIMPL).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_METHODIMPL)[i].copy(m_mdMetadata, CLR_TABLE_METHODIMPL);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_METHODIMPL).add(row);
		srcTranslation.Add(CLR_TOKEN_METHODIMPL | (i + 1), CLR_TOKEN_METHODIMPL | (dwRowIndex + 1));
	}

	// Merge ImplMap tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_IMPLMAP).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_IMPLMAP)[i].copy(m_mdMetadata, CLR_TABLE_IMPLMAP);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_IMPLMAP).add(row);
		srcTranslation.Add(CLR_TOKEN_IMPLMAP | (i + 1), CLR_TOKEN_IMPLMAP | (dwRowIndex + 1));
	}

	// Merge FieldRVA tables
	if (pFile.m_mdMetadata.getTable(CLR_TABLE_FIELDRVA).getRowCount() > 0)
		return false; // FieldRVA merge not supported

	// Merge NestedClass tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS)[i].copy(m_mdMetadata, CLR_TABLE_NESTEDCLASS);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_NESTEDCLASS).add(row);
		srcTranslation.Add(CLR_TOKEN_NESTEDCLASS | (i + 1), CLR_TOKEN_NESTEDCLASS | (dwRowIndex + 1));
	}

	// Merge DeclSecurity tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_DECLSECURITY).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_DECLSECURITY)[i].copy(m_mdMetadata, CLR_TABLE_DECLSECURITY);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_DECLSECURITY).add(row);
		srcTranslation.Add(CLR_TOKEN_DECLSECURITY | (i + 1), CLR_TOKEN_DECLSECURITY | (dwRowIndex + 1));
	}

	// Merge CustomAttribute tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE).getRowCount(); i++)
	{
		// Don't copy over attributes on the other assembly
		if (pFile.m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)[i].getRefTableIndex(
			CLR_CUSTOMATTRIBUTE_PARENT_COLINDEX) == CLR_TABLE_ASSEMBLY)
			continue;

		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE)[i].copy(m_mdMetadata, CLR_TABLE_CUSTOMATTRIBUTE);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE).add(row);
		srcTranslation.Add(CLR_TOKEN_CUSTOMATTRIBUTE | (i + 1), CLR_TOKEN_CUSTOMATTRIBUTE | (dwRowIndex + 1));
	}

	// Merge ENCLog tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_ENCLOG).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_ENCLOG)[i].copy(m_mdMetadata, CLR_TABLE_ENCLOG);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_ENCLOG).add(row);
		srcTranslation.Add(CLR_TOKEN_ENCLOG | (i + 1), CLR_TOKEN_ENCLOG | (dwRowIndex + 1));
	}

	// Merge ENCMap tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_ENCMAP).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_ENCMAP)[i].copy(m_mdMetadata, CLR_TABLE_ENCMAP);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_ENCMAP).add(row);
		srcTranslation.Add(CLR_TOKEN_ENCMAP | (i + 1), CLR_TOKEN_ENCMAP | (dwRowIndex + 1));
	}

	// Merge StandAloneSig tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_STANDALONESIG).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_STANDALONESIG)[i].copy(m_mdMetadata, CLR_TABLE_STANDALONESIG);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_STANDALONESIG).add(row);
		srcTranslation.Add(CLR_TOKEN_STANDALONESIG | (i + 1), CLR_TOKEN_STANDALONESIG | (dwRowIndex + 1));
	}

	// Merge TypeSpec tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_TYPESPEC).getRowCount(); i++)
	{
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_TYPESPEC)[i].copy(m_mdMetadata, CLR_TABLE_TYPESPEC);
		row.replaceTokens(srcTranslation);
		DWORD dwRowIndex = m_mdMetadata.getTable(CLR_TABLE_TYPESPEC).add(row);
		srcTranslation.Add(CLR_TOKEN_TYPESPEC | (i + 1), CLR_TOKEN_TYPESPEC | (dwRowIndex + 1));
	}

	// Remove AssemblyRef for the source from the dest AssemblyRef table.
	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).getRowCount(); i++)
	{
		if (m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF)[i].getString(CLR_ASSEMBLYREF_NAME_COLINDEX).CompareTo(
			pFile.m_mdMetadata.getTable(CLR_TABLE_ASSEMBLY)[0].getString(CLR_ASSEMBLY_NAME_COLINDEX)) == 0)
		{
			// AssemblyRef refers to the assembly being merged.  Remove the AssemblyRef.
			DWORD dwRefToken = destTranslation.ReverseLookup(CLR_TOKEN_ASSEMBLYREF | (i + 1));

			destTranslation.Add(dwRefToken, 0);
			for (DWORD j=i; j<(m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).getRowCount() - 1); j++)
			{
				destTranslation.Add(destTranslation.ReverseLookup(CLR_TOKEN_ASSEMBLYREF | (j + 2)),
					CLR_TOKEN_ASSEMBLYREF | (j + 1));
			}

			// Delete rows from AssemblyRefProcessor table that refer to the deleted AssemblyRef
			for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREFPROCESSOR).getRowCount(); j++)
			{
				if (m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREFPROCESSOR)[i].getToken(
					CLR_ASSEMBLYREFPROCESSOR_ASSEMBLYREF_COLINDEX) == dwRefToken)
				{
					m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREFPROCESSOR).remove(i);
					i--;
				}
			}

			// Delete rows from AssemblyRefProcessor table that refer to the deleted AssemblyRef
			for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREFOS).getRowCount(); j++)
			{
				if (m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREFOS)[i].getToken(
					CLR_ASSEMBLYREFOS_ASSEMBLYREF_COLINDEX) == dwRefToken)
				{
					m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREFOS).remove(i);
					i--;
				}
			}

			m_mdMetadata.getTable(CLR_TABLE_ASSEMBLYREF).remove(i);
			i--;
		}
	}

	// Go through dest TypeRef list and find any that have associated TypeDefs, removing the TypeRef and
	// redirecting it to the TypeDef if found.
	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_TYPEREF).getRowCount(); i++)
	{
		for (DWORD j=0; j<m_mdMetadata.getTable(CLR_TABLE_TYPEDEF).getRowCount(); j++)
		{
			if ((m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAME_COLINDEX).CompareTo(
				m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[j].getString(CLR_TYPEDEF_NAME_COLINDEX)) == 0) &&
				(m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[i].getString(CLR_TYPEREF_NAMESPACE_COLINDEX).CompareTo(
				m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[j].getString(CLR_TYPEDEF_NAMESPACE_COLINDEX)) == 0) &&
				(getOuterClassName(CLR_TOKEN_TYPEREF | (i + 1)).CompareTo(
				getOuterClassName(CLR_TOKEN_TYPEDEF | (j + 1))) == 0))
			{
				// This TypeRef now has a TypeDef referring to the same class.  Remove the TypeRef and point
				// the token to the TypeDef.
				DWORD dwRefToken = destTranslation.ReverseLookup(CLR_TOKEN_TYPEREF | (i + 1));
				DWORD dwDefToken = CLR_TOKEN_TYPEDEF | (j + 1);
				destTranslation.Add(dwRefToken, dwDefToken);
				for (DWORD k=i; k<(m_mdMetadata.getTable(CLR_TABLE_TYPEREF).getRowCount() - 1); k++)
				{
					destTranslation.Add(destTranslation.ReverseLookup(CLR_TOKEN_TYPEREF | (k + 2)),
						CLR_TOKEN_TYPEREF | (k + 1));
				}
				m_mdMetadata.getTable(CLR_TABLE_TYPEREF).remove(i);
				i--;
				break;
			}
		}
	}

	// Run the existing methods through the translation table
	for (DWORD i=0; i<m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount(); i++)
	{
		if (m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_RVA_COLINDEX] == 0)
			continue;
		if (m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_FLAGS_COLINDEX] & CLR_METHOD_IMPL_PINVOKE)
			continue;

		CLRMethod method;
		if (!method.parse(this, m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_RVA_COLINDEX]))
			return false;
		if (!method.replaceTokensInIL(destTranslation))
			return false;
		method.setMethodIndex(i);
		replaceMethod(&method);
	}

	// Run existing tables through the translation table
	for (DWORD i=0; i<CLR_METADATA_TABLE_COUNT; i++)
	{
		for (DWORD j=0; j<m_mdMetadata.getTable(i).getRowCount(); j++)
			m_mdMetadata.getTable(i)[j].replaceTokens(destTranslation);
	}

	// Fill In Method table with methods referenced by MethodList in the added TypeDefs.
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD).getRowCount(); i++)
	{
		// Copy Field to dest binary
		CLRTableRow row(&m_mdMetadata.getTable(CLR_TABLE_METHOD));
		row[CLR_METHOD_IMPLFLAGS_COLINDEX] = pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_IMPLFLAGS_COLINDEX];
		row[CLR_METHOD_FLAGS_COLINDEX] = pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_FLAGS_COLINDEX];
		row.setString(CLR_METHOD_NAME_COLINDEX, pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i].getString(CLR_METHOD_NAME_COLINDEX));

		CLRSignature sig;
		sig.replaceTokens(pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i].getBlobData(CLR_METHOD_SIGNATURE_COLINDEX),
			srcTranslation);
		row.setBlob(CLR_METHOD_SIGNATURE_COLINDEX, sig.getData(), sig.getSize());

		row[CLR_METHOD_PARAMLIST_COLINDEX] = pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_PARAMLIST_COLINDEX] +
			dwParamOffset;

		if (pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_RVA_COLINDEX] != 0)
		{
			CLRMethod method;
			if (!method.parse(&pFile, pFile.m_mdMetadata.getTable(CLR_TABLE_METHOD)[i][CLR_METHOD_RVA_COLINDEX]))
				return false;
			if (!method.replaceTokensInIL(srcTranslation))
				return false;

			if (!m_pReplacedILSection)
			{
				// Create a new section to contain the modified IL code
				m_pReplacedILSection = new PESection();
				m_pReplacedILSection->create(".heatil", IMAGE_SCN_MEM_READ | IMAGE_SCN_CNT_INITIALIZED_DATA);
				m_vpSections.Add(m_pReplacedILSection);
				m_peNTHeader.FileHeader.NumberOfSections++;

				// Lay out the sections so that the replaced IL code section has a valid virtual address
				layoutSections();
			}

			// Calculate RVA to the new IL code
			DWORD dwMethodRVA = m_pReplacedILSection->getRawDataSize() + m_pReplacedILSection->getStartVirtualAddress();

			// Write out the IL code
			if (!method.save(this, m_pReplacedILSection))
				return false;

			// Align code on a 4 byte boundary
			while (m_pReplacedILSection->getRawDataSize() & 3)
			{
				BYTE bZero = 0;
				m_pReplacedILSection->appendData(&bZero, 1);
			}

			// Update the metadata to point to the new IL code
			row[CLR_METHOD_RVA_COLINDEX] = dwMethodRVA;
		}
		else
			row[CLR_METHOD_RVA_COLINDEX] = 0;

		m_mdMetadata.getTable(CLR_TABLE_METHOD).add(row);
	}

	// Merge Param tables
	for (DWORD i=0; i<pFile.m_mdMetadata.getTable(CLR_TABLE_PARAM).getRowCount(); i++)
	{
		// Copy Param to dest binary
		CLRTableRow row = pFile.m_mdMetadata.getTable(CLR_TABLE_PARAM)[i].copy(m_mdMetadata, CLR_TABLE_PARAM);
		m_mdMetadata.getTable(CLR_TABLE_PARAM).add(row);
	}

	// Sort tables that need sorted columns
	m_mdMetadata.getTable(CLR_TABLE_CONSTANT).sort();
	m_mdMetadata.getTable(CLR_TABLE_CUSTOMATTRIBUTE).sort();
	m_mdMetadata.getTable(CLR_TABLE_IMPLMAP).sort();

	m_bNeedHashTableUpdate = true;
	updateHashTables();

	return true;
}


//*************************************************************************
// Method:	 PEFile::getFullTypeName
// Description: Returns the full name of a type token
// Parameters:
//	dwTypeToken - Token of type
//
// Return Value: Full class name including namespace and enclosing types
//*************************************************************************
SiString PEFile::getFullTypeName(DWORD dwTypeToken)
{
	SiString outerClass = getOuterClassName(dwTypeToken);
	if (outerClass.GetLength() > 0)
	{
		SiString fullName = outerClass;
		fullName += "+";

		if (CLR_TOKEN_TYPE(dwTypeToken) == CLR_TABLE_TYPEDEF)
		{
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwTypeToken) - 1].getString(
				CLR_TYPEDEF_NAME_COLINDEX);
		}
		else
		{
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwTypeToken) - 1].getString(
				CLR_TYPEREF_NAME_COLINDEX);
		}
		return fullName;
	}
	else
	{
		if (CLR_TOKEN_TYPE(dwTypeToken) == CLR_TABLE_TYPEDEF)
		{
			SiString fullName = m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwTypeToken) - 1].getString(
				CLR_TYPEDEF_NAMESPACE_COLINDEX);
			fullName += ".";
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEDEF)[CLR_TOKEN_RID(dwTypeToken) - 1].getString(
				CLR_TYPEDEF_NAME_COLINDEX);
			return fullName;
		}
		else
		{
			SiString fullName = m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwTypeToken) - 1].getString(
				CLR_TYPEREF_NAMESPACE_COLINDEX);
			fullName += ".";
			fullName += m_mdMetadata.getTable(CLR_TABLE_TYPEREF)[CLR_TOKEN_RID(dwTypeToken) - 1].getString(
				CLR_TYPEREF_NAME_COLINDEX);
			return fullName;
		}
	}
}
