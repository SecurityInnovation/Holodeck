//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionChangesParser.h
//
// DESCRIPTION: Parser for the file corruption changes XML
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.Xml.dll>
#include "defines.h"
#include "CorruptAction.h"

using namespace System::Xml;
using namespace System::Xml::XPath;
using namespace System::Xml::Schema;
using namespace System::IO;
using namespace System;
using namespace System::Collections;

namespace UIData
{
	//*************************************************************************
	// Contains functionality to get data from a file corruption changes XML file
	//*************************************************************************
	public __gc class FileCorruptionChangesParser
	{
	private:
		Queue *actions;
		String* origFile;
		String* corruptFile;
		bool isLoaded;

		void ValidationHandler(Object *sender, ValidationEventArgs *args);
		void ValidateDocument(String *xmlFileName);
		CorruptActionType GetActionTypeFromString(String *actionTypeString);
		Byte GetByteArrayFromString(String *arrayString, int size) __gc[];
		CorruptAction *ParseAction(XPathNavigator *childNavigator);
	public:
		FileCorruptionChangesParser();

		bool Load(String *xmlChangesFile);
		bool SaveRTFDetails(String* outFile);

		__property Array *get_Actions() { return actions->ToArray(); }
		__property String *get_OriginalFile() { return origFile; }
		__property String *get_CorruptedFile() { return corruptFile; }
		__property bool get_IsLoaded() { return isLoaded; }
	};
};