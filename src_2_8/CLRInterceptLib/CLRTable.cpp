//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		CLRTable.cpp
//
// DESCRIPTION: Contains classes to manage tables in the CLR metadata
//
//				See "Inside Microsoft .NET IL Assembler" by Serge Lidin
//				for details on the file structure of .NET executables.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 31 Mar 2003	103	 R. Wagner	 File created.
//*************************************************************************

#include <windows.h>
#include "CLRTable.h"
#include "CLRMetadata.h"

using namespace SiUtils;


//*************************************************************************
// Metadata table schema definitions
// See "Inside Microsoft .NET IL Assembler" by Serge Lidin
// for details on metadata tables.
//*************************************************************************

char* g_pszTableNames[CLR_METADATA_TABLE_COUNT] = {"Module", "TypeRef", "TypeDef",
	"FieldPtr", "Field", "MethodPtr", "Method", "ParamPtr", "Param", "InterfaceImpl",
	"MemberRef", "Constant", "CustomAttribute", "FieldMarshal", "DeclSecurity",
	"ClassLayout", "FieldLayout", "StandAlongSig", "EventMap", "EventPtr", "Event",
	"PropertyMap", "PropertyPtr", "Property", "MethodSemantics", "MethodImpl",
	"ModuleRef", "TypeSpec", "ImplMap", "FieldRVA", "ENCLog", "ENCMap", "Assembly",
	"AssemblyProcessor", "AssemblyOS", "AssemblyRef", "AssemblyRefProcessor",
	"AssemblyRefOS", "File", "ExportedType", "ManifestResource", "NestedClass", "GenericParam", "MethodSpec",
	"GenericParamConstraint", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", ""};

// Coded token types
static CLRCodedTokenTableList s_cttTypeDefOrRefTableList = {3, 2, {CLR_TABLE_TYPEDEF, CLR_TABLE_TYPEREF, CLR_TABLE_TYPESPEC}};
static CLRCodedTokenTableList s_cttHasConstantTableList = {3, 2, {CLR_TABLE_FIELD, CLR_TABLE_PARAM, CLR_TABLE_PROPERTY}};
static CLRCodedTokenTableList s_cttHasCustomAttributeTableList = {19, 5, {CLR_TABLE_METHOD, CLR_TABLE_FIELD, CLR_TABLE_TYPEREF,
	CLR_TABLE_TYPEDEF, CLR_TABLE_PARAM, CLR_TABLE_INTERFACEIMPL, CLR_TABLE_MEMBERREF, CLR_TABLE_MODULE, CLR_TABLE_DECLSECURITY,
	CLR_TABLE_PROPERTY, CLR_TABLE_EVENT, CLR_TABLE_STANDALONESIG, CLR_TABLE_MODULEREF, CLR_TABLE_TYPESPEC, CLR_TABLE_ASSEMBLY,
	CLR_TABLE_ASSEMBLYREF, CLR_TABLE_FILE, CLR_TABLE_EXPORTEDTYPE, CLR_TABLE_MANIFESTRESOURCE}};
static CLRCodedTokenTableList s_cttHasFieldMarshalTableList = {2, 1, {CLR_TABLE_FIELD, CLR_TABLE_PARAM}};
static CLRCodedTokenTableList s_cttHasDeclSecurityTableList = {3, 2, {CLR_TABLE_TYPEDEF, CLR_TABLE_METHOD, CLR_TABLE_ASSEMBLY}};
static CLRCodedTokenTableList s_cttMemberRefParentTableList = {5, 3, {CLR_TABLE_TYPEDEF, CLR_TABLE_TYPEREF, CLR_TABLE_MODULEREF,
	CLR_TABLE_METHOD, CLR_TABLE_TYPESPEC}};
static CLRCodedTokenTableList s_cttHasSemanticsTableList = {2, 1, {CLR_TABLE_EVENT, CLR_TABLE_PROPERTY}};
static CLRCodedTokenTableList s_cttMethodDefOrRefTableList = {2, 1, {CLR_TABLE_METHOD, CLR_TABLE_MEMBERREF}};
static CLRCodedTokenTableList s_cttMemberForwardedTableList = {2, 1, {CLR_TABLE_FIELD, CLR_TABLE_METHOD}};
static CLRCodedTokenTableList s_cttImplementationTableList = {3, 2, {CLR_TABLE_FILE, CLR_TABLE_ASSEMBLYREF, CLR_TABLE_EXPORTEDTYPE}};
static CLRCodedTokenTableList s_cttCustomAttributeTypeTableList = {5, 3, {CLR_TABLE_TYPEREF, CLR_TABLE_TYPEDEF, CLR_TABLE_METHOD,
	CLR_TABLE_MEMBERREF, CLR_COL_STRING}};
static CLRCodedTokenTableList s_cttResolutionScopeTableList = {4, 2, {CLR_TABLE_MODULE, CLR_TABLE_MODULEREF,
	CLR_TABLE_ASSEMBLYREF, CLR_TABLE_TYPEREF}};
static CLRCodedTokenTableList s_cttTypeOrMethodDefTableList = {2, 1, {CLR_TABLE_TYPEDEF, CLR_TABLE_METHOD}};

// Array of all coded token types, with indicies equal to those in the actual coded token.
CLRCodedTokenTableList* g_pcttCodedTokenTables[] = {&s_cttTypeDefOrRefTableList, &s_cttHasConstantTableList,
	&s_cttHasCustomAttributeTableList, &s_cttHasFieldMarshalTableList, &s_cttHasDeclSecurityTableList,
	&s_cttMemberRefParentTableList, &s_cttHasSemanticsTableList, &s_cttMethodDefOrRefTableList,
	&s_cttMemberForwardedTableList, &s_cttImplementationTableList, &s_cttCustomAttributeTypeTableList,
	&s_cttResolutionScopeTableList, &s_cttTypeOrMethodDefTableList};

// Table column definitions
static CLRTableColDef s_tcModuleCols[5] = {{CLR_COL_USHORT}, {CLR_COL_STRING}, {CLR_COL_GUID}, {CLR_COL_GUID},
	{CLR_COL_GUID}};
static CLRTableColDef s_tcTypeRefCols[3] = {{CLR_COL_TOKEN_RESOLUTIONSCOPE}, {CLR_COL_STRING}, {CLR_COL_STRING}};
static CLRTableColDef s_tcTypeDefCols[6] = {{CLR_COL_ULONG}, {CLR_COL_STRING}, {CLR_COL_STRING},
	{CLR_COL_TOKEN_TYPEDEFORREF}, {CLR_TABLE_FIELD}, {CLR_TABLE_METHOD}};
static CLRTableColDef s_tcFieldPtrCols[1] = {{CLR_TABLE_FIELD}};
static CLRTableColDef s_tcFieldCols[3] = {{CLR_COL_USHORT}, {CLR_COL_STRING}, {CLR_COL_SIGNATURE}};
static CLRTableColDef s_tcMethodPtrCols[1] = {{CLR_TABLE_METHOD}};
static CLRTableColDef s_tcMethodCols[6] = {{CLR_COL_ULONG}, {CLR_COL_USHORT}, {CLR_COL_USHORT}, {CLR_COL_STRING},
	{CLR_COL_SIGNATURE}, {CLR_TABLE_PARAM}};
static CLRTableColDef s_tcParamPtrCols[1] = {{CLR_TABLE_PARAM}};
static CLRTableColDef s_tcParamCols[3] = {{CLR_COL_USHORT}, {CLR_COL_USHORT}, {CLR_COL_STRING}};
static CLRTableColDef s_tcInterfaceImplCols[2] = {{CLR_TABLE_TYPEDEF}, {CLR_COL_TOKEN_TYPEDEFORREF}};
static CLRTableColDef s_tcMemberRefCols[3] = {{CLR_COL_TOKEN_MEMBERREFPARENT}, {CLR_COL_STRING}, {CLR_COL_SIGNATURE}};
static CLRTableColDef s_tcConstantCols[3] = {{CLR_COL_BYTE}, {CLR_COL_TOKEN_HASCONSTANT}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcCustomAttributeCols[3] = {{CLR_COL_TOKEN_HASCUSTOMATTRIBUTE},
	{CLR_COL_TOKEN_CUSTOMATTRIBUTETYPE}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcFieldMarshalCols[2] = {{CLR_COL_TOKEN_HASFIELDMARSHAL}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcDeclSecurityCols[3] = {{CLR_COL_SHORT}, {CLR_COL_TOKEN_HASDECLSECURITY}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcClassLayoutCols[3] = {{CLR_COL_USHORT}, {CLR_COL_ULONG}, {CLR_TABLE_TYPEDEF}};
static CLRTableColDef s_tcFieldLayoutCols[2] = {{CLR_COL_ULONG}, {CLR_TABLE_FIELD}};
static CLRTableColDef s_tcStandAloneSigCols[1] = {{CLR_COL_SIGNATURE}};
static CLRTableColDef s_tcEventMapCols[2] = {{CLR_TABLE_TYPEDEF}, {CLR_TABLE_EVENT}};
static CLRTableColDef s_tcEventPtrCols[1] = {{CLR_TABLE_EVENT}};
static CLRTableColDef s_tcEventCols[3] = {{CLR_COL_USHORT}, {CLR_COL_STRING}, {CLR_COL_TOKEN_TYPEDEFORREF}};
static CLRTableColDef s_tcPropertyMapCols[2] = {{CLR_TABLE_TYPEDEF}, {CLR_TABLE_PROPERTY}};
static CLRTableColDef s_tcPropertyPtrCols[1] = {{CLR_TABLE_PROPERTY}};
static CLRTableColDef s_tcPropertyCols[3] = {{CLR_COL_USHORT}, {CLR_COL_STRING}, {CLR_COL_SIGNATURE}};
static CLRTableColDef s_tcMethodSemanticsCols[3] = {{CLR_COL_USHORT}, {CLR_TABLE_METHOD},
	{CLR_COL_TOKEN_HASSEMANTICS}};
static CLRTableColDef s_tcMethodImplCols[3] = {{CLR_TABLE_TYPEDEF}, {CLR_COL_TOKEN_METHODDEFORREF},
	{CLR_COL_TOKEN_METHODDEFORREF}};
static CLRTableColDef s_tcModuleRefCols[1] = {{CLR_COL_STRING}};
static CLRTableColDef s_tcTypeSpecCols[1] = {{CLR_COL_SIGNATURE_NOCALLCONV}};
static CLRTableColDef s_tcENCLogCols[2] = {{CLR_COL_TOKEN}, {CLR_COL_ULONG}};
static CLRTableColDef s_tcImplMapCols[4] = {{CLR_COL_USHORT}, {CLR_COL_TOKEN_MEMBERFORWARDED}, {CLR_COL_STRING},
	{CLR_TABLE_MODULEREF}};
static CLRTableColDef s_tcENCMapCols[1] = {{CLR_COL_TOKEN}};
static CLRTableColDef s_tcFieldRVACols[2] = {{CLR_COL_ULONG}, {CLR_TABLE_FIELD}};
static CLRTableColDef s_tcAssemblyCols[9] = {{CLR_COL_ULONG}, {CLR_COL_USHORT}, {CLR_COL_USHORT}, {CLR_COL_USHORT},
	{CLR_COL_USHORT}, {CLR_COL_ULONG}, {CLR_COL_BLOB}, {CLR_COL_STRING}, {CLR_COL_STRING}};
static CLRTableColDef s_tcAssemblyProcessorCols[1] = {{CLR_COL_ULONG}};
static CLRTableColDef s_tcAssemblyOSCols[3] = {{CLR_COL_ULONG}, {CLR_COL_ULONG}, {CLR_COL_ULONG}};
static CLRTableColDef s_tcAssemblyRefCols[9] = {{CLR_COL_USHORT}, {CLR_COL_USHORT}, {CLR_COL_USHORT},
	{CLR_COL_USHORT}, {CLR_COL_ULONG}, {CLR_COL_BLOB}, {CLR_COL_STRING}, {CLR_COL_STRING}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcAssemblyRefProcessorCols[2] = {{CLR_COL_ULONG}, {CLR_TABLE_ASSEMBLYREF}};
static CLRTableColDef s_tcAssemblyRefOSCols[4] = {{CLR_COL_ULONG}, {CLR_COL_ULONG}, {CLR_COL_ULONG},
	{CLR_TABLE_ASSEMBLYREF}};
static CLRTableColDef s_tcFileCols[3] = {{CLR_COL_ULONG}, {CLR_COL_STRING}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcExportedTypeCols[5] = {{CLR_COL_ULONG}, {CLR_COL_TOKEN}, {CLR_COL_STRING},
	{CLR_COL_STRING}, {CLR_COL_TOKEN_IMPLEMENTATION}};
static CLRTableColDef s_tcManifestResourceCols[4] = {{CLR_COL_ULONG}, {CLR_COL_ULONG}, {CLR_COL_STRING},
	{CLR_COL_TOKEN_IMPLEMENTATION}};
static CLRTableColDef s_tcNestedClassCols[2] = {{CLR_TABLE_TYPEDEF}, {CLR_TABLE_TYPEDEF}};
static CLRTableColDef s_tcGenericParamCols[6] = {{CLR_COL_USHORT}, {CLR_COL_USHORT}, {CLR_COL_TOKEN_TYPEORMETHODDEF},
	{CLR_COL_STRING}, {CLR_COL_TOKEN_TYPEDEFORREF}, {CLR_COL_TOKEN_TYPEDEFORREF}};
static CLRTableColDef s_tcMethodSpecCols[2] = {{CLR_COL_TOKEN_METHODDEFORREF}, {CLR_COL_BLOB}};
static CLRTableColDef s_tcGenericParamConstraint[2] = {{CLR_TABLE_GENERICPARAM}, {CLR_COL_TOKEN_TYPEDEFORREF}};

// Array of table definitions, with indicies equal to those used in the actual file.
CLRTableDef g_ptdTableFormat[CLR_METADATA_TABLE_COUNT] = {{s_tcModuleCols, 5, -1}, {s_tcTypeRefCols, 3, -1}, {s_tcTypeDefCols, 6, -1},
	{s_tcFieldPtrCols, 1, -1}, {s_tcFieldCols, 3, -1}, {s_tcMethodPtrCols, 1, -1}, {s_tcMethodCols, 6, -1},
	{s_tcParamPtrCols, 1, -1}, {s_tcParamCols, 3, -1}, {s_tcInterfaceImplCols, 2, 0}, {s_tcMemberRefCols, 3, -1},
	{s_tcConstantCols, 3, 1}, {s_tcCustomAttributeCols, 3, 0}, {s_tcFieldMarshalCols, 2, 0},
	{s_tcDeclSecurityCols, 3, 1}, {s_tcClassLayoutCols, 3, 2}, {s_tcFieldLayoutCols, 2, 1},
	{s_tcStandAloneSigCols, 1, -1}, {s_tcEventMapCols, 2, -1}, {s_tcEventPtrCols, 1, -1}, {s_tcEventCols, 3, -1},
	{s_tcPropertyMapCols, 2, -1}, {s_tcPropertyPtrCols, 1, -1}, {s_tcPropertyCols, 3, -1},
	{s_tcMethodSemanticsCols, 3, 1}, {s_tcMethodImplCols, 3, 0}, {s_tcModuleRefCols, 1, -1},
	{s_tcTypeSpecCols, 1, -1}, {s_tcImplMapCols, 4, 1}, {s_tcFieldRVACols, 2, 1}, {s_tcENCLogCols, 2, -1},
	{s_tcENCMapCols, 1, -1}, {s_tcAssemblyCols, 9, -1}, {s_tcAssemblyProcessorCols, 1, -1},
	{s_tcAssemblyOSCols, 3, -1}, {s_tcAssemblyRefCols, 9, -1}, {s_tcAssemblyRefProcessorCols, 2, -1},
	{s_tcAssemblyRefOSCols, 4, -1}, {s_tcFileCols, 3, -1}, {s_tcExportedTypeCols, 5, -1},
	{s_tcManifestResourceCols, 4, -1}, {s_tcNestedClassCols, 2, 0}, {s_tcGenericParamCols, 4, 2},
	{s_tcMethodSpecCols, 2, -1}, {s_tcGenericParamConstraint, 2, 0}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1},
	{NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1},
	{NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1},
	{NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}, {NULL, 0, -1}};


//*************************************************************************
// Method:	 CLRTable::CLRTable
// Description: Default constructor for the CLRTable type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRTable::CLRTable()
{
	m_mdMetadata = NULL;
	m_ptcCols = NULL;
}


//*************************************************************************
// Method:	 CLRTable::~CLRTable
// Description: Destructor for the CLRTable type.
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
CLRTable::~CLRTable()
{
	if (m_ptcCols == NULL)
	{
		delete[] m_ptcCols;
		m_ptcCols = NULL;
	}
}


//*************************************************************************
// Method:	 CLRTable::operator[]
// Description: Returns the table row at the given index
// Parameters:
//	index - Row index to retrieve
//
// Return Value: CLRTableRow object representing the requested table row
//*************************************************************************
CLRTableRow& CLRTable::operator[](const int index)
{
	return m_vtrRows[index];
}


//*************************************************************************
// Method:	 CLRTable::add
// Description: Adds a new table row at the end of a table
// Parameters:
//	trRow - Table row to add
//
// Return Value: Index of the new row
//*************************************************************************
DWORD CLRTable::add(CLRTableRow &trRow)
{
	DWORD dwIndex = (DWORD)m_vtrRows.GetLength();
	m_vtrRows.Add(trRow);
	return dwIndex;
}


//*************************************************************************
// Method:	 CLRTable::remove
// Description: Removes a table row from the table
// Parameters:
//	index - Table row to remove
//
// Return Value: None
//*************************************************************************
void CLRTable::remove(DWORD index)
{
	m_vtrRows.Remove(index);
}


//*************************************************************************
// Method:	 CLRTable::sizeHint
// Description: Preallocates enough space to hold the given number of rows
// Parameters:
//	dwSize - Number of rows to preallocate for
//
// Return Value: None
//*************************************************************************
void CLRTable::sizeHint(const DWORD dwSize)
{
	m_vtrRows.EnsureSize(dwSize);
}


//*************************************************************************
// Method:	 CLRTable::getRowCount
// Description: Returns the current number of rows in the table
// Parameters:
//	None
//
// Return Value: Number of rows in the table
//*************************************************************************
DWORD CLRTable::getRowCount()
{
	return (DWORD)m_vtrRows.GetLength();
}


//*************************************************************************
// Method:	 CLRTable::getMetadata
// Description: Returns the metadata that contains this table
// Parameters:
//	None
//
// Return Value: Pointer to the metadata containing this table
//*************************************************************************
CLRMetadata* CLRTable::getMetadata()
{
	return m_mdMetadata;
}


//*************************************************************************
// Method:	 CLRTable::setMetadata
// Description: Sets the metadata that contains this table
// Parameters:
//	mdMetadata - Pointer to metadata that contains this table
//
// Return Value: None
//*************************************************************************
void CLRTable::setMetadata(CLRMetadata* mdMetadata)
{
	m_mdMetadata = mdMetadata;
}


//*************************************************************************
// Method:	 CLRTable::getColDefs
// Description: Returns the column definitions for the columns in this table
// Parameters:
//	None
//
// Return Value: Array of column definitions
//*************************************************************************
CLRTableColDef* CLRTable::getColDefs()
{
	return m_ptcCols;
}


//*************************************************************************
// Method:	 CLRTable::getColCount
// Description: Returns the number of columns for this table
// Parameters:
//	None
//
// Return Value: Number of columns
//*************************************************************************
DWORD CLRTable::getColCount()
{
	return m_dwColCount;
}


//*************************************************************************
// Method:	 CLRTable::setColDefs
// Description: Sets the column definitions for this table
// Parameters:
//	ptcCols - Array of column definitions
//  dwColCount - Number of columns
//  dwIndex - Index of the table
//
// Return Value: None
//*************************************************************************
void CLRTable::setColDefs(const CLRTableColDef* ptcCols, const DWORD dwColCount, DWORD dwIndex)
{
	if (m_ptcCols)
		delete[] m_ptcCols;
	m_ptcCols = new CLRTableColDef[dwColCount];

	memcpy(m_ptcCols, ptcCols, sizeof(CLRTableColDef) * dwColCount);
	m_dwColCount = dwColCount;

	m_dwIndex = dwIndex;
}


//*************************************************************************
// Method:	 CLRTable::printTable
// Description: Prints a metadata table to a file for debugging
// Parameters:
//	None
//
// Return Value: None
//*************************************************************************
void CLRTable::printTable(FILE* fp)
{
	fprintf(fp, "Table %s:\n", g_pszTableNames[m_dwIndex]);
	for (int i=0; i<(int)m_vtrRows.GetLength(); i++)
		m_vtrRows[i].printTableRow(fp, i);
	fprintf(fp, "\n");
}


//*************************************************************************
// Function:	TableRowCompareFunc
// Description: Compares two table rows for sorting
// Parameters:
//	a - First row
//  b - Second row
//
// Return Value: If a is greater than b, > 0
//				 If a is less than b, < 0
//				 If a is equal to b, = 0
//*************************************************************************
int __cdecl TableRowCompareFunc(const void* a, const void* b)
{
	CLRTableRow* aRow = (CLRTableRow*)a;
	CLRTableRow* bRow = (CLRTableRow*)b;
	int sortCol = g_ptdTableFormat[aRow->getTable().getIndex()].bKeyIndex;
	return (*aRow)[sortCol] - (*bRow)[sortCol];
}


//*************************************************************************
// Method:	 CLRTable::sort
// Description: Sorts the table if there is a valid sorted column
// Parameters:
//	index - Table index
//
// Return Value: None
//*************************************************************************
void CLRTable::sort()
{
	if (g_ptdTableFormat[m_dwIndex].bKeyIndex == -1)
		return;
	qsort(&m_vtrRows[0], m_vtrRows.GetLength(), sizeof(CLRTableRow), TableRowCompareFunc);
}
