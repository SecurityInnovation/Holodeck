//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DotNetReplacementLibrary.h
//
// DESCRIPTION: Contains definition for the class ReplacementLibrary.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Apr 2002	103	 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include "SettingsManager.h"
#include "FunctionTestList.h"
#include "Log.h"

using namespace System;
using namespace Log;

namespace Replacement
{
	public __delegate bool OriginalMethodCaller(Object* params __gc[], Object* &retValue, Exception* &exception);

	public __gc class DotNetReplacementLibrary
	{
		String* originalDllName;

		bool GetFunctionTests(String* category, String* functionName, FunctionTestList &testList);
		bool DoesTestMatch(int paramCount, void** paramValues, ParameterType* paramTypes, FunctionTestList &testList,
			Exception* &matchingErrorCode, Type* returnType, Object* &matchingReturnValue, int *matchingTest);
		void ModifyChangeParameters(int paramCount, void** paramValues, ParameterType* paramTypes, FunctionTestList *testList,
			int matchingTest);

		int ConvertParamsToTypedArrays(Object* params __gc[], void** &paramValues, ParameterType* &paramTypes);
		void FreeParamTypedArrays(int paramCount, void** paramValues, ParameterType* paramTypes);

		Exception* ConvertExceptionStringToObject(String* str);
		Object* ConvertValueStringToObject(Type* type, String* str);

		unsigned int IncrementCallLevel();
		unsigned int DecrementCallLevel();

	public:
		DotNetReplacementLibrary(String* dllName);

		bool RunStandardTestsAndGetResults(OriginalMethodCaller* ofCaller, String* category, String* functionName,
			Object* params __gc[], Type* returnType, Object* &returnValue, Exception* &exception);

		bool IsCategoryLoggingEnabled(String* category);
		bool IsFunctionLoggingEnabled(String* category, String* functionName);

		void SendLog(String* category, String* functionName, int paramCount, void** paramValues,
			ParameterType* paramTypes, Exception* exception, Object* returnValue);

		static void EnableInterception();
		static void DisableInterception();

		static void EnableNativeInterception();
		static void DisableNativeInterception();
	};
}
