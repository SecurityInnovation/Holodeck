//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Fault.h
//
// DESCRIPTION: Contains definition for the class Fault
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "FaultFunction.h"
#include <stdio.h>

#using <mscorlib.dll>
using namespace System;
using namespace System::Collections;

namespace FunctionDatabase
{
	public __value enum FaultType
	{
		MemoryFault,
		DiskFault,
		NetworkFault,
		RegistryFault,
		ProcessFault,
		CustomFault
	};

	//*************************************************************************
	// Represents a fault specification in the function database
	//*************************************************************************
	public __gc class Fault
	{
	private:
		String *name;
		FaultType type;
		int faultID;			// the enumeration value that matches the name
		String *returnValue;
		String *errorCode;

		Queue *faultFunctions;
	public:
		Fault();
		~Fault();

		__property String *get_Name() { return name; }
		__property void set_Name(String *value) { name = value; }

		__property FaultType get_Type() { return type; }
		__property void set_Type(FaultType value) { type = value; }

		__property String *get_ReturnValue() { return returnValue; }
		__property void set_ReturnValue(String *value) { returnValue = value; }

		__property String *get_ErrorCode() { return errorCode; }
		__property void set_ErrorCode(String *value) { errorCode = value; }

		__property int get_FaultID() { return faultID; }
		__property void set_FaultID(int value) { faultID = value; }

		__property Array *get_FaultFunctions() { return faultFunctions->ToArray(); }

		void AddFaultFunction(FaultFunction *function) { if (function != NULL) faultFunctions->Enqueue(function); }
	};
}