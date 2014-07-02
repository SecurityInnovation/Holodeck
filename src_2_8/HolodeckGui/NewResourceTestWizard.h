//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewResourceTestWizard.h
//
// DESCRIPTION: Contains definition for the class NewResourceTestWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "NewResourceTestWizardControl.h"
#include "ResourceTestDataNode.h"

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
	public __gc class NewResourceTestWizard : public Form
	{
	protected:
		NewResourceTestWizardControl *wizardControl;
		String *originalTitle;

		void onControlPageEnter(WizardPage *wp, WizardControl *wc);
		void onControlPageLeave(WizardPage *wp, WizardControl *wc);
		void onCancelClicked(Object *sender, EventArgs *args);
		void onFinishClicked(Object *sender, EventArgs *args);

	public:
		NewResourceTestWizard(SortedList *resourcePaths, ResourceTestDataNode *dataNode, bool modifyTest);
		~NewResourceTestWizard();

		__property ArrayList *get_ResourcePaths() { return wizardControl->ResourcePaths; }
		__property String *get_FaultName() { return wizardControl->FaultName; }
		__property FaultType get_FaultType() { return wizardControl->FaultType; }
	};
}