//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionSearchSelectionPage.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionSearchSelectionPage
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
	public __value enum NetworkCorruptionSearchType
	{
		SearchCorruptReplace,
		SearchCorruptLongStrings,
		SearchCorruptRepeatedSequence
	};

	public __value struct NetworkCorruptionSearchSettings
	{
		int searchStringType;
		String* searchString;
		NetworkCorruptionSearchType type;
		int length;
		int corruptStringType;
		String* corruptString;
	};
}

using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the find and replace page of the network corruption wizard
	//*************************************************************************
	public __gc class NetworkCorruptionSearchSelectionPage : public WizardPage
	{
	public:
		NetworkCorruptionSearchSelectionPage();
		~NetworkCorruptionSearchSelectionPage();

		__property String* get_SearchFor();
		__property String* get_SearchRegularExpression();
		__property String* get_ReplaceWith();
		__property String* get_ReplacementString();

		__property NetworkCorruptionSearchSettings get_Settings();
		__property void set_Settings(NetworkCorruptionSearchSettings settings);

		bool Validate();

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