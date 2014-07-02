//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionTreeView.cpp
//
// DESCRIPTION: Contains implementation for the class FunctionTreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "FunctionTreeView.h"
#include "defines.h"

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// Method:		FunctionTreeView
	// Description: Constructor for the FunctionTreeView class
	//
	// Parameters:
	//	checkBoxes - If true, check boxes will be shown allowing the user
	//		to enable/disable functions
	//	hideAW - If true hide distinction between ansi and widechar functions
	//
	// Return Value: None
	//*************************************************************************
	FunctionTreeView::FunctionTreeView(bool checkBoxes, bool hideAW)
	{
		this->ImageIndex = -1;
		this->Location = System::Drawing::Point(8, 32);
		this->Name = "functionTreeView";
		this->SelectedImageIndex = -1;
		this->HideSelection = false;
		this->CheckBoxes = checkBoxes;
		this->HideAW = hideAW;
		this->functionTreeView_Checked_Handler = new TreeViewEventHandler (this, functionTreeView_Checked);
		this->add_AfterCheck (functionTreeView_Checked_Handler);
		this->functionTreeView_AfterSelect_Handler = new TreeViewEventHandler(this, functionTreeView_AfterSelect);
		this->add_AfterSelect (functionTreeView_AfterSelect_Handler);

		// initialize the function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		nativeInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!nativeInterceptedFunctionDB->IsLoaded)
			nativeInterceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		dotNetInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		if (!dotNetInterceptedFunctionDB->IsLoaded)
			dotNetInterceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

		// get the categories from the database and add it to the treeview
		nativeNode = new TreeNode("Native functions");
		nativeNode->Tag = S"Platform"; // save the node type as a tag
		nativeNode->Checked = true;

		dotNetNode = new TreeNode(".NET functions");
		dotNetNode->Tag = S"Platform"; // save the node type as a tag
		dotNetNode->Checked = true;

		// Add the native functions, organized by category
		Array *categoryList = nativeInterceptedFunctionDB->GetCategoryArray();
		for (int i=0; i<categoryList->Length; i++)
		{
			String* category = dynamic_cast<String*>(categoryList->Item[i]);
			// Convert categories like "FILE" to "File Functions"
			TreeNode* categoryNode = new TreeNode(String::Concat(category->Substring(0, 1),
				category->Substring(1)->ToLower(), " Functions"));
			categoryNode->Tag = String::Concat("NativeCategory:", category); // save the node type as a tag

			Array *funcList = nativeInterceptedFunctionDB->GetFunctionCategoryArray(category);
			for (int j=0; j<funcList->Length; j++)
			{
				InterceptedFunction *interceptedFunction = dynamic_cast<InterceptedFunction*>(funcList->Item[j]);
				String *functionName = interceptedFunction->Name;
				if (hideAW)
				{
					if (functionName->EndsWith("A"))
					{
						//don't add the A function
						continue;
					}
					else if (functionName->EndsWith("W"))
					{
						//remove the W from the name
						functionName = functionName->Remove(functionName->Length-1, 1);
					}
				}

				TreeNode* funcNode = new TreeNode(functionName);
				// save the node type and full function name as a tag
				funcNode->Tag = String::Concat("Function:", interceptedFunction->Name);
				funcNode->Checked = true;
				AddChildNodeSorted(categoryNode, funcNode);
			}

			categoryNode->Checked = true;
			AddChildNodeSorted(nativeNode, categoryNode);
		}

		// Add the .NET functions, organized by category and class
		categoryList = dotNetInterceptedFunctionDB->GetCategoryArray();
		for (int i=0; i<categoryList->Length; i++)
		{
			String* category = dynamic_cast<String*>(categoryList->Item[i]);
			// Convert categories like "FILE" to "File Functions"
			TreeNode* categoryNode = new TreeNode(String::Concat(category->Substring(0, 1),
				category->Substring(1)->ToLower(), " Functions"));
			categoryNode->Tag = String::Concat("DotNetCategory:", category); // save the node type as a tag

			Array *classList = dotNetInterceptedFunctionDB->GetClassCategoryArray(category);
			for (int j=0; j<classList->Length; j++)
			{
				String* className = dynamic_cast<String*>(classList->Item[j]);
				TreeNode* classNode = new TreeNode(className);
				classNode->Tag = new String("DotNetClass"); // save the node type as a tag

				// Add the functions in this class
				Array* funcList = dotNetInterceptedFunctionDB->GetClassFunctionsArray(className);
				for (int k=0; k<funcList->Length; k++)
				{
					String* funcName = dynamic_cast<String*>(funcList->Item[k]);

					// Get the name of the function without the preceding class name
					String* funcNameWithoutClass = funcName;
					int dotIndex = funcNameWithoutClass->LastIndexOf('.');
					if (dotIndex != -1)
						funcNameWithoutClass = funcNameWithoutClass->Substring(dotIndex + 1);

					TreeNode* funcNode = new TreeNode(funcNameWithoutClass);
					// save the node type and full function name as a tag
					funcNode->Tag = String::Concat("Function:", funcName);
					funcNode->Checked = true;
					AddChildNodeSorted(classNode, funcNode);
				}

				classNode->Checked = true;
				AddChildNodeSorted(categoryNode, classNode);
			}

			categoryNode->Checked = true;
			AddChildNodeSorted(dotNetNode, categoryNode);
		}


		this->Nodes->Add(nativeNode);
		this->Nodes->Add(dotNetNode);
	}


	//*************************************************************************
	// Method:		~FunctionTreeView
	// Description: Destructor for the FunctionTreeView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionTreeView::~FunctionTreeView()
	{
	}

	
	//*************************************************************************
	// Method:		functionTreeView_Checked
	// Description: Called when the state of a check box in the tree view
	//				is changed.
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::functionTreeView_Checked(Object* sender, TreeViewEventArgs* e)
	{
		String* nodeType = dynamic_cast<String*>(e->Node->Tag);

		static bool dontPropogateToParent = false; // to prevent infinite recursion when (un)checking all children
		static bool dontUpdateChildren = false; // to prevent state override when updating parent state

		if ((!dontUpdateChildren) && ((nodeType->StartsWith("Platform")) || (nodeType->StartsWith("NativeCategory")) ||
			(nodeType->StartsWith("DotNetCategory")) || (nodeType->StartsWith("DotNetClass")) || (nodeType->StartsWith("InternalSymbols"))))
		{
			// Parent node
			bool oldPropogateToParent = dontPropogateToParent;
			dontPropogateToParent = true;
			if (e->Node->Checked)
			{
				// Check all child nodes
				for (int i=0; i<e->Node->Nodes->Count; i++)
				{
					if (!e->Node->Nodes->Item[i]->Checked)
						e->Node->Nodes->Item[i]->Checked = true;
				}
			}
			else
			{
				// Uncheck all child nodes
				for (int i=0; i<e->Node->Nodes->Count; i++)
				{
					if (e->Node->Nodes->Item[i]->Checked)
						e->Node->Nodes->Item[i]->Checked = false;
				}
			}
			dontPropogateToParent = oldPropogateToParent;
		}

		if ((!dontPropogateToParent) && (e->Node->Parent != NULL))
		{
			bool oldUpdateChildren = dontUpdateChildren;
			dontUpdateChildren = true;

			// See if all sibling nodes are unchecked
			bool allUnchecked = true;
			for (int i=0; i<e->Node->Parent->Nodes->Count; i++)
			{
				if (e->Node->Parent->Nodes->Item[i]->Checked)
				{
					allUnchecked = false;
					break;
				}
			}

			if (allUnchecked)
			{
				// All nodes unchecked.  The parent node should be unchecked
				if (e->Node->Parent->Checked)
					e->Node->Parent->Checked = false;
			}
			else
			{
				// Some or all nodes checked.  The parent node should be checked
				if (!e->Node->Parent->Checked)
					e->Node->Parent->Checked = true;
			}

			dontUpdateChildren = oldUpdateChildren;
		}

		//If event is defined and this is a function node (node which has a parent but no children)
		//then the raise event
		if (OnFunctionCheckChanged && e->Node->Parent && e->Node->Nodes->Count == 0)
		{
			String * fullFunctionName = (dynamic_cast <String *> (e->Node->Tag))->Substring(nodeType->IndexOf(':') + 1);
			OnFunctionCheckChanged->Invoke(fullFunctionName, e->Node->Checked);
		}
	}


	//*************************************************************************
	// Method:		functionTreeView_AfterSelect
	// Description: Called when a tree view item is clicked
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::functionTreeView_AfterSelect(Object* sender, TreeViewEventArgs* e)
	{
		if (OnFunctionClicked)
		{
			String* nodeType = dynamic_cast<String*>(e->Node->Tag);
			if (nodeType == NULL)
			{
				OnFunctionClicked->Invoke(NULL);
				return;
			}

			if (!nodeType->StartsWith("Function"))
			{
				// Category or class name
				OnFunctionClicked->Invoke(NULL);
				return;
			}

			// Full function name follows the ':' in the tag string
			String* fullFunctionName = nodeType->Substring(nodeType->IndexOf(':') + 1);

			InterceptedFunctionDB *nativeDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			if (nativeDB->GetFunctionByName(fullFunctionName) && hideAW)
			{
				//if native and we are hiding A/W then trim A or W from name
				if (fullFunctionName->EndsWith("A") || fullFunctionName->EndsWith("W"))
					fullFunctionName = fullFunctionName->Remove(fullFunctionName->Length - 1, 1);
			}

			OnFunctionClicked->Invoke(fullFunctionName);
		}
	}


	//*************************************************************************
	// Method:		get_DisabledFunctionsTable
	// Description: Gets a list of the unchecked functions, organized by
	//				category.
	//
	// Parameters:
	//	None
	//
	// Return Value: Hash table containing disabled functions organized
	//		by category.
	//*************************************************************************
	Hashtable * FunctionTreeView::get_DisabledFunctionsTable()
	{
		Hashtable* disabledFunctionsTable = new Hashtable();

		// update the disabled information storage
		for (int i=0; i< this->nativeNode->Nodes->Count; i++)
		{
			// contains native functions, make a list of the functions which are un-checked
			ArrayList * functionsList = new ArrayList();
			for (int j=0; j< nativeNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				TreeNode *treeNode = nativeNode->Nodes->Item[i]->Nodes->Item[j];
				if (!treeNode->Checked)
				{
					//if hiding a/w and this is one of the hidden functions then add both to the list
					String *tagString = dynamic_cast <String*> (treeNode->Tag);
					if (!tagString)
						continue;

					int index = tagString->IndexOf(':');
					if (index == -1)
						continue;

					String *fullFunctionName = tagString->Substring(index + 1);
					if (hideAW && (treeNode->Text->CompareTo(fullFunctionName) != 0))
					{
						functionsList->Add(String::Concat(treeNode->Text, "A"));
						functionsList->Add(String::Concat(treeNode->Text, "W"));
					}
					else
						functionsList->Add (treeNode->Text);
				}
			}
			// add the root-node name along with a list of disabled functions
			String* categoryName = dynamic_cast<String*>(nativeNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name
			disabledFunctionsTable->Add (categoryName, functionsList);
		}

		for (int i=0; i< this->dotNetNode->Nodes->Count; i++)
		{
			// contains a .NET class list, enumerate the functions in each class
			ArrayList * functionsList;

			String* categoryName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			if (disabledFunctionsTable->ContainsKey(categoryName))
				functionsList = dynamic_cast<ArrayList*>(disabledFunctionsTable->Item[categoryName]);
			else
				functionsList = new ArrayList();

			for (int j=0; j< this->dotNetNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				// make a list of the functions which are un-checked
				for (int k=0; k< dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Count; k++)
				{
					if (! dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked)
					{
						String* funcName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Tag);
						funcName = funcName->Substring(funcName->IndexOf(':') + 1); // extract original full function name
						functionsList->Add (funcName);
					}
				}
			}
			// add the root-node name along with a list of disabled functions
			if (disabledFunctionsTable->ContainsKey(categoryName))
				disabledFunctionsTable->Item[categoryName] = functionsList;
			else
				disabledFunctionsTable->Add (categoryName, functionsList);
		}
		
		return disabledFunctionsTable;
	}

	//*************************************************************************
	// Method:		get_EnabledFunctionsTable
	// Description: Gets a list of the checked functions, organized by
	//				category.
	//
	// Parameters:
	//	None
	//
	// Return Value: Hash table containing enabled functions organized
	//		by category.
	//*************************************************************************
	Hashtable * FunctionTreeView::get_EnabledFunctionsTable()
	{
		Hashtable* enabledFunctionsTable = new Hashtable();

		// update the enabled information storage
		for (int i=0; i< this->nativeNode->Nodes->Count; i++)
		{
			// contains native functions, make a list of the functions which are checked
			ArrayList * functionsList = new ArrayList();
			for (int j=0; j< nativeNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				TreeNode *treeNode = nativeNode->Nodes->Item[i]->Nodes->Item[j];
				if (treeNode->Checked)
				{
					//if hiding a/w and this is one of the hidden functions then add both to the list
					String *tagString = dynamic_cast <String*> (treeNode->Tag);
					if (!tagString)
						continue;

					int index = tagString->IndexOf(':');
					if (index == -1)
						continue;

					String *fullFunctionName = tagString->Substring(index + 1);
					if (hideAW && (treeNode->Text->CompareTo(fullFunctionName) != 0))
					{
						functionsList->Add(String::Concat(treeNode->Text, "A"));
						functionsList->Add(String::Concat(treeNode->Text, "W"));
					}
					else
						functionsList->Add (treeNode->Text);
				}
			}
			//if any functions were checked, then add to table of enabled functions
			if (functionsList->Count > 0)
			{
				String* categoryName = dynamic_cast<String*>(nativeNode->Nodes->Item[i]->Tag);
				categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name
				enabledFunctionsTable->Add (categoryName, functionsList);
			}
		}

		bool addedNew = false;
		for (int i=0; i< this->dotNetNode->Nodes->Count; i++)
		{
			addedNew = false;
			// contains a .NET class list, enumerate the functions in each class
			ArrayList * functionsList;

			String* categoryName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			if (enabledFunctionsTable->ContainsKey(categoryName))
				functionsList = dynamic_cast<ArrayList*>(enabledFunctionsTable->Item[categoryName]);
			else
				functionsList = new ArrayList();

			for (int j=0; j< this->dotNetNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				// make a list of the functions which are un-checked
				for (int k=0; k< dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Count; k++)
				{
					if (dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked)
					{
						String* funcName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Tag);
						funcName = funcName->Substring(funcName->IndexOf(':') + 1); // extract original full function name
						functionsList->Add (funcName);
						addedNew = true;
					}
				}
			}
			if (addedNew)
			{
				// add the root-node name along with a list of disabled functions
				if (enabledFunctionsTable->ContainsKey(categoryName))
					enabledFunctionsTable->Item[categoryName] = functionsList;
				else
					enabledFunctionsTable->Add (categoryName, functionsList);
			}
		}
		
		return enabledFunctionsTable;
	}


	//*************************************************************************
	// Method:		set_DisabledFunctionsTable
	// Description: Sets the list of disabled functions, organized by
	//				category.
	//
	// Parameters:
	//	disabledFunctionsTable - Hash table containing disabled functions
	//		organized by category.
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::set_DisabledFunctionsTable(Hashtable* disabledFunctionsTable)
	{
		// restore check-marks from stored values for native functions
		for (int i=0; i< nativeNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(nativeNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			// check if this category has any disabled functions
			ArrayList * dFunctions = static_cast <ArrayList *> (disabledFunctionsTable->Item[categoryName]);

			if (!dFunctions || (dFunctions->Count == 0))
				continue;

			// cycle through the list and reset check-marks
			for (int j=0 ; j < nativeNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				TreeNode *treeNode = nativeNode->Nodes->Item[i]->Nodes->Item[j];
				String *tagString = dynamic_cast <String*> (treeNode->Tag);
				if (!tagString)
					continue;

				int index = tagString->IndexOf(':');
				if (index == -1)
					continue;

				String *fullFunctionName = tagString->Substring(index + 1);
				String *functionName1 = treeNode->Text;
				String *functionName2;

				//if hiding a/w functions and this is one of the hidden functions then process differently
				if (hideAW && (treeNode->Text->CompareTo(fullFunctionName) != 0))
				{
					functionName1 = String::Concat(treeNode->Text, "A");
					functionName2 = String::Concat(treeNode->Text, "W");
				}

				if (dFunctions->Contains(functionName1) || dFunctions->Contains(functionName2))
					treeNode->Checked = false;
				else
					treeNode->Checked = true;
			}

		}

		// restore check-marks from stored values for .NET functions
		for (int i=0; i< dotNetNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			// check if this category has any disabled functions
			ArrayList * dFunctions = static_cast <ArrayList *> (disabledFunctionsTable->Item[categoryName]);

			// go through each class node in this category
			for (int j=0 ; j < dotNetNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				// cycle through the list and reset check-marks
				for (int k=0 ; k < dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Count; k++)
				{
						String* funcName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Tag);
						funcName = funcName->Substring(funcName->IndexOf(':') + 1); // extract original full function name

						if ((dFunctions) && (dFunctions->Contains (funcName)))
							dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked = false;
						else
							dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked = true;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		set_EnabledFunctionsTable
	// Description: Sets the list of enabled functions, organized by
	//				category.
	//
	// Parameters:
	//	disabledFunctionsTable - Hash table containing enabled functions
	//		organized by category.
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::set_EnabledFunctionsTable(Hashtable* enabledFunctionsTable)
	{
		// restore check-marks from stored values for native functions
		for (int i=0; i< nativeNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(nativeNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			// check if this category has any enabled functions
			ArrayList * eFunctions = static_cast <ArrayList *> (enabledFunctionsTable->Item[categoryName]);
			if (!eFunctions || (eFunctions->Count == 0))
				continue;

			// cycle through the list and reset check-marks
			for (int j=0 ; j < nativeNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				TreeNode *treeNode = nativeNode->Nodes->Item[i]->Nodes->Item[j];
				String *tagString = dynamic_cast <String*> (treeNode->Tag);
				if (!tagString)
					continue;

				int index = tagString->IndexOf(':');
				if (index == -1)
					continue;

				String *fullFunctionName = tagString->Substring(index + 1);
				String *functionName1 = treeNode->Text;
				String *functionName2;

				//if hiding a/w functions and this is one of the hidden functions then process differently
				if (hideAW && (treeNode->Text->CompareTo(fullFunctionName) != 0))
				{
					functionName1 = String::Concat(treeNode->Text, "A");
					functionName2 = String::Concat(treeNode->Text, "W");
				}

				if (eFunctions->Contains(functionName1) || eFunctions->Contains(functionName2))
					treeNode->Checked = true;
				else
					treeNode->Checked = false;
			}
		}

		// restore check-marks from stored values for .NET functions
		for (int i=0; i< dotNetNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			// check if this category has any enabled functions
			ArrayList * eFunctions = static_cast <ArrayList *> (enabledFunctionsTable->Item[categoryName]);

			// go through each class node in this category
			for (int j=0 ; j < dotNetNode->Nodes->Item[i]->Nodes->Count; j++)
			{
				// cycle through the list and reset check-marks
				for (int k=0 ; k < dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Count; k++)
				{
						String* funcName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Tag);
						funcName = funcName->Substring(funcName->IndexOf(':') + 1); // extract original full function name

						if ((eFunctions) && (eFunctions->Contains (funcName)))
							dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked = true;
						else
							dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked = false;
				}
			}
		}

		//restore check-marks for internal symbols only if symbols are found
		if (this->Nodes->Count > 2)
		{
			TreeNode * symbolsNode = this->Nodes->Item[2];
			String* categoryName = "InternalSymbols";

			// check if this category has any enabled functions
			ArrayList * eFunctions = static_cast <ArrayList *> (enabledFunctionsTable->Item[categoryName]);
			if (eFunctions && (eFunctions->Count > 0))
			{
				// cycle through the list and reset check-marks
				for (int j=0 ; j < symbolsNode->Nodes->Count; j++)
				{
					TreeNode *treeNode = symbolsNode->Nodes->Item[j];
					String *tagString = dynamic_cast <String*> (treeNode->Tag);
					if (!tagString)
						continue;

					int index = tagString->IndexOf(':');
					if (index == -1)
						continue;

					String *fullFunctionName = tagString->Substring(index + 1);
					String *functionName = treeNode->Text;
					
					if (eFunctions->Contains(functionName))
						treeNode->Checked = true;
					else
						treeNode->Checked = false;
				}
			}
		}

	}

	//*************************************************************************
	// Method:		get_DisabledFunctionsList
	// Description: Gets a list of the unchecked functions
	//
	// Parameters:
	//	None
	//
	// Return Value: Array containing disabled functions
	//*************************************************************************
	ArrayList * FunctionTreeView::get_DisabledFunctionsList()
	{
		ArrayList* functionList = new ArrayList();

		// Get a simple list of functions from the hash table
		Hashtable* functionListByCategory = this->get_DisabledFunctionsTable();

		IEnumerator *enumerator = functionListByCategory->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList* categoryFuncs = dynamic_cast<ArrayList*>(functionListByCategory->Item[dynamic_cast<String*>(enumerator->Current)]);
			if (categoryFuncs)
				functionList->AddRange(categoryFuncs);
		}

		return functionList;
	}

	//*************************************************************************
	// Method:		get_EnabledFunctionsList
	// Description: Gets a list of the checked functions
	//
	// Parameters:
	//	None
	//
	// Return Value: Array containing enabled functions
	//*************************************************************************
	ArrayList * FunctionTreeView::get_EnabledFunctionsList()
	{
		ArrayList* functionList = new ArrayList();

		// Get a simple list of functions from the hash table
		Hashtable* functionListByCategory = this->get_EnabledFunctionsTable();

		IEnumerator *enumerator = functionListByCategory->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList* categoryFuncs = dynamic_cast<ArrayList*>(functionListByCategory->Item[dynamic_cast<String*>(enumerator->Current)]);
			if (categoryFuncs)
				functionList->AddRange(categoryFuncs);
		}

		return functionList;
	}

	//*************************************************************************
	// Method:		set_DisabledFunctionsList
	// Description: Sets the list of disabled functions
	//
	// Parameters:
	//	disabledFunctionsList - Array containing disabled functions
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::set_DisabledFunctionsList(ArrayList* disabledFunctionsList)
	{
		Hashtable* functionTable = new Hashtable();

		InterceptedFunctionDB *nativeDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		InterceptedFunctionDB *dotNetDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);

		// Convert the list of functions into a hash table organized by category
		if (disabledFunctionsList != NULL)
		{
			IEnumerator *enumerator = disabledFunctionsList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *functionName = dynamic_cast<String *>(enumerator->Current);
				InterceptedFunction *function = nativeDB->GetFunctionByName(functionName);
				if (function == NULL)
				{
					// Try .NET database
					function = dotNetDB->GetFunctionByName(functionName);
				}

				if (function != NULL)
				{
					ArrayList* categoryFunctions;
					if (functionTable->ContainsKey(function->Category))
						categoryFunctions = dynamic_cast<ArrayList*>(functionTable->Item[function->Category]);
					else
						categoryFunctions = new ArrayList();

					categoryFunctions->Add(functionName);

					if (functionTable->ContainsKey(function->Category))
						functionTable->Item[function->Category] = categoryFunctions;
					else
						functionTable->Add(function->Category, categoryFunctions);
				}
			}
		}

		this->set_DisabledFunctionsTable(functionTable);
	}

	//*************************************************************************
	// Method:		set_EnabledFunctionsList
	// Description: Sets the list of enabled functions
	//
	// Parameters:
	//	disabledFunctionsList - Array containing enabled functions
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::set_EnabledFunctionsList(ArrayList* enabledFunctionsList)
	{
		Hashtable* functionTable = new Hashtable();

		InterceptedFunctionDB *nativeDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		InterceptedFunctionDB *dotNetDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);

		// Convert the list of functions into a hash table organized by category
		if (enabledFunctionsList != NULL)
		{
			IEnumerator *enumerator = enabledFunctionsList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				String *functionName = dynamic_cast<String *>(enumerator->Current);
				InterceptedFunction *function = nativeDB->GetFunctionByName(functionName);
				if (function == NULL)
				{
					// Try .NET database
					function = dotNetDB->GetFunctionByName(functionName);
				}

				String * category = "";
				if (!function)
					category = "InternalSymbols";
				else
					category = function->Category;

				ArrayList* categoryFunctions;
				if (functionTable->ContainsKey(category))
					categoryFunctions = dynamic_cast<ArrayList*>(functionTable->Item[category]);
				else
					categoryFunctions = new ArrayList();

				categoryFunctions->Add(functionName);

				if (functionTable->ContainsKey(category))
					functionTable->Item[category] = categoryFunctions;
				else
					functionTable->Add(category, categoryFunctions);
			}
		}

		this->set_EnabledFunctionsTable(functionTable);
	}

	//*************************************************************************
	// Method:		GetChildNodeWithValue
	// Description: Gets the child node with the value specified
	//
	// Parameters:
	//	node - the node to search the children of
	//	value - the value to look for
	//	isNative - true if we are looking at native functions
	//
	// Return Value: the tree node if found, NULL otherwise
	//*************************************************************************
	TreeNode *FunctionTreeView::GetChildNodeWithValue(TreeNode *node, String *value, bool isNative)
	{
		int index;
		String *function;

		if (!node->Tag)
			return NULL;

		function = dynamic_cast <String*> (node->Tag);

		index = function->IndexOf(":");
		if (index >= 0)
			function = function->Substring(++index);

		//if we are hiding a/w then trim off the end of values to compare
		if (isNative && hideAW)
		{
			if (value->EndsWith("A") || value->EndsWith("W"))
				value = value->Remove(value->Length - 1, 1);

			if (function->EndsWith("A") || function->EndsWith("W"))
				function = function->Remove(function->Length - 1, 1);
		}

		if (function->Equals(value)) 
			return node;

		IEnumerator *enumerator = node->Nodes->GetEnumerator();
		while (enumerator->MoveNext())
		{
			TreeNode *childNode = dynamic_cast<TreeNode *>(enumerator->Current);
			TreeNode *foundNode = GetChildNodeWithValue(childNode, value, isNative);
			if (foundNode)
				return foundNode;
		}
		
		return NULL;
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
	bool FunctionTreeView::SelectNodeWithValue(String *value)
	{
		TreeNode *nodeToSelect = GetChildNodeWithValue(nativeNode, value, true);
		if (nodeToSelect)
		{
			this->SelectedNode = nodeToSelect;
			return true;
		}

		nodeToSelect = GetChildNodeWithValue(dotNetNode, value, true);
		if (nodeToSelect)
		{
			this->SelectedNode = nodeToSelect;
			return true;
		}

		if (this->Nodes->Count > 2)
		{
			TreeNode * symbolNode = this->Nodes->Item[2];
			nodeToSelect = GetChildNodeWithValue(symbolNode, value, true);
			if (nodeToSelect)
			{
				this->SelectedNode = nodeToSelect;
				return true;
			}
		}

		return false;
	}


	//*************************************************************************
	// Method:		DisableNativeCategory
	// Description: Unchecks all functions in the specified native category
	//
	// Parameters:
	//	category - Name of category (e.g. "FILE")
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::DisableNativeCategory(String* category)
	{
		for (int i=0; i< nativeNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(nativeNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			if (categoryName->Equals(category))
				nativeNode->Nodes->Item[i]->Checked = false;
		}
	}

	//*************************************************************************
	// Method:		DisableSingleNativeFunction
	// Description: Disables a native function
	//
	// Parameters:
	//	category - Name of category (e.g. "FILE")
	//	functionName - the name of the function to be disabled
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::DisableSingleNativeFunction(String* category, String* functionName)
	{
		for (int i=0; i< nativeNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(nativeNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			if (categoryName->Equals(category))
			{
				for (int j=0; j < nativeNode->Nodes->Item[i]->Nodes->Count; j++)
				{
					if (String::Compare (nativeNode->Nodes->Item[i]->Nodes->Item[j]->Text, functionName) == 0)
					{
						nativeNode->Nodes->Item[i]->Nodes->Item[j]->Checked = false;
						return;
					}
				}
				return;
			}
		}
	}

	//*************************************************************************
	// Method:		DisableDotNetCategory
	// Description: Unchecks all functions in the specified .net category
	//
	// Parameters:
	//	category - Name of category (e.g. "FILE")
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::DisableDotNetCategory(String* category)
	{
		for (int i=0; i< dotNetNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			if (categoryName->Equals(category))
				dotNetNode->Nodes->Item[i]->Checked = false;
		}
	}

	//*************************************************************************
	// Method:		DisableSingleDotNetFunction
	// Description: Disables a .net function
	//
	// Parameters:
	//	category - Name of category (e.g. "FILE")
	//	className - Name of the class that the function lives in
	//	functionName - the name of the function to be disabled
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::DisableSingleDotNetFunction(String *category, String* className, String *functionName)
	{
		for (int i=0; i< dotNetNode->Nodes->Count; i++)
		{
			String* categoryName = dynamic_cast<String*>(dotNetNode->Nodes->Item[i]->Tag);
			categoryName = categoryName->Substring(categoryName->IndexOf(':') + 1); // extract original category name

			if (categoryName->Equals(category))
			{
				for (int j=0; j < dotNetNode->Nodes->Item[i]->Nodes->Count; j++)
				{
					if (String::Compare (dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Text, className) == 0)
					{
						for (int k = 0; k < dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Count; k++)
						{
							if (String::Compare(dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Text, functionName) == 0)
							{
								dotNetNode->Nodes->Item[i]->Nodes->Item[j]->Nodes->Item[k]->Checked = false;
								return;
							}
						}
					}
				}
				return;
			}
		}
	}

	//*************************************************************************
	// Method:		AddChildNodeSorted
	// Description: Adds a child node in sorted order
	//
	// Parameters:
	//	parent - Parent node to insert into
	//  child - Node to insert
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::AddChildNodeSorted(TreeNode* parent, TreeNode* child)
	{
		int i;
		for (i=0; i<parent->Nodes->Count; i++)
		{
			if (parent->Nodes->Item[i]->Text->CompareTo(child->Text) > 0)
				break;
		}
		parent->Nodes->Insert(i, child);
	}

	//*************************************************************************
	// Method:		GetFunctionDefinitionFromFunctionName
	// Description: Returns the funtion definition of the function specified
	//
	// Parameters:
	//	functionName	- the name of the function for which to retrieve the
	//					definition
	//
	// Return Value: (InterceptedFunction)
	//*************************************************************************
	InterceptedFunction * FunctionTreeView::GetFunctionDefinitionFromFunctionName (String * functionName)
	{
		InterceptedFunction * function;
		//Check the native database first
		function = nativeInterceptedFunctionDB->GetFunctionByName (functionName);
		//If not found, check the .net database
		if (!function)
			function = dotNetInterceptedFunctionDB->GetFunctionByName (functionName);
		
		return function;
	}

	//*************************************************************************
	// Method:		toggleViewState
	// Description: Toggles visibility of nodes
	//
	// Parameters:
	//	isDotNet	- if we are toggling the dotnet node
	//	isVisible	- toggle to state
	//	
	//
	// Return Value: None
	//*************************************************************************
	void FunctionTreeView::toggleViewState(bool isDotNet, bool isVisible)
	{
		TreeNode * node;
		if (isDotNet)
			node = dotNetNode;
		else
			node = nativeNode;

		if (isVisible)
		{
			if (this->Nodes->Contains(node))
				this->Nodes->Remove (node);
		}
		else
		{
			if (!this->Nodes->Contains(node))
				this->Nodes->Add (node);
		}

	}
}
