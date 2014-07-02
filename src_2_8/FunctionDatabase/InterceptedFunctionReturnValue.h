//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionReturnValue.h
//
// DESCRIPTION: Contains definition for the class to represent a return value
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
	// Possible types of a function's return value in the functions.xml file
	//*************************************************************************
	public __value enum ReturnValueType
	{
		Success = 0,
		Informational,
		Warning,
		Error
	};

	//*************************************************************************
	// Possible operator types of a function's return value in the functions.xml file
	//*************************************************************************
	public __value enum ReturnValueOperatorType
	{
		EqualTo = 0,
		GreaterThan,
		LessThan,
		GreaterThanOrEqualTo,
		LessThanOrEqualTo,
		NotEqualTo
	};

	//*************************************************************************
	// Representation of a function's return value in the functions.xml file
	//*************************************************************************
	public __gc class InterceptedFunctionReturnValue
	{
	protected:
		ReturnValueType type;
		ReturnValueOperatorType operatorType;
		unsigned int retValue;

	public:
		InterceptedFunctionReturnValue();
		~InterceptedFunctionReturnValue();
		InterceptedFunctionReturnValue* GetCloneObject();

		__property ReturnValueType get_Type() { return type; }
		__property void set_Type(ReturnValueType value) { type = value; }

		__property ReturnValueOperatorType get_OperatorType() { return operatorType; }
		__property void set_OperatorType(ReturnValueOperatorType value) { operatorType = value; }

		__property unsigned int get_ReturnValue() { return retValue; }
		__property void set_ReturnValue(unsigned int value) { retValue = value; }
	};
}