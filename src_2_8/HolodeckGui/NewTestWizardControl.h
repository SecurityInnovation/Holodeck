//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewTestWizardControl.h
//
// DESCRIPTION: Contains definition for the class NewTestWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "WizardPages/TestInParamSpecificationPage.h"
#include "WizardPages/TestFiringSpecificationPage.h"
#include "WizardPages/TestFunctionSelectionPage.h"
#include "WizardPages/TestOutParamSelectionPage.h"
#include "WizardPages/TestReturnValueSelectionPage.h"
#include "WizardPages/TestErrorCodeSelectionPage.h"
#include "WizardPages/TestSummaryPage.h"
#include "ScheduledTestDataNode.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <HoloScript.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the new test wizard controller class
	//*************************************************************************
	public __gc class NewTestWizardControl  : public WizardControl
	{
	protected:
		TestFunctionSelectionPage *functionSelectionPage;
		TestInParamSpecificationPage * inParamSpecificationPage;
		TestFiringSpecificationPage * testFiringSpecificationPage;
		TestOutParamSelectionPage *outParamSelectionPage;
		TestReturnValueSelectionPage *returnValueSelectionPage;
		TestErrorCodeSelectionPage *errorCodeSelectionPage;
		TestSummaryPage *summaryPage;

		bool skipPageLeaveActions;
		bool inSummaryMode;
		String *function;
		String *returnValue;
		bool specifyErrorCode;
		String *errorCode;
		ArrayList *parametersToMatch;
		ArrayList *parametersToChange;
		int firingOption;
		bool pauseAppOnTestFire;
		Object *firingData;
		bool isHiddenFunction;

		void onPageEnter(WizardPage *wp, WizardControl *wc);
		void onPageLeave(WizardPage *wp, WizardControl *wc);
		void onFunctionSelected(bool valid);
		void onModifyInParams();
		void onModifyOutParams();
		void onModifyReturnValue();
		void onModifyErrorCode();
		void onModifyFiringSpec();

		bool isNumericValue(String *value, bool allowNegative);

	public:
		NewTestWizardControl(ScheduledTestDataNode *dataNode, bool modifyTest);
		~NewTestWizardControl();

		__property int get_FiringOption() {return this->firingOption;}
		__property bool get_PauseAppOnTestFire() {return this->pauseAppOnTestFire;}
		__property Object * get_FiringData() {return this->firingData;}
		__property String *get_Function() { return function; }
		__property String *get_ReturnValue() { return returnValue; }
		__property bool get_SpecifyErrorCode() { return specifyErrorCode; }
		__property String *get_ErrorCode() { return errorCode; }
		__property ArrayList *get_ParametersToMatch() { return parametersToMatch; }
		__property ArrayList *get_ParametersToChange() { return parametersToChange; }
		__property bool get_IsHiddenFunction() { return isHiddenFunction; }
		__property bool get_InSummaryMode() { return inSummaryMode; }
		__property void set_SkipPageLeaveActions(bool value) { this->skipPageLeaveActions = value;}

		void OnNextClick(CancelEventArgs *args);

		void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
		void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
	};
}