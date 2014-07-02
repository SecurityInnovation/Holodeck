//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultTreeView.cpp
//
// DESCRIPTION: Contains implementation for the class FaultTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include "FaultTreeView.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FaultTreeView
	// Description: Constructor for the FaultTreeView class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	FaultTreeView::FaultTreeView()
	{
		this->ImageIndex = -1;
		this->Location = System::Drawing::Point(8, 32);
		this->Name = "faultTreeView";
		this->SelectedImageIndex = -1;
		this->HideSelection = false;
		this->CheckBoxes = false;
		this->faultTreeView_AfterSelect_Handler = new TreeViewEventHandler(this, faultTreeView_AfterSelect);
		this->add_AfterSelect (faultTreeView_AfterSelect_Handler);

		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
		IList *diskFaults = faultDB->DiskFaults;
		IList *registryFaults = faultDB->RegistryFaults;
		IList *processFaults = faultDB->ProcessFaults;
		IList *customFaults = faultDB->CustomFaults;

		if (diskFaults->Count > 0)
		{
			diskNode = Nodes->Add("Disk Faults");
			for (int i=0; i<diskFaults->Count; i++)
			{
				Fault *fault = dynamic_cast <Fault*> (diskFaults->Item[i]);
				diskNode->Nodes->Add(fault->Name);
			}
		}

		if (registryFaults->Count > 0)
		{
			registryNode = Nodes->Add("Registry Faults");
			for (int i=0; i<registryFaults->Count; i++)
			{
				Fault *fault = dynamic_cast <Fault*> (registryFaults->Item[i]);
				registryNode->Nodes->Add(fault->Name);
			}
		}

		if (processFaults->Count > 0)
		{
			processNode = Nodes->Add("Process/Library Faults");
			for (int i=0; i<processFaults->Count; i++)
			{
				Fault *fault = dynamic_cast <Fault*> (processFaults->Item[i]);
				processNode->Nodes->Add(fault->Name);
			}
		}

		if (customFaults->Count > 0)
		{
			customNode = Nodes->Add("Custom Faults");
			for (int i=0; i<customFaults->Count; i++)
			{
				Fault *fault = dynamic_cast <Fault*> (customFaults->Item[i]);
				customNode->Nodes->Add(fault->Name);
			}
		}
	}


	//*************************************************************************
	// Method:		~FaultTreeView
	// Description: Destructor for the FaultTreeView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultTreeView::~FaultTreeView()
	{
	}

	//*************************************************************************
	// Method:		faultTreeView_AfterSelect
	// Description: Called when a tree view item is clicked
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FaultTreeView::faultTreeView_AfterSelect(Object* sender, TreeViewEventArgs* e)
	{
		if (OnFaultClicked && (e->Node != diskNode) && (e->Node != registryNode) && (e->Node != processNode) && (e->Node != customNode) )
			OnFaultClicked->Invoke(e->Node->Text);
		faultType = GetFaultType(e->Node);
	}

	//*************************************************************************
	// Method:		GetFaultType
	// Description: Returns fault type of the given node
	//
	// Parameters:
	//	treeNode - node to check fault type for
	//
	// Return Value: the fault type of the node
	//*************************************************************************
	FunctionDatabase::FaultType FaultTreeView::GetFaultType(TreeNode *treeNode)
	{
		if (registryNode->Nodes->Contains(treeNode))
			return RegistryFault;
		else if (diskNode->Nodes->Contains(treeNode))
			return DiskFault;
		else if (processNode->Nodes->Contains(treeNode))
			return ProcessFault;
		else 
			return CustomFault;
	}

	//*************************************************************************
	// Method:		GetNodeWithValue
	// Description: Gets the child node with the value specified
	//
	// Parameters:
	//	faultName - the fault name to look for
	//	faultType - the fault type to look for
	//
	// Return Value: the tree node if found, NULL otherwise
	//*************************************************************************
	TreeNode *FaultTreeView::GetNodeWithValue(String *faultName, FunctionDatabase::FaultType faultType)
	{
		TreeNode *faultTypeNode, *faultNode;
		
		if (!faultName)
			return 0;

		switch (faultType)
		{
		case DiskFault:
			faultTypeNode = diskNode;
			break;
		case ProcessFault:
			faultTypeNode = processNode;
			break;
		case RegistryFault:
			faultTypeNode = registryNode;
			break;
		case CustomFault:
			faultTypeNode = customNode;
			break;
		default:
			return 0;
		}

		for (int j=0; j<faultTypeNode->Nodes->Count; j++)
		{
			faultNode = faultTypeNode->Nodes->Item[j];
			if (faultNode->Text->CompareTo(faultName) == 0)
				return faultNode;
		}
		
		return 0;
	}

	//*************************************************************************
	// Method:		SelectNodeWithValue
	// Description: Selects the tree node with the corresponding value
	//
	// Parameters:
	//	faultName - the fault name to look for
	//	faultType - the fault type to look for
	//
	// Return Value: true if found, false otherwise
	//*************************************************************************
	bool FaultTreeView::SelectNodeWithValue(String *faultName, FunctionDatabase::FaultType faultType)
	{
		TreeNode *nodeToSelect = GetNodeWithValue(faultName, faultType);
		if (nodeToSelect)
		{
			this->ExpandAll();
			this->SelectedNode = nodeToSelect;
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		set_FaultType
	// Description: Sets the fault type for the treeview
	//
	// Parameters:
	//	value - faulttype to set
	//
	// Return Value: none
	//*************************************************************************
	void FaultTreeView::set_FaultType(FunctionDatabase::FaultType value)
	{
		faultType = value;
		if (Nodes->Contains(registryNode))
			Nodes->Remove(registryNode);
		if (Nodes->Contains(processNode))
			Nodes->Remove(processNode);
		if (Nodes->Contains(diskNode))
			Nodes->Remove(diskNode);
		if (customNode && Nodes->Contains(customNode))
			Nodes->Remove(customNode);

		switch (value)
		{
		case FunctionDatabase::DiskFault:
			Nodes->Add(diskNode);
			break;
		case FunctionDatabase::RegistryFault:
			Nodes->Add(registryNode);
			break;
		case FunctionDatabase::ProcessFault:
			Nodes->Add(processNode);
			break;
		default:
			break;
		}

		if (customNode)
			Nodes->Add(customNode);
	}

}
