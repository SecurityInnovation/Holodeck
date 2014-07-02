//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StringConverter.cpp
//
// DESCRIPTION: Contains implementation for the class StringConverter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 Oct 2003		 B Shirey	 File created.
//*************************************************************************
#include "StringConverter.h"

namespace Holodeck
{
	//*************************************************************************
	// Method:		StringConverter
	// Description: Constructor for the StringConverter class
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	StringConverter::StringConverter()
	{
	}

	//*************************************************************************
	// Method:		~StringConverter
	// Description: Destructor for the StringConverter class
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	StringConverter::~StringConverter()
	{
	}

	//*************************************************************************
	// Method:		GetInstance
	// Description: gets the single instance of this class
	//
	// Parameters:
	//  None
	//
	// Return Value: the single instance of this class
	//*************************************************************************
	StringConverter *StringConverter::GetInstance()
	{
		if (instance == NULL)
			instance = new StringConverter();
		return instance;
	}

	//*************************************************************************
	// Method:		ConvertToSiString
	// Description: converts a managed string to an sistring
	//
	// Parameters:
	//  managedStr - the managed string to convert
	//	siString - the sistring to hold the converted value
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool StringConverter::ConvertToSiString(String *managedStr, SiString *siString)
	{
		if (!managedStr || !siString)
			return false;

		const __wchar_t __gc *str = PtrToStringChars(managedStr);
		if (!str)
			return false;

		wchar_t *strTemp = new wchar_t[managedStr->Length + 1];
		int i = 0;
		for (; i < managedStr->Length; i++)
			strTemp[i] = str[i];
		strTemp[i] = L'\0';

		siString->operator =(strTemp);
		delete[] strTemp;

		return true;
	}
}