//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTypeSignature.h
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

#ifndef __CLRTYPESIGNATURE_H__
#define __CLRTYPESIGNATURE_H__

#include <windows.h>
#include "CLRTranslationTable.h"
#include "SiString.h"


struct CLRTypeName
{
	SiUtils::SiString sOuterClass;
	SiUtils::SiString sNamespace;
	SiUtils::SiString sName;
};


class CLRSignature;
class CLRMetadata;

class CLRTypeSignature
{
	BYTE m_bTypeCode;
	BYTE* m_pbAssocData;
	DWORD m_dwAssocDataSize;
	CLRTypeSignature* m_ptsAssocType;
	CLRSignature* m_psAssocMethodSig;
	CLRTypeName m_tnTypeName;
	bool m_bPrivateType;
	DWORD m_dwTypeToken;

	static DWORD readCompressedInt(const BYTE* pbTypeSig, DWORD& dwOffset);
	static void skipCompressedInt(const BYTE* pbTypeSig, DWORD& dwOffset);
	CLRTypeName readTypeToken(CLRMetadata* mdMetadata, const BYTE* pbTypeSig, DWORD& dwOffset);
	static bool writeByte(BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, BYTE bVal);
	static bool writeData(BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, const BYTE* pbData, DWORD dwDataSize);
	static bool writeCompressedInt(BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, DWORD dwValue);
	static bool writeTypeToken(CLRMetadata* mdMetadata, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen, const CLRTypeName& tnName);
	static void replaceTypeToken(const BYTE* pbTypeSig, DWORD& dwOffset, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen,
		CLRTranslationTable& table);

public:
	CLRTypeSignature();
	CLRTypeSignature(const CLRTypeSignature& tsType);
	CLRTypeSignature(BYTE bBasicTypeCode);
	CLRTypeSignature(BYTE bTypeCode, const CLRTypeName& tnName);
	CLRTypeSignature(BYTE bTypeCode, const CLRTypeSignature& tsType);
	~CLRTypeSignature();

	CLRTypeSignature& operator=(const CLRTypeSignature& tsType);

	DWORD parse(CLRMetadata* mdMetadata, const BYTE* pbTypeSig);
	bool save(CLRMetadata* mdMetadata, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen);
	static DWORD replaceTokens(const BYTE* pbOrigSig, BYTE*& pbSig, DWORD& dwLen, DWORD& dwMaxLen,
		CLRTranslationTable& table);

	bool operator==(const CLRTypeSignature& tsType);
	bool operator!=(const CLRTypeSignature& tsType);

	BYTE getTypeCode() const;
	CLRTypeSignature* getAssocType() const;
	CLRSignature* getAssocMethodSig() const;
	CLRTypeName getAssocTypeName() const;
	bool isPrivate() const;
	DWORD getTypeToken() const;
};


#endif
