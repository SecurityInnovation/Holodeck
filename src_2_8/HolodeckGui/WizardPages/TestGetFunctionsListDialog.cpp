//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestInParamSpecificationPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestInParamSpecificationPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 Dec 2003		 Ady K		 File created.
//*************************************************************************

#include "TestGetFunctionsListDialog.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestGetFunctionsListDialog
	// Description: Constructor for the TestGetFunctionsListDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestGetFunctionsListDialog::TestGetFunctionsListDialog(String* title, bool EnableMoveButtons, Array * processSymbols)
	{
		this->processSymbols = processSymbols;
		
		InitializeComponent();

		this->btnMoveUp->Click += new EventHandler (this, btnMoveUp_Click);
		this->btnMoveDn->Click += new EventHandler (this, btnMoveDn_Click);

		this->Text = title;
		this->ResizeRedraw = false;

		if (!EnableMoveButtons)
		{
			this->btnMoveDn->Enabled = false;
			this->btnMoveUp->Enabled = false;
		}
		
		tvFunctions = new CustomTestProjectGenerator::FunctionTreeView(true, false);

		// 
		// treeView1
		// 
		this->tvFunctions->ImageIndex = -1;
		this->tvFunctions->Dock = DockStyle::Fill;
		this->tvFunctions->Name = S"tvFunctions";
		this->tvFunctions->SelectedImageIndex = -1;
		this->tvFunctions->Size = System::Drawing::Size(160, 184);
		this->tvFunctions->TabIndex = 5;
		this->tvFunctions->NativeEnable = false;
		this->tvFunctions->DotNetEnable = false;

		this->pnlTreeView->Controls->Add(this->tvFunctions);

		this->lvItems->SelectedIndexChanged += new EventHandler (this, lvItems_SelectChanged);
		this->Resize += new EventHandler (this, onFormResize);
	}


	//*************************************************************************
	// Method:		DisplayDialog
	// Description: Displays the dialog and update the UI with the functionList
	//				passed in.
	//
	// Parameters:
	//	functionList	- the list of selected functions
	//
	// Return Value: (ArrayList) updated list of selected functions
	//*************************************************************************
	ArrayList * TestGetFunctionsListDialog::DisplayDialog(System::Windows::Forms::IWin32Window * parentWindow, ArrayList * functionList)
	{
		if (!functionList)
			functionList = new ArrayList();

		//Define the Checked event handler
		CustomTestProjectGenerator::FunctionCheckChangedDelegate * CheckedEventHandler = new CustomTestProjectGenerator::FunctionCheckChangedDelegate(this, functionCheckChanged_Handler);

		if (!symbolsNode)
		{
			String * moduleName = "";
			symbolsNode = new TreeNode("[symbols not found]");
			symbolsNode->Tag = S"InternalSymbols:EmptyList";
			this->lnkSymbolsLearnMore->Visible = true;
			//add process symbols, if any
			if (this->processSymbols != NULL && this->processSymbols->Length > 0)
			{
				IEnumerator *enumerator = processSymbols->GetEnumerator();
				while (enumerator->MoveNext())
				{
					String * symbol = dynamic_cast <String *> (enumerator->Current);
					String * functionName = symbol->Substring (symbol->IndexOf (" "))->Replace ("!", "")->Replace(" ", "");
					if (moduleName->Length == 0)
					{
						moduleName = symbol->Substring (0, symbol->IndexOf (" "))->Replace ("!", "");
						symbolsNode->Text = String::Concat(moduleName, " functions");
						symbolsNode->Tag = String::Concat("InternalSymbols:", moduleName);
						this->lnkSymbolsLearnMore->Visible = false;
					}

					if (symbolsNode != NULL)
					{
						InterceptedFunction *interceptedFunction = new InterceptedFunction();
						interceptedFunction->Name = functionName;
						interceptedFunction->Category = "InternalSymbols";

						TreeNode* funcNode = new TreeNode(interceptedFunction->Name);
						// save the node type and full function name as a tag
						funcNode->Tag = String::Concat("Function:", interceptedFunction->Name);
						funcNode->Checked = false;
						tvFunctions->AddChildNodeSorted(symbolsNode, funcNode);
					}
				}
			}
			//Add the symbols node
			this->tvFunctions->Nodes->Add (symbolsNode);
		}

		//Set the enabled list
		this->tvFunctions->EnabledFunctionsList = functionList;

		//Update the listview manually the first time
		this->lvItems->Items->Clear();
		for (int i=0; i < functionList->Count; i++)
			this->functionCheckChanged_Handler (dynamic_cast <String *> (functionList->Item[i]), true);

		//Set the checked event handler
		this->tvFunctions->OnFunctionCheckChanged += CheckedEventHandler;
		//Display dialog
		this->ShowDialog(parentWindow);
		//Remove the checked event handler
		this->tvFunctions->OnFunctionCheckChanged -= CheckedEventHandler;

		//Return the functions list from the listview where the user may
		//have changed the order of the functions
		return this->GetArrayListFromListView();
	}

	//*************************************************************************
	// Method:		functionCheckChanged_Handler
	// Description: Called when the user checks a funtion in the treeview
	//				Updates the listview of selected functions
	//
	// Parameters:
	//	functionName - the list of selected functions
	//
	// Return Value: (ArrayList) updated list of selected functions
	//*************************************************************************
	void TestGetFunctionsListDialog::functionCheckChanged_Handler(String * functionName, bool IsChecked)
	{
		String * originalDLL = "";

		InterceptedFunction * func = this->tvFunctions->GetFunctionDefinitionFromFunctionName (functionName);
		if (!func)
			originalDLL = "Internal Symbol";
		else
			originalDLL = func->OriginalDll;

		if (IsChecked)
		{
			String * funcNameWithoutClass = functionName;
			
			int dotIndex = functionName->LastIndexOf('.');
			if (dotIndex != -1)
				funcNameWithoutClass = functionName->Substring(dotIndex + 1);

			//The listviewitem will have the function name only
			ListViewItem * newItem = new ListViewItem (funcNameWithoutClass);
			//The fullname (with class) will be stored in the tag information
			newItem->Tag = functionName;
			newItem->SubItems->Add (originalDLL);
			this->lvItems->Items->Add (newItem);
		}
		else
		{
			for (int i=0; i < this->lvItems->Items->Count; i++)
			{
				ListViewItem * lvItem = this->lvItems->Items->Item[i];
				String * fullFunctionName = dynamic_cast <String *> (lvItem->Tag);
				if ((fullFunctionName->CompareTo (functionName) == 0) &&
					(lvItem->SubItems->Item[1]->Text->CompareTo (originalDLL) == 0))
				{
					this->lvItems->Items->Remove (lvItem);
					break;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		btnMoveUp_Click
	// Description: Called when the user clicks the Up arrow button
	//				Method moves the selected listviewitem up by one item
	//
	//*************************************************************************
	void TestGetFunctionsListDialog::btnMoveUp_Click(System::Object *  sender, System::EventArgs *  e)
	{
		//If no items are selected, then return
		if (this->lvItems->SelectedItems->Count < 1)
			return;

		//If the first item is selected, then return
		if (this->lvItems->SelectedItems->Item[0]->Index == 0)
			return;

		//Get the listviewitem
		ListViewItem * lvItem = this->lvItems->SelectedItems->Item[0];
		//Get the index of where to place it
		int index = lvItem->Index - 1;
		//Remove it from the list
		this->lvItems->Items->Remove (lvItem);
		//Add it again at the new index
		this->lvItems->Items->Insert (index, lvItem);
		//Ensure that the selected item is visible
		this->lvItems->EnsureVisible (index);
	}

	//*************************************************************************
	// Method:		btnMoveDn_Click
	// Description: Called when the user clicks the Down arrow button
	//				Method moves the selected listviewitem down by one item
	//
	//*************************************************************************
	void TestGetFunctionsListDialog::btnMoveDn_Click(System::Object *  sender, System::EventArgs *  e)
	{
		//If no items are selected, then return
		if (this->lvItems->SelectedItems->Count < 1)
			return;

		//If the last item is selected, then return
		if (this->lvItems->SelectedItems->Item[0]->Index == this->lvItems->Items->Count - 1)
			return;
		
		//Get the listviewitem
		ListViewItem * lvItem = this->lvItems->SelectedItems->Item[0];
		//Get the index of where to place it
		int index = lvItem->Index + 1;
		//Remove it from the list
		this->lvItems->Items->Remove (lvItem);
		//Add it again at the new index
		this->lvItems->Items->Insert (index, lvItem);
		//Ensure that the selected item is visible
		this->lvItems->EnsureVisible (index);
	}

	//*************************************************************************
	// Method:		GetArrayListFromListView
	// Description: Reads the item tags from the listview and arranges them in
	//				an arraylist
	//
	// Parameters:
	//	None
	//
	// Return Value: (ArrayList) list of selected functions in user specified order
	//*************************************************************************
	ArrayList * TestGetFunctionsListDialog::GetArrayListFromListView()
	{
		ArrayList * functions = new ArrayList (this->lvItems->Items->Count);
		
		for (int i=0; i < this->lvItems->Items->Count; i++)
			functions->Add (dynamic_cast <String *> (this->lvItems->Items->Item[i]->Tag));

		return functions;
	}

	//*************************************************************************
	// Method:		lvItems_SelectChanged
	// Description: Called when the user changes selection in the listview
	//				This method selects the corresponding method in the treeview
	//
	//*************************************************************************
	void TestGetFunctionsListDialog::lvItems_SelectChanged (Object * sender, EventArgs * e)
	{
		if (this->lvItems->SelectedItems->Count < 1)
			return;

		String * itemText = dynamic_cast <String *> (this->lvItems->SelectedItems->Item[0]->Tag);
		this->tvFunctions->SelectNodeWithValue (itemText);
	}

	//*************************************************************************
	// Method:		onFormResize
	// Description: Called when the dialog is resized
	//*************************************************************************	
	void TestGetFunctionsListDialog::onFormResize (Object * sender, EventArgs * e)
	{
		//Resize the left side of the dialog
		this->pnlSelectFunctions->Width = this->Width / 2;

		//Relocate the up and down buttons to be in vertically aligned
		this->pnlUpDownButtons->Top = this->pnlSelectedMethods->Height / 2 - this->pnlUpDownButtons->Height /2;
	}

	//*************************************************************************
	// Method:		lnkSymbolsLearnMore_LinkClicked
	// Description: Called when the learn more about symbols link is clicked
	//*************************************************************************	
	void TestGetFunctionsListDialog::lnkSymbolsLearnMore_LinkClicked(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e)
	{
		if (UIEventRegistry::getInstance()->OnURLClicked)
		{
			UIEventRegistry::getInstance()->OnURLClicked->Invoke(String::Concat(S"external*file://",
				GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), S"/", USING_SYMBOLS_HELP_FILE));
		}
	}
}