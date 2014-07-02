//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionParameter.h
//
// DESCRIPTION: Contains definition for the class InterceptedFunctionParameter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "ParameterType.h"

#using <mscorlib.dll>
using namespace System;
using namespace System::Collections;

namespace FunctionDatabase
{
	public __gc class ParamTestCase
	{
		public:
		String * testValue;
		String * testOperator;

		ParamTestCase()
		{	
			this->testValue = "";
			this->testOperator = "";
		}

		ParamTestCase(String * testValue, String * testOperator)
		{
			this->testValue = testValue;
			this->testOperator = testOperator;
		}

		__property String* get_TestValue() { return testValue; }
		__property void set_TestValue(String *value) { testValue = value; }

		__property String* get_TestOperator() { return testOperator; }
		__property void set_TestOperator(String *value) { testOperator = value; }
	};


	//*************************************************************************
	// Contains attributes of an intercepted function parameter
	//*************************************************************************
	public __gc class InterceptedFunctionParameter
	{
	private:
		int id;
		String *name;
		String *type;
		ParameterType compareAsType;
		String *access;
		ArrayList * paramTestCases;
		String * changeValue;
		
		bool pointsToResource;
		int concatWith;
		String *mapTo;

	public:
		InterceptedFunctionParameter();
		~InterceptedFunctionParameter();
		InterceptedFunctionParameter* CopyObject();

		__property int get_ID() { return id; }
		__property void set_ID(int value) { id = value; }

		__property String* get_Name() { return name; }
		__property void set_Name(String *value) { name = value; }

		__property String* get_Type() { return type; }
		__property void set_Type(String *value) { type = value; }

		__property ParameterType get_CompareAsType() { return compareAsType; }
		__property void set_CompareAsType(ParameterType value) { compareAsType = value; }

		__property String* get_Access() { return access; }
		__property void set_Access(String *value) { access = value; }

		__property void set_ChangeValue (String * value) { changeValue = value;}
		__property String * get_ChangeValue() {return changeValue;}

		__property int get_TestCasesCount() {return paramTestCases->Count;}
		
		ParamTestCase * TestCase (int index) {return dynamic_cast <ParamTestCase *> (paramTestCases->Item[index]);}
		void AddTestCase (ParamTestCase * testCase) {paramTestCases->Add (testCase);}
		void RemoveTestCase (ParamTestCase * testCase) {paramTestCases->Remove (testCase);}
		void RemoveTestCaseByIndex (int index) {paramTestCases->RemoveAt (index);}
		void RemoveTestAllCases () {paramTestCases->Clear();}

		__property static ParameterType get_NullParameterType() { return NullType; }
		__property static ParameterType get_IIDRefParameterType() { return IIDRefType; }
		__property static ParameterType get_StringParameterType() { return StringType; }
		__property static ParameterType get_WideStringParameterType() { return WideStringType; }
		__property static ParameterType get_BooleanParameterType() { return BooleanType; }
		__property static ParameterType get_IntegerParameterType() { return IntegerType; }
		__property static ParameterType get_UnsignedLongParameterType() { return UnsignedLongType; }
		__property static ParameterType get_RealParameterType() { return RealType; }
		__property static ParameterType get_PointerParameterType() { return PointerType; }
		__property static ParameterType get_Integer64ParameterType() { return Integer64Type; }
		__property static ParameterType get_IgnoreParameterType() { return IgnoreType; }
		__property static ParameterType get_OutStringParameterType() { return OutStringType; }
		__property static ParameterType get_OutWideStringParameterType() { return OutWideStringType; }

		__property bool get_PointsToResource() { return pointsToResource; }
		__property void set_PointsToResource(bool value) { pointsToResource = value; }

		__property int get_ConcatWith() { return concatWith; }
		__property void set_ConcatWith(int value) { concatWith = value; }

		__property String* get_MapTo() { return mapTo; }
		__property void set_MapTo(String *value) {mapTo = value; }
	};
}