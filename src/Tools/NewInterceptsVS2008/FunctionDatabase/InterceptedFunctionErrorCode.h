//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionErrorCode.h
//
// DESCRIPTION: Contains definition for the class to represent an error code
//	in an intercepted function
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 22 SEP 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>

namespace FunctionDatabase
{
	//*************************************************************************
	// Possible types of a function's error code in the functions.xml file
	//*************************************************************************
	public __value enum ErrorCodeType
	{
		Success = 0,
		Informational,
		Warning,
		Error
	};

	//*************************************************************************
	// Representation of a function's error code in the functions.xml file
	//*************************************************************************
	public __gc class InterceptedFunctionErrorCode
	{
	protected:
		ErrorCodeType type;
		unsigned int ecValue;
	public:
		InterceptedFunctionErrorCode();
		~InterceptedFunctionErrorCode();
		InterceptedFunctionErrorCode* GetCloneObject();

		__property ErrorCodeType get_Type() { return type; }
		__property void set_Type(ErrorCodeType value) { type = value; }

		__property unsigned int get_ECValue() { return ecValue; }
		__property void set_ECValue(unsigned int value) { ecValue = value; }

	};
}