//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		VirtualResourceTreeView.cpp
//
// DESCRIPTION: Contains implementation for the class VirtualResourceTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include "VirtualResourceTreeView.h"
#include "DisplayableDataNode.h"

#define EDIT_TEXT "<Click to add a new resource path...>"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		VirtualResourceTreeView
	// Description: Constructor for the VirtualResourceTreeView class
	//
	// Parameters:
	//	resourceDataArray - contains all the resource information to populate the tree with
	//
	// Return Value: None
	//*************************************************************************
	VirtualResourceTreeView::VirtualResourceTreeView(bool Expand)
	{
		root = new TreeNode();
		root->Expand();
		registryHash = new Hashtable();
		fileHash = new Hashtable();
		processHash = new Hashtable();
		comHash = new Hashtable();
		nodeSelectedTable = new Hashtable();

		expand = Expand;

		this->HideSelection = true;
		this->ShowLines = true;
		this->ShowPlusMinus = true;
		this->ColumnTracking = false;
		this->ShowRootLines = false;
		this->FullRowSelect = false;
		this->Columns->Add("", 1000, HorizontalAlignment::Left);
		this->HeaderStyle = ColumnHeaderStyle::None;
		
		this->Location = System::Drawing::Point(8, 32);
		this->Name = "VirtualResourceTreeView";
		this->LabelEdit = true;

		//virtual tree view delegate handlers
		GetParentNodeHandler = new GetParentNodeDelegate(this, onGetParentNode);
		GetFirstChildHandler = new GetFirstChildDelegate(this, onGetFirstChildNode);
		GetLastChildHandler = new GetLastChildDelegate(this, onGetLastChildNode);
		GetNextChildHandler = new GetNextChildDelegate(this, onGetNextChildNode);
		GetPrevChildHandler = new GetPrevChildDelegate(this, onGetPrevChildNode);
		GetNextSiblingHandler = new GetNextSiblingDelegate(this, onGetNextSiblingNode);
		GetPrevSiblingHandler = new GetPrevSiblingDelegate(this, onGetPrevSiblingNode);
		GetChildNodeAtHandler = new GetChildNodeAtDelegate(this, onGetChildNodeAt);
		SetIsSelectedHandler = new SetIsSelectedDelegate(this, onSetIsSelected);
		SetIsExpandedHandler = new SetIsExpandedDelegate(this, onSetIsExpanded);
		SetIsFocusedHandler = new SetIsFocusedDelegate(this, onSetIsFocused);
		GetRowNumberHandler = new GetRowNumberDelegate(this, onGetRowNumber);
		GetNodeIndexHandler = new GetNodeIndexDelegate(this, onGetNodeIndex);
		GetChildNodeCountHandler = new GetChildNodeCountDelegate(this, onGetChildNodeCount);
		GetVisibleNodeCountHandler = new GetVisibleNodeCountDelegate(this, onGetVisibleNodeCount);

		QueryTreeListNode += new QueryTreeListNodeDelegate(this, onTreeListViewQueryItem);
		QueryTreeListNodeAtRow += new QueryTreeListNodeAtRowDelegate(this, onTreeListViewQueryRow);
		UnselectAllNodes += new UnselectAllNodesDelegate(this, onTreeListViewUnselectAll);

		this->RootNode = ConvertToTreeListNode(root);
	}


	//*************************************************************************
	// Method:		~VirtualResourceTreeView
	// Description: Destructor for the VirtualResourceTreeView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	VirtualResourceTreeView::~VirtualResourceTreeView()
	{
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
	TreeNode *VirtualResourceTreeView::GetNodeWithValue(String *value)
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
	// Method:		GetFaultType
	// Description: Given a tree node determines what fault type would be associated
	//
	// Parameters:
	//	treeNode - node to check
	//
	// Return Value: fault type, custom if unknown
	//*************************************************************************
	FaultType VirtualResourceTreeView::GetFaultType(TreeNode *treeNode)
	{
		if (!treeNode)
			return FaultType::CustomFault;

		String *resourcePath = BuildResourcePath(treeNode, false);

		if (resourcePath->StartsWith("File"))
			return FaultType::DiskFault;
		else if (resourcePath->StartsWith("Process"))
			return FaultType::ProcessFault;
		else if (resourcePath->StartsWith("Registry"))
			return FaultType::RegistryFault;
		else 
			return FaultType::CustomFault;
	}

	//*************************************************************************
	// Method:		BuildResourcePath
	// Description: Returns a full path given a node
	//
	// Parameters:
	//	resourceNode - node to build path from
	//	realPath - if true, strip off first part of string that is not part of the system path
	//
	// Return Value: resource path, or NULL on failure
	//*************************************************************************
	String *VirtualResourceTreeView::BuildResourcePath(TreeNode *resourceNode, bool realPath)
	{
		String *retVal;
		if (!resourceNode)
			return NULL;

		System::Text::StringBuilder *stringBuilder = new System::Text::StringBuilder(dynamic_cast <String*> (resourceNode->Text));
		TreeNode *parentNode = resourceNode->Parent;
		while (parentNode && parentNode->Parent)
		{
			stringBuilder->Insert(0, String::Concat(parentNode->Text, "\\"));
			parentNode = parentNode->Parent;
		}

		retVal = stringBuilder->ToString();
		if (retVal->StartsWith("COM"))
		{
			//special case com objects, we just want clsid not name
			UIData::DisplayableDataNode *dataNode = dynamic_cast <UIData::DisplayableDataNode*> (resourceNode->Tag);
			if (!dataNode)
				return resourceNode->Text;
			else
				return dataNode->Name;
		}
		else if (realPath)
		{
			//strip first part of path off
			int index = retVal->IndexOf("\\");
			if (index != -1)
				retVal = retVal->Substring(index+1);
			else
				retVal = NULL;
		}

		return retVal;
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
	bool VirtualResourceTreeView::SelectNodeWithValue(String *value, bool check)
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
	TreeNode *VirtualResourceTreeView::AddNewResource(UIData::NodeType type, String *resourcePath)
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
				ItemCount++;
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
				ItemCount++;
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
				ItemCount++;
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
				ItemCount++;
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
			ItemCount++;

			if (!resourceHash->Contains(pathString->ToString()->ToLower()))
				resourceHash->Add(pathString->ToString()->ToLower(), treeNode);

			pathString->Append("\\");
			constructedPathString = constructedPathString->Substring(index+1);
			index = constructedPathString->IndexOf("\\");
		}
		
		treeNode = treeNode->Nodes->Add(constructedPathString);
		treeNode->ForeColor = SystemColors::InfoText;
		ItemCount++;
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
	void VirtualResourceTreeView::AddResourceTree(UIData::NodeType type, SortedList *resourcePaths)
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
			SelectedIndex = 0;
			
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
	bool VirtualResourceTreeView::IsSpecialNode(TreeNode *resourceNode)
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
	void VirtualResourceTreeView::Clear()
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
		this->Invalidate();
	}

	//*************************************************************************
	// virtual tree view helper functions
	//*************************************************************************

	//*************************************************************************
	// Method:		GetNodeAtRow
	// Description: recursive search to find the node at the correct row
	//
	// Parameters:
	//	treeNode - node we are currently searching
	//	rowCount - the row index we are currently looking at
	//	row - row we are looking for
	//	visibleOnly - true if we only care about visible entries
	//
	// Return Value: the tree list node at the specified row
	//*************************************************************************
	TreeNode *VirtualResourceTreeView::GetNodeAtRow(TreeNode *treeNode, int &rowCount, unsigned int row, bool visibleOnly)
	{
		//bugbug - need to improve this for perf reasons
		TreeNode *retVal;

		if ( (rowCount < row) && (!visibleOnly || (visibleOnly && treeNode->IsExpanded)) )
		{
			for (int i=0; i < treeNode->Nodes->Count; i++)
			{
				retVal = GetNodeAtRow(treeNode->Nodes->Item[i], ++rowCount, row, visibleOnly);
				if ( (retVal) || (rowCount > row) )
					break;
			}
		}
		else if (rowCount == row)
			retVal = treeNode;

		return retVal;
	}


	//*************************************************************************
	// Method:		GetRowNumber
	// Description: recursively find the row number of a node
	//
	// Parameters:
	//	nodeToLookFor - the node to get the row number of
	//	searchNode - current node we are searching
	//	visibleOnly - true to only count the visible nodes, false to count them all
	//	rowNumber - the rownumber we have reached
	//
	// Return Value: the row number of the node
	//*************************************************************************
	int VirtualResourceTreeView::GetRowNumber(TreeNode *nodeToLookFor, TreeNode *searchNode, bool visibleOnly, int &rowNumber)
	{
		//bugbug - need to improve this for perf reasons
		int retVal;

		if (nodeToLookFor == searchNode)
			return rowNumber;

		if (!visibleOnly || (visibleOnly && searchNode->IsExpanded) )
		{
			for (int i=0; i < searchNode->Nodes->Count; i++)
			{
				retVal = GetRowNumber(nodeToLookFor, searchNode->Nodes->Item[i], visibleOnly, ++rowNumber);
				if (retVal != -1)
					return retVal;
			}
		}

		return -1;
	}

	//*************************************************************************
	// Method:		GetVisibleNodeCount
	// Description: gets the number of visible nodes under the specified node
	//
	// Parameters:
	//	node - the node to get the visible count of
	//
	// Return Value: the visible count of the node (always at least 1)
	//*************************************************************************
	int VirtualResourceTreeView::GetVisibleNodeCount(TreeNode *node)
	{
		// start at 1 for this node
		int result = 1;

		if (node->IsExpanded && node->Nodes)
		{
			for (int i = 0; i < node->Nodes->Count; i++)
			{
				result += GetVisibleNodeCount(dynamic_cast<TreeNode *>(node->Nodes->Item[i]));
			}
		}
		return result;
	}

	//*************************************************************************
	// Method:		GetChildNodeCount
	// Description: gets the number of child nodes of a particular node
	//
	// Parameters:
	//	node - the node to get the child count of
	//
	// Return Value: the child count of the node, 0 if failed
	//*************************************************************************
	int VirtualResourceTreeView::GetChildNodeCount(TreeNode *node)
	{
		int result = 0;

		if (node && node->Nodes)
		{
			for (int i = 0; i < node->Nodes->Count; i++)
			{
				result += GetChildNodeCount(dynamic_cast<TreeNode *>(node->Nodes->Item[i])) + 1;
			}
		}
		return result;
	}

	TreeNode *VirtualResourceTreeView::NodeAtMousePos(MouseEventArgs *e)
	{
		TreeListNode *treeListNode = NodeInNodeRow(e);

		if (!treeListNode)
			return NULL;
		else
			return dynamic_cast <TreeNode*> (treeListNode->Tag);
	}

	//*************************************************************************
	// Method:		ConvertToTreeListNode
	// Description: called when the ui needs to get the tree list node at a 
	//	particular row
	//
	// Parameters:
	//	row - the row index to get the node for
	//
	// Return Value: the tree list node at the specified row
	//*************************************************************************
	TreeListNode *VirtualResourceTreeView::ConvertToTreeListNode(TreeNode *treeNode)
	{
		if (!treeNode)
			return NULL;

		TreeListNode *retVal = new TreeListNode();
		retVal->Text = treeNode->Text;
		retVal->IsExpanded = treeNode->IsExpanded;
		retVal->Selected = nodeSelectedTable->Contains(treeNode);
		retVal->Focused = (focusedNode == treeNode);
		retVal->IsVisible = true;
		retVal->Tag = treeNode;
		retVal->ForeColor = treeNode->ForeColor;
		InitTreeListNodeDelegates(retVal);
		return retVal;
	}

	//*************************************************************************
	// Method:		InitTreeListNodeDelegates
	// Description: attaches delegates to a treelistnode item
	//
	// Parameters:
	//	node - the node to attach the delegates to
	//
	// Return Value: None
	//*************************************************************************
	void VirtualResourceTreeView::InitTreeListNodeDelegates(TreeListNode *node)
	{
		node->GetParentNode += GetParentNodeHandler;
		node->GetFirstChild += GetFirstChildHandler;
		node->GetLastChild += GetLastChildHandler;
		node->GetNextChild += GetNextChildHandler;
		node->GetPrevChild += GetPrevChildHandler;
		node->GetNextSibling += GetNextSiblingHandler;
		node->GetPrevSibling += GetPrevSiblingHandler;
		node->GetChildNodeAt += GetChildNodeAtHandler;
		node->SetIsExpanded += SetIsExpandedHandler;
		node->SetIsSelected += SetIsSelectedHandler;
		node->SetIsFocused += SetIsFocusedHandler;
		node->GetRowNumber += GetRowNumberHandler;
		node->GetNodeIndex += GetNodeIndexHandler;
		node->GetChildNodeCount += GetChildNodeCountHandler;
		node->GetVisibleNodeCount += GetVisibleNodeCountHandler;
	}

	//*************************************************************************
	// virtual tree view delegate handling functions
	//*************************************************************************

	//*************************************************************************
	// Method:		onTreeListViewQueryItem
	// Description: called when the ui needs to get the tree list node at a 
	//	particular index
	//
	// Parameters:
	//	index - the index to get the node for
	//
	// Return Value: the tree list node at the specified index
	//*************************************************************************
	TreeListNode *VirtualResourceTreeView::onTreeListViewQueryItem(unsigned int index)
	{
		int rowCount = -1;
		TreeNode *treeNode;

		for (int i=0; i < Nodes->Count; i++)
		{
			treeNode = GetNodeAtRow(Nodes->Item[i], ++rowCount, index, false);
			if ( (treeNode) || (rowCount > index) )
				break;
		}

		return ConvertToTreeListNode(treeNode);
	}

	//*************************************************************************
	// Method:		onTreeListViewQueryRow
	// Description: called when the ui needs to get the tree list node at a 
	//	particular row
	//
	// Parameters:
	//	row - the row index to get the node for
	//
	// Return Value: the tree list node at the specified row
	//*************************************************************************
	TreeListNode *VirtualResourceTreeView::onTreeListViewQueryRow(unsigned int row)
	{
		int rowCount = -1;
		TreeNode *treeNode;

		for (int i=0; i < Nodes->Count; i++)
		{
			treeNode = GetNodeAtRow(Nodes->Item[i], ++rowCount, row, true);
			if ( (treeNode) || (rowCount > row) )
				break;
		}

		return ConvertToTreeListNode(treeNode);
	}

	//*************************************************************************
	// Method:		onTreeListViewUnselectAll
	// Description: called when the ui needs to unselect all tree nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void VirtualResourceTreeView::onTreeListViewUnselectAll()
	{
		nodeSelectedTable->Clear();
	}

	//*************************************************************************
	// Method:		onGetRowNumber
	// Description: Called when pane needs to know the row number of a node
	//
	// Parameters:
	//	node - the node to get the row number of
	//	visibleOnly - true to only count the visible nodes, false to count them all
	//
	// Return Value: the row number of the node
	//*************************************************************************
	int VirtualResourceTreeView::onGetRowNumber(TreeListNode *node, bool visibleOnly)
	{
		if (!node)
			return -1;

		TreeNode *nodeToLookFor = dynamic_cast <TreeNode*> (node->Tag);
		int retVal = -1;
		int rowNumber = -1;

		for (int i=0; i < Nodes->Count; i++)
		{
			retVal = GetRowNumber(nodeToLookFor, Nodes->Item[i], visibleOnly, ++rowNumber);
			if (retVal != -1)
				return retVal;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		onGetRowNumber
	// Description: Called when pane needs to know the row number of a node
	//
	// Parameters:
	//	node - the node to get the row number of
	//	visibleOnly - true to only count the visible nodes, false to count them all
	//
	// Return Value: the row number of the node
	//*************************************************************************
	int VirtualResourceTreeView::onGetRowNumber(TreeNode *nodeToLookFor, bool visibleOnly)
	{
		if (!nodeToLookFor)
			return -1;

		int retVal = -1;
		int rowNumber = -1;

		for (int i=0; i < Nodes->Count; i++)
		{
			retVal = GetRowNumber(nodeToLookFor, Nodes->Item[i], visibleOnly, ++rowNumber);
			if (retVal != -1)
				return retVal;
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		onGetNodeIndex
	// Description: Called when pane needs to the index of a node in another node
	//
	// Parameters:
	//	parent - the node containing the child node
	//	child - the child node to get the index of 
	//
	// Return Value: the index of the child node in the parent node
	//*************************************************************************
	int VirtualResourceTreeView::onGetNodeIndex(TreeListNode *parent, TreeListNode *child)
	{
		if (!parent || !child)
			return -1;

		TreeNode *parentTreeNode = dynamic_cast<TreeNode*>(parent->Tag);
		TreeNode *childTreeNode = dynamic_cast<TreeNode*>(child->Tag);
		if (!parentTreeNode || !childTreeNode || !parentTreeNode->Nodes)
			return -1;

		return parentTreeNode->Nodes->IndexOf(childTreeNode);
	}

	//*************************************************************************
	// Method:		onGetParentNode
	// Description: called when the pane needs to get a node's parent node
	//
	// Parameters:
	//	node - the node to get the parent of
	//
	// Return Value: the parent of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetParentNode(TreeListNode *node)
	{
		if (!node)
			return NULL;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return NULL;

		if (treeNode->Parent)
			return ConvertToTreeListNode(treeNode->Parent);

		return NULL;
	}

	//*************************************************************************
	// Method:		onGetFirstChildNode
	// Description: called when the pane needs to get a node's first child node
	//
	// Parameters:
	//	node - the node to get the first child of
	//
	// Return Value: the first child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetFirstChildNode(TreeListNode *node)
	{
		if (!node)
			return NULL;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return NULL;

		return ConvertToTreeListNode(treeNode->FirstNode);
	}

	//*************************************************************************
	// Method:		onGetLastChildNode
	// Description: called when the pane needs to get a node's last child node
	//
	// Parameters:
	//	node - the node to get the last child of
	//
	// Return Value: the last child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetLastChildNode(TreeListNode *node)
	{
		if (!node)
			return NULL;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return NULL;

		return ConvertToTreeListNode(treeNode->LastNode);
	}

	//*************************************************************************
	// Method:		onGetNextChildNode
	// Description: called when the pane needs to get a node's next child node
	//
	// Parameters:
	//	node - the node to get the next child of
	//	curChild - the current child of the node
	//
	// Return Value: the next child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetNextChildNode(TreeListNode *node, TreeListNode *curChild)
	{
		if (!node || !curChild)
			return NULL;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		TreeNode *curChildTreeNode = dynamic_cast<TreeNode*>(curChild->Tag);

		if (!treeNode || !curChildTreeNode)
			return NULL;

		if (treeNode->Nodes && (treeNode->Nodes->Count > 0))
		{
			int thisIndex = treeNode->Nodes->IndexOf(curChildTreeNode);
			if (thisIndex < treeNode->Nodes->Count - 1)
			{
				TreeNode *nextChildNode = treeNode->Nodes->Item[thisIndex + 1];
				if (!nextChildNode)
					return NULL;

				return ConvertToTreeListNode(nextChildNode);
			}
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		onGetPrevChildNode
	// Description: called when the pane needs to get a node's previous child node
	//
	// Parameters:
	//	node - the node to get the previous child of
	//	curChild - the current child of the node
	//
	// Return Value: the previous child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetPrevChildNode(TreeListNode *node, TreeListNode *curChild)
	{
		if (!node || !curChild)
			return NULL;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		TreeNode *curChildTreeNode = dynamic_cast<TreeNode*>(curChild->Tag);

		if (!treeNode || !curChildTreeNode)
			return NULL;

		if (treeNode->Nodes && (treeNode->Nodes->Count > 0))
		{
			int thisIndex = treeNode->Nodes->IndexOf(curChildTreeNode);
			if (thisIndex > 0)
			{
				TreeNode *prevChildNode = treeNode->Nodes->Item[thisIndex - 1];
				if (!prevChildNode)
					return NULL;

				return ConvertToTreeListNode(prevChildNode);
			}
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		onGetChildNodeAt
	// Description: called when the pane needs to get a node's child node
	//
	// Parameters:
	//	node - the node to get the child of
	//	index - the index of the child to get
	//
	// Return Value: the specified child of the node, NULL if failed
	//*************************************************************************
	TreeListNode *VirtualResourceTreeView::onGetChildNodeAt(TreeListNode *node, unsigned int index)
	{
		if (!node)
			return NULL;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return NULL;

		if (treeNode->Nodes && ((unsigned int)treeNode->Nodes->Count > index)) 
		{
			TreeNode *childNode = treeNode->Nodes->Item[index];
			if (!childNode)
				return NULL;

			return ConvertToTreeListNode(childNode);
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		onGetNextSiblingNode
	// Description: called when the pane needs to get a node's next sibling node
	//
	// Parameters:
	//	node - the node to get the next sibling of
	//
	// Return Value: the next sibling of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetNextSiblingNode(TreeListNode *node)
	{
		try
		{
			int index;
			TreeNode *treeNode, *parentNode;
			
			treeNode = dynamic_cast<TreeNode*>(node->Tag);
			parentNode = treeNode->Parent;
			index = parentNode->Nodes->IndexOf(treeNode);
			return ConvertToTreeListNode(parentNode->Nodes->Item[index+1]);
		}
		catch (...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetPrevSiblingNode
	// Description: called when the pane needs to get a node's previous sibling node
	//
	// Parameters:
	//	node - the node to get the previous sibling of
	//
	// Return Value: the previous sibling of the node, NULL if failed
	//*************************************************************************
	ParentChildList *VirtualResourceTreeView::onGetPrevSiblingNode(TreeListNode *node)
	{
		try
		{
			int index;
			TreeNode *treeNode, *parentNode;
			
			treeNode = dynamic_cast<TreeNode*>(node->Tag);
			parentNode = treeNode->Parent;
			index = parentNode->Nodes->IndexOf(treeNode);
			return ConvertToTreeListNode(parentNode->Nodes->Item[index-1]);
		}
		catch (...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onSetIsSelected
	// Description: called when the pane needs to set whether or not a node is
	//	selected
	//
	// Parameters:
	//	node - the node to set
	//	selected - true for selected, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void VirtualResourceTreeView::onSetIsSelected(TreeListNode *node, bool selected)
	{
		if (!node)
			return;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return;

		if (selected && !nodeSelectedTable->Contains(treeNode))
			nodeSelectedTable->Add(treeNode, treeNode);
		else if (!selected && nodeSelectedTable->Contains(treeNode))
			nodeSelectedTable->Remove(treeNode);
	}

	//*************************************************************************
	// Method:		onSetIsExpanded
	// Description: called when the pane needs to set whether or not a node is
	//	expanded
	//
	// Parameters:
	//	node - the node to set
	//	expanded - true for expanded, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void VirtualResourceTreeView::onSetIsExpanded(TreeListNode *node, bool expanded)
	{
		if (!node)
			return;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return;
		

		if (expanded)
			treeNode->Expand();
		else
			treeNode->Collapse();
	}

	//*************************************************************************
	// Method:		onSetIsFocused
	// Description: called when the pane needs to set whether or not a node is
	//	focused
	//
	// Parameters:
	//	node - the node to set
	//	focused - true for focused, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void VirtualResourceTreeView::onSetIsFocused(TreeListNode *node, bool focused)
	{
		if (!node)
			return;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return;

		if (focused)
			focusedNode = treeNode;
		else
			focusedNode = NULL;
	}

	//*************************************************************************
	// Method:		onGetChildNodeCount
	// Description: called when the pane needs to get the number of child nodes
	//	of a particular node
	//
	// Parameters:
	//	node - the node to get the child count of
	//
	// Return Value: the child count of the node, 0 if failed
	//*************************************************************************
	int VirtualResourceTreeView::onGetChildNodeCount(TreeListNode *node)
	{
		if (!node)
			return 0;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode || !treeNode->Nodes)
			return 0;

		return GetChildNodeCount(treeNode);
	}

	//*************************************************************************
	// Method:		onGetVisibleNodeCount
	// Description: called when the pane needs to get the number of visible nodes
	//	of a particular node
	//
	// Parameters:
	//	node - the node to get the visible count of
	//
	// Return Value: the visible count of the node, 0 if failed
	//*************************************************************************
	int VirtualResourceTreeView::onGetVisibleNodeCount(TreeListNode *node)
	{
		if (!node)
			return 0;

		TreeNode *treeNode = dynamic_cast<TreeNode*>(node->Tag);
		if (!treeNode)
			return 0;

		if (!treeNode->Nodes)
			return 1;

		return GetVisibleNodeCount(treeNode);
	}
}

