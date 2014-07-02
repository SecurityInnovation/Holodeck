//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionRandomSelectionPage.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionRandomSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 May 2003		 B. Shirey	 File created.
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

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the random corruption page of the file corruption wizard
	//*************************************************************************
	public __gc class FileCorruptionRandomSelectionPage : public WizardPage
	{
	public:
		FileCorruptionRandomSelectionPage();
		~FileCorruptionRandomSelectionPage();

		__property unsigned int get_NumBytesToMatch() { return corruptAmount->Value; }
		__property bool get_CorruptRandomSingleBytes() { return singleBytes->Checked; }
		__property bool get_CorruptRandomLongStrings() { return longStrings->Checked; }
		__property bool get_CorruptRepeatedSequence() { return repeatedSeq->Checked; }
		__property unsigned int get_LongStringLength() { return UInt32::Parse(longStringMaxLen->Text); }
		__property unsigned int get_SequenceLength() { return UInt32::Parse(seqMaxLen->Text); }
		__property String *get_SequenceString() { return seqString->Text; }
		__property String *get_SequenceStringType() { return seqStringType->SelectedText; }

	protected:
		TrackBar *corruptAmount;
		Label *corruptAmountLabel;
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