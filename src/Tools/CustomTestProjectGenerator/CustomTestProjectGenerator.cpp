//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomTestProjectGenerator.cpp
//
// DESCRIPTION: Contains implementation for the class CustomTestProjectGen
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	---------------------------------------
// 12 Feb 2004		 Ady K			File Created
//*************************************************************************
#include "CustomTestProjectGenerator.h"

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// Method:		CustomTestProjectGen
	// Description: Constructor for the CustomTestProjectGen class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	CustomTestProjectGen::CustomTestProjectGen()
	{
		CustomTestProjectWizard *ctpw = new CustomTestProjectWizard();
		ctpw->ShowDialog();
	}

	//*************************************************************************
	// Method:		~CustomTestProjectGen
	// Description: Destructor for the CIProjectOptions class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	CustomTestProjectGen::~CustomTestProjectGen()
	{
	
	}
}
