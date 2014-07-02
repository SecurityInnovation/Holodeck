//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestGenStatusBox.cpp
//
// DESCRIPTION: Contains implementation for the class TestGenStatusBox
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Mar 2004		 J. Taylor	 File created.
//*************************************************************************
#include "TestGenStatusBox.h"
#include "defines.h"
#include "ImageListManager.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestGenStatusBox
	// Description: Constructor for the TestGenStatusBox class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestGenStatusBox::TestGenStatusBox()
	{
		dataEventRegistry = DataEventRegistry::getInstance();
		InitializeUI();
	}

	//*************************************************************************
	// Method:		~TestGenStatusBox
	// Description: Destructor for the TestGenStatusBox class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestGenStatusBox::~TestGenStatusBox()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes all UI components for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TestGenStatusBox::InitializeUI()
	{
			this->testsLeftLabel = new System::Windows::Forms::Label();
			this->testsRunLabel = new System::Windows::Forms::Label();
			this->label3 = new System::Windows::Forms::Label();
			this->label2 = new System::Windows::Forms::Label();
			this->generationProgressBar = new System::Windows::Forms::ProgressBar();
			this->generationTypeLabel = new System::Windows::Forms::Label();
			this->label1 = new System::Windows::Forms::Label();
			this->stopGenerationButton = new System::Windows::Forms::Button();
			this->percentComplete = new System::Windows::Forms::Label();
			this->descriptionLabel = new System::Windows::Forms::Label();
			this->label5 = new System::Windows::Forms::Label();
			this->SuspendLayout();
			// 
			// testsLeftLabel
			// 
			this->testsLeftLabel->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->testsLeftLabel->ForeColor = System::Drawing::SystemColors::Highlight;
			this->testsLeftLabel->Location = System::Drawing::Point(296, 168);
			this->testsLeftLabel->Name = S"testsLeftLabel";
			this->testsLeftLabel->Size = System::Drawing::Size(80, 23);
			this->testsLeftLabel->TabIndex = 15;
			// 
			// testsRunLabel
			// 
			this->testsRunLabel->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->testsRunLabel->ForeColor = System::Drawing::SystemColors::Highlight;
			this->testsRunLabel->Location = System::Drawing::Point(112, 168);
			this->testsRunLabel->Name = S"testsRunLabel";
			this->testsRunLabel->Size = System::Drawing::Size(64, 23);
			this->testsRunLabel->TabIndex = 14;
			// 
			// label3
			// 
			this->label3->Location = System::Drawing::Point(208, 168);
			this->label3->Name = S"label3";
			this->label3->Size = System::Drawing::Size(88, 23);
			this->label3->TabIndex = 13;
			this->label3->Text = S"Tests left to run:";
			// 
			// label2
			// 
			this->label2->Location = System::Drawing::Point(48, 168);
			this->label2->Name = S"label2";
			this->label2->Size = System::Drawing::Size(64, 23);
			this->label2->TabIndex = 12;
			this->label2->Text = S"Tests run:";
			// 
			// generationProgressBar
			// 
			this->generationProgressBar->Location = System::Drawing::Point(36, 128);
			this->generationProgressBar->Name = S"generationProgressBar";
			this->generationProgressBar->Size = System::Drawing::Size(276, 24);
			this->generationProgressBar->TabIndex = 11;
			// 
			// generationTypeLabel
			// 
			this->generationTypeLabel->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->generationTypeLabel->ForeColor = System::Drawing::SystemColors::Highlight;
			this->generationTypeLabel->Location = System::Drawing::Point(136, 8);
			this->generationTypeLabel->Name = S"generationTypeLabel";
			this->generationTypeLabel->Size = System::Drawing::Size(224, 40);
			this->generationTypeLabel->TabIndex = 10;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(40, 8);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(88, 24);
			this->label1->TabIndex = 9;
			this->label1->Text = S"You are running:";
			// 
			// stopGenerationButton
			// 
			this->stopGenerationButton->Location = System::Drawing::Point(172, 216);
			this->stopGenerationButton->Name = S"stopGenerationButton";
			this->stopGenerationButton->Size = System::Drawing::Size(80, 40);
			this->stopGenerationButton->TabIndex = 8;
			this->stopGenerationButton->Text = S"Stop Test Generation";
			this->stopGenerationButton->Click += new EventHandler(this, StopButtonClicked);
			// 
			// percentComplete
			// 
			this->percentComplete->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 9, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->percentComplete->ForeColor = System::Drawing::SystemColors::Highlight;
			this->percentComplete->Location = System::Drawing::Point(320, 128);
			this->percentComplete->Name = S"percentComplete";
			this->percentComplete->Size = System::Drawing::Size(96, 23);
			this->percentComplete->TabIndex = 16;
			// 
			// descriptionLabel
			// 
			this->descriptionLabel->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->descriptionLabel->ForeColor = System::Drawing::SystemColors::Highlight;
			this->descriptionLabel->Location = System::Drawing::Point(176, 56);
			this->descriptionLabel->Name = S"descriptionLabel";
			this->descriptionLabel->Size = System::Drawing::Size(184, 64);
			this->descriptionLabel->TabIndex = 18;
			// 
			// label5
			// 
			this->label5->Location = System::Drawing::Point(40, 56);
			this->label5->Name = S"label5";
			this->label5->Size = System::Drawing::Size(128, 48);
			this->label5->TabIndex = 17;
			this->label5->Text = S"Test Generation is now:";
			// 
			// Form1
			// 
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->FormBorderStyle = FormBorderStyle::FixedDialog;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(424, 262);
			this->Controls->Add(this->descriptionLabel);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->percentComplete);
			this->Controls->Add(this->testsLeftLabel);
			this->Controls->Add(this->testsRunLabel);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->generationProgressBar);
			this->Controls->Add(this->generationTypeLabel);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->stopGenerationButton);
			this->Name = S"Test Generation Status";
			this->Text = S"Test Generation Status";
			this->Icon = ImageListManager::getInstance()->AppIcon;
			this->ResumeLayout(false);
	}

	//*************************************************************************
	// Method:		Show
	// Description: Show this form as a modeless dlg
	//
	// Parameters:
	//	testType - what type of test is being run
	//
	// Return Value: None
	//*************************************************************************
	void TestGenStatusBox::Show(GenerationType generationType, Intensity intensity)
	{
		this->generationType = generationType;
		this->intensity = intensity;

		switch (intensity)
		{
		case High:
			generationTypeLabel->Text = "High intensity ";
			break;
		case Medium:
			generationTypeLabel->Text = "Medium intensity ";
			break;
		case Low:
			generationTypeLabel->Text = "Low intensity ";
			break;
		}

		switch (generationType)
		{
		case RandomStressTest:
			generationTypeLabel->Text = String::Concat(generationTypeLabel->Text, "random stress test generation");
			percentComplete->Enabled = false;
			generationProgressBar->Enabled = false;
			break;
		case IntelligentStressTest:
			generationTypeLabel->Text = String::Concat(generationTypeLabel->Text, "intelligent stress test generation");
			percentComplete->Enabled = false;
			generationProgressBar->Enabled = false;
			break;
		case CodeCoverageTest:
			generationTypeLabel->Text = String::Concat(generationTypeLabel->Text, "code coverage test generation");
			percentComplete->Text = "0% Complete";
			break;
		}

		Form::Show();
	}

	//*************************************************************************
	// Method:		UpdateProgressBar
	// Description: Update the progress bar
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void TestGenStatusBox::UpdateProgressBar()
	{
		if (testsLeft == 0 || testsLeft == -1)
		{
			generationProgressBar->Value = 0;
			return;
		}

		Decimal numerator = testsRun;
		Decimal denominator = testsRun + testsLeft;
		Decimal Result = Decimal::Divide(numerator, denominator);
		Result = Decimal::Round(Result * 100, 0);
		generationProgressBar->Value = Decimal::ToInt16(Result);
		percentComplete->Text = String::Concat(generationProgressBar->Value.ToString(), "% Complete");
	}

	//*************************************************************************
	// Method:		StopButtonClicked
	// Description: called when the stop button is clicked
	//
	// Parameters:
	//	sender - object who made the call
	//	e - event args
	//
	// Return Value: None
	//*************************************************************************
	void TestGenStatusBox::StopButtonClicked(Object *sender, EventArgs *e)
	{
		if ( (generationType == CodeCoverageTest) && dataEventRegistry->OnStopCodeCoverageTestGeneration)
			dataEventRegistry->OnStopCodeCoverageTestGeneration->Invoke();
		else if (dataEventRegistry->OnStopStressTestGeneration)
			dataEventRegistry->OnStopStressTestGeneration->Invoke();
	}
}