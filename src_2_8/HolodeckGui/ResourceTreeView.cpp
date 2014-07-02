//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTreeView.cpp
//
// DESCRIPTION: Contains implementation for the class ResourceTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include "ResourceTreeView.h"
#include "DisplayableDataNode.h"

#define EDIT_TEXT "<Double-click here to add a new resource path...>"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ResourceTreeView
	// Description: Constructor for the ResourceTreeView class
	//
	// Parameters:
	//	resourceDataArray - contains all the resource information to populate the tree with
	//
	// Return Value: None
	//*************************************************************************
	ResourceTreeView::ResourceTreeView(bool checkBoxes, bool Expand)
	{
		registryHash = new Hashtable();
		fileHash = new Hashtable();
		processHash = new Hashtable();
		comHash = new Hashtable();
		checkedResourcePaths = new ArrayList();
		guiThreadId = GetCurrentThreadId();
		timeoutCount = 0;

		expand = Expand;

		this->ImageIndex = -1;
		this->Location = System::Drawing::Point(8, 32);
		this->Name = "resourceTreeView";
		this->SelectedImageIndex = -1;
		this->CheckBoxes = checkBoxes;
		this->LabelEdit = true;
		this->HideSelection = false;
		this->resourceTreeView_Checked_Handler = new TreeViewEventHandler (this, resourceTreeView_Checked);
		this->add_AfterCheck(resourceTreeView_Checked_Handler);
		this->resourceTreeView_AfterSelect_Handler = new TreeViewEventHandler(this, resourceTreeView_AfterSelect);
		this->add_AfterSelect(resourceTreeView_AfterSelect_Handler);
		this->add_BeforeLabelEdit(new NodeLabelEditEventHandler(this, resourceTreeView_BeforeLabelEdit));
		this->add_AfterLabelEdit(new NodeLabelEditEventHandler(this, resourceTreeView_AfterLabelEdit));
		this->add_DoubleClick(new EventHandler(this, resourceTreeView_DoubleClick));
	}


	//*************************************************************************
	// Method:		~ResourceTreeView
	// Description: Destructor for the ResourceTreeView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTreeView::~ResourceTreeView()
	{
	}

	//*************************************************************************
	// Method:		RemoveChecks
	// Description: Recursively removes checks from a treenode and all its children
	//
	// Parameters:
	//	treeNode - node to remove checks from
	//	leaveCheckedNode - don't uncheck this one
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::RemoveChecks(TreeNode *treeNode, TreeNode *leaveCheckedNode)
	{
		if (treeNode->Checked && (treeNode != leaveCheckedNode) )
			treeNode->Checked = false;

		for (int i=0; i<treeNode->Nodes->Count; i++)
			RemoveChecks(treeNode->Nodes->Item[i], leaveCheckedNode);
	}

	//*************************************************************************
	// Method:		RemoveAllChecks
	// Description: Remove all checks in the treeview
	//
	// Parameters:
	//	leaveCheckedNode - the one node not to uncheck
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::RemoveAllChecks(TreeNode *leaveCheckedNode)
	{
		for (int i=0; i<Nodes->Count; i++)
			RemoveChecks(Nodes->Item[i], leaveCheckedNode);
	}

	//*************************************************************************
	// Method:		resourceTreeView_Checked
	// Description: Called when the state of a check box in the tree view
	//				is changed.
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::resourceTreeView_Checked(Object* sender, TreeViewEventArgs* e)
	{
		//bugbug - don't allow resources of different types to be checked at once.
		if (e->Node->Checked)
		{
			if (IsSpecialNode(e->Node))
				e->Node->Checked = false;
			else
			{
				if (GetFaultType(e->Node) != faultType)
					RemoveAllChecks(e->Node);
				
				checkedResourcePaths->Add(BuildResourcePath(e->Node));
				faultType = GetFaultType(e->Node);
			}
		}
		else
		{
			checkedResourcePaths->Remove(BuildResourcePath(e->Node));
		}

		if (e->Node->Text->CompareTo(EDIT_TEXT) == 0)
			e->Node->BeginEdit();

		this->SelectedNode = e->Node;
	}

	//*************************************************************************
	// Method:		resourceTreeView_DoubleClick
	// Description: Called when the treeview is double clicked
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::resourceTreeView_DoubleClick(Object* sender, EventArgs* e)
	{
		if (CurrentSelection->CompareTo(EDIT_TEXT) == 0)
			SelectedNode->BeginEdit();
	}

	//*************************************************************************
	// Method:		resourceTreeView_AfterSelect
	// Description: Called when a tree view item is clicked
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::resourceTreeView_AfterSelect(Object* sender, TreeViewEventArgs* e)
	{
		String *resourcePath;

		if (!IsSpecialNode(e->Node))
		{
			resourcePath = BuildResourcePath(e->Node);
			if (!this->CheckBoxes)
			{
				checkedResourcePaths->Clear();
				checkedResourcePaths->Add(resourcePath);
				faultType = GetFaultType(e->Node);
			}
		}

		if (OnResourceClicked)
			OnResourceClicked->Invoke(resourcePath);

	}

	//*************************************************************************
	// Method:		resourceTreeView_AfterLabelEdit
	// Description: Called after an edit is complete
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::resourceTreeView_AfterLabelEdit(Object* sender, NodeLabelEditEventArgs* e)
	{
		//if unchanged, ignore
		if (!e->Label || (e->Label->CompareTo(EDIT_TEXT) == 0) )
			return;

		if (!this->CheckBoxes)
		{
			checkedResourcePaths->Clear();
			checkedResourcePaths->Add(e->Label);
		}

		if (OnResourceClicked)
			OnResourceClicked->Invoke(e->Label);

		e->Node->Parent->Nodes->Insert(0, new TreeNode(EDIT_TEXT));
	}

	//*************************************************************************
	// Method:		resourceTreeView_BeforeLabelEdit
	// Description: Called before an edit
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::resourceTreeView_BeforeLabelEdit(Object* sender, NodeLabelEditEventArgs* e)
	{
		if (e->Node->Text->CompareTo(EDIT_TEXT) != 0)
			e->CancelEdit = true;
	}

	//*************************************************************************
	// Method:		GetNodeWithValue
	// Description: Gets the child node with the value specified
	//
	// Parameters:
	//	value - the value to look for
	//
	// Return Value: the tree node if found, NULL otherwise
	//*************************************************************************
	TreeNode *ResourceTreeView::GetNodeWithValue(String *value)
	{
		TreeNode *treeNode;

		treeNode = dynamic_cast <TreeNode*> (registryHash->Item[value->ToLower()]);
		if (treeNode)
			return treeNode;

		treeNode = dynamic_cast <TreeNode*> (fileHash->Item[value->ToLower()]);
		if (treeNode)
			return treeNode;

		treeNode = dynamic_cast <TreeNode*> (processHash->Item[value->ToLower()]);
		if (treeNode)
			return treeNode;

		treeNode = dynamic_cast <TreeNode*> (comHash->Item[value->ToLower()]);

		return treeNode;
	}

	//*************************************************************************
	// Method:		SelectNodeWithValue
	// Description: Selects the tree node with the corresponding value
	//
	// Parameters:
	//	value - the value to look for
	//
	// Return Value: true if found, false otherwise
	//*************************************************************************
	bool ResourceTreeView::SelectNodeWithValue(String *value, bool check)
	{
		TreeNode *nodeToSelect = GetNodeWithValue(value);
		if (nodeToSelect)
		{
			this->ExpandAll();
			nodeToSelect->Checked = check;
			this->SelectedNode = nodeToSelect;
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		AddNewResource
	// Description: Adds a new resourcepath to the tree
	//
	// Parameters:
	//	type - specifies the resource type
	//	resourcePath - path to add
	//
	// Return Value: TreeNode that was added or null if failure
	//*************************************************************************
	TreeNode *ResourceTreeView::AddNewResource(UIData::NodeType type, String *resourcePath)
	{
		TreeNode *resourceNode;
		TreeNode *treeNode;
		TreeNode *parentNode;
		Hashtable *resourceHash;
		String *constructedPathString;
		String *subString;
		String *preResourcePath = "";
		int index, pathIndex;
		bool doubleSlashStart = false;

		if (!resourcePath || resourcePath->Length == 0)
			return NULL;

		switch (type)
		{
		case UIData::RegistryEntry:
			if (!registryNode)
			{
				registryNode = Nodes->Add("Registry Entries");
				registryNode->ForeColor = SystemColors::InfoText;
			}

			if ( (expand) && (!registryNode->IsExpanded) )
				registryNode->Expand();

			resourceNode = registryNode;
			resourceHash = registryHash;
			break;
		case UIData::File:
			if (!fileNode)
			{
				fileNode = Nodes->Add("Files and Folders");
				fileNode->ForeColor = SystemColors::InfoText;
			}

			if ( (expand) && (!fileNode->IsExpanded) )
				fileNode->Expand();

			resourceNode = fileNode;
			resourceHash = fileHash;
			break;
		case UIData::Process:
			if (!processNode)
			{
				processNode = Nodes->Add("Processes and Libraries");
				processNode->ForeColor = SystemColors::InfoText;
			}

			if ( (expand) && (!processNode->IsExpanded) )
				processNode->Expand();

			resourceNode = processNode;
			resourceHash = processHash;
			break;
		case UIData::ComObject:
			if (!comNode)
			{
				comNode = Nodes->Add("COM Objects");
				comNode->ForeColor = SystemColors::InfoText;
			}

			if ( (expand) && (!comNode->IsExpanded) )
				comNode->Expand();

			resourceNode = comNode;
			resourceHash = comHash;
			break;
		default:
			return NULL;
		}

		//trim whitespace from end
		resourcePath = resourcePath->TrimEnd(NULL);

		//remove all '\\'
		pathIndex = resourcePath->IndexOf("\\\\");
		if (pathIndex == 0)
			doubleSlashStart = true;
		resourcePath = resourcePath->Replace("\\\\", "\\");

		//remove trailing slash
		index = resourcePath->LastIndexOf("\\");
		if ((index != -1) && (index == resourcePath->Length-1))
			resourcePath = resourcePath->Remove(index, resourcePath->Length - index);

		//remove leading slash
		pathIndex = resourcePath->IndexOf("\\");
		if (pathIndex == 0)
			resourcePath = resourcePath->Remove(0, 1);

		//check to see if the node already exists
		treeNode = dynamic_cast <TreeNode*> (resourceHash->Item[resourcePath->ToLower()]);
		if (treeNode)
			return treeNode;

		//check to see if we can add to an existing sub-path
		constructedPathString = String::Copy(resourcePath);
		subString = resourcePath;
		treeNode = resourceNode;
		index = constructedPathString->LastIndexOf("\\");
		while (index != -1)
		{			
			constructedPathString = constructedPathString->Remove(index, constructedPathString->Length - index);
			if (doubleSlashStart)
			{
				//add back doubleslash so hashing is accurate
				String *doubleSlashString = constructedPathString->ToLower()->Insert(0, "\\\\");
				parentNode = dynamic_cast <TreeNode*> (resourceHash->Item[doubleSlashString]);
			}
			else
				parentNode = dynamic_cast <TreeNode*> (resourceHash->Item[constructedPathString->ToLower()]);

			if (parentNode)
			{
				subString = resourcePath->Substring(constructedPathString->Length+1);
				preResourcePath = resourcePath->Substring(0, constructedPathString->Length+1);
				treeNode = parentNode;
				break;
			}
			index = constructedPathString->LastIndexOf("\\");
		}

		//add '\\' back on if this is the start of a new path
		if (!parentNode && doubleSlashStart)
		{
			subString = String::Concat("\\\\", subString);
			index = subString->IndexOf("\\", 2);
		}
		else
			index = subString->IndexOf("\\");


		//now do the add, creating subpath keys as necessary
		constructedPathString = String::Copy(subString);

		System::Text::StringBuilder *pathString = new System::Text::StringBuilder(preResourcePath);
		while (index != -1)
		{
			subString = constructedPathString->Substring(0, index);
			pathString->Append(subString);
			treeNode = treeNode->Nodes->Add(subString);
			treeNode->ForeColor = SystemColors::InfoText;

			if (!resourceHash->Contains(pathString->ToString()->ToLower()))
				resourceHash->Add(pathString->ToString()->ToLower(), treeNode);

			pathString->Append("\\");
			constructedPathString = constructedPathString->Substring(index+1);
			index = constructedPathString->IndexOf("\\");
		}
		
		treeNode = treeNode->Nodes->Add(constructedPathString);
		treeNode->ForeColor = SystemColors::InfoText;
		if (!resourceHash->Contains(resourcePath->ToLower()))
		{
			if (doubleSlashStart)
			{
				//add back doubleslash so hashing is accurate
				resourcePath = resourcePath->Insert(0, "\\\\");
			}
			resourceHash->Add(resourcePath->ToLower(), treeNode);
		}

		this->Invalidate();
		return treeNode;
	}

	//*************************************************************************
	// Method:		AddResourceTree
	// Description: Adds a set of resources to the treeview and creates a hashtable to use for later lookup
	//
	// Parameters:
	//	resourcePaths - sorted list of resource paths
	//	resourceNode - node to add to
	//	resourceHash - hashtable to build up
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTreeView::AddResourceTree(UIData::NodeType type, SortedList *resourcePaths)
	{
		//setup the edit node
		AddNewResource(type, EDIT_TEXT);

		for (int i=0; i<resourcePaths->Count; i++)
		{
			String *resourcePath = dynamic_cast <String*> (resourcePaths->GetByIndex(i));
			AddNewResource(type, resourcePath);
		}

		//maintain selection on root node
		if (Nodes->Item[0])
			SelectedNode = Nodes->Item[0];
	}

	//*************************************************************************
	// Method:		BuildResourcePath
	// Description: Returns a full path given a node
	//
	// Parameters:
	//	resourceNode - node to build path from
	//
	// Return Value: resource path, or NULL on failure
	//*************************************************************************
	String *ResourceTreeView::BuildResourcePath(TreeNode *resourceNode)
	{
		String *retVal;
		if (!resourceNode)
			return NULL;
			
		if (resourceNode->FullPath->StartsWith("COM"))
		{
			//special case com objects, we just want clsid not name
			UIData::DisplayableDataNode *dataNode = dynamic_cast <UIData::DisplayableDataNode*> (resourceNode->Tag);
			if (!dataNode)
				return resourceNode->Text;
			else
				return dataNode->Name;
		}
		else
		{
			//strip first part of path off
			retVal = resourceNode->FullPath;
			int index = retVal->IndexOf("\\");
			if (index != -1)
				retVal = retVal->Substring(index+1);
			else
				return NULL;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		IsSpecialNode
	// Description:	checks to see if current node is a special node
	//
	// Parameters:
	//	resourceNode - node to check
	//
	// Return Value: true if special, false otherwise
	//*************************************************************************
	bool ResourceTreeView::IsSpecialNode(TreeNode *resourceNode)
	{
		if ( (resourceNode->Text->CompareTo(EDIT_TEXT) == 0) || (resourceNode == registryNode) || (resourceNode == fileNode) || (resourceNode == processNode) || (resourceNode == comNode) )	
			return true;
		else
			return false;
	}

	//*************************************************************************
	// Method:		Clear
	// Description:	Clears the treeview
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ResourceTreeView::Clear()
	{
		registryNode = NULL;
		fileNode = NULL;
		processNode = NULL;
		comNode = NULL;
		registryHash->Clear();
		fileHash->Clear();
		processHash->Clear();
		comHash->Clear();
		Nodes->Clear();
	}

	//*************************************************************************
	// Method:		GetFaultType
	// Description: Given a tree node determines what fault type would be associated
	//
	// Parameters:
	//	treeNode - node to check
	//
	// Return Value: fault type, custom if unknown
	//*************************************************************************
	FaultType ResourceTreeView::GetFaultType(TreeNode *treeNode)
	{
		if (!treeNode)
			return FaultType::CustomFault;

		String *resourcePath = treeNode->FullPath;

		if (resourcePath->StartsWith("File"))
			return FaultType::DiskFault;
		else if (resourcePath->StartsWith("Process"))
			return FaultType::ProcessFault;
		else if (resourcePath->StartsWith("Registry"))
			return FaultType::RegistryFault;
		else 
			return FaultType::CustomFault;
	}
}
