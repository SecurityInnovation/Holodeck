//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionReturnValue.cpp
//
// DESCRIPTION: Contains implementation for the class to represent a return
//	value in an intercepted function
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 22 SEP 2002		 B. Shirey	 File created.
//*************************************************************************
#include "InterceptedFunctionReturnValue.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		InterceptedFunctionReturnValue
	// Description: Constructor for the InterceptedFunctionReturnValue class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionReturnValue::InterceptedFunctionReturnValue()
	{
		type = Success;
		operatorType = EqualTo;
		retValue = 0;
	}

	//*************************************************************************
	// Method:		~InterceptedFunctionReturnValue
	// Description: Destructor for the InterceptedFunctionReturnValue class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionReturnValue::~InterceptedFunctionReturnValue()
	{
	}

	//*************************************************************************
	// Method:		GetCloneObject
	// Description: Gets a clone copy of the current instance
	//
	// Parameters:
	//	None
	//
	// Return Value: Cloned copy of this instance
	//*************************************************************************
	InterceptedFunctionReturnValue* InterceptedFunctionReturnValue::GetCloneObject()
	{
		InterceptedFunctionReturnValue * cloneObject = new InterceptedFunctionReturnValue();
		cloneObject->Type = this->Type;
		cloneObject->OperatorType = this->OperatorType;
		cloneObject->ReturnValue = this->ReturnValue;

		return cloneObject;
	}
}