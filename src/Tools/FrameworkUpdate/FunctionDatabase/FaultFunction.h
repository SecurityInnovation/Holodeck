//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultFunction.h
//
// DESCRIPTION: Contains definition for the class FaultFunction
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#using <mscorlib.dll>
using namespace System;
using namespace System::Collections;

//bugbug - is there some way to merge this with FunctionTest.h?
namespace FunctionDatabase
{
	//*************************************************************************
	// Represents a function fault specification in the function database
	//*************************************************************************
	public __gc class FaultFunction
	{
	public:
		__value enum ExistsType
		{
			NAEXISTS,
			EXISTS,
			NOTEXISTS,
			CREATE
		};

		__value struct CheckResource
		{
			int ParamIndex;
			ExistsType Exists;
		};

		__value enum AllocationType
		{
			NA,		//Not applicable
			GT,		//Greater than
			GTE,	//Greater than or equal
			LT,		//Less than
			LTE,	//Less than or equal
			E,		//Equal
		};

	private:
		String *name;
		String *returnValue;
		String *errorCode;
		bool passThrough, passThroughReturnValue, passThroughErrorCode;
		String *exception;
		AllocationType allocation;
		ArrayList *matchParams;
		ArrayList *changeParams;
		//bugbug - this should be a struct ptr, not an arraylist
		ArrayList *checkResources;
		String *testID;

		bool pauseAppAfterTestFire;
		int firingOption;
		Object * firingDetails;

	public:
		FaultFunction();
		~FaultFunction();

		// BUGBUG: No idea why this is required but without this we get a member inaccessible error on MemberwiseClone()
		System::Object __gc *MemberwiseClone(void) { return MemberwiseClone(); }


		__property String *get_Name() { return name; }
		__property void set_Name(String *value) { name = value; }

		__property void set_FiringOption(int value) {this->firingOption = value;}
		__property int get_FiringOption() {return this->firingOption;}

		__property void set_PauseAppAfterTestFire (bool value) {this->pauseAppAfterTestFire = value;}
		__property bool get_PauseAppAfterTestFire() {return this->pauseAppAfterTestFire;}

		__property void set_FiringDetails (Object * value) {this->firingDetails = value;}
		__property Object * get_FiringDetails() {return this->firingDetails;}
		
		__property String *get_ReturnValue() { return returnValue; }
		__property void set_ReturnValue(String *value) { returnValue = value; }

		__property String *get_ErrorCode() { return errorCode; }
		__property void set_ErrorCode(String *value) { errorCode = value; }

		__property bool get_PassThrough() { return passThrough; }
		__property void set_PassThrough(bool value) { passThrough = value; }

		__property bool get_PassThroughReturnValue() { return passThroughReturnValue; }
		__property void set_PassThroughReturnValue(bool value) { passThroughReturnValue = value; }

		__property bool get_PassThroughErrorCode() { return passThroughErrorCode; }
		__property void set_PassThroughErrorCode(bool value) { passThroughErrorCode = value; }

		__property String *get_Exception() { return exception; }
		__property void set_Exception(String *value) { exception = value; }

		__property AllocationType get_Allocation() { return allocation; }
		__property void set_Allocation(AllocationType value) { allocation = value; }
		__property void set_AllocationString(String *value) 
		{
			if (value->CompareTo("GT") == 0)
				allocation = FaultFunction::AllocationType::GT;
			else if (value->CompareTo("GTE") == 0)
				allocation = FaultFunction::AllocationType::GTE;
			else if (value->CompareTo("LT") == 0)
				allocation = FaultFunction::AllocationType::LT;
			else if (value->CompareTo("LTE") == 0)
				allocation = FaultFunction::AllocationType::LTE;
			else if (value->CompareTo("E") == 0)
				allocation = FaultFunction::AllocationType::E;
			else
				allocation = FaultFunction::AllocationType::NA;		
		}

		__property ArrayList *get_MatchParams() { return matchParams; }
		__property void set_MatchParams(ArrayList *value) { matchParams = value; }

		__property ArrayList *get_ChangeParams() { return changeParams; }
		__property void set_ChangeParams(ArrayList *value) { changeParams = value; }

		__property ArrayList *get_CheckResources() { return checkResources; }
		__property void set_CheckResources(ArrayList *value) { checkResources = value; }

		__property String *get_TestID() { return testID; }
		__property void set_TestID(String *value) { testID = value; }
	};
}