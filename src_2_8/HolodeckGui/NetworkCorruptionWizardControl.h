//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionWizardControl.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "WizardPages/NetworkCorruptionPortSelectionPage.h"
#include "WizardPages/NetworkCorruptionTypeSelectionPage.h"
#include "WizardPages/CorruptionRandomSelectionPage.h"
#include "WizardPages/CorruptionSearchSelectionPage.h"
#include "WizardPages/CorruptionRegExprSelectionPage.h"
#include "WizardPages/NetworkCorruptionSummaryPage.h"

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
	public __value struct NetworkCorruptionUISettings
	{
		String* type;
		CorruptionRandomSettings randomSettings;
		CorruptionSearchSettings searchSettings;
		CorruptionRegExprSettings regExprSettings;
	};

	public __gc class NetworkCorruptionDataNode;
}


namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the network corruption wizard controller class
	//*************************************************************************
	public __gc class NetworkCorruptionWizardControl : public WizardControl
	{
	protected:
		NetworkCorruptionPortSelectionPage *portSelectionPage;
		NetworkCorruptionTypeSelectionPage *typeSelectionPage;
		CorruptionRandomSelectionPage *randomSelectionPage;
		CorruptionSearchSelectionPage *searchSelectionPage;
		CorruptionRegExprSelectionPage *regExprSelectionPage;
		NetworkCorruptionSummaryPage *summaryPage;

		CorruptionRandomSettings randomSettings;
		CorruptionSearchSettings searchSettings;
		CorruptionRegExprSettings regExprSettings;

		ArrayList* portList;
		bool sentData, receivedData;
		String *portStr;
		String *searchRegExpr;
		String *replacementStr;
		String *corruptType;
		String *searchFor;
		String *replaceWith;

		ArrayList* createdDataNodes;

		bool inSummaryMode;
		bool skipPageLeaveActions;

		void onPageEnter(WizardPage *wp, WizardControl *wc);
		void onPageLeave(WizardPage *wp, WizardControl *wc);

		void onModifyPort();
		void onModifyType();
		void onModifySearchAndReplace();

	public:
		NetworkCorruptionWizardControl(NetworkCorruptionDataNode* dataNode, bool modifyNode);
		~NetworkCorruptionWizardControl();

		__property ArrayList *get_PortList() { return portList; }
		__property bool get_SentData() { return sentData; }
		__property bool get_ReceivedData() { return receivedData; }
		__property String *get_PortString() { return portStr; }
		__property String *get_SearchRegularExpression() { return searchRegExpr; }
		__property String *get_ReplacementString() { return replacementStr; }
		__property String *get_CorruptType() { return corruptType; }
		__property String *get_SearchFor() { return searchFor; }
		__property String *get_ReplaceWith() { return replaceWith; }
		__property bool get_InSummaryMode() { return inSummaryMode; }
		__property void set_SkipPageLeaveActions(bool value) { this->skipPageLeaveActions = value;}

		__property NetworkCorruptionUISettings get_Settings();
		__property void set_Settings(NetworkCorruptionUISettings settings);

		void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
		void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }

		void OnNextClick(CancelEventArgs *args);
	};
}