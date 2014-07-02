//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustomInterceptWizardControl.h
//
// DESCRIPTION: Contains definition for the class AddCustomInterceptWizardControl
//
//===============================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 01 Oct 2003		 P. Singh	 File created.
//*******************************************************************************
#pragma once

#include "AddCustIntWizPg1.h"
#include "AddCustIntWizPg2.h"
#include "AddCustIntWizPg3.h"
#include "AddCustIntWizPg4.h"

#undef MessageBox

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::Threading;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace GuiSupport;

namespace AddNewIntercepts
{
	class VerificationUtils
	{
	public:
		static bool verifyFunctionDetails(functionInformation * finfo);
		static bool isValidIdentifier(String * idname);
	};

	//*************************************************************************
	// class that represents the add custom intercept wizard controller class
	//*************************************************************************
	public __gc class AddCustomInterceptWizardControl : public WizardControl
	{
		protected:
			AddCustIntWizPg1 *addCustIntPg1;
			AddCustIntWizPg2 *addCustIntPg2;
			AddCustIntWizPg3 *addCustIntPg3;
			AddCustIntWizPg4 *addCustIntPg4;

			Threading::Thread *builderThread;

			String *projectPath;
			String *projectName;

			void onPageEnter(WizardPage *wp, WizardControl *wc);
			void onPageLeave(WizardPage *wp, WizardControl *wc);
			void onNextClick(Object *sender, CancelEventArgs *e);
			void onFinishClick(Object *sender, EventArgs *e);

			WizardPage *currentWP;
			GuiSupport::WaitDialog * waitDialog;
			bool createSuccess;

			void BuildProjectsThread();

		public:
			AddCustomInterceptWizardControl();
			~AddCustomInterceptWizardControl();

			__property String* get_ProjectName(void) { return projectName; }
			__property void set_ProjectName(String *value) { projectName = value; }
			__property String* get_ProjectPath(void) { return projectPath; }
			__property void set_ProjectPath(String *value) { projectPath = value; }

			void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
			void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }	
			void waitDialogCancelEventHandlerFunc (Object * sender, EventArgs * e);
	};
}