//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PropertiesPane.cpp
//
// DESCRIPTION: Contains implementation for the class PropertiesPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 11 Mar 2003		 B. Shirey		File created.
// 17 Mar 2003		 A. Kakrania	Added PropertyGrid control & EventHandler to catch FocusChange
//*************************************************************************
#include "PropertiesPane.h"
#include "PropertiesDescriptor.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		PropertiesPane
	// Description: Constructor for the PropertiesPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	PropertiesPane::PropertiesPane()
	{
		propertyGrid = new PropertyGrid();
		propertyGrid->Dock = DockStyle::Fill;
		propertyGrid->PropertySort = PropertySort::Alphabetical;
		propertyGrid->ViewForeColor = SystemColors::WindowText;

		this->Controls->Add (propertyGrid);
		
		FocusChangedDelegate * FocusChangedHandler = new FocusChangedDelegate (this, OnFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;

		//Start the refresh thread, refreshes the property grid every 4 seconds
		ThreadStart *refreshGridThreadStart = new ThreadStart(this, RefreshGridThreadFunc);
		refreshGridThread = new Threading::Thread(refreshGridThreadStart);
		refreshGridThread->IsBackground = true;
		refreshGridThread->Start();
	}

	//*************************************************************************
	// Method:		~PropertiesPane
	// Description: Destructor for the PropertiesPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	PropertiesPane::~PropertiesPane()
	{
		if (refreshGridThread != NULL && refreshGridThread->IsAlive)
			refreshGridThread->Abort();
	}

	//*************************************************************************
	// Method:		OnFocusChanged
	// Description: called when the focus in the UI shifts to another control
	//
	// Parameters:
	//	dataNode	- the object which the focus is on
	//
	// Return Value: none
	//*************************************************************************
	void PropertiesPane::OnFocusChanged(DisplayableDataNode *dataNode)
	{
		if (dataNode != NULL)
		{
			if (dataNode->DataArray)
				propertyGrid->SelectedObject = new PropertiesDescriptor (dataNode->DataArray);
			else
				propertyGrid->SelectedObject = new PropertiesDescriptor (dataNode);
		}
	}

	//*************************************************************************
	// Method:		RefreshGridThreadFunc
	// Description: Thread function which refreshes the grid every 4 seconds
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void PropertiesPane::RefreshGridThreadFunc()
	{
		try
		{
			while (true)
			{
				Thread::Sleep (4000);
				this->propertyGrid->Refresh();
			}
		}
		catch (...)
		{
			return;
		}
	}
}