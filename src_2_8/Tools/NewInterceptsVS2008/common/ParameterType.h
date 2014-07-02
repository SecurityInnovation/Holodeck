//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ParameterType.h
//
// DESCRIPTION: Contains definitions for the enumerations ParameterType and
//	ParameterOperatorType
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

//*************************************************************************
// Enumerates possible parameter types
//*************************************************************************
typedef enum
{
	NullType = 0,
	StringType,
	WideStringType,
	BooleanType,
	IntegerType,
	UnsignedLongType,
	RealType,
	PointerType,
	PointerPointerType,
	Integer64Type,
	OutStringType,
	OutWideStringType,
	IIDRefType,
	IgnoreType
} ParameterType;

//*************************************************************************
// Enumerates possible parameter operator types
//*************************************************************************
typedef enum
{
	EqualsType = 0,
	NotEqualToType,
	LessThanType,
	GreaterThanType,
	LessThanOrEqualType,
	GreaterThanOrEqualType,
	StartsWithType,
	EndsWithType,
	ContainsSubstringType,
	NotContainsSubstringType,
	ContainsBinaryType,
	NotContainsBinaryType,
	EqualsResourceType,
	NotEqualToResourceType,
	ContainsResourceType,
	NotContainsResourceType,
	StartsWithResourceType
} ParameterOperatorType;