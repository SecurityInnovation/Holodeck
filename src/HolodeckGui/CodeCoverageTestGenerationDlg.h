//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CodeCoverageTestGenerationDlg.h
//
// DESCRIPTION: Contains definition for the class CodeCoverageTestGenerationDlg
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
	// class that represents the CodeCoverageTestGenerationDlg
	//*************************************************************************
	public __gc class CodeCoverageTestGenerationDlg : public Form
	{
	public:
		__property UIData::Intensity get_Intensity() { return (UIData::Intensity)intensityTrackBar->Value; }
		__property int get_Timeout() { return Int32::Parse(timeoutTextBox->Text); }

		CodeCoverageTestGenerationDlg();
		~CodeCoverageTestGenerationDlg();
	private:
		TestGenerationDataArray *testGenerationDataArray;
		System::Windows::Forms::GroupBox *groupBox;
		System::Windows::Forms::Button *cancelButton;
		System::Windows::Forms::Button *okButton;
		System::Windows::Forms::Label *intensityLabel;
		System::Windows::Forms::TrackBar *intensityTrackBar;
		System::Windows::Forms::Label *intensityValueLabel;
		System::Windows::Forms::Label *intensityDescription;
		System::Windows::Forms::Label *generationTypeDescription;
		System::Windows::Forms::Label *timeoutLabel;
		System::Windows::Forms::TextBox *timeoutTextBox;

		String *generationDescription;

		UIData::GenerationType generationType;
		UIData::Intensity intensity;

		void InitializeUI();
		void onTrackBarValueChanged(Object *sender, EventArgs *e);
		String *GetIntensityDescription();

		void onOKClicked(Object *sender, EventArgs *e);
	};
}