//****************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomTestProjectWizard.h
//
// DESCRIPTION: Contains definition for the class CustomTestProjectWizard
//
//============================================================================
// Modification History
//
// Date         SCR  Name     Purpose
// -----------  ---  -------  ----------------------------------------
// 12 Feb 2004		 Ady K	  File created.
//****************************************************************************
#pragma once

#include "CustomTestProjectWizardControl.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// class that represents the add custom intercepts wizard dialog
	//*************************************************************************
	public __gc class CustomTestProjectWizard : public Form
	{
		protected:
			CustomTestProjectWizardControl *wizardControl;
			String *originalTitle;

			void onControlPageEnter(WizardPage *wp, WizardControl *wc);
			void onControlPageLeave(WizardPage *wp, WizardControl *wc);
			void onCancelClicked(Object *sender, EventArgs *args);
			void onFinishClicked(Object *sender, EventArgs *args);

		public:
			CustomTestProjectWizard();
			~CustomTestProjectWizard();
	};
}