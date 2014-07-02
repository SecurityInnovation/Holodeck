//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTreeView.h
//
// DESCRIPTION: Contains definition for the class ResourceTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "ResourcePaneDataArray.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <SIControls.dll>

using namespace SIControls;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Threading;

namespace HolodeckGui
{
	public __gc class VirtualResourceTreeView: public TreeListView
	{
	public:
		VirtualResourceTreeView(bool expand);
		~VirtualResourceTreeView();
	
		__property String *get_CurrentSelection() 
		{ 
			if (firstSelectedNode) 
				return firstSelectedNode->Text; 
			else
				return 0;
		}
		__property void set_CurrentSelection(String *value) { SelectNodeWithValue(value, true); }

		__property TreeNode *get_SelectedNode() 
		{ 
			if (firstSelectedNode)
				return dynamic_cast <TreeNode*> (firstSelectedNode->Tag);
			else
				return NULL;
		}
		__property void set_SelectedNode(TreeNode *treeNode) 
		{ 
			int index = onGetRowNumber(treeNode, false);
			if (index != -1)
				SelectedIndex = index;
		}

		__property TreeNodeCollection *get_Nodes() { return root->Nodes; }

		TreeNode *NodeAtMousePos(MouseEventArgs *e);
		String *BuildResourcePath(TreeNode *resourceNode, bool realPath);
		FaultType GetFaultType(TreeNode *treeNode);
		TreeNode *AddNewResource(UIData::NodeType type, String *resourcePath);
		void AddResourceTree(UIData::NodeType, SortedList *resourcePaths);
		void Clear();
		bool IsSpecialNode(TreeNode *resourceNode);
		TreeNode *GetNodeWithValue(String *value);

	private:
		TreeNode *registryNode, *fileNode, *processNode, *comNode;
		Hashtable *registryHash, *fileHash, *processHash, *comHash;
		bool expand;
		TreeNode *root;
		TreeNode *focusedNode;
		Hashtable *nodeSelectedTable;

		//virtual treeview delegates
		GetParentNodeDelegate *GetParentNodeHandler;
		GetFirstChildDelegate *GetFirstChildHandler;
		GetLastChildDelegate *GetLastChildHandler;
		GetNextChildDelegate *GetNextChildHandler;
		GetPrevChildDelegate *GetPrevChildHandler;
		GetNextSiblingDelegate *GetNextSiblingHandler;
		GetPrevSiblingDelegate *GetPrevSiblingHandler;
		GetChildNodeAtDelegate *GetChildNodeAtHandler;
		SetIsSelectedDelegate *SetIsSelectedHandler;
		SetIsFocusedDelegate *SetIsFocusedHandler;
		SetIsExpandedDelegate *SetIsExpandedHandler;
		GetRowNumberDelegate *GetRowNumberHandler;
		GetNodeIndexDelegate *GetNodeIndexHandler;
		GetChildNodeCountDelegate *GetChildNodeCountHandler;
		GetVisibleNodeCountDelegate *GetVisibleNodeCountHandler;


		bool SelectNodeWithValue(String *value, bool check);
		TreeNode *GetNodeAtRow(TreeNode *treeNode, int &rowCount, unsigned int row, bool visibleOnly);
		int GetVisibleNodeCount(TreeNode *node);
		int GetChildNodeCount(TreeNode *node);
		int GetRowNumber(TreeNode *nodeToLookFor, TreeNode *currentNode, bool visibleOnly, int &rowNumber);
		TreeListNode *ConvertToTreeListNode(TreeNode *treeNode);
		void InitTreeListNodeDelegates(TreeListNode *node);

		//virtual treeview delegates handling functions
		ParentChildList *onGetParentNode(TreeListNode *node);
		ParentChildList *onGetFirstChildNode(TreeListNode *node);
		ParentChildList *onGetLastChildNode(TreeListNode *node);
		ParentChildList *onGetNextChildNode(TreeListNode *node, TreeListNode *curChild);
		ParentChildList *onGetPrevChildNode(TreeListNode *node, TreeListNode *curChild);
		TreeListNode *onGetChildNodeAt(TreeListNode *node, unsigned int index);
		void onSetIsSelected(TreeListNode *node, bool selected);
		void onSetIsExpanded(TreeListNode *node, bool expanded);
		void onSetIsFocused(TreeListNode *node, bool focused);
		ParentChildList *onGetNextSiblingNode(TreeListNode *node);
		ParentChildList *onGetPrevSiblingNode(TreeListNode *node);
		int onGetChildNodeCount(TreeListNode *node);
		int onGetRowNumber(TreeListNode *node, bool visibleOnly);
		int onGetRowNumber(TreeNode *node, bool visibleOnly);
		int onGetNodeIndex(TreeListNode *parent, TreeListNode *child);
		int onGetVisibleNodeCount(TreeListNode *node);
		TreeListNode *onTreeListViewQueryItem(unsigned int index);
		TreeListNode *onTreeListViewQueryRow(unsigned int row);
		void onTreeListViewUnselectAll();
	};
}
