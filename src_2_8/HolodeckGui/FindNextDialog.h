//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FindNextDialog.h
//
// DESCRIPTION: Contains definition for the class FindNextDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	---------------------------------------
// 05 May 2003		 A. Kakrania	File created.
//*************************************************************************
#pragma once

#include "LogPane.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <HoloScript.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;

using namespace HoloScript;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the find dialog and search capabilities
	//*************************************************************************
	public __gc class FindNextDialog : public System::Windows::Forms::Form
	{
	private:
		void InitializeUI();

		Label * lblLookFor;
		TextBox * txtFindString;
		GroupBox * grpFindInCategories;
		CheckBox * chkFieldThreadID;
		CheckBox * chkFieldDllName;
		CheckBox * chkFieldFuncName;
		CheckBox * chkFieldReturnValue;
		CheckBox * chkFieldErrorCode;
		CheckBox * chkFieldException;
		CheckBox * chkFieldParameters;
		CheckBox * chkMatchWhole;
		CheckBox * chkCaseSensitive;
		CheckBox * chkFieldCategory;
		Button * btnFindnext;
		Button * btnCancel;
		Panel * pnlSearchDirection;
		RadioButton * rbSrchUp;
		RadioButton * rbSrchDn;
		Label * lblSrchDirection;

		bool entryFound;
		System::Threading::Thread *SearchThread;
		LogPane *logPane;

		void btnCancelEventHandlerFunc (Object * sender, EventArgs * e);
		void btnFindEventHandlerFunc (Object * sender, EventArgs * e);
		bool MatchString (String * str1, String * str2, bool matchWhole, bool caseSensitive);
		void SearchThreadFunc();
		void SelectFoundEntry (TreeListNode *node);

	public:
		FindNextDialog();
		~FindNextDialog();

		__property void set_CurrentLogPane(LogPane *value);
		__property LogPane *get_CurrentLogPane() { return logPane; }

		void SearchNext();
		void Show();
		void Hide();
		void Dispose(bool disposing);
	};
}