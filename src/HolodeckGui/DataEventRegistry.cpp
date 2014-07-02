//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DataEventRegistry.cpp
//
// DESCRIPTION: Contains some implementation for the singleton class 
//	DataEventRegistry
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "DataEventRegistry.h"

namespace UIData
{
	//*************************************************************************
	// Method:		DataEventRegistry
	// Description: Constructor for the DataEventRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DataEventRegistry::DataEventRegistry()
	{
	}

	//*************************************************************************
	// Method:		~DataEventRegistry
	// Description: Destructor for the DataEventRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DataEventRegistry::~DataEventRegistry()
	{
	}

	//*************************************************************************
	// Method:		getInstance
	// Description: Return the single instance of the DataEventRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the DataEventRegistry class
	//*************************************************************************
	DataEventRegistry *DataEventRegistry::getInstance()
	{
		if (instance == NULL)
			instance = new DataEventRegistry();

		return instance;
	}
}