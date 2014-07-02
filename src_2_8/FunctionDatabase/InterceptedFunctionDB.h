//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionDB.h
//
// DESCRIPTION: Contains definition for the singleton class 
//	InterceptedFunctionDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.Xml.dll>
#using <System.Windows.Forms.dll>
#include "defines.h"
#include "InterceptedFunction.h"
#include "InterceptedFunctionErrorCode.h"
#include "InterceptedFunctionParameter.h"

using namespace System;
using namespace System::Xml;
using namespace Microsoft::Win32;
using namespace System::IO;
using namespace System::Xml::XPath;
using namespace System::Xml::Schema;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace FunctionDatabase
{
	//*************************************************************************
	// Contains functionality to get data from the intercepted function database
	//*************************************************************************
	public __gc class InterceptedFunctionDB
	{
	private:
		static InterceptedFunctionDB *nativeInstance = NULL;
		static InterceptedFunctionDB *dotNetInstance = NULL;

		Queue *functions;
		ArrayList *interceptedDlls;
		bool isLoaded;

		Hashtable *categoryFunctionsTable;
		Hashtable *categoryClassesTable;
		Hashtable *dllFunctionsTable;
		Hashtable *dllClassesTable;
		Hashtable *functionsByNameTable;
		Hashtable *functionsByReplacementNameTable;
		Hashtable *classFunctionsTable;

		InterceptedFunctionDB();
		~InterceptedFunctionDB();

		void ValidationHandler(Object *sender, ValidationEventArgs *args);
		void ValidateDocument(String *xmlFileName);
		ParameterType GetParameterTypeFromString(String *parameterTypeString);
		InterceptedFunction *ParseFunction(XPathNavigator *childNavigator);
		InterceptedFunctionParameter *ParseParameter(XPathNavigator *childNavigator);
		bool ParseErrorCode(XPathNavigator *childNavigator, InterceptedFunctionErrorCode *errorCode);
		bool ParseReturnValue(XPathNavigator *childNavigator, InterceptedFunctionReturnValue *returnValue);
	public:
		static InterceptedFunctionDB *GetInstance(String* databaseFileName);
		bool LoadDatabase(String *databaseFolder, String* databaseFileName);

		__property Array *get_Functions() { return functions->ToArray(); }
		__property Array *get_InterceptedDLLs() { return interceptedDlls->ToArray(); }
		__property bool get_IsLoaded() { return isLoaded; }

		void AddFunction(InterceptedFunction *function);
		Array *GetFunctionCategoryArray(String *category);
		Array *GetCategoryArray();
		Array *GetDLLArray();
		Array *GetReplacementDLLArray();
		Array *GetFunctionsDllArray(String *OriginalDll);
		Array *GetClassArray();
		Array *GetClassCategoryArray(String *category);
		Array *GetClassDllArray(String *OriginalDll);
		Array *GetClassFunctionsArray(String* ClassStr);
		InterceptedFunction *GetFunctionByName(String *name);
		ArrayList *GetAllFunctionsByName(String *name);
		InterceptedFunction *GetFunctionByReplacementName(String *replacementName);
		ArrayList *GetAllReplacementFunctionsByName(String *replacementName);
	};
};