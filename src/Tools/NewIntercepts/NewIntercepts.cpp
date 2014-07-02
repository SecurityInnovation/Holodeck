//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewIntercepts.cpp
//
// DESCRIPTION: Contains implementation for the class NewIntercepts
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 30 Sep 2003		 Ady K	   Created
//*******************************************************************************
#include "NewIntercepts.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		NewIntercepts
	// Description: Constructor for the NewIntercepts class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	NewIntercepts::NewIntercepts ()
	{
		AddCustomInterceptWizard *p = new AddCustomInterceptWizard();
		p->ShowDialog();
	}

	//*************************************************************************
	// Method:		~NewIntercepts
	// Description: Desstructor for the NewIntercepts class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	NewIntercepts::~NewIntercepts()
	{

	}
}