//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		ManagedRegistrationData.cpp
//
// DESCRIPTION: Contains definition for the managed wrapper around the
//	RegistrationData class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 09 Sep 2003		 B Shirey	 File created.
//*************************************************************************
#pragma once

#include "ProductRegistrationLibrary.h"
#include <vcclr.h>

#using <mscorlib.dll>

using namespace RegistrationLibrary;
using namespace System;

namespace ManagedRegistrationLibrary
{
	//*************************************************************************
	// The managed wrapper around the RegistrationData class
	//*************************************************************************
	public __gc class ManagedRegistrationData
	{
	protected:
		RegistrationData *data;
	public:
		ManagedRegistrationData();
		~ManagedRegistrationData();

		__property unsigned long get_NumberOfLicensesPurchased() { return data->GetNumberOfLicensesPurchased(); }
		__property void set_NumberOfLicensesPurchased(unsigned long value) { data->SetNumberOfLicensesPurchased(value); }

		__property long get_NumberOfDaysValid() { return data->GetNumberOfDaysValid(); }
		__property void set_NumberOfDaysValid(long value) { data->SetNumberOfDaysValid(value); }

		__property int get_KeyVersion() { return data->GetKeyVersion(); }
		__property void set_KeyVersion(int value) { data->SetKeyVersion((RegKeyVersion)value); }

		__property int get_CustType() { return data->GetCustomerType(); }
		__property void set_CustType(int value) { data->SetCustomerType((CustomerType)value); }

		__property int get_Functionality() { return data->GetFunctionalityType(); }
		__property void set_Functionality(int value) { data->SetFunctionalityType((FunctionalityType)value); }

		__property int get_Product() { return data->GetProductType(); }
		__property void set_Product(int value) { data->SetProductType((ProductType)value); }

		__property String *get_SerialNumber() { return (String *)((wchar_t *)data->GetSerialNumber()); }
		__property void set_SerialNumber(String *value) 
		{ 
			const __wchar_t __pin *str = PtrToStringChars(value);
			SiString siStr = str;
			data->SetSerialNumber(siStr); 
		}

		__property String *get_RegistrationKey() { return (String *)((wchar_t *)data->GetRegistrationKey()); }
		__property void set_RegistrationKey(String *value)
		{ 
			const __wchar_t __pin *str = PtrToStringChars(value);
			SiString siStr = str;
			data->SetRegistrationKey(siStr); 
		}
	};
}
