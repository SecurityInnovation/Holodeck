//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StressTestGenerationDlg.h
//
// DESCRIPTION: Contains definition for the class StressTestGenerationDlg
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "defines.h"
#include "TestGenerationDataArray.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the StressTestGenerationDlg
	//*************************************************************************
	public __gc class StressTestGenerationDlg : public Form
	{
	public:
		__property UIData::Intensity get_Intensity() { return (UIData::Intensity)intensityTrackBar->Value; }
		__property UIData::GenerationType get_GenerationType()
		{
			if (randomRadio->Checked)
				return UIData::RandomStressTest;
			else 
				return UIData::IntelligentStressTest;
		}

		StressTestGenerationDlg();
		~StressTestGenerationDlg();
	private:
		TestGenerationDataArray *testGenerationDataArray;
		System::Windows::Forms::GroupBox *groupBox;
		System::Windows::Forms::Button *cancelButton;
		System::Windows::Forms::Button *okButton;
		System::Windows::Forms::RadioButton *intelligentRadio;
		System::Windows::Forms::RadioButton *randomRadio;
		System::Windows::Forms::TrackBar *intensityTrackBar;
		System::Windows::Forms::Label *intensityValueLabel;
		System::Windows::Forms::Label *intensityDescription;
		System::Windows::Forms::Label *generationTypeDescription;

		String *randomGenerationDescription, *intelligentGenerationDescription;

		UIData::GenerationType generationType;
		UIData::Intensity intensity;

		void InitializeUI();
		void onTrackBarValueChanged(Object *sender, EventArgs *e);
		void onRadioCheckedChanged(Object *sender, EventArgs *e);
		String *GetIntensityDescription();
	};
}