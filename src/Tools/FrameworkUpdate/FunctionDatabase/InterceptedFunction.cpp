//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunction.h
//
// DESCRIPTION: Contains implementation for the class InterceptedFunction
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include "InterceptedFunction.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		InterceptedFunction
	// Description: Constructor for the InterceptedFunction class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunction::InterceptedFunction()
	{
		modifiers = new Queue();
		ccModifiers = new Queue();
		typeModifiers = new Queue();
		parameters = new Queue();
		errorCodes = new Queue();
		exceptions = new Queue();
		returnValues = new Queue();
	}

	//*************************************************************************
	// Method:		~InterceptedFunction
	// Description: Destructor for the InterceptedFunction class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunction::~InterceptedFunction()
	{
	}

	//*************************************************************************
	// Method:		get_Modifiers
	// Description: get method for the Modifiers property
	//				Combines the TypeModifiers and Calling Convention Modifiers
	//				and returns the combined array
	//
	// Parameters:
	//	None
	//
	// Return Value: An array of modifiers
	//*************************************************************************
	Array *InterceptedFunction::get_Modifiers()
	{
		Array * typeMods = this->typeModifiers->ToArray();
		Array * ccMods = this->ccModifiers->ToArray();
		ArrayList * modifiers = new ArrayList();
		
		for (int i=0; i < typeMods->Count; i++)
			modifiers->Add (typeMods->Item[i]);

		for (int j=0; j < ccMods->Count; j++)
			modifiers->Add (ccMods->Item[j]);

		return modifiers->ToArray();
	}

	//*************************************************************************
	// Method:		get_ModifiersString
	// Description: get method for the ModifiersString property
	//				Returns a combined string of modifiers, both typeModifiers
	//				and calling convention modifiers
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representing the modifiers of the function
	//*************************************************************************
	String *InterceptedFunction::get_ModifiersString()
	{
		String *returnValue = "";

		int i = 0;
		IEnumerator *enumerator = this->Modifiers->GetEnumerator();
		while (enumerator->MoveNext())
		{
			String *modifier = dynamic_cast<String *>(enumerator->Current);
			if (!modifier)
				continue;

			if (i == 0)
				returnValue = modifier;
			else
				returnValue = String::Concat(returnValue, ", ", modifier);

			i++;
		}

		return returnValue;
	}

	//*************************************************************************
	// Method:		AddTypeModifier
	// Description: adds a type modifier to the function
	//
	// Parameters:
	//	modifier - a string representing the modifier to add
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunction::AddTypeModifier(String *modifier)
	{
		this->typeModifiers->Enqueue(modifier);
	}

	//*************************************************************************
	// Method:		AddCallingConventionModifier
	// Description: adds a calling convention modifier to the function
	//
	// Parameters:
	//	modifier - a string representing the modifier to add
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunction::AddCallingConventionModifier(String *modifier)
	{
		this->ccModifiers->Enqueue(modifier);
	}

	//*************************************************************************
	// Method:		AddParameter
	// Description: adds a parameter to the function
	//
	// Parameters:
	//	param - the parameter to add
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunction::AddParameter(InterceptedFunctionParameter *param)
	{
		parameters->Enqueue(param);
	}

	//*************************************************************************
	// Method:		AddErrorCode
	// Description: adds an error code to the function
	//
	// Parameters:
	//	errorCode - the error code to add
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunction::AddErrorCode(InterceptedFunctionErrorCode *errorCode)
	{
		errorCodes->Enqueue(errorCode);
	}

	//*************************************************************************
	// Method:		AddReturnValue
	// Description: adds a return value to the function
	//
	// Parameters:
	//	returnValue - the return value to add
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunction::AddReturnValue(InterceptedFunctionReturnValue *returnValue)
	{
		returnValues->Enqueue(returnValue);
	}

	//*************************************************************************
	// Method:		AddException
	// Description: adds an exception to the function
	//
	// Parameters:
	//	exception - the exception to add
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunction::AddException(String* exception)
	{
		exceptions->Enqueue(exception);
	}

	//*************************************************************************
	// Method:		GetCloneObject
	// Description: Gets a clone copy of the current instance
	//
	// Parameters:
	//	None
	//
	// Return Value: Cloned copy of this instance
	//*************************************************************************
	InterceptedFunction* InterceptedFunction::GetCloneObject()
	{
		InterceptedFunction * clone = new InterceptedFunction();

		clone->Name = this->Name;
		clone->ReturnType = this->ReturnType;
		clone->HelpLink = this->HelpLink;

		Array * typeMods = this->typeModifiers->ToArray();
		for (int tm=0; tm < typeMods->Length; tm++)
			clone->AddTypeModifier (dynamic_cast <String*> (typeMods->Item[tm]));

		Array * ccMods = this->ccModifiers->ToArray();
		for (int ccm=0; ccm < ccMods->Length; ccm++)
			clone->AddCallingConventionModifier (dynamic_cast <String*> (ccMods->Item[ccm]));

		clone->Category = this->Category;

		for (int p=0; p < this->Parameters->Length; p++)
		{
			InterceptedFunctionParameter *ifp = dynamic_cast <InterceptedFunctionParameter *> (this->Parameters->Item[p]);
			clone->AddParameter (ifp->CopyObject());
		}
		for (int ec=0; ec < this->ErrorCodes->Length; ec++)
		{
			InterceptedFunctionErrorCode *ifec = dynamic_cast <InterceptedFunctionErrorCode *> (this->ErrorCodes->Item[ec]);
			clone->AddErrorCode (ifec->GetCloneObject());
		}
		for (int rv=0; rv < this->ReturnValues->Length; rv++)
		{	
			InterceptedFunctionReturnValue * ifrv = dynamic_cast <InterceptedFunctionReturnValue*> (this->ReturnValues->Item[rv]);
			clone->AddReturnValue (ifrv->GetCloneObject());
		}
		for (int e=0; e < this->Exceptions->Length; e++)
		{
			clone->AddException (dynamic_cast <String*> (this->Exceptions->Item[e]));
		}

		clone->OriginalDll = this->OriginalDll;
		clone->InterceptedDll = this->InterceptedDll;
		clone->ReplacementFunction = this->ReplacementFunction;

		return clone;
	}

} // namespace