//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RegistrationDialog.cpp
//
// DESCRIPTION: Contains implementation for the class RegistrationDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 Sep 2003		 B. Shirey	 File created.
//*************************************************************************
#include "RegistrationDialog.h"
#include "ImageListManager.h"
#include "UserNotification.h"

#include <vcclr.h>

using namespace HolodeckRegistration;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		RegistrationDialog
	// Description: Constructor for the RegistrationDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationDialog::RegistrationDialog()
	{
		isFromTimeTrial = false;
		InitializeUI();
		UpdateFields();
	}

	//*************************************************************************
	// Method:		RegistrationDialog
	// Description: Constructor for the RegistrationDialog class
	//
	// Parameters:
	//	isFromTimeTrialDialog - true if coming from the time trial dialog, 
	//		false otherwise
	//
	// Return Value: None
	//*************************************************************************
	RegistrationDialog::RegistrationDialog(bool isFromTimeTrialDialog)
	{
		isFromTimeTrial = isFromTimeTrialDialog;
		InitializeUI();
		UpdateFields();
	}

	//*************************************************************************
	// Method:		~RegistrationDialog
	// Description: Destructor for the RegistrationDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RegistrationDialog::~RegistrationDialog()
	{
		isFromTimeTrial = false;
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes controls for the dialog
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationDialog::InitializeUI()
	{
		informationLabel = new Label();
		informationLabel->Font = new System::Drawing::Font("Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, ((System::Byte)(0)));
		informationLabel->Location = System::Drawing::Point(8, 8);
		informationLabel->Size = System::Drawing::Size(384, 32);

		Label *serialNumberLabel = new Label();
		serialNumberLabel->Text = "Serial Number: ";
		serialNumberLabel->Location = System::Drawing::Point(8, 16);
		serialNumberLabel->Size = System::Drawing::Size(130, 16);

		serialNumberTextBox = new TextBox();
		serialNumberTextBox->Location = System::Drawing::Point(150, 16);
		serialNumberTextBox->Size = System::Drawing::Size(220, 20);
		serialNumberTextBox->Enabled = false;

		Label *registrationKeyLabel = new Label();
		registrationKeyLabel->Text = "Registration Key:";
		registrationKeyLabel->Location = System::Drawing::Point(8, 44);
		registrationKeyLabel->Size = System::Drawing::Size(130, 16);

		registrationKeyTextBox = new TextBox();
		registrationKeyTextBox->Location = System::Drawing::Point(150, 44);
		registrationKeyTextBox->Size = System::Drawing::Size(220, 20);
		registrationKeyTextBox->Enabled = false;

		editButton = new Button();
		editButton->Text = "Edit...";
		editButton->Location = System::Drawing::Point(80, 80);
		editButton->Size = System::Drawing::Size(104, 23);
		editButton->Click += new EventHandler(this, onEditButtonClick);
		
		registerButton = new Button();
		registerButton->Text = "Register";
		registerButton->Location = System::Drawing::Point(200, 80);
		registerButton->Size = System::Drawing::Size(104, 23);
		registerButton->Enabled = false;
		registerButton->Click += new EventHandler(this, onRegisterButtonClick);

		GroupBox *snKeyBox = new GroupBox();
		snKeyBox->Location = System::Drawing::Point(8, 50);
		snKeyBox->Size = System::Drawing::Size(384, 110);

		snKeyBox->Controls->Add(serialNumberLabel);
		snKeyBox->Controls->Add(serialNumberTextBox);
		snKeyBox->Controls->Add(registrationKeyLabel);
		snKeyBox->Controls->Add(registrationKeyTextBox);
		snKeyBox->Controls->Add(editButton);
		snKeyBox->Controls->Add(registerButton);

		Label *functionalityNameLabel = new Label();
		functionalityNameLabel->Text = "Functionality Level:";
		functionalityNameLabel->Location = System::Drawing::Point(8, 25);
		functionalityNameLabel->Size = System::Drawing::Size(130, 23);

		functionalityLevelLabel = new Label();
		functionalityLevelLabel->Text = "N/A";
		functionalityLevelLabel->Location = System::Drawing::Point(145, 25);
		functionalityLevelLabel->Size = System::Drawing::Size(225, 23);

		Label *daysValidNameLabel = new Label();
		daysValidNameLabel->Text = "# Days License is Valid:";
		daysValidNameLabel->Location = System::Drawing::Point(8, 49);
		daysValidNameLabel->Size = System::Drawing::Size(130, 23);

		daysValidLabel = new Label();
		daysValidLabel->Text = "N/A";
		daysValidLabel->Location = System::Drawing::Point(145, 49);
		daysValidLabel->Size = System::Drawing::Size(225, 23);

		Label *licensesNameLabel = new Label();
		licensesNameLabel->Text = "# Licenses Purchased:";
		licensesNameLabel->Location = System::Drawing::Point(8, 73);
		licensesNameLabel->Size = System::Drawing::Size(130, 23);

		licensesLabel = new Label();
		licensesLabel->Text = "N/A";
		licensesLabel->Location = System::Drawing::Point(145, 73);
		licensesLabel->Size = System::Drawing::Size(225, 23);

		GroupBox *detailsBox = new GroupBox();
		detailsBox->Text = "License Details";
		detailsBox->Location = System::Drawing::Point(8, 200);
		detailsBox->Size = System::Drawing::Size(384, 105);

		detailsBox->Controls->Add(functionalityNameLabel);
		detailsBox->Controls->Add(daysValidNameLabel);
		detailsBox->Controls->Add(licensesNameLabel);
		detailsBox->Controls->Add(functionalityLevelLabel);
		detailsBox->Controls->Add(daysValidLabel);
		detailsBox->Controls->Add(licensesLabel);

		doneButton = new Button();
		doneButton->Text = "Done";
		doneButton->Location = System::Drawing::Point(160, 315);
		doneButton->Size = System::Drawing::Size(75, 23);

		this->Controls->Add(informationLabel);
		this->Controls->Add(snKeyBox);
		this->Controls->Add(detailsBox);
		this->Controls->Add(doneButton);

		this->AcceptButton = doneButton;
		this->CancelButton = doneButton;
		this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ShowInTaskbar = true;
		this->ClientSize = System::Drawing::Size(400, 350);
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->Name = "RegistrationDialog";
		if (isFromTimeTrial)
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
		else
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
		this->Text = "Register Holodeck";
		this->Icon = ImageListManager::getInstance()->AppIcon;
	}

	//*************************************************************************
	// Method:		UpdateFields
	// Description: updates the fields that depend on successful registration
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationDialog::UpdateFields()
	{
		RegistrationMgr *mgr = RegistrationMgr::GetInstance();
		RegistrationData *data = mgr->GetRegistrationData();

		if ((data == NULL) || (data->GetSerialNumber().GetLength() == 0) || (data->GetRegistrationKey().GetLength() == 0))
		{
			informationLabel->Text = "This copy of Holodeck is NOT registered!\nClick the Edit button below to register.";
			serialNumberTextBox->Text = "";
			registrationKeyTextBox->Text = "";
			functionalityLevelLabel->Text = "Free Trial Version";
			daysValidLabel->Text = "N/A";
			licensesLabel->Text = "0";
		}
		else
		{
			informationLabel->Text = "This copy of Holodeck IS successfully registered!\nClick the Edit button below to change your registration information.";
			serialNumberTextBox->Text = (wchar_t *)data->GetSerialNumber();
			registrationKeyTextBox->Text = (wchar_t *)data->GetRegistrationKey();

			int daysValid = mgr->GetNumberOfDaysValid();
			if (daysValid == NO_TIME_EXPIRATION)
				daysValidLabel->Text = "No Expiration";
			else
			{
				// get the number of days left
				int daysLeft = mgr->GetNumberOfDaysRemaining();
				if (daysLeft == -1)
					daysValidLabel->Text = String::Format("{0} (expired 1 day ago)", __box(daysValid));
				else if (daysLeft < 0)
					daysValidLabel->Text = String::Format("{0} (expired {1} days ago)", __box(daysValid), __box(0 - daysLeft));
				else
					daysValidLabel->Text = String::Format("{0} ({1} remaining)", __box(daysValid), __box(daysLeft));
			}

			switch(data->GetFunctionalityType())
			{
			case FeatureDisabledTrial:
				functionalityLevelLabel->Text = "Free Trial Version";
				break;
			case TimeTrial:
				functionalityLevelLabel->Text = "Fully Functional Time-Trial Version";
				break;
			case FullVersion:
				functionalityLevelLabel->Text = "Full Version";
				break;
			}

			unsigned int licenses = data->GetNumberOfLicensesPurchased();
			if (licenses == SITE_LICENSE)
				licensesLabel->Text = "Site License";
			else
				licensesLabel->Text = String::Format("{0}", __box(licenses));
		}
	}

	//*************************************************************************
	// Method:		onEditButtonClick
	// Description: called when the user clicks the edit button
	//
	// Parameters:
	//	sender - the sender of the click event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationDialog::onEditButtonClick(Object *sender, EventArgs *args)
	{
		editButton->Enabled = false;
		registerButton->Enabled = true;
		serialNumberTextBox->Enabled = true;
		registrationKeyTextBox->Enabled = true;
		serialNumberTextBox->Focus();

		this->AcceptButton = registerButton;
		registerButton->NotifyDefault(true);
	}

	//*************************************************************************
	// Method:		onRegisterButtonClick
	// Description: called when the user clicks the register button
	//
	// Parameters:
	//	sender - the sender of the click event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void RegistrationDialog::onRegisterButtonClick(Object *sender, EventArgs *args)
	{
		if ((serialNumberTextBox->Text->Trim()->Length == 0) ||
			(registrationKeyTextBox->Text->Trim()->Length == 0))
			return;

		const __wchar_t __pin *snString = PtrToStringChars(serialNumberTextBox->Text->Trim());
		const __wchar_t __pin *regKey = PtrToStringChars(registrationKeyTextBox->Text->Trim());

		if (RegistrationMgr::GetInstance()->SaveRegistrationData(snString, regKey))
		{
			editButton->Enabled = true;
			registerButton->Enabled = false;
			serialNumberTextBox->Enabled = false;
			registrationKeyTextBox->Enabled = false;

			RegistrationMgr::GetInstance()->ReloadRegistrationData();

			UpdateFields();

			if (isFromTimeTrial)
				UserNotification::InfoNotify("Registration was successful!\nThe new settings will take effect immediately.");
			else
				UserNotification::InfoNotify("Registration was successful!\nThe new settings will take effect the next time you run Holodeck.");
		}
		else
			UserNotification::ErrorNotify("Registration was NOT successful!\nPlease try again.");

		this->AcceptButton = doneButton;
		doneButton->NotifyDefault(true);
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
	void RegistrationDialog::OnLoad(EventArgs *args)
	{
		if (isFromTimeTrial)
			onEditButtonClick(NULL, EventArgs::Empty);
	}
}