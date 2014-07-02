//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HoloScriptRegistry.cpp
//
// DESCRIPTION: Contains implementation for the singleton class 
//	HoloScriptRegistry
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "HoloScriptRegistry.h"

namespace HoloScript
{
	//*************************************************************************
	// Method:		HoloScriptRegistry
	// Description: Constructor for the HoloScriptRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HoloScriptRegistry::HoloScriptRegistry()
	{
		processToHoloScriptAppTable = new SortedList();
		terminatedProcessToHoloScriptAppTable = new SortedList();
	}

	//*************************************************************************
	// Method:		~HoloScriptRegistry
	// Description: Destructor for the HoloScriptRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HoloScriptRegistry::~HoloScriptRegistry()
	{
		terminateAppsAndRemoveAll();
	}

	//*************************************************************************
	// Method:		getInstance
	// Description: Return the single instance of the HoloScriptRegistry class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of the HoloScriptRegistry class
	//*************************************************************************
	HoloScriptRegistry *HoloScriptRegistry::getInstance()
	{
		if (instance == NULL)
			instance = new HoloScriptRegistry();

		return instance;
	}

	//*************************************************************************
	// Method:		getHoloScriptAppForProcess
	// Description: Return the HoloScriptApplication object for a given process
	//
	// Parameters:
	//	procID - the process to get the HoloScriptApplication object for
	//
	// Return Value: the HoloScriptApplication object for the specified process
	//*************************************************************************
	HoloScriptApplication *HoloScriptRegistry::getHoloScriptAppForProcess(unsigned int procID)
	{
		return dynamic_cast<HoloScriptApplication *>(processToHoloScriptAppTable->get_Item(__box(procID)));
	}

	//*************************************************************************
	// Method:		getHoloScriptAppForTerminatedProcess
	// Description: Return the HoloScriptApplication object for a given process
	//
	// Parameters:
	//	procID - the process to get the HoloScriptApplication object for
	//
	// Return Value: the HoloScriptApplication object for the specified process
	//*************************************************************************
	HoloScriptApplication *HoloScriptRegistry::getHoloScriptAppForTerminatedProcess(unsigned int procID)
	{
		return dynamic_cast<HoloScriptApplication *>(terminatedProcessToHoloScriptAppTable->get_Item(__box(procID)));
	}

	//*************************************************************************
	// Method:		getHoloScriptAppByIndex
	// Description: Return the HoloScriptApplication object for a given index
	//
	// Parameters:
	//	index - the index to get the HoloScriptApplication object for
	//
	// Return Value: the HoloScriptApplication object for the specified index
	//*************************************************************************
	HoloScriptApplication *HoloScriptRegistry::getHoloScriptAppByIndex(int index)
	{
		return dynamic_cast<HoloScriptApplication *>(processToHoloScriptAppTable->GetByIndex(index));
	}

	//*************************************************************************
	// Method:		getTerminatedHoloScriptAppByIndex
	// Description: Return the HoloScriptApplication object for a given index
	//
	// Parameters:
	//	index - the index to get the HoloScriptApplication object for
	//
	// Return Value: the HoloScriptApplication object for the specified index
	//*************************************************************************
	HoloScriptApplication *HoloScriptRegistry::getTerminatedHoloScriptAppByIndex(int index)
	{
		return dynamic_cast<HoloScriptApplication *>(terminatedProcessToHoloScriptAppTable->GetByIndex(index));
	}
		
	//*************************************************************************
	// Method:		RemoveTerminatedProcesses
	// Description: Remove all hsApps from the terminate process table
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void HoloScriptRegistry::RemoveTerminatedProcesses()
	{ 
		for (int i=0; i< terminatedProcessToHoloScriptAppTable->Count; i++)
		{
			HoloScriptApplication *holoScriptApp = dynamic_cast <HoloScriptApplication*> (terminatedProcessToHoloScriptAppTable->GetByIndex(i));
			if (holoScriptApp)
				delete holoScriptApp;
		}

		terminatedProcessToHoloScriptAppTable->Clear(); 
	}

	//*************************************************************************
	// Method:		addHoloScriptAppForProcess
	// Description: Adds the HoloScriptApplication object for a given process
	//
	// Parameters:
	//	procID - the process to add the HoloScriptApplication object for
	//	app - the HoloScriptApplication object to associate with the process
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::addHoloScriptAppForProcess(unsigned int procID, HoloScriptApplication *app)
	{
		if (processToHoloScriptAppTable->Contains(__box(procID)))
			processToHoloScriptAppTable->Remove(__box(procID));
		else if (terminatedProcessToHoloScriptAppTable->Contains(__box(procID)))
			terminatedProcessToHoloScriptAppTable->Remove(__box(procID));

		processToHoloScriptAppTable->Add(__box(procID), app);
	}

	//*************************************************************************
	// Method:		removeHoloScriptAppForTermintatedProcess
	// Description: removes the HoloScriptApplication object for a given process
	//
	// Parameters:
	//	procID - the process representing the Holoscript object to remove
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::removeHoloScriptAppForTerminatedProcess(unsigned int procID)
	{
		terminatedProcessToHoloScriptAppTable->Remove(__box(procID));
		processToHoloScriptAppTable->Remove(__box(procID));
	}

	//*************************************************************************
	// Method:		PauseApplication
	// Description: pauses the application with the specified proc id
	//
	// Parameters:
	//	procID - the process to pause
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::PauseApplication(unsigned int procID)
	{
		HoloScriptApplication *holoScriptApp = this->getHoloScriptAppForProcess(procID);
		if (holoScriptApp)
			holoScriptApp->PauseApplication();
	}

	//*************************************************************************
	// Method:		ResumeApplication
	// Description: resumes the application with the specified proc id
	//
	// Parameters:
	//	procID - the process to un-pause
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::ResumeApplication(unsigned int procID)
	{
		HoloScriptApplication *holoScriptApp = this->getHoloScriptAppForProcess(procID);
		if (holoScriptApp)
			holoScriptApp->ResumeApplication();
	}

	//*************************************************************************
	// Method:		TerminateApplication
	// Description: terminates the application with the specified proc id
	//
	// Parameters:
	//	procID - the process to terminate
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::TerminateApplication(unsigned int procID)
	{
		HoloScriptApplication *holoScriptApp = this->getHoloScriptAppForProcess(procID);

		processToHoloScriptAppTable->Remove(__box(procID));

		if (!terminatedProcessToHoloScriptAppTable->Contains(__box(procID)))
			terminatedProcessToHoloScriptAppTable->Add(__box(procID), holoScriptApp);

		if (holoScriptApp)
			holoScriptApp->TerminateApplication();
	}

	//*************************************************************************
	// Method:		StartTerminateApplication
	// Description: terminates the application but doesn't yet remove from the running table
	//
	// Parameters:
	//	procID - the process to terminate
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::StartTerminateApplication(unsigned int procID)
	{
		HoloScriptApplication *holoScriptApp = this->getHoloScriptAppForProcess(procID);
		
		if (holoScriptApp && !terminatedProcessToHoloScriptAppTable->Contains(__box(procID)))
			terminatedProcessToHoloScriptAppTable->Add(__box(procID), holoScriptApp);

		if (holoScriptApp && holoScriptApp->IsRunning)
			holoScriptApp->TerminateApplication();
	}

	//*************************************************************************
	// Method:		EndTerminateApplication
	// Description: completes termination by removing from the running table 
	//	and adding to terminated table
	//
	// Parameters:
	//	procID - the process to terminate
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::EndTerminateApplication(unsigned int procID)
	{
		if (processToHoloScriptAppTable->Contains(__box(procID)))
			processToHoloScriptAppTable->Remove(__box(procID));
	}

	//*************************************************************************
	// Method:		terminateAppsAndRemoveAll
	// Description: terminates each holoscript app and removes it from the table
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void HoloScriptRegistry::terminateAppsAndRemoveAll()
	{
		while (processToHoloScriptAppTable->Count > 0)
		{
			HoloScriptApplication *holoScriptApp = dynamic_cast<HoloScriptApplication *>(processToHoloScriptAppTable->GetByIndex(0));
			if (holoScriptApp != NULL)
			{
				//Remove from the terminated list; if it was put there before to preserve the object
				unsigned int processID = 0;
				holoScriptApp->GetApplicationProcessID (processID);
				if (processToHoloScriptAppTable->Contains(__box(processID)))
					processToHoloScriptAppTable->Remove(__box(processID));

				holoScriptApp->TerminateApplication();

				delete holoScriptApp;
				holoScriptApp = NULL;
			}
		}
		
		for (int remDead=0; remDead < terminatedProcessToHoloScriptAppTable->Count; remDead++)
		{
			HoloScriptApplication *holoScriptApp = dynamic_cast<HoloScriptApplication *>(terminatedProcessToHoloScriptAppTable->GetByIndex(remDead));
			if (holoScriptApp != NULL)
			{
				//Remove from the terminated list; if it was put there before to preserve the object
				unsigned int processID = 0;
				holoScriptApp->GetApplicationProcessID (processID);
				holoScriptApp->TerminateApplication();

				delete holoScriptApp;
				holoScriptApp = NULL;
			}
		}
		terminatedProcessToHoloScriptAppTable->Clear();

		
		ServicesExeApplication = NULL;
	}

	//*************************************************************************
	// Method:		get_AnyAppsRunning
	// Description: are any holoscript apps running?
	//
	// Parameters:
	//	None
	//
	// Return Value: true if at least one app is running
	//*************************************************************************
	bool HoloScriptRegistry::get_AnyAppsRunning()
	{
	
		if (processToHoloScriptAppTable->Count > 0)
			return true;
		else
			return false;		
	}
}