//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CIProjectDetails.h
//
// DESCRIPTION: Contains implementation  for the class CIProjectDetails
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 12 Feb 2004		 Ady K	   File Created
//*******************************************************************************
#include "CIProjectDetails.h"

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// Method:		CIProjectDetails
	// Description: Constructor for the CIProjectDetails class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	CIProjectDetails::CIProjectDetails(System::Drawing::Image * wizImage) : WizardPage()
	{
		this->SubTitle = "Selecting libraries and methods";

		//initialize the various gui components
		this->InitializeComponent();
		
		//set the cool wizard image
		if (wizImage)
			this->pbWizardImg->Image = wizImage;

		//form the function treeview
		this->fTreeView = new FunctionTreeView(true, false);
		this->fTreeView->Dock = DockStyle::Fill;
		this->pnlFunctionTree->Controls->Add (fTreeView);

		//set the handler methods
		this->chkbxFaultInjection->CheckedChanged += new EventHandler (this, chkbxFaultInjection_Checked);
	}

	//*************************************************************************
	// Method:		InitializeComponent
	// Description: Init UI Components
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void CIProjectDetails::InitializeComponent(void)
	{
		this->pbWizardImg = new System::Windows::Forms::PictureBox();
		this->chkbxFaultInjection = new System::Windows::Forms::CheckBox();
		this->grpMain = new System::Windows::Forms::GroupBox();
		this->lblInfo2 = new System::Windows::Forms::Label();
		this->lblInfo = new System::Windows::Forms::Label();
		this->pnlFunctionTree = new System::Windows::Forms::Panel();
		this->grpMain->SuspendLayout();
		this->SuspendLayout();
		// 
		// pbWizardImg
		// 
		this->pbWizardImg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->pbWizardImg->Location = System::Drawing::Point(8, 8);
		this->pbWizardImg->Name = S"pbWizardImg";
		this->pbWizardImg->Size = System::Drawing::Size(112, 296);
		this->pbWizardImg->TabIndex = 1;
		this->pbWizardImg->TabStop = false;
		// 
		// grpMain
		// 
		this->grpMain->Controls->Add(this->chkbxFaultInjection);
		this->grpMain->Controls->Add(this->lblInfo2);
		this->grpMain->Controls->Add(this->lblInfo);
		this->grpMain->Controls->Add(this->pnlFunctionTree);
		this->grpMain->Location = System::Drawing::Point(128, 6);
		this->grpMain->Name = S"grpMain";
		this->grpMain->Size = System::Drawing::Size(496, 296);
		this->grpMain->TabIndex = 2;
		this->grpMain->TabStop = false;
		// 
		// lblInfo2
		// 
		this->lblInfo2->Location = System::Drawing::Point(296, 200);
		this->lblInfo2->Name = S"lblInfo2";
		this->lblInfo2->Size = System::Drawing::Size(192, 80);
		this->lblInfo2->TabIndex = 4;
		this->lblInfo2->Text = S"If you would like to intercept libraries which are not in this list, you may do s" 
			S"o by exiting this wizard and selecting Add Interception Library from under the T" 
			S"ools menu in Holodeck";
		// 
		// lblInfo
		// 
		this->lblInfo->Location = System::Drawing::Point(8, 16);
		this->lblInfo->Name = S"lblInfo";
		this->lblInfo->Size = System::Drawing::Size(264, 32);
		this->lblInfo->TabIndex = 1;
		this->lblInfo->Text = S"Select the libraries and methods that you wish to intercept in your application:";
		//
		// chkbxFaultInjection
		//
		this->chkbxFaultInjection->Location = System::Drawing::Point(296, 48);
		this->chkbxFaultInjection->Name = S"chkbxFaultInjection";
		this->chkbxFaultInjection->Size = System::Drawing::Size(192, 40);
		this->chkbxFaultInjection->TabIndex = 5;
		this->chkbxFaultInjection->Text = S"Include functions required for Holodeck Fault Injection";
		this->chkbxFaultInjection->Checked = false;
		// 
		// pnlFunctionTree
		// 
		this->pnlFunctionTree->BackColor = System::Drawing::Color::White;
		this->pnlFunctionTree->Location = System::Drawing::Point(8, 48);
		this->pnlFunctionTree->Name = S"pnlFunctionTree";
		this->pnlFunctionTree->Size = System::Drawing::Size(280, 240);
		this->pnlFunctionTree->TabIndex = 0;
		// 
		// CIProjectDetails
		// 
		//this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ClientSize = System::Drawing::Size(638, 316);
		this->Controls->Add(this->grpMain);
		this->Controls->Add(this->pbWizardImg);
		this->Name = S"CIProjectDetails";
		this->Text = S"CIProjectDetails";
		this->grpMain->ResumeLayout(false);
		this->ResumeLayout(false);

	}		



	//*************************************************************************
	// Method:		Dispose
	// Description: Dispose method
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void CIProjectDetails::Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}

	//*************************************************************************
	// Method:		set_IsAUTNative
	// Description: Property to set the default view of the functions treeview
	//*************************************************************************	
	void CIProjectDetails::set_IsAUTNative(bool value)
	{
		__isAUTNative = value;
		
		//Disable all functions
		fTreeView->NativeEnable = false;
		fTreeView->DotNetEnable = false;

		//if not native then
		if (!value)
		{
			fTreeView->ExpandDotNetFunctionTree();
			fTreeView->CollapseNativeFunctionTree();
			fTreeView->HideNative = true;
			fTreeView->HideDotNet = false;
			this->chkbxFaultInjection->Checked = true;
			this->chkbxFaultInjection->Enabled = false;
		}
		else
		{
			fTreeView->ExpandNativeFunctionTree();
			fTreeView->CollapseDotNetFunctionTree();
			fTreeView->HideNative = false;
			fTreeView->HideDotNet = true;
			this->chkbxFaultInjection->Checked = false;
			this->chkbxFaultInjection->Enabled = true;
		}
	}

	//*************************************************************************
	// Method:		chkbxFaultInjection_Checked
	// Description: Handler method for check change event
	//*************************************************************************	
	void CIProjectDetails::chkbxFaultInjection_Checked (Object * sender, EventArgs * e)
	{
		//If this is not a native project; then dont do anything, we will use this checkmark later
		if (!__isAUTNative)
			return;

		if (!faultDB)
			faultDB = FunctionDatabase::FaultFunctionDB::GetInstance();

		if (!funcDB)
			funcDB = FunctionDatabase::InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);

		Hashtable * funcTable;
		
		if (chkbxFaultInjection->Checked)
			funcTable = fTreeView->EnabledFunctionsTable;
		else
			funcTable = fTreeView->DisabledFunctionsTable;

		for (int i=0; i < faultDB->Faults->Count; i++)
		{
			FunctionDatabase::Fault * fault = dynamic_cast <Fault *> (faultDB->Faults->Item[i]);
			for (int j=0; j < fault->FaultFunctions->Count; j++)
			{
				FaultFunction * func = dynamic_cast <FaultFunction *> (fault->FaultFunctions->Item[j]);
				
				InterceptedFunction * ifunc = funcDB->GetFunctionByName (func->Name);
				if (ifunc == NULL)
					continue;

				if (! funcTable->ContainsKey (ifunc->Category))
					funcTable->Add (ifunc->Category, new ArrayList());	

				ArrayList * lst = dynamic_cast <ArrayList *> (funcTable->Item[ifunc->Category]);
				lst->Add (func->Name);
			}
		}

		if (chkbxFaultInjection->Checked)
			fTreeView->EnabledFunctionsTable = funcTable;
		else
			fTreeView->DisabledFunctionsTable = funcTable;
	}
}
