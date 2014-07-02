//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HelpContentsPane.h
//
// DESCRIPTION: Contains definition for the class HelpContentsPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "Pane.h"
#include "HTMLBrowser.h"

#include <stdio.h>

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

namespace HolodeckGui
{
	__delegate void HelpContentsTitleChangedDelegate(String *text);

	//*************************************************************************
	// class to hold the contents of a help topic
	//*************************************************************************
	public __gc class HelpContentsPane : public Pane
	{
	public:
		HelpContentsPane();
		~HelpContentsPane();

		void showContents(String *location);

		HelpContentsTitleChangedDelegate *OnTitleChanged;

	private:
		HTMLBrowser * htmlBrowser;

		WebBrowserTitleChangedDelegate *browserTitleChangedHandler;

		void onTitleChanged(Object *sender, String *text);
	};
}