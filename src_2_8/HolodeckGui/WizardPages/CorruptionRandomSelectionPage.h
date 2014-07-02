//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CorruptionRandomSelectionPage.h
//
// DESCRIPTION: Contains definition for the class CorruptionRandomSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace UIData
{
	public __value enum CorruptionRandomType
	{
		RandomCorruptSingleBytes,
		RandomCorruptLongStrings,
		RandomCorruptRepeatedSequence
	};

	public __value struct CorruptionRandomSettings
	{
		int corruptAmount;
		CorruptionRandomType type;
		int length;
		int corruptStringType;
		String* corruptString;
		bool overwrite;
	};
}

using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the random corruption page of the network corruption wizard
	//*************************************************************************
	public __gc class CorruptionRandomSelectionPage : public WizardPage
	{
	public:
		CorruptionRandomSelectionPage();
		~CorruptionRandomSelectionPage();

		__property String* get_SearchFor();
		__property String* get_SearchRegularExpression();
		__property String* get_ReplaceWith();
		__property String* get_ReplacementString();
		__property bool get_Overwrite();

		__property CorruptionRandomSettings get_Settings();
		__property void set_Settings(CorruptionRandomSettings settings);

		bool Validate();

	protected:
		TrackBar *corruptAmount;
		Label *corruptAmountLabel;
		RadioButton *insert;
		RadioButton *overwrite;
		RadioButton *singleBytes;
		RadioButton *longStrings;
		Label* longStringsUpToLabel;
		Label* longStringsCharacterLabel;
		TextBox *longStringMaxLen;
		RadioButton *repeatedSeq;
		Label* repeatedSeqUpToLabel;
		Label* repeatedSeqOfLabel;
		TextBox *seqMaxLen;
		TextBox *seqString;
		ComboBox *seqStringType;

		void onSingleBytesCheck(Object *sender, EventArgs *e);
		void onLongStringsCheck(Object *sender, EventArgs *e);
		void onRepeatedSeqCheck(Object *sender, EventArgs *e);
		void onCorruptAmountValueChanged(Object *sender, EventArgs *e);
	};
}