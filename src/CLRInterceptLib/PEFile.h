//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		PEFile.h
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

#ifndef __PEFILE_H__
#define __PEFILE_H__

#include <windows.h>
#include <corhdr.h>
#include "PESection.h"
#include "CLRMetadata.h"
#include "CLRMethod.h"
#include "CLRSignature.h"
#include "SiArray.h"
#include "SiString.h"
#include "SiHashTable.h"


// CLRMemberDescription contains information about a member of a type
struct CLRMemberDescription
{
	CLRTypeName tnType;
	SiUtils::SiString sName;
	CLRSignature sSignature;
	DWORD dwFlags; // Ignored when calling find methods
	DWORD dwAssemblyRefToken; // Used only when creating a member reference
};


struct CLRNewMethodDescription
{
	CLRMemberDescription mdMember;
	DWORD dwImplFlags;
	CLRMethod mMethod;
};


// CLRNewTypeDescription is used when adding a new type to an executable
struct CLRNewTypeDescription
{
	CLRTypeName tnTypeName;
	CLRTypeName tnBaseName;
	DWORD dwFlags;
	SiUtils::SiArray<CLRNewMethodDescription> vmdMethods; // tnType field inside CLDMemberDescription not required
	SiUtils::SiArray<CLRMemberDescription> vmdFields; // tnType field inside CLDMemberDescription not required
};


class PEFile
{
	bool m_fManaged; // true if the executable is based on the .NET runtime

	IMAGE_DOS_HEADER m_mzDOSHeader;
	IMAGE_NT_HEADERS m_peNTHeader;
	BYTE* m_pbDOSData; // Data between DOS header and NT header
	DWORD m_dwDOSDataLen;

	SiUtils::SiArray<PESection*> m_vpSections; // Vector containing all sections in the executable

	IMAGE_COR20_HEADER* m_pchManagedHeader;
	CLRMetadata m_mdMetadata;

	PESection* m_pReplacedILSection; // Section containing all replaced IL code

	SiUtils::SiHashTable<int> m_htTypeDefHash;
	SiUtils::SiHashTable<int> m_htTypeRefHash;
	bool m_bNeedHashTableUpdate;

	void updateHashTables();

public:
	PEFile();
	~PEFile();

	bool load(const SiUtils::SiString& szFileName, bool fParseFile);
	bool save(const SiUtils::SiString& szFileName);

	bool isManagedExecutable();
	int  getDotNetVersion();
	CLRMetadata& getMetadata();
	bool saveMetadata();

	const BYTE* rvaToPointer(DWORD dwRVA);
	const BYTE* fileOffsetToPointer(DWORD dwFileOffset);

	const BYTE* getImportTable();
	const BYTE* getExportTable();

	void layoutSections();
	DWORD getFirstSectionFileOffset();

	CLRMethod* findMethod(const CLRMemberDescription& mdMember, bool localOnly);
	DWORD findMemberRefToken(const CLRMemberDescription& mdMember, bool fCreateIfNotFound);
	DWORD findTypeToken(const CLRTypeName& tnType);
	DWORD findAssemblyRef(const SiUtils::SiString& sAssemblyName);

	DWORD createUserStringToken(const SiUtils::SiString& sStr);
	bool replaceMethod(CLRMethod* pMethod);
	DWORD createAssemblyRef(PEFile& peRefBinary);
	DWORD createTypeRef(DWORD dwAssemblyRef, CLRTypeName& tnType);
	DWORD createType(CLRNewTypeDescription& ntdType);
	bool addNestedType(DWORD dwEnclosingToken, DWORD dwEnclosedToken);
	SiUtils::SiString getOuterClassName(DWORD dwEnclosedToken);
	SiUtils::SiString getFullTypeName(DWORD dwTypeToken);

	SiUtils::SiArray<CLRTypeName> enumerateTypes();
	SiUtils::SiArray<CLRMemberDescription> enumerateMethods(CLRTypeName& tnType);
	const CLRBlobHeapEntry* getAttributeValue(CLRTypeName& tnType, CLRTypeName& tnAttribute);

	SiUtils::SiArray<SiUtils::SiString> enumerateNativeFunctions();
	SiUtils::SiArray<SiUtils::SiString> enumerateImportedModules();
	SiUtils::SiArray<SiUtils::SiString> enumerateNativeImports(SiUtils::SiString sModuleName);

	DWORD getChecksum();
	IMAGE_COR20_HEADER* getManagedHeader() { return m_pchManagedHeader; }

	bool merge(PEFile& pFile);
};


#endif
