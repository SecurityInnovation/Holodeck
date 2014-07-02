//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomInterceptProject.h
//
// DESCRIPTION: Contains implementation  for the class CustomInterceptProject
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 12 Feb 2004		 Ady K	   File Created
//*******************************************************************************

#include "CustomInterceptProject.h"

namespace CustomTestProjectGenerator
{

	//*************************************************************************
	// Method:		CustomInterceptProject
	// Description: Constructor for the CustomInterceptProject class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	CustomInterceptProject::CustomInterceptProject(System::Drawing::Image * wizImage) : WizardPage()
	{
		this->SubTitle = "Selecting the application to be tested";
		
		this->InitializeComponent();
		
		this->lblInfo->Text = "This wizard will assist you in creating a custom interception project. You may test an application by having your test project either launch it or attach to an already running instance of it.\n\nIn the case of attaching to a running instance, you will be required to specify the ProcessID in the code which will be generated for you.";
		
		if (wizImage)
			this->pbWizardImg->Image = wizImage;

		//set event handlers
		btnBrowseApp->Click += new EventHandler (this, btnBrowseApp_ClickEventHandler);
		rbNativeApp->CheckedChanged += new EventHandler (this, radio_CheckedChangedHandler);
		rbDotNetApp->CheckedChanged += new EventHandler (this, radio_CheckedChangedHandler);
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
	void CustomInterceptProject::InitializeComponent(void)
	{
		this->pbWizardImg = new System::Windows::Forms::PictureBox();
		this->grpMain = new System::Windows::Forms::GroupBox();
		this->rbProcessID = new System::Windows::Forms::RadioButton();
		this->rbExecutable = new System::Windows::Forms::RadioButton();
		this->lblInfo = new System::Windows::Forms::Label();
		this->pnlAttach = new System::Windows::Forms::Panel();
		this->rbNativeApp = new System::Windows::Forms::RadioButton();
		this->rbDotNetApp = new System::Windows::Forms::RadioButton();
		this->pnlvA = new System::Windows::Forms::Panel();
		this->pnlhA = new System::Windows::Forms::Panel();
		this->pnlhB = new System::Windows::Forms::Panel();
		this->pnlLaunch = new System::Windows::Forms::Panel();
		this->btnBrowseApp = new System::Windows::Forms::Button();
		this->tbAUT = new System::Windows::Forms::TextBox();
		this->lblCmdLine = new System::Windows::Forms::Label();
		this->tbCmdLine = new System::Windows::Forms::TextBox();
		this->chkProcessChain = new System::Windows::Forms::CheckBox();
		this->grpMain->SuspendLayout();
		this->pnlAttach->SuspendLayout();
		this->pnlLaunch->SuspendLayout();
		this->SuspendLayout();
		// 
		// pbWizardImg
		// 
		this->pbWizardImg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->pbWizardImg->Location = System::Drawing::Point(8, 8);
		this->pbWizardImg->Name = S"pbWizardImg";
		this->pbWizardImg->Size = System::Drawing::Size(112, 296);
		this->pbWizardImg->TabStop = false;
		// 
		// grpMain
		// 
		this->grpMain->Controls->Add(this->chkProcessChain);
		this->grpMain->Controls->Add(this->pnlLaunch);
		this->grpMain->Controls->Add(this->pnlhB);
		this->grpMain->Controls->Add(this->pnlhA);
		this->grpMain->Controls->Add(this->pnlvA);
		this->grpMain->Controls->Add(this->pnlAttach);
		this->grpMain->Controls->Add(this->rbProcessID);
		this->grpMain->Controls->Add(this->rbExecutable);
		this->grpMain->Controls->Add(this->lblInfo);
		this->grpMain->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->grpMain->Location = System::Drawing::Point(128, 6);
		this->grpMain->Name = S"grpMain";
		this->grpMain->Size = System::Drawing::Size(496, 296);
		this->grpMain->TabIndex = 1;
		this->grpMain->TabStop = false;
		// 
		// rbProcessID
		// 
		this->rbProcessID->Location = System::Drawing::Point(24, 176);
		this->rbProcessID->Name = S"rbProcessID";
		this->rbProcessID->Size = System::Drawing::Size(200, 16);
		this->rbProcessID->TabIndex = 6;
		this->rbProcessID->Text = S"Attach to a running instance of a:";
		// 
		// rbExecutable
		// 
		this->rbExecutable->Checked = true;
		this->rbExecutable->Location = System::Drawing::Point(24, 88);
		this->rbExecutable->Name = S"rbExecutable";
		this->rbExecutable->Size = System::Drawing::Size(392, 16);
		this->rbExecutable->TabIndex = 5;
		this->rbExecutable->TabStop = true;
		this->rbExecutable->Text = S"Launch the specified executable for testing";
		// 
		// lblInfo
		// 
		this->lblInfo->Location = System::Drawing::Point(8, 16);
		this->lblInfo->Name = S"lblInfo";
		this->lblInfo->Size = System::Drawing::Size(485, 72);
		this->lblInfo->Text = S"This wizard will assist you in creating a custom interception project. You may te" 
			S"st an application by having your test project either launch it or attach to an a" 
			S"lready running process. In the case of attaching to a running process, you will " 
			S"be required to specify the ProcessID in the code which will be generated for you" 
			S".";
		// 
		// pnlAttach
		// 
		this->pnlAttach->Controls->Add(this->rbDotNetApp);
		this->pnlAttach->Controls->Add(this->rbNativeApp);
		this->pnlAttach->Location = System::Drawing::Point(56, 200);
		this->pnlAttach->Name = S"pnlAttach";
		this->pnlAttach->Size = System::Drawing::Size(176, 56);
		this->pnlAttach->TabIndex = 10;
		// 
		// rbNativeApp
		// 
		this->rbNativeApp->Location = System::Drawing::Point(8, 8);
		this->rbNativeApp->Name = S"rbNativeApp";
		this->rbNativeApp->Size = System::Drawing::Size(160, 16);
		this->rbNativeApp->TabIndex = 0;
		this->rbNativeApp->Text = S"Native Win32 application";
		// 
		// rbDotNetApp
		// 
		this->rbDotNetApp->Location = System::Drawing::Point(8, 32);
		this->rbDotNetApp->Name = S"rbDotNetApp";
		this->rbDotNetApp->Size = System::Drawing::Size(120, 16);
		this->rbDotNetApp->TabIndex = 1;
		this->rbDotNetApp->Text = S".NET application";
		// 
		// pnlvA
		// 
		this->pnlvA->BackColor = System::Drawing::Color::Black;
		this->pnlvA->Location = System::Drawing::Point(48, 192);
		this->pnlvA->Name = S"pnlvA";
		this->pnlvA->Size = System::Drawing::Size(3, 48);
		this->pnlvA->TabIndex = 11;
		// 
		// pnlhA
		// 
		this->pnlhA->BackColor = System::Drawing::Color::Black;
		this->pnlhA->Location = System::Drawing::Point(48, 216);
		this->pnlhA->Name = S"pnlhA";
		this->pnlhA->Size = System::Drawing::Size(18, 2);
		this->pnlhA->TabIndex = 12;
		// 
		// pnlhB
		// 
		this->pnlhB->BackColor = System::Drawing::Color::Black;
		this->pnlhB->Location = System::Drawing::Point(48, 240);
		this->pnlhB->Name = S"pnlhB";
		this->pnlhB->Size = System::Drawing::Size(18, 2);
		this->pnlhB->TabIndex = 13;
		// 
		// pnlLaunch
		// 
		this->pnlLaunch->Controls->Add(this->tbCmdLine);
		this->pnlLaunch->Controls->Add(this->lblCmdLine);
		this->pnlLaunch->Controls->Add(this->btnBrowseApp);
		this->pnlLaunch->Controls->Add(this->tbAUT);
		this->pnlLaunch->Location = System::Drawing::Point(40, 104);
		this->pnlLaunch->Name = S"pnlLaunch";
		this->pnlLaunch->Size = System::Drawing::Size(432, 64);
		this->pnlLaunch->TabIndex = 14;
		// 
		// btnBrowseApp
		// 
		this->btnBrowseApp->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
		this->btnBrowseApp->Location = System::Drawing::Point(400, 0);
		this->btnBrowseApp->Name = S"btnBrowseApp";
		this->btnBrowseApp->Size = System::Drawing::Size(24, 20);
		this->btnBrowseApp->TabIndex = 1;
		this->btnBrowseApp->Text = S"...";
		// 
		// tbAUT
		// 
		this->tbAUT->Location = System::Drawing::Point(0, 0);
		this->tbAUT->Name = S"tbAUT";
		this->tbAUT->Size = System::Drawing::Size(400, 20);
		this->tbAUT->TabIndex = 0;
		this->tbAUT->Text = S"";
		// 
		// lblCmdLine
		// 
		this->lblCmdLine->Location = System::Drawing::Point(0, 24);
		this->lblCmdLine->Name = S"lblCmdLine";
		this->lblCmdLine->Size = System::Drawing::Size(320, 16);
		this->lblCmdLine->TabIndex = 2;
		this->lblCmdLine->Text = S"with the following command line argument string: (optional) ";
		// 
		// tbCmdLine
		// 
		this->tbCmdLine->Location = System::Drawing::Point(0, 40);
		this->tbCmdLine->Name = S"tbCmdLine";
		this->tbCmdLine->Size = System::Drawing::Size(400, 20);
		this->tbCmdLine->TabIndex = 13;
		this->tbCmdLine->Text = S"";
		// 
		// chkProcessChain
		// 
		this->chkProcessChain->Location = System::Drawing::Point(24, 264);
		this->chkProcessChain->Name = S"chkProcessChain";
		this->chkProcessChain->Size = System::Drawing::Size(152, 16);
		this->chkProcessChain->TabIndex = 3;
		this->chkProcessChain->Text = S"Enable process chaining";
		// 
		// CustomInterceptProject
		// 
		//this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ClientSize = System::Drawing::Size(638, 316);
		this->Controls->Add(this->grpMain);
		this->Controls->Add(this->pbWizardImg);
		this->Name = S"CustomInterceptProject";
		this->grpMain->ResumeLayout(false);
		this->pnlAttach->ResumeLayout(false);
		this->pnlLaunch->ResumeLayout(false);
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
	void CustomInterceptProject::Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}



	
	//*************************************************************************
	// Method:		btnBrowseApp_ClickEventHandler
	// Description: Handler method
	//*************************************************************************
	void CustomInterceptProject::btnBrowseApp_ClickEventHandler(Object * sender, EventArgs * e)
	{
		if (!this->browseDialog)
			this->browseDialog = new OpenFileDialog();

		this->browseDialog->Filter = "Applications (*.exe)|*.exe";
		this->browseDialog->CheckFileExists = true;
		this->browseDialog->CheckPathExists = true;
		this->browseDialog->Multiselect = false;
		this->browseDialog->Title = "Select the application to test";

		System::Windows::Forms::DialogResult browseResult = this->browseDialog->ShowDialog (this);
		
		if (browseResult == DialogResult::OK)
		{
			this->tbAUT->Text = this->browseDialog->FileName;
			this->rbExecutable->Checked = true;
		}
	}

	
	
	//*************************************************************************
	// Method:		radio_CheckedChangedHandler
	// Description: Handler method
	//*************************************************************************	
	void CustomInterceptProject::radio_CheckedChangedHandler(Object * sender, EventArgs * e)
	{
		RadioButton * rb = dynamic_cast <RadioButton *> (sender);
		if (!rb || !rb->Checked) return;

		if (rb == rbNativeApp || rb == rbDotNetApp)
			this->rbProcessID->Checked = true;
	}
}

