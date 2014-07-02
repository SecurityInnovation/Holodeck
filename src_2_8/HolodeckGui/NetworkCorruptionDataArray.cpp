//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "RegistrationMgr.h"
#include "NetworkCorruptionDataArray.h"

using namespace HolodeckRegistration;

namespace UIData
{
	//*************************************************************************
	// Method:		NetworkCorruptionDataArray
	// Description: Constructor for the NetworkCorruptionDataArray class
	//
	// Parameters:
	//	procID - the process id for the array
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionDataArray::NetworkCorruptionDataArray(unsigned int procID)
	{
		processID = procID;
		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		ToggleNetworkCorruptionHandler = new ToggleNetworkCorruptionDelegate(this, onToggleNetworkCorruption);
		dataEventRegistry->OnToggleNetworkCorruption += ToggleNetworkCorruptionHandler;
		DeleteNetworkCorruptionHandler = new DeleteNetworkCorruptionDelegate(this, onDeleteNetworkCorruption);
		dataEventRegistry->OnDeleteNetworkCorruption += DeleteNetworkCorruptionHandler;
		CreateNetworkCorruptionHandler = new CreateNetworkCorruptionDelegate(this, onCreateNetworkCorruption);
		dataEventRegistry->OnCreateNetworkCorruption += CreateNetworkCorruptionHandler;
		ModifyNetworkCorruptionHandler = new ModifyNetworkCorruptionDelegate(this, onModifyNetworkCorruption);
		dataEventRegistry->OnModifyNetworkCorruption += ModifyNetworkCorruptionHandler;

	}

	//*************************************************************************
	// Method:		~NetworkCorruptionDataArray
	// Description: Destructor for the NetworkCorruptionDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionDataArray::~NetworkCorruptionDataArray()
	{
		CleanUp();
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
	void NetworkCorruptionDataArray::CleanUp()
	{
		dataEventRegistry->OnModifyNetworkCorruption -= ModifyNetworkCorruptionHandler;
		dataEventRegistry->OnToggleNetworkCorruption -= ToggleNetworkCorruptionHandler;
		dataEventRegistry->OnDeleteNetworkCorruption -= DeleteNetworkCorruptionHandler;
		dataEventRegistry->OnCreateNetworkCorruption -= CreateNetworkCorruptionHandler;
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateNetworkCorruption
	// Description: Called when a ui pane creates a fault
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void NetworkCorruptionDataArray::onCreateNetworkCorruption(DisplayableDataNode *dataNode)
	{
		Object *args[] = __gc new Object*[1];
		NetworkCorruptionDataNode *ncDataNode;
		String *testID;

		if (!dataNode->CreatedDelegate)
			return;
		else if (dataNode->processID == processID)
		{
			ncDataNode = dynamic_cast<NetworkCorruptionDataNode *>(dataNode);
			if (!ncDataNode)
				return;

			//Write details to registry only if test is ON
			if (ncDataNode->IsTestOn)
			{
				holoScriptApp->CreateNetworkCorruptionFault(ncDataNode->SentData, ncDataNode->ReceivedData,
					ncDataNode->PortList->ToArray(), ncDataNode->SearchRegularExpression, ncDataNode->ReplacementString, &testID);
				
				ncDataNode->Name = String::Concat(ncDataNode->PortString, " - ", ncDataNode->CorruptType);
				ncDataNode->ID = testID;
			}
			else
			{
				ncDataNode->Name = String::Concat(ncDataNode->PortString, " - ", ncDataNode->CorruptType);
				ncDataNode->ID = holoScriptApp->GetUUID();
			}

			CreateDataNode(ncDataNode);
			args[0] = ncDataNode;
			ncDataNode->CreatedDelegate->DynamicInvoke(args);

			hasCreatedOneNCFault = true;
		}
	}

	//*************************************************************************
	// Method:		onDeleteNetworkCorruption
	// Description: Called when a ui pane deletes a fault
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void NetworkCorruptionDataArray::onDeleteNetworkCorruption(DisplayableDataNode *dataNode)
	{
		if (!dataNode->DeletedDelegate)
			return;
		else
		{
			String *testID = dataNode->ID;

			holoScriptApp->DeleteNetworkCorruptionFault(testID);
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);
			DeleteDataNode(dataNode);
		}
	}

	//*************************************************************************
	// Method:		onToggleNetworkCorruption
	// Description: called when a test state is toggled
	//
	// Parameters:
	//	dataNode - node being modified
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionDataArray::onToggleNetworkCorruption(DisplayableDataNode *dataNode)
	{
		NetworkCorruptionDataNode *ncDataNode = dynamic_cast<NetworkCorruptionDataNode *>(dataNode);
		if (!ncDataNode)
			return;

		String *testID = dataNode->ID;

		if (!ncDataNode->IsTestOn)
		{
			holoScriptApp->DeleteNetworkCorruptionFault(testID);
		}
		else
		{
			holoScriptApp->CreateNetworkCorruptionFault(ncDataNode->SentData, ncDataNode->ReceivedData,
				ncDataNode->PortList->ToArray(), ncDataNode->SearchRegularExpression, ncDataNode->ReplacementString, &testID);
			
			ncDataNode->Name = String::Concat(ncDataNode->PortString, " - ", ncDataNode->CorruptType);
			ncDataNode->ID = testID;
		}
	}

	//*************************************************************************
	// Method:		onModifyNetworkCorruption
	// Description: Called when a ui pane modifies a fault
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void NetworkCorruptionDataArray::onModifyNetworkCorruption(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		Object *args[] = __gc new Object*[1];
		NetworkCorruptionDataNode *ncDataNode;
		String *testID;

		NetworkCorruptionDataNode *originalNode = dynamic_cast<NetworkCorruptionDataNode *>
			(GetDataNodeByID(dataNode->ID));

		if ((!dataNode->ModifiedDelegate) || (!originalNode))
			return;
		else if (dataNode->processID == processID)
		{
			ncDataNode = dynamic_cast<NetworkCorruptionDataNode *>(modifiedDataNode);
			if (!ncDataNode)
				return;

			holoScriptApp->DeleteNetworkCorruptionFault(dataNode->ID);
			this->DeleteDataNode(dataNode);
			if (dataNode->DeletedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeletedDelegate->DynamicInvoke(args);
			}

			holoScriptApp->CreateNetworkCorruptionFault(ncDataNode->SentData, ncDataNode->ReceivedData,
				ncDataNode->PortList->ToArray(), ncDataNode->SearchRegularExpression, ncDataNode->ReplacementString, &testID);
			
			ncDataNode->Name = String::Concat(ncDataNode->PortString, " - ", ncDataNode->CorruptType);
			ncDataNode->ID = testID;

			CreateDataNode(ncDataNode);
			args[0] = ncDataNode;
			ncDataNode->CreatedDelegate->DynamicInvoke(args);
		}
	}
}