//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		InterceptedFunctionDB.cpp
//
// DESCRIPTION: Contains implementation of the singleton class 
//	InterceptedFunctionDB.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include <stdio.h>
#include "InterceptedFunctionDB.h"
#include "ErrorCodeDB.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		InterceptedFunctionDB
	// Description: Constructor for the InterceptedFunctionDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionDB::InterceptedFunctionDB() : isLoaded(false)
	{
		functions = new Queue();
		interceptedDlls = new ArrayList();
		categoryFunctionsTable = new Hashtable();
		categoryClassesTable = new Hashtable();
		dllFunctionsTable = new Hashtable();
		dllClassesTable = new Hashtable();
		functionsByNameTable = new Hashtable();
		functionsByReplacementNameTable = new Hashtable();
		classFunctionsTable = new Hashtable();
	}

	//*************************************************************************
	// Method:		~InterceptedFunctionDB
	// Description: Destructor for the InterceptedFunctionDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	InterceptedFunctionDB::~InterceptedFunctionDB()
	{
	}

	//*************************************************************************
	// Method:		GetInstance
	// Description: return the single instance of the class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the class
	//*************************************************************************
	InterceptedFunctionDB *InterceptedFunctionDB::GetInstance(String* databaseFileName)
	{
		if (databaseFileName->Equals(FUNCTION_DB_FILE_NAME))
		{
			if (nativeInstance == NULL)
				nativeInstance = new InterceptedFunctionDB();
			return nativeInstance;
		}
		else if (databaseFileName->Equals(DOT_NET_FUNC_DB_FILE_NAME))
		{
			if (dotNetInstance == NULL)
				dotNetInstance = new InterceptedFunctionDB();
			return dotNetInstance;
		}
		else
		{
			// Unknown database file name, return a new instance
			return new InterceptedFunctionDB();
		}
	}

	//*************************************************************************
	// Method:		ValidationHandler
	// Description: callback function that handles any xml document validation 
	//	errors
	//
	// Parameters:
	//	sender - the object that sent the validation error
	//	args - the arguments that indicate the validation error
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunctionDB::ValidationHandler(Object *sender, ValidationEventArgs *args)
	{
		throw new Exception(args->Message);
	}
	
	//*************************************************************************
	// Method:		ValidateDocument
	// Description: checks the xml document for syntax errors
	//
	// Parameters:
	//	xmlFileName - the filename of the document to validate
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunctionDB::ValidateDocument(String *xmlFileName)
	{
		XmlTextReader *reader = new XmlTextReader(xmlFileName);
		XmlValidatingReader *validator = new XmlValidatingReader(reader);

		validator->ValidationType = ValidationType::DTD;
		validator->ValidationEventHandler += new ValidationEventHandler(this, ValidationHandler);

		// do nothing in this loop, all errors will be handled by ValidationHandler
		while (validator->Read())
		{
		}
	}

	//*************************************************************************
	// Method:		GetParameterTypeFromString
	// Description: converts a compare as type string to a corresponding
	//	ParameterType value
	//
	// Parameters:
	//	parameterTypeString - the string representing the parameter type
	//
	// Return Value: the converted ParameterType value
	//*************************************************************************
	ParameterType InterceptedFunctionDB::GetParameterTypeFromString(String *parameterTypeString)
	{
		if (parameterTypeString->CompareTo("StringType") == 0)
			return StringType;
		else if (parameterTypeString->CompareTo("WideStringType") == 0)
			return WideStringType;
		else if (parameterTypeString->CompareTo("BooleanType") == 0)
			return BooleanType;
		else if (parameterTypeString->CompareTo("IntegerType") == 0)
			return IntegerType;
		else if (parameterTypeString->CompareTo("UnsignedLongType") == 0)
			return UnsignedLongType;
		else if (parameterTypeString->CompareTo("RealType") == 0)
			return RealType;
		else if (parameterTypeString->CompareTo("PointerType") == 0)
			return PointerType;
		else if (parameterTypeString->CompareTo("PointerPointerType") == 0)
			return PointerPointerType;
		else if (parameterTypeString->CompareTo("Integer64Type") == 0)
			return Integer64Type;
		else if (parameterTypeString->CompareTo("OutStringType") == 0)
			return OutStringType;
		else if (parameterTypeString->CompareTo("OutWideStringType") == 0)
			return OutWideStringType;
		else if (parameterTypeString->CompareTo("IIDRefType") == 0)
			return IIDRefType;
		else if (parameterTypeString->CompareTo("IgnoreType") == 0)
			return IgnoreType;
		else if (parameterTypeString->CompareTo("NullType") == 0)
			return NullType;

		return NullType;
	}

	//*************************************************************************
	// Method:		ParseFunction
	// Description: parses the xml document and extracts a function from it
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the function node
	//		to extract
	//
	// Return Value: the extracted intercepted function
	//*************************************************************************
	InterceptedFunction *InterceptedFunctionDB::ParseFunction(XPathNavigator *childNavigator)
	{
		XPathNavigator *functionNavigator = childNavigator->Clone();
		InterceptedFunction *function = new InterceptedFunction();

		if (functionNavigator->NodeType == XPathNodeType::Comment)
			return NULL;

		bool hasMoreFunctionElements = functionNavigator->MoveToFirstChild();
		while (hasMoreFunctionElements)
		{
			if (functionNavigator->Name->CompareTo("FunctionName") == 0)
				function->Name = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("OriginalDll") == 0)
				function->OriginalDll = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("InterceptedDll") == 0)
				function->InterceptedDll = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("ReplacementFunctionName") == 0)
				function->ReplacementFunction = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("ReturnType") == 0)
				function->ReturnType = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("TypeModifier") == 0)
				function->AddTypeModifier(functionNavigator->Value);
			else if (functionNavigator->Name->CompareTo("CallingConventionModifier") == 0)
				function->AddCallingConventionModifier(functionNavigator->Value);
			else if (functionNavigator->Name->CompareTo("Category") == 0)
				function->Category = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("ReturnValue") == 0)
			{
				InterceptedFunctionReturnValue *returnValue = new InterceptedFunctionReturnValue();
				if (ParseReturnValue(functionNavigator, returnValue))
					function->AddReturnValue(returnValue);
			}
			else if (functionNavigator->Name->CompareTo("ErrorCode") == 0)
			{
				InterceptedFunctionErrorCode *errorCode = new InterceptedFunctionErrorCode();
				if (ParseErrorCode(functionNavigator, errorCode))
					function->AddErrorCode(errorCode);
			}
			else if (functionNavigator->Name->CompareTo("Exception") == 0)
				function->AddException(functionNavigator->Value);
			else if (functionNavigator->Name->CompareTo("HelpLink") == 0)
				function->HelpLink = functionNavigator->Value;
			else if (functionNavigator->Name->CompareTo("Param") == 0)
			{
				InterceptedFunctionParameter *param = ParseParameter(functionNavigator);
				if (param != NULL)
					function->AddParameter(param);
			}

			hasMoreFunctionElements = functionNavigator->MoveToNext();
		}

		return function;
	}

	//*************************************************************************
	// Method:		ParseParameter
	// Description: parses the xml document and extracts a parameter from it
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the parameter node
	//		to extract
	//
	// Return Value: the extracted intercepted function parameter
	//*************************************************************************
	InterceptedFunctionParameter *InterceptedFunctionDB::ParseParameter(XPathNavigator *childNavigator)
	{
		XPathNavigator *paramNavigator = childNavigator->Clone();
		InterceptedFunctionParameter *parameter = new InterceptedFunctionParameter();

		parameter->Name = paramNavigator->Value;
		parameter->Access = "NONE";

		// get the attributes of the fault tag
		bool hasMoreAttributes = paramNavigator->MoveToFirstAttribute();
		while (hasMoreAttributes)
		{
			if (paramNavigator->Name->CompareTo("Index") == 0)
				parameter->ID = Int32::Parse(paramNavigator->Value);
			else if (paramNavigator->Name->CompareTo("Type") == 0)
				parameter->Type = paramNavigator->Value;
			else if (paramNavigator->Name->CompareTo("CompareAs") == 0)
				parameter->CompareAsType = GetParameterTypeFromString(paramNavigator->Value);
			else if (paramNavigator->Name->CompareTo("Access") == 0)
				parameter->Access = paramNavigator->Value;
			else if (paramNavigator->Name->CompareTo("PointsToResource") == 0)
				parameter->PointsToResource = Convert::ToBoolean(paramNavigator->Value);
			else if (paramNavigator->Name->CompareTo("ConcatWith") == 0)
				parameter->ConcatWith = Int32::Parse(paramNavigator->Value);
			else if (paramNavigator->Name->CompareTo("MapTo") == 0)
				parameter->MapTo = paramNavigator->Value;

			hasMoreAttributes = paramNavigator->MoveToNextAttribute();
		}
		// get back to the fault tag
		paramNavigator->MoveToParent();

		return parameter;
	}

	//*************************************************************************
	// Method:		ParseErrorCode
	// Description: parses the xml document and extracts an error code value
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the error code node
	//		to extract
	//	errorCode - (out) - the error code object parsed from the xml document
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool InterceptedFunctionDB::ParseErrorCode(XPathNavigator *childNavigator, InterceptedFunctionErrorCode *errorCode)
	{
		XPathNavigator *ecNavigator = childNavigator->Clone();

		String *nodeValue = ecNavigator->Value;
		String *identifyBy = NULL;
		String *type = "Success";

		// get the attributes of the error code tag
		bool hasMoreAttributes = ecNavigator->MoveToFirstAttribute();
		while (hasMoreAttributes)
		{
			if (ecNavigator->Name->CompareTo("IdentifyBy") == 0)
				identifyBy = ecNavigator->Value;
			else if (ecNavigator->Name->CompareTo("Type") == 0)
				type = ecNavigator->Value;

			hasMoreAttributes = ecNavigator->MoveToNextAttribute();
		}
		// get back to the error code tag
		ecNavigator->MoveToParent();

		if (identifyBy == NULL)
			return false;

		// convert the type string to a enum value
		ErrorCodeType typeVal;
		if (type->CompareTo("Success") == 0)
			typeVal = ErrorCodeType::Success;
		else if (type->CompareTo("Informational") == 0)
			typeVal = ErrorCodeType::Informational;
		else if (type->CompareTo("Warning") == 0)
			typeVal = ErrorCodeType::Warning;
		else if (type->CompareTo("Error") == 0)
			typeVal = ErrorCodeType::Error;

		// fill in the value
		UInt32 *valueInt = NULL;
		if (identifyBy->CompareTo("Value") == 0)
		{
			valueInt = new UInt32(UInt32::Parse(nodeValue));
			if (valueInt == NULL)
				return false;

			errorCode->ECValue = *valueInt;
			errorCode->Type = typeVal;
			return true;
		}
		else if (identifyBy->CompareTo("Name") == 0)
		{
			String *codeString = ErrorCodeDB::GetInstance()->GetCodeFromString(nodeValue);
			if (!codeString)
				return false;

			valueInt = new UInt32(UInt32::Parse(codeString));
			if (valueInt == NULL)
				return false;

			errorCode->ECValue = *valueInt;
			errorCode->Type = typeVal;
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		ParseReturnValue
	// Description: parses the xml document and extracts a return value
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the return value node
	//		to extract
	//	returnValue - (out) - the return value object parsed from the xml document
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool InterceptedFunctionDB::ParseReturnValue(XPathNavigator *childNavigator, InterceptedFunctionReturnValue *returnValue)
	{
		XPathNavigator *rvNavigator = childNavigator->Clone();

		String *nodeValue = rvNavigator->Value;
		String *type = "Success";
		String *operatorType = "EqualTo";

		// get the attributes of the return value tag
		bool hasMoreAttributes = rvNavigator->MoveToFirstAttribute();
		while (hasMoreAttributes)
		{
			if (rvNavigator->Name->CompareTo("Type") == 0)
				type = rvNavigator->Value;
			else if (rvNavigator->Name->CompareTo("ValueOperator") == 0)
				operatorType = rvNavigator->Value;

			hasMoreAttributes = rvNavigator->MoveToNextAttribute();
		}
		// get back to the error code tag
		rvNavigator->MoveToParent();

		// convert the type string to an enum value
		ReturnValueType typeVal;
		if (type->CompareTo("Success") == 0)
			typeVal = ReturnValueType::Success;
		else if (type->CompareTo("Informational") == 0)
			typeVal = ReturnValueType::Informational;
		else if (type->CompareTo("Warning") == 0)
			typeVal = ReturnValueType::Warning;
		else if (type->CompareTo("Error") == 0)
			typeVal = ReturnValueType::Error;

		// convert the operator type to an enum value
		ReturnValueOperatorType operatorTypeVal = ReturnValueOperatorType::EqualTo;
		if (operatorType->CompareTo("EqualTo") == 0)
			operatorTypeVal = ReturnValueOperatorType::EqualTo;
		else if (operatorType->CompareTo("GreaterThan") == 0)
			operatorTypeVal = ReturnValueOperatorType::GreaterThan;
		else if (operatorType->CompareTo("LessThan") == 0)
			operatorTypeVal = ReturnValueOperatorType::LessThan;
		else if (operatorType->CompareTo("GreaterThanOrEqualTo") == 0)
			operatorTypeVal = ReturnValueOperatorType::GreaterThanOrEqualTo;
		else if (operatorType->CompareTo("LessThanOrEqualTo") == 0)
			operatorTypeVal = ReturnValueOperatorType::LessThanOrEqualTo;
		else if (operatorType->CompareTo("NotEqualTo") == 0)
			operatorTypeVal = ReturnValueOperatorType::NotEqualTo;

		// fill in the value
		UInt32 *valueInt = NULL;

		try
		{
			if (nodeValue->CompareTo("-1") == 0)
				valueInt = new UInt32(0xFFFFFFFF);
			else
				valueInt = new UInt32(UInt32::Parse(nodeValue));
		}
		catch(Exception *)
		{
			int i = 0;
		}

		if (valueInt == NULL)
			return false;

		returnValue->ReturnValue = *valueInt;
		returnValue->Type = typeVal;
		returnValue->OperatorType = operatorTypeVal;
		return true;
	}

	//*************************************************************************
	// Method:		LoadDatabase
	// Description: load the database into this object
	//
	// Parameters:
	//	databaseFolder - The folder that contains the database files
	//
	// Return Value: true if loaded successfully, false otherwise
	//*************************************************************************
	bool InterceptedFunctionDB::LoadDatabase(String *databaseFolder, String *databaseFileName)
	{
		ArrayList * xmlFiles = new ArrayList();

		ErrorCodeDB *db = ErrorCodeDB::GetInstance();
		if (!db->IsLoaded)
			db->LoadDatabase(databaseFolder);

		// clear all old data
		functions->Clear();

		//add the user XML files to the list
		String * userXMLs[];
		if (String::Compare (databaseFileName, FUNCTION_DB_FILE_NAME, true) == 0)				//for native
			userXMLs = Directory::GetFiles (databaseFolder, String::Concat (NATIVE_USER_FILENAME_DB_PREFIX, "*.xml"));
		else if (String::Compare (databaseFileName, DOT_NET_FUNC_DB_FILE_NAME, true) == 0)	    //for dotnet
			userXMLs = Directory::GetFiles (databaseFolder, String::Concat (DOTNET_USER_FILENAME_DB_PREFIX, "*.xml"));

		for (int i=0; i < userXMLs->Length; i++)
			xmlFiles->Add (userXMLs->Item[i]);		
		
		// add the Holodeck XML files to the list
		if (databaseFolder != NULL)
		{
			if (!databaseFolder->EndsWith("\\"))
				databaseFolder = String::Concat(databaseFolder, "\\");
			
			xmlFiles->Add (String::Concat(databaseFolder, databaseFileName));
		}
		else
		{
			databaseFolder = new String("");
			xmlFiles->Add (databaseFileName);
		}

		//Load the XML files and add to the database
		for (int loadXMLs=0; loadXMLs < xmlFiles->Count; loadXMLs++)
		{
			String *xmlFileName = dynamic_cast<String *> (xmlFiles->Item[loadXMLs]);
			// make sure the document is syntactically correct
			ValidateDocument(xmlFileName);

			// load in the data
			try
			{
				XPathDocument *doc = new XPathDocument(xmlFileName);
				XPathNavigator *navigator = doc->CreateNavigator();

				bool hasMoreFunctions = false;
				navigator->MoveToFirstChild();

				if (navigator->HasChildren)
				{
					navigator->MoveToFirstChild();
					hasMoreFunctions = true;
				}

				while (hasMoreFunctions)
				{
					InterceptedFunction *function = ParseFunction(navigator);

					if (function)
						AddFunction(function);

					hasMoreFunctions = navigator->MoveToNext();
				}
			}
			catch(System::IO::FileNotFoundException *e)
			{
				String *msg = new String("Could not find the function database file: \"");
				msg->Concat(e->FileName);
				throw new Exception(msg);
			}
		}

		isLoaded = true;
		return true;
	}

	//*************************************************************************
	// Method:		AddFunction
	// Description: Adds a function to the database
	//
	// Parameters:
	//	function - the function to add to the database
	//
	// Return Value: None
	//*************************************************************************
	void InterceptedFunctionDB::AddFunction(InterceptedFunction *function)
	{
		if (function != NULL)
		{
			functions->Enqueue(function);

			// add to the category function table
			String *category = function->Category;
			SortedList *categoryFunctions = dynamic_cast<SortedList *>(categoryFunctionsTable->get_Item(category));
			if (categoryFunctions)
			{
				//only allow one of each name into this list.  
				if (!categoryFunctions->Contains(function->Name))
					categoryFunctions->Add(function->Name, function);
			}
			else
			{
				categoryFunctions = new SortedList();
				categoryFunctions->Add(function->Name, function);
				categoryFunctionsTable->Add(category, categoryFunctions);
			}

			String *lowerCategory = function->Category->ToLower();
			String *lowerDll = function->OriginalDll->ToLower();
			int dotIndex = function->Name->LastIndexOf('.');
			if (dotIndex != -1)
			{
				String* className = function->Name->Substring(0, dotIndex);

				// add to class functions table
				ArrayList *classFunctions = dynamic_cast<ArrayList *>(classFunctionsTable->get_Item(className));
				if (classFunctions)
				{
					classFunctions->Add(function->Name);
				}
				else
				{
					classFunctions = new ArrayList();
					classFunctions->Add(function->Name);
					classFunctionsTable->Add(className, classFunctions);
				}

				// add to the category classes table
				ArrayList *categoryClasses = dynamic_cast<ArrayList *>(categoryClassesTable->get_Item(lowerCategory));
				if (!categoryClasses)
				{
					categoryClasses = new ArrayList();
					categoryClasses->Add(className);
					categoryClassesTable->Add(lowerCategory, categoryClasses);
				}
				else if (!categoryClasses->Contains(className))
					categoryClasses->Add(className);

				// add to the dll classes table
				ArrayList *dllClasses = dynamic_cast<ArrayList *>(dllClassesTable->get_Item(lowerDll));
				if (!dllClasses)
				{
					dllClasses = new ArrayList();
					dllClasses->Add(className);
					dllClassesTable->Add(lowerDll, dllClasses);
				}
				else if (!dllClasses->Contains(className))
					dllClasses->Add(className);
			}

			// add to the dll function table
			String *dll = function->OriginalDll;
			ArrayList *dllFunctions = dynamic_cast<ArrayList *>(dllFunctionsTable->get_Item(dll));
			if (dllFunctions)
			{
				dllFunctions->Add(function->Name);
			}
			else
			{
				dllFunctions = new ArrayList();
				dllFunctions->Add(function->Name);
				dllFunctionsTable->Add(dll, dllFunctions);
			}

			//add to the interceptedDlls array
			if (!interceptedDlls->Contains (function->InterceptedDll))
				interceptedDlls->Add (function->InterceptedDll);

			if (!functionsByNameTable->ContainsKey(function->Name))
			{
				ArrayList *newList = new ArrayList();
				newList->Add(function);
				functionsByNameTable->Add(function->Name, newList);
			}
			else
			{
				ArrayList *list = dynamic_cast<ArrayList*>(functionsByNameTable->Item[function->Name]);
				list->Add(function);
			}

			if (!functionsByReplacementNameTable->ContainsKey(function->ReplacementFunction))
			{
				ArrayList *newList = new ArrayList();
				newList->Add(function);
				functionsByReplacementNameTable->Add(function->ReplacementFunction, newList);
			}
			else
			{
				ArrayList *list = dynamic_cast<ArrayList*>(functionsByReplacementNameTable->Item[function->ReplacementFunction]);
				list->Add(function);
			}
		}
	}

	//*************************************************************************
	// Method:		GetFunctionCategoryArray
	// Description: Gets an array of functions that have the specified category.
	//	Will only return one instance of each name, if you need all functions of a given name
	//	use GetAllFunctionsByName
	//
	// Parameters:
	//	category - The category to get the array of functions for
	//
	// Return Value: the array of functions with the specified category
	//*************************************************************************
	Array *InterceptedFunctionDB::GetFunctionCategoryArray(String *category)
	{
		try
		{
			SortedList *categoryFunctions = dynamic_cast<SortedList *>(categoryFunctionsTable->get_Item(category));
			if (!categoryFunctions)
				return NULL;

			ArrayList *arrayList = new ArrayList(categoryFunctions->Values);

			return arrayList->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetCategoryArray
	// Description: Gets an array of all the categories which exist in the db
	//
	// Parameters:
	//	None
	//
	// Return Value: the array of categories in the db
	//*************************************************************************
	Array *InterceptedFunctionDB::GetCategoryArray()
	{
		try
		{
			ArrayList *categories = new ArrayList();
			IEnumerator *enumerator = categoryFunctionsTable->Keys->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *category = dynamic_cast<String *>(enumerator->Current);
				categories->Add(category);
			}
			return categories->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetDLLArray
	// Description: gets an array of all the DLLs which exist in the db
	//
	// Parameters:
	//	None
	//
	// Return Value: the array of DLLs  in the db
	//*************************************************************************
	Array *InterceptedFunctionDB::GetDLLArray()
	{
		try
		{
			ArrayList *dlls = new ArrayList();
			IEnumerator *enumerator = dllFunctionsTable->Keys->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *dll = dynamic_cast<String *>(enumerator->Current);
				dlls->Add(dll);
			}
			return dlls->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetFunctionByName
	// Description: gets the function from the database with the specified name
	//
	// Parameters:
	//	name - The name of the function to get
	//
	// Return Value: the function with the specified name, null if not found
	//*************************************************************************
	InterceptedFunction *InterceptedFunctionDB::GetFunctionByName(String *name)
	{
		if (name == NULL)
			return NULL;

		if (!functionsByNameTable->ContainsKey(name))
			return NULL;

		ArrayList* list = dynamic_cast<ArrayList*>(functionsByNameTable->Item[name]);
		if (list == NULL)
			return NULL;
		if (list->Count == 0)
			return NULL;

		return dynamic_cast<InterceptedFunction*>(list->Item[0])->GetCloneObject();;
	}

	//*************************************************************************
	// Method:		GetAllFunctionsByName
	// Description: gets all the functions from the database with the specified name
	//
	// Parameters:
	//	name - The name of the function to get
	//
	// Return Value: an arraylist of functions with the specified name, null if not found
	//*************************************************************************
	ArrayList *InterceptedFunctionDB::GetAllFunctionsByName(String *name)
	{
		if (name == NULL)
			return NULL;

		if (!functionsByNameTable->ContainsKey(name))
			return NULL;

		ArrayList * list = dynamic_cast<ArrayList*>(functionsByNameTable->Item[name]);
		ArrayList * clonedList = new ArrayList(list->Count);
		for (int i=0; i < list->Count; i++)
			clonedList->Add (dynamic_cast <InterceptedFunction*> (list->Item[i])->GetCloneObject());
		
		return clonedList;
	}

	//*************************************************************************
	// Method:		GetFunctionByReplacementName
	// Description: Gets the function from the database with the specified
	//				replacement name
	//
	// Parameters:
	//	replacementName - the replacement name of the function
	//
	// Return Value: (InterceptedFunction*) the function corresponding to the replacement name
	//*************************************************************************	
	InterceptedFunction* InterceptedFunctionDB::GetFunctionByReplacementName(String *replacementName)
	{
		if (replacementName == NULL)
			return NULL;

		if (!functionsByReplacementNameTable->ContainsKey(replacementName))
			return NULL;

		ArrayList* list = dynamic_cast<ArrayList*>(functionsByReplacementNameTable->Item[replacementName]);
		if (list == NULL)
			return NULL;
		if (list->Count == 0)
			return NULL;

		return dynamic_cast<InterceptedFunction*>(list->Item[0])->GetCloneObject();
	}

	//*************************************************************************
	// Method:		GetAllReplacementFunctionsByName
	// Description: gets all the replacement functions from the database with the specified name
	//
	// Parameters:
	//	replacementName - The name of the function to get
	//
	// Return Value: an arraylist of functions with the specified name, null if not found
	//*************************************************************************
	ArrayList *InterceptedFunctionDB::GetAllReplacementFunctionsByName(String *replacementName)
	{
		if (replacementName == NULL)
			return NULL;

		if (!functionsByReplacementNameTable->ContainsKey(replacementName))
			return NULL;

		ArrayList * list = dynamic_cast<ArrayList*>(functionsByReplacementNameTable->Item[replacementName]);
		ArrayList * clonedList = new ArrayList(list->Count);
		for (int i=0; i < list->Count; i++)
			clonedList->Add (dynamic_cast <InterceptedFunction*> (list->Item[i])->GetCloneObject());

		return clonedList;
	}

	//*************************************************************************
	// Method:		GetFunctionsDllArray
	// Description: Gets the functions which belong to the specified DLL
	//
	// Parameters:
	//	OriginalDll - The DLL form which to return the list of DLLs
	//
	// Return Value: an array of function names
	//*************************************************************************
	Array *InterceptedFunctionDB::GetFunctionsDllArray(String *OriginalDll)
	{
		try
		{
			ArrayList *dllFunctions = dynamic_cast<ArrayList *>(dllFunctionsTable->get_Item(OriginalDll));
			if (!dllFunctions)
				return NULL;

			return dllFunctions->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetClassDllArray
	// Description: Gets the classes which belong to the specified .NET DLL
	//
	// Parameters:
	//	OriginalDll - The DLL form which to return the list of DLLs
	//
	// Return Value: an array of class names
	//*************************************************************************
	Array *InterceptedFunctionDB::GetClassDllArray(String *OriginalDll)
	{
		try
		{
			ArrayList *dllClasses = dynamic_cast<ArrayList *>(dllClassesTable->get_Item(OriginalDll->ToLower()));
			if (!dllClasses)
				return NULL;

			return dllClasses->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetClassCategoryArray
	// Description: Gets the classes which belong to the specified category
	//
	// Parameters:
	//	category - The category from which to return the list of classes
	//
	// Return Value: an array of class names
	//*************************************************************************
	Array *InterceptedFunctionDB::GetClassCategoryArray(String *category)
	{
		try
		{
			ArrayList *categoryClasses = dynamic_cast<ArrayList *>(categoryClassesTable->get_Item(category->ToLower()));
			if (!categoryClasses)
				return NULL;

			return categoryClasses->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetClassArray
	// Description: Gets the all .NET classes in the DB
	//
	// Parameters:
	//	None
	//
	// Return Value: an array of class names
	//*************************************************************************
	Array *InterceptedFunctionDB::GetClassArray()
	{
		try
		{
			ArrayList *classes = new ArrayList();
			IEnumerator *enumerator = classFunctionsTable->Keys->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *currClass = dynamic_cast<String *>(enumerator->Current);
				classes->Add(currClass);
			}
			return classes->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetClassFunctionsArray
	// Description: Gets the functions which belong to the .NET class
	//
	// Parameters:
	//	ClassStr - The class from which to return the list of functions
	//
	// Return Value: an array of function names
	//*************************************************************************
	Array *InterceptedFunctionDB::GetClassFunctionsArray(String* ClassStr)
	{
		try
		{
			ArrayList *classFunctions = dynamic_cast<ArrayList *>(classFunctionsTable->get_Item(ClassStr));
			if (!classFunctions)
				return NULL;

			return classFunctions->ToArray();
		}
		catch(System::InvalidCastException *)
		{
			// just catch it and return null
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		GetReplacementDLLArray
	// Description: Gets the names of all the replacement libraries in the db
	//
	// Parameters:
	//	None
	//
	// Return Value: An array of replacement library names
	//*************************************************************************
	Array* InterceptedFunctionDB::GetReplacementDLLArray()
	{
		return interceptedDlls->ToArray();
	}
}