//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTypeSignature.cpp
//
// DESCRIPTION: Contains classes to manage the basic type signatures found
//				within method and field signatures in .NET classes.
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Apr 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include "CLRSignature.h"
#include "CLRTypeSignature.h"
#include "CLRMetadata.h"
#include "CLRMetadataDefs.h"
#include "PEFile.h"


//*************************************************************************
// Method:	 CLRTypeSignature::CLRTypeSignature
// Description: Default constructor for the CLRTypeSignature type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRTypeSignature::CLRTypeSignature()
{
	m_pbAssocData = NULL;
	m_dwAssocDataSize = 0;
	m_ptsAssocType = NULL;
	m_psAssocMethodSig = NULL;
	m_bPrivateType = false;
	m_dwTypeToken = 0;
}


//*************************************************************************
// Method:	 CLRTypeSignature::CLRTypeSignature
// Description: Constructor for the CLRTypeSignature type that initializes
//		the type signature to a basic type
// Parameters:
//	bBasicTypeCode - Type code, one of the CLR_DATATYPE_* defines
//
// Return Value: None
//*************************************************************************
CLRTypeSignature::CLRTypeSignature(BYTE bBasicTypeCode)
{
	m_bTypeCode = bBasicTypeCode;
	m_pbAssocData = NULL;
	m_dwAssocDataSize = 0;
	m_ptsAssocType = NULL;
	m_psAssocMethodSig = NULL;
	m_bPrivateType = false;
	m_dwTypeToken = 0;
}


//*************************************************************************
// Method:	 CLRTypeSignature::CLRTypeSignature
// Description: Constructor for the CLRTypeSignature type that initializes
//		the type signature to a type that has an associated type name (for
//		example, a reference type)
// Parameters:
//	bTypeCode - Type code, one of the CLR_DATATYPE_* defines
//  tnName - Associated type name
//
// Return Value: None
//*************************************************************************
CLRTypeSignature::CLRTypeSignature(BYTE bTypeCode, const CLRTypeName& tnName)
{
	m_bTypeCode = bTypeCode;
	m_tnTypeName = tnName;
	m_pbAssocData = NULL;
	m_dwAssocDataSize = 0;
	m_ptsAssocType = NULL;
	m_psAssocMethodSig = NULL;
	m_bPrivateType = false;
	m_dwTypeToken = 0;
}


//*************************************************************************
// Method:	 CLRTypeSignature::CLRTypeSignature
// Description: Constructor for the CLRTypeSignature type that initializes
//		the type signature to a type that has an associated type signature
//		(for example, arrays)
// Parameters:
//	bTypeCode - Type code, one of the CLR_DATATYPE_* defines
//  tsType - Associated type signature
//
// Return Value: None
//*************************************************************************
CLRTypeSignature::CLRTypeSignature(BYTE bTypeCode, const CLRTypeSignature& tsType)
{
	m_bTypeCode = bTypeCode;
	m_pbAssocData = NULL;
	m_dwAssocDataSize = 0;
	m_ptsAssocType = new CLRTypeSignature(tsType);
	m_psAssocMethodSig = NULL;
	m_bPrivateType = false;
	m_dwTypeToken = 0;
}


//*************************************************************************
// Method:	 CLRTypeSignature::CLRTypeSignature
// Description: Copy constructor for the CLRTypeSignature type.
// Parameters:
//	tsType - Type signature to copy
//
// Return Value: None
//*************************************************************************
CLRTypeSignature::CLRTypeSignature(const CLRTypeSignature& tsType)
{
	m_bTypeCode = tsType.m_bTypeCode;
	m_dwAssocDataSize = tsType.m_dwAssocDataSize;
	m_tnTypeName = tsType.m_tnTypeName;

	if (tsType.m_pbAssocData)
	{
		m_pbAssocData = new BYTE[m_dwAssocDataSize];
		memcpy(m_pbAssocData, tsType.m_pbAssocData, m_dwAssocDataSize);
	}
	else
		m_pbAssocData = NULL;

	if (tsType.m_ptsAssocType)
		m_ptsAssocType = new CLRTypeSignature(*tsType.m_ptsAssocType);
	else
		m_ptsAssocType = NULL;

	if (tsType.m_psAssocMethodSig)
		m_psAssocMethodSig = new CLRSignature(*tsType.m_psAssocMethodSig);
	else
		m_psAssocMethodSig = NULL;

	m_bPrivateType = tsType.m_bPrivateType;
	m_dwTypeToken = tsType.m_dwTypeToken;
}


//*************************************************************************
// Method:	 CLRTypeSignature::~CLRTypeSignature
// Description: Destructor for the CLRTypeSignature type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRTypeSignature::~CLRTypeSignature()
{
	if (m_pbAssocData)
	{
		delete[] m_pbAssocData;
		m_pbAssocData = NULL;
	}

	if (m_ptsAssocType)
	{
		delete m_ptsAssocType;
		m_ptsAssocType = NULL;
	}

	if (m_psAssocMethodSig)
	{
		delete m_psAssocMethodSig;
		m_psAssocMethodSig = NULL;
	}
}


//*************************************************************************
// Method:	 CLRTypeSignature::opreator=
// Description: Assignement operator for the CLRTypeSignature type.
// Parameters:
//	tsType - New type signature
//
// Return Value: Reference to the assigned value
//*************************************************************************
CLRTypeSignature& CLRTypeSignature::operator=(const CLRTypeSignature& tsType)
{
	if (m_pbAssocData)
	{
		delete[] m_pbAssocData;
		m_pbAssocData = NULL;
	}

	if (m_ptsAssocType)
	{
		delete m_ptsAssocType;
		m_ptsAssocType = NULL;
	}

	if (m_psAssocMethodSig)
	{
		delete m_psAssocMethodSig;
		m_psAssocMethodSig = NULL;
	}

	m_bTypeCode = tsType.m_bTypeCode;
	m_dwAssocDataSize = tsType.m_dwAssocDataSize;
	m_tnTypeName = tsType.m_tnTypeName;

	if (tsType.m_pbAssocData)
	{
		m_pbAssocData = new BYTE[m_dwAssocDataSize];
		memcpy(m_pbAssocData, tsType.m_pbAssocData, m_dwAssocDataSize);
	}
	else
		m_pbAssocData = NULL;

	if (tsType.m_ptsAssocType)
		m_ptsAssocType = new CLRTypeSignature(*tsType.m_ptsAssocType);
	else
		m_ptsAssocType = NULL;

	if (tsType.m_psAssocMethodSig)
		m_psAssocMethodSig = new CLRSignature(*tsType.m_psAssocMethodSig);
	else
		m_psAssocMethodSig = NULL;

	m_bPrivateType = tsType.m_bPrivateType;
	m_dwTypeToken = tsType.m_dwTypeToken;

	return *this;
}


//*************************************************************************
// Method:	 CLRTypeSignature::readCompressedInt
// Description: Reads a compressed 32-bit integer out of the data stream.
//				The most significant 2 bits specify how many bytes to use
//				to encode the integer, with the data in big endian format.
// Parameters:
//	pbTypeSig - Data stream to read from
//  dwOffset - Offset in the data stream to read from.  Offset will be
//		updated according to how many bytes are read
//
// Return Value: Integer that was decoded
//*************************************************************************
DWORD CLRTypeSignature::readCompressedInt(const BYTE* pbTypeSig, DWORD& dwOffset)
{
	DWORD dwValue = pbTypeSig[dwOffset++];
	if (dwValue < 0x80)
		return dwValue;
	else if (dwValue < 0xc0)
	{
		// 16-bit value
		dwValue = (dwValue << 8) | pbTypeSig[dwOffset++];
		dwValue &= 0x3fff;
	}
	else
	{
		// 32-bit value
		dwValue = (dwValue << 8) | pbTypeSig[dwOffset++];
		dwValue = (dwValue << 8) | pbTypeSig[dwOffset++];
		dwValue = (dwValue << 8) | pbTypeSig[dwOffset++];
		dwValue &= 0x1fffffff;
	}
	return dwValue;
}


//*************************************************************************
// Method:	 CLRTypeSignature::skipCompressedInt
// Description: Skips over a compressed integer, updating the offset so
//				that it is past the compressed integer.
// Parameters:
//	pbTypeSig - Data stream containing the compressed integer
//  dwOffset - Offset of the beginning of the compressed integer.  This
//		will be updated to be just after the compressed integer.
//
// Return Value: None
//*************************************************************************
void CLRTypeSignature::skipCompressedInt(const BYTE* pbTypeSig, DWORD& dwOffset)
{
	BYTE bValue = pbTypeSig[dwOffset++];
	if (bValue < 0x80)
		return;
	else if (bValue < 0xc0)
	{
		// 16-bit value
		dwOffset++;
	}
	else
	{
		// 32-bit value
		dwOffset += 3;
	}
}


//*************************************************************************
// Method:	 CLRTypeSignature::readTypeToken
// Description: Reads a TypeDefOrRef coded token from the data stream and
//				gets the name of the referenced type
// Parameters:
//	mdMetadata - Metadata containing the type token
//  pbTypeSig - Data stream to read from
//  dwOffset - Offset within the data stream to read from.  Will be updated
//		to be just past the token read.
//
// Return Value: Type name referenced by the coded token
//*************************************************************************
CLRTypeName CLRTypeSignature::readTypeToken(CLRMetadata* mdMetadata, const BYTE* pbTypeSig,
											DWORD& dwOffset)
{
	DWORD dwCodedToken = readCompressedInt(pbTypeSig, dwOffset);
	// TypeDefOrRef coded token, bottom 2 bits contain type of token
	DWORD dwTokenType = dwCodedToken & 3;
	DWORD dwTokenRowIndex = (dwCodedToken >> 2) - 1; // RIDs are one-indexed
	if (dwTokenType == CLR_TYPEDEFORREF_TYPEDEF)
	{
		// TypeDef
		CLRTypeName tnName;
		tnName.sNamespace = mdMetadata->getTable(CLR_TABLE_TYPEDEF)[dwTokenRowIndex].
			getString(CLR_TYPEDEF_NAMESPACE_COLINDEX);
		tnName.sName = mdMetadata->getTable(CLR_TABLE_TYPEDEF)[dwTokenRowIndex].
			getString(CLR_TYPEDEF_NAME_COLINDEX);
		tnName.sOuterClass = mdMetadata->getFile().getOuterClassName(CLR_TOKEN_TYPEDEF | (dwTokenRowIndex + 1));
		if (((mdMetadata->getTable(CLR_TABLE_TYPEDEF)[dwTokenRowIndex][CLR_TYPEDEF_FLAGS_COLINDEX] &
			CLR_TYPEDEF_VIS_MASK) != CLR_TYPEDEF_VIS_PUBLIC) && ((mdMetadata->getTable(CLR_TABLE_TYPEDEF)
			[dwTokenRowIndex][CLR_TYPEDEF_FLAGS_COLINDEX] & CLR_TYPEDEF_VIS_MASK) != CLR_TYPEDEF_VIS_NESTED_PUBLIC))
			m_bPrivateType = true;
		else
			m_bPrivateType = false;
		m_dwTypeToken = CLR_TOKEN_TYPEDEF | (dwTokenRowIndex + 1);
		return tnName;
	}
	else
	{
		// TypeRef
		CLRTypeName tnName;
		tnName.sNamespace = mdMetadata->getTable(CLR_TABLE_TYPEREF)[dwTokenRowIndex].
			getString(CLR_TYPEREF_NAMESPACE_COLINDEX);
		tnName.sName = mdMetadata->getTable(CLR_TABLE_TYPEREF)[dwTokenRowIndex].
			getString(CLR_TYPEREF_NAME_COLINDEX);
		tnName.sOuterClass = mdMetadata->getFile().getOuterClassName(CLR_TOKEN_TYPEREF | (dwTokenRowIndex + 1));
		m_bPrivateType = false;
		m_dwTypeToken = CLR_TOKEN_TYPEREF | (dwTokenRowIndex + 1);
		return tnName;
	}
}


//*************************************************************************
// Method:	 CLRTypeSignature::writeByte
// Description: Writes a single byte to the end of a signature buffer
// Parameters:
//	pbSig - Signature buffer to write to.  Will be enlarged if it is not
//		big enough to hold the new data.
//  dwLen - Current length of the signature buffer data, will be updated.
//  dwMaxLen - Maximum length of the data in the current signature buffer,
//		will be updated if the buffer is enlarged.
//  bVal - Byte to write
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTypeSignature::writeByte(BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, BYTE bVal)
{
	if (dwLen >= dwMaxLen)
	{
		// Make a new, larger buffer
		dwMaxLen += SIGNATURE_DATA_REALLOC_SIZE;
		BYTE* pbNewData = new BYTE[dwMaxLen];
		if (!pbNewData)
			return false;

		// Copy the old data into the new buffer and switch to the new buffer
		memcpy(pbNewData, pbSig, dwLen);
		delete[] pbSig;
		pbSig = pbNewData;
	}

	pbSig[dwLen++] = bVal;
	return true;
}


//*************************************************************************
// Method:	 CLRTypeSignature::writeData
// Description: Writes a block of data to the end of a signature buffer
// Parameters:
//	pbSig - Signature buffer to write to.  Will be enlarged if it is not
//		big enough to hold the new data.
//  dwLen - Current length of the signature buffer data, will be updated.
//  dwMaxLen - Maximum length of the data in the current signature buffer,
//		will be updated if the buffer is enlarged.
//  pbData - Data to write
//  dwDataSize - Size in bytes of data to write
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTypeSignature::writeData(BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, const BYTE* pbData, DWORD dwDataSize)
{
	for (DWORD i=0; i<dwDataSize; i++)
	{
		if (!writeByte(pbSig, dwLen, dwMaxLen, pbData[i]))
			return false;
	}
	return true;
}


//*************************************************************************
// Method:	 CLRTypeSignature::writeCompressedInt
// Description: Writes a compressed integer, in the same format used by
//				readCompressedInt, to the end of a signature buffer
// Parameters:
//	pbSig - Signature buffer to write to.  Will be enlarged if it is not
//		big enough to hold the new data.
//  dwLen - Current length of the signature buffer data, will be updated.
//  dwMaxLen - Maximum length of the data in the current signature buffer,
//		will be updated if the buffer is enlarged.
//  dwValue - Integer to encode and write
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTypeSignature::writeCompressedInt(BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, DWORD dwValue)
{
	if (dwValue < 0x80)
	{
		if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)dwValue))
			return false;
		return true;
	}

	if (dwValue < 0x4000)
	{
		dwValue |= 0x8000;
		// Write out in big endian format
		if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)(dwValue >> 8)))
			return false;
		if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)(dwValue & 0xff)))
			return false;
		return true;
	}

	dwValue |= 0xc0000000;
	// Write out in big endian format
	if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)(dwValue >> 24)))
		return false;
	if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)((dwValue >> 16) & 0xff)))
		return false;
	if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)((dwValue >> 8) & 0xff)))
		return false;
	if (!writeByte(pbSig, dwLen, dwMaxLen, (BYTE)(dwValue & 0xff)))
		return false;
	return true;
}


//*************************************************************************
// Method:	 CLRTypeSignature::writeTypeToken
// Description: Writes a coded token representing a type name to the end
//				of a signature buffer
// Parameters:
//	pbSig - Signature buffer to write to.  Will be enlarged if it is not
//		big enough to hold the new data.
//  dwLen - Current length of the signature buffer data, will be updated.
//  dwMaxLen - Maximum length of the data in the current signature buffer,
//		will be updated if the buffer is enlarged.
//  tnName - Type to write
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTypeSignature::writeTypeToken(CLRMetadata* mdMetadata, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen,
									  const CLRTypeName& tnName)
{
	DWORD dwTypeToken = mdMetadata->getFile().findTypeToken(tnName);
	if (dwTypeToken == 0)
		return false;

	if (CLR_TOKEN_TYPE(dwTypeToken) == CLR_TABLE_TYPEDEF)
	{
		// Construct a TypeDefOrRef coded token.  Bottom two bits are the token type, TypeDef in this case
		DWORD dwTypeRID = CLR_TOKEN_RID(dwTypeToken);
		DWORD dwCodedToken = (dwTypeRID << 2) | CLR_TYPEDEFORREF_TYPEDEF;

		// Write the coded token as a compressed int
		if (!writeCompressedInt(pbSig, dwLen, dwMaxLen, dwCodedToken))
			return false;
	}
	else
	{
		// Construct a TypeDefOrRef coded token.  Bottom two bits are the token type, TypeRef in this case
		DWORD dwTypeRID = CLR_TOKEN_RID(dwTypeToken);
		DWORD dwCodedToken = (dwTypeRID << 2) | CLR_TYPEDEFORREF_TYPEREF;

		// Write the coded token as a compressed int
		if (!writeCompressedInt(pbSig, dwLen, dwMaxLen, dwCodedToken))
			return false;
	}

	return true;
}


//*************************************************************************
// Method:	 CLRTypeSignature::replaceTypeToken
// Description: Replaces a type token using a translation table
// Parameters:
//  pbOrigSig - Original signature containing type token
//  dwOffset - Reference to current offset in original signature
//	pbSig - Signature buffer to write to.  Will be enlarged if it is not
//		big enough to hold the new data.
//  dwLen - Current length of the signature buffer data, will be updated.
//  dwMaxLen - Maximum length of the data in the current signature buffer,
//		will be updated if the buffer is enlarged.
//  table - Translation table to use
//
// Return Value: true on success, false on failure
//*************************************************************************
void CLRTypeSignature::replaceTypeToken(const BYTE* pbOrigSig, DWORD& dwOffset, BYTE*& pbSig, DWORD& dwLen,
										DWORD& dwMaxLen, CLRTranslationTable& table)
{
	DWORD dwCodedToken = readCompressedInt(pbOrigSig, dwOffset);
	// TypeDefOrRef coded token, bottom 2 bits contain type of token
	DWORD dwTokenType = dwCodedToken & 3;
	DWORD dwTokenRowIndex = dwCodedToken >> 2;

	DWORD dwTokenValue;

	if (dwTokenType == CLR_TYPEDEFORREF_TYPEDEF)
		dwTokenValue = CLR_TOKEN_TYPEDEF | dwTokenRowIndex;
	else
		dwTokenValue = CLR_TOKEN_TYPEREF | dwTokenRowIndex;

	dwTokenValue = table.Translate(dwTokenValue);

	if (CLR_TOKEN_TYPE(dwTokenValue) == CLR_TABLE_TYPEDEF)
	{
		dwTokenRowIndex = CLR_TOKEN_RID(dwTokenValue);
		dwCodedToken = (dwTokenRowIndex << 2) | CLR_TYPEDEFORREF_TYPEDEF;
	}
	else
	{
		dwTokenRowIndex = CLR_TOKEN_RID(dwTokenValue);
		dwCodedToken = (dwTokenRowIndex << 2) | CLR_TYPEDEFORREF_TYPEREF;
	}

	writeCompressedInt(pbSig, dwLen, dwMaxLen, dwCodedToken);
}


//*************************************************************************
// Method:	 CLRTypeSignature::parse
// Description: Parses a type signature
// Parameters:
//	mdMetadata - Metadata containing the type signature
//  pbTypeSig - Type signature data
//
// Return Value: Size in bytes of the type signature
//*************************************************************************
DWORD CLRTypeSignature::parse(CLRMetadata* mdMetadata, const BYTE* pbTypeSig)
{
	DWORD dwOffset = 0;
	DWORD dwAssocDataBeginOffset, dwSizeCount, dwBoundCount, dwArgCount;

	m_bTypeCode = pbTypeSig[dwOffset++];

	switch (m_bTypeCode)
	{
	case CLR_DATATYPE_PTR:
		// Parse pointer type
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_BYREF:
		// Parse reference type
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_VALUETYPE:
		m_tnTypeName = readTypeToken(mdMetadata, pbTypeSig, dwOffset);
		break;
	case CLR_DATATYPE_CLASS:
		m_tnTypeName = readTypeToken(mdMetadata, pbTypeSig, dwOffset);
		break;
	case CLR_DATATYPE_GENERICPARAM:
	case CLR_DATATYPE_UNKNOWN_CLR12:
		// New CLR 1.2 type, meaning assumed
		dwArgCount = readCompressedInt(pbTypeSig, dwOffset);
		// Save in the associated data
		m_dwAssocDataSize = sizeof(DWORD);
		m_pbAssocData = new BYTE[sizeof(DWORD)];
		memcpy(m_pbAssocData, &dwArgCount, sizeof(DWORD));
		break;
	case CLR_DATATYPE_GENERIC:
		// New CLR 1.2 type, meaning assumed
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		// Parse template arguments
		dwAssocDataBeginOffset = dwOffset;
		dwArgCount = readCompressedInt(pbTypeSig, dwOffset);
		for (DWORD i=0; i<dwArgCount; i++)
		{
			CLRTypeSignature sig;
			dwOffset += sig.parse(mdMetadata, pbTypeSig + dwOffset);
		}
		// Save in the associated data
		m_dwAssocDataSize = dwOffset - dwAssocDataBeginOffset;
		m_pbAssocData = new BYTE[m_dwAssocDataSize];
		memcpy(m_pbAssocData, pbTypeSig + dwAssocDataBeginOffset, m_dwAssocDataSize);
		break;
	case CLR_DATATYPE_ARRAY:
		// Parse base type
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		// Save the offset before rank, sizes, and bounds, and calculate
		// how much information needs to be saved
		dwAssocDataBeginOffset = dwOffset;
		skipCompressedInt(pbTypeSig, dwOffset); // Skip rank
		dwSizeCount = readCompressedInt(pbTypeSig, dwOffset);
		for (DWORD i=0; i<dwSizeCount; i++)
			skipCompressedInt(pbTypeSig, dwOffset); // Skip sizes
		dwBoundCount = readCompressedInt(pbTypeSig, dwOffset);
		for (DWORD i=0; i<dwBoundCount; i++)
			skipCompressedInt(pbTypeSig, dwOffset); // Skip bounds
		// Save the rank, sizes, and bounds in the associated data
		m_dwAssocDataSize = dwOffset - dwAssocDataBeginOffset;
		m_pbAssocData = new BYTE[m_dwAssocDataSize];
		memcpy(m_pbAssocData, pbTypeSig + dwAssocDataBeginOffset, m_dwAssocDataSize);
		break;
	case CLR_DATATYPE_VECTOR:
		// Parse base type
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_FUNCPTR:
		// Read function pointer type signature
		m_psAssocMethodSig = new CLRSignature();
		dwOffset += m_psAssocMethodSig->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_MOD_REQUIRED:
		m_tnTypeName = readTypeToken(mdMetadata, pbTypeSig, dwOffset);
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_MOD_OPTIONAL:
		m_tnTypeName = readTypeToken(mdMetadata, pbTypeSig, dwOffset);
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_SENTINEL:
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	case CLR_DATATYPE_PINNED:
		m_ptsAssocType = new CLRTypeSignature();
		dwOffset += m_ptsAssocType->parse(mdMetadata, pbTypeSig + dwOffset);
		break;
	default:
		break;
	}

	return dwOffset;
}


//*************************************************************************
// Method:	 CLRTypeSignature::save
// Description: Writes a type signature to the end of a signature buffer
// Parameters:
//	mdMetadata - Metadata containing type signature
//	pbSig - Signature buffer to write to.  Will be enlarged if it is not
//		big enough to hold the new data.
//  dwLen - Current length of the signature buffer data, will be updated.
//  dwMaxLen - Maximum length of the data in the current signature buffer,
//		will be updated if the buffer is enlarged.
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRTypeSignature::save(CLRMetadata* mdMetadata, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen)
{
	// Write out type code
	if (!writeByte(pbSig, dwLen, dwMaxLen, m_bTypeCode))
		return false;

	// Write out associated information
	switch (m_bTypeCode)
	{
	case CLR_DATATYPE_PTR:
		// Write pointer type
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	case CLR_DATATYPE_BYREF:
		// Write reference type
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	case CLR_DATATYPE_VALUETYPE:
		if (!writeTypeToken(mdMetadata, pbSig, dwLen, dwMaxLen, m_tnTypeName))
			return false;
		break;
	case CLR_DATATYPE_CLASS:
		if (!writeTypeToken(mdMetadata, pbSig, dwLen, dwMaxLen, m_tnTypeName))
			return false;
		break;
	case CLR_DATATYPE_GENERICPARAM:
	case CLR_DATATYPE_UNKNOWN_CLR12:
		// New CLR 1.2 type, meaning assumed
		if (!writeCompressedInt(pbSig, dwLen, dwMaxLen, *((DWORD*)m_pbAssocData)))
			return false;
		break;
	case CLR_DATATYPE_GENERIC:
		// New CLR 1.2 type, meaning assumed
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		if (!writeData(pbSig, dwLen, dwMaxLen, m_pbAssocData, m_dwAssocDataSize))
			return false;
		break;
	case CLR_DATATYPE_ARRAY:
		// Write base type
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		// Write rank, size, and bound information
		if (!writeData(pbSig, dwLen, dwMaxLen, m_pbAssocData, m_dwAssocDataSize))
			return false;
		break;
	case CLR_DATATYPE_VECTOR:
		// Write base type
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	case CLR_DATATYPE_FUNCPTR:
		// Write function pointer type signature
		if (!m_psAssocMethodSig->save(mdMetadata))
			return false;
		if (!writeData(pbSig, dwLen, dwMaxLen, m_psAssocMethodSig->getData(), m_psAssocMethodSig->getSize()))
			return false;
		break;
	case CLR_DATATYPE_MOD_REQUIRED:
		if (!writeTypeToken(mdMetadata, pbSig, dwLen, dwMaxLen, m_tnTypeName))
			return false;
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	case CLR_DATATYPE_MOD_OPTIONAL:
		if (!writeTypeToken(mdMetadata, pbSig, dwLen, dwMaxLen, m_tnTypeName))
			return false;
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	case CLR_DATATYPE_SENTINEL:
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	case CLR_DATATYPE_PINNED:
		if (!m_ptsAssocType->save(mdMetadata, pbSig, dwLen, dwMaxLen))
			return false;
		break;
	default:
		break;
	}

	return true;
}


//*************************************************************************
// Method:	 CLRSignature::replaceTokens
// Description: Replaces tokens in the signature and stores the new
//				signature into pbSig
// Parameters:
//  pbOrigSig - Original signature
//	pbSig - Signature to write the new data to
//  dwLen - Reference to length of dest signature
//  dwMaxLen - Reference to data area size of dest signature
//
// Return Value: size of signature in pbOrigSig
//*************************************************************************
DWORD CLRTypeSignature::replaceTokens(const BYTE* pbOrigSig, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen,
									  CLRTranslationTable& table)
{
	DWORD dwOffset = 0;
	DWORD dwValue, dwSizeCount, dwBoundCount, dwArgCount;
	CLRSignature* psAssocMethodSig;

	BYTE bTypeCode = pbOrigSig[dwOffset++];
	writeByte(pbSig, dwLen, dwMaxLen, bTypeCode);

	switch (bTypeCode)
	{
	case CLR_DATATYPE_PTR:
		// Parse pointer type
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_BYREF:
		// Parse reference type
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_VALUETYPE:
		replaceTypeToken(pbOrigSig, dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_CLASS:
		replaceTypeToken(pbOrigSig, dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_GENERICPARAM:
	case CLR_DATATYPE_UNKNOWN_CLR12:
		// New CLR 1.2 type, meaning assumed
		dwValue = readCompressedInt(pbOrigSig, dwOffset);
		writeCompressedInt(pbSig, dwLen, dwMaxLen, dwValue);
		break;
	case CLR_DATATYPE_GENERIC:
		// New CLR 1.2 type, meaning assumed
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		dwArgCount = readCompressedInt(pbOrigSig, dwOffset);
		writeCompressedInt(pbSig, dwLen, dwMaxLen, dwArgCount);
		for (DWORD i=0; i<dwArgCount; i++)
			dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_ARRAY:
		// Parse base type
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		// Save the offset before rank, sizes, and bounds, and calculate
		// how much information needs to be saved
		dwValue = readCompressedInt(pbOrigSig, dwOffset); // rank
		writeCompressedInt(pbSig, dwLen, dwMaxLen, dwValue);
		dwSizeCount = readCompressedInt(pbOrigSig, dwOffset);
		writeCompressedInt(pbSig, dwLen, dwMaxLen, dwSizeCount);
		for (DWORD i=0; i<dwSizeCount; i++)
		{
			dwValue = readCompressedInt(pbOrigSig, dwOffset);
			writeCompressedInt(pbSig, dwLen, dwMaxLen, dwValue);
		}
		dwBoundCount = readCompressedInt(pbOrigSig, dwOffset);
		writeCompressedInt(pbSig, dwLen, dwMaxLen, dwBoundCount);
		for (DWORD i=0; i<dwBoundCount; i++)
		{
			dwValue = readCompressedInt(pbOrigSig, dwOffset);
			writeCompressedInt(pbSig, dwLen, dwMaxLen, dwValue);
		}
		break;
	case CLR_DATATYPE_VECTOR:
		// Parse base type
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_FUNCPTR:
		// Read function pointer type signature
		psAssocMethodSig = new CLRSignature();
		dwOffset += psAssocMethodSig->replaceTokens(pbOrigSig + dwOffset, table);
		writeData(pbSig, dwLen, dwMaxLen, psAssocMethodSig->getData(), psAssocMethodSig->getSize());
		delete psAssocMethodSig;
		break;
	case CLR_DATATYPE_MOD_REQUIRED:
		replaceTypeToken(pbOrigSig, dwOffset, pbSig, dwLen, dwMaxLen, table);
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_MOD_OPTIONAL:
		replaceTypeToken(pbOrigSig, dwOffset, pbSig, dwLen, dwMaxLen, table);
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_SENTINEL:
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	case CLR_DATATYPE_PINNED:
		dwOffset += CLRTypeSignature::replaceTokens(pbOrigSig + dwOffset, pbSig, dwLen, dwMaxLen, table);
		break;
	default:
		break;
	}
	return dwOffset;
}


//*************************************************************************
// Method:	 CLRTypeSignature::operator==
// Description: Determines if two type signatures are equal
// Parameters:
//	tsType - Second type signature to check
//
// Return Value: true if the type signatures are equal, false otherwise
//*************************************************************************
bool CLRTypeSignature::operator==(const CLRTypeSignature& tsType)
{
	if (m_bPrivateType)
	{
		// Private types in a replacement library are represented using System.Object
		if (tsType.m_bTypeCode == CLR_DATATYPE_OBJECT)
			return true;
	}
	else if (tsType.m_bPrivateType)
	{
		// Private types in a replacement library are represented using System.Object
		if (m_bTypeCode == CLR_DATATYPE_OBJECT)
			return true;
	}
	else if (((m_bTypeCode == CLR_DATATYPE_ARRAY) || (m_bTypeCode == CLR_DATATYPE_VECTOR)) &&
		m_ptsAssocType && (m_ptsAssocType->isPrivate()))
	{
		if ((tsType.m_bTypeCode == CLR_DATATYPE_CLASS) && (tsType.m_tnTypeName.sNamespace == "System") &&
			(tsType.m_tnTypeName.sName == "Array"))
			return true;
	}
	else if (((tsType.m_bTypeCode == CLR_DATATYPE_ARRAY) || (tsType.m_bTypeCode == CLR_DATATYPE_VECTOR)) &&
		tsType.m_ptsAssocType && (tsType.m_ptsAssocType->isPrivate()))
	{
		if ((m_bTypeCode == CLR_DATATYPE_CLASS) && (m_tnTypeName.sNamespace == "System") &&
			(m_tnTypeName.sName == "Array"))
			return true;
	}

	if (m_bTypeCode != tsType.m_bTypeCode)
		return false;
	if (m_tnTypeName.sNamespace != (wchar_t*)tsType.m_tnTypeName.sNamespace)
		return false;
	if (m_tnTypeName.sName != (wchar_t*)tsType.m_tnTypeName.sName)
		return false;

	if (m_pbAssocData)
	{
		if (!tsType.m_pbAssocData)
			return false;
		if (m_dwAssocDataSize != tsType.m_dwAssocDataSize)
			return false;
		if (memcmp(m_pbAssocData, tsType.m_pbAssocData, m_dwAssocDataSize) != 0)
			return false;
	}

	if (m_ptsAssocType)
	{
		if (!tsType.m_ptsAssocType)
			return false;
		if ((*m_ptsAssocType) != (*tsType.m_ptsAssocType))
			return false;
	}

	if (m_psAssocMethodSig)
	{
		if (!tsType.m_psAssocMethodSig)
			return false;
		if ((*m_psAssocMethodSig) != (*tsType.m_psAssocMethodSig))
			return false;
	}

	return true;
}


//*************************************************************************
// Method:	 CLRTypeSignature::operator!=
// Description: Determines if two type signatures are different
// Parameters:
//	tsType - Second type signature to check
//
// Return Value: true if the type signatures are different, false otherwise
//*************************************************************************
bool CLRTypeSignature::operator!=(const CLRTypeSignature& tsType)
{
	return !((*this) == tsType);
}


//*************************************************************************
// Method:	 CLRTypeSignature::getTypeCode
// Description: Returns the type code, which is one of the CLR_DATATYPE_*
//				defines in CLRMetadataDefs.h
// Parameters:
//	None
//
// Return Value: Type code
//*************************************************************************
BYTE CLRTypeSignature::getTypeCode() const
{
	return m_bTypeCode;
}


//*************************************************************************
// Method:	 CLRTypeSignature::getAssocType
// Description: Gets the type signature associated with this type
//				signature
// Parameters:
//	None
//
// Return Value: Pointer to associated type signature, NULL if none
//*************************************************************************
CLRTypeSignature* CLRTypeSignature::getAssocType() const
{
	return m_ptsAssocType;
}


//*************************************************************************
// Method:	 CLRTypeSignature::getAssocMethodSig
// Description: Gets the method signature associated with this type
//				signature
// Parameters:
//	None
//
// Return Value: Pointer to associated method signature, NULL if none
//*************************************************************************
CLRSignature* CLRTypeSignature::getAssocMethodSig() const
{
	return m_psAssocMethodSig;
}


//*************************************************************************
// Method:	 CLRTypeSignature::getAssocTypeName
// Description: Gets the type name associated with this type signature
// Parameters:
//	None
//
// Return Value: Associated type name
//*************************************************************************
CLRTypeName CLRTypeSignature::getAssocTypeName() const
{
	return m_tnTypeName;
}


//*************************************************************************
// Method:	 CLRTypeSignature::isPrivate
// Description: Determines if this type has any private components
// Parameters:
//	None
//
// Return Value: true if type is private
//*************************************************************************
bool CLRTypeSignature::isPrivate() const
{
	if (m_bPrivateType)
		return true;
	if (m_ptsAssocType && m_ptsAssocType->m_bPrivateType)
		return true;
	if (m_psAssocMethodSig)
	{
		if (m_psAssocMethodSig->getReturnType().isPrivate())
			return true;
		for (DWORD i=0; i<m_psAssocMethodSig->getParamTypes().GetLength(); i++)
		{
			if (m_psAssocMethodSig->getParamTypes()[i].isPrivate())
				return true;
		}
	}
	return false;
}


//*************************************************************************
// Method:	 CLRTypeSignature::getTypeToken
// Description: Returns the type token (only valid for data types
//				CLR_DATATYPE_CLASS or CLR_DATATYPE_VALUETYPE)
// Parameters:
//	None
//
// Return Value: Type token, or zero if no token
//*************************************************************************
DWORD CLRTypeSignature::getTypeToken() const
{
	return m_dwTypeToken;
}
