//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunction.h
//
// DESCRIPTION: Contains definition for the class InterceptedFunction
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "InterceptedFunctionParameter.h"
#include "InterceptedFunctionErrorCode.h"
#include "InterceptedFunctionReturnValue.h"

#using <mscorlib.dll>
using namespace System;
using namespace System::Collections;

namespace FunctionDatabase
{
	//*************************************************************************
	// Contains functionality to get data from the intercepted function database
	//*************************************************************************
	public __gc class InterceptedFunction
	{
	private:
		String *name;
		String *returnType;
		Queue *typeModifiers;
		Queue *ccModifiers;
		Queue *modifiers;
		String *category;
		Queue *parameters;
		Queue *errorCodes;
		Queue *returnValues;
		Queue *exceptions;
		String *originalDllName;
		String *interceptedDllName;
		String *replacementFunction;
		String *helpLink;

	public:
		InterceptedFunction();
		~InterceptedFunction();

		__property String *get_Name() { return name; }
		__property void set_Name(String *value) { name = value; }

		__property String *get_ReturnType() { return returnType; }
		__property void set_ReturnType(String *value) { returnType = value; }

		__property String *get_HelpLink() { return helpLink; }
		__property void set_HelpLink(String *value) { helpLink = value; }

		__property Array *get_Modifiers();
		__property Array *get_TypeModifiers() {return this->typeModifiers->ToArray();}
		__property Array *get_CallingConventionModifiers() {return this->ccModifiers->ToArray();}
		__property String *get_ModifiersString();
		
		__property String *get_Category() { return category; }
		__property void set_Category(String *value) { category = value; }

		__property Array *get_Parameters() { return parameters->ToArray(); }
		__property unsigned int get_ParameterCount() { return parameters->Count; }

		__property Array *get_ErrorCodes() { return errorCodes->ToArray(); }
		__property unsigned int get_ErrorCodeCount() { return errorCodes->Count; }

		__property Array *get_ReturnValues() { return returnValues->ToArray(); }
		__property unsigned int get_ReturnValueCount() { return returnValues->Count; }

		__property Array *get_Exceptions() { return exceptions->ToArray(); }
		__property unsigned int get_ExceptionCount() { return exceptions->Count; }

		__property String *get_OriginalDll() { return originalDllName; }
		__property void set_OriginalDll(String *value) { originalDllName = value; }

		__property String *get_InterceptedDll() { return interceptedDllName; }
		__property void set_InterceptedDll(String *value) { interceptedDllName = value; }

		__property String *get_ReplacementFunction() { return replacementFunction; }
		__property void set_ReplacementFunction(String *value) { replacementFunction = value; }

		void AddTypeModifier(String *modifier);
		void AddCallingConventionModifier(String *modifier);
		void AddParameter(InterceptedFunctionParameter *param);
		void AddErrorCode(InterceptedFunctionErrorCode *errorCode);
		void AddReturnValue(InterceptedFunctionReturnValue *returnValue);
		void AddException(String* exception);
		InterceptedFunction * GetCloneObject();
	};
};
