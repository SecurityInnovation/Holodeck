//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		UIEventRegistry.cpp
//
// DESCRIPTION: Contains some implementation for the singleton class 
//	UIEventRegistry
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "UIEventRegistry.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		UIEventRegistry
	// Description: Constructor for the UIEventRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	UIEventRegistry::UIEventRegistry()
	{
	}

	//*************************************************************************
	// Method:		~UIEventRegistry
	// Description: Destructor for the UIEventRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	UIEventRegistry::~UIEventRegistry()
	{
	}

	//*************************************************************************
	// Method:		getInstance
	// Description: Return the single instance of the UIEventRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the UIEventRegistry class
	//*************************************************************************
	UIEventRegistry *UIEventRegistry::getInstance()
	{
		if (instance == NULL)
			instance = new UIEventRegistry();

		return instance;
	}
}