//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionTestList.h
//
// DESCRIPTION: Contains definition for the class FunctionTestList.
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
#include "FunctionTest.h"
using namespace SiUtils;

namespace Replacement
{
	//*************************************************************************
	// Contains a list of all tests registered for a given function
	//*************************************************************************
	class FunctionTestList
	{
	protected:
		int numTests;
		FunctionTest **tests;
		HANDLE heap;

	public:
		FunctionTestList();
		FunctionTestList(const FunctionTestList& testList);
		~FunctionTestList();

		FunctionTestList& operator=(const FunctionTestList& testList);

		inline int GetTestCount() { return numTests; };

		void AddTest(FunctionTest *test);
		FunctionTest* GetTestAt(int index);
	};
}