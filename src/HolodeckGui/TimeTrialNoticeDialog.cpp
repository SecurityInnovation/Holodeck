//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TimeTrialNoticeDialog.cpp
//
// DESCRIPTION: Contains implementation for the class TimeTrialNoticeDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 May 2004		 B. Shirey	 File created.
//*************************************************************************
#include "TimeTrialNoticeDialog.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TimeTrialNoticeDialog
	// Description: Constructor for the TimeTrialNoticeDialog class
	//
	// Parameters:
	//	daysLeft - the number of days left til expiration
	//
	// Return Value: None
	//*************************************************************************
	TimeTrialNoticeDialog::TimeTrialNoticeDialog(int daysLeft)
	{
		InitializeComponent();
		if (daysLeft > 0)
			infoLabel->Text = String::Format("You have {0} days left before this trial version of Holodeck expires.  You may continue to use it for free until then, or you can register your serial number and registration key now.", daysLeft.ToString());
		else
			infoLabel->Text = "This trial version of Holodeck has expired.  You may no longer use it without registering a serial number and registration key, which you can obtain by purchasing Holodeck.";
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Disposes the class
	//
	// Parameters:
	//	disposing - true if disposing, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void TimeTrialNoticeDialog::Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}

	//*************************************************************************
	// Method:		InitializeComponent
	// Description: Initializes the UI for the dialog
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TimeTrialNoticeDialog::InitializeComponent()
	{
		System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(HolodeckGui::TimeTrialNoticeDialog));
		this->logoPictureBox = new System::Windows::Forms::PictureBox();
		this->infoLabel = new System::Windows::Forms::Label();
		this->purchaseLinkLabel = new System::Windows::Forms::LinkLabel();
		this->label1 = new System::Windows::Forms::Label();
		this->groupBox1 = new System::Windows::Forms::GroupBox();
		this->continueButton = new System::Windows::Forms::Button();
		this->registerButton = new System::Windows::Forms::Button();
		this->label2 = new System::Windows::Forms::Label();
		this->groupBox1->SuspendLayout();
		this->SuspendLayout();
		// 
		// logoPictureBox
		// 
		this->logoPictureBox->Image = (__try_cast<System::Drawing::Image *  >(resources->GetObject(S"logoPictureBox.Image")));
		this->logoPictureBox->Location = System::Drawing::Point(0, 0);
		this->logoPictureBox->Name = S"logoPictureBox";
		this->logoPictureBox->Size = System::Drawing::Size(498, 96);
		this->logoPictureBox->TabIndex = 0;
		this->logoPictureBox->TabStop = false;
		// 
		// infoLabel
		// 
		this->infoLabel->Location = System::Drawing::Point(8, 104);
		this->infoLabel->Name = S"infoLabel";
		this->infoLabel->Size = System::Drawing::Size(472, 32);
		this->infoLabel->TabIndex = 1;
		this->infoLabel->TextAlign = System::Drawing::ContentAlignment::TopCenter;
		// 
		// purchaseLinkLabel
		// 
		this->purchaseLinkLabel->Location = System::Drawing::Point(99, 153);
		this->purchaseLinkLabel->Name = S"purchaseLinkLabel";
		this->purchaseLinkLabel->Size = System::Drawing::Size(300, 13);
		this->purchaseLinkLabel->TabIndex = 2;
		this->purchaseLinkLabel->TabStop = true;
		this->purchaseLinkLabel->Text = S"http://www.securityinnovation.com/holodeck/pricing.shtml";
		this->purchaseLinkLabel->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, purchaseLinkLabel_LinkClicked);
		// 
		// label1
		// 
		this->label1->Location = System::Drawing::Point(1, 140);
		this->label1->Name = S"label1";
		this->label1->Size = System::Drawing::Size(482, 12);
		this->label1->TextAlign = System::Drawing::ContentAlignment::TopCenter;
		this->label1->TabIndex = 3;
		this->label1->Text = S"You can purchase Holodeck from here:";
		// 
		// groupBox1
		// 
		this->groupBox1->Controls->Add(this->continueButton);
		this->groupBox1->Controls->Add(this->registerButton);
		this->groupBox1->Controls->Add(this->label2);
		this->groupBox1->Location = System::Drawing::Point(8, 168);
		this->groupBox1->Name = S"groupBox1";
		this->groupBox1->Size = System::Drawing::Size(472, 88);
		this->groupBox1->TabIndex = 4;
		this->groupBox1->TabStop = false;
		// 
		// continueButton
		// 
		this->continueButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->continueButton->Location = System::Drawing::Point(244, 48);
		this->continueButton->Name = S"continueButton";
		this->continueButton->Size = System::Drawing::Size(144, 23);
		this->continueButton->TabIndex = 2;
		this->continueButton->Text = S"No, I\'ll register later";
		this->continueButton->Click += new System::EventHandler(this, continueButton_Click);
		// 
		// registerButton
		// 
		this->registerButton->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->registerButton->Location = System::Drawing::Point(84, 48);
		this->registerButton->Name = S"registerButton";
		this->registerButton->Size = System::Drawing::Size(144, 23);
		this->registerButton->TabIndex = 1;
		this->registerButton->Text = S"Yes, I\'d like to register";
		this->registerButton->Click += new System::EventHandler(this, registerButton_Click);
		// 
		// label2
		// 
		this->label2->Location = System::Drawing::Point(52, 16);
		this->label2->Name = S"label2";
		this->label2->Size = System::Drawing::Size(400, 23);
		this->label2->TabIndex = 0;
		this->label2->Text = S"Would you like to register this copy of Holodeck Enterprise Edition 2.8 now\?";
		// 
		// TimeTrialNoticeDialog
		// 
		this->Icon = ImageListManager::getInstance()->AppIcon;
		this->AcceptButton = this->registerButton;
		this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->CancelButton = this->continueButton;
		this->ClientSize = System::Drawing::Size(490, 264);
		this->ControlBox = false;
		this->Controls->Add(this->groupBox1);
		this->Controls->Add(this->label1);
		this->Controls->Add(this->purchaseLinkLabel);
		this->Controls->Add(this->infoLabel);
		this->Controls->Add(this->logoPictureBox);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->Name = S"TimeTrialNoticeDialog";
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
		this->Text = S"Holodeck Time Trial";
		this->groupBox1->ResumeLayout(false);
		this->ResumeLayout(false);
	}

	//*************************************************************************
	// Method:		registerButton_Click
	// Description: called when the register button is clicked
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void TimeTrialNoticeDialog::registerButton_Click(System::Object *sender, System::EventArgs *e)
	{
	}

	//*************************************************************************
	// Method:		continueButton_Click
	// Description: called when the continue button is clicked
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void TimeTrialNoticeDialog::continueButton_Click(System::Object *sender, System::EventArgs *e)
	{
	}

	//*************************************************************************
	// Method:		purchaseLinkLabel_LinkClicked
	// Description: called when the purchase link label is clicked
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void TimeTrialNoticeDialog::purchaseLinkLabel_LinkClicked(System::Object *sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *e)
	{
		HTMLBrowser *browser = new HTMLBrowser();
		browser->Navigate(purchaseLinkLabel->Text);
	}

	//*************************************************************************
	// Method:		OnLoad
	// Description: called when the form loads
	//
	// Parameters:
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TimeTrialNoticeDialog::OnLoad(EventArgs *args)
	{
		this->continueButton->NotifyDefault(true);
		this->continueButton->Focus();
	}
}