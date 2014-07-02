//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CIProjectOptions.h
//
// DESCRIPTION: Contains implementation  for the class CIProjectOptions
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 12 Feb 2004		 Ady K	   File Created
//*******************************************************************************

#include "CIProjectOptions.h"

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// Method:		CIProjectOptions
	// Description: Constructor for the CIProjectOptions class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	CIProjectOptions::CIProjectOptions(System::Drawing::Image * wizImage) : WizardPage()
	{
		this->SubTitle = "Specifying the output options";

		this->InitializeComponent();
		if (wizImage)
			this->pbWizardImg->Image = wizImage;

		//set event handlers
		this->tbProjectName->TextChanged += new EventHandler (this, tb_TextChangedHandler);
		this->btnBrowseLocation->Click += new EventHandler(this, btnBrowseLocation_ClickHandler);

		//Hide the language selection option, since right now we dont support multi-lang
		//support for diff. types of projects
		this->lblLanguage->Visible = false;
		this->pnlLanguage->Visible = false;
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
	void CIProjectOptions::InitializeComponent(void)
	{
		this->pbWizardImg = new System::Windows::Forms::PictureBox();
		this->grpMain = new System::Windows::Forms::GroupBox();
		this->lnlProjectName = new System::Windows::Forms::Label();
		this->tbProjectName = new System::Windows::Forms::TextBox();
		this->lblLanguage = new System::Windows::Forms::Label();
		this->pnlLanguage = new System::Windows::Forms::Panel();
		this->rbCSharp = new System::Windows::Forms::RadioButton();
		this->rbVB = new System::Windows::Forms::RadioButton();
		this->rbmCPP = new System::Windows::Forms::RadioButton();
		this->tbProjectLocation = new System::Windows::Forms::TextBox();
		this->lblLocation = new System::Windows::Forms::Label();
		this->btnBrowseLocation = new System::Windows::Forms::Button();
		this->pnlvA = new System::Windows::Forms::Panel();
		this->pnlhA = new System::Windows::Forms::Panel();
		this->pnlhB = new System::Windows::Forms::Panel();
		this->pnlhC = new System::Windows::Forms::Panel();
		this->grpMain->SuspendLayout();
		this->pnlLanguage->SuspendLayout();
		this->SuspendLayout();
		// 
		// pbWizardImg
		// 
		this->pbWizardImg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->pbWizardImg->Location = System::Drawing::Point(8, 8);
		this->pbWizardImg->Name = S"pbWizardImg";
		this->pbWizardImg->Size = System::Drawing::Size(112, 296);
		this->pbWizardImg->TabIndex = 2;
		this->pbWizardImg->TabStop = false;
		// 
		// grpMain
		// 
		this->grpMain->Controls->Add(this->btnBrowseLocation);
		this->grpMain->Controls->Add(this->tbProjectLocation);
		this->grpMain->Controls->Add(this->lblLocation);
		this->grpMain->Controls->Add(this->pnlLanguage);
		this->grpMain->Controls->Add(this->lblLanguage);
		this->grpMain->Controls->Add(this->tbProjectName);
		this->grpMain->Controls->Add(this->lnlProjectName);
		this->grpMain->Location = System::Drawing::Point(128, 6);
		this->grpMain->Name = S"grpMain";
		this->grpMain->Size = System::Drawing::Size(496, 296);
		this->grpMain->TabIndex = 3;
		this->grpMain->TabStop = false;
		// 
		// lnlProjectName
		// 
		this->lnlProjectName->Location = System::Drawing::Point(16, 24);
		this->lnlProjectName->Name = S"lnlProjectName";
		this->lnlProjectName->Size = System::Drawing::Size(280, 16);
		this->lnlProjectName->Text = S"Specify a name for your test project:";
		// 
		// tbProjectName
		// 
		this->tbProjectName->Location = System::Drawing::Point(24, 40);
		this->tbProjectName->Name = S"tbProjectName";
		this->tbProjectName->Size = System::Drawing::Size(352, 20);
		this->tbProjectName->TabIndex = 0;
		this->tbProjectName->Text = S"";
		// 
		// lblLanguage
		// 
		this->lblLanguage->Location = System::Drawing::Point(16, 168);
		this->lblLanguage->Name = S"lblLanguage";
		this->lblLanguage->Size = System::Drawing::Size(344, 16);
		this->lblLanguage->TabIndex = 2;
		this->lblLanguage->Text = S"Which language would you like your test project to be in:";
		// 
		// pnlLanguage
		// 
		this->pnlLanguage->Controls->Add(this->pnlhC);
		this->pnlLanguage->Controls->Add(this->pnlhB);
		this->pnlLanguage->Controls->Add(this->pnlhA);
		this->pnlLanguage->Controls->Add(this->rbmCPP);
		this->pnlLanguage->Controls->Add(this->rbVB);
		this->pnlLanguage->Controls->Add(this->rbCSharp);
		this->pnlLanguage->Controls->Add(this->pnlvA);
		this->pnlLanguage->Location = System::Drawing::Point(16, 182);
		this->pnlLanguage->Name = S"pnlLanguage";
		this->pnlLanguage->Size = System::Drawing::Size(392, 80);
		this->pnlLanguage->TabIndex = 3;
		// 
		// rbCSharp
		// 
		this->rbCSharp->Location = System::Drawing::Point(24, 8);
		this->rbCSharp->Name = S"rbCSharp";
		this->rbCSharp->Size = System::Drawing::Size(40, 16);
		this->rbCSharp->TabIndex = 0;
		this->rbCSharp->Text = S"C#";
		// 
		// rbVB
		// 
		this->rbVB->Location = System::Drawing::Point(24, 32);
		this->rbVB->Name = S"rbVB";
		this->rbVB->Size = System::Drawing::Size(64, 16);
		this->rbVB->TabIndex = 1;
		this->rbVB->Text = S"VB.NET";
		// 
		// rbmCPP
		// 
		this->rbmCPP->Location = System::Drawing::Point(24, 56);
		this->rbmCPP->Name = S"rbmCPP";
		this->rbmCPP->Size = System::Drawing::Size(96, 16);
		this->rbmCPP->TabIndex = 2;
		this->rbVB->Checked = true;
		this->rbmCPP->Text = S"Managed C++";
		// 
		// tbProjectLocation
		// 
		this->tbProjectLocation->Location = System::Drawing::Point(24, 112);
		this->tbProjectLocation->Name = S"tbProjectLocation";
		this->tbProjectLocation->Size = System::Drawing::Size(328, 20);
		this->tbProjectLocation->TabIndex = 1;
		this->tbProjectLocation->Text = S"";
		// 
		// lblLocation
		// 
		this->lblLocation->Location = System::Drawing::Point(16, 96);
		this->lblLocation->Name = S"lblLocation";
		this->lblLocation->Size = System::Drawing::Size(280, 16);
		this->lblLocation->TabIndex = 4;
		this->lblLocation->Text = S"Where would you like your project code to be placed:";
		// 
		// btnBrowseLocation
		// 
		this->btnBrowseLocation->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
		this->btnBrowseLocation->Location = System::Drawing::Point(352, 112);
		this->btnBrowseLocation->Name = S"btnBrowseLocation";
		this->btnBrowseLocation->Size = System::Drawing::Size(24, 20);
		this->btnBrowseLocation->TabIndex = 2;
		this->btnBrowseLocation->Text = S"...";
		// 
		// pnlvA
		// 
		this->pnlvA->BackColor = System::Drawing::Color::Black;
		this->pnlvA->Location = System::Drawing::Point(8, 0);
		this->pnlvA->Name = S"pnlvA";
		this->pnlvA->Size = System::Drawing::Size(3, 64);
		this->pnlvA->TabIndex = 13;
		// 
		// pnlhA
		// 
		this->pnlhA->BackColor = System::Drawing::Color::Black;
		this->pnlhA->Location = System::Drawing::Point(9, 14);
		this->pnlhA->Name = S"pnlhA";
		this->pnlhA->Size = System::Drawing::Size(15, 3);
		this->pnlhA->TabIndex = 15;
		// 
		// pnlhB
		// 
		this->pnlhB->BackColor = System::Drawing::Color::Black;
		this->pnlhB->Location = System::Drawing::Point(8, 38);
		this->pnlhB->Name = S"pnlhB";
		this->pnlhB->Size = System::Drawing::Size(15, 3);
		this->pnlhB->TabIndex = 16;
		// 
		// pnlhC
		// 
		this->pnlhC->BackColor = System::Drawing::Color::Black;
		this->pnlhC->Location = System::Drawing::Point(8, 62);
		this->pnlhC->Name = S"pnlhC";
		this->pnlhC->Size = System::Drawing::Size(15, 3);
		this->pnlhC->TabIndex = 17;
		// 
		// CIProjectOptions
		// 
		//this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ClientSize = System::Drawing::Size(638, 316);
		this->Controls->Add(this->grpMain);
		this->Controls->Add(this->pbWizardImg);
		this->Name = S"CIProjectOptions";
		this->Text = S"CIProjectOptions";
		this->grpMain->ResumeLayout(false);
		this->pnlLanguage->ResumeLayout(false);
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
	void CIProjectOptions::Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}

	//*************************************************************************
	// Method:		tb_TextChangedHandler
	// Description: Handler method
	//*************************************************************************	
	void CIProjectOptions::tb_TextChangedHandler(Object * sender, EventArgs *e)
	{
		TextBox * tb = dynamic_cast <TextBox *> (sender);

		if (!msgBalloon)
			msgBalloon = new GuiSupport::MessageBalloon();
		
		if (!this->IsProjectNameValid())
		{
			msgBalloon->Title = "Warning";
			msgBalloon->TitleIcon = TooltipIcon::Error;
			msgBalloon->Text = "Invalid characters used";
			msgBalloon->Parent = tb;
			msgBalloon->Align = BalloonAlignment::BottomRight;
			msgBalloon->CenterStem = false;
			msgBalloon->UseAbsolutePositioning = true;
			msgBalloon->Show();
		}
		else
			msgBalloon->Hide();
	}

	//*************************************************************************
	// Method:		PageLeft
	// Description: Called by wizardcontrol when leaving this page
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void CIProjectOptions::PageLeft()
	{
		//Hide the msg balloon if still visible
		if (msgBalloon)
			msgBalloon->Hide();
	}

	//*************************************************************************
	// Method:		btnBrowseLocation_ClickHandler
	// Description: Event handler for browse button click
	//*************************************************************************	
	void CIProjectOptions::btnBrowseLocation_ClickHandler(Object * sender, EventArgs * e)
	{
		if (!folderBrowser)
			folderBrowser = new FolderBrowserDialog();

		folderBrowser->ShowNewFolderButton = true;
		folderBrowser->ShowDialog(this);

		this->tbProjectLocation->Text = folderBrowser->SelectedPath;
	}

	//*************************************************************************
	// Method:		IsProjectNameValid
	// Description: Tells the caller if the project name is valid or not
	//*************************************************************************	
	bool CIProjectOptions::IsProjectNameValid()
	{
		bool isValid = true;
		for (int i=0; i < this->tbProjectName->Text->Length; i++)
		{
			Char c = this->tbProjectName->Text->Chars[i];
			if ((c < 'A'|| c > 'Z') && (c < 'a' || c > 'z'))
			{
				isValid = false;
				break;
			}
		}

		return isValid;
	}
}