//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultTreeView.h
//
// DESCRIPTION: Contains definition for the class FaultTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	public __delegate void FaultClickedDelegate(String* faultName);

	public __gc class FaultTreeView: public TreeView
	{
	public:
		FaultClickedDelegate* OnFaultClicked;

		FaultTreeView();
		~FaultTreeView();

		__property void set_FaultType(FaultType value);
		__property FunctionDatabase::FaultType get_FaultType() { return faultType; }

		void SetCurrentSelection(String *faultName, FunctionDatabase::FaultType faultType) { SelectNodeWithValue(faultName, faultType); }

	private:
		TreeViewEventHandler * faultTreeView_AfterSelect_Handler;
		TreeNode *registryNode, *diskNode, *processNode, *customNode;
		FunctionDatabase::FaultType faultType;

		void faultTreeView_AfterSelect(Object* sender, TreeViewEventArgs* e);
		TreeNode *GetNodeWithValue(String *faultName, FunctionDatabase::FaultType faultType);
		bool SelectNodeWithValue(String *faultName, FunctionDatabase::FaultType faultType);
		FunctionDatabase::FaultType GetFaultType(TreeNode *treeNode);
	};
}
