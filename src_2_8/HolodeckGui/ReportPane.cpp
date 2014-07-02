//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReportPane.cpp
//
// DESCRIPTION: Contains implementation for the singleton class ReportPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 8 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ReportPane.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ReportPane
	// Description: Constructor for the ReportPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ReportPane::ReportPane(ReportPaneDataArray *reportDataArray)
	{
		htmlBrowser = new HTMLBrowser();
		Controls->Add(htmlBrowser->browser);

		dataArray = reportDataArray;
	}

	//*************************************************************************
	// Method:		~ReportPane
	// Description: Destructor for the ReportPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ReportPane::~ReportPane()
	{
	}

	//*************************************************************************
	// Method:		showContents
	// Description: Display the contents at location in the pane
	//
	// Parameters:
	//	location - the location of the contents to display
	//
	// Return Value: true if successfull
	//*************************************************************************
	bool ReportPane::showContents(String *location)
	{
		try
		{
			htmlBrowser->Navigate(location);
			return true;
		}
		catch(...)
		{
			UserNotification::ErrorNotify(String::Concat("Report file can't be found at: ", location));
			return false;
		}
	}
}