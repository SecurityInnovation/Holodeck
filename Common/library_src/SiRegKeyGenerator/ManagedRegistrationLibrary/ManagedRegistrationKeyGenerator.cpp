//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		ManagedRegistrationKeyGenerator.cpp
//
// DESCRIPTION: Contains implementation for the managed wrapper around the
//	RegistrationKeyGenerator class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 09 Sep 2003		 B Shirey	 File created.
//*************************************************************************
#include "ManagedRegistrationKeyGenerator.h"

namespace ManagedRegistrationLibrary
{
	//*************************************************************************
	// Method:		ManagedRegistrationKeyGenerator
	// Description: Constructor for the ManagedRegistrationKeyGenerator class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ManagedRegistrationKeyGenerator::ManagedRegistrationKeyGenerator()
	{
		keyGenerator = RegistrationKeyGenerator::GetInstance();
	}

	//*************************************************************************
	// Method:		~ManagedRegistrationKeyGenerator
	// Description: Destructor for the ManagedRegistrationKeyGenerator class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ManagedRegistrationKeyGenerator::~ManagedRegistrationKeyGenerator()
	{
	}

	//*************************************************************************
	// Method:		GetInstance
	// Description: returns the single instance of this class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of this class
	//*************************************************************************
	ManagedRegistrationKeyGenerator *ManagedRegistrationKeyGenerator::GetInstance()
	{
		if (instance == NULL)
			instance = new ManagedRegistrationKeyGenerator();
		return instance;
	}

	//*************************************************************************
	// Method:		GenerateRegistrationKey
	// Description: generates a registration key based on the info provided
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ManagedRegistrationKeyGenerator::GenerateRegistrationKey(ManagedRegistrationData *data)
	{
		RegistrationData regData;
		regData.SetCustomerType((CustomerType)data->CustType);
		regData.SetFunctionalityType((FunctionalityType)data->Functionality);
		regData.SetKeyVersion((RegKeyVersion)data->KeyVersion);
		regData.SetProductType((ProductType)data->Product);
		regData.SetNumberOfDaysValid(data->NumberOfDaysValid);
		regData.SetNumberOfLicensesPurchased(data->NumberOfLicensesPurchased);

		const __wchar_t __pin *sn = PtrToStringChars(data->SerialNumber);
		const __wchar_t __pin *key = PtrToStringChars(data->RegistrationKey);

		regData.SetSerialNumber((sn == NULL) ? "" : (SiString)sn);
		regData.SetRegistrationKey((key == NULL) ? "" : (SiString)key);

		bool result = keyGenerator->GenerateRegistrationKey(&regData);

		data->CustType = regData.GetCustomerType();
		data->Functionality = regData.GetFunctionalityType();
		data->KeyVersion = regData.GetKeyVersion();
		data->Product = regData.GetProductType();
		data->NumberOfDaysValid = regData.GetNumberOfDaysValid();
		data->NumberOfLicensesPurchased = regData.GetNumberOfLicensesPurchased();
		data->SerialNumber = (String *)((wchar_t *)regData.GetSerialNumber());
		data->RegistrationKey = (String *)((wchar_t *)regData.GetRegistrationKey());

		return result;
	}

	//*************************************************************************
	// Method:		GenerateRegistrationKey
	// Description: decodes a registration key based on the info provided
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ManagedRegistrationKeyGenerator::DecodeRegistrationKey(ManagedRegistrationData *data)
	{
		RegistrationData regData;
		regData.SetCustomerType((CustomerType)data->CustType);
		regData.SetFunctionalityType((FunctionalityType)data->Functionality);
		regData.SetKeyVersion((RegKeyVersion)data->KeyVersion);
		regData.SetProductType((ProductType)data->Product);
		regData.SetNumberOfDaysValid(data->NumberOfDaysValid);
		regData.SetNumberOfLicensesPurchased(data->NumberOfLicensesPurchased);

		const __wchar_t __pin *sn = PtrToStringChars(data->SerialNumber);
		const __wchar_t __pin *key = PtrToStringChars(data->RegistrationKey);

		regData.SetSerialNumber((sn == NULL) ? "" : (SiString)sn);
		regData.SetRegistrationKey((key == NULL) ? "" : (SiString)key);

		bool result = keyGenerator->DecodeRegistrationKey(&regData);

		data->CustType = regData.GetCustomerType();
		data->Functionality = regData.GetFunctionalityType();
		data->KeyVersion = regData.GetKeyVersion();
		data->Product = regData.GetProductType();
		data->NumberOfDaysValid = regData.GetNumberOfDaysValid();
		data->NumberOfLicensesPurchased = regData.GetNumberOfLicensesPurchased();
		data->SerialNumber = (String *)((wchar_t *)regData.GetSerialNumber());
		data->RegistrationKey = (String *)((wchar_t *)regData.GetRegistrationKey());

		return result;
	}
}