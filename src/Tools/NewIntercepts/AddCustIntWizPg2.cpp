//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg2.cpp
//
// DESCRIPTION: Contains implementation for the class AddCustIntWizPg2
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  --------  ----------------------------------------
// 01 Oct 2003		 P. Singh  File created.
// 06 Oct 2003		 Ady K	   Added functionality
//*******************************************************************************
#include "addcustintwizpg2.h"
#include "EditFunctionDetails.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		AddCustIntWizPg2
	// Description: Constructor for the AddCustIntWizPg2 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg2::AddCustIntWizPg2() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->subTitleText = "You may review the details of the functions you have selected. Select a function and click the \"Edit Details\" button to edit.";
		this->SubTitle = subTitleText;

		CustFuncSpecGrpBox = new GroupBox();
		EditDetailsBtn = new Button();
		lvCustFuncSpecList = new ListView();
		colEdited = new ColumnHeader();
		colFuncName = new ColumnHeader();
		colTypeName = new ColumnHeader();
		colLocation = new ColumnHeader();
		colFuncMods = new ColumnHeader();
		colRetType = new ColumnHeader();
		colNParams = new ColumnHeader();
		colParamTypes = new ColumnHeader();

		// 
		// CustFuncSpecGrpBox
		//
		this->CustFuncSpecGrpBox->Controls->Add(this->lvCustFuncSpecList);
		this->CustFuncSpecGrpBox->Controls->Add(this->EditDetailsBtn);

		this->CustFuncSpecGrpBox->Location = System::Drawing::Point(8, 16);
		this->CustFuncSpecGrpBox->Name = "CustFuncSpecGrpBox";
		this->CustFuncSpecGrpBox->Size = System::Drawing::Size(880, 360);
		this->CustFuncSpecGrpBox->TabStop = false;
		this->CustFuncSpecGrpBox->Text = "Custom Function Specification";

		// 
		// EditDetailsBtn
		// 
		this->EditDetailsBtn->Location = System::Drawing::Point(776, 323);
		this->EditDetailsBtn->Name = "EditDetailsBtn";
		this->EditDetailsBtn->Size = System::Drawing::Size(88, 24);
		this->EditDetailsBtn->TabIndex = 1;
		this->EditDetailsBtn->Text = "Edit details...";

		// 
		// lvCustFuncSpecList
		//
		this->lvCustFuncSpecList->Columns->Add(this->colEdited);
		this->lvCustFuncSpecList->Columns->Add(this->colFuncName);
		this->lvCustFuncSpecList->Columns->Add(this->colTypeName);
		this->lvCustFuncSpecList->Columns->Add(this->colLocation);
		this->lvCustFuncSpecList->Columns->Add(this->colFuncMods);
		this->lvCustFuncSpecList->Columns->Add(this->colRetType);
		this->lvCustFuncSpecList->Columns->Add(this->colNParams);
		this->lvCustFuncSpecList->Columns->Add(this->colParamTypes);

		this->lvCustFuncSpecList->Location = System::Drawing::Point(16, 24);
		this->lvCustFuncSpecList->MultiSelect = false;
		this->lvCustFuncSpecList->Name = "lvCustFuncSpecList";
		this->lvCustFuncSpecList->Size = System::Drawing::Size(848, 284);
		this->lvCustFuncSpecList->View = System::Windows::Forms::View::Details;
		this->lvCustFuncSpecList->FullRowSelect = true;
		this->lvCustFuncSpecList->HideSelection = false;

		// 
		// colEdited
		// 
		this->colEdited->Text = "";
		this->colEdited->Width = 25;
		// 
		// colFuncName
		// 
		this->colFuncName->Text = "Function";
		this->colFuncName->Width = 164;
		// 
		// colTypeName
		// 
		this->colTypeName->Text = "Type";
		this->colTypeName->Width = 114;
		// 
		// colLocation
		// 
		this->colLocation->Text = "Location";
		this->colLocation->Width = 118;
		// 
		// colFuncMods
		// 
		this->colFuncMods->Text = "Modifiers";
		this->colFuncMods->Width = 90;
		// 
		// colRetType
		// 
		this->colRetType->Text = "Return Type";
		this->colRetType->Width = 90;
		// 
		// colNParams
		// 
		this->colNParams->Text = "#Params";
		this->colNParams->Width = 55;
		// 
		// colParamTypes
		// 
		this->colParamTypes->Text = "Parameter Types";
		this->colParamTypes->Width = 188;

		// 
		// AddCustIntWizPg2
		// 
		this->ClientSize = System::Drawing::Size(896, 486);
		this->Controls->Add(this->CustFuncSpecGrpBox);
		this->Name = "AddCustIntWizPg2";
		this->CustFuncSpecGrpBox->ResumeLayout(false);
		this->ResumeLayout(false);

		EditDetailsBtn->Click += new EventHandler(this, onEditDetailsBtnClick);
		
		Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		String * holodeckPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		String * templatePath = dynamic_cast<String *> (key->GetValue(NEWINTERCEPTS_TPL_PATH_NAME, new String(".")));
		templatePath = String::Concat(holodeckPath, Path::DirectorySeparatorChar.ToString(), templatePath);
		if (! templatePath->EndsWith (Path::DirectorySeparatorChar.ToString()))
			templatePath = String::Concat (templatePath, Path::DirectorySeparatorChar.ToString());

		imgList = new System::Windows::Forms::ImageList();
		imgList->ImageSize = System::Drawing::Size (16,16);
		System::Drawing::Icon * icon = new System::Drawing::Icon (String::Concat (templatePath, "CHECKMRK.ICO"));
		if (icon)
			imgList->Images->Add (icon);

		this->lvCustFuncSpecList->SmallImageList = imgList;
	}

	//*************************************************************************
	// Method:		~AddCustIntWizPg2
	// Description: Destructor for the AddCustIntWizPg2 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg2::~AddCustIntWizPg2()
	{
	}

	//*************************************************************************
	// Method:		onEditDetailsBtnClick
	// Description: Event handler for edit details button
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg2::onEditDetailsBtnClick(Object *sender, EventArgs *e)
	{
		EditFunctionDetails * pgEditFuncSepc = new EditFunctionDetails();
		
		if (this->lvCustFuncSpecList->SelectedIndices->Count > 0)
		{
			int indexLvItem = this->lvCustFuncSpecList->SelectedIndices->Item[0];
			ListViewItem * lvi = this->lvCustFuncSpecList->Items->Item[indexLvItem];
			functionInformation * fInfo = dynamic_cast <functionInformation *> (lvi->Tag);
			functionInformation * updatedInfo = pgEditFuncSepc->displayFunctionInformation (fInfo, true);
			//If function information was edited then update the listview with the new info
			if (updatedInfo)
				updateListViewItemByIndex (indexLvItem, updatedInfo);				
		}

		pgEditFuncSepc = NULL;
	}

	//*************************************************************************
	// Method:		updateFunctionSpecificationView
	// Description: Updates the Function Specification ListView with the provided function
	//				information
	//
	// Parameters:
	//	funcInformation - the function information to be displayed in the listview
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg2::updateFunctionSpecificationView(ArrayList * funcInformation)
	{
		int i = 0;
		functionInformation * fInfo = NULL;

		//Clear the listview of all previous data
		this->lvCustFuncSpecList->Items->Clear();

		//Add the new items
		for (i=0; i < funcInformation->Count; i++)
		{
			TreeNode * node = dynamic_cast <TreeNode *> (funcInformation->Item[i]);
			fInfo = dynamic_cast <functionInformation *> (node->Tag);
			if (fInfo != NULL)
			{
				ListViewItem * lvi = this->lvCustFuncSpecList->Items->Add (new ListViewItem());
				this->updateListViewItemByIndex (lvi->Index, fInfo);
			}
		}
	}

	//*************************************************************************
	// Method:		updateListViewItemByIndex
	// Description: Updates a ListViewItem with the supplied information
	//
	// Parameters:
	//	index - index of the ListViewItem to update 
	//	mInfo - the update information
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg2::updateListViewItemByIndex (int index, functionInformation * mInfo)
	{
		ListViewItem * lvItem = this->lvCustFuncSpecList->Items->Item[index];

		//If no definition is provided or if definition name is null
		//Then query library for managed functions; and leave editable for native
		if ((mInfo->selectedDefinition == NULL) || (mInfo->selectedDefinition->Name == NULL))
		{
			if (!mInfo->isManaged)
			{
				mInfo->selectedDefinition = new InterceptedFunction();
				mInfo->selectedDefinition->Name = mInfo->functionName;
				mInfo->selectedDefinition->OriginalDll = Path::GetFileName(mInfo->libraryName);
			}
			else
				mInfo->selectedDefinition = getManagedDefinition (mInfo);

			//error handling
			if (!mInfo->selectedDefinition)
				return;
		}

		//Update some library information, even if already present
		mInfo->selectedDefinition->OriginalDll = Path::GetFileName(mInfo->libraryName);
		String * interceptedLib = Path::GetFileNameWithoutExtension(mInfo->selectedDefinition->OriginalDll);
		String * interceptedExt = mInfo->selectedDefinition->OriginalDll->Replace (interceptedLib, "");
		mInfo->selectedDefinition->InterceptedDll = mInfo->replacementLibraryName;

		//Check if category is defined, if not, then put default value
		if ((! mInfo->selectedDefinition->Category)||(mInfo->selectedDefinition->Category->Trim()->Length == 0))
			mInfo->selectedDefinition->Category = "CUSTOM";

		//Clear the old values
		lvItem->SubItems->Clear();

		//Start adding the new values
		//Add "Complete" column value
		//lvItem->Text = mInfo->isComplete ? "yes" : "no";

		//Add function name column value
		lvItem->SubItems->Add (getBaseName (mInfo->functionName));

		//Add type column value
		lvItem->SubItems->Add (getBaseName (mInfo->typeName));

		//Add location column value
		lvItem->SubItems->Add (Path::GetFileName (mInfo->libraryName));

		//Add Modifiers column value
		lvItem->SubItems->Add (mInfo->selectedDefinition->ModifiersString);

		//Add Return Type column value
		lvItem->SubItems->Add (mInfo->selectedDefinition->ReturnType);

		//Add #Params column value
		lvItem->SubItems->Add (mInfo->selectedDefinition->ParameterCount.ToString());

		//Add ParameterTypes column value
		String * paramTypeString = "";
		for (unsigned int i=0; i < mInfo->selectedDefinition->ParameterCount; i++)
		{
			InterceptedFunctionParameter * ifParam = dynamic_cast <InterceptedFunctionParameter *> (mInfo->selectedDefinition->Parameters->Item[i]);
			paramTypeString = String::Concat (paramTypeString, ifParam->Type, ",");
		}
		//Get rid of the ending ','
		if (paramTypeString->Length > 1)
			paramTypeString = paramTypeString->Substring (0, paramTypeString->Length - 1);
		lvItem->SubItems->Add (paramTypeString);

		//Store the methodInfo structure in the TAG of the lvi
		lvItem->Tag = mInfo;

		//Check to see if we have enough information about this method to allow code generation
		//If not then, set the return value to indicate that the definition is not complete
		if ((! mInfo->selectedDefinition->ReturnType) || (mInfo->selectedDefinition->ReturnType->Trim()->Length == 0))
		{
			//lvItem->ForeColor = System::Drawing::SystemColors::HotTrack;
			lvItem->ImageIndex = 0;
		}
		else
		{
			//lvItem->ForeColor = System::Drawing::SystemColors::ControlText;
			lvItem->ImageIndex = -1;
		}
	}

	//*************************************************************************
	// Method:		getManagedDefinition
	// Description: Uses reflection to get information about a managed method
	//
	// Parameters:
	//	fInfo	- structure which contains query data
	//
	// Return Value: (InterceptedFunction) the function definition
	//*************************************************************************
	InterceptedFunction * AddCustIntWizPg2::getManagedDefinition (functionInformation * fInfo)
	{
		
		InterceptedFunction * iFunc = new InterceptedFunction();
		
		//Set the function name
		iFunc->Name = fInfo->functionName;

		//Set the library location name
		iFunc->OriginalDll = Path::GetFileName(fInfo->libraryName);

		//LOAD THE ASSEMBLY TO GET MORE INFORMATION
		Assembly * asmLibrary = Assembly::LoadFrom (fInfo->libraryName);
		
		//Get the module
		Module * asmModule = asmLibrary->GetModule (fInfo->moduleName);

		//Get the type
		Type * moduleType = asmModule->GetType (fInfo->typeName);

		//Get the method 
		if (!fInfo->isConstructor)
		{
			MethodInfo * mi = moduleType->GetMethod (getBaseName(fInfo->functionName), fInfo->bindingFlags, NULL, fInfo->paramTypeArray, NULL);

			if (!mi)
				return NULL;

			//Get Parameter Info
			ParameterInfo * paramInfo[] = mi->GetParameters();

			//Set the modifiers
			if (mi->IsPrivate) iFunc->AddTypeModifier ("private");
			if (mi->IsPublic) iFunc->AddTypeModifier ("public");
			if (mi->IsVirtual) iFunc->AddTypeModifier ("virtual");
			if (mi->IsAbstract) iFunc->AddTypeModifier ("abstract");
			if (mi->IsStatic) iFunc->AddTypeModifier ("static");

			//Set return type
			iFunc->ReturnType = mi->ReturnType->ToString();

			//Set Parameter Info
			for (int parCount = 0; parCount < paramInfo->Count; parCount++)
			{
				ParameterInfo * pInfo = paramInfo[parCount];
				InterceptedFunctionParameter * ifParam = new InterceptedFunctionParameter ();
				ifParam->ID = pInfo->Position;
				ifParam->Name = pInfo->Name;
				ifParam->Type = pInfo->ParameterType->FullName;
				ifParam->Access = "";
				if (pInfo->IsIn)
					ifParam->Access = "IN ";
				
				if (pInfo->IsOut)
					ifParam->Access = String::Concat(ifParam->Access, "OUT");
				
				if (ifParam->Access->Length == 0)
					ifParam->Access = "NONE";

				iFunc->AddParameter (ifParam);
			}
		}
		else
		{
			ConstructorInfo * ci = moduleType->GetConstructor (fInfo->bindingFlags, NULL, fInfo->paramTypeArray, NULL);
			
			if (!ci)
				return NULL;

			//Get Parameter Info
			ParameterInfo * paramInfo[] = ci->GetParameters();
	
			//Set the modifiers
			if (ci->IsPrivate) iFunc->AddTypeModifier ("private");
			if (ci->IsPublic) iFunc->AddTypeModifier ("public");
			if (ci->IsVirtual) iFunc->AddTypeModifier ("virtual");
			if (ci->IsAbstract) iFunc->AddTypeModifier ("abstract");
			if (ci->IsStatic) iFunc->AddTypeModifier ("static");

			//Set return type
			iFunc->ReturnType = S"System.Void";

			//Set Parameter Info
			for (int parCount = 0; parCount < paramInfo->Count; parCount++)
			{
				ParameterInfo * pInfo = paramInfo[parCount];
				InterceptedFunctionParameter * ifParam = new InterceptedFunctionParameter ();
				ifParam->ID = pInfo->Position;
				ifParam->Name = pInfo->Name;
				ifParam->Type = pInfo->ParameterType->FullName;
				ifParam->Access = "";
				if (pInfo->IsIn)
					ifParam->Access = "IN ";
				
				if (pInfo->IsOut)
					ifParam->Access = String::Concat(ifParam->Access, "OUT");
				
				if (ifParam->Access->Length == 0)
					ifParam->Access = "NONE";

				iFunc->AddParameter (ifParam);
			}
		}

		return iFunc;
	}

	//*************************************************************************
	// Method:		getBaseName
	// Description: Gets the base function name from a full function name
	//				Example:
	//				Full function name = System.Windows.Forms.MessageBox.Show
	//				Base function name = Show
	//
	// Parameters:
	//	fullFunctionName	- the full funtion name
	//
	// Return Value: (String) the base function name
	//*************************************************************************
	String* AddCustIntWizPg2::getBaseName (String * fullFunctionName)
	{
		if (fullFunctionName == NULL)
			return "";

		int lastDot = 0;
		for (int i=0; i < fullFunctionName->Length; i++)
		{
			if (String::Compare (fullFunctionName->Chars[i].ToString(), ".") == 0)
				lastDot = i;
		}
		
		if ((lastDot != 0) && (lastDot != fullFunctionName->Length))
			return fullFunctionName->Substring (lastDot + 1, fullFunctionName->Length - lastDot - 1);
		else
			return fullFunctionName;
	}
}