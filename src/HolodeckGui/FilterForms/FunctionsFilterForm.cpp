//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FunctionsFilterForm.cpp
//
// DESCRIPTION: Contains definition for the class FunctionsFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#include "defines.h"
#include "FunctionsFilterForm.h"
#include "../ImageListManager.h"
#include "../UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FunctionsFilterForm
	// Description: Constructor for the FunctionsFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionsFilterForm::FunctionsFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler)
	{
		InitializeUI();
		
		filterEnabled = false;

		// initialize the native function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		nativeInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!nativeInterceptedFunctionDB->IsLoaded)
			nativeInterceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		// add internal button handlers
		this->btnOK->Click += new EventHandler (this, OKClick_Func);
		this->btnCancel->Click += new EventHandler (this, CancelClick_Func);

		this->Icon = ImageListManager::getInstance()->AppIcon;
		
		// if available, add external button handlers
		if (OK_Click_EventHandler)
			this->btnOK->Click += OK_Click_EventHandler;
		if (Cancel_Click_EventHandler)
			this->btnCancel->Click += Cancel_Click_EventHandler;

		// initialize the disabled information storage
		disabledFunctionsTable = new Hashtable ();

		// add UI element changed handler
		TreeViewEventHandler * tvFunctions_AfterCheck_Handler = new TreeViewEventHandler (this, tvFunctions_AfterCheck_HandlerFunc);
		this->tvFunctions->add_AfterCheck (tvFunctions_AfterCheck_Handler);

		CheckedEventHandler = new CustomTestProjectGenerator::FunctionCheckChangedDelegate(this, functionCheckChanged_Handler);
		this->tvFunctions->OnFunctionCheckChanged += CheckedEventHandler;
		this->catchEvent = false;
	}

	//*************************************************************************
	// Method:		~FunctionsFilterForm
	// Description: Destructor for the DLLFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FunctionsFilterForm::~FunctionsFilterForm()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: UI Elements creation and initialization
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FunctionsFilterForm::InitializeUI()
	{
		this->lblInfo = new System::Windows::Forms::Label();
		this->tvFunctions = new CustomTestProjectGenerator::FunctionTreeView(true, true);
		this->btnOK = new System::Windows::Forms::Button();
		this->btnCancel = new System::Windows::Forms::Button();
		this->chbxEnableFilter = new System::Windows::Forms::CheckBox();
		this->lnkLoggingOptionsLearnMore = new LinkLabel();
		this->SuspendLayout();

		// 
		// lnkLoggingOptionsLearnMore
		// 
		lnkLoggingOptionsLearnMore->Location = System::Drawing::Point (8, 435);
		lnkLoggingOptionsLearnMore->Name = "lnkLoggingOptionsLearnMore";
		lnkLoggingOptionsLearnMore->Size = System::Drawing::Size(292, 32);
		lnkLoggingOptionsLearnMore->Text = "Some features may not work with the current logging options, click here to learn more.";
		lnkLoggingOptionsLearnMore->LinkClicked += new LinkLabelLinkClickedEventHandler (this, lnkLoggingOptionsLearnMore_Clicked);
		lnkLoggingOptionsLearnMore->LinkBehavior = System::Windows::Forms::LinkBehavior::HoverUnderline;
		lnkLoggingOptionsLearnMore->Visible = false;
		// 
		// lblInfo
		// 
		this->lblInfo->Location = System::Drawing::Point(8, 8);
		this->lblInfo->Name = "lblInfo";
		this->lblInfo->Size = System::Drawing::Size(280, 30);
		this->lblInfo->TabIndex = 0;
		this->lblInfo->Text = "Select the functions to display.  This will change which log entries will be displayed in the current log pane.";
		// 
		// tvFunctions
		// 
		this->tvFunctions->Location = System::Drawing::Point(8, 42);
		this->tvFunctions->Name = "tvFunctions";
		this->tvFunctions->Size = System::Drawing::Size(280, 388);
		// 
		// btnOK
		// 
		this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->btnOK->Location = System::Drawing::Point(104, 440);
		this->btnOK->Name = "btnOK";
		this->btnOK->Size = System::Drawing::Size(88, 24);
		this->btnOK->TabIndex = 8;
		this->btnOK->Text = "OK";
		// 
		// btnCancel
		// 
		this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->btnCancel->Location = System::Drawing::Point(200, 440);
		this->btnCancel->Name = "btnCancel";
		this->btnCancel->Size = System::Drawing::Size(88, 24);
		this->btnCancel->TabIndex = 9;
		this->btnCancel->Text = "Cancel";
		// 
		// chbxEnableFilter
		// 
		this->chbxEnableFilter->Location = System::Drawing::Point(8, 440);
		this->chbxEnableFilter->Name = "chbxEnableFilter";
		this->chbxEnableFilter->Size = System::Drawing::Size(88, 16);
		this->chbxEnableFilter->TabIndex = 10;
		this->chbxEnableFilter->Text = "Enable Filter";
		// 
		// FunctionsFilterForm
		// 
		this->AcceptButton = this->btnOK;
		this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->CancelButton = this->btnCancel;
		this->ClientSize = System::Drawing::Size(298, 472);
		this->Controls->Add(lnkLoggingOptionsLearnMore);
		this->Controls->Add(btnCancel);
		this->Controls->Add(btnOK);
		this->Controls->Add(tvFunctions);
		this->Controls->Add(lblInfo);
		this->Controls->Add(chbxEnableFilter);

		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ControlBox = false;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;

		this->Name = "FunctionsFilterForm";
		this->Text = "Functions Filter";
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
		this->ResumeLayout(false);
	}

	//*************************************************************************
	// Method:		OKClick_Func
	// Description: Internal handler for FunctionsFilterForm OK button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FunctionsFilterForm::OKClick_Func (System::Object * sender, EventArgs * e)
	{
		// update the disabled information storage
		disabledFunctionsTable = this->tvFunctions->get_DisabledFunctionsTable();
		
		// update filter enable status
		this->FilterEnabled = this->chbxEnableFilter->Checked;
	}
	//*************************************************************************
	// Method:		CancelClick_Func
	// Description: Internal handler for FunctionsFilterForm Cancel button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FunctionsFilterForm::CancelClick_Func(System::Object * sender, EventArgs * e)
	{
		// restore check-marks from stored values
		this->tvFunctions->set_DisabledFunctionsTable(disabledFunctionsTable);

		// restore to old enable setting
		this->chbxEnableFilter->Checked = this->FilterEnabled;
	}


	//*************************************************************************
	// Method:		tvFunctions_AfterCheck_HandlerFunc
	// Description: Internal handler for checkbox changes event
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FunctionsFilterForm::tvFunctions_AfterCheck_HandlerFunc(System::Object * sender, TreeViewEventArgs * e)
	{
		this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		displayWarning
	// Description: Resizes the form and displays the warning help label
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FunctionsFilterForm::displayWarning()
	{
		this->ClientSize = System::Drawing::Size(298, 507);
		this->btnCancel->Top += 35;
		this->btnOK->Top += 35;
		this->lnkLoggingOptionsLearnMore->Visible = true;
	}

	//*************************************************************************
	// Method:		hideWarning
	// Description: Resizes the form and hides the warning help label
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FunctionsFilterForm::hideWarning()
	{
		this->ClientSize = System::Drawing::Size(298, 472);
		this->btnCancel->Top -= 35;
		this->btnOK->Top -= 35;
		this->lnkLoggingOptionsLearnMore->Visible = false;
	}

	//*************************************************************************
	// Method:		lnkLoggingOptionsLearnMore_Clicked
	// Description: Click event handler
	//*************************************************************************
	void FunctionsFilterForm::lnkLoggingOptionsLearnMore_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e)
	{
		if (UIEventRegistry::getInstance()->OnURLClicked)
		{
			UIEventRegistry::getInstance()->OnURLClicked->Invoke(String::Concat(S"external*file://",
				GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), S"/", LOGGING_OPTIONS_HELP_FILE));
		}
	}

	//*************************************************************************
	// Method:		functionCheckChanged_Handler
	// Description: Click event handler for when treenodes are un\checked
	//*************************************************************************
	void FunctionsFilterForm::functionCheckChanged_Handler(String * functionName, bool IsChecked)
	{
		if (!this->catchEvent)
			return;

		if (!missingFunctions)
			return;

		if (missingFunctions->Contains (functionName))
		{
			missingFunctions->Remove (functionName);
		}
		else if (!IsChecked)
		{
			InterceptedFunction * func = nativeInterceptedFunctionDB->GetFunctionByName (functionName);
			if (!func)
				return;
			
			if (exceptionCases->Contains (func->Name))
				return;

			if (requiredCategories->Contains (func->Category->ToUpper()))
				missingFunctions->Add (func->Name);
		}

		if (missingFunctions->Count > 0)
		{
			if (this->lnkLoggingOptionsLearnMore->Visible == false)
				this->displayWarning();
		}
		else
		{
			if (this->lnkLoggingOptionsLearnMore->Visible == true)
				this->hideWarning();
		}
	}

	//*************************************************************************
	// Method:		set_FunctionsList
	// Description: Property for setting functions list and also calculating
	//				missing required functions
	//*************************************************************************	
	void FunctionsFilterForm::set_FunctionsList(ArrayList* value)
	{
		missingFunctions = NULL;
		tvFunctions->DisabledFunctionsList = value;
		this->catchEvent=true;

		ArrayList * disabledFunctions = value;
		missingFunctions = new ArrayList();
	
		if (!requiredCategories)
		{
			requiredCategories = new ArrayList();
			requiredCategories->Add (S"PROCESS");
			requiredCategories->Add (S"REGISTRY");
			requiredCategories->Add (S"LIBRARY");
			requiredCategories->Add (S"FILE");
			requiredCategories->Add (S"COM");
		}

		if (!exceptionCases)
		{
			exceptionCases = new ArrayList();
			exceptionCases->Add (S"LeaveCriticalSection");
		}

		InterceptedFunction * func = NULL;
		for (int i=0; i < disabledFunctions->Count; i++)
		{
			String * funcName = dynamic_cast <String*> (disabledFunctions->Item[i]);
			func = nativeInterceptedFunctionDB->GetFunctionByName (funcName);
			if (!func)
				continue;
			
			if (exceptionCases->Contains (func->Name))
				continue;

			if (funcName->EndsWith (S"A") && nativeInterceptedFunctionDB->GetFunctionByName (String::Concat (funcName->Substring (0, funcName->Length - 1), S"W")))
				continue;

			if (requiredCategories->Contains (func->Category->ToUpper()))
				missingFunctions->Add (func->Name);
		}

		if (missingFunctions->Count > 0 && this->lnkLoggingOptionsLearnMore->Visible == false)
				this->displayWarning();
	}

}