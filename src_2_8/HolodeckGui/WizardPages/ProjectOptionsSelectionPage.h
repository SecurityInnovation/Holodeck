//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectOptionsSelectionPage.h
//
// DESCRIPTION: Contains definition for the class ProjectOptionsSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor   File created.
//*************************************************************************
#pragma once

#include "defines.h"
#include "../UIEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <GuiSupport.dll>
#using <CustomTestProjectGenerator.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace GuiSupport;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the options selection page of the create project
	// wizard
	//*************************************************************************
	public __gc class ProjectOptionsSelectionPage : public WizardPage
	{
	public:
		ProjectOptionsSelectionPage();
		~ProjectOptionsSelectionPage();

		__property bool get_PauseOnStart() { return pauseCheckBox->Checked; }
		__property void set_PauseOnStart(bool value) { pauseCheckBox->Checked = value; }
		__property bool get_ProcessChaining() { return processChainingCheckBox->Checked; }
		__property void set_ProcessChaining(bool value) { processChainingCheckBox->Checked = value; }
		__property bool get_InheritSettings() { return inheritSettingsCheckBox->Checked; }
		__property void set_InheritSettings(bool value) { inheritSettingsCheckBox->Checked = value; }
		__property bool get_AttachDebugger() { return attachDebuggerCheckBox->Checked; }
		__property void set_AttachDebugger(bool value) { attachDebuggerCheckBox->Checked = value; }
		__property bool get_LogFirstChance() { return logFirstChanceCheckBox->Checked; }
		__property void set_LogFirstChance(bool value) { logFirstChanceCheckBox->Checked = value; }
		__property ArrayList *get_FilteredFunctionList();
		__property void set_FilteredFunctionList(ArrayList *value);
		void SetApplication(String* value, bool attaching);
		__property void set_ApplicationPath(String* value) { SetApplication(value, false); }

	protected:
		CustomTestProjectGenerator::FunctionCheckChangedDelegate * CheckedEventHandler;
		CustomTestProjectGenerator::FunctionTreeView *logFilterTreeView;

		FunctionDatabase::InterceptedFunctionDB * nativeInterceptedFunctionDB;

		CheckBox *pauseCheckBox;
		CheckBox *processChainingCheckBox;
		CheckBox *inheritSettingsCheckBox;
		CheckBox *attachDebuggerCheckBox;
		CheckBox *logFirstChanceCheckBox;
		LinkLabel *lnkLoggingOptionsLearnMore;
		
		ArrayList *missingFunctions;
		ArrayList * exceptionCases;
		ArrayList * requiredCategories;

		void processChainingCheckedChanged(Object *sender, EventArgs *e);
		void attachDebuggerCheckedChanged(Object *sender, EventArgs *e);
		void lnkLoggingOptionsLearnMore_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e);
		void functionCheckChanged_Handler(String * functionName, bool IsChecked);
	};
}