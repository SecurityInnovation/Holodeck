//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultFunctionDB.cpp
//
// DESCRIPTION: Contains implementation for the singleton class FaultFunctionDB
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 06 JAN 2003		 B. Shirey	 File created.
//*************************************************************************
#include "faultfunctiondb.h"

namespace FunctionDatabase
{
	//*************************************************************************
	// Method:		FaultFunctionDB
	// Description: Constructor for the FaultFunctionDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultFunctionDB::FaultFunctionDB() : isLoaded(false)
	{
		faults = new SortedList();
		diskFaults = new SortedList();
		memoryFaults = new SortedList();
		networkFaults = new SortedList();
		registryFaults = new SortedList();
		processFaults = new SortedList();
		customFaults = new SortedList();
	}

	//*************************************************************************
	// Method:		~FaultFunctionDB
	// Description: Destructor for the FaultFunctionDB class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultFunctionDB::~FaultFunctionDB()
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
	FaultFunctionDB *FaultFunctionDB::GetInstance()
	{
		if (instance == NULL)
			instance = new FaultFunctionDB();

		return instance;
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
	void FaultFunctionDB::ValidationHandler(Object *sender, ValidationEventArgs *args)
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
	void FaultFunctionDB::ValidateDocument(String *xmlFileName)
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
	// Method:		ParseFault
	// Description: parses the xml document and extracts a fault from it
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the fault node
	//		to extract
	//
	// Return Value: the extracted fault
	//*************************************************************************
	Fault *FaultFunctionDB::ParseFault(XPathNavigator *childNavigator)
	{
		XPathNavigator *faultNavigator = childNavigator->Clone();
		Fault *fault = new Fault();
		static int currentLoadedFaultID = 0;

		// get the attributes of the fault tag
		bool hasMoreAttributes = faultNavigator->MoveToFirstAttribute();
		while (hasMoreAttributes)
		{
			if (faultNavigator->Name->CompareTo("Name") == 0)
			{
				fault->Name = faultNavigator->Value->Replace('_', ' ');
				fault->FaultID = currentLoadedFaultID++;
			}
			else if (faultNavigator->Name->CompareTo("Type") == 0)
				fault->Type = StringToFaultType(faultNavigator->Value->ToLower());
			else if (faultNavigator->Name->CompareTo("ReturnValue") == 0)
				fault->ReturnValue = faultNavigator->Value;
			else if (faultNavigator->Name->CompareTo("ErrorCode") == 0)
				fault->ErrorCode = faultNavigator->Value;

			hasMoreAttributes = faultNavigator->MoveToNextAttribute();
		}
		// get back to the fault tag
		faultNavigator->MoveToParent();

		bool hasMoreFaultElements = faultNavigator->MoveToFirstChild();
		while (hasMoreFaultElements)
		{
			if (faultNavigator->Name->CompareTo("Function") == 0)
			{
				FaultFunction *function = ParseFunction(faultNavigator, fault->ReturnValue, fault->ErrorCode);
				if (function != NULL)
					fault->AddFaultFunction(function);
			}

			hasMoreFaultElements = faultNavigator->MoveToNext();
		}


		return fault;
	}

	//*************************************************************************
	// Method:		GetFaultIDFromString
	// Description: converts a fault string to a corresponding integer value
	//
	// Parameters:
	//	faultName - the string representing the fault name
	//	faultType - type of fault to look for
	//
	// Return Value: the converted fault id, -1 on failure
	//*************************************************************************
	int FaultFunctionDB::GetFaultIDFromString(String *faultName, FaultType faultType)
	{
		faultName = faultName->Replace('_', ' ');

		Fault *fault = GetFaultFromString(faultName, faultType);

		if (fault)
			return fault->FaultID;
		else
			return -1;
	}

	//*************************************************************************
	// Method:		GetFaultFromString
	// Description: converts a fault string to a corresponding integer value
	//
	// Parameters:
	//	faultName - the string representing the fault name
	//	faultType - type of fault to look for
	//
	// Return Value: the found fault, NULL on failure
	//*************************************************************************
	Fault *FaultFunctionDB::GetFaultFromString(String *faultName, FaultType faultType)
	{
		Fault *fault;

		switch (faultType)
		{
		case DiskFault:
			fault = dynamic_cast <Fault*> (diskFaults->Item[faultName]);	
			break;
		case MemoryFault:
			fault = dynamic_cast <Fault*> (memoryFaults->Item[faultName]);	
			break;
		case NetworkFault:
			fault = dynamic_cast <Fault*> (networkFaults->Item[faultName]);	
			break;
		case RegistryFault:
			fault = dynamic_cast <Fault*> (registryFaults->Item[faultName]);	
			break;
		case ProcessFault:
			fault = dynamic_cast <Fault*> (processFaults->Item[faultName]);	
			break;
		case CustomFault:
			fault = dynamic_cast <Fault*> (customFaults->Item[faultName]);	
			break;
		}

		return fault;
	}

	//*************************************************************************
	// Method:		ParseFunction
	// Description: parses the xml document and extracts a fault function from it
	//
	// Parameters:
	//	childNaviator - the path navigator that represents the fault function node
	//		to extract
	//	returnValue - the default return value to set for the function if not 
	//		overridden in the xml
	//	errorCode - the default errorCode to set for the function if not overridden
	//		in the xml
	//
	// Return Value: the extracted fault function
	//*************************************************************************
	FaultFunction *FaultFunctionDB::ParseFunction(XPathNavigator *childNavigator, String *returnValue, String *errorCode)
	{
		XPathNavigator *functionNavigator = childNavigator->Clone();
		FaultFunction *function = new FaultFunction();
		bool hasMoreAttributes, hasMoreParams, hasMoreElements;
		bool hasOverriddenReturnValue = false;
		bool hasOverriddenErrorCode = false;

		// get the attributes of the function tag
		hasMoreAttributes = functionNavigator->MoveToFirstAttribute();
		while (hasMoreAttributes)
		{
			if (functionNavigator->Name->CompareTo("Name") == 0)
			{
				function->Name = functionNavigator->Value;
			}
			else if (functionNavigator->Name->CompareTo("PassThrough") == 0)
			{
				function->PassThrough = Convert::ToBoolean(functionNavigator->Value);
			}
			else if (functionNavigator->Name->CompareTo("Exception") == 0)
			{
				function->Exception = functionNavigator->Value;
			}
			else if (functionNavigator->Name->CompareTo("Allocation") == 0)
			{
				function->AllocationString = functionNavigator->Value;
			}
			else if (functionNavigator->Name->CompareTo("OverrideErrorCode") == 0)
			{
				if (functionNavigator->Value->CompareTo("No Change") == 0)
				{
					function->PassThroughErrorCode = true;
				}
				else
				{
					function->ErrorCode = functionNavigator->Value;
					hasOverriddenErrorCode = true;
				}
			}
			else if (functionNavigator->Name->CompareTo("OverrideReturnValue") == 0)
			{
				if (functionNavigator->Value->CompareTo("No Change") == 0)
				{
					function->PassThroughReturnValue = true;
				}
				else
				{
					function->ReturnValue = functionNavigator->Value;
					hasOverriddenReturnValue = true;
				}
			}

			hasMoreAttributes = functionNavigator->MoveToNextAttribute();
		}
		//ensure 'no change' is only set when PassThrough is true
		if (!function->PassThrough)
		{
			function->PassThroughErrorCode = false;
			function->PassThroughReturnValue = false;
		}


		functionNavigator->MoveToParent();

		hasMoreElements = functionNavigator->MoveToFirstChild();
		while (hasMoreElements)
		{
			if (functionNavigator->Name->CompareTo("CheckResource") == 0)
			{
				FaultFunction::CheckResource checkResource;

				hasMoreAttributes = functionNavigator->MoveToFirstAttribute();
				while (hasMoreAttributes)
				{
					if (functionNavigator->Name->CompareTo("ParamIndex") == 0)
					{
						checkResource.ParamIndex = Convert::ToInt32(functionNavigator->Value);
					}
					else if (functionNavigator->Name->CompareTo("Exists") == 0)
					{
						checkResource.Exists = (FaultFunction::ExistsType) Convert::ToInt32(functionNavigator->Value);
					}

					hasMoreAttributes = functionNavigator->MoveToNextAttribute();
				}

				function->CheckResources->Add(__box(checkResource));
				functionNavigator->MoveToParent();
			}
			else if (functionNavigator->Name->CompareTo("MatchParams") == 0)
			{
				hasMoreParams = functionNavigator->MoveToFirstChild();

				while (hasMoreParams)
				{
					InterceptedFunctionParameter *matchParam = new InterceptedFunctionParameter();
					ParamTestCase * paramTestCase = new ParamTestCase();
					hasMoreAttributes = functionNavigator->MoveToFirstAttribute();

					while (hasMoreAttributes)
					{
						if (functionNavigator->Name->CompareTo("Name") == 0)
						{
							matchParam->Name = functionNavigator->Value;
						}
						else if (functionNavigator->Name->CompareTo("TestOperator") == 0)
						{
							paramTestCase->TestOperator = functionNavigator->Value;
						}
						else if (functionNavigator->Name->CompareTo("TestValue") == 0)
						{
							paramTestCase->TestValue = functionNavigator->Value;
						}
						else if (functionNavigator->Name->CompareTo("CompareAsType") == 0)
						{
							matchParam->CompareAsType = (ParameterType) Convert::ToInt32(functionNavigator->Value);
						}
						else if (functionNavigator->Name->CompareTo("ID") == 0)
						{
							matchParam->ID = Convert::ToInt32(functionNavigator->Value);
						}

						hasMoreAttributes = functionNavigator->MoveToNextAttribute();
					}

					//Add test case to this match param
					matchParam->AddTestCase (paramTestCase);

					//Check if this param is in the list already; if so, add the test case to it
					//otherwise add the param with the test case to the function match list
					bool foundParam = false;
					for (int fp=0; fp < function->MatchParams->Count; fp++)
					{
						InterceptedFunctionParameter * ifp = dynamic_cast <InterceptedFunctionParameter *> (function->MatchParams->Item[fp]);
						if (String::Compare (ifp->Name, matchParam->Name) == 0)
						{
							ifp->AddTestCase (paramTestCase);
							foundParam = true;
						}
					}
					if (!foundParam)
						function->MatchParams->Add(matchParam);

					functionNavigator->MoveToParent();

					hasMoreParams = functionNavigator->MoveToNext();
				}

				functionNavigator->MoveToParent();
			}
			else if (functionNavigator->Name->CompareTo("ChangeParams"))
			{
				hasMoreParams = functionNavigator->MoveToFirstChild();

				while (hasMoreParams)
				{
					InterceptedFunctionParameter *changeParam = new InterceptedFunctionParameter();
					
					hasMoreAttributes = functionNavigator->MoveToFirstAttribute();
					while (hasMoreAttributes)
					{
						if (functionNavigator->Name->CompareTo("Name") == 0)
						{
							changeParam->Name = functionNavigator->Value;
						}
						else if (functionNavigator->Name->CompareTo("TestValue") == 0)
						{
							changeParam->ChangeValue = functionNavigator->Value;
						}
						else if (functionNavigator->Name->CompareTo("CompareAsType") == 0)
						{
							changeParam->CompareAsType = (ParameterType) Convert::ToInt32(functionNavigator->Value);
						}
						else if (functionNavigator->Name->CompareTo("ID") == 0)
						{
							changeParam->ID = Convert::ToInt32(functionNavigator->Value);
						}

						hasMoreAttributes = functionNavigator->MoveToNextAttribute();
					}

					function->ChangeParams->Add(changeParam);
					functionNavigator->MoveToParent();

					hasMoreParams = functionNavigator->MoveToNext();
				}

				functionNavigator->MoveToParent();
			}

			hasMoreElements = functionNavigator->MoveToNext();
		}

		// get back to the function tag
		functionNavigator->MoveToParent();

		if (!hasOverriddenReturnValue)
			function->ReturnValue = returnValue;

		if (!hasOverriddenErrorCode)
			function->ErrorCode = errorCode;

		return function;
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
	bool FaultFunctionDB::LoadDatabase(String *databaseFolder)
	{
		String *xmlFileName = new String("");

		// get the full path to the database file
		if (databaseFolder != NULL)
		{
			if (databaseFolder->EndsWith("\\"))
				xmlFileName = String::Concat(databaseFolder, FAULT_DB_FILE_NAME);
			else
				xmlFileName = String::Concat(databaseFolder, "\\", FAULT_DB_FILE_NAME);
		}
		else
			xmlFileName = FAULT_DB_FILE_NAME;

		// make sure the document is syntactically correct
		ValidateDocument(xmlFileName);

		// clear all old data
		faults->Clear();
		diskFaults->Clear();
		memoryFaults->Clear();
		networkFaults->Clear();
		registryFaults->Clear();
		processFaults->Clear();
		customFaults->Clear();

		// load in the data
		try
		{
			XPathDocument *doc = new XPathDocument(xmlFileName);
			XPathNavigator *navigator = doc->CreateNavigator();

			bool hasMoreFaults = false;
			navigator->MoveToFirstChild();

			// check for comments
			while (navigator->NodeType == XPathNodeType::Comment)
				navigator->MoveToNext();

			if (navigator->HasChildren)
			{
				navigator->MoveToFirstChild();
				hasMoreFaults = true;
			}

			while (hasMoreFaults)
			{
				bool faultAdded = false;
				Fault *fault = ParseFault(navigator);
				
				switch (fault->Type)
				{
				case DiskFault:
					if (!diskFaults->Contains(fault->Name))
					{
						diskFaults->Add(fault->Name, fault);
						faultAdded = true;
					}
					break;
				case MemoryFault:
					if (!memoryFaults->Contains(fault->Name))
					{
						memoryFaults->Add(fault->Name, fault);
						faultAdded = true;
					}
					break;
				case NetworkFault:
					if (!networkFaults->Contains(fault->Name))
					{
						networkFaults->Add(fault->Name, fault);
						faultAdded = true;
					}
					break;
				case RegistryFault:
					if (!registryFaults->Contains(fault->Name))
					{
						registryFaults->Add(fault->Name, fault);
						faultAdded = true;
					}
					break;
				case ProcessFault:
					if (!processFaults->Contains(fault->Name))
					{
						processFaults->Add(fault->Name, fault);
						faultAdded = true;
					}
					break;
				case CustomFault:
					if (!customFaults->Contains(fault->Name))
					{
						customFaults->Add(fault->Name, fault);
						faultAdded = true;
					}
					break;
				}

				if ( (fault != NULL) && faultAdded)
				{
					faultAdded = false;
					if (!faults->Contains(__box(fault->FaultID)))
						faults->Add(__box(fault->FaultID), fault);
				}

				hasMoreFaults = navigator->MoveToNext();
			}
		}
		catch(System::IO::FileNotFoundException *e)
		{
			String *msg = new String("Could not find the fault database file: \"");
			msg->Concat(e->FileName);
			throw new Exception(msg);
		}

		isLoaded = true;
		return true;
	}

	//*************************************************************************
	// Method:		GetFunctionsWithFaultID
	// Description: get an array of FaultFunction objects which have the fault
	//	type and fault id passed in to this function
	//
	// Parameters:
	//	faultID - The faultid for the type of fault to get the functions for
	//
	// Return Value: the array of functions with the specified type and id, null if 
	//	unsuccessful
	//*************************************************************************
	Array *FaultFunctionDB::GetFunctionsWithFaultID(int faultID)
	{
			Fault *fault = dynamic_cast <Fault*> (faults->Item[__box(faultID)]);

			if (fault)
				return fault->FaultFunctions;
			else
				return NULL;
	}

	//*************************************************************************
	// Method:		StringToFaultType
	// Description: Converts a fault type string to the enum
	//
	// Parameters:
	//	faultType - string representing fault type
	//
	// Return Value: a fault type from the enum.  Returns CustomFault if string is unrecognized
	//*************************************************************************
	FaultType FaultFunctionDB::StringToFaultType(String *faultType)
	{
		if (faultType->Equals("disk"))
			return DiskFault;
		else if (faultType->Equals("memory"))
			return MemoryFault;
		else if (faultType->Equals("network"))
			return NetworkFault;
		else if (faultType->Equals("registry"))
			return RegistryFault;
		else if (faultType->Equals("process"))
			return ProcessFault;
		else
			return CustomFault;
	}

	//*************************************************************************
	// Method:		GetFault
	// Description: returns the fault associated with the given ID
	//
	// Parameters:
	//	faultID - The faultid for the fault to return
	//
	// Return Value: a fault object or null on failure
	//*************************************************************************
	Fault *FaultFunctionDB::GetFault(int faultID)
	{
		if (faultID < 0)
			return NULL;

		return dynamic_cast <Fault*> (faults->Item[__box(faultID)]);
	}	
} // namespace