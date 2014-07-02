//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewProjectWizard.h
//
// DESCRIPTION: Contains definition for the class NewProjectWizard
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "NewProjectWizardControl.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the new project wizard dialog
	//*************************************************************************
	public __gc class NewProjectWizard : public Form
	{
	protected:
		NewProjectWizardControl *wizardControl;
		String *originalTitle;

		void onControlPageEnter(WizardPage *wp, WizardControl *wc);
		void onControlPageLeave(WizardPage *wp, WizardControl *wc);
		void onCancelClicked(Object *sender, EventArgs *args);
		void onFinishClicked(Object *sender, EventArgs *args);

	public:
		NewProjectWizard(bool newProject);
		~NewProjectWizard();

		__property bool get_LaunchApp() { return wizardControl->LaunchApp; }
		__property bool get_AttachToApp() { return wizardControl->AttachToApp; }
		__property bool get_StartAService() { return wizardControl->StartAService; }
		__property bool get_AttachToAService() { return wizardControl->AttachToAService; }

		__property void set_LaunchApp(bool value) { wizardControl->LaunchApp = value; }
		__property void set_AttachToApp(bool value) { wizardControl->AttachToApp = value; }
		__property void set_StartAService(bool value) { wizardControl->StartAService = value; }
		__property void set_AttachToAService(bool value) { wizardControl->AttachToAService = value; }

		__property String *get_ProjectPath() { return wizardControl->ProjectPath; }
		__property String *get_ProjectName() { return wizardControl->ProjectName; }
		__property String *get_ProjectFolder() { return wizardControl->ProjectFolder; }
		__property String *get_ApplicationPath() { return wizardControl->ApplicationPath; }
		__property String *get_ApplicationName() { return wizardControl->ApplicationName; }
		__property unsigned int get_ProcessID() { return wizardControl->ProcessID; }
		__property String *get_CommandLineParams() { return wizardControl->CommandLineParams; }
		__property String *get_ServiceToStart() { return wizardControl->ServiceToStart; }
		__property String *get_ServiceToAttachTo() { return wizardControl->ServiceToAttachTo; }

		__property bool get_PauseOnStart() { return wizardControl->PauseOnStart; }
		__property bool get_ProcessChaining() { return wizardControl->ProcessChaining; }
		__property bool get_InheritSettings() { return wizardControl->InheritSettings; }
		__property bool get_AttachDebugger() { return wizardControl->AttachDebugger; }
		__property bool get_LogFirstChance() { return wizardControl->LogFirstChance; }
		__property ArrayList *get_FilteredFunctionList() { return wizardControl->FilteredFunctionList; }

		__property void set_ApplicationPath(String *value) { wizardControl->ApplicationPath = value; }
		__property void set_ApplicationName(String *value) { wizardControl->ApplicationName = value; }
		__property void set_FilteredFunctionList(ArrayList *value) { wizardControl->FilteredFunctionList = value; }
		__property void set_PauseOnStart(bool value) { wizardControl->PauseOnStart = value; }
		__property void set_ProcessChaining(bool value) { wizardControl->ProcessChaining = value; }
		__property void set_InheritSettings(bool value) { wizardControl->InheritSettings = value; }
		__property void set_AttachDebugger(bool value) { wizardControl->AttachDebugger = value; }
		__property void set_LogFirstChance(bool value) { wizardControl->LogFirstChance = value; }
		__property void set_CommandLineParams(String *value) { wizardControl->CommandLineParams = value; }
		__property void set_ProcessID(unsigned int value) { wizardControl->ProcessID = value; }
		__property void set_ServiceToStart(String *value) { wizardControl->ServiceToStart = value; }
		__property void set_ServiceToAttachTo(String *value) { wizardControl->ServiceToAttachTo = value; }

		void ReassessFunctions() { wizardControl->ReassessFunctions(); }
	};
}