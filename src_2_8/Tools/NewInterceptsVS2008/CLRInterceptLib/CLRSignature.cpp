//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRSignature.h
//
// DESCRIPTION: Contains classes to manage method and field signatures in
//				.NET classes.
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 4 Apr 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include <string.h>
#include "CLRSignature.h"
#include "CLRMetadataDefs.h"


//*************************************************************************
// Method:	 CLRSignature::CLRSignature
// Description: Default constructor for the CLRSignature type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRSignature::CLRSignature()
{
	m_pbData = NULL;
	m_dwDataSize = 0;
	m_dwDataMaxSize = 0;
	m_dwGenericParamCount = 0;
}


//*************************************************************************
// Method:	 CLRSignature::CLRSignature
// Description: Copy constructor for the CLRSignature type.
// Parameters:
//	sSig - Signature to copy
//
// Return Value: None
//*************************************************************************
CLRSignature::CLRSignature(const CLRSignature& sSig)
{
	m_dwDataSize = sSig.m_dwDataSize;
	m_bCallConv = sSig.m_bCallConv;
	m_tsReturnType = sSig.m_tsReturnType;
	m_dwGenericParamCount = sSig.m_dwGenericParamCount;
	m_vtsParamTypes = sSig.m_vtsParamTypes;

	if (sSig.m_pbData)
	{
		// Create buffer to hold signature data, padding to allow for expansion when modifying
		m_pbData = new BYTE[m_dwDataSize];
		m_dwDataMaxSize = m_dwDataSize;
		memcpy(m_pbData, sSig.m_pbData, m_dwDataSize);
	}
	else
	{
		m_pbData = NULL;
		m_dwDataMaxSize = 0;
	}
}


//*************************************************************************
// Method:	 CLRSignature::~CLRSignature
// Description: Destructor for the CLRSignature type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRSignature::~CLRSignature()
{
	if (m_pbData)
	{
		delete[] m_pbData;
		m_pbData = NULL;
	}
}


//*************************************************************************
// Method:	 CLRSignature::operator==
// Description: Tests to see if two signatures are equal
// Parameters:
//	sOtherSig - Second signature to check
//
// Return Value: true if the signatures are equal, false otherwise
//*************************************************************************
bool CLRSignature::operator==(const CLRSignature& sOtherSig)
{
	if (m_bCallConv != sOtherSig.m_bCallConv)
		return false;
	if (m_tsReturnType != sOtherSig.m_tsReturnType)
		return false;
	if (m_vtsParamTypes.GetLength() != sOtherSig.m_vtsParamTypes.GetLength())
		return false;
	for (DWORD i=0; i<m_vtsParamTypes.GetLength(); i++)
	{
		if (m_vtsParamTypes[i] != sOtherSig.m_vtsParamTypes[i])
			return false;
	}
	return true;
}


//*************************************************************************
// Method:	 CLRSignature::operator!=
// Description: Tests to see if two signatures are different
// Parameters:
//	sOtherSig - Second signature to check
//
// Return Value: true if the signatures are different, false otherwise
//*************************************************************************
bool CLRSignature::operator!=(const CLRSignature& sOtherSig)
{
	return !((*this) == sOtherSig);
}


//*************************************************************************
// Method:	 CLRSignature::operator=
// Description: Assignment operator for CLRSignature type.
// Parameters:
//	sSig - New signature
//
// Return Value: Reference to the value assigned
//*************************************************************************
CLRSignature& CLRSignature::operator=(const CLRSignature& sSig)
{
	if (m_pbData)
	{
		delete[] m_pbData;
		m_pbData = NULL;
		m_dwDataMaxSize = 0;
	}

	m_dwDataSize = sSig.m_dwDataSize;
	m_bCallConv = sSig.m_bCallConv;
	m_tsReturnType = sSig.m_tsReturnType;
	m_dwGenericParamCount = sSig.m_dwGenericParamCount;
	m_vtsParamTypes = sSig.m_vtsParamTypes;

	if (sSig.m_pbData)
	{
		// Create buffer to hold signature data, padding to allow for expansion when modifying
		m_pbData = new BYTE[m_dwDataSize];
		m_dwDataMaxSize = m_dwDataSize;
		memcpy(m_pbData, sSig.m_pbData, m_dwDataSize);
	}

	return *this;
}


//*************************************************************************
// Method:	 CLRSignature::readCompressedInt
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
DWORD CLRSignature::readCompressedInt(const BYTE* pbTypeSig, DWORD& dwOffset)
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
// Method:	 CLRSignature::writeByte
// Description: Writes a single byte to the end of the signature data buffer
// Parameters:
//	bVal - Byte to write
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRSignature::writeByte(BYTE bVal)
{
	if (m_dwDataSize >= m_dwDataMaxSize)
	{
		// Make a new, larger buffer
		m_dwDataMaxSize += SIGNATURE_DATA_REALLOC_SIZE;
		BYTE* pbNewData = new BYTE[m_dwDataMaxSize];
		if (!pbNewData)
			return false;

		// Copy the old data into the new buffer and switch to the new buffer
		memcpy(pbNewData, m_pbData, m_dwDataSize);
		delete[] m_pbData;
		m_pbData = pbNewData;
	}

	m_pbData[m_dwDataSize++] = bVal;
	return true;
}


//*************************************************************************
// Method:	 CLRSignature::writeCompressedInt
// Description: Adds a compressed integer, in the same format as used in
//				readCompressedInt, to the end of the signature data buffer
// Parameters:
//	dwValue - Integer to encode
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRSignature::writeCompressedInt(DWORD dwValue)
{
	if (dwValue < 0x80)
	{
		if (!writeByte((BYTE)dwValue))
			return false;
		return true;
	}

	if (dwValue < 0x4000)
	{
		dwValue |= 0x8000;
		// Write out in big endian format
		if (!writeByte((BYTE)(dwValue >> 8)))
			return false;
		if (!writeByte((BYTE)(dwValue & 0xff)))
			return false;
		return true;
	}

	dwValue |= 0xc0000000;
	// Write out in big endian format
	if (!writeByte((BYTE)(dwValue >> 24)))
		return false;
	if (!writeByte((BYTE)((dwValue >> 16) & 0xff)))
		return false;
	if (!writeByte((BYTE)((dwValue >> 8) & 0xff)))
		return false;
	if (!writeByte((BYTE)(dwValue & 0xff)))
		return false;
	return true;
}


//*************************************************************************
// Method:	 CLRSignature::parse
// Description: Parses a method or field signature and stores the
//				information in data structures accessible to the caller.
// Parameters:
//	mdMetadata - Metadata that contains this signature
//  pbSig - Pointer to the signature data
//
// Return Value: Size in bytes of the signature
//*************************************************************************
DWORD CLRSignature::parse(CLRMetadata* mdMetadata, const BYTE* pbSig)
{
	DWORD dwOffset = 0;

	m_bCallConv = pbSig[dwOffset++];

	if (m_bCallConv == CLR_CALLCONV_FIELD)
	{
		// Field signature, has a single basic type representing the type of the field
		dwOffset += m_tsReturnType.parse(mdMetadata, pbSig + dwOffset);
	}
	else if (m_bCallConv == CLR_CALLCONV_LOCAL)
	{
		// Local variables
		DWORD dwParamCount = readCompressedInt(pbSig, dwOffset);
		for (DWORD i=0; i<dwParamCount; i++)
		{
			CLRTypeSignature tsType;
			dwOffset += tsType.parse(mdMetadata, pbSig + dwOffset);
			m_vtsParamTypes.Add(tsType);
		}
	}
	else if (m_bCallConv & CLR_CALLCONV_GENERIC)
	{
		// New CLR 1.2 type, meaning assumed
		m_dwGenericParamCount = readCompressedInt(pbSig, dwOffset);
		DWORD dwParamCount = readCompressedInt(pbSig, dwOffset);

		dwOffset += m_tsReturnType.parse(mdMetadata, pbSig + dwOffset);
		for (DWORD i=0; i<dwParamCount; i++)
		{
			CLRTypeSignature tsType;
			dwOffset += tsType.parse(mdMetadata, pbSig + dwOffset);
			m_vtsParamTypes.Add(tsType);
		}
	}
	else
	{
		// Method signature
		DWORD dwParamCount = readCompressedInt(pbSig, dwOffset);

		dwOffset += m_tsReturnType.parse(mdMetadata, pbSig + dwOffset);
		for (DWORD i=0; i<dwParamCount; i++)
		{
			CLRTypeSignature tsType;
			dwOffset += tsType.parse(mdMetadata, pbSig + dwOffset);
			m_vtsParamTypes.Add(tsType);
		}
	}

	// Save the signature data
	m_dwDataSize = dwOffset;
	m_pbData = new BYTE[m_dwDataSize];
	m_dwDataMaxSize = m_dwDataSize;
	memcpy(m_pbData, pbSig, m_dwDataSize);

	return m_dwDataSize;
}


//*************************************************************************
// Method:	 CLRSignature::save
// Description: Saves the signature to the signature data buffer contained
//				in this CLRSignature instance.  Use getData and getSize
//				to retrieve this data.
// Parameters:
//	mdMetadata - Metadata that will contain this signature
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRSignature::save(CLRMetadata* mdMetadata)
{
	m_dwDataSize = 0;

	if (!writeByte(m_bCallConv))
		return false;

	if (m_bCallConv == CLR_CALLCONV_FIELD)
	{
		// Field signature
		if (!m_tsReturnType.save(mdMetadata, m_pbData, m_dwDataSize, m_dwDataMaxSize))
			return false;
	}
	else if (m_bCallConv == CLR_CALLCONV_LOCAL)
	{
		// Local variables
		if (!writeCompressedInt((DWORD)m_vtsParamTypes.GetLength()))
			return false;
		for (DWORD i=0; i<m_vtsParamTypes.GetLength(); i++)
		{
			if (!m_vtsParamTypes[i].save(mdMetadata, m_pbData, m_dwDataSize, m_dwDataMaxSize))
				return false;
		}
	}
	else if (m_bCallConv & CLR_CALLCONV_GENERIC)
	{
		// New CLR 1.2 type, meaning assumed
		if (!writeCompressedInt(m_dwGenericParamCount))
			return false;
		if (!writeCompressedInt((DWORD)m_vtsParamTypes.GetLength()))
			return false;
		if (!m_tsReturnType.save(mdMetadata, m_pbData, m_dwDataSize, m_dwDataMaxSize))
			return false;
		for (DWORD i=0; i<m_vtsParamTypes.GetLength(); i++)
		{
			if (!m_vtsParamTypes[i].save(mdMetadata, m_pbData, m_dwDataSize, m_dwDataMaxSize))
				return false;
		}
	}
	else
	{
		// Method signature
		if (!writeCompressedInt((DWORD)m_vtsParamTypes.GetLength()))
			return false;
		if (!m_tsReturnType.save(mdMetadata, m_pbData, m_dwDataSize, m_dwDataMaxSize))
			return false;
		for (DWORD i=0; i<m_vtsParamTypes.GetLength(); i++)
		{
			if (!m_vtsParamTypes[i].save(mdMetadata, m_pbData, m_dwDataSize, m_dwDataMaxSize))
				return false;
		}
	}

	return true;
}


//*************************************************************************
// Method:	 CLRSignature::replaceTokens
// Description: Replaces tokens in the signature and stores the new
//				signature in the m_pbData member
// Parameters:
//	pbSig - Signature in which to replace tokens
//  table - Translation table to use
//
// Return Value: size of signature in pbSig
//*************************************************************************
DWORD CLRSignature::replaceTokens(const BYTE* pbSig, CLRTranslationTable& table)
{
	DWORD dwOffset = 0;

	m_bCallConv = pbSig[dwOffset++];
	writeByte(m_bCallConv);

	if (m_bCallConv == CLR_CALLCONV_FIELD)
	{
		// Field signature, has a single basic type representing the type of the field
		dwOffset += CLRTypeSignature::replaceTokens(pbSig + dwOffset, m_pbData, m_dwDataSize, m_dwDataMaxSize, table);
	}
	else if (m_bCallConv == CLR_CALLCONV_LOCAL)
	{
		// Local variables
		DWORD dwParamCount = readCompressedInt(pbSig, dwOffset);
		writeCompressedInt(dwParamCount);
		for (DWORD i=0; i<dwParamCount; i++)
			dwOffset += CLRTypeSignature::replaceTokens(pbSig + dwOffset, m_pbData, m_dwDataSize, m_dwDataMaxSize, table);
	}
	else if (m_bCallConv & CLR_CALLCONV_GENERIC)
	{
		// New CLR 1.2 type, meaning assumed
		DWORD dwGenericParamCount = readCompressedInt(pbSig, dwOffset);
		writeCompressedInt(dwGenericParamCount);
		DWORD dwParamCount = readCompressedInt(pbSig, dwOffset);
		writeCompressedInt(dwParamCount);
		dwOffset += CLRTypeSignature::replaceTokens(pbSig + dwOffset, m_pbData, m_dwDataSize, m_dwDataMaxSize, table);
		for (DWORD i=0; i<dwParamCount; i++)
			dwOffset += CLRTypeSignature::replaceTokens(pbSig + dwOffset, m_pbData, m_dwDataSize, m_dwDataMaxSize, table);
	}
	else
	{
		// Method signature
		DWORD dwParamCount = readCompressedInt(pbSig, dwOffset);
		writeCompressedInt(dwParamCount);
		dwOffset += CLRTypeSignature::replaceTokens(pbSig + dwOffset, m_pbData, m_dwDataSize, m_dwDataMaxSize, table);
		for (DWORD i=0; i<dwParamCount; i++)
			dwOffset += CLRTypeSignature::replaceTokens(pbSig + dwOffset, m_pbData, m_dwDataSize, m_dwDataMaxSize, table);
	}

	return dwOffset;
}


//*************************************************************************
// Method:	 CLRSignature::convertToInstanceMethod
// Description: Converts a static handler method signature to an instance
//				signature.  This is done by removing the first "this"
//				parameter, as it is implied in an instance signature.
// Parameters:
//	None
//
// Return Value: Instance method signature
//*************************************************************************
CLRSignature CLRSignature::convertToInstanceMethod() const
{
	if (m_vtsParamTypes.GetLength() == 0)
	{
		// Must have at least one parameter ("this" parameter)
		return *this;
	}

	if ((m_vtsParamTypes[0].getTypeCode() != CLR_DATATYPE_OBJECT) &&
		(m_vtsParamTypes[0].getTypeCode() != CLR_DATATYPE_CLASS) &&
		(m_vtsParamTypes[0].getTypeCode() != CLR_DATATYPE_VALUETYPE) &&
		(m_vtsParamTypes[0].getTypeCode() != CLR_DATATYPE_PTR) &&
		(m_vtsParamTypes[0].getTypeCode() != CLR_DATATYPE_BYREF))
	{
		// First parameter must be a "this" parameter
		return *this;
	}

	// Create a new signature that lacks the first "this" paramater, and has
	// the instance method calling convention
	CLRSignature psNewSig(*this);
	psNewSig.removeFirstParam();
	psNewSig.setCallConv(psNewSig.getCallConv() | CLR_CALLCONV_INSTANCE);
	return psNewSig;
}


//*************************************************************************
// Method:	 CLRSignature::getParamCount
// Description: Returns the number of parameters passed into the method,
//				including the implied "this" parameter if it exists
// Parameters:
//	None
//
// Return Value: Number of parameters
//*************************************************************************
DWORD CLRSignature::getParamCount() const
{
	if ((m_bCallConv & CLR_CALLCONV_INSTANCE) && (!(m_bCallConv & CLR_CALLCONV_EXPLICITTHIS)))
	{
		// Instance methods have an implied "this" parameter
		return (DWORD)m_vtsParamTypes.GetLength() + 1;
	}
	return (DWORD)m_vtsParamTypes.GetLength();
}


//*************************************************************************
// Method:	 CLRSignature::getData
// Description: Returns the signature data buffer used by the save method
// Parameters:
//	None
//
// Return Value: Pointer to the signature data
//*************************************************************************
const BYTE* CLRSignature::getData() const
{
	return m_pbData;
}


//*************************************************************************
// Method:	 CLRSignature::getSize
// Description: Gets the size of the data in the signature data buffer
// Parameters:
//	None
//
// Return Value: Size in bytes of the signature data buffer
//*************************************************************************
DWORD CLRSignature::getSize() const
{
	return m_dwDataSize;
}


//*************************************************************************
// Method:	 CLRSignature::getCallConv
// Description: Gets the calling convention of this signature.  One of the
//				CLR_CALLCONV_* defines in CLRMetadataDefs.h
// Parameters:
//	None
//
// Return Value: Calling convention
//*************************************************************************
BYTE CLRSignature::getCallConv() const
{
	return m_bCallConv;
}


//*************************************************************************
// Method:	 CLRSignature::setCallConv
// Description: Sets the calling convention for this signature
// Parameters:
//	bCallConv - Calling convention, one of the CLR_CALLCONV_* defines
//
// Return Value: None
//*************************************************************************
void CLRSignature::setCallConv(BYTE bCallConv)
{
	m_bCallConv = bCallConv;
}


//*************************************************************************
// Method:	 CLRSignature::removeFirstParam
// Description: Removes the first parameter from the signature.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void CLRSignature::removeFirstParam()
{
	m_vtsParamTypes.Remove(0);
}


//*************************************************************************
// Method:	 CLRSignature::setReturnType
// Description: Sets the return value type for this signature
// Parameters:
//	tsTypeSig - Type signature for the return value
//
// Return Value: None
//*************************************************************************
void CLRSignature::setReturnType(const CLRTypeSignature& tsTypeSig)
{
	m_tsReturnType = tsTypeSig;
}


//*************************************************************************
// Method:	 CLRSignature::insertParamAtEnd
// Description: Inserts a new parameter as the last parameter
// Parameters:
//	tsTypeSig - Type signature of the new parameter
//
// Return Value: None
//*************************************************************************
void CLRSignature::insertParamAtEnd(const CLRTypeSignature& tsTypeSig)
{
	m_vtsParamTypes.Add(tsTypeSig);
}


//*************************************************************************
// Method:	 CLRSignature::getParamTypes
// Description: Returns an array of the types of each of the parameters
// Parameters:
//	None
//
// Return Value: Array containing the type signatures of each parameter
//*************************************************************************
const SiUtils::SiArray<CLRTypeSignature>& CLRSignature::getParamTypes() const
{
	return m_vtsParamTypes;
}
