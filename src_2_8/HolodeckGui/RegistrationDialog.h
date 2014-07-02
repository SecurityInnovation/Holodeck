//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RegistrationDialog.h
//
// DESCRIPTION: Contains definition for the class RegistrationDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 05 Sep 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "RegistrationMgr.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that provides controls for entering product registration
	//*************************************************************************
	public __gc class RegistrationDialog : public System::Windows::Forms::Form
	{
	protected:
		void InitializeUI();
		void UpdateFields();

		TextBox *serialNumberTextBox;
		TextBox *registrationKeyTextBox;
		Button *doneButton;
		Button *editButton;
		Button *registerButton;
		Label *informationLabel;
		Label *functionalityLevelLabel;
		Label *daysValidLabel;
		Label *licensesLabel;
		bool isFromTimeTrial;

		void onEditButtonClick(Object *sender, EventArgs *args);
		void onRegisterButtonClick(Object *sender, EventArgs *args);
		void OnLoad(EventArgs *args);

	public:
		RegistrationDialog();
		RegistrationDialog(bool isFromTimeTrialDialog);
		~RegistrationDialog();
	};
}