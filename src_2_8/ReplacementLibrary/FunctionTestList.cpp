//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionTestList.cpp
//
// DESCRIPTION: Contains implementation for the class FunctionTestList.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include "FunctionTestList.h"

namespace Replacement
{
	//*************************************************************************
	// Method:		FunctionTestList
	// Description: Constructor for the FunctionTestList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionTestList::FunctionTestList() : numTests(0), tests(NULL)
	{
		heap = GetProcessHeap();
	}

	//*************************************************************************
	// Method:		FunctionTestList
	// Description: Copy constructor for the FunctionTestList class
	//
	// Parameters:
	//	testList - FunctionTestList to copy
	//
	// Return Value: None
	//*************************************************************************
	FunctionTestList::FunctionTestList(const FunctionTestList& testList)
	{
		heap = GetProcessHeap();

		tests = (FunctionTest **)HeapAlloc(heap, HEAP_ZERO_MEMORY, testList.numTests * sizeof(FunctionTest *));
		memcpy(tests, testList.tests, testList.numTests * sizeof(FunctionTest *));
		numTests = testList.numTests;

		for (int i=0; i<numTests; i++)
			tests[i]->AddRef();
	}

	//*************************************************************************
	// Method:		FunctionTestList
	// Description: Destructor for the FunctionTestList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionTestList::~FunctionTestList()
	{
		if (tests != NULL)
		{
			for (int i=0; i<numTests; i++)
				tests[i]->Release();

			HeapFree(heap, HEAP_ZERO_MEMORY, tests);
			tests = NULL;
		}

		heap = NULL;
	}

	//*************************************************************************
	// Method:		operator=
	// Description: Assignment operator
	//
	// Parameters:
	//	testList - FunctionTestList to assign
	//
	// Return Value: Value assigned
	//*************************************************************************
	FunctionTestList& FunctionTestList::operator=(const FunctionTestList& testList)
	{
		if (tests == NULL)
			tests = (FunctionTest **)HeapAlloc(heap, HEAP_ZERO_MEMORY, testList.numTests * sizeof(FunctionTest *));
		else
			tests = (FunctionTest **)HeapReAlloc(heap, HEAP_ZERO_MEMORY, tests, testList.numTests * sizeof(FunctionTest *));
		memcpy(tests, testList.tests, testList.numTests * sizeof(FunctionTest *));
		numTests = testList.numTests;

		for (int i=0; i<numTests; i++)
			tests[i]->AddRef();

		return *this;
	}

	//*************************************************************************
	// Method:		AddTest
	// Description: Add a test to the list
	//
	// Parameters:
	//	test - The test to add
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTestList::AddTest(FunctionTest *test)
	{
		if (test == NULL)
			return;

		numTests++;
		if (tests == NULL)
			tests = (FunctionTest **)HeapAlloc(heap, HEAP_ZERO_MEMORY, numTests * sizeof(FunctionTest *));
		else
			tests = (FunctionTest **)HeapReAlloc(heap, HEAP_ZERO_MEMORY, tests, numTests * sizeof(FunctionTest *));

		if (tests == NULL)
			return;

		tests[numTests - 1] = test;
		test->AddRef();
	}

	//*************************************************************************
	// Method:		GetTestAt
	// Description: Gets the function test structure at position <index>
	//
	// Parameters:
	//	index - The index of the test to get
	//
	// Return Value: The function test at position <index>
	//*************************************************************************
	FunctionTest* FunctionTestList::GetTestAt(int index)
	{
		if ((index <= 0) || (index >= numTests))
			return tests[0];

		return tests[index];
	}
}