//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewProjectWizardControl.h
//
// DESCRIPTION: Contains definition for the class NewProjectWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "WizardPages/ProjectWorkspaceSelectionPage.h"
#include "WizardPages/ProjectApplicationSelectionPage.h"
#include "WizardPages/ProjectOptionsSelectionPage.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace System::Security::Permissions;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the new project wizard controller class
	//*************************************************************************
	public __gc class NewProjectWizardControl : public WizardControl
	{
	protected:
		ProjectWorkspaceSelectionPage *workspaceSelectionPage;
		ProjectApplicationSelectionPage *applicationSelectionPage;
		ProjectOptionsSelectionPage *optionsSelectionPage;

		String *projectPath;
		String *projectName;
		String *projectFolder;
		bool attachToApp;
		String *applicationPath;
		String *applicationName;
		unsigned int processID;
		String *commandLineParams;
		bool pauseOnStart;
		bool processChaining;
		bool inheritSettings;
		bool attachDebugger;
		bool logFirstChance;
		ArrayList *filteredFunctionList;
		WizardPage *currentWP;

		void onPageEnter(WizardPage *wp, WizardControl *wc);
		void onPageLeave(WizardPage *wp, WizardControl *wc);
		void onNextClick(Object *sender, CancelEventArgs *e);
		void onFinishClick(Object *sender, EventArgs *e);

	public:
		NewProjectWizardControl(bool newProject);
		~NewProjectWizardControl();

		__property bool get_LaunchApp() { return applicationSelectionPage->LaunchApp; }
		__property bool get_AttachToApp() { return applicationSelectionPage->AttachToApp; }
		__property bool get_StartAService() { return applicationSelectionPage->StartAService; }
		__property bool get_AttachToAService() { return applicationSelectionPage->AttachToAService; }

		__property void set_LaunchApp(bool value) { applicationSelectionPage->LaunchApp = value; }
		__property void set_AttachToApp(bool value) { applicationSelectionPage->AttachToApp = value; }
		__property void set_StartAService(bool value) { applicationSelectionPage->StartAService = value; }
		__property void set_AttachToAService(bool value) { applicationSelectionPage->AttachToAService = value; }

		__property String *get_ProjectPath() { return projectPath; }
		__property String *get_ProjectName() { return projectName; }
		__property String *get_ProjectFolder() { return projectFolder; }
		__property String *get_ApplicationPath() { return applicationPath; }
		__property String *get_ApplicationName() { return applicationName; }
		__property unsigned int get_ProcessID() { return processID; }
		__property String *get_CommandLineParams() { return commandLineParams; }
		__property String *get_ServiceToStart() { return applicationSelectionPage->ServiceToStart; }
		__property String *get_ServiceToAttachTo() { return applicationSelectionPage->ServiceToAttachTo; }

		__property bool get_PauseOnStart() { return pauseOnStart; }
		__property bool get_ProcessChaining() { return processChaining; }
		__property bool get_InheritSettings() { return inheritSettings; }
		__property bool get_AttachDebugger() { return attachDebugger; }
		__property bool get_LogFirstChance() { return logFirstChance; }
		__property ArrayList *get_FilteredFunctionList() { return filteredFunctionList; }

		__property void set_ApplicationPath(String *value) { applicationPath = value; }
		__property void set_ApplicationName(String *value) { applicationName = value; }
		__property void set_ProcessID(unsigned int value ) { processID = value; }
		__property void set_CommandLineParams(String *value) { commandLineParams = value; }
		__property void set_PauseOnStart(bool value) { pauseOnStart = value; }
		__property void set_ProcessChaining(bool value) { processChaining = value; }
		__property void set_InheritSettings(bool value) { inheritSettings = value; }
		__property void set_AttachDebugger(bool value) { attachDebugger = value; }
		__property void set_LogFirstChance(bool value) { logFirstChance = value; }
		__property void set_FilteredFunctionList(ArrayList *value) { filteredFunctionList = value; }
		__property void set_ServiceToStart(String *value) { applicationSelectionPage->ServiceToStart = value; }
		__property void set_ServiceToAttachTo(String *value) { applicationSelectionPage->ServiceToAttachTo = value; }

		void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
		void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
		void ReassessFunctions() { optionsSelectionPage->ApplicationPath = applicationPath; filteredFunctionList = optionsSelectionPage->FilteredFunctionList; }
	};
}