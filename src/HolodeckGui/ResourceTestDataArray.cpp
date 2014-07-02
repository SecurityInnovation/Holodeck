//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ResourceTestDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Sept 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourceTestDataArray.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ResourceTestDataArray
	// Description: Constructor for the ResourceTestDataArray class
	//
	// Parameters:
	//	procID - the process id for the array
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestDataArray::ResourceTestDataArray(int procID)
	{
		processID = procID;
		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		ToggleResourceTestHandler = new ToggleResourceTestDelegate(this, onToggleResourceTest);
		dataEventRegistry->OnToggleResourceTest += ToggleResourceTestHandler;
		DeleteResourceTestHandler = new DeleteResourceTestDelegate(this, onDeleteResourceTest);
		dataEventRegistry->OnDeleteResourceTest += DeleteResourceTestHandler;
		CreateResourceTestHandler = new CreateResourceTestDelegate(this, onCreateResourceTest);
		dataEventRegistry->OnCreateResourceTest += CreateResourceTestHandler;
		ModifyResourceTestHandler = new ModifyResourceTestDelegate(this, onModifyResourceTest);
		dataEventRegistry->OnModifyResourceTest += ModifyResourceTestHandler;
	}

	//*************************************************************************
	// Method:		~ResourceTestDataArray
	// Description: Destructor for the ResourceTestDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestDataArray::~ResourceTestDataArray()
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
	void ResourceTestDataArray::CleanUp()
	{
		dataEventRegistry->OnToggleResourceTest -= ToggleResourceTestHandler;
		dataEventRegistry->OnDeleteResourceTest -= DeleteResourceTestHandler;
		dataEventRegistry->OnCreateResourceTest -= CreateResourceTestHandler;
		dataEventRegistry->OnModifyResourceTest -= ModifyResourceTestHandler;
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateResourceTest
	// Description: Called when a ui pane creates a test
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void ResourceTestDataArray::onCreateResourceTest(DisplayableDataNode *dataNode)
	{
		try
		{
			if (dataNode->processID != processID)
				return;

			Array *threadList;
			ResourceTestDataNode *testDataNode = dynamic_cast <ResourceTestDataNode*> (dataNode);
			ArrayList *list = new ArrayList();

			//make sure we remove any old faults on this resource
			ResourceTestDataNode *deleteTestDataNode = dynamic_cast <ResourceTestDataNode*> (itemList->Item[String::Concat(testDataNode->Name, Convert::ToString(processID))]);
			if (deleteTestDataNode)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = deleteTestDataNode;
				deleteTestDataNode->DeleteDelegate->DynamicInvoke(args);
			}

			//Write details to registry only if test is ON
			if (testDataNode->IsTestOn)
			{
				list->Add(__box(testDataNode->threadID));
				threadList = list->ToArray();
				testDataNode->ID = String::Concat(testDataNode->Name, Convert::ToString(processID));
				testDataNode->DataArray = testDataNode;
				testDataNode->TestIDs = holoScriptApp->InjectResourceFault(threadList, (Int32::Parse(testDataNode->FaultID)), testDataNode->Name);
			}
			else
			{
				testDataNode->ID = String::Concat(testDataNode->Name, Convert::ToString(processID));
			}

			CreateDataNode(testDataNode);
			if (testDataNode->CreatedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = testDataNode;
				testDataNode->CreatedDelegate->DynamicInvoke(args);
			}

			hasCreatedOneTest = true;
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		onDeleteResourceTest
	// Description: Called when a ui pane deletes a test
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void ResourceTestDataArray::onDeleteResourceTest(DisplayableDataNode *dataNode)
	{
		Array *threadList;
		ResourceTestDataNode *testDataNode = dynamic_cast <ResourceTestDataNode*> (dataNode->DataArray);

		if (dataNode->processID != processID)
			return;

		if (testDataNode && testDataNode->TestIDs)
		{
			ArrayList *list = new ArrayList();
			list->Add(__box(testDataNode->threadID));
			threadList = list->ToArray();
			holoScriptApp->RemoveResourceFault(threadList, testDataNode->TestIDs);
			DeleteDataNode(testDataNode);

			if (testDataNode->DeletedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = testDataNode;
				testDataNode->DeletedDelegate->DynamicInvoke(args);
			}
		}
	}

	//*************************************************************************
	// Method:		onToggleResourceTest
	// Description: called when a test state is toggled
	//
	// Parameters:
	//	dataNode - node being modified
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestDataArray::onToggleResourceTest(DisplayableDataNode *dataNode)
	{
		ResourceTestDataNode *rtestDataNode = dynamic_cast<ResourceTestDataNode *>(dataNode);
		if (!rtestDataNode)
			return;

		String *testID = dataNode->ID;
		int splitPos = dataNode->Name->IndexOf(" - ");
		if (splitPos != -1)
			testID = dataNode->Name->Substring(splitPos + 3);

		ArrayList *list = new ArrayList();
		list->Add(__box(rtestDataNode->threadID));

		if (!rtestDataNode->IsTestOn)
		{
			holoScriptApp->RemoveResourceFault(list->ToArray(), rtestDataNode->TestIDs);
		}
		else
		{
			rtestDataNode->TestIDs = holoScriptApp->InjectResourceFault(list->ToArray(), (Int32::Parse(rtestDataNode->FaultID)), rtestDataNode->Name);
		}
	}

	//*************************************************************************
	// Method:		onModifyResourceTest
	// Description: called when a test node is modified
	//
	// Parameters:
	//	dataNode - node that will be modified
	//	modifiedDataNode - node that contains the modification
	//	propertyName - name of the property that was modified
	//
	// Return Value: true if succesfull, false if not
	//*************************************************************************
	void ResourceTestDataArray::onModifyResourceTest(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		if (dataNode->processID != processID)
			return;

		onDeleteResourceTest(dataNode);
		onCreateResourceTest(modifiedDataNode);
	}
}