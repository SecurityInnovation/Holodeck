//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CIProjectSummary.h
//
// DESCRIPTION: Contains implementation for the class CIProjectSummary
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 12 Feb 2004		 Ady K	   File Created
//*******************************************************************************

#include "CIProjectSummary.h"

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// Method:		CIProjectSummary
	// Description: Constructor for the CIProjectSummary class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	CIProjectSummary::CIProjectSummary(System::Drawing::Image * wizImage) : WizardPage() 
	{
		this->SubTitle = "A summary of what was done";

		this->InitializeComponent();
		
		if (wizImage)
			this->pbWizardImg->Image = wizImage;

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
	void CIProjectSummary::InitializeComponent(void)
	{
		this->progressBar = new System::Windows::Forms::ProgressBar();
		this->pbWizardImg = new System::Windows::Forms::PictureBox();
		this->grpMain = new System::Windows::Forms::GroupBox();
		this->lblInfo = new System::Windows::Forms::TextBox();
		this->grpMain->SuspendLayout();
		this->SuspendLayout();
		// 
		// pbWizardImg
		// 
		this->pbWizardImg->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->pbWizardImg->Location = System::Drawing::Point(8, 8);
		this->pbWizardImg->Name = S"pbWizardImg";
		this->pbWizardImg->Size = System::Drawing::Size(112, 296);
		this->pbWizardImg->TabIndex = 3;
		this->pbWizardImg->TabStop = false;
		// 
		// grpMain
		// 
		this->grpMain->Controls->Add(this->progressBar);
		this->grpMain->Controls->Add(this->lblInfo);
		this->grpMain->Location = System::Drawing::Point(128, 6);
		this->grpMain->Name = S"grpMain";
		this->grpMain->Size = System::Drawing::Size(496, 296);
		this->grpMain->TabIndex = 4;
		this->grpMain->TabStop = false;
		// 
		// lblInfo
		// 
		this->lblInfo->BorderStyle = System::Windows::Forms::BorderStyle::None;
		this->lblInfo->Font = new System::Drawing::Font(S"Tahoma", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
		this->lblInfo->Location = System::Drawing::Point(8, 70);
		this->lblInfo->Multiline = true;
		this->lblInfo->Name = S"lblInfo";
		this->lblInfo->ReadOnly = true;
		this->lblInfo->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
		this->lblInfo->Size = System::Drawing::Size(480, 218);
		this->lblInfo->TabIndex = 0;
		this->lblInfo->Text = S"";
		//
		// progressBar
		//
		this->progressBar->Location = System::Drawing::Point(98,30);
		this->progressBar->Name = S"progressBar";
		this->progressBar->Size = System::Drawing::Size (300, 20);
		this->progressBar->Maximum = 4;
		this->progressBar->Minimum = 0;
		this->progressBar->Value = 0;
		// 
		// CIProjectSummary
		// 
		//this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ClientSize = System::Drawing::Size(638, 316);
		this->Controls->Add(this->grpMain);
		this->Controls->Add(this->pbWizardImg);
		this->Name = S"CIProjectSummary";
		this->Text = S"CIProjectSummary";
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
	void CIProjectSummary::Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}
}
