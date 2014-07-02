//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultFunctionDB.h
//
// DESCRIPTION: Contains definition for the singleton class FaultFunctionDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 06 JAN 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "InterceptedFunction.h"
#include "FaultFunction.h"
#include "Fault.h"
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
	//*************************************************************************
	// Contains functionality to get data from the fault function database
	//*************************************************************************
	public __gc class FaultFunctionDB
	{
	private:
		static FaultFunctionDB *instance = NULL;
		SortedList *faults;
		SortedList *diskFaults;
		SortedList *memoryFaults;
		SortedList *networkFaults;
		SortedList *registryFaults;
		SortedList *processFaults;
		SortedList *customFaults;
		bool isLoaded;

		FaultFunctionDB();
		~FaultFunctionDB();

		void ValidationHandler(Object *sender, ValidationEventArgs *args);
		void ValidateDocument(String *xmlFileName);

		Fault *ParseFault(XPathNavigator *childNavigator);
		FaultFunction *ParseFunction(XPathNavigator *childNavigator, String *returnValue, String *errorCode);

	public:
		static FaultFunctionDB *GetInstance();
		bool LoadDatabase(String *databaseFolder);
		Array *GetFunctionsWithFaultID(int faultID);
		Fault *GetFault(int faultID);
		int GetFaultIDFromString(String *faultName, FaultType faultType);
		Fault *GetFaultFromString(String *faultName, FaultType faultType);
		FaultType StringToFaultType(String *faultType);

		__property IList *get_Faults() { return faults->GetValueList(); }
		__property IList *get_DiskFaults() { return diskFaults->GetValueList(); }
		__property IList *get_MemoryFaults() { return memoryFaults->GetValueList(); }
		__property IList *get_NetworkFaults() { return networkFaults->GetValueList(); } 
		__property IList *get_RegistryFaults() { return registryFaults->GetValueList(); } 
		__property IList *get_ProcessFaults() { return processFaults->GetValueList(); } 
		__property IList *get_CustomFaults() { return customFaults->GetValueList(); }
		__property bool get_IsLoaded() { return isLoaded; }
	};
} // namespace