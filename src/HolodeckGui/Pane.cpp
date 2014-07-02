//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Pane.cpp
//
// DESCRIPTION: Contains some implementation for the abstract class Pane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "Pane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		Pane
	// Description: Constructor for the Pane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Pane::Pane()
	{
		uiEventRegistry = uiEventRegistry->getInstance();
	}

	//*************************************************************************
	// Method:		~Pane
	// Description: Destructor for the Pane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Pane::~Pane()
	{
	}
}
