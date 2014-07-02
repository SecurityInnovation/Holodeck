//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NonDockablePaneManager.h
//
// DESCRIPTION: Contains definition for the class NonDockablePaneManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "FaultPane.h"
#include "LogPane.h"
#include "ReportPane.h"
#include "ScheduledTestPane.h"
#include "NetworkCorruptionPane.h"
#include "ImageListManager.h"
#include "HelpContentsPane.h"
#include "ResourceTestPane.h"
#include <stdio.h>

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;

namespace HolodeckGui
{
	public __gc class NonDockablePaneManager : public Crownwood::Magic::Controls::TabControl
	{
	public:
		NonDockablePaneManager();
		~NonDockablePaneManager();

		void displayHelpPane(String *location);
		void addPage(Crownwood::Magic::Controls::TabPage *newPage);
	private:
		HelpContentsPane *helpPane;
		Crownwood::Magic::Controls::TabPage *helpTabPage;

		HelpContentsTitleChangedDelegate *helpTitleChangedHandler;

		void InitializeUI();
		void onCloseButtonPressed(Object *sender, EventArgs *args);
		void onSelectionChanged(Object *sender, EventArgs *args);
		void onHelpTitleChanged(String *text);
		String *DeconstructTitle(String *title);
	};
}