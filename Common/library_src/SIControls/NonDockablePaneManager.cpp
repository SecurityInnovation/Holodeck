//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NonDockablePaneManager.cpp
//
// DESCRIPTION: Contains implementation for the class NonDockablePaneManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "NonDockablePaneManager.h"
#include "UIEventRegistry.h"
#include "LogPane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NonDockablePaneManager
	// Description: Constructor for the NonDockablePaneManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NonDockablePaneManager::NonDockablePaneManager()
	{
		InitializeUI();
	}

	//*************************************************************************
	// Method:		~NonDockablePaneManager
	// Description: Destructor for the NonDockablePaneManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NonDockablePaneManager::~NonDockablePaneManager()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: initialize the components in the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePaneManager::InitializeUI()
	{
		BoldSelectedPage = true;
		HotTrack = true;
		ShowClose = true;
		ShowArrows = true;
		ShrinkPagesToFit = false;
		PositionTop = true;
		Dock = DockStyle::Fill;
		Style = VisualStyle::IDE;
		IDEPixelBorder = true;
		ClosePressed += new EventHandler(this, onCloseButtonPressed);
		SelectionChanged += new EventHandler(this, onSelectionChanged);
		BackColor = Color::Gainsboro;

		helpPane = new HelpContentsPane();
		helpTabPage = new Crownwood::Magic::Controls::TabPage("Help Contents", helpPane, UIData::HelpDetails);

		helpTitleChangedHandler = new HelpContentsTitleChangedDelegate(this, onHelpTitleChanged);
		helpPane->OnTitleChanged += helpTitleChangedHandler;

		this->ImageList = ImageListManager::getInstance()->NodeTypeImageList;
	}

	//*************************************************************************
	// Method:		onCloseButtonPressed
	// Description: Handles the pane closing event for non dockable panes
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the cancel event
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePaneManager::onCloseButtonPressed(Object *sender, EventArgs *args)
	{
		Crownwood::Magic::Controls::TabPage *currentPage = SelectedTab;
		if (currentPage != NULL)
		{
			TabPages->Remove(currentPage);
			if (currentPage != helpTabPage)
			{
				Pane *pane = dynamic_cast <Pane*> (currentPage->Control);
				pane->Dispose(false);
			}
		}
	}

	//*************************************************************************
	// Method:		onSelectionChanged
	// Description: Handles the pane selction change event
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the change event
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePaneManager::onSelectionChanged(Object *sender, EventArgs *args)
	{
		Type *type;
		DisplayableDataNode *dataNode = NULL;
		Crownwood::Magic::Controls::TabPage *selectedPage = SelectedTab;
		if (selectedPage)
		{
			type = selectedPage->Control->GetType();
			if (type->Name->CompareTo("LogPane") == 0)
			{
				LogPane *logPane = dynamic_cast <LogPane*> (selectedPage->Control);
				dataNode = new DisplayableDataNode(DeconstructTitle(selectedPage->Title), UIData::ActiveLogPane, logPane->ProcessID, logPane->ThreadID);
				dataNode->DataArray = logPane->DataArray;
			}
			else if (type->Name->CompareTo("FaultPane") == 0)
			{
				FaultPane *faultPane = dynamic_cast <FaultPane*> (selectedPage->Control);
				dataNode = new DisplayableDataNode("Faults", UIData::Faults, faultPane->ProcessID, faultPane->ThreadID);
				dataNode->DataArray = faultPane->DataArray;
			}
			else if (type->Name->CompareTo("ReportPane") == 0)
			{
				ReportPane *reportPane = dynamic_cast <ReportPane*> (selectedPage->Control);
				dataNode = new DisplayableDataNode(selectedPage->Title, UIData::Report, 0, 0);
				dataNode->DataArray = reportPane->DataArray;
			}
			else if (type->Name->CompareTo("ScheduledTestPane") == 0)
			{
				ScheduledTestPane *scheduledTestPane = dynamic_cast <ScheduledTestPane*> (selectedPage->Control);
				dataNode = new DisplayableDataNode("Tests", UIData::Tests, scheduledTestPane->ProcessID, scheduledTestPane->ThreadID);
				dataNode->DataArray = scheduledTestPane->DataArray;
			}
			else if (type->Name->CompareTo("NetworkCorruptionPane") == 0)
			{
				NetworkCorruptionPane *networkCorruptionPane = dynamic_cast <NetworkCorruptionPane*> (selectedPage->Control);
				dataNode = new DisplayableDataNode("Network Corruption Fault", UIData::NetworkCorruptionFault, networkCorruptionPane->ProcessID, 0);
				dataNode->DataArray = networkCorruptionPane->DataArray;
			}
			else if (type->Name->CompareTo("ResourceTestPane") == 0)
			{
				ResourceTestPane *resourceTestPane = dynamic_cast <ResourceTestPane*> (selectedPage->Control);
				dataNode = new DisplayableDataNode("Resource Test", UIData::ResourceTest, resourceTestPane->ProcessID, 0);
				dataNode->DataArray = resourceTestPane->DataArray;
			}
			
			if (dataNode != NULL)
				UIEventRegistry::getInstance()->OnFocusChanged(dataNode);
		}
	}

	//*************************************************************************
	// Method:		onHelpTitleChanged
	// Description: Handles the help pane title change event
	//
	// Parameters:
	//	text - the new help pane title
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePaneManager::onHelpTitleChanged(String *text)
	{
		helpTabPage->Title = text;
	}

	//*************************************************************************
	// Method:		displayHelpPane
	// Description: displays the contents of the location in the help pane
	//
	// Parameters:
	//	location - the location of the page to display
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePaneManager::displayHelpPane(String *location)
	{
		helpPane->showContents(location);
		if (!TabPages->Contains(helpTabPage))
			TabPages->Add(helpTabPage);
		helpTabPage->Selected = true;
	}

	//*************************************************************************
	// Method:		DeconstructTitle
	// Description: return just exename portion of title for passing to focus changed delegates
	//
	// Parameters:
	//	title - tab page title to deconstruct
	//
	// Return Value: string that contains just the exe name
	//*************************************************************************
	String *NonDockablePaneManager::DeconstructTitle(String *title)
	{
		int index = title->LastIndexOf(".");
		return title->Substring(0, index+4);
	}

	//*************************************************************************
	// Method:		addPage
	// Description: adds a tab page to the window.  Cross thread safe
	//
	// Parameters:
	//	newPage - the page to add
	//
	// Return Value: None
	//*************************************************************************
	void NonDockablePaneManager::addPage(Crownwood::Magic::Controls::TabPage *newPage)
	{
		this->TabPages->Add(newPage);
	}
}