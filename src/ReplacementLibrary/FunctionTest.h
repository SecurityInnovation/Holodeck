//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionTest.h
//
// DESCRIPTION: Contains definition for the class FunctionTest and its
//	supporting structures.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "SiArray.h"
#include "SiString.h"
#include "ParameterType.h"
using namespace SiUtils;

namespace Replacement
{
	//*************************************************************************
	// The structure to hold a parameter for a test
	//*************************************************************************
	struct Parameter
	{
		int position;
		int matchIndex;
		void *value;
		unsigned int valueSize;
		ParameterType type;
		ParameterOperatorType operatorType;
	};

	enum ExistsType
	{
		NAEXISTS,
		EXISTS,
		NOTEXISTS,
		CREATE
	};

	struct CheckResource
	{
		int ParamIndex;
		ExistsType Exists;
	};

	enum AllocationType
	{
		NA,		//Not applicable
		GT,		//Greater than
		GTE,	//Greater than or equal
		LT,		//Less than
		LTE,	//Less than or equal
		E,		//Equal
	};

	//*************************************************************************
	// The class to hold a single test for a function
	//*************************************************************************
	class FunctionTest
	{
	protected:
		SiString *testID;
		SiString *returnValue;
		SiString *errorCode;
		SiString *exception;
		SiString *registryPath;
		int firingOption;
		bool pauseAppOnTestFire;
		SiArray<SiString> firingDetails;
		AllocationType allocation;

		Parameter **matchParameters;
		Parameter **changeParameters;
		int numMatchParameters;
		int numChangeParameters;
		bool passThrough, passThroughReturnValue, passThroughErrorCode;
		CheckResource *checkResource;

		int refCount;

		void Cleanup();

		static HANDLE heap;
	public:
		FunctionTest();
		~FunctionTest();

		void AddRef();
		void Release();

		void *operator new(size_t size);
		void operator delete(void *p);

		inline const SiString& GetTestID() { return *testID; };
		inline void SetTestID(const SiString &testID) { *(this->testID) = testID; };

		inline const SiArray<SiString> GetFiringDetails() {return firingDetails;};
		inline void SetFiringDetails(const SiArray<SiString> &value) { this->firingDetails = value;};

		inline const int GetFiringOption() {return this->firingOption;};
		inline void SetFiringOption(const int value) {this->firingOption = value;};

		inline const bool GetPauseAppOnTestFire() {return this->pauseAppOnTestFire;};
		inline void SetPauseAppOnTestFire(const bool value) {this->pauseAppOnTestFire = value;};

		inline const SiString& GetRegistryPath() {return *registryPath; };
		inline void SetRegistryPath(const SiString &registryPath) { *(this->registryPath) = registryPath; };

		inline const SiString& GetReturnValue() { return *returnValue; };
		inline void SetReturnValue(const SiString &returnValue) { *(this->returnValue) = returnValue; };

		inline const SiString& GetErrorCode() { return *errorCode; };
		inline void SetErrorCode(const SiString &errorCode) { *(this->errorCode) = errorCode; };

		inline const SiString& GetException() { return *exception; };
		inline void SetException(const SiString &exception) { *(this->exception) = exception; };

		inline const AllocationType GetAllocation() { return allocation; };
		inline void SetAllocation(const AllocationType allocation) { this->allocation = allocation; };
		inline void SetAllocationString(SiString &value) 
		{
			if (value == "GT")
				allocation = GT;
			else if (value == "GTE")
				allocation = GTE;
			else if (value == "LT")
				allocation = LT;
			else if (value == "LTE")
				allocation = LTE;
			else if (value == "E")
				allocation = E;
			else
				allocation = NA;		
		}

		inline const bool GetPassThrough() { return passThrough; };
		inline void SetPassThrough(const bool passThrough) { this->passThrough = passThrough; };

		inline const bool GetPassThroughReturnValue() { return passThroughReturnValue; };
		inline void SetPassThroughReturnValue(const bool passThroughReturnValue) { this->passThroughReturnValue = passThroughReturnValue; };

		inline const bool GetPassThroughErrorCode() { return passThroughErrorCode; };
		inline void SetPassThroughErrorCode(const bool passThroughErrorCode) { this->passThroughErrorCode = passThroughErrorCode; };

		inline const CheckResource& GetCheckResource() { return *checkResource; };
		inline void SetCheckResource(const CheckResource &checkResource) { *(this->checkResource) = checkResource; };

		inline int GetMatchParameterCount() { return this->numMatchParameters; };
		inline int GetChangeParameterCount() { return this->numChangeParameters; };

		void AddParameterToMatch(int position, int matchIndex, void *value, int valueSize, ParameterType type, ParameterOperatorType operatorType);
		Parameter &GetParameterToMatchAt(int index);

		void AddParameterToChange(int position, void *value, int valueSize, ParameterType type);
		Parameter &GetParameterToChangeAt(int index);
	};
}
