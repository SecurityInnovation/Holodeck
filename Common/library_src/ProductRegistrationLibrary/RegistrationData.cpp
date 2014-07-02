//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		RegistrationData.cpp
//
// DESCRIPTION: Contains implementation for the class that contains the 
//				contents of a product key
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Aug 2003		 B Shirey	 File created.
//*************************************************************************
#include "RegistrationData.h"

namespace RegistrationLibrary
{
	//*************************************************************************
	// Method:		RegistrationData
	// Description: Constructor for the RegistrationData class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationData::RegistrationData() : numberOfLicensesPurchased(0),
		numberOfDaysValid(0), keyVersion(HolodeckBasic), customerType(NormalCustomer),
		productType(Holodeck), functionalityType(FeatureDisabledTrial)
	{
	}

	//*************************************************************************
	// Method:		RegistrationData
	// Description: Destructor for the RegistrationData class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationData::~RegistrationData()
	{
	}
}
