//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RegistrationMgr.h
//
// DESCRIPTION: Contains definition for the singleton class RegistrationMgr
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Sep 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "ProductRegistrationLibrary.h"

using namespace RegistrationLibrary;

namespace HolodeckRegistration
{
	//*************************************************************************
	// class that manages product registration
	//*************************************************************************
	class RegistrationMgr
	{
	private:
		static RegistrationMgr *instance;
		RegistrationData *regData;

		RegistrationMgr();
		~RegistrationMgr();

		void LoadDataFromRegistry();
	public:
		static RegistrationMgr *GetInstance();

		void ConvertToFeatureDisabledTrial();

		void ReloadRegistrationData();
		bool SaveRegistrationData(const SiString &serialNumber, const SiString &registrationKey);

		RegistrationData *GetRegistrationData() { return regData; }

		bool IsFeatureDisabledTrial();
		bool IsTimeTrial();
		int GetNumberOfDaysValid();
		int GetNumberOfDaysRemaining();
	};
}