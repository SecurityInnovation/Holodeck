//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRMethod.h
//
// DESCRIPTION: Contains classes to manage the IL code in a method within
//				a .NET executable.
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 3 Apr 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include "PEFile.h"
#include "CLRMethod.h"
#include "ILInstr.h"

using namespace SiUtils;


//*************************************************************************
// Method:	 CLRMethod::CLRMethod
// Description: Default constructor for the CLRMethod type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRMethod::CLRMethod()
{
	m_pFile = NULL;
	m_dwTypeIndex = m_dwMethodIndex = 0;

	m_wMethodFlags = 0;
	m_wMaxStack = 8; // default
	m_dwLocalVarToken = 0;
	m_sLocalVars.setCallConv(CLR_CALLCONV_LOCAL);

	m_pbILCode = NULL;
	m_dwCodeSize = 0;
}


//*************************************************************************
// Method:	 CLRMethod::CLRMethod
// Description: Copy constructor for the CLRMethod type.
// Parameters:
//	mMethod - CLRMethod object to copy
//
// Return Value: None
//*************************************************************************
CLRMethod::CLRMethod(const CLRMethod& mMethod)
{
	m_pFile = mMethod.m_pFile;
	m_dwTypeIndex = mMethod.m_dwTypeIndex;
	m_dwMethodIndex = mMethod.m_dwMethodIndex;
	m_wMethodFlags = mMethod.m_wMethodFlags;
	m_wMaxStack = mMethod.m_wMaxStack;
	m_dwLocalVarToken = mMethod.m_dwLocalVarToken;
	m_sLocalVars = mMethod.m_sLocalVars;
	m_vExceptions = mMethod.m_vExceptions;

	if (mMethod.m_pbILCode)
	{
		m_dwCodeSize = mMethod.m_dwCodeSize;
		m_pbILCode = new BYTE[m_dwCodeSize];
		memcpy(m_pbILCode, mMethod.m_pbILCode, m_dwCodeSize);
	}
	else
	{
		m_pbILCode = NULL;
		m_dwCodeSize = 0;
	}
}


//*************************************************************************
// Method:	 CLRMethod::~CLRMethod
// Description: Destructor for the CLRMethod type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRMethod::~CLRMethod()
{
	if (m_pbILCode)
	{
		delete[] m_pbILCode;
		m_pbILCode = NULL;
	}
}


//*************************************************************************
// Method:	 CLRMethod::operator=
// Description: Assignment operator for the CLRMethod type.
// Parameters:
//	mMethod - New method contents
//
// Return Value: Reference to the value assigned
//*************************************************************************
CLRMethod& CLRMethod::operator=(const CLRMethod& mMethod)
{
	if (m_pbILCode)
	{
		delete[] m_pbILCode;
		m_pbILCode = NULL;
		m_dwCodeSize = 0;
	}

	m_pFile = mMethod.m_pFile;
	m_dwTypeIndex = mMethod.m_dwTypeIndex;
	m_dwMethodIndex = mMethod.m_dwMethodIndex;
	m_wMethodFlags = mMethod.m_wMethodFlags;
	m_wMaxStack = mMethod.m_wMaxStack;
	m_dwLocalVarToken = mMethod.m_dwLocalVarToken;
	m_sLocalVars = mMethod.m_sLocalVars;
	m_vExceptions = mMethod.m_vExceptions;

	if (mMethod.m_pbILCode)
	{
		m_dwCodeSize = mMethod.m_dwCodeSize;
		m_pbILCode = new BYTE[m_dwCodeSize];
		memcpy(m_pbILCode, mMethod.m_pbILCode, m_dwCodeSize);
	}

	return *this;
}


//*************************************************************************
// Method:	 CLRMethod::parse
// Description: Parses the method data and stores the header and IL code
//				in data structures accessible to the caller.
// Parameters:
//	pFile - PE file containing method to parse
//  dwTypeIndex - TypeDef row index of the type containing the method
//  dwMethodIndex - Method row index of the method
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMethod::parse(PEFile* pFile, DWORD dwTypeIndex, DWORD dwMethodIndex)
{
	m_pFile = pFile;
	m_dwTypeIndex = dwTypeIndex;
	m_dwMethodIndex = dwMethodIndex;

	// Fail if method is native
	if (pFile->getMetadata().getTable(CLR_TABLE_METHOD)[dwMethodIndex][CLR_METHOD_FLAGS_COLINDEX] &
		CLR_METHOD_IMPL_PINVOKE)
		return false;

	// Read in the signature if there is one
	const CLRBlobHeapEntry* pSigBlob = pFile->getMetadata().getBlob(pFile->getMetadata().getTable(
		CLR_TABLE_METHOD)[dwMethodIndex][CLR_METHOD_SIGNATURE_COLINDEX]);
	if (pSigBlob)
		m_sSig.parse(&pFile->getMetadata(), pSigBlob->pbData);

	// Find the method data
	DWORD dwRVA = pFile->getMetadata().getTable(CLR_TABLE_METHOD)[dwMethodIndex][CLR_METHOD_RVA_COLINDEX];
	if (dwRVA == 0)
	{
		// No method code, just return now
		return true;
	}

	if (!parse(pFile, dwRVA))
		return false;
	return true;
}


//*************************************************************************
// Method:	 CLRMethod::parse
// Description: Parses the method data and stores the header and IL code
//				in data structures accessible to the caller.
// Parameters:
//	pFile - PE file containing method to parse
//  dwRVA - RVA to method
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMethod::parse(PEFile* pFile, DWORD dwRVA)
{
	const BYTE* pMethodData = pFile->rvaToPointer(dwRVA);
	if (!pMethodData)
	{
		// No method code, just return now
		return true;
	}

	const BYTE* pILPtr; // Pointer to IL instructions
	DWORD dwILOffset; // Offset from start of header to IL instructions
	if ((pMethodData[0] & 3) == 2) // tiny header
	{
		// Tiny header contains only the code size
		m_dwCodeSize = pMethodData[0] >> 2;
		pILPtr = pMethodData + 1;
		dwILOffset = 1;
	}
	else if ((pMethodData[0] & 3) == 3) // fat header
	{
		// First word contains flags
		m_wMethodFlags = (*((WORD*)&pMethodData[0]) >> 2);
		// Max stack follows
		m_wMaxStack = *((WORD*)&pMethodData[2]);
		// Code size follows
		m_dwCodeSize = *((DWORD*)&pMethodData[4]);
		// Local variable token is last
		m_dwLocalVarToken = *((DWORD*)&pMethodData[8]);

		pILPtr = pMethodData + 12;
		dwILOffset = 12;
	}
	else
	{
		// Invalid header type
		return false;
	}

	if (m_dwLocalVarToken != 0)
	{
		// Parse the local variables signature
		DWORD dwSigBlobOffset = pFile->getMetadata().getTable(CLR_TABLE_STANDALONESIG)
			[CLR_TOKEN_RID(m_dwLocalVarToken) - 1][CLR_STANDALONESIG_SIGNATURE_COLINDEX];
		const CLRBlobHeapEntry* pEntry = pFile->getMetadata().getBlob(dwSigBlobOffset);
		if (pEntry)
			m_sLocalVars.parse(&pFile->getMetadata(), pEntry->pbData);
	}

	// Copy the IL instructions into a new buffer
	m_pbILCode = new BYTE[m_dwCodeSize];
	memcpy(m_pbILCode, pILPtr, m_dwCodeSize);

	m_vExceptions.Clear();
	if (m_wMethodFlags & 2)
	{
		// Exception handling section follows IL code at the next 4-byte boundary
		DWORD dwExceptionOffset = dwILOffset + m_dwCodeSize;
		dwExceptionOffset = (dwExceptionOffset + 3) & (~3);
		const BYTE* pExceptionData = pMethodData + dwExceptionOffset;

		if (pExceptionData[0] & 0x40)
		{
			// Fat format
			DWORD dwHeaderSize = (*((DWORD*)&pExceptionData[1])) & 0xffffff; // only 3 bytes of size
			DWORD dwClauseCount = dwHeaderSize / 24; // 24 bytes per clause

			pExceptionData += 4; // Go past header
			for (DWORD i=0; i<dwClauseCount; i++)
			{
				CLRExceptionClause ecClause;
				ecClause.dwFlags = *((DWORD*)&pExceptionData[0]);
				ecClause.dwTryOffset = *((DWORD*)&pExceptionData[4]);
				ecClause.dwTryLen = *((DWORD*)&pExceptionData[8]);
				ecClause.dwHandlerOffset = *((DWORD*)&pExceptionData[12]);
				ecClause.dwHandlerLen = *((DWORD*)&pExceptionData[16]);
				ecClause.dwFilter = *((DWORD*)&pExceptionData[20]);
				m_vExceptions.Add(ecClause);
				pExceptionData += 24; // Go to next clause
			}
		}
		else
		{
			// Tiny format
			DWORD dwHeaderSize = pExceptionData[1];
			DWORD dwClauseCount = dwHeaderSize / 12; // 12 bytes per clause

			pExceptionData += 4; // Go past header
			for (DWORD i=0; i<dwClauseCount; i++)
			{
				CLRExceptionClause ecClause;
				ecClause.dwFlags = *((WORD*)&pExceptionData[0]);
				ecClause.dwTryOffset = *((WORD*)&pExceptionData[2]);
				ecClause.dwTryLen = pExceptionData[4];
				ecClause.dwHandlerOffset = *((WORD*)&pExceptionData[5]);
				ecClause.dwHandlerLen = pExceptionData[7];
				ecClause.dwFilter = *((DWORD*)&pExceptionData[8]);
				m_vExceptions.Add(ecClause);
				pExceptionData += 12; // Go to next clause
			}
		}
	}

	return true;
}


//*************************************************************************
// Method:	 CLRMethod::save
// Description: Writes the possibly modified method to a PE section.
// Parameters:
//	pSection - PE section to write to
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMethod::save(PEFile* pFile, PESection* pSection)
{
	if ((m_dwLocalVarToken == 0) && (m_sLocalVars.getParamCount() > 0))
	{
		// Local variables have changed, save new signature
		m_sLocalVars.save(&pFile->getMetadata());
		CLRTableRow trRow(&pFile->getMetadata().getTable(CLR_TABLE_STANDALONESIG));
		trRow.setBlob(CLR_STANDALONESIG_SIGNATURE_COLINDEX, m_sLocalVars.getData(), m_sLocalVars.getSize());
		m_dwLocalVarToken = (CLR_TABLE_STANDALONESIG << 24) + (pFile->getMetadata().
			getTable(CLR_TABLE_STANDALONESIG).add(trRow) + 1);
	}

	// Write out method header
	DWORD dwHeaderSize;
	if ((m_dwCodeSize < 64) && (m_wMethodFlags == 0) && (m_wMaxStack == 8) && (m_dwLocalVarToken == 0) &&
		(m_vExceptions.GetLength() != 0))
	{
		// Tiny header is possible.  Write out a tiny header
		BYTE bTinyHeader = (BYTE)(2 | (m_dwCodeSize << 2)); // Lower two bits = 2 makes it a tiny header
		if (!pSection->appendData(&bTinyHeader, 1))
			return false;
		dwHeaderSize = 1;
	}
	else
	{
		// Fat header required.  Calculate required method flags.  Lower
		// two bits = 3 makes it a fat header.  Upper 4 bits = size of
		// header in double words (3 in this case).
		if (m_vExceptions.GetLength() > 0)
			m_wMethodFlags |= 2; // Set exception handler flag if there are exceptions
		WORD wMethodFlags = 0x3003 | (m_wMethodFlags << 2);
		if (!pSection->appendData(&wMethodFlags, 2))
			return false;
		if (!pSection->appendData(&m_wMaxStack, 2))
			return false;
		if (!pSection->appendData(&m_dwCodeSize, 4))
			return false;
		if (!pSection->appendData(&m_dwLocalVarToken, 4))
			return false;
		dwHeaderSize = 12;
	}

	// Done writing header, write the IL code
	if (!pSection->appendData(m_pbILCode, m_dwCodeSize))
		return false;

	if (m_vExceptions.GetLength() > 0)
	{
		// Exception tables must start on a 4-byte boundary, calculate how much padding we need
		DWORD dwMethodILSize = dwHeaderSize + m_dwCodeSize;
		DWORD dwPadding = ((dwMethodILSize + 3) & (~3)) - dwMethodILSize;
		if (dwPadding > 0)
		{
			DWORD dwZero = 0;
			if (!pSection->appendData(&dwZero, dwPadding))
				return false;
		}

		bool bFatExceptionHeader = false;
		// See if we need a fat exception header
		for (DWORD i=0; i<m_vExceptions.GetLength(); i++)
		{
			if (m_vExceptions[i].dwFlags > 0xffff)
				bFatExceptionHeader = true;
			if (m_vExceptions[i].dwTryOffset > 0xffff)
				bFatExceptionHeader = true;
			if (m_vExceptions[i].dwTryLen > 0xff)
				bFatExceptionHeader = true;
			if (m_vExceptions[i].dwHandlerOffset > 0xffff)
				bFatExceptionHeader = true;
			if (m_vExceptions[i].dwHandlerLen > 0xff)
				bFatExceptionHeader = true;
		}
		// Check to make sure the total data size can fit in the tiny header size field
		if ((m_vExceptions.GetLength() * 12) > 0xff)
			bFatExceptionHeader = true;

		if (bFatExceptionHeader)
		{
			// We need fat exception header and clauses
			BYTE bKind = 0x41; // Flags for fat exception handler table
			DWORD dwDataSize = m_vExceptions.GetLength() * 24;
			if (!pSection->appendData(&bKind, 1))
				return false;
			if (!pSection->appendData(&dwDataSize, 3))
				return false;
			// Write out the exception clauses
			for (DWORD i=0; i<m_vExceptions.GetLength(); i++)
			{
				if (!pSection->appendData(&m_vExceptions[i].dwFlags, 4))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwTryOffset, 4))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwTryLen, 4))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwHandlerOffset, 4))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwHandlerLen, 4))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwFilter, 4))
					return false;
			}
		}
		else
		{
			// We can use tiny exception header and clauses
			BYTE bKind = 0x1; // Flags for tiny exception handler table
			BYTE bDataSize = (BYTE)(m_vExceptions.GetLength() * 12);
			WORD wReserved = 0;
			if (!pSection->appendData(&bKind, 1))
				return false;
			if (!pSection->appendData(&bDataSize, 1))
				return false;
			if (!pSection->appendData(&wReserved, 2))
				return false;
			// Write out the exception clauses
			for (DWORD i=0; i<m_vExceptions.GetLength(); i++)
			{
				if (!pSection->appendData(&m_vExceptions[i].dwFlags, 2))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwTryOffset, 2))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwTryLen, 1))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwHandlerOffset, 2))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwHandlerLen, 1))
					return false;
				if (!pSection->appendData(&m_vExceptions[i].dwFilter, 4))
					return false;
			}
		}
	}

	return true;
}


//*************************************************************************
// Method:	 CLRMethod::insertILAtStart
// Description: Inserts a block of IL code at the start of a method
// Parameters:
//	pbIL - Pointer to IL code to add
//  dwSize - Size of IL code to add
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMethod::insertILAtStart(const BYTE* pbIL, DWORD dwSize)
{
	// Create a new buffer to hold the IL code with the new instructions inserted
	BYTE* pbNewIL = new BYTE[m_dwCodeSize + dwSize];
	if (!pbNewIL)
		return false;

	// Fill in the buffer with the new code first and the old code following
	memcpy(pbNewIL, pbIL, dwSize);
	memcpy(pbNewIL + dwSize, m_pbILCode, m_dwCodeSize);
	m_dwCodeSize += dwSize;

	// Delete the old buffer and replace it with the new
	delete[] m_pbILCode;
	m_pbILCode = pbNewIL;

	// Update the offsets of the exception handling clauses
	for (DWORD i=0; i<m_vExceptions.GetLength(); i++)
	{
		m_vExceptions[i].dwTryOffset += dwSize;
		m_vExceptions[i].dwHandlerOffset += dwSize;

		if ((m_vExceptions[i].dwFilter & 0xff000000) == 0)
		{
			// The upper byte of the filter will be nonzero if it is a coded token.  If the upper
			// byte is zero, the filter is an offset into the method IL, so update the offset.
			m_vExceptions[i].dwFilter += dwSize;
		}
	}
	return true;
}


//*************************************************************************
// Method:	 CLRMethod::insertILAtEnd
// Description: Inserts a block of IL code at the end of a method
// Parameters:
//	pbIL - Pointer to IL code to add
//  dwSize - Size of IL code to add
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMethod::insertILAtEnd(const BYTE* pbIL, DWORD dwSize)
{
	// Create a new buffer to hold the IL code with the new instructions inserted
	BYTE* pbNewIL = new BYTE[m_dwCodeSize + dwSize];
	if (!pbNewIL)
		return false;

	// Fill in the buffer with the old code first and the new code following
	memcpy(pbNewIL, m_pbILCode, m_dwCodeSize);
	memcpy(pbNewIL + m_dwCodeSize, pbIL, dwSize);
	m_dwCodeSize += dwSize;

	// Delete the old buffer and replace it with the new
	delete[] m_pbILCode;
	m_pbILCode = pbNewIL;
	return true;
}


//*************************************************************************
// Method:	 CLRMethod::replaceTokensInIL
// Description: Replaces all occurences of a token in the IL stream
// Parameters:
//	table - Translation table to use
//
// Return Value: true on success, false on failure
//*************************************************************************
bool CLRMethod::replaceTokensInIL(CLRTranslationTable& table)
{
	m_dwLocalVarToken = table.Translate(m_dwLocalVarToken);

	for (DWORD i=0; i<m_vExceptions.GetLength(); i++)
	{
		if (m_vExceptions[i].dwFlags == 0)
			m_vExceptions[i].dwFilter = table.Translate(m_vExceptions[i].dwFilter);
	}

	return CLRReplaceTokensInIL(m_pbILCode, m_dwCodeSize, table);
}


//*************************************************************************
// Method:	 CLRMethod::getILCode
// Description: Returns the IL code for this method
// Parameters:
//	None
//
// Return Value: Pointer to the IL code
//*************************************************************************
const BYTE* CLRMethod::getILCode()
{
	return m_pbILCode;
}


//*************************************************************************
// Method:	 CLRMethod::getILCodeSize
// Description: Returns the size of the IL code for this method
// Parameters:
//	None
//
// Return Value: Size in bytes of the IL code
//*************************************************************************
DWORD CLRMethod::getILCodeSize()
{
	return m_dwCodeSize;
}


//*************************************************************************
// Method:	 CLRMethod::setSignature
// Description: Sets the signature of this method.
// Parameters:
//	sSig - Signature of the method
//
// Return Value: None
//*************************************************************************
void CLRMethod::setSignature(const CLRSignature& sSig)
{
	m_sSig = sSig;
}


//*************************************************************************
// Method:	 CLRMethod::getSignature
// Description: Returns the signature of this method
// Parameters:
//	None
//
// Return Value: Signature of the method
//*************************************************************************
const CLRSignature& CLRMethod::getSignature()
{
	return m_sSig;
}


//*************************************************************************
// Method:	 CLRMethod::getTypeIndex
// Description: Returns the type that contains this method
// Parameters:
//	None
//
// Return Value: TypeDef row index of the type containing the method
//*************************************************************************
DWORD CLRMethod::getTypeIndex()
{
	return m_dwTypeIndex;
}


//*************************************************************************
// Method:	 CLRMethod::getMethodIndex
// Description: Returns the method index
// Parameters:
//	None
//
// Return Value: Method row index of the method
//*************************************************************************
DWORD CLRMethod::getMethodIndex()
{
	return m_dwMethodIndex;
}


//*************************************************************************
// Method:	 CLRMethod::getParamCount
// Description: Returns the number of parameters that the method takes,
//				including the implied "this" parameter if it is present
// Parameters:
//	None
//
// Return Value: Number of parameters
//*************************************************************************
DWORD CLRMethod::getParamCount()
{
	return m_sSig.getParamCount();
}


//*************************************************************************
// Method:	 CLRMethod::getMaxStack
// Description: Returns the maximum stack depth of the method
// Parameters:
//	None
//
// Return Value: Maximum stack depth
//*************************************************************************
DWORD CLRMethod::getMaxStack()
{
	return m_wMaxStack;
}


//*************************************************************************
// Method:	 CLRMethod::setMaxStack
// Description: Sets the maximum stack depth for the method
// Parameters:
//	dwMaxStack - Maximum stack depth
//
// Return Value: None
//*************************************************************************
void CLRMethod::setMaxStack(DWORD dwMaxStack)
{
	m_wMaxStack = (WORD)dwMaxStack;
}


//*************************************************************************
// Method:	 CLRMethod::setTypeIndex
// Description: Sets the type containing this method
// Parameters:
//	dwTypeIndex - TypeDef row index of the type containing this method
//
// Return Value: None
//*************************************************************************
void CLRMethod::setTypeIndex(DWORD dwTypeIndex)
{
	m_dwTypeIndex = dwTypeIndex;
}


//*************************************************************************
// Method:	 CLRMethod::setMethodIndex
// Description: Sets the method index for this method
// Parameters:
//	dwMethodIndex - Method row index for this method
//
// Return Value: None
//*************************************************************************
void CLRMethod::setMethodIndex(DWORD dwMethodIndex)
{
	m_dwMethodIndex = dwMethodIndex;
}


//*************************************************************************
// Method:	 CLRMethod::getLocalVars
// Description: Gets the local variables signature
// Parameters:
//	None
//
// Return Value: Local variables signature
//*************************************************************************
CLRSignature& CLRMethod::getLocalVars()
{
	m_dwLocalVarToken = 0; // Force rewrite of local var information
	return m_sLocalVars;
}
