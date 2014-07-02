//*************************************************************************
// Copyright (C) Security Innovation Corp., 2002-2004 – All Rights Reserved.
//
// FILE:		HTMLBrowser.h
//
// DESCRIPTION: Contains definitions for the class HTMLBrowser
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 06 Aug 2004		 Ady K.		 File created.
//*************************************************************************
#pragma once

#include "UserNotification.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;
using namespace SHDocVw;
using namespace AxSHDocVw;
using namespace Microsoft::Win32;
using namespace System::IO;

namespace HolodeckGui
{
	public __delegate void WebBrowserTitleChangedDelegate(Object * sender, String * title);
	public __delegate void WebBrowserLinkClickedDelegate(Object * sender, String * url);

	//*************************************************************************
	// class that represents the return value selection page of the create test
	// wizard
	//*************************************************************************
	public __gc class HTMLBrowser
	{
	public:
		AxSHDocVw::AxWebBrowser * browser;

		HTMLBrowser();
		~HTMLBrowser();

		void Navigate (String * url);
		void Navigate (String * url, String * targetFrame);

		WebBrowserTitleChangedDelegate * OnTitleChanged;
		WebBrowserLinkClickedDelegate * OnLinkClicked;

		__property bool get_AllowNavigationInThisBrowser() { return allowNavigationInThisBrowser;}
		__property void set_AllowNavigationInThisBrowser(bool value) { allowNavigationInThisBrowser = value;}

	protected:
		bool allowNavigationInThisBrowser;
		String * targetFrame;
		String * currentUrl;

		void title_ChangedHandler(Object * sender, DWebBrowserEvents2_TitleChangeEvent * e);
		void before_NavigateHandler(Object * sender, DWebBrowserEvents2_BeforeNavigate2Event * e);
	};
}


