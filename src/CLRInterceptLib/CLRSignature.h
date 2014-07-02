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

#ifndef __CLRSIGNATURE_H__
#define __CLRSIGNATURE_H__

#include <windows.h>
#include "CLRTypeSignature.h"
#include "CLRTranslationTable.h"
#include "SiArray.h"

#define SIGNATURE_DATA_REALLOC_SIZE 32


class CLRSignature
{
	BYTE* m_pbData;
	DWORD m_dwDataSize;
	DWORD m_dwDataMaxSize;

	BYTE m_bCallConv;
	CLRTypeSignature m_tsReturnType;
	DWORD m_dwGenericParamCount; // assumed meaning for CLR 1.2
	SiUtils::SiArray<CLRTypeSignature> m_vtsParamTypes;

	DWORD readCompressedInt(const BYTE* pbTypeSig, DWORD& dwOffset);

	bool writeByte(BYTE bVal);
	bool writeCompressedInt(DWORD dwValue);

public:
	CLRSignature();
	CLRSignature(const CLRSignature& sSig);
	~CLRSignature();

	bool operator==(const CLRSignature& sOtherSig);
	bool operator!=(const CLRSignature& sOtherSig);

	CLRSignature& operator=(const CLRSignature& sSig);

	DWORD parse(CLRMetadata* mdMetadata, const BYTE* pbSig);
	bool save(CLRMetadata* mdMetadata);
	CLRSignature convertToInstanceMethod() const;
	DWORD replaceTokens(const BYTE* pbSig, CLRTranslationTable& table);

	DWORD getParamCount() const;
	const BYTE* getData() const;
	DWORD getSize() const;
	BYTE getCallConv() const;
	const SiUtils::SiArray<CLRTypeSignature>& getParamTypes() const;
	CLRTypeSignature getReturnType() const { return m_tsReturnType; }

	void setCallConv(BYTE bCallConv);
	void removeFirstParam();
	void setReturnType(const CLRTypeSignature& tsTypeSig);
	void insertParamAtEnd(const CLRTypeSignature& tsTypeSig);
};


#endif
