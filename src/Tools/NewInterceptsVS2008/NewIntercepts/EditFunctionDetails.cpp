//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		EditFunctionDetails.cpp
//
// DESCRIPTION: Contains implementation for the class EditFunctionDetails
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 10 Oct 2003		 Ady K	   Created
//*******************************************************************************
#include "EditFunctionDetails.h"


namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		EditFunctionDetails
	// Description: Constructor for the EditFunctionDetails class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	EditFunctionDetails::EditFunctionDetails()
	{
		//Initialize UI components
		InitializeComponent();

		//Set tool tip help
		this->toolTip->SetToolTip (this->cmbCategory, "Select a category or enter a new one. This will be displayed in the 'Category' column of the Holodeck log");

		//Load the function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		dotNetInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		if (!dotNetInterceptedFunctionDB->IsLoaded)
			dotNetInterceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);

		//Set the Category combobox options
		Array * categoryArray = dotNetInterceptedFunctionDB->GetCategoryArray();
		for (int i=0; i< categoryArray->Count; i++)
			this->cmbCategory->Items->Add (dynamic_cast <String *>(categoryArray->Item[i]));

		internalCopyofFuncInfo = NULL;

		txtEditor = new TextBox();
		txtEditor->BorderStyle = BorderStyle::FixedSingle;
		txtEditor->KeyPress += new System::Windows::Forms::KeyPressEventHandler(this, &AddNewIntercepts::EditFunctionDetails::txtEditor_KeyPress);
		txtEditor->LostFocus += new System::EventHandler(this, &AddNewIntercepts::EditFunctionDetails::txtEditor_LostFocus);
		lvParams->MouseDown += new MouseEventHandler (this, &AddNewIntercepts::EditFunctionDetails::lvParams_MouseDown);
		lvParams->DoubleClick += new EventHandler (this, &AddNewIntercepts::EditFunctionDetails::lvParams_MouseDoubleClick);
		this->subItemSelected = 0;

		//Add events to update function definition textbox
		this->cmbReturnType->TextChanged += new EventHandler (this, &AddNewIntercepts::EditFunctionDetails::definitionChanged_Handler);
		this->txtTypeMods->TextChanged += new EventHandler (this, &AddNewIntercepts::EditFunctionDetails::definitionChanged_Handler);
		this->txtCCMods->TextChanged += new EventHandler (this, &AddNewIntercepts::EditFunctionDetails::definitionChanged_Handler);
		//Other update events are linked to btnParamAdd, btnParamDel and dynamic txtBox
	}

	//*************************************************************************
	// Method:		definitionChanged_Handler
	// Description: Called whenever values which affect the function definition are
	//				changed in the UI.
	//				This method updates the function definition textbox with the
	//				updated definition
	//*************************************************************************
	void EditFunctionDetails::definitionChanged_Handler (Object * sender, EventArgs * e)
	{
		this->updateFunctionDefinition();
	}

	//*************************************************************************
	// Method:		txtEditor_KeyPress
	// Description: Handles the event when a key is pressed in the txtEditor textbox
	//
	//*************************************************************************
	void EditFunctionDetails::txtEditor_KeyPress (Object * sender , KeyPressEventArgs * e)
	{
		if ((e->KeyChar == 13) || (e->KeyChar == 27))
		{
			this->txtEditor->Hide();
		}
	}

	//*************************************************************************
	// Method:		txtEditor_LostFocus
	// Description: Handles the event when focus is lost from the txtEditor textbox
	//
	//*************************************************************************
	void EditFunctionDetails::txtEditor_LostFocus (Object * sender, EventArgs * e)
	{
		if (!txtEditor->Focused)
		{
			if (lviParam)
				lviParam->SubItems->Item[subItemSelected]->Text = txtEditor->Text;
			
			txtEditor->Hide();

			//Call the definition changed handler function
			this->definitionChanged_Handler (sender, e);
		}
	}

	//*************************************************************************
	// Method:		lvParams_MouseDown
	// Description: Handles the event a user clicks in the params listviewbox
	//
	//*************************************************************************
	void EditFunctionDetails::lvParams_MouseDown(Object * sender, MouseEventArgs * e)
	{
		if (txtEditor->Parent)
			txtEditor->Parent->Controls->Remove (txtEditor);
		txtEditor->Visible = false;
		lvParams->Controls->Add (txtEditor);
		lviParam = this->lvParams->GetItemAt (e->X, e->Y);
		firstMouseClickPoint.X = e->X ;
		firstMouseClickPoint.Y = e->Y ;
	}

	//*************************************************************************
	// Method:		lvParams_MouseDown
	// Description: Handles the event a user dbl-clicks in the params listviewbox
	//
	//*************************************************************************
	void EditFunctionDetails::lvParams_MouseDoubleClick(Object * sender, EventArgs * e)
	{
		if (lviParam != NULL)
		{
			// Check the subitem clicked
			int colStartX = 0; 
			int colEndX = lvParams->Columns->Item[0]->Width;

			for (int i = 0; i < lvParams->Columns->Count; i++)
			{
				if ((firstMouseClickPoint.X > colStartX) && (firstMouseClickPoint.X < colEndX))
				{
					subItemSelected = i;
					break; 
				}
				
				colStartX = colEndX;

				if (i < (lvParams->Columns->Count - 1))
				{
					colEndX += lvParams->Columns->Item[i + 1]->Width;
				}
			}

			txtEditor->Text = lviParam->SubItems->Item[subItemSelected]->Text;
			txtEditor->Width = colEndX - colStartX;
			txtEditor->Height = lviParam->Bounds.Bottom - lviParam->Bounds.Top;
			txtEditor->Location = System::Drawing::Point(colStartX , lviParam->Bounds.Y);
			txtEditor->Show();
			txtEditor->Focus();
		}
	}

	//*************************************************************************
	// Method:		displayFunctionInformation
	// Description: Displays the information supplied in the appropriate UI fields
	//
	// Parameters:
	// fInfo - information to be displayed
	// isEditable - to allow the user to edit the information or not
	//
	// Return Value: (functionInformation) the edited function information
	//*************************************************************************
	functionInformation * EditFunctionDetails::displayFunctionInformation (functionInformation * fInfo, bool isEditable)
	{
		if (!fInfo)
			return NULL;
		//Set the type values for the return type field
		setComboBoxValuesFromArray (this->cmbReturnType, fInfo->isManaged?getManagedTypes():getUnManagedTypes(), true);
		
		//Save an internal copy of the supplied function information
		//to remember non-displayable attributes
		this->internalCopyofFuncInfo = fInfo;
		
		//Set value for the function name
		this->txtName->Text = fInfo->functionName;
		//Set value for the function location
		this->txtLocation->Text = fInfo->libraryName;
		//Set value for the category
		this->cmbCategory->Text = fInfo->selectedDefinition->Category;
		//Set value for the return type
		this->cmbReturnType->Text = fInfo->selectedDefinition->ReturnType;
		//Set value for the function type modifiers
		String * typeMods = "";
		for (int mods = 0; mods < fInfo->selectedDefinition->TypeModifiers->Count; mods++)
			typeMods = String::Concat (typeMods, fInfo->selectedDefinition->TypeModifiers->Item[mods]->ToString(), S" ");
		this->txtTypeMods->Text = typeMods->Trim();
		String * ccMods = "";
		//Set value for the calling convention modifiers
		for (int mods = 0; mods < fInfo->selectedDefinition->CallingConventionModifiers->Count; mods++)
			ccMods = String::Concat (ccMods, fInfo->selectedDefinition->CallingConventionModifiers->Item[mods]->ToString(), S" ");
		this->txtCCMods->Text = ccMods->Trim();
		//Set value(s) for params group
		this->lvParams->Items->Clear();
		for (unsigned int i=0; i< fInfo->selectedDefinition->ParameterCount; i++)
		{
			InterceptedFunctionParameter * pInfo = dynamic_cast <InterceptedFunctionParameter *> (fInfo->selectedDefinition->Parameters->Item[i]);
			ListViewItem * lviParam = new ListViewItem (pInfo->ID.ToString());
			lviParam->SubItems->Add (pInfo->Name);
			lviParam->SubItems->Add (pInfo->Type);
			this->lvParams->Items->Add (lviParam);
		}

		//If managed function, then disable editing for some fields
		if (fInfo->isManaged)
		{
			this->cmbReturnType->Enabled = false;
			this->panelParamButtons->Enabled = false;
		}

		//Update the function definition textbox with a value
		this->updateFunctionDefinition();

		if (this->ShowDialog() == DialogResult::OK)
		{
			functionInformation * fi = gatherEditedInformation();
			if (!AddNewIntercepts::VerificationUtils::verifyFunctionDetails (fi))
			{
				String * errMsg = S"Some fields contain characters or other invalid data which may lead to a fault during code generation. Please fix them before proceeding to the next page.";
				MessageBox::Show (errMsg, "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Information);
			}
			
			return fi;
		}
		else
			return NULL;
	}

	//*************************************************************************
	// Method:		getUnManagedTypes
	// Description: Returns a list of unmanaged types
	//
	// Parameters:
	//	None
	//
	// Return Value: (arraylist) list of unmanaged types
	//*************************************************************************
	ArrayList * EditFunctionDetails::getUnManagedTypes()
	{
		ArrayList * nativeTypes = new ArrayList();
		nativeTypes->Add (new String("char"));
		nativeTypes->Add (new String("bool"));
		nativeTypes->Add (new String("short"));
		nativeTypes->Add (new String("int"));
		nativeTypes->Add (new String("long"));
		nativeTypes->Add (new String("float"));
		nativeTypes->Add (new String("double"));
		nativeTypes->Add (new String("unsigned char"));
		nativeTypes->Add (new String("unsigned short"));
		nativeTypes->Add (new String("unsigned int"));
		nativeTypes->Add (new String("void"));

		nativeTypes->Sort ();
		return nativeTypes;
	}

	//*************************************************************************
	// Method:		getManagedTypes
	// Description: Returns a list of managed types
	//
	// Parameters:
	//	None
	//
	// Return Value: (arraylist) list of managed types
	//*************************************************************************
	ArrayList * EditFunctionDetails::getManagedTypes()
	{
		ArrayList * managedTypes = new ArrayList();
		//Set the ReturnType combobox options
		Assembly * tempASM = Assembly::LoadWithPartialName ("mscorlib.dll");
		Type * typeTypeCode = tempASM->GetType ("System.TypeCode");
		MemberInfo * mi[] = typeTypeCode->GetMembers(static_cast <BindingFlags> (BindingFlags::Public|BindingFlags::Static));
		for (int i=0; i < mi->Length; i++)
			managedTypes->Add ((mi[i]->Name));

		return managedTypes;
	}

	//*************************************************************************
	// Method:		setComboBoxValuesFromArray
	// Description: Sets the values inside a combo-box based on the array provided
	//
	// Parameters:
	//	cbox - the combobox to add the values to
	//	values - the values to be added
	//	shouldClear - should the combobox be cleared before the new items are added
	//
	// Return Value: None
	//*************************************************************************
	void EditFunctionDetails::setComboBoxValuesFromArray (ComboBox * cbox, ArrayList * values, bool shouldClear)
	{
		if (shouldClear)
			cbox->Items->Clear();
		for (int i=0; i< values->Count; i++)
			cbox->Items->Add (values->Item[i]);
	}

	//*************************************************************************
	// Method:		btnParamAdd_Click
	// Description: Handles the event when the user wants to add a parameter
	//				Adds a new listviewitem to the params listview
	//*************************************************************************
	void EditFunctionDetails::btnParamAdd_Click(System::Object *  sender, System::EventArgs *  e)
	{
		ListViewItem * newLvi = lvParams->Items->Add(new ListViewItem(lvParams->Items->Count.ToString()));
		newLvi->SubItems->Add ("[double click to edit]");
		newLvi->SubItems->Add ("[param type]");
		
		//Call the definition changed handler function
		this->definitionChanged_Handler (sender, e);
	}

	//*************************************************************************
	// Method:		btnParamDel_Click
	// Description: Handles the event when the user wants to del a parameter
	//				Removes the selected param from the params listview
	//*************************************************************************
	void EditFunctionDetails::btnParamDel_Click(System::Object *  sender, System::EventArgs *  e)
	{
		if (lvParams->SelectedItems->Count > 0)
			lvParams->Items->Remove (lvParams->SelectedItems->Item[0]);

		//Call the definition changed handler function
		this->definitionChanged_Handler (sender, e);
	}

	//*************************************************************************
	// Method:		gatherEditedInformation
	// Description: Places the information from the various UI fields into
	//				a functionInformation structure and returns it
	//
	// Parameters:
	//	None
	//
	// Return Value: (functionInformation) Updated function information
	//*************************************************************************
	functionInformation * EditFunctionDetails::gatherEditedInformation()
	{
		functionInformation * editedInfo = internalCopyofFuncInfo;
		editedInfo->selectedDefinition = new InterceptedFunction();
		editedInfo->selectedDefinition->Name = this->txtName->Text;
		editedInfo->selectedDefinition->ReturnType = this->cmbReturnType->Text;
		
		ArrayList * typeMods = this->getStringTokens (this->txtTypeMods->Text);
		for (int t=0; t<typeMods->Count; t++)
			editedInfo->selectedDefinition->AddTypeModifier (dynamic_cast <String *> (typeMods->Item[t]));

		ArrayList * ccMods = this->getStringTokens (this->txtCCMods->Text);
		for (int t=0; t<ccMods->Count; t++)
			editedInfo->selectedDefinition->AddCallingConventionModifier (dynamic_cast <String *> (ccMods->Item[t]));

		editedInfo->selectedDefinition->Category = this->cmbCategory->Text;
		editedInfo->selectedDefinition->OriginalDll = System::IO::Path::GetFileName (this->txtLocation->Text);

		//Add the parameters
		for (int i=0; i < this->lvParams->Items->Count; i++)
		{
			ListViewItem * pItem = lvParams->Items->Item[i];
			InterceptedFunctionParameter * ifParam = new InterceptedFunctionParameter();
			ifParam->ID = Int32::Parse (pItem->Text);
			ifParam->Name = pItem->SubItems->Item[1]->Text;
			ifParam->Type = pItem->SubItems->Item[2]->Text;
			editedInfo->selectedDefinition->AddParameter (ifParam);
		}

		return editedInfo;
	}

	//*************************************************************************
	// Method:		getStringTokens
	// Description: Returns an arraylist of words contained in a string, delimited
	//				by a space
	//
	// Parameters:
	//	tString	- the string to search for words in
	//
	// Return Value: (ArrayList) an arraylist of words contained in tString
	//*************************************************************************	
	ArrayList * EditFunctionDetails::getStringTokens (String * tString)
	{
		tString = String::Concat (tString->Trim(), S" ");
		ArrayList * tokens = new ArrayList();
		int lastDelimiterIndex = 0;
		for (int i=0; i < tString->Length; i++)
		{
			if (String::Compare (tString->Substring (i, 1), S" ") == 0)
			{
				tokens->Add (tString->Substring (lastDelimiterIndex, i-lastDelimiterIndex)->Trim());
				lastDelimiterIndex = i;
			}
		}

		return tokens;
	}

	//*************************************************************************
	// Method:		updateFunctionDefinition
	// Description: Forms a function definition string and updates the definition
	//				textbox
	//
	// Parameters:
	//	None
	//
	// Return Value: (None)
	//*************************************************************************	
	void EditFunctionDetails::updateFunctionDefinition()
	{
		String * definition = "";
		//Add the function type modifiers
		definition = String::Concat (definition, this->txtTypeMods->Text, S" ");
		//Add the return type
		definition = String::Concat (definition, this->cmbReturnType->Text, S" ");
		//Add the calling convention modifiers
		definition = String::Concat (definition, this->txtCCMods->Text, S" ");
		//Add the function name
		definition = String::Concat (definition, this->txtName->Text, S" (");
		//Get param types string
		String * paramTypes = "";
		for (int p=0; p < this->lvParams->Items->Count; p++)
		{
			ListViewItem * lvi = this->lvParams->Items->Item[p];
			paramTypes = String::Concat (paramTypes, lvi->SubItems->Item[2]->Text);
			if (p < this->lvParams->Items->Count - 1)
				paramTypes = String::Concat (paramTypes, S", ");
		}
		//Add the param types
		definition = String::Concat (definition, paramTypes, S")");

		this->txtDefinition->Text = definition->Trim();
	}
}

