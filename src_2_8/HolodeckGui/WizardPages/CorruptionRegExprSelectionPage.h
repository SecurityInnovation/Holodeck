//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CorruptionRegExprSelectionPage.h
//
// DESCRIPTION: Contains definition for the class CorruptionRegExprSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "../UIEventRegistry.h"
#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace UIData
{
	public __value struct CorruptionRegExprSettings
	{
		String* searchString;
		String* corruptString;
	};
}

using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the regular expression page of the network corruption wizard
	//*************************************************************************
	public __gc class CorruptionRegExprSelectionPage : public WizardPage
	{
	public:
		CorruptionRegExprSelectionPage();
		~CorruptionRegExprSelectionPage();

		__property String* get_SearchFor() { return searchExpr->Text; }
		__property String* get_SearchRegularExpression();
		__property String* get_ReplaceWith() { return replacementString->Text; }
		__property String* get_ReplacementString();

		__property CorruptionRegExprSettings get_Settings();
		__property void set_Settings(CorruptionRegExprSettings settings);

		bool Validate();

	protected:
		TextBox *searchExpr;
		TextBox *replacementString;
		LinkLabel* helpLinkLabel;

		void onHelpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);
	};
}