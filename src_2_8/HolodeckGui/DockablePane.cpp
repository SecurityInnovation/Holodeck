//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DockablePane.cpp
//
// DESCRIPTION: Contains some implementation for the abstract class DockablePane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "DockablePane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		DockablePane
	// Description: Constructor for the DockablePane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DockablePane::DockablePane()
	{
		this->BackColor = Color::White;
		this->BorderStyle = BorderStyle::Fixed3D;
	}

	//*************************************************************************
	// Method:		~DockablePane
	// Description: Destructor for the DockablePane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DockablePane::~DockablePane()
	{
	}
}