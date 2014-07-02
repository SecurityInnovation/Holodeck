//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ErrorCodeDB.h
//
// DESCRIPTION: Contains definition for the singleton class ErrorCodeDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 FEB 2003		 B. Shirey	 File created.
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
	public __gc class ErrorCodeDB
	{
	private:
		static ErrorCodeDB *instance = NULL;
		Hashtable *byCodeKeyTable;
		Hashtable *byStringKeyTable;
		bool isLoaded;

		ErrorCodeDB();
		~ErrorCodeDB();

		void ValidationHandler(Object *sender, ValidationEventArgs *args);
		void ValidateDocument(String *xmlFileName);
	public:
		static ErrorCodeDB *GetInstance();
		bool LoadDatabase(String *databaseFolder);

		String *GetCodeFromString(String *str) { String * ret; this->byStringKeyTable->get_Item(str) ? ret = dynamic_cast<String *>(this->byStringKeyTable->get_Item(str)): ret = NULL; return ret;}
		String *GetStringFromCode(String *code) { String * ret; this->byCodeKeyTable->get_Item(code) ? ret = dynamic_cast<String *>(this->byCodeKeyTable->get_Item(code)) : ret = NULL; return ret;}

		ICollection *GetAllValues() { return this->byStringKeyTable->Keys; }

		__property bool get_IsLoaded() { return isLoaded; }
	};
}
