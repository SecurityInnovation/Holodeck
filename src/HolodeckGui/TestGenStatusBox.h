//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestGenStatusBox.h
//
// DESCRIPTION: Contains definition for the class TestGenStatusBox
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Mar 2004		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "UIEventRegistry.h"
#include "DataEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace UIData
{
	public __value enum GenerationType
	{
		RandomStressTest,
		IntelligentStressTest,
		CodeCoverageTest
	};

	public __value enum Intensity
	{
		Low,
		Medium,
		High
	};
}

namespace HolodeckGui
{
	//*********************************************************************
	// defines the test generation status box
	//*********************************************************************
	public __gc class TestGenStatusBox : public System::Windows::Forms::Form
	{
	public:

		TestGenStatusBox();
		~TestGenStatusBox();
		void Show(GenerationType generationType, Intensity intensity);

		__property unsigned int get_TestsRun() { return testsRun; }
		__property void set_TestsRun(unsigned int value) 
		{ 
			testsRun = value;
			testsRunLabel->Text = value.ToString(); 
			UpdateProgressBar();
		}
		__property unsigned int get_TestsLeft() { return testsLeft; }
		__property void set_TestsLeft(unsigned int value) 
		{ 
			testsLeft = value;
			if (value == -1)
				testsLeftLabel->Text = "Unknown";
			else
                testsLeftLabel->Text = value.ToString(); 
			UpdateProgressBar();
		}

		__property void set_DescriptionText(String *value) { descriptionLabel->Text = value; }

	private: // UI COMPONENT MEMBERS //
		Label *testsLeftLabel;
		Label *testsRunLabel;
		Label *label3;
		Label *label2;
		ProgressBar *generationProgressBar;
		Label *generationTypeLabel;
		Label *label1;
		Button *stopGenerationButton;
		Label *percentComplete;
		Label *descriptionLabel;
		Label *label5;
		
		unsigned int testsRun;
		unsigned int testsLeft;
		GenerationType generationType;
		Intensity intensity;
		DataEventRegistry *dataEventRegistry;

		void InitializeUI();
		void UpdateProgressBar();
		void StopButtonClicked(Object *sender, EventArgs *e);
	};
}