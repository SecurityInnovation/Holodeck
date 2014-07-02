//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HelpContentsPane.cpp
//
// DESCRIPTION: Contains implementation for the singleton class HelpContentsPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "HelpContentsPane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		HelpContentsPane
	// Description: Constructor for the HelpContentsPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HelpContentsPane::HelpContentsPane()
	{
		htmlBrowser = new HTMLBrowser();

		browserTitleChangedHandler = new WebBrowserTitleChangedDelegate(this, onTitleChanged);
		htmlBrowser->OnTitleChanged += browserTitleChangedHandler;

		Controls->Add(htmlBrowser->browser);
	}

	//*************************************************************************
	// Method:		~HelpContentsPane
	// Description: Destructor for the HelpContentsPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HelpContentsPane::~HelpContentsPane()
	{
	}

	//*************************************************************************
	// Method:		showContents
	// Description: Display the contents at location in the pane
	//
	// Parameters:
	//	location - the location of the contents to display
	//
	// Return Value: None
	//*************************************************************************
	void HelpContentsPane::showContents(String *location)
	{
		if (htmlBrowser != 0)
			htmlBrowser->Navigate(location);
	}

	//*************************************************************************
	// Method:		onTitleChanged
	// Description: Gets called when the title of the help page changes
	//
	// Parameters:
	//	sender - the object who sent the event
	//	text - the new title text
	//
	// Return Value: None
	//*************************************************************************
	void HelpContentsPane::onTitleChanged(Object *sender, String *text)
	{
		OnTitleChanged->Invoke(text);
	}
}