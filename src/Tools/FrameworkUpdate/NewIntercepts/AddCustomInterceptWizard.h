//****************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustomInterceptWizard.h
//
// DESCRIPTION: Contains definition for the class AddCustomInterceptWizard
//
//============================================================================
// Modification History
//
// Date         SCR  Name     Purpose
// -----------  ---  -------  ----------------------------------------
// 01 Oct 2003		 P.Singh  File created.
//****************************************************************************
#pragma once

#include "AddCustomInterceptWizardControl.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace AddNewIntercepts
{
	//*************************************************************************
	// class that represents the add custom intercepts wizard dialog
	//*************************************************************************
	public __gc class AddCustomInterceptWizard : public Form
	{
		protected:
			AddCustomInterceptWizardControl *wizardControl;
			String *originalTitle;

			void onControlPageEnter(WizardPage *wp, WizardControl *wc);
			void onControlPageLeave(WizardPage *wp, WizardControl *wc);
			void onCancelClicked(Object *sender, EventArgs *args);
			void onFinishClicked(Object *sender, EventArgs *args);

		public:
			AddCustomInterceptWizard();
			~AddCustomInterceptWizard();
	};
}