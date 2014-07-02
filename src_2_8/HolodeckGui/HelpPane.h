//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HelpPane.h
//
// DESCRIPTION: Contains definition for the class HelpPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "DockablePane.h"
#include "FaultPaneDataArray.h"
#include "LogPaneDataArray.h"
#include "defines.h"
#include "HTMLBrowser.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace AxSHDocVw;
using namespace MSHTML;
using namespace UIData;

namespace HolodeckGui
{
	__delegate void HelpPaneLinkClickedDelegate(Object *sender, String *url);

	//*************************************************************************
	// class that all represents the context sensitive dockable help pane
	//*************************************************************************
	public __gc class HelpPane : public DockablePane
	{
	private:
		HTMLBrowser * htmlBrowser;
		bool displayHelpUrls;
		String *lastUrl;
		String *lastTargetFrame;

		FocusChangedDelegate *FocusChangedHandler;
		WebBrowserLinkClickedDelegate *LinkClickedHandler;
		CloseProjectUIDelegate *CloseProjectHandler;

		void onFocusChanged(DisplayableDataNode *dataNode);
		void onBrowserLinkClicked(Object *sender, String *url);
		void onCloseProject();

	public:
		HelpPane();
		~HelpPane();

		HelpPaneLinkClickedDelegate *OnHelpPaneLinkClicked;

		void DisplayUrl(String *url, String *targetFrame);

		__property bool get_DisplayUrls() { return displayHelpUrls; }
		__property void set_DisplayUrls(bool value);
	};
}