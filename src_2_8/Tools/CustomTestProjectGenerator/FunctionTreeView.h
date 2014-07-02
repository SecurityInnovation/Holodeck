//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionTreeView.h
//
// DESCRIPTION: Contains definition for the class FunctionTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;
using namespace FunctionDatabase;

namespace CustomTestProjectGenerator
{
	public __delegate void FunctionClickedDelegate(String* functionName);
	public __delegate void FunctionCheckChangedDelegate(String* functionName, bool isChecked);

	public __gc class FunctionTreeView: public TreeView
	{
	private:
		TreeNode *nativeNode;
		TreeNode *dotNetNode;
		TreeViewEventHandler * functionTreeView_Checked_Handler;
		TreeViewEventHandler * functionTreeView_AfterSelect_Handler;
		bool hideAW;

		FunctionDatabase::InterceptedFunctionDB * nativeInterceptedFunctionDB;
		FunctionDatabase::InterceptedFunctionDB * dotNetInterceptedFunctionDB;

		void functionTreeView_Checked(Object* sender, TreeViewEventArgs* e);
		void functionTreeView_AfterSelect(Object* sender, TreeViewEventArgs* e);

		TreeNode *GetChildNodeWithValue(TreeNode *node, String *value, bool isNative);

		void toggleViewState(bool isDotNet, bool isVisible);

	public:
		FunctionTreeView(bool checkBoxes, bool hideAW);
		~FunctionTreeView();

		bool SelectNodeWithValue(String *value);
		void AddChildNodeSorted(TreeNode* parent, TreeNode* child);

		__property Hashtable * get_DisabledFunctionsTable();
		__property void set_DisabledFunctionsTable(Hashtable* disabledFunctionsTable);
		__property ArrayList * get_DisabledFunctionsList();
		__property void set_DisabledFunctionsList(ArrayList* disabledFunctionsList);

		__property Hashtable * get_EnabledFunctionsTable();
		__property void set_EnabledFunctionsTable(Hashtable* enabledFunctionsTable);
		__property ArrayList * get_EnabledFunctionsList();
		__property void set_EnabledFunctionsList(ArrayList* enabledFunctionsList);

		__property void set_NativeEnable(bool value) { nativeNode->Checked = value; }
		__property void set_DotNetEnable(bool value) { dotNetNode->Checked = value; }
		
		__property bool get_HideAW() { return hideAW; }
		__property void set_HideAW(bool value) { hideAW = value; }

		__property void set_HideDotNet(bool value) { toggleViewState(true, value); }
		__property void set_HideNative(bool value) { toggleViewState(false, value); }

		void ExpandNativeFunctionTree() { nativeNode->Expand(); }
		void ExpandDotNetFunctionTree() { dotNetNode->Expand(); }
		void CollapseNativeFunctionTree() { nativeNode->Collapse(); }
		void CollapseDotNetFunctionTree() { dotNetNode->Collapse(); }

		void DisableNativeCategory(String* category);
		void DisableSingleNativeFunction(String* category, String* functionName);
		void DisableDotNetCategory(String *category);
		void DisableSingleDotNetFunction(String *category, String* className, String *functionName);

		FunctionClickedDelegate* OnFunctionClicked;
		FunctionCheckChangedDelegate* OnFunctionCheckChanged;

		InterceptedFunction * GetFunctionDefinitionFromFunctionName (String * functionName);
	};
}
