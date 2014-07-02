//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionWizard.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "FileCorruptionWizardControl.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the file corruption wizard dialog
	//*************************************************************************
	public __gc class FileCorruptionWizard : public Form
	{
	protected:
		FileCorruptionWizardControl *wizardControl;
		String *originalTitle;

		void onControlPageEnter(WizardPage *wp, WizardControl *wc);
		void onControlPageLeave(WizardPage *wp, WizardControl *wc);
		void onCancelClicked(Object *sender, EventArgs *args);
		void onFinishClicked(Object *sender, EventArgs *args);

	public:
		FileCorruptionWizard(FileCorruptionDataNode* dataNode, bool modifyNode);
		~FileCorruptionWizard();

		__property String *get_OriginalFile() { return wizardControl->OriginalFile; }
		__property String *get_SearchRegularExpression() { return wizardControl->SearchRegularExpression; }
		__property String *get_ReplacementString() { return wizardControl->ReplacementString; }
		__property String *get_CorruptType() { return wizardControl->CorruptType; }
		__property String *get_SearchFor() { return wizardControl->SearchFor; }
		__property String *get_ReplaceWith() { return wizardControl->ReplaceWith; }
		__property bool get_Regenerate() { return wizardControl->Regenerate; }
		__property FileCorruptionUISettings get_Settings() { return wizardControl->Settings; }
	};
}