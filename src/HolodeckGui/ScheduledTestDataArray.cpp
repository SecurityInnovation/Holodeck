//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ScheduledTestDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ScheduledTestDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "ScheduledTestDataArray.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ScheduledTestDataArray
	// Description: Constructor for the ScheduledTestDataArray class
	//
	// Parameters:
	//	procID - the process id for the array
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestDataArray::ScheduledTestDataArray(int procID)
	{
		processID = procID;
		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		ToggleTestHandler = new ToggleTestDelegate(this, onToggleTest);
		dataEventRegistry->OnToggleTest += ToggleTestHandler;
		DeleteTestHandler = new DeleteTestDelegate(this, onDeleteTest);
		dataEventRegistry->OnDeleteTest += DeleteTestHandler;
		CreateTestHandler = new CreateTestDelegate(this, onCreateTest);
		dataEventRegistry->OnCreateTest += CreateTestHandler;
		ModifyTestHandler = new ModifyTestDelegate(this, onModifyTest);
		dataEventRegistry->OnModifyTest += ModifyTestHandler;
	}

	//*************************************************************************
	// Method:		~ScheduledTestDataArray
	// Description: Destructor for the ScheduledTestDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestDataArray::~ScheduledTestDataArray()
	{
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Cleans up all dataarray delegates
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ScheduledTestDataArray::CleanUp()
	{
		dataEventRegistry->OnToggleTest -= ToggleTestHandler;
		dataEventRegistry->OnDeleteTest -= DeleteTestHandler;
		dataEventRegistry->OnCreateTest -= CreateTestHandler;
		dataEventRegistry->OnModifyTest -= ModifyTestHandler;
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateTest
	// Description: Called when a ui pane creates a test
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void ScheduledTestDataArray::onCreateTest(DisplayableDataNode *dataNode)
	{
		Object *args[] = __gc new Object*[1];
		ScheduledTestDataNode *testDataNode;
		ArrayList *newThreadList = new ArrayList();
		FaultFunction *functionTest = new FaultFunction();

		if (!dataNode->CreatedDelegate)
			return;
		else if (dataNode->processID == processID)
		{
			testDataNode = dynamic_cast<ScheduledTestDataNode *>(dataNode);
			if (!testDataNode)
				return;
			
			//Write details to registry only if test is ON
			if (testDataNode->IsTestOn)
			{
				if (testDataNode->threadID != 0)
					newThreadList->Add(__box(testDataNode->threadID));

				//if this is a parameter value array, convert to interceptedfunctionparameter array
				if ( (testDataNode->MatchParameters) && (testDataNode->MatchParameters->Count > 0) && (testDataNode->MatchParameters->get_Item(0)->GetType()->Name->CompareTo("String") == 0) )
					testDataNode->MatchParameters = ConvertToParameterArray(testDataNode->MatchParameters, testDataNode->Function, true);

				if ( (testDataNode->ChangeParameters) && (testDataNode->ChangeParameters->Count > 0) && (testDataNode->ChangeParameters->get_Item(0)->GetType()->Name->CompareTo("String") == 0) )
					testDataNode->ChangeParameters = ConvertToParameterArray(testDataNode->ChangeParameters, testDataNode->Function, false);

				functionTest->Name = testDataNode->Function;
				functionTest->ReturnValue = testDataNode->ReturnValue;
				functionTest->ErrorCode = testDataNode->ErrorCode;
				functionTest->MatchParams = testDataNode->MatchParameters;
				functionTest->ChangeParams = testDataNode->ChangeParameters;
				functionTest->FiringOption = testDataNode->FiringOption;
				functionTest->PauseAppAfterTestFire = testDataNode->PauseAppOnTestFire;
				functionTest->FiringDetails = testDataNode->FiringDetails;
				
				holoScriptApp->CreateScheduledTest(newThreadList->ToArray(), testDataNode->Category, functionTest);
				
				testDataNode->TestID = functionTest->TestID;
				testDataNode->Name = String::Concat(testDataNode->Function);
				testDataNode->ID = functionTest->TestID;
			}
			else
			{
				testDataNode->Name = String::Concat(testDataNode->Function);
			}

			CreateDataNode(testDataNode);
			args[0] = testDataNode;
			testDataNode->CreatedDelegate->DynamicInvoke(args);

			hasCreatedOneTest = true;
		}
	}

	//*************************************************************************
	// Method:		onDeleteTest
	// Description: Called when a ui pane deletes a test
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void ScheduledTestDataArray::onDeleteTest(DisplayableDataNode *dataNode)
	{
		if (!dataNode->DeletedDelegate)
			return;
		else
		{
			String *testID = dataNode->ID;
			int splitPos = dataNode->Name->IndexOf(" - ");
			if (splitPos != -1)
				testID = dataNode->Name->Substring(splitPos + 3);

			ArrayList *threadList = new ArrayList();
			if (dataNode->threadID != 0)
				threadList->Add(__box(dataNode->threadID));

			holoScriptApp->DeleteScheduledTest(threadList->ToArray(), testID);
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);
			DeleteDataNode(dataNode);
		}
	}

	//*************************************************************************
	// Method:		onToggleTest
	// Description: called when a test state is toggled
	//
	// Parameters:
	//	dataNode - node being modified
	//
	// Return Value: None
	//*************************************************************************
	void ScheduledTestDataArray::onToggleTest(DisplayableDataNode *dataNode)
	{
		ScheduledTestDataNode *testDataNode = dynamic_cast<ScheduledTestDataNode *>(dataNode);
		if (!testDataNode)
			return;

		String *testID = dataNode->ID;
		int splitPos = dataNode->Name->IndexOf(" - ");
		if (splitPos != -1)
			testID = dataNode->Name->Substring(splitPos + 3);

		ArrayList *threadList = new ArrayList();
		if (dataNode->threadID != 0)
			threadList->Add(__box(dataNode->threadID));

		if (!testDataNode->IsTestOn)
		{
			holoScriptApp->DeleteScheduledTest(threadList->ToArray(), testID);
		}
		else
		{
			FaultFunction *functionTest = new FaultFunction();
			
			//if this is a parameter value array, convert to interceptedfunctionparameter array
			if ( (testDataNode->MatchParameters) && (testDataNode->MatchParameters->Count > 0) && (testDataNode->MatchParameters->get_Item(0)->GetType()->Name->CompareTo("String") == 0) )
				testDataNode->MatchParameters = ConvertToParameterArray(testDataNode->MatchParameters, testDataNode->Function, true);

			if ( (testDataNode->ChangeParameters) && (testDataNode->ChangeParameters->Count > 0) && (testDataNode->ChangeParameters->get_Item(0)->GetType()->Name->CompareTo("String") == 0) )
				testDataNode->ChangeParameters = ConvertToParameterArray(testDataNode->ChangeParameters, testDataNode->Function, false);

			functionTest->TestID = testDataNode->TestID;
			functionTest->Name = testDataNode->Function;
			functionTest->ReturnValue = testDataNode->ReturnValue;
			functionTest->ErrorCode = testDataNode->ErrorCode;
			functionTest->MatchParams = testDataNode->MatchParameters;
			functionTest->ChangeParams = testDataNode->ChangeParameters;
			functionTest->FiringOption = testDataNode->FiringOption;
			functionTest->PauseAppAfterTestFire = testDataNode->PauseAppOnTestFire;
			functionTest->FiringDetails = testDataNode->FiringDetails;
			holoScriptApp->CreateScheduledTest(threadList->ToArray(), testDataNode->Category, functionTest);
		}
	}

	//*************************************************************************
	// Method:		onModifyTest
	// Description: called when a test node is modified
	//
	// Parameters:
	//	dataNode - node that will be modified
	//	modifiedDataNode - node that contains the modification
	//	propertyName - name of the property that was modified
	//
	// Return Value: None
	//*************************************************************************
	void ScheduledTestDataArray::onModifyTest(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		Object *args[] = __gc new Object*[3];
		ScheduledTestDataNode *testDataNode;
		ArrayList *newThreadList = new ArrayList();
		ArrayList *oldThreadList = new ArrayList();
		ScheduledTestDataNode *originalNode = dynamic_cast<ScheduledTestDataNode *>(GetDataNodeByID(dataNode->ID));
		FaultFunction *functionTest = new FaultFunction();

		if ( (!dataNode->ModifiedDelegate) || (!originalNode) )
			return;
		else if (dataNode->processID == processID)
		{
			testDataNode = dynamic_cast<ScheduledTestDataNode *>(modifiedDataNode);
			if (!testDataNode)
				return;
			
			if (originalNode->threadID != 0)
				oldThreadList->Add(__box(originalNode->threadID));

			if (testDataNode->threadID != 0)
				newThreadList->Add(__box(testDataNode->threadID));

			holoScriptApp->DeleteScheduledTest(oldThreadList->ToArray(), dataNode->ID);
			this->DeleteDataNode(dataNode);
			if (dataNode->DeletedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeletedDelegate->DynamicInvoke(args);
			}

			functionTest->Name = testDataNode->Function;
			functionTest->ReturnValue = testDataNode->ReturnValue;
			functionTest->ErrorCode = testDataNode->ErrorCode;
			functionTest->MatchParams = testDataNode->MatchParameters;
			functionTest->ChangeParams = testDataNode->ChangeParameters;
			functionTest->FiringOption = testDataNode->FiringOption;
			functionTest->FiringDetails = testDataNode->FiringDetails;
			functionTest->PauseAppAfterTestFire = testDataNode->PauseAppOnTestFire;
			holoScriptApp->CreateScheduledTest(newThreadList->ToArray(), testDataNode->Category, functionTest);

			testDataNode->TestID = functionTest->TestID;
			testDataNode->Name = String::Concat(testDataNode->Function);
			testDataNode->ID = functionTest->TestID;
			this->CreateDataNode(testDataNode);
			if (dataNode->CreatedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = testDataNode;
				dataNode->CreatedDelegate->DynamicInvoke(args);
			}
		}
	}

	//*************************************************************************
	// Method:		ConvertToParameterArray
	// Description: Converts a value array to a interceptedfunctionparamaterarray
	//				
	//
	// Parameters:
	//	parameters - parameter value array
	//	functioName - name of the function these parameters will be passed to
	//  matchParameters - true if we are converting match parameters, false if we are converting change parameters
	//
	// Return Value: An array of InterceptedFunctionParameter types
	//*************************************************************************
	ArrayList *ScheduledTestDataArray::ConvertToParameterArray(ArrayList *parameters, String *functionName, bool matchParameters)
	{
		ArrayList *parameterArray = new ArrayList(parameters);
		try
		{
			interceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			InterceptedFunction *interceptedFunction = interceptedFunctionDB->GetFunctionByName(functionName);

			if (interceptedFunction == NULL)
			{
				// Try the .NET database
				interceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
				interceptedFunction = interceptedFunctionDB->GetFunctionByName(functionName);
			}

			Array *interceptedParameters = interceptedFunction->Parameters;

			for (int i=0, j=0; i<interceptedParameters->Length; i++, j++)
			{
				String *strParamValue = dynamic_cast <String*> (parameterArray->get_Item(j));
				if (!strParamValue)
				{
					parameterArray = NULL;
					continue;
				}
										
				InterceptedFunctionParameter *param = dynamic_cast <InterceptedFunctionParameter*> (interceptedParameters->GetValue(i));
				param->RemoveTestAllCases();

				if ( (matchParameters) & (param->Access->CompareTo("OUT") == 0) )
				{
					parameterArray->Remove(strParamValue);
					j--;
				}
				else if ( (!matchParameters) & (param->Access->CompareTo("IN") == 0) )
				{
					parameterArray->Remove(strParamValue);
					j--;
				}
				else
				{
					//If Match Parameters are wanted then add match params to the structure
					if (matchParameters)
						param->AddTestCase (new ParamTestCase (strParamValue, S"="));
					//else add the change value to the structure
					else
						param->ChangeValue = strParamValue;

					parameterArray->set_Item(j, param);
				}
			}
		}
		catch (...)
		{
			parameterArray = NULL;
		}

		return parameterArray;
	}
}