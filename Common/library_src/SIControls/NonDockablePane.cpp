//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		NonDockablePane.cpp
//
// DESCRIPTION: Contains some implementation for the abstract class DockablePane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#include "NonDockablePane.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		NonDockablePane
	// Description: Constructor for the NonDockablePane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NonDockablePane::NonDockablePane()
	{
	}

	//*************************************************************************
	// Method:		~NonDockablePane
	// Description: Destructor for the NonDockablePane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NonDockablePane::~NonDockablePane()
	{
	}

	//*************************************************************************
	// Method:		OnClosed
	// Description: Called when the pane is being closed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePane::OnClosed()
	{
		Dispose(true);
	}
}