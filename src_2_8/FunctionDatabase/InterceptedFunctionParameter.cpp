//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionParameter.cpp
//
// DESCRIPTION: Contains implementation for the class 
//	InterceptedFunctionParameter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include "interceptedfunctionparameter.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		InterceptedFunctionParameter
	// Description: Constructor for the InterceptedFunctionParameter class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionParameter::InterceptedFunctionParameter()
	{
		paramTestCases = new ArrayList();
		concatWith = -1;
	}

	//*************************************************************************
	// Method:		~InterceptedFunctionParameter
	// Description: Destructor for the InterceptedFunctionParameter class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionParameter::~InterceptedFunctionParameter()
	{
	}

	//*************************************************************************
	// Method:		CopyObject
	// Description: Creates a clone of the current instance
	//
	// Parameters:
	//	None
	//
	// Return Value: A clone of the current instance
	//*************************************************************************
	InterceptedFunctionParameter* InterceptedFunctionParameter::CopyObject()
	{
		InterceptedFunctionParameter *copyObject = new InterceptedFunctionParameter();

		copyObject->ID = this->ID;
		copyObject->Name = this->Name;
		copyObject->Type = this->Type;
		copyObject->CompareAsType = this->CompareAsType;
		copyObject->Access = this->Access;
		copyObject->ChangeValue = this->ChangeValue;
		copyObject->PointsToResource = this->PointsToResource;
		copyObject->ConcatWith = this->ConcatWith;
		copyObject->MapTo = this->MapTo;

		copyObject->RemoveTestAllCases();
		for (int i=0; i < this->paramTestCases->Count; i++)
		{
			ParamTestCase * ptc = dynamic_cast <ParamTestCase *> (this->paramTestCases->Item[i]);
			copyObject->AddTestCase (new ParamTestCase (ptc->TestValue, ptc->TestOperator));
		}

		return copyObject;
	}
}