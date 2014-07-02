//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PortDB.h
//
// DESCRIPTION: Contains definition for the singleton class PortDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 2 May 2003		 R. Wagner	 File created.
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
	public __gc class PortDB
	{
	private:
		static PortDB *instance = NULL;
		Hashtable *byNumberKeyTable;
		bool isLoaded;

		PortDB();
		~PortDB();

		void ValidationHandler(Object *sender, ValidationEventArgs *args);
		void ValidateDocument(String *xmlFileName);
	public:
		static PortDB *GetInstance();
		bool LoadDatabase(String *databaseFolder);

		String *GetNameFromNumber(int number) { return dynamic_cast<String *>(this->byNumberKeyTable->get_Item(__box(number)->ToString())); }

		ICollection *GetAllValues() { return this->byNumberKeyTable->Keys; }

		__property bool get_IsLoaded() { return isLoaded; }
	};
}
