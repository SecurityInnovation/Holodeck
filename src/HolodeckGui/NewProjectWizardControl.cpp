//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewProjectWizardControl.cpp
//
// DESCRIPTION: Contains implementation for the class NewProjectWizardControl
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
#include "NewProjectWizardControl.h"
#include "UserNotification.h"

#using <HoloScript.dll>

using namespace HoloScript;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NewProjectWizardControl
	// Description: Constructor for the NewProjectWizardControl class
	//
	// Parameters:
	//	newProject - if true we are a new project wizard, if false we are a new test app wizard
	//
	// Return Value: None
	//*************************************************************************
	NewProjectWizardControl::NewProjectWizardControl(bool newProject)
	{
		this->Profile = Crownwood::Magic::Controls::WizardControl::Profiles::Configure;
		this->Visible = true;
		this->AssignDefaultButton = true;
		this->SelectedIndex = 0;
		this->Dock = DockStyle::Fill;
		this->EnableNextButton = WizardControl::Status::Yes;
		this->EnableBackButton = WizardControl::Status::No;
		this->EnableFinishButton = WizardControl::Status::No;
		this->ButtonNextText = "&Next";
		this->ButtonBackText = "&Back";
		this->ButtonFinishText = "&Finish";
		this->ButtonCancelText = "&Cancel";

		if (newProject)
		{		
			this->Title = "Workspace Selection";
			workspaceSelectionPage = new ProjectWorkspaceSelectionPage();
			applicationSelectionPage = new ProjectApplicationSelectionPage();
			optionsSelectionPage = new ProjectOptionsSelectionPage();

			this->WizardPages->Add(workspaceSelectionPage);
			this->WizardPages->Add(applicationSelectionPage);
			this->WizardPages->Add(optionsSelectionPage);

			currentWP = workspaceSelectionPage;
		}
		else
		{
			this->Title = "Application Selection";
			applicationSelectionPage = new ProjectApplicationSelectionPage();
			optionsSelectionPage = new ProjectOptionsSelectionPage();
			
			this->WizardPages->Add(applicationSelectionPage);
			this->WizardPages->Add(optionsSelectionPage);

			currentWP = applicationSelectionPage;
		}

		this->WizardPageEnter += new WizardPageHandler(this, onPageEnter);
		this->WizardPageLeave += new WizardPageHandler(this, onPageLeave);
		this->NextClick += new CancelEventHandler(this, onNextClick);
		this->FinishClick += new EventHandler(this, onFinishClick);

	}

	//*************************************************************************
	// Method:		~NewProjectWizardControl
	// Description: Destructor for the NewProjectWizardControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NewProjectWizardControl::~NewProjectWizardControl()
	{
	}

	//*************************************************************************
	// Method:		onPageEnter
	// Description: Called when a page is being entered
	//
	// Parameters:
	//	wp - The wizard page being entered
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewProjectWizardControl::onPageEnter(WizardPage *wp, WizardControl *wc)
	{
		// set the title
		if (wp == workspaceSelectionPage)
		{
			wc->Title = "Workspace Selection";
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::No;
		}
		else if (wp == applicationSelectionPage)
		{
			wc->Title = "Application Selection";
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
		}
		else if (wp == optionsSelectionPage)
		{
			wc->Title = "Options Selection";
			this->EnableNextButton = WizardControl::Status::No;
			this->EnableBackButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::Yes;
		}
		currentWP = wp;
	}

	//*************************************************************************
	// Method:		onPageLeave
	// Description: Called when a page is being left
	//
	// Parameters:
	//	wp - The wizard page being left
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewProjectWizardControl::onPageLeave(WizardPage *wp, WizardControl *wc)
	{
		if (wp == workspaceSelectionPage)
		{
			this->projectPath = workspaceSelectionPage->ProjectPath;
			this->projectName = workspaceSelectionPage->ProjectName;
			this->projectFolder = workspaceSelectionPage->ProjectFolder;
		}
		else if (wp == applicationSelectionPage)
		{
			this->attachToApp = applicationSelectionPage->AttachToApp;
			this->applicationPath = applicationSelectionPage->ApplicationPath;
			this->applicationName = applicationSelectionPage->ApplicationName;
			this->processID = applicationSelectionPage->ProcessID;
			this->commandLineParams = applicationSelectionPage->CommandLineParams;

			if (applicationSelectionPage->AttachToAService)
			{
				optionsSelectionPage->SetApplication(ServiceHelper::GetServiceExecutablePath(applicationSelectionPage->ServiceToAttachTo), true);

				// If we are running on Windows 2000, we won't be able to detach a debugger.  Since we are
				// attaching to a running service and we will want to be able to detach, disable the debugger
				// as the default.
				OSVERSIONINFO osVersion;
				osVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
				GetVersionEx(&osVersion);
				if ((osVersion.dwMajorVersion < 5) || ((osVersion.dwMajorVersion == 5) && (osVersion.dwMinorVersion == 0)))
				{
					// Windows 2000, uncheck the attach debugger option by default
					optionsSelectionPage->AttachDebugger = false;
				}
			}
			else if (applicationSelectionPage->StartAService)
				optionsSelectionPage->SetApplication(ServiceHelper::GetServiceExecutablePath(applicationSelectionPage->ServiceToStart), false);
			else if (applicationSelectionPage->AttachToApp)
				optionsSelectionPage->SetApplication(this->applicationPath, true);
			else
				optionsSelectionPage->SetApplication(this->applicationPath, false);
		}
	}

	//*************************************************************************
	// Method:		onFinishClick
	// Description: Event handler for next button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NewProjectWizardControl::onFinishClick(Object *sender, EventArgs *e)
	{
		RegistryHelper *regHelper = RegistryHelper::getInstance();
		if (currentWP == optionsSelectionPage)
		{
			this->pauseOnStart = optionsSelectionPage->PauseOnStart;
			this->processChaining = optionsSelectionPage->ProcessChaining;
			this->inheritSettings = optionsSelectionPage->InheritSettings;
			this->attachDebugger = optionsSelectionPage->AttachDebugger;
			this->logFirstChance = optionsSelectionPage->LogFirstChance;
			this->filteredFunctionList = optionsSelectionPage->FilteredFunctionList;
			if (workspaceSelectionPage)
                regHelper->SetLastProjectSavePath (System::IO::Path::GetDirectoryName (workspaceSelectionPage->ProjectPath));
		}
	}

	//*************************************************************************
	// Method:		onNextClick
	// Description: Event handler for next button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NewProjectWizardControl::onNextClick(Object *sender, CancelEventArgs *e)
	{
		int index;
		String *projectFolder;
		Type *type = sender->GetType();

		try
		{
			if ( (currentWP == workspaceSelectionPage) && (workspaceSelectionPage->ProjectPath) )
			{
				// Path::GetFullPath returns an empty string for certain illegal chars, 
				// ArgumentException is thrown for illegal chars or for lack of permissions
				String *filePath = Path::GetFullPath(workspaceSelectionPage->ProjectPath);
				String *fileExtension = Path::GetExtension(filePath);
				if (fileExtension && (fileExtension->Length == 0))
				{
					UserNotification::InfoNotify("Please enter a name for your project");
					e->Cancel = true;
				}
				else
				{
					//check to see if project file already exists and ask for overwrite
					if (File::Exists(filePath))
					{
						if (UserNotification::AskNotify (String::Concat ("The project file ", filePath, " already exists. Do you wish to overwrite?")) != IDYES)
							e->Cancel = true;
					}
					
					//check path
					index = filePath->LastIndexOf("\\");
					projectFolder = filePath->Substring(0, index);
					if ( (!System::IO::Directory::Exists(projectFolder)) || (filePath->Chars[index+1] == '.') )
					{
						UserNotification::InfoNotify("Please select a valid project path");
						e->Cancel = true;
					}
					else
					{
						try
						{
							unsigned int i = 1;
							String *fileName = String::Format("{0}\\hdtest{1}.txt", projectFolder, i.ToString());
							// i will eventually rollover to 0 once we've used every other possibility, so stop then
							while (System::IO::File::Exists(fileName) && (i != 0))
							{
								i++;
								fileName = String::Format("{0}\\hdtest{1}.txt", projectFolder, i.ToString());
							}

							// couldn't find a file to try to write.  very unlikely
							if (System::IO::File::Exists(fileName))
								throw new Exception();

							//try writing a file...
							StreamWriter *testFile = System::IO::File::CreateText(fileName);
							testFile->Write("Test");
							testFile->Close();
							System::IO::File::Delete(fileName);
						}
						catch(...)
						{
							UserNotification::InfoNotify("You don't have permissions to write to this path");
							e->Cancel = true;
						}
					}
				}
			}
			else if (currentWP == applicationSelectionPage)
			{
				if ( (applicationSelectionPage->LaunchApp) && 
					((!System::IO::File::Exists(applicationSelectionPage->ApplicationPath)) || (!applicationSelectionPage->ApplicationPath->ToLower()->EndsWith(".exe"))) )
				{
					UserNotification::InfoNotify("Please select a valid application path");
					e->Cancel = true;
				}
				else if ( (applicationSelectionPage->LaunchApp) && (HoloScriptApplication::IsSystemProcess(applicationSelectionPage->ApplicationPath)) )
				{
					UserNotification::InfoNotify("This is an important system process and attaching Holodeck to it may make the system unstable.  Please choose a different application to launch");
					e->Cancel = true;
				}
				else if ( (applicationSelectionPage->AttachToApp) && (applicationSelectionPage->ProcessID == 0) )
				{
					UserNotification::InfoNotify("Please select an application to attach to");
					e->Cancel = true;
				}
				else if ( (applicationSelectionPage->StartAService) && (!applicationSelectionPage->ServiceToStart))
				{
					UserNotification::InfoNotify("Please select a service to start");
					e->Cancel = true;
				}
				else if (applicationSelectionPage->AttachToAService)
				{
					if (applicationSelectionPage->ServiceToAttachTo)
					{
						// find out if the service is shared or runs in its own process
						ServiceController *service = new ServiceController(applicationSelectionPage->ServiceToAttachTo);
						if (service && ((service->ServiceType & ServiceType::Win32ShareProcess) != 0))
						{
							String *msg = String::Format("The service that you have chosen to attach to - '{0}' - shares its process space with other services.\n\nSince there is more than 1 service running in the process, it is possible that you will receive logs from services other than the one you have chosen to attach to.  To avoid this, Stop the {0} service and launch it from the 'Start Service' tab of the New Project or New Test Application wizard in Holodeck.", applicationSelectionPage->ServiceToAttachTo);
							UserNotification::InfoNotify(msg);
						}
					}
					else
					{
						UserNotification::InfoNotify("Please select a service to attach to");
						e->Cancel = true;
					}
				}
			}
		}
		catch (ArgumentOutOfRangeException *)
		{
			UserNotification::InfoNotify("Please enter the full path to the project");
			e->Cancel = true;
		}
		catch (ArgumentException *)
		{
			UserNotification::InfoNotify("You have entered an invalid project path.\n\nYou may not use any of the following characters in the project path: /, *, ?, :, <, >, |, \"");
			e->Cancel = true;
		}		
		catch (Security::SecurityException *)
		{
			UserNotification::InfoNotify("You don't have permissions to write to this path");
			e->Cancel = true;
		}
		catch (...)
		{
			UserNotification::InfoNotify("Please select a valid project path");
			e->Cancel = true;
		}
	}
}
