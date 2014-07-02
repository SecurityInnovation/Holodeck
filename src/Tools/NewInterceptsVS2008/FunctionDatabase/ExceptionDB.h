//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionDB.h
//
// DESCRIPTION: Contains definition for the singleton class ExceptionDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 APR 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include "defines.h"
#using <mscorlib.dll>
#using <System.Xml.dll>

using namespace System::Xml;
using namespace System::Xml::XPath;
using namespace System::Xml::Schema;
using namespace System;
using namespace System::Collections;

namespace FunctionDatabase
{
	public __gc class ExceptionDB
	{
	private:
		static ExceptionDB *instance = NULL;
		ArrayList *exceptionList;
		bool isLoaded;

		ExceptionDB();
		~ExceptionDB();

		void ValidationHandler(Object *sender, ValidationEventArgs *args);
		void ValidateDocument(String *xmlFileName);
	public:
		static ExceptionDB *GetInstance();
		bool LoadDatabase(String *databaseFolder);

		ICollection *GetAllValues() { return this->exceptionList; }

		__property bool get_IsLoaded() { return isLoaded; }
	};
}
