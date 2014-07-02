//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionTest.cpp
//
// DESCRIPTION: Contains implementation for the class FunctionTest.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include "functiontest.h"

namespace Replacement
{
	HANDLE FunctionTest::heap = NULL;

	//*************************************************************************
	// Method:		FunctionTest
	// Description: Constructor for the FunctionTest class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionTest::FunctionTest() : matchParameters(NULL), changeParameters(NULL),
		numMatchParameters(0), numChangeParameters(0), refCount(0)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		testID = new SiString();
		returnValue = new SiString();
		errorCode = new SiString();
		registryPath = new SiString();
		exception = new SiString();
		checkResource = new CheckResource;
		checkResource->ParamIndex = -1;
		checkResource->Exists = NAEXISTS;
	}

	//*************************************************************************
	// Method:		~FunctionTest
	// Description: Destructor for the FunctionTest class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionTest::~FunctionTest()
	{
		Cleanup();
	}

	//*************************************************************************
	// Method:		AddRef
	// Description: Increments the reference count of the object
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTest::AddRef()
	{
		InterlockedIncrement((LONG*)&refCount);
	}

	//*************************************************************************
	// Method:		Release
	// Description: Decrements the reference count and frees the object if
	//				the reference count becomes zero
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTest::Release()
	{
		try
		{
			if (InterlockedDecrement((LONG*)&refCount) == 0)
					delete this;
		}
		catch(...)
		{
		}

	}

	//*************************************************************************
	// Method:		Cleanup
	// Description: Cleans up memory for the FunctionTest class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTest::Cleanup()
	{
		// free the matchParameters memory
		if (matchParameters != NULL)
		{
			for (int i = 0; i < numMatchParameters; i++)
			{
				if (matchParameters[i] != NULL)
				{
					if (matchParameters[i]->value != NULL)
					{
						HeapFree(heap, HEAP_ZERO_MEMORY, matchParameters[i]->value);
						matchParameters[i]->value = NULL;
					}

					HeapFree(heap, HEAP_ZERO_MEMORY, matchParameters[i]);
					matchParameters[i] = NULL;
				}
			}
			HeapFree(heap, HEAP_ZERO_MEMORY, matchParameters);
			matchParameters = NULL;
		}

		// free the changeParameters memory
		if (changeParameters != NULL)
		{
			for (int i = 0; i < numChangeParameters; i++)
			{
				if (changeParameters[i] != NULL)
				{
					if (changeParameters[i]->value != NULL)
					{
						HeapFree(heap, HEAP_ZERO_MEMORY, changeParameters[i]->value);
						changeParameters[i]->value = NULL;
					}

					HeapFree(heap, HEAP_ZERO_MEMORY, changeParameters[i]);
					changeParameters[i] = NULL;
				}
			}
			HeapFree(heap, HEAP_ZERO_MEMORY, changeParameters);
			changeParameters = NULL;
		}

		if (testID != NULL)
		{
			delete testID;
			testID = NULL;
		}

		if (returnValue != NULL)
		{
			delete returnValue;
			returnValue = NULL;
		}

		if (errorCode != NULL)
		{
			delete errorCode;
			errorCode = NULL;
		}

		if (exception != NULL)
		{
			delete exception;
			exception = NULL;
		}

		if (registryPath != NULL)
		{
			delete registryPath;
			registryPath = NULL;
		}

		if (checkResource != NULL)
		{
			delete checkResource;
			checkResource = NULL;
		}

	}

	//*************************************************************************
	// Method:		operator new
	// Description: Allocates the memory for the object
	//
	// Parameters:
	//	size - The number of bytes to allocate
	//
	// Return Value: the pointer to the allocated memory
	//*************************************************************************
	void *FunctionTest::operator new(size_t size)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		void *p = HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
		return p;
	}

	//*************************************************************************
	// Method:		operator delete
	// Description: Deallocates the memory for the object
	//
	// Parameters:
	//	p - The pointer to memory to delete
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTest::operator delete(void *p)
	{
		if ((p != NULL) && (heap != NULL))
		{
			HeapFree(heap, HEAP_ZERO_MEMORY, p);
			p = NULL;
		}
	}

	//*************************************************************************
	// Method:		AddParameterToMatch
	// Description: Adds a match parameter to the test
	//
	// Parameters:
	//	position - The position of the parameter in the function
	//	value - The value of the parameter
	//	valueSize - The number of bytes of the value
	//	type - the type of the value of the parameter
	//	operatorType - the operator type of the parameter
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTest::AddParameterToMatch(int position, int matchIndex, void *value, int valueSize, ParameterType type, ParameterOperatorType operatorType)
	{
		numMatchParameters++;
		if (matchParameters == NULL)
			matchParameters = (Parameter **)HeapAlloc(heap, HEAP_ZERO_MEMORY, numMatchParameters * sizeof(FunctionTest *));
		else
			matchParameters = (Parameter **)HeapReAlloc(heap, HEAP_ZERO_MEMORY, matchParameters, numMatchParameters * sizeof(FunctionTest *));

		if (matchParameters == NULL)
			return;

		Parameter *param = (Parameter *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(Parameter));
		if (param == NULL)
			return;

		param->position = position;
		param->type = type;
		param->valueSize = valueSize;
		param->operatorType = operatorType;
		param->matchIndex = matchIndex;

		param->value = HeapAlloc(heap, HEAP_ZERO_MEMORY, valueSize);
		if (param->value == NULL)
			return;

		CopyMemory(param->value, value, valueSize);

		matchParameters[numMatchParameters - 1] = param;
	}

	//*************************************************************************
	// Method:		GetParameterToMatchAt
	// Description: Gets the parameter to match structure at position <index>
	//
	// Parameters:
	//	index - The index of the match parameter to get
	//
	// Return Value: The match parameter at position <index>
	//*************************************************************************
	Parameter& FunctionTest::GetParameterToMatchAt(int index)
	{
		if ((index <= 0) || (index >= numMatchParameters))
			return *(matchParameters[0]);

		return *(matchParameters[index]);
	}

	//*************************************************************************
	// Method:		AddParameterToChange
	// Description: Adds a change parameter to the test
	//
	// Parameters:
	//	position - The position of the parameter in the function
	//	value - The value of the parameter
	//	valueSize - The number of bytes of the value
	//	type - the type of the value of the parameter
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTest::AddParameterToChange(int position, void *value, int valueSize, ParameterType type)
	{
		numChangeParameters++;
		if (changeParameters == NULL)
			changeParameters = (Parameter **)HeapAlloc(heap, HEAP_ZERO_MEMORY, numChangeParameters * sizeof(FunctionTest *));
		else
			changeParameters = (Parameter **)HeapReAlloc(heap, HEAP_ZERO_MEMORY, changeParameters, numChangeParameters * sizeof(FunctionTest *));

		if (changeParameters == NULL)
			return;

		Parameter *param = (Parameter *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(Parameter));
		if (param == NULL)
			return;

		param->position = position;
		param->type = type;
		param->valueSize = valueSize;
		param->operatorType = EqualsType;

		param->value = HeapAlloc(heap, HEAP_ZERO_MEMORY, valueSize);
		if (param->value == NULL)
			return;

		CopyMemory(param->value, value, valueSize);

		changeParameters[numChangeParameters - 1] = param;
	}

	//*************************************************************************
	// Method:		GetParameterToChangeAt
	// Description: Gets the parameter to change structure at position <index>
	//
	// Parameters:
	//	index - The index of the change parameter to get
	//
	// Return Value: The change parameter at position <index>
	//*************************************************************************
	Parameter& FunctionTest::GetParameterToChangeAt(int index)
	{
		if ((index <= 0) || (index >= numChangeParameters))
			return *(changeParameters[0]);

		return *(changeParameters[index]);
	}
}