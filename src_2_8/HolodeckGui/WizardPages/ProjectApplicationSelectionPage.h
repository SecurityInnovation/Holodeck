//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectApplicationSelectionPage.h
//
// DESCRIPTION: Contains definition for the class ProjectApplicationSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor   File created.
//*************************************************************************
#pragma once

#include "ProcessListItemSorter.h"
#include "ServiceListItemSorter.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.ServiceProcess.dll>
#using <System.Windows.Forms.dll>
#using <System.Management.dll>
#using <System.Configuration.Install.dll>
#using <MagicLibrary.dll>
#using <GuiSupport.dll>
#using <HoloScript.dll>

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::ServiceProcess;
using namespace System::Management;
using namespace System::Configuration::Install;
using namespace Crownwood::Magic::Controls;
using namespace GuiSupport;
using namespace HoloScript;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the application selection page of the create project
	// wizard
	//*************************************************************************
	public __gc class ProjectApplicationSelectionPage : public WizardPage
	{
	public:
		ProjectApplicationSelectionPage();
		~ProjectApplicationSelectionPage();

		__property bool get_LaunchApp() { return (tabControl->SelectedTab == launchApplicationTabPage); }
		__property bool get_AttachToApp() { return (tabControl->SelectedTab == attachToApplicationTabPage); }
		__property bool get_StartAService() { return (tabControl->SelectedTab == startServiceTabPage); }
		__property bool get_AttachToAService() { return (tabControl->SelectedTab == attachToServiceTabPage); }

		__property void set_LaunchApp(bool value);
		__property void set_AttachToApp(bool value);
		__property void set_StartAService(bool value);
		__property void set_AttachToAService(bool value);

		__property String *get_ApplicationPath() { return AttachToApp ? applicationPath : fileInputBox->Text; }
		__property void set_ApplicationPath(String *value) { fileInputBox->Text = value; }
		__property String *get_ApplicationName() { return applicationName; }
		__property unsigned int get_ProcessID() { return processID; }
		__property String *get_CommandLineParams() { return paramsTextBox->Text; }
		__property void set_CommandLineParams(String *value) { paramsTextBox->Text = value; }
		__property String *get_ServiceToStart() { return serviceToStartName; }
		__property void set_ServiceToStart(String *value) { serviceToStartName = value; }
		__property String *get_ServiceToAttachTo() { return serviceToAttachToName; }
		__property void set_ServiceToAttachTo(String *value) { serviceToAttachToName = value; }

	protected:
		String *applicationPath;
		String *applicationName;
		unsigned int processID;
		int lastAttachAppClickedColumn;
		int lastLaunchServiceClickedColumn;
		int lastAttachServiceClickedColumn;
		String *serviceToStartName;
		String *serviceToAttachToName;

		Crownwood::Magic::Controls::TabControl *tabControl;
		Crownwood::Magic::Controls::TabPage *launchApplicationTabPage;
		Crownwood::Magic::Controls::TabPage *attachToApplicationTabPage;
		Crownwood::Magic::Controls::TabPage *startServiceTabPage;
		Crownwood::Magic::Controls::TabPage *attachToServiceTabPage;

		Button *browseButton;
		OpenFileDialog *openFileDialog;
		ComboBox *fileInputBox;
		Label *paramsLabel;
		TextBox *paramsTextBox;
		ColumnHeader *pidColumnHeader;
		ColumnHeader *procNameColumnHeader;
		ColumnHeader *serviceToStartNameColumnHeader;
		ColumnHeader *serviceToStartDescriptionColumnHeader;
		ColumnHeader *serviceToAttachToNameColumnHeader;
		ColumnHeader *serviceToAttachToDescriptionColumnHeader;
		ListView *processListView;
		ListView *serviceToStartListView;
		ListView *serviceToAttachToListView;
		LinkLabel *refreshApplicationsLink;
		LinkLabel *refreshServicesToStartLink;
		LinkLabel *refreshServicesToAttachToLink;

		ProcessListItemSorter *pidSorter;
		ProcessListItemSorter *processSorter;
		ServiceListItemSorter *serviceToStartNameSorter;
		ServiceListItemSorter *serviceToStartDescriptionSorter;
		ServiceListItemSorter *serviceToAttachToNameSorter;
		ServiceListItemSorter *serviceToAttachToDescriptionSorter;

		bool ProcessCmdKey(Message *msg, Keys keyData);
		void onBrowseClicked(Object *sender, EventArgs *e);
		void onRefreshApplicationsClicked(Object *sender, EventArgs *e);
		void onRefreshServicesToStartClicked(Object *sender, EventArgs *e);
		void onRefreshServicesToAttachToClicked(Object *sender, EventArgs *e);
		void onSelectedTabChanged(Object *sender, EventArgs *e);
		void onTextChanged(Object *sender, EventArgs *e);
		void onProcessSelectedIndexChanged(Object *sender, EventArgs *e);
		void onServiceToStartSelectedIndexChanged(Object *sender, EventArgs *e);
		void onServiceToAttachToSelectedIndexChanged(Object *sender, EventArgs *e);
		void onProcessColumnClicked(Object *sender, ColumnClickEventArgs *e);
		void onServiceToStartColumnClicked(Object *sender, ColumnClickEventArgs *e);
		void onServiceToAttachToColumnClicked(Object *sender, ColumnClickEventArgs *e);
	};
}