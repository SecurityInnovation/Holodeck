//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectOptionsSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectOptionsSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#define _WIN32_WINNT 0x400 // IsDebuggerPresent requires NT4
#include "CLRIntercept.h"
#include "ProjectOptionsSelectionPage.h"
#include "defines.h"
#include <vcclr.h>
#include <stdio.h>
#include "..\..\common\RegistrationMgr.h"

using namespace HolodeckRegistration;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ProjectOptionsSelectionPage
	// Description: Constructor for the ProjectOptionsSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectOptionsSelectionPage::ProjectOptionsSelectionPage() : WizardPage()
	{
		long lastItemHeight = 0;
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Set log filtering and other project options";

		// initialize the native function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		nativeInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!nativeInterceptedFunctionDB->IsLoaded)
			nativeInterceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);

		pauseCheckBox = new CheckBox();
		processChainingCheckBox = new CheckBox();
		inheritSettingsCheckBox = new CheckBox();
		attachDebuggerCheckBox = new CheckBox();
		logFirstChanceCheckBox = new CheckBox();
		logFilterTreeView = new CustomTestProjectGenerator::FunctionTreeView(true, true);
		lnkLoggingOptionsLearnMore = new LinkLabel();

		// 
		// lnkLoggingOptionsLearnMore
		// 
		lnkLoggingOptionsLearnMore->Location = System::Drawing::Point (16, 224);
		lnkLoggingOptionsLearnMore->Name = "lnkLoggingOptionsLearnMore";
		lnkLoggingOptionsLearnMore->Size = System::Drawing::Size(240, 32);
		lnkLoggingOptionsLearnMore->Text = "Some features may not work with the current logging options, click here to learn more";
		lnkLoggingOptionsLearnMore->LinkClicked += new LinkLabelLinkClickedEventHandler (this, lnkLoggingOptionsLearnMore_Clicked);
		lnkLoggingOptionsLearnMore->LinkBehavior = System::Windows::Forms::LinkBehavior::HoverUnderline;
		lnkLoggingOptionsLearnMore->Visible = false;
		// 
		// pauseCheckBox
		// 
		pauseCheckBox->Location = System::Drawing::Point(266, 8);
		pauseCheckBox->Name = "pauseCheckBox";
		pauseCheckBox->Size = System::Drawing::Size(140, 50);
		pauseCheckBox->TabIndex = 0;
		pauseCheckBox->Text = "Pause Application on Start";
		lastItemHeight += 58;
		// 
		// processChainingCheckBox
		// 
		processChainingCheckBox->Location = System::Drawing::Point(266, lastItemHeight + 8);
		processChainingCheckBox->Name = "processChainingCheckBox";
		processChainingCheckBox->Size = System::Drawing::Size(140, 30);
		processChainingCheckBox->TabIndex = 1;
		processChainingCheckBox->Text = "Process Chaining";
		processChainingCheckBox->CheckedChanged += new EventHandler(this, processChainingCheckedChanged);
		lastItemHeight = processChainingCheckBox->Top + processChainingCheckBox->Height;
		// 
		// inheritSettingsCheckBox
		// 
		inheritSettingsCheckBox->Location = System::Drawing::Point(285, lastItemHeight);
		inheritSettingsCheckBox->Name = "inheritSettingsCheckBox";
		inheritSettingsCheckBox->Size = System::Drawing::Size(140, 50);
		inheritSettingsCheckBox->TabIndex = 2;
		inheritSettingsCheckBox->Text = "Child Processes Inherit Settings";
		lastItemHeight = inheritSettingsCheckBox->Top + inheritSettingsCheckBox->Height;
		// 
		// attachDebuggerCheckBox
		// 
		attachDebuggerCheckBox->Location = System::Drawing::Point(266, lastItemHeight+8);
		attachDebuggerCheckBox->Name = "attachDebuggerCheckBox";
		attachDebuggerCheckBox->Size = System::Drawing::Size(140, 50);
		attachDebuggerCheckBox->TabIndex = 3;
		attachDebuggerCheckBox->Text = "Attach Debugger and Log Exceptions";
		attachDebuggerCheckBox->CheckedChanged += new EventHandler(this, attachDebuggerCheckedChanged);
		lastItemHeight = attachDebuggerCheckBox->Top + attachDebuggerCheckBox->Height;
		// 
		// logFirstChanceCheckBox
		// 
		logFirstChanceCheckBox->Location = System::Drawing::Point(285, lastItemHeight);
		logFirstChanceCheckBox->Name = "attachDebuggerCheckBox";
		logFirstChanceCheckBox->Size = System::Drawing::Size(140, 50);
		logFirstChanceCheckBox->TabIndex = 3;
		logFirstChanceCheckBox->Text = "Log First Chance Exceptions";

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			processChainingCheckBox->Checked = false;
			processChainingCheckBox->Enabled = false;
			inheritSettingsCheckBox->Checked = false;
			inheritSettingsCheckBox->Enabled = false;
		}
		else
		{
			processChainingCheckBox->Checked = true;
			processChainingCheckBox->Enabled = true;
			inheritSettingsCheckBox->Checked = true;
			inheritSettingsCheckBox->Enabled = true;
		}

		if (IsDebuggerPresent())
		{
			// don't attach debugger by default if we are being debugged (usually a sign of being
			// run under a test harness)
			attachDebuggerCheckBox->Checked = false;
			logFirstChanceCheckBox->Enabled = false;
		}
		else
		{
			attachDebuggerCheckBox->Checked = true;
			logFirstChanceCheckBox->Enabled = true;
		}

		// 
		// logFilterTreeView
		// 
		logFilterTreeView->ImageIndex = -1;
		logFilterTreeView->Location = System::Drawing::Point(16, 8);
		logFilterTreeView->Name = "logFilterTreeView";
		logFilterTreeView->SelectedImageIndex = -1;
		logFilterTreeView->Size = System::Drawing::Size(240, 216);
		logFilterTreeView->TabIndex = 3;
		// 
		// Form
		// 
		Controls->Add(lnkLoggingOptionsLearnMore);
		Controls->Add(logFilterTreeView);
		Controls->Add(processChainingCheckBox);
		Controls->Add(inheritSettingsCheckBox);
		Controls->Add(attachDebuggerCheckBox);
		Controls->Add(logFirstChanceCheckBox);
		Controls->Add(pauseCheckBox);

		//Add event handler
		CheckedEventHandler = new CustomTestProjectGenerator::FunctionCheckChangedDelegate(this, functionCheckChanged_Handler);
		this->logFilterTreeView->OnFunctionCheckChanged += CheckedEventHandler;
	}

	//*************************************************************************
	// Method:		~ProjectOptionsSelectionPage
	// Description: Destructor for the ProjectOptionsSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectOptionsSelectionPage::~ProjectOptionsSelectionPage()
	{
	}


	//*************************************************************************
	// Method:		processChainingCheckedChanged
	// Description: called when state of the process chaining check box changes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectOptionsSelectionPage::processChainingCheckedChanged(Object *sender, EventArgs *e)
	{
		if (processChainingCheckBox->Checked)
		{
			inheritSettingsCheckBox->Enabled = true;
			inheritSettingsCheckBox->Checked = true;
		}
		else
		{
			inheritSettingsCheckBox->Enabled = false;
			inheritSettingsCheckBox->Checked = false;
		}
	}

	//*************************************************************************
	// Method:		attachDebuggerCheckedChanged
	// Description: called when state of the attach debugger check box changes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectOptionsSelectionPage::attachDebuggerCheckedChanged(Object *sender, EventArgs *e)
	{
		logFirstChanceCheckBox->Enabled = attachDebuggerCheckBox->Checked;
		if (!attachDebuggerCheckBox->Checked)
			logFirstChanceCheckBox->Checked = false;
	}

	ArrayList *ProjectOptionsSelectionPage::get_FilteredFunctionList()
	{
		return logFilterTreeView->DisabledFunctionsList;
	}


	void ProjectOptionsSelectionPage::set_FilteredFunctionList(ArrayList *value)
	{
		logFilterTreeView->DisabledFunctionsList = value;
	}


	void ProjectOptionsSelectionPage::SetApplication(String* value, bool attaching)
	{
		this->lnkLoggingOptionsLearnMore->Visible = false;
		this->missingFunctions = NULL;

		const wchar_t __pin* wszValue = PtrToStringChars(value);
		if (CLRIsModuleManaged(wszValue))
		{
			if (!attaching)
			{
				// .NET application, default state is disable all native, enable all .NET
				logFilterTreeView->NativeEnable = false;
				logFilterTreeView->DotNetEnable = true;

				logFilterTreeView->DisableDotNetCategory("MISCELLANEOUS");
				logFilterTreeView->DisableDotNetCategory("TIME");
				logFilterTreeView->DisableSingleDotNetFunction("FILE", "System.IO.BinaryReader", "Read()");
				logFilterTreeView->DisableSingleDotNetFunction("FILE", "System.IO.BinaryReader", "PeekChar()");

				logFilterTreeView->ExpandDotNetFunctionTree();
				logFilterTreeView->CollapseNativeFunctionTree();
			}
			else
			{
				MessageBoxA(NULL, "You have selected to attach to a .NET application.  Holodeck\n"
					"cannot log or schedule tests for .NET functions when attaching to\n"
					"a running application.  Native APIs and all built-in faults are\n"
					"unaffected by this limitation.\n", "Warning",
					MB_OK | MB_ICONWARNING);
			}
		}
		else
		{
			// Native application, default state is enable all native, disable all .NET
			logFilterTreeView->NativeEnable = true;
			logFilterTreeView->DotNetEnable = false;

			// Disable some functions by default
			logFilterTreeView->DisableNativeCategory("MEMORY");
			logFilterTreeView->DisableNativeCategory("TIME");
			logFilterTreeView->DisableNativeCategory("MISCELLANEOUS");
			logFilterTreeView->DisableNativeCategory("DANGEROUS");
			logFilterTreeView->DisableSingleNativeFunction("PROCESS", "LeaveCriticalSection");

			logFilterTreeView->ExpandNativeFunctionTree();
			logFilterTreeView->CollapseDotNetFunctionTree();
		}

		ArrayList * disabledFunctions = logFilterTreeView->DisabledFunctionsList;
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

		if (missingFunctions->Count > 0)
			this->lnkLoggingOptionsLearnMore->Visible = true;
	}

	//*************************************************************************
	// Method:		lnkLoggingOptionsLearnMore_Clicked
	// Description: Click event handler
	//*************************************************************************
	void ProjectOptionsSelectionPage::lnkLoggingOptionsLearnMore_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e)
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
	void ProjectOptionsSelectionPage::functionCheckChanged_Handler(String * functionName, bool IsChecked)
	{
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
			this->lnkLoggingOptionsLearnMore->Visible = true;
		else
			this->lnkLoggingOptionsLearnMore->Visible = false;
	}
}
