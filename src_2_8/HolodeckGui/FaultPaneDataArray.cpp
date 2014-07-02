//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultPaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class FaultPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include "FaultPaneDataArray.h"

namespace UIData
{

	//*************************************************************************
	// Method:		FaultPaneDataArray
	// Description: Constructor for the FaultPaneDataArray class
	//
	// Parameters:
	//	procID - The process id associated with these faults
	//
	// Return Value: None
	//*************************************************************************
	FaultPaneDataArray::FaultPaneDataArray(unsigned int procID)
	{
		processID = procID;
		internalDelete = false;

		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		DeleteFaultHandler = new DeleteFaultDelegate(this, onDeleteFault);
		dataEventRegistry->OnDeleteFault += DeleteFaultHandler;
		CreateFaultHandler = new CreateFaultDelegate(this, onCreateFault);
		dataEventRegistry->OnCreateFault += CreateFaultHandler;
	}

	//*************************************************************************
	// Method:		FaultPaneDataArray
	// Description: Destructor for the FaultPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultPaneDataArray::~FaultPaneDataArray()
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
	void FaultPaneDataArray::CleanUp()
	{
		dataEventRegistry->OnDeleteFault -= DeleteFaultHandler;
		dataEventRegistry->OnCreateFault -= CreateFaultHandler;
		DisplayableDataArray::CleanUp();
	}
	//*************************************************************************
	// Method:		onCreateFault
	// Description: Called when a ui pane creates a fault
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void FaultPaneDataArray::onCreateFault(DisplayableDataNode *dataNode)
	{
		FaultPaneDataNode *faultNode = dynamic_cast <FaultPaneDataNode*> (dataNode);
		if (!faultNode)
			return;

		//retain a copy in dataarray for use when project pane makes a datanode copy
		faultNode->DataArray = faultNode;

		if (!faultNode->CreatedDelegate)
			return;
		else if (faultNode->processID == processID)
		{
			FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
			IList *faults = faultDB->Faults;

			for (int i=0; i<faults->Count; i++)
			{
				Fault *fault = dynamic_cast <Fault*> (faults->Item[i]);
				
				if (FaultsMatch(faultNode, fault))
				{
					faultNode->FaultID = fault->FaultID;
					faultNode->ID = String::Concat(faultNode->processID.ToString(), faultNode->threadID.ToString(), faultNode->FaultID.ToString());
					break;
				}
			}

			if (faultNode->FaultID == -1)
				return;

			//if we are switching to a new fault, then delete the old one.
			if (FindFaultNode(faultNode->Type, faultNode->threadID))
			{
				internalDelete = true;
				onDeleteFault(faultNode);
				internalDelete = false;
			}			

			if (!this->itemList->Contains(faultNode->ID))
			{
				ArrayList *list = new ArrayList();
				list->Add(__box(faultNode->threadID));
				Array *threadList = list->ToArray();
				holoScriptApp->InjectFault(threadList, faultNode->FaultID);
				CreateDataNode(faultNode);
				Object *args[] = __gc new Object*[1];
				args[0] = faultNode;
				faultNode->CreatedDelegate->DynamicInvoke(args);
			}
			else
				return;

		}
	}

	//*************************************************************************
	// Method:		onDeleteFault
	// Description: Called when a ui pane deletes a fault
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void FaultPaneDataArray::onDeleteFault(DisplayableDataNode *dataNode)
	{
		FaultPaneDataNode *faultNode = dynamic_cast <FaultPaneDataNode*> (dataNode);
		if (!faultNode)
		{
			faultNode = dynamic_cast <FaultPaneDataNode*> (dataNode->DataArray);
			if (!faultNode)
				return;
		}

		if (!faultNode->DeletedDelegate)
			return;
		else if (faultNode->processID == processID)
		{
			switch (faultNode->Type)
			{
			case UIData::DiskFault:
			case UIData::MemoryFault:
			case UIData::NetworkFault:
			case UIData::RegistryFault:
			case UIData::ProcessFault:
			case UIData::CustomFault:
				faultNode = FindFaultNode(faultNode->Type, faultNode->threadID);
				if (!faultNode)
					return;
				break;
			}

			ArrayList *list = new ArrayList();
			list->Add(__box(faultNode->threadID));
			Array *threadList = list->ToArray();
			holoScriptApp->RemoveFault(threadList, faultNode->FaultID);
			if (internalDelete)
				this->itemList->Remove(faultNode->ID);
			else
				DeleteDataNode(faultNode);
			Object *args[] = __gc new Object*[1];
			args[0] = faultNode;
			faultNode->DeletedDelegate->DynamicInvoke(args);
		}
	}

	//*************************************************************************
	// Method:		FindFaultNode
	// Description: Find the fault node of a given type, only should be one per type
	//
	// Parameters:
	//	type - the type to find
	//	threadID - threadID to match
	//
	// Return Value: datanode that was found, null if nothing found
	//*************************************************************************
	FaultPaneDataNode *FaultPaneDataArray::FindFaultNode(NodeType type, unsigned int threadID)
	{
		FaultPaneDataNode *faultNode;

		for (int i=0; i < this->Count; i++)
		{
			faultNode = dynamic_cast <FaultPaneDataNode*> (this->GetDataNodeByIndex(i));
			if ( (faultNode) && (faultNode->Type == type) && (faultNode->threadID == threadID) )
				return faultNode;
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		FaultsMatch
	// Description: Check if a fault and fault node object correspond to the same fault
	//
	// Parameters:
	//	faultNode - fault node to check
	//	fault - fault object to check
	//
	// Return Value: return true if they match, false otherwise
	//*************************************************************************
	bool FaultPaneDataArray::FaultsMatch(FaultPaneDataNode *faultNode, Fault *fault)
	{
		bool retVal = false;

		switch (faultNode->Type)
		{
		case NodeType::MemoryFault:
			if ( (fault->Type == FaultType::MemoryFault) && fault->Name->Equals(faultNode->Name))
				retVal = true;
			break;
		case NodeType::DiskFault:
			if ( (fault->Type == FaultType::DiskFault) && fault->Name->Equals(faultNode->Name))
				retVal = true;
			break;
		case NodeType::NetworkFault:
			if ( (fault->Type == FaultType::NetworkFault) && fault->Name->Equals(faultNode->Name))
				retVal = true;
			break;
		case NodeType::RegistryFault:
			if ( (fault->Type == FaultType::RegistryFault) && fault->Name->Equals(faultNode->Name))
				retVal = true;
			break;
		case NodeType::ProcessFault:
			if ( (fault->Type == FaultType::ProcessFault) && fault->Name->Equals(faultNode->Name))
				retVal = true;
			break;
		case NodeType::CustomFault:
			if ( (fault->Type == FaultType::CustomFault) && fault->Name->Equals(faultNode->Name))
				retVal = true;
			break;
		}

		return retVal;
	}

}