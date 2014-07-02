//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionSearchSelectionPage.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionSearchSelectionPage
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
	// class that represents the find and replace page of the file corruption wizard
	//*************************************************************************
	public __gc class FileCorruptionSearchSelectionPage : public WizardPage
	{
	public:
		FileCorruptionSearchSelectionPage();
		~FileCorruptionSearchSelectionPage();

		__property String *get_FindString() { return findString->Text; }
		__property String *get_FindStringType() { return findStringType->Text; }
		__property bool get_ReplaceWithAnother() { return replaceWithAnother->Checked; }
		__property bool get_ReplaceWithLongString() { return longStrings->Checked; }
		__property bool get_ReplaceWithSequence() { return repeatedSeq->Checked; }
		__property String *get_ReplaceWithAnotherString() { return replaceString->Text; }
		__property String *get_ReplaceWithAnotherStringType() { return replaceStringType->SelectedText; }
		__property unsigned int get_LongStringSize() { return UInt32::Parse(longStringMaxLen->Text); }
		__property unsigned int get_RepeatedSequenceCount() { return UInt32::Parse(seqMaxLen->Text); }
		__property String *get_SequenceString() { return seqString->Text; }
		__property String *get_SequenceStringType() { return seqStringType->SelectedText; }

	protected:
		TextBox *findString;
		ComboBox *findStringType;
		RadioButton *replaceWithAnother;
		TextBox *replaceString;
		ComboBox *replaceStringType;
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

		void onReplaceWithAnotherCheck(Object *sender, EventArgs *e);
		void onLongStringsCheck(Object *sender, EventArgs *e);
		void onRepeatedSeqCheck(Object *sender, EventArgs *e);
	};
}