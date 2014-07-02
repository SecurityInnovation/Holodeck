//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionRegExprSelectionPage.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionRegExprSelectionPage
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
	// class that represents the regular expression page of the file corruption wizard
	//*************************************************************************
	public __gc class FileCorruptionRegExprSelectionPage : public WizardPage
	{
	public:
		FileCorruptionRegExprSelectionPage();
		~FileCorruptionRegExprSelectionPage();

		__property String *get_SearchExpression() { return searchExpr->Text; }
		__property String *get_ReplacementExpression() { return replacementString->Text; }

	protected:
		TextBox *searchExpr;
		TextBox *replacementString;
		Button *helpButton;
	};
}