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

#ifndef __CLRMETHOD_H__
#define __CLRMETHOD_H__

#include <windows.h>
#include <corhdr.h>
#include "CLRSignature.h"
#include "CLRTranslationTable.h"
#include "SiArray.h"


#define CLR_METHOD_SEH_TABLE_PRESENT_FLAG 2
#define CLR_METHOD_LOCAL_VAR_INITIALIZE_FLAG 4


struct CLRExceptionClause
{
	DWORD dwFlags;
	DWORD dwTryOffset;
	DWORD dwTryLen;
	DWORD dwHandlerOffset;
	DWORD dwHandlerLen;
	DWORD dwFilter;
};


class PEFile;
class PESection;

class CLRMethod
{
	PEFile* m_pFile;
	DWORD m_dwTypeIndex, m_dwMethodIndex;

	WORD m_wMethodFlags;
	WORD m_wMaxStack;
	DWORD m_dwLocalVarToken;
	CLRSignature m_sLocalVars;

	BYTE* m_pbILCode;
	DWORD m_dwCodeSize;

	CLRSignature m_sSig;
	SiUtils::SiArray<CLRExceptionClause> m_vExceptions;

public:
	CLRMethod();
	CLRMethod(const CLRMethod& mMethod);
	~CLRMethod();

	CLRMethod& operator=(const CLRMethod& mMethod);

	bool parse(PEFile* pFile, DWORD dwTypeIndex, DWORD dwMethodIndex);
	bool parse(PEFile* pFile, DWORD dwRVA);
	bool save(PEFile* pFile, PESection* pSection);

	void setSignature(const CLRSignature& sSig);
	const CLRSignature& getSignature();

	bool insertILAtStart(const BYTE* pbIL, DWORD dwSize);
	bool insertILAtEnd(const BYTE* pbIL, DWORD dwSize);

	bool replaceTokensInIL(CLRTranslationTable& table);

	const BYTE* getILCode();
	DWORD getILCodeSize();

	DWORD getParamCount();
	DWORD getMaxStack();
	void setMaxStack(DWORD dwMaxStack);

	DWORD getTypeIndex();
	DWORD getMethodIndex();

	void setTypeIndex(DWORD dwTypeIndex);
	void setMethodIndex(DWORD dwMethodIndex);

	CLRSignature& getLocalVars();
};


#endif
