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

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Threading;

namespace HolodeckGui
{
	public __delegate void ResourceClickedDelegate(String* resourceName);

	public __gc class ResourceTreeView: public TreeView
	{
	public:
		ResourceClickedDelegate* OnResourceClicked;


		ResourceTreeView(bool checkBoxes, bool expand);
		~ResourceTreeView();

		__property ArrayList *get_CheckedResourcePaths() { return checkedResourcePaths; }
		__property FunctionDatabase::FaultType get_FaultType() { return faultType; }

		__property String *get_CurrentSelection() 
		{ 
			if (SelectedNode) 
				return SelectedNode->Text; 
			else
				return 0;
		}
		__property void set_CurrentSelection(String *value) { SelectNodeWithValue(value, true); }

		String *BuildResourcePath(TreeNode *resourceNode);
		FunctionDatabase::FaultType GetFaultType(TreeNode *treeNode);
		TreeNode *AddNewResource(UIData::NodeType type, String *resourcePath);
		void AddResourceTree(UIData::NodeType, SortedList *resourcePaths);
		void Clear();
		bool IsSpecialNode(TreeNode *resourceNode);
		TreeNode *GetNodeWithValue(String *value);

	private:
		TreeViewEventHandler * resourceTreeView_Checked_Handler;
		TreeViewEventHandler * resourceTreeView_AfterSelect_Handler;
		TreeNode *registryNode, *fileNode, *processNode, *comNode;
		Hashtable *registryHash, *fileHash, *processHash, *comHash;
		ArrayList *checkedResourcePaths;
		FunctionDatabase::FaultType faultType;
		bool expand;
		unsigned long guiThreadId;
		unsigned long timeoutCount;

		void resourceTreeView_Checked(Object* sender, TreeViewEventArgs* e);
		void resourceTreeView_DoubleClick(Object *sender, EventArgs *e);
		void resourceTreeView_AfterSelect(Object* sender, TreeViewEventArgs* e);
		void resourceTreeView_AfterLabelEdit(Object* sender, NodeLabelEditEventArgs* e);
		void resourceTreeView_BeforeLabelEdit(Object* sender, NodeLabelEditEventArgs* e);
		bool SelectNodeWithValue(String *value, bool check);
		void RemoveAllChecks(TreeNode *leaveCheckedNode);
		void RemoveChecks(TreeNode *treeNode, TreeNode *leaveCheckedNode);

	};
}
