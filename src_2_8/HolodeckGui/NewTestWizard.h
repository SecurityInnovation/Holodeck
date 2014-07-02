//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewTestWizard.h
//
// DESCRIPTION: Contains definition for the class NewTestWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "NewTestWizardControl.h"
#include "ScheduledTestDataNode.h"

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
	// class that represents the new test wizard dialog
	//*************************************************************************
	public __gc class NewTestWizard : public Form
	{
	protected:
		NewTestWizardControl *wizardControl;
		String *originalTitle;

		void onControlPageEnter(WizardPage *wp, WizardControl *wc);
		void onControlPageLeave(WizardPage *wp, WizardControl *wc);
		void onCancelClicked(Object *sender, EventArgs *args);
		void onFinishClicked(Object *sender, EventArgs *args);

	public:
		NewTestWizard(ScheduledTestDataNode *dataNode, bool modifyTest);
		~NewTestWizard();

		__property String *get_Function() { return wizardControl->Function; }
		__property String *get_ReturnValue() { return wizardControl->ReturnValue; }
		__property bool get_SpecifyErrorCode() { return wizardControl->SpecifyErrorCode; }
		__property String *get_ErrorCode() { return wizardControl->ErrorCode; }
		__property ArrayList *get_ParametersToMatch() { return wizardControl->ParametersToMatch; }
		__property ArrayList *get_ParametersToChange() { return wizardControl->ParametersToChange; }
		__property int get_FiringOption() { return wizardControl->FiringOption; }
		__property bool get_PauseAppOnTestFire() { return wizardControl->PauseAppOnTestFire; }
		__property Object* get_FiringData() { return wizardControl->FiringData; }
		__property bool get_IsHiddenFunction() { return wizardControl->IsHiddenFunction; }
	};
}