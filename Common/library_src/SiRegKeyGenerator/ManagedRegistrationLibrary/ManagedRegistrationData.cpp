//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		ManagedRegistrationData.cpp
//
// DESCRIPTION: Contains implementation for the managed wrapper around the
//	RegistrationData class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 09 Sep 2003		 B Shirey	 File created.
//*************************************************************************
#include "ManagedRegistrationData.h"

namespace ManagedRegistrationLibrary
{
	//*************************************************************************
	// Method:		ManagedRegistrationData
	// Description: Constructor for the ManagedRegistrationData class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ManagedRegistrationData::ManagedRegistrationData()
	{
		data = new RegistrationData();
	}

	//*************************************************************************
	// Method:		~ManagedRegistrationData
	// Description: Destructor for the ManagedRegistrationData class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ManagedRegistrationData::~ManagedRegistrationData()
	{
		if (data)
			delete data;
		data = NULL;
	}
}