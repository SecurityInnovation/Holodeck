//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionErrorCode.cpp
//
// DESCRIPTION: Contains implementation for the class to represent an error code
//	in an intercepted function
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 22 SEP 2002		 B. Shirey	 File created.
//*************************************************************************
#include "InterceptedFunctionErrorCode.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		InterceptedFunctionErrorCode
	// Description: Constructor for the InterceptedFunctionErrorCode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionErrorCode::InterceptedFunctionErrorCode()
	{
		type = Success;
		ecValue = 0;
	}

	//*************************************************************************
	// Method:		~InterceptedFunctionErrorCode
	// Description: Destructor for the InterceptedFunctionErrorCode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionErrorCode::~InterceptedFunctionErrorCode()
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
	InterceptedFunctionErrorCode* InterceptedFunctionErrorCode::GetCloneObject()
	{
		InterceptedFunctionErrorCode * cloneObject = new InterceptedFunctionErrorCode();
		cloneObject->Type = this->Type;
		cloneObject->ECValue = this->ECValue;

		return cloneObject;
	}
}