//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CodeCoverageTestGenerationDlg.cpp
//
// DESCRIPTION: Contains implementation for the class CodeCoverageTestGenerationDlg
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "CodeCoverageTestGenerationDlg.h"
#include "ImageListManager.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		CodeCoverageTestGenerationDlg
	// Description: Constructor for the CodeCoverageTestGenerationDlg class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	CodeCoverageTestGenerationDlg::CodeCoverageTestGenerationDlg()
	{
		testGenerationDataArray = new TestGenerationDataArray(NULL, NULL);
		generationType = GenerationType::CodeCoverageTest;
		intensity = Intensity::Low;
		testGenerationDataArray->LoadSettings(generationType, intensity);
		
		generationDescription = "Attempt to hit as many error paths as possible. Rerun the application multiple times with different combinations of Limits, Faults, and Tests";

		InitializeUI();
		intensityTrackBar->ValueChanged += new EventHandler(this, onTrackBarValueChanged);
	}

	//*************************************************************************
	// Method:		~CodeCoverageTestGenerationDlg
	// Description: Destructor for the CodeCoverageTestGenerationDlg class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	CodeCoverageTestGenerationDlg::~CodeCoverageTestGenerationDlg()
	{
		delete testGenerationDataArray;
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes the UI
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CodeCoverageTestGenerationDlg::InitializeUI()
	{
		cancelButton = new System::Windows::Forms::Button();
		okButton = new System::Windows::Forms::Button();
		groupBox = new System::Windows::Forms::GroupBox();
		intensityTrackBar = new System::Windows::Forms::TrackBar();
		intensityValueLabel = new System::Windows::Forms::Label();
		intensityDescription = new System::Windows::Forms::Label();
		generationTypeDescription = new System::Windows::Forms::Label();
		timeoutLabel = new System::Windows::Forms::Label();
		timeoutTextBox = new System::Windows::Forms::TextBox();
		
		// 
		// cancelButton
		// 
		cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		cancelButton->Location = System::Drawing::Point(258, 358);
		cancelButton->Name = "cancelButton";
		cancelButton->TabIndex = 10;
		cancelButton->Text = "Cancel";
		// 
		// okButton
		// 
		okButton->Click += new EventHandler(this, onOKClicked);
		okButton->Location = System::Drawing::Point(170, 358);
		okButton->Name = "okButton";
		okButton->TabIndex = 9;
		okButton->Text = "OK";
		// 
		// groupBox
		// 
		groupBox->Controls->Add(intensityDescription);
		groupBox->Controls->Add(intensityValueLabel);
		groupBox->Controls->Add(intensityTrackBar);

		groupBox->Location = System::Drawing::Point(8, 104);
		groupBox->Name = "groupBox";
		groupBox->Size = System::Drawing::Size(330, 207);
		groupBox->TabIndex = 19;
		groupBox->TabStop = false;
		// 
		// intensityValueLabel
		// 
		intensityValueLabel->Font = new System::Drawing::Font("Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, 0);
		intensityValueLabel->Location = System::Drawing::Point(80, 20);
		intensityValueLabel->Name = "intensityValueLabel";
		intensityValueLabel->Text = "Low";
		intensityValueLabel->Size = System::Drawing::Size(72, 24);
		intensityValueLabel->TabIndex = 19;
		// 
		// intensityTrackBar
		// 
		intensityTrackBar->LargeChange = 1;
		intensityTrackBar->Location = System::Drawing::Point(16, 20);
		intensityTrackBar->Maximum = 2;
		intensityTrackBar->Minimum = 0;
		intensityTrackBar->Name = "intensityTrackBar";
		intensityTrackBar->Orientation = System::Windows::Forms::Orientation::Vertical;
		intensityTrackBar->Size = System::Drawing::Size(45, 171);
		intensityTrackBar->TabIndex = 17;
		intensityTrackBar->TickStyle = System::Windows::Forms::TickStyle::Both;
		intensityTrackBar->Value = 0;
		// 
		// intensityDescription
		// 
		intensityDescription->Location = System::Drawing::Point(80, 48);
		intensityDescription->Name = "intensityDescription";
		intensityDescription->Size = System::Drawing::Size(242, 143);
		intensityDescription->TabIndex = 20;
		intensityDescription->Text = GetIntensityDescription();
		// 
		// generationTypeDescription
		// 
		generationTypeDescription->Font = new System::Drawing::Font("Trebuchet MS", 9.75, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 0);
		generationTypeDescription->Location = System::Drawing::Point(36, 16);
		generationTypeDescription->Name = "generationTypeDescription";
		generationTypeDescription->Size = System::Drawing::Size(274, 80);
		generationTypeDescription->TabIndex = 20;
		generationTypeDescription->Text = generationDescription;
		//
		// timeoutLabel
		//
		timeoutLabel->Location = System::Drawing::Point(16, 326);
		timeoutLabel->Name = "timeoutLabel";
		timeoutLabel->Text = "Time in seconds to wait before restarting application:";
		timeoutLabel->Size = System::Drawing::Size(276, 22);
		timeoutLabel->TabIndex = 21;
		//
		// timeoutTextBox
		//
		timeoutTextBox->Location = System::Drawing::Point(292, 324);
		timeoutTextBox->Name = "timeoutTextBox";
		timeoutTextBox->Text = "300";
		timeoutTextBox->Size = System::Drawing::Size(40, 24);
		timeoutTextBox->TabIndex = 22;
		// 
		// Form
		// 
		AutoScaleBaseSize = System::Drawing::Size(5, 13);
		BackColor = System::Drawing::SystemColors::ControlLight;
		ClientSize = System::Drawing::Size(346, 392);
		FormBorderStyle = FormBorderStyle::FixedDialog;
		MaximizeBox = false;
		MinimizeBox = false;
		Controls->Add(generationTypeDescription);
		Controls->Add(groupBox);
		Controls->Add(timeoutLabel);
		Controls->Add(timeoutTextBox);
		Controls->Add(cancelButton);
		Controls->Add(okButton);

		Name = "Form";
		Text = "Code Coverage Test Generator";

		this->Icon = ImageListManager::getInstance()->AppIcon;
	}

	//*************************************************************************
	// Method:		onTrackBarValueChanged
	// Description: called when the trackbar value changes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CodeCoverageTestGenerationDlg::onTrackBarValueChanged(Object *sender, EventArgs *e)
	{
		if (intensityTrackBar->Value == 0)
		{
			intensityValueLabel->Text = "Low";
			intensity = Intensity::Low;
		}
		else if (intensityTrackBar->Value == 1)
		{
			intensityValueLabel->Text = "Medium";
			intensity = Intensity::Medium;
		}
		else if (intensityTrackBar->Value == 2)
		{
			intensityValueLabel->Text = "High";
			intensity = Intensity::High;
		}
		testGenerationDataArray->LoadSettings(generationType, intensity);
		intensityDescription->Text = GetIntensityDescription();
	}

	//*************************************************************************
	// Method:		GetIntensityDescription
	// Description: Build an intensity description string based on 
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	String *CodeCoverageTestGenerationDlg::GetIntensityDescription()
	{		
		StringBuilder *description = new StringBuilder("-");
		
		if (testGenerationDataArray->AllowDiskLimit)
			description->Append("Allow disk limits\n-");
		else
			description->Append("Disallow disk limits\n-");
		
		if (testGenerationDataArray->AllowMemoryLimit)
			description->Append("Allow memory limits\n-");
		else
			description->Append("Disallow memory limits\n-");

		if (testGenerationDataArray->AllowNetworkLimit)
			description->Append("Allow network limits\n-");
		else
			description->Append("Disallow network limits\n-");

		if (testGenerationDataArray->AllowDiskFaults)
			description->Append("Allow disk faults\n-");
		else
			description->Append("Disallow disk faults\n-");

		if (testGenerationDataArray->AllowMemoryFaults)
			description->Append("Allow memory faults\n-");
		else
			description->Append("Disallow memory faults\n-");

		if (testGenerationDataArray->AllowNetworkFaults)
			description->Append("Allow network faults\n-");
		else
			description->Append("Disallow network faults\n-");

		if (testGenerationDataArray->AllowRegistryFaults)
			description->Append("Allow registry faults\n-");
		else
			description->Append("Disallow registry faults\n-");

		if (testGenerationDataArray->AllowProcessFaults)
			description->Append("Allow process faults\n-");
		else
			description->Append("Disallow process faults\n-");

		if (testGenerationDataArray->AllowCustomFaults)
			description->Append("Allow custom faults\n-");
		else
			description->Append("Disallow custom faults\n-");

		if (testGenerationDataArray->AllowTests)
			description->Append("Allow API tests\n-");
		else
			description->Append("Disallow API tests\n-");

		if (testGenerationDataArray->AllowResourceTests)
			description->Append("Allow resource based tests and fuzzing");
		else
			description->Append("Disallow resource based tests and fuzzing");

		return description->ToString();
	}

	//*************************************************************************
	// Method:		onOKClicked
	// Description: called when the OK button is clicked
	//
	// Parameters:
	//	sender - Object that sent the event
	//  e - Event properties
	//
	// Return Value: None
	//*************************************************************************
	void CodeCoverageTestGenerationDlg::onOKClicked(Object *sender, EventArgs *e)
	{
		// Validate the time entered into the text box
		int value;
		try
		{
			value = Int32::Parse(timeoutTextBox->Text);
		}
		catch (Exception*)
		{
			// Parse failed
			UserNotification::ErrorNotify("Time to wait should be a positive integer");
			return;
		}

		if (value <= 0)
		{
			UserNotification::ErrorNotify("Time to wait should be a positive integer");
			return;
		}

		DialogResult = DialogResult::OK;
		Close();
	}
}
