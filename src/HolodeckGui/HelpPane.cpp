//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HelpPane.cpp
//
// DESCRIPTION: Contains implementation for the class HelpPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "HelpPane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		HelpPane
	// Description: Constructor for the HelpPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HelpPane::HelpPane()
	{
		htmlBrowser = new HTMLBrowser();
		htmlBrowser->AllowNavigationInThisBrowser = false;

		this->displayHelpUrls = true;
		lastUrl = "about:blank";
		lastTargetFrame = NULL;

		Controls->Add(htmlBrowser->browser);

		FocusChangedHandler = new FocusChangedDelegate(this, onFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;

		LinkClickedHandler = new WebBrowserLinkClickedDelegate(this, onBrowserLinkClicked);
		htmlBrowser->OnLinkClicked += LinkClickedHandler;

		CloseProjectHandler = new CloseProjectUIDelegate(this, onCloseProject);
		uiEventRegistry->OnCloseProjectUI += CloseProjectHandler;
	}

	//*************************************************************************
	// Method:		~HelpPane
	// Description: Destructor for the HelpPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	HelpPane::~HelpPane()
	{
	}

	//*************************************************************************
	// Method:		set_DisplayUrls
	// Description: Sets whether or not a page will be displayed
	//
	// Parameters:
	//	value - true to display urls in the pane, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void HelpPane::set_DisplayUrls(bool value)
	{ 
		displayHelpUrls = value; 
		if (value)
			DisplayUrl(lastUrl, lastTargetFrame);
	}

	//*************************************************************************
	// Method:		DisplayUrl
	// Description: Display an html page in the help pane
	//
	// Parameters:
	//	url - the url to display
	//	targetFrame - the frame to display the url in
	//
	// Return Value: None
	//*************************************************************************
	void HelpPane::DisplayUrl(String *url, String *targetFrame)
	{
		lastUrl = url;
		lastTargetFrame = targetFrame;
		if (htmlBrowser && url && this->DisplayUrls)
			htmlBrowser->Navigate(url, targetFrame);
	}

	//*************************************************************************
	// Method:		OnFocusChanged
	// Description: Called when focus changes in any ui pane
	//
	// Parameters:
	//	dataNode - datanode representing focus change
	//
	// Return Value: none
	//*************************************************************************
	void HelpPane::onFocusChanged(DisplayableDataNode *dataNode)
	{
		String *url = NULL;

		if (dataNode)
		{
			url = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/");
			switch(dataNode->Type)
			{
			case UIData::Process:
				url = String::Concat(url, PROCESS_DYNAMIC_HELP_FILE);
				break;
			case UIData::Thread:
				url = String::Concat(url, THREAD_DYNAMIC_HELP_FILE);
				break;
			case UIData::Reports:
			case UIData::Report:
				url = String::Concat(url, REPORTS_DYNAMIC_HELP_FILE);
				break;
			case UIData::RecordedSessions:
			case UIData::RecordedSession:
				url = String::Concat(url, RECORDED_DYNAMIC_HELP_FILE);
				break;
			case UIData::Tests:
			case UIData::Test:
				url = String::Concat(url, TESTS_DYNAMIC_HELP_FILE);
				break;
			case UIData::Files:
			case UIData::File:
			case UIData::Registry:
			case UIData::RegistryEntry:
				url = String::Concat(url, RESOURCES_DYNAMIC_HELP_FILE);
				break;
			case UIData::Resources:
			case UIData::MemoryResource:
			case UIData::DiskResource:
			case UIData::NetworkResource:
				url = String::Concat(url, RESOURCES_DYNAMIC_HELP_FILE);
				break;
			case UIData::Limits:
			case UIData::DiskLimit:
			case UIData::MemoryLimit:
			case UIData::NetworkUpLimit:
			case UIData::NetworkDownLimit:
				url = String::Concat(url, LIMITS_DYNAMIC_HELP_FILE);
				break;
			case UIData::Logs:
			case UIData::SavedLogs:
			case UIData::ActiveLogPane:
			case UIData::SavedLogPane:
				url = String::Concat(url, LOGS_DYNAMIC_HELP_FILE);
				break;
			case UIData::Faults:
			case UIData::MemoryFaultType:
			case UIData::DiskFaultType:
			case UIData::NetworkFaultType:
			case UIData::RegistryFaultType:
			case UIData::ProcessFaultType:
			case UIData::CustomFaultType:
			case UIData::MemoryFault:
			case UIData::DiskFault:
			case UIData::NetworkFault:
			case UIData::RegistryFault:
			case UIData::ProcessFault:
			case UIData::CustomFault:
				url = String::Concat(url, FAULTS_DYNAMIC_HELP_FILE);
				break;
			case UIData::NodeType::NetworkCorruptionFault:
			case UIData::NodeType::NetworkCorruptionFaultType:
				url = String::Concat(url, NETWORK_DYNAMIC_HELP_FILE);
				break;
			case UIData::HelpDetails:
			case UIData::DynamicHelp:
				url = BLANK_HTML_FILE;
				break;
			case UIData::CorruptedFiles:
			case UIData::CorruptedFile:
			case UIData::SavedCorruptedFiles:
			case UIData::SavedCorruptedFile:
				url = String::Concat(url, CORRUPTEDFILES_DYNAMIC_HELP_FILE);
				break;
			case UIData::ExceptionPane:
			case UIData::ExceptionInfo:
				url = String::Concat(url, EXCEPTIONS_DYNAMIC_HELP_FILE);
				break;
			case UIData::FileCorruptionFaultType:
			case UIData::FileCorruptionFault:
				url = String::Concat(url, FILEFAULTS_DYNAMIC_HELP_FILE);
				break;
			case UIData::NetworkLogs:
			case UIData::ActiveNetworkLogPane:
			case UIData::SavedNetworkLogs:
			case UIData::SavedNetworkLogPane:
				url = String::Concat(url, NETWORKLOGS_DYNAMIC_HELP_FILE);
				break;
			case UIData::ResourceFaults:
			case UIData::ResourceFault:
				url = String::Concat(url, RESOURCEFAULTS_DYNAMIC_HELP_FILE);
				break;
			default:
				url = BLANK_HTML_FILE;
				break;
			}
		}
		else
			return;

		if (url != NULL)
			DisplayUrl(url, "HelpDetailsPane");
	}

	//*************************************************************************
	// Method:		OnBrowserLinkClicked
	// Description: Called when one of the browser's links is clicked
	//
	// Parameters:
	//	sender - the sender of the event
	//	url - the url to navigate to
	//
	// Return Value: none
	//*************************************************************************
	void HelpPane::onBrowserLinkClicked(Object *sender, String *url)
	{
		OnHelpPaneLinkClicked(sender, url);
	}

	//*************************************************************************
	// Method:		onCloseProject
	// Description: Called when a project is closed
	//
	// Parameters:
	//	None
	//
	// Return Value: none
	//*************************************************************************
	void HelpPane::onCloseProject()
	{
		DisplayUrl(BLANK_HTML_FILE, "HelpDetailsPane");
	}
}