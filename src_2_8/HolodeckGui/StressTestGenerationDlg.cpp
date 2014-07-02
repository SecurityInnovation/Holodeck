//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StressTestGenerationDlg.cpp
//
// DESCRIPTION: Contains implementation for the class StressTestGenerationDlg
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "StressTestGenerationDlg.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		StressTestGenerationDlg
	// Description: Constructor for the StressTestGenerationDlg class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	StressTestGenerationDlg::StressTestGenerationDlg()
	{
		testGenerationDataArray = new TestGenerationDataArray(NULL, NULL);
		generationType = GenerationType::RandomStressTest;
		intensity = Intensity::Low;
		testGenerationDataArray->LoadSettings(generationType, intensity);
		
		randomGenerationDescription = "Randomly set faults, limits, and tests to flush out product stability bugs";
		intelligentGenerationDescription = "Create tests based on observation of API calls made by the application under test";

		InitializeUI();
		intensityTrackBar->ValueChanged += new EventHandler(this, onTrackBarValueChanged);
		randomRadio->CheckedChanged += new EventHandler(this, onRadioCheckedChanged);
		intelligentRadio->CheckedChanged += new EventHandler(this, onRadioCheckedChanged);
	}

	//*************************************************************************
	// Method:		~StressTestGenerationDlg
	// Description: Destructor for the StressTestGenerationDlg class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	StressTestGenerationDlg::~StressTestGenerationDlg()
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
	void StressTestGenerationDlg::InitializeUI()
	{
		cancelButton = new System::Windows::Forms::Button();
		okButton = new System::Windows::Forms::Button();
		intelligentRadio = new System::Windows::Forms::RadioButton();
		randomRadio = new System::Windows::Forms::RadioButton();
		groupBox = new System::Windows::Forms::GroupBox();
		intensityTrackBar = new System::Windows::Forms::TrackBar();
		intensityValueLabel = new System::Windows::Forms::Label();
		intensityDescription = new System::Windows::Forms::Label();
		generationTypeDescription = new System::Windows::Forms::Label();
		
		// 
		// cancelButton
		// 
		cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		cancelButton->Location = System::Drawing::Point(328, 362);
		cancelButton->Name = "cancelButton";
		cancelButton->TabIndex = 10;
		cancelButton->Text = "Cancel";
		// 
		// okButton
		// 
		okButton->DialogResult = System::Windows::Forms::DialogResult::OK;
		okButton->Location = System::Drawing::Point(240, 362);
		okButton->Name = "okButton";
		okButton->TabIndex = 9;
		okButton->Text = "OK";
		// 
		// randomRadio
		// 
		randomRadio->Location = System::Drawing::Point(32, 16);
		randomRadio->Name = "randomRadio";
		randomRadio->TabIndex = 11;
		randomRadio->Font = new System::Drawing::Font("Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 0);
		randomRadio->Text = "Random Tests";
		randomRadio->Checked = true;
		// 
		// intelligentRadio
		// 
		intelligentRadio->Location = System::Drawing::Point(280, 16);
		intelligentRadio->Name = "intelligentRadio";
		intelligentRadio->Size = System::Drawing::Size(112, 24);
		intelligentRadio->TabIndex = 12;
		intelligentRadio->Font = new System::Drawing::Font("Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 0);
		intelligentRadio->Text = "Intelligent Tests";
		// 
		// groupBox
		// 
		groupBox->Controls->Add(intensityDescription);
		groupBox->Controls->Add(intensityValueLabel);
		groupBox->Controls->Add(intensityTrackBar);

		groupBox->Location = System::Drawing::Point(8, 112);
		groupBox->Name = "groupBox";
		groupBox->Size = System::Drawing::Size(400, 234);
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
		intensityTrackBar->Size = System::Drawing::Size(45, 198);
		intensityTrackBar->TabIndex = 17;
		intensityTrackBar->TickStyle = System::Windows::Forms::TickStyle::Both;
		intensityTrackBar->Value = 0;
		// 
		// intensityDescription
		// 
		intensityDescription->Location = System::Drawing::Point(80, 48);
		intensityDescription->Name = "intensityDescription";
		intensityDescription->Size = System::Drawing::Size(312, 170);
		intensityDescription->TabIndex = 20;
		intensityDescription->Text = GetIntensityDescription();
		// 
		// generationTypeDescription
		// 
		generationTypeDescription->Font = new System::Drawing::Font("Trebuchet MS", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, 0);
		generationTypeDescription->Location = System::Drawing::Point(36, 48);
		generationTypeDescription->Name = "generationTypeDescription";
		generationTypeDescription->Size = System::Drawing::Size(344, 48);
		generationTypeDescription->TabIndex = 20;
		generationTypeDescription->Text = randomGenerationDescription;
		// 
		// Form
		// 
		AutoScaleBaseSize = System::Drawing::Size(5, 13);
		BackColor = System::Drawing::SystemColors::ControlLight;
		ClientSize = System::Drawing::Size(416, 400);
		FormBorderStyle = FormBorderStyle::FixedDialog;
		MaximizeBox = false;
		MinimizeBox = false;
		Controls->Add(generationTypeDescription);
		Controls->Add(groupBox);
		Controls->Add(cancelButton);
		Controls->Add(okButton);
		Controls->Add(randomRadio);
		Controls->Add(intelligentRadio);

		Name = "Form";
		Text = "Stress Test Generator";

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
	void StressTestGenerationDlg::onTrackBarValueChanged(Object *sender, EventArgs *e)
	{
		if (intensityTrackBar->Value == 0)
		{
			intensity = Intensity::Low;
			intensityValueLabel->Text = "Low";
		}
		else if (intensityTrackBar->Value == 1)
		{
			intensity = Intensity::Medium;
			intensityValueLabel->Text = "Medium";
		}
		else if (intensityTrackBar->Value == 2)
		{
			intensity = Intensity::High;
			intensityValueLabel->Text = "High";
		}
		testGenerationDataArray->LoadSettings(generationType, intensity);
		intensityDescription->Text = GetIntensityDescription();
	}

	//*************************************************************************
	// Method:		onRadioCheckedChanged
	// Description: called when the radio check changes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void StressTestGenerationDlg::onRadioCheckedChanged(Object *sender, EventArgs *e)
	{
		if (randomRadio->Checked)
		{
			generationType = GenerationType::RandomStressTest;
			generationTypeDescription->Text = randomGenerationDescription;
		}
		else
		{
			generationType = GenerationType::IntelligentStressTest;
			generationTypeDescription->Text = intelligentGenerationDescription;
		}

		onTrackBarValueChanged(sender, e);
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
	String *StressTestGenerationDlg::GetIntensityDescription()
	{		
		StringBuilder *description = new StringBuilder("-");

		description->Append(testGenerationDataArray->TestInterval/1000);
		description->Append(" second test interval\n-");
		description->Append(testGenerationDataArray->TestDuration/1000);
		description->Append(" second test duration\n-Set ");
		description->Append(testGenerationDataArray->NumOverlappingTests);
		description->Append(" test at a time\n-");
		if (generationType == GenerationType::RandomStressTest)
		{
			if (testGenerationDataArray->AllowDiskLimit)
			{
				description->Append("Allow disk limits up to ");
				description->Append(testGenerationDataArray->DiskLimitMax);
				description->Append(" bytes over recorded usage\n-");
			}
			else
				description->Append("Disallow disk limits\n-");
			
			if (testGenerationDataArray->AllowMemoryLimit)
			{
				description->Append("Allow memory limits up to ");
				description->Append(testGenerationDataArray->MemoryLimitMax);
				description->Append(" bytes over recorded usage\n-");
			}
			else
				description->Append("Disallow memory limits\n-");

			if (testGenerationDataArray->AllowNetworkLimit)
			{
				description->Append("Allow network limits from ");
				description->Append(testGenerationDataArray->NetworkLowRange/1000);
				description->Append(" kb/sec to ");
				description->Append(testGenerationDataArray->NetworkHighRange/1000);
				description->Append(" kb/sec\n-");
			}
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

			if (testGenerationDataArray->AllowNetworkCorruption)
				description->Append("Allow network fuzzing");
			else
				description->Append("Disallow network fuzzing");
		}
		else
		{
			if (testGenerationDataArray->AllowTests)
				description->Append("Allow API tests\n-");
			else
				description->Append("Disallow API tests\n-");

			if (testGenerationDataArray->AllowResourceTests)
				description->Append("Allow resource based tests and fuzzing");
			else
				description->Append("Disallow resource based tests and fuzzing");
		}

		return description->ToString();
	}
}
