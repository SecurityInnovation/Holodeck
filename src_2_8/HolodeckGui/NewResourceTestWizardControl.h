//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewResourceTestWizardControl.h
//
// DESCRIPTION: Contains definition for the class NewResourceTestWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 17 Nov 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "WizardPages/ResourceTestResourceSelectionPage.h"
#include "WizardPages/ResourceTestFaultSelectionPage.h"
#include "WizardPages/ResourceTestSummaryPage.h"
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
using namespace Crownwood::Magic::Controls;
using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the new resource test wizard controller class
	//*************************************************************************
	public __gc class NewResourceTestWizardControl : public WizardControl
	{
	protected:
		ResourceTestResourceSelectionPage *resourceSelectionPage;
		ResourceTestFaultSelectionPage *faultSelectionPage;
		ResourceTestSummaryPage *summaryPage;
		WizardPage *currentWP;

		bool inSummaryMode;
		bool skipPageLeaveActions;
		ArrayList *resourcePaths;
		String *faultName;

		void onPageEnter(WizardPage *wp, WizardControl *wc);
		void onPageLeave(WizardPage *wp, WizardControl *wc);
		void onModifyResource();
		void onModifyFault();

	public:
		NewResourceTestWizardControl(SortedList *resourcePaths, ResourceTestDataNode *dataNode, bool modifyTest);
		~NewResourceTestWizardControl();

		__property ArrayList *get_ResourcePaths() { return resourceSelectionPage->ResourcePaths; }
		__property String *get_FaultName() { return faultSelectionPage->FaultName; }
		__property FaultType get_FaultType() { return faultSelectionPage->FaultType; }
		__property bool get_InSummaryMode() { return inSummaryMode; }
		__property void set_SkipPageLeaveActions(bool value) { this->skipPageLeaveActions = value;}

		void OnNextClick(CancelEventArgs *args);

		void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
		void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
	};
}