//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectApplicationSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectApplicationSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include "stdio.h"
#include "ProjectApplicationSelectionPage.h"
#include "defines.h"
#include "../UserNotification.h"

#undef MessageBox

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ProjectApplicationSelectionPage
	// Description: Constructor for the ProjectApplicationSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectApplicationSelectionPage::ProjectApplicationSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select an application to test";
		lastAttachAppClickedColumn = -1;
		lastLaunchServiceClickedColumn = -1;
		lastAttachServiceClickedColumn = -1;

		Label *fileInputLabel = new Label();
		fileInputLabel->Location = System::Drawing::Point(8, 96);
		fileInputLabel->Text = "Application to Launch:";
		fileInputLabel->Size = System::Drawing::Size(152, 16);

		fileInputBox = new ComboBox();
		fileInputBox->Location = System::Drawing::Point(8, 116);
		fileInputBox->Name = "fileInputBox";
		fileInputBox->Size = System::Drawing::Size(306, 21);
		fileInputBox->TabIndex = 0;
		fileInputBox->TextChanged += new EventHandler(this, onTextChanged);

		Array *recentItems = RegistryHelper::getInstance()->GetRecentlyLoadedItems();
		for (int i = 0; i < recentItems->Length; i++)
		{
			String *val = dynamic_cast <String*> (recentItems->GetValue(0, i));
			if (val)
			{
				fileInputBox->Items->Add(val);
			}
		}

		browseButton = new Button();
		browseButton->Location = System::Drawing::Point(322, 116);
		browseButton->Name = "browseButton";
		browseButton->TabIndex = 1;
		browseButton->Text = "B&rowse...";

		openFileDialog = new OpenFileDialog();
		browseButton->Click += new EventHandler(this, onBrowseClicked);

		paramsLabel = new Label();
		paramsLabel->Location = System::Drawing::Point(8, 156);
		paramsLabel->Name = "paramsLabel";
		paramsLabel->Size = System::Drawing::Size(152, 16);
		paramsLabel->TabIndex = 6;
		paramsLabel->Text = "Command Line Parameters:";

		paramsTextBox = new TextBox();
		paramsTextBox->Location = System::Drawing::Point(8, 176);
		paramsTextBox->Name = "paramsTextBox";
		paramsTextBox->Size = System::Drawing::Size(314 + browseButton->Width, 20);
		paramsTextBox->TabIndex = 5;

		Label *launchApplicationInstructions = new Label();
		launchApplicationInstructions->Text = "Please select an application to launch.  Click 'Browse...' to select the application.  You can also pull down a list of recently launched applications by clicking the down arrow in the 'Application to Launch' field.  If you want to pass command line parameters to the application you can do so by typing them in.";
		launchApplicationInstructions->Location = System::Drawing::Point(8, 8);
		launchApplicationInstructions->Size = System::Drawing::Size(314 + browseButton->Width, 80);

		launchApplicationTabPage = new Crownwood::Magic::Controls::TabPage("Launch an App");
		launchApplicationTabPage->Controls->Add(launchApplicationInstructions);
		launchApplicationTabPage->Controls->Add(fileInputLabel);
		launchApplicationTabPage->Controls->Add(fileInputBox);
		launchApplicationTabPage->Controls->Add(browseButton);
		launchApplicationTabPage->Controls->Add(paramsLabel);
		launchApplicationTabPage->Controls->Add(paramsTextBox);

		processListView = new ListView();
		
		pidSorter = new ProcessListItemSorter(1);
		processSorter = new ProcessListItemSorter(0);

		pidColumnHeader = new ColumnHeader();
		pidColumnHeader->Text = "PID";
		pidColumnHeader->Width = 50;
		
		procNameColumnHeader = new ColumnHeader();
		procNameColumnHeader->Text = "Process name";
		procNameColumnHeader->Width = 322;

		processListView->Columns->Add(procNameColumnHeader);
		processListView->Columns->Add(pidColumnHeader);
		processListView->FullRowSelect = true;
		processListView->HideSelection = false;
		processListView->GridLines = true;
		processListView->Location = System::Drawing::Point(8, 60);
		processListView->MultiSelect = false;
		processListView->Name = "processListBox";
		processListView->Size = System::Drawing::Size(392, 164);
		processListView->Sorting = System::Windows::Forms::SortOrder::Ascending;
		processListView->TabIndex = 4;
		processListView->View = System::Windows::Forms::View::Details;
		processListView->ListViewItemSorter = pidSorter;
		processListView->ColumnClick += new ColumnClickEventHandler(this, onProcessColumnClicked);
		processListView->SelectedIndexChanged += new EventHandler(this, onProcessSelectedIndexChanged);
 
		refreshApplicationsLink = new LinkLabel();
		refreshApplicationsLink->Location = System::Drawing::Point(350, 44);
		refreshApplicationsLink->Size = System::Drawing::Size(50, 16);
		refreshApplicationsLink->TabStop = true;
		refreshApplicationsLink->Text = "Refresh";
		refreshApplicationsLink->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
		refreshApplicationsLink->Click += new EventHandler(this, onRefreshApplicationsClicked);

		Label *attachToApplicationInstructions = new Label();
		attachToApplicationInstructions->Text = "Please select a process to attach to.  All currently running processes are listed below.";
		attachToApplicationInstructions->Location = System::Drawing::Point(8, 8);
		attachToApplicationInstructions->Size = System::Drawing::Size(392, 36);

		attachToApplicationTabPage = new Crownwood::Magic::Controls::TabPage("Attach to an App");
		attachToApplicationTabPage->Controls->Add(attachToApplicationInstructions);
		attachToApplicationTabPage->Controls->Add(refreshApplicationsLink);
		attachToApplicationTabPage->Controls->Add(processListView);

		serviceToStartNameSorter = new ServiceListItemSorter(0);
		serviceToStartDescriptionSorter = new ServiceListItemSorter(1);

		serviceToStartNameColumnHeader = new ColumnHeader();
		serviceToStartNameColumnHeader->Text = "Name";
		serviceToStartNameColumnHeader->Width = 150;
		
		serviceToStartDescriptionColumnHeader = new ColumnHeader();
		serviceToStartDescriptionColumnHeader->Text = "Description";
		serviceToStartDescriptionColumnHeader->Width = 300;

		serviceToStartListView = new ListView();
		serviceToStartListView->Columns->Add(serviceToStartNameColumnHeader);
		serviceToStartListView->Columns->Add(serviceToStartDescriptionColumnHeader);
		serviceToStartListView->FullRowSelect = true;
		serviceToStartListView->HideSelection = false;
		serviceToStartListView->GridLines = true;
		serviceToStartListView->Location = System::Drawing::Point(8, 60);
		serviceToStartListView->MultiSelect = false;
		serviceToStartListView->Name = "processListBox";
		serviceToStartListView->Size = System::Drawing::Size(372, 144);
		serviceToStartListView->Sorting = System::Windows::Forms::SortOrder::Ascending;
		serviceToStartListView->TabIndex = 4;
		serviceToStartListView->View = System::Windows::Forms::View::Details;
		serviceToStartListView->ListViewItemSorter = serviceToStartNameSorter;
		serviceToStartListView->ColumnClick += new ColumnClickEventHandler(this, onServiceToStartColumnClicked);
		serviceToStartListView->SelectedIndexChanged += new EventHandler(this, onServiceToStartSelectedIndexChanged);

		refreshServicesToStartLink = new LinkLabel();
		refreshServicesToStartLink->Location = System::Drawing::Point(330, 40);
		refreshServicesToStartLink->Size = System::Drawing::Size(50, 16);
		refreshServicesToStartLink->TabStop = true;
		refreshServicesToStartLink->Text = "Refresh";
		refreshServicesToStartLink->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
		refreshServicesToStartLink->Click += new EventHandler(this, onRefreshServicesToStartClicked);

		Label *startServiceInstructions = new Label();
		startServiceInstructions->Text = "Please select a service to start.  All services that are not disabled and are not currently running are listed below.";
		startServiceInstructions->Location = System::Drawing::Point(8, 8);
		startServiceInstructions->Size = System::Drawing::Size(372, 30);

		startServiceTabPage = new Crownwood::Magic::Controls::TabPage("Start Service");
		startServiceTabPage->Controls->Add(startServiceInstructions);
		startServiceTabPage->Controls->Add(refreshServicesToStartLink);
		startServiceTabPage->Controls->Add(serviceToStartListView);

		serviceToAttachToNameSorter = new ServiceListItemSorter(0);
		serviceToAttachToDescriptionSorter = new ServiceListItemSorter(1);

		serviceToAttachToNameColumnHeader = new ColumnHeader();
		serviceToAttachToNameColumnHeader->Text = "Name";
		serviceToAttachToNameColumnHeader->Width = 150;
		
		serviceToAttachToDescriptionColumnHeader = new ColumnHeader();
		serviceToAttachToDescriptionColumnHeader->Text = "Description";
		serviceToAttachToDescriptionColumnHeader->Width = 300;

		serviceToAttachToListView = new ListView();
		serviceToAttachToListView->Columns->Add(serviceToAttachToNameColumnHeader);
		serviceToAttachToListView->Columns->Add(serviceToAttachToDescriptionColumnHeader);
		serviceToAttachToListView->FullRowSelect = true;
		serviceToAttachToListView->HideSelection = false;
		serviceToAttachToListView->GridLines = true;
		serviceToAttachToListView->Location = System::Drawing::Point(8, 60);
		serviceToAttachToListView->MultiSelect = false;
		serviceToAttachToListView->Name = "processListBox";
		serviceToAttachToListView->Size = System::Drawing::Size(372, 144);
		serviceToAttachToListView->Sorting = System::Windows::Forms::SortOrder::Ascending;
		serviceToAttachToListView->TabIndex = 4;
		serviceToAttachToListView->View = System::Windows::Forms::View::Details;
		serviceToAttachToListView->ListViewItemSorter = serviceToAttachToNameSorter;
		serviceToAttachToListView->ColumnClick += new ColumnClickEventHandler(this, onServiceToAttachToColumnClicked);
		serviceToAttachToListView->SelectedIndexChanged += new EventHandler(this, onServiceToAttachToSelectedIndexChanged);

		refreshServicesToAttachToLink = new LinkLabel();
		refreshServicesToAttachToLink->Location = System::Drawing::Point(330, 40);
		refreshServicesToAttachToLink->Size = System::Drawing::Size(50, 16);
		refreshServicesToAttachToLink->TabStop = true;
		refreshServicesToAttachToLink->Text = "Refresh";
		refreshServicesToAttachToLink->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
		refreshServicesToAttachToLink->Click += new EventHandler(this, onRefreshServicesToAttachToClicked);

		Label *attachToServiceInstructions = new Label();
		attachToServiceInstructions->Text = "Please select a service to attach to.  All services that are currently running are listed below.";
		attachToServiceInstructions->Location = System::Drawing::Point(8, 8);
		attachToServiceInstructions->Size = System::Drawing::Size(372, 30);

		attachToServiceTabPage = new Crownwood::Magic::Controls::TabPage("Attach to Service");
		attachToServiceTabPage->Controls->Add(attachToServiceInstructions);
		attachToServiceTabPage->Controls->Add(refreshServicesToAttachToLink);
		attachToServiceTabPage->Controls->Add(serviceToAttachToListView);

		tabControl = new Crownwood::Magic::Controls::TabControl();
		tabControl->BoldSelectedPage = true;
		tabControl->HotTrack = true;
		tabControl->ShowClose = false;
		tabControl->ShowArrows = false;
		tabControl->ShrinkPagesToFit = true;
		tabControl->PositionTop = true;
		tabControl->Dock = DockStyle::Fill;
		tabControl->Style = Crownwood::Magic::Common::VisualStyle::IDE;
		tabControl->IDEPixelBorder = true;
		tabControl->SelectionChanged += new EventHandler(this, onSelectedTabChanged);
		tabControl->SwitchTabPagesOnCtrlTab = true;

		tabControl->TabPages->Add(launchApplicationTabPage);
		tabControl->TabPages->Add(attachToApplicationTabPage);

		// Don't allow attach or launch of services on Longhorn (causes system to crash, see bug 1853)
		OSVERSIONINFO osVersion;
		osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
		GetVersionEx(&osVersion);
		if (osVersion.dwMajorVersion < 6)
		{
			tabControl->TabPages->Add(startServiceTabPage);
			tabControl->TabPages->Add(attachToServiceTabPage);
		}

		tabControl->SelectedTab = launchApplicationTabPage;

		Controls->Add(tabControl);
	}

	//*************************************************************************
	// Method:		~ProjectApplicationSelectionPage
	// Description: Destructor for the ProjectApplicationSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectApplicationSelectionPage::~ProjectApplicationSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		ProcessCmdKey
	// Description: Handles nonstandard key presses
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	bool ProjectApplicationSelectionPage::ProcessCmdKey(Message *msg, Keys keyData)	
	{
		if ((msg->Msg == WM_KEYDOWN) || (msg->Msg == WM_SYSKEYDOWN))
		{
			switch(keyData)
			{
			case Keys::Control | Keys::Tab:
				tabControl->SimulateCtrlTabPress();
				return true;
			case Keys::Control | Keys::Tab | Keys::Shift:
				tabControl->SimulateCtrlShiftTabPress();
				return true;
			}
		}

		return __super::ProcessCmdKey(msg, keyData);
	}

	//*************************************************************************
	// Method:		set_LaunchApp
	// Description: Sets whether to set the launch app tab page to be current
	//
	// Parameters:
	//	value - true to set it current, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::set_LaunchApp(bool value)
	{
		if (value)
			tabControl->SelectedTab = launchApplicationTabPage;
	}

	//*************************************************************************
	// Method:		set_AttachToApp
	// Description: Sets whether to set the attacht to app tab page to be current
	//
	// Parameters:
	//	value - true to set it current, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::set_AttachToApp(bool value)
	{
		if (value)
			tabControl->SelectedTab = attachToApplicationTabPage;
	}

	//*************************************************************************
	// Method:		set_StartService
	// Description: Sets whether to set the start service tab page to be current
	//
	// Parameters:
	//	value - true to set it current, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::set_StartAService(bool value)
	{
		if (value)
			tabControl->SelectedTab = startServiceTabPage;
	}

	//*************************************************************************
	// Method:		set_AttachToService
	// Description: Sets whether to set the attach to service tab page to be current
	//
	// Parameters:
	//	value - true to set it current, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::set_AttachToAService(bool value)
	{
		if (value)
			tabControl->SelectedTab = attachToServiceTabPage;
	}

	//*************************************************************************
	// Method:		onBrowseClick
	// Description: Event handler for browse button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onBrowseClicked(Object *sender, EventArgs *e)
	{
		openFileDialog->Filter = "Applications (*.exe)|*.exe";
		if (File::Exists(fileInputBox->Text))
			openFileDialog->FileName = fileInputBox->Text;
		else if (Directory::Exists(fileInputBox->Text))
			openFileDialog->InitialDirectory = fileInputBox->Text;

		openFileDialog->ShowDialog(this);
		fileInputBox->Text = openFileDialog->FileName;
	}

	//*************************************************************************
	// Method:		onRefreshApplicationsClicked
	// Description: Event handler for application refresh link click
	//
	// Parameters:
	//	sender - the sender of the click event
	//	e - the args representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onRefreshApplicationsClicked(Object *sender, EventArgs *e)
	{
		System::Diagnostics::Process *processList[];
		System::Diagnostics::Process *currentProcess;

		try
		{
			processList = System::Diagnostics::Process::GetProcesses();
		}
		catch (Exception *)
		{
			MessageBox::Show ("There was a problem getting the list of processes from your computer. Please ensure that you have sufficient rights.", "Holodeck");
			processListView->Clear();
		}

		if (processList == NULL)
			return;

		ListViewItem *lvi = new ListViewItem();
		processListView->Items->Clear();
		for (int i=0; i<processList->Length; i++)
		{
			lvi = new ListViewItem();
			currentProcess = dynamic_cast <System::Diagnostics::Process*> (processList->GetValue(i));
			
			try
			{
				lvi->Text = Path::GetFileName(currentProcess->MainModule->FileName);
			}
			catch(...)
			{
				lvi->Text = currentProcess->ProcessName;
			}
			lvi->SubItems->Add (currentProcess->Id.ToString());

			HoloScriptApplication *app = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(currentProcess->Id);
			if ((!HoloScriptApplication::IsSystemProcess(currentProcess->Id)) && (app == NULL))
				processListView->Items->Add(lvi);
		}
		
		processListView->ListViewItemSorter = processSorter;
		processListView->Sort();
	}

	//*************************************************************************
	// Method:		onRefreshServicesToStartClicked
	// Description: Event handler for start service refresh link click
	//
	// Parameters:
	//	sender - the sender of the click event
	//	e - the args representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onRefreshServicesToStartClicked(Object *sender, EventArgs *e)
	{
		try
		{
			ServiceController *services[] = ServiceController::GetServices();
			ServiceController *thisService = NULL;

			serviceToStartListView->Items->Clear();
			for (int i = 0; i < services->Length; i++)
			{
				thisService = services[i];
				if (thisService && (thisService->Status == ServiceControllerStatus::Stopped))
				{
					String *path = String::Format("Win32_Service.Name='{0}'", thisService->ServiceName);
					ManagementObject *wmiService = new ManagementObject(path);
					if (!wmiService)
						continue;
					wmiService->Get();

					Object *description = wmiService->get_Item(S"Description");
					Object *startMode = wmiService->get_Item(S"Startmode");

					// don't show disabled services
					if (startMode->ToString()->ToLower()->CompareTo("disabled") == 0)
						continue;
					
					ListViewItem *lvi = new ListViewItem();
					lvi->Text = thisService->DisplayName;
					if (!description)
						lvi->SubItems->Add("--No Description Available--");
					else
						lvi->SubItems->Add(description->ToString());

					if (!HoloScriptApplication::IsSystemService(thisService->ServiceName, false))
						serviceToStartListView->Items->Add(lvi);
				}
			}
		}
		catch(...)
		{
			UserNotification::InfoNotify("Holodeck was unable to get information about all the possible services to start.\n\nPlease make sure your 'Windows Management Instrumentation' (winmgmt) service is started and configured correctly");
		}
		
		serviceToStartListView->ListViewItemSorter = serviceToStartNameSorter;
		serviceToStartListView->Sort();
	}

	//*************************************************************************
	// Method:		onRefreshServicesToAttachToClicked
	// Description: Event handler for attach to service refresh link click
	//
	// Parameters:
	//	sender - the sender of the click event
	//	e - the args representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onRefreshServicesToAttachToClicked(Object *sender, EventArgs *e)
	{
		try
		{
			ServiceController *services[] = ServiceController::GetServices();
			ServiceController *thisService = NULL;

			serviceToAttachToListView->Items->Clear();
			for (int i = 0; i < services->Length; i++)
			{
				thisService = services[i];
				if (thisService && (thisService->Status == ServiceControllerStatus::Running))
				{
					String *path = String::Format("Win32_Service.Name='{0}'", thisService->ServiceName);
					ManagementObject *wmiService = new ManagementObject(path);
					if (!wmiService)
						continue;
					wmiService->Get();

					Object *description = wmiService->get_Item(S"Description");
					
					ListViewItem *lvi = new ListViewItem();
					lvi->Text = thisService->DisplayName;
					if (!description)
						lvi->SubItems->Add("--No Description Available--");
					else
						lvi->SubItems->Add(description->ToString());

					ServiceHelper *helper = new ServiceHelper();
					unsigned long pid = helper->GetProcessIDOfRunningService(thisService->ServiceName);
					if (pid != 0)
					{
						// NOTE: do not need to check the terminated list if not found here since if the service isn't
						// running we aren't going to display it anyway
						HoloScriptApplication *app = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(pid);

						if ((!HoloScriptApplication::IsSystemService(thisService->ServiceName, true)) && (app == NULL))
							serviceToAttachToListView->Items->Add(lvi);
					}
				}
			}
		}
		catch(...)
		{
			UserNotification::InfoNotify("Holodeck was unable to get information about all the possible services to attach to.\n\nPlease make sure your 'Windows Management Instrumentation' (winmgmt) service is started and configured correctly");
		}
		
		serviceToAttachToListView->ListViewItemSorter = serviceToAttachToNameSorter;
		serviceToAttachToListView->Sort();
	}

	//*************************************************************************
	// Method:		onTextChanged
	// Description: Event handler for file combobox text changes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onTextChanged(Object *sender, EventArgs *e)
	{
		int index = fileInputBox->Text->LastIndexOf("\\");

		index++;
		applicationPath = fileInputBox->Text;
		applicationName = fileInputBox->Text->Substring(index, applicationPath->Length - index);
		processID = 0;
	}

	//*************************************************************************
	// Method:		onProcessSelectedIndexChanged
	// Description: Event handler for list view index change
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onProcessSelectedIndexChanged(Object *sender, EventArgs *e)
	{
		if (processListView->SelectedItems->Count > 0)
		{
			applicationName = processListView->SelectedItems->Item[0]->Text;
			processID = Convert::ToUInt32(processListView->SelectedItems->Item[0]->SubItems->get_Item(1)->Text);

			System::Diagnostics::Process *currentProcess = System::Diagnostics::Process::GetProcessById(processID);
			try
			{
				// Get full path info for the process
				applicationPath = currentProcess->MainModule->FileName;
			}
			catch (...)
			{
				// No valid path info
				applicationPath = "";
			}
		}
	}

	//*************************************************************************
	// Method:		onServiceToStartSelectedIndexChanged
	// Description: Event handler for list view index change
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onServiceToStartSelectedIndexChanged(Object *sender, EventArgs *e)
	{
		if (serviceToStartListView->SelectedItems->Count > 0)
		{
			serviceToStartName = serviceToStartListView->SelectedItems->Item[0]->Text;
		}
	}

	//*************************************************************************
	// Method:		onServiceToAttachToSelectedIndexChanged
	// Description: Event handler for list view index change
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onServiceToAttachToSelectedIndexChanged(Object *sender, EventArgs *e)
	{
		if (serviceToAttachToListView->SelectedItems->Count > 0)
		{
			serviceToAttachToName = serviceToAttachToListView->SelectedItems->Item[0]->Text;
		}
	}

	//*************************************************************************
	// Method:		onProcessColumnClicked
	// Description: Event handler for column header click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onProcessColumnClicked(Object *sender, ColumnClickEventArgs *e)
	{
		ProcessListItemSorter *sorter = NULL;

		switch(e->Column)
		{
			case 1:
				processListView->ListViewItemSorter = pidSorter;
				break;
			case 0:
				processListView->ListViewItemSorter = processSorter;
				break;
			default:
				break;
		}

		if ((lastAttachAppClickedColumn == e->Column) && (processListView->Sorting == SortOrder::Ascending))
		{
			processListView->Sorting = SortOrder::Descending;
			sorter = dynamic_cast <ProcessListItemSorter*> (processListView->ListViewItemSorter);
			sorter->Ascending = false;
		}
		else
		{
			processListView->Sorting = SortOrder::Ascending;
			sorter = dynamic_cast <ProcessListItemSorter*> (processListView->ListViewItemSorter);
			sorter->Ascending = true;
		}

		lastAttachAppClickedColumn = e->Column;
		processListView->Sort();
	}

	//*************************************************************************
	// Method:		onServiceToStartColumnClicked
	// Description: Event handler for column header click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onServiceToStartColumnClicked(Object *sender, ColumnClickEventArgs *e)
	{
		ServiceListItemSorter *sorter = NULL;

		switch(e->Column)
		{
			case 0:
				serviceToStartListView->ListViewItemSorter = serviceToStartNameSorter;
				break;
			case 1:
				serviceToStartListView->ListViewItemSorter = serviceToStartDescriptionSorter;
				break;
			default:
				break;
		}

		if ((lastLaunchServiceClickedColumn == e->Column) && (serviceToStartListView->Sorting == SortOrder::Ascending))
		{
			serviceToStartListView->Sorting = SortOrder::Descending;
			sorter = dynamic_cast <ServiceListItemSorter*> (serviceToStartListView->ListViewItemSorter);
			sorter->Ascending = false;
		}
		else
		{
			serviceToStartListView->Sorting = SortOrder::Ascending;
			sorter = dynamic_cast <ServiceListItemSorter*> (serviceToStartListView->ListViewItemSorter);
			sorter->Ascending = true;
		}

		lastLaunchServiceClickedColumn = e->Column;
		serviceToStartListView->Sort();
	}

	//*************************************************************************
	// Method:		onServiceToAttachToColumnClicked
	// Description: Event handler for column header click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onServiceToAttachToColumnClicked(Object *sender, ColumnClickEventArgs *e)
	{
		ServiceListItemSorter *sorter = NULL;

		switch(e->Column)
		{
			case 0:
				serviceToAttachToListView->ListViewItemSorter = serviceToAttachToNameSorter;
				break;
			case 1:
				serviceToAttachToListView->ListViewItemSorter = serviceToAttachToDescriptionSorter;
				break;
			default:
				break;
		}

		if ((lastAttachServiceClickedColumn == e->Column) && (serviceToAttachToListView->Sorting == SortOrder::Ascending))
		{
			serviceToAttachToListView->Sorting = SortOrder::Descending;
			sorter = dynamic_cast <ServiceListItemSorter*> (serviceToAttachToListView->ListViewItemSorter);
			sorter->Ascending = false;
		}
		else
		{
			serviceToAttachToListView->Sorting = SortOrder::Ascending;
			sorter = dynamic_cast <ServiceListItemSorter*> (serviceToAttachToListView->ListViewItemSorter);
			sorter->Ascending = true;
		}

		lastAttachServiceClickedColumn = e->Column;
		serviceToAttachToListView->Sort();
	}

	//*************************************************************************
	// Method:		onSelectedTabChanged
	// Description: Event handler for changing the selected tab
	//
	// Parameters:
	//	sender - the sender of the change event
	//	e - the args representing the change event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectApplicationSelectionPage::onSelectedTabChanged(Object *sender, EventArgs *e)
	{
		if (AttachToApp)
			onRefreshApplicationsClicked(NULL, EventArgs::Empty);
		else if (StartAService)
			onRefreshServicesToStartClicked(NULL, EventArgs::Empty);
		else if (AttachToAService)
			onRefreshServicesToAttachToClicked(NULL, EventArgs::Empty);
	}
}
