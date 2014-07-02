//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StringConverter.h
//
// DESCRIPTION: Contains definition for the class StringConverter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 08 Oct 2003		 B Shirey	 File created.
//*************************************************************************
#pragma once

#include "SiString.h"
#include <vcclr.h>

#using <mscorlib.dll>

using namespace SiUtils;
using namespace System;

namespace Holodeck
{
	//*************************************************************************
	// class that converts managed strings to SiStrings
	//*************************************************************************
	public __gc class StringConverter
	{
	private:
		static StringConverter *instance = NULL;
		StringConverter();
		~StringConverter();
	public:
		static StringConverter *GetInstance();
		bool ConvertToSiString(String *managedStr, SiString *siString);
	};
}