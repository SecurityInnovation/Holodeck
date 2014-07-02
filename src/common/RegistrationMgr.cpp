//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RegistrationMgr.cpp
//
// DESCRIPTION: Contains implementation for the singleton class RegistrationMgr
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Sep 2003		 B. Shirey	 File created.
//*************************************************************************
#include "RegistrationMgr.h"
#include "Registry.h"
#include "defines.h"

using namespace SiUtils;

namespace HolodeckRegistration
{
	RegistrationMgr *RegistrationMgr::instance = NULL;

	//*************************************************************************
	// Method:		RegistrationMgr
	// Description: Constructor for the RegistrationMgr class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationMgr::RegistrationMgr()
	{
		regData = new RegistrationData();

		regData->SetCustomerType(NormalCustomer);
		regData->SetNumberOfDaysValid(15);
		regData->SetFunctionalityType(TimeTrial);
		regData->SetProductType(Holodeck);
		regData->SetKeyVersion(HolodeckVista);
		regData->SetNumberOfLicensesPurchased(0);
		regData->SetSerialNumber("");
		regData->SetRegistrationKey("");

		LoadDataFromRegistry();
	}

	//*************************************************************************
	// Method:		RegistrationMgr
	// Description: Destructor for the RegistrationMgr class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationMgr::~RegistrationMgr()
	{
		if (regData != NULL)
			delete regData;
		regData = NULL;
	}

	//*************************************************************************
	// Method:		LoadDataFromRegistry
	// Description: loads the serial number and registration key from the registry
	//	and decodes the information
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationMgr::LoadDataFromRegistry()
	{
		Registry registry;

		regData->SetCustomerType(NormalCustomer);
		regData->SetNumberOfDaysValid(15);
		regData->SetFunctionalityType(TimeTrial);
		regData->SetProductType(Holodeck);
		regData->SetKeyVersion(HolodeckVista);
		regData->SetNumberOfLicensesPurchased(0);
		regData->SetSerialNumber("");
		regData->SetRegistrationKey("");

		if (!registry.OpenKey(ROOT_KEY, ROOT_PATH, KEY_QUERY_VALUE))
			return;

		SiString serialNum, regKey;
		if (!registry.Read(SERIAL_NUMBER_VALUE_NAME, serialNum))
			return;

		if (!registry.Read(REGISTRATION_KEY_VALUE_NAME, regKey))
			return;

		regData->SetSerialNumber(serialNum);
		regData->SetRegistrationKey(regKey);

		if (RegistrationKeyGenerator::GetInstance()->DecodeRegistrationKey(regData) == -1)
		{
			//if older key was used, offer to convert to time-trial
			MessageBox (0, "The current license information is for a previous release.\n\nHolodeck 2.8 and later require a newer license. Your license will be converted to a 15 day time-trial until a newer license is purchased.\n\nYou may of course revert back to the previous release of Holodeck to continue using your old registeration key.", "License Warning", 0);
			this->ConvertToFeatureDisabledTrial();
		}
	}

	//*************************************************************************
	// Method:		GetInstance
	// Description: returns the single instance of the class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of this class
	//*************************************************************************
	RegistrationMgr *RegistrationMgr::GetInstance()
	{
		if (instance == NULL)
			instance = new RegistrationMgr();

		return instance;
	}

	//*************************************************************************
	// Method:		ReloadRegistrationData
	// Description: reloads registration data from the registry
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of this class
	//*************************************************************************
	void RegistrationMgr::ReloadRegistrationData()
	{
		LoadDataFromRegistry();
	}

	//*************************************************************************
	// Method:		SaveRegistrationData
	// Description: reloads registration data from the registry
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of this class
	//*************************************************************************
	bool RegistrationMgr::SaveRegistrationData(const SiString &serialNumber, const SiString &registrationKey)
	{
		if ((serialNumber.GetLength() == 0) || (registrationKey.GetLength() == 0))
			return false;

		RegistrationData tempData;
		tempData.SetSerialNumber(serialNumber);
		tempData.SetRegistrationKey(registrationKey);

		if (RegistrationKeyGenerator::GetInstance()->DecodeRegistrationKey(&tempData) == 1)
		{
			Registry registry;

			if (!registry.OpenKey(ROOT_KEY, ROOT_PATH))
				return false;

			SiString serialNum, regKey;
			if (!registry.Write(SERIAL_NUMBER_VALUE_NAME, serialNumber))
				return false;

			if (!registry.Write(REGISTRATION_KEY_VALUE_NAME, registrationKey))
				return false;

			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		IsFeatureDisabledTrial
	// Description: returns true if the registration data indicates this is a
	//	feature disabled trial, false otherwise
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the registration data indicates this is a
	//	feature disabled trial, false otherwise
	//*************************************************************************
	bool RegistrationMgr::IsFeatureDisabledTrial()
	{
		if (regData == NULL)
			return false;

		if (regData->GetFunctionalityType() == FeatureDisabledTrial)
			return true;

		return false;
	}

	//*************************************************************************
	// Method:		IsTimeTrial
	// Description: returns true if the registration data indicates this is a
	//	time trial, false otherwise
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the registration data indicates this is a
	//	time trial, false otherwise
	//*************************************************************************
	bool RegistrationMgr::IsTimeTrial()
	{
		// if regData is null, set it to time trial, something wierd is
		// going on
		if (regData == NULL)
			return true;

		if (regData->GetFunctionalityType() == TimeTrial)
			return true;

		return false;
	}

	//*************************************************************************
	// Method:		GetNumberOfDaysValid
	// Description: returns the number of days indicated by the registration data
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of days indicated by the registration data
	//*************************************************************************
	int RegistrationMgr::GetNumberOfDaysValid()
	{
		if (regData == NULL)
			return 0;

		return regData->GetNumberOfDaysValid();
	}

	//*************************************************************************
	// Method:		GetNumberOfDaysRemaining
	// Description: returns the number of days remaining for this license
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of days remaining for this license
	//*************************************************************************
	int RegistrationMgr::GetNumberOfDaysRemaining()
	{
		DWORD dwHighValue, dwLowValue;
		::FILETIME currentFileTime;
		ULARGE_INTEGER trialStartTime, currentTime, calculatedTime;
		int numDays;
		Registry registry;

		if (!registry.OpenKey(ROOT_KEY, ROOT_PATH, KEY_QUERY_VALUE))
			return 0;

		if (!registry.Read("Config3", dwHighValue))
			return 0;

		if (!registry.Read("Config4", dwLowValue))
			return 0;

		trialStartTime.HighPart = dwHighValue;
		trialStartTime.LowPart = dwLowValue;

		GetSystemTimeAsFileTime(&currentFileTime);
		currentTime.HighPart = currentFileTime.dwHighDateTime;
		currentTime.LowPart = currentFileTime.dwLowDateTime;

		calculatedTime.QuadPart = currentTime.QuadPart - trialStartTime.QuadPart;
        numDays = (int)((calculatedTime.QuadPart * .0000001) / 86400);	//calculated in 100ns increments

		return GetNumberOfDaysValid() - numDays;
	}

	//*************************************************************************
	// Method:		ConvertToFeatureDisabledTrial
	// Description: converts the functionality to a feature disabled trial
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationMgr::ConvertToFeatureDisabledTrial()
	{
		Registry registry;

		if (!registry.OpenKey(ROOT_KEY, ROOT_PATH))
			return;

		registry.DeleteValue(SERIAL_NUMBER_VALUE_NAME);
		registry.DeleteValue(REGISTRATION_KEY_VALUE_NAME);

		//Reset the trial period. This technique is from HolodeckConfig.cpp
		//registry.DeleteValue("Config1");
		//registry.DeleteValue("Config2");
		//
		//HKEY configKey;
		//FILETIME fileTime;
		//GetSystemTimeAsFileTime(&fileTime);

		//RegSetValueEx(configKey, "Config1", 0, REG_DWORD, (const BYTE*) &fileTime.dwHighDateTime, sizeof(DWORD));
		//RegSetValueEx(configKey, "Config2", 0, REG_DWORD, (const BYTE*) &fileTime.dwLowDateTime, sizeof(DWORD));
		ReloadRegistrationData();
	}
}