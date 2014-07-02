//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		RegistrationData.h
//
// DESCRIPTION: Contains definition for the class that contains the 
//				contents of a product key
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Aug 2003		 B Shirey	 File created.
//*************************************************************************
#pragma once

#include "RegistrationEnums.h"
#include "SiString.h"

using namespace SiUtils;

namespace RegistrationLibrary
{
	//*************************************************************************
	// contains registration data
	//*************************************************************************
	class RegistrationData
	{
	protected:
		unsigned long numberOfLicensesPurchased;
		long numberOfDaysValid;

		RegKeyVersion keyVersion;
		CustomerType customerType;
		ProductType productType;
		FunctionalityType functionalityType;

		SiString serialNumber;
		SiString registrationKey;

	public:
		RegistrationData();
		~RegistrationData();

		unsigned long GetNumberOfLicensesPurchased() { return numberOfLicensesPurchased; }
		void SetNumberOfLicensesPurchased(unsigned long licenses) { numberOfLicensesPurchased = licenses; }

		long GetNumberOfDaysValid() { return numberOfDaysValid; }
		void SetNumberOfDaysValid(long daysLeft) { numberOfDaysValid = daysLeft; }

		RegKeyVersion GetKeyVersion() { return keyVersion; }
		void SetKeyVersion(RegKeyVersion version) { keyVersion = version; }

		CustomerType GetCustomerType() { return customerType; }
		void SetCustomerType(CustomerType customer) { customerType = customer; }

		ProductType GetProductType() { return productType; }
		void SetProductType(ProductType product) { productType = product; }

		FunctionalityType GetFunctionalityType() { return functionalityType; }
		void SetFunctionalityType(FunctionalityType functionality) { functionalityType = functionality; }

		const SiString &GetSerialNumber() { return serialNumber; }
		void SetSerialNumber(const SiString &serialNum) { serialNumber = serialNum; }

		const SiString &GetRegistrationKey() { return registrationKey; }
		void SetRegistrationKey(const SiString &regKey) { registrationKey = regKey; }
	};
}