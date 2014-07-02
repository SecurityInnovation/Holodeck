//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		RegistrationEnums.h
//
// DESCRIPTION: Contains enumerations for the RegistrationLibrary
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Aug 2003		 B Shirey	 File created.
//*************************************************************************
#pragma once

namespace RegistrationLibrary
{
	//*************************************************************************
	// Constants used by the library
	//*************************************************************************
	#define NO_TIME_EXPIRATION 0
	#define SITE_LICENSE 0
	#define ASCII_ZERO 48
	#define ASCII_BIG_A 65
	#define INDEX_OFFSET 67
	#define NUM_LETTERS 26
	#define MIN_SERIAL_NUMBER_LENGTH 7

	//*************************************************************************
	// SI Products
	//*************************************************************************
	enum ProductType
	{
		Holodeck = 0
	};

	//*************************************************************************
	// Different Registration Key Versions
	//*************************************************************************
	enum RegKeyVersion
	{
		HolodeckBasic = 0
	};

	//*************************************************************************
	// Functionality Levels
	//*************************************************************************
	enum FunctionalityType
	{
		FeatureDisabledTrial = 0,
		TimeTrial,
		FullVersion
	};

	//*************************************************************************
	// Customer Types
	//*************************************************************************
	enum CustomerType
	{
		NormalCustomer = 0
	};
}