//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Fault.cpp
//
// DESCRIPTION: Contains implementation for the class Fault
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#include "Fault.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		Fault
	// Description: Constructor for the Fault class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Fault::Fault()
	{
		faultFunctions = new Queue();
	}

	//*************************************************************************
	// Method:		~Fault
	// Description: Destructor for the Fault class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Fault::~Fault()
	{
		if (faultFunctions != NULL)
			faultFunctions->Clear();
	}
}