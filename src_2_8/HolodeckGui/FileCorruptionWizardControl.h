//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionWizardControl.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "WizardPages/FileCorruptionTypeSelectionPage.h"
#include "WizardPages/CorruptionRandomSelectionPage.h"
#include "WizardPages/CorruptionSearchSelectionPage.h"
#include "WizardPages/CorruptionRegExprSelectionPage.h"
#include "WizardPages/FileCorruptionSummaryPage.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace UIData;


namespace UIData
{
	public __value struct FileCorruptionUISettings
	{
		String* type;
		bool regen;
		CorruptionRandomSettings randomSettings;
		CorruptionSearchSettings searchSettings;
		CorruptionRegExprSettings regExprSettings;
	};

	public __gc class FileCorruptionDataNode;
}


namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the file corruption wizard controller class
	//*************************************************************************
	public __gc class FileCorruptionWizardControl : public WizardControl
	{
	protected:
		FileCorruptionTypeSelectionPage *typeSelectionPage;
		CorruptionRandomSelectionPage *randomSelectionPage;
		CorruptionSearchSelectionPage *searchSelectionPage;
		CorruptionRegExprSelectionPage *regExprSelectionPage;
		FileCorruptionSummaryPage *summaryPage;

		CorruptionRandomSettings randomSettings;
		CorruptionSearchSettings searchSettings;
		CorruptionRegExprSettings regExprSettings;

		String *origFile;
		String *searchRegExpr;
		String *replacementStr;
		String *corruptType;
		String *searchFor;
		String *replaceWith;
		bool regen;

		ArrayList* createdDataNodes;

		bool inSummaryMode;
		bool skipPageLeaveActions;

		void onPageEnter(WizardPage *wp, WizardControl *wc);
		void onPageLeave(WizardPage *wp, WizardControl *wc);

		void onModifyOriginalFile();
		void onModifyType();
		void onModifySearchAndReplace();

	public:
		FileCorruptionWizardControl(FileCorruptionDataNode* dataNode, bool modifyNode);
		~FileCorruptionWizardControl();

		__property String *get_OriginalFile() { return origFile; }
		__property String *get_SearchRegularExpression() { return searchRegExpr; }
		__property String *get_ReplacementString() { return replacementStr; }
		__property String *get_CorruptType() { return corruptType; }
		__property String *get_SearchFor() { return searchFor; }
		__property String *get_ReplaceWith() { return replaceWith; }
		__property bool get_Regenerate() { return regen; }
		__property bool get_InSummaryMode() { return inSummaryMode; }
		__property void set_SkipPageLeaveActions(bool value) { this->skipPageLeaveActions = value;}

		__property FileCorruptionUISettings get_Settings();
		__property void set_Settings(FileCorruptionUISettings settings);

		void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
		void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }

		void OnNextClick(CancelEventArgs *args);
	};
}