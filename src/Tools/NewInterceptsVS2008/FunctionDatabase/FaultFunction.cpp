//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultFunction.cpp
//
// DESCRIPTION: Contains implementation for the class FaultFunction
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#include "FaultFunction.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		FaultFunction
	// Description: Constructor for the FaultFunction class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultFunction::FaultFunction() : errorCode(0), returnValue(0)
	{
		matchParams = new ArrayList();
		changeParams = new ArrayList();
		checkResources = new ArrayList();

		passThroughReturnValue = false;
		passThroughErrorCode = false;

		pauseAppAfterTestFire = false;
		firingOption = 1;
		firingDetails = dynamic_cast <Object *> (new String (S"100"));
	}

	//*************************************************************************
	// Method:		~FaultFunction
	// Description: Destructor for the FaultFunction class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultFunction::~FaultFunction()
	{
	}
}