//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PropertiesPane.h
//
// DESCRIPTION: Contains definition for the class PropertiesPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 11 Mar 2003		 B. Shirey		File created.
// 17 Mar 2003		 A. Kakrania	Added PropertyGrid control & EventHandler to catch FocusChange
//*************************************************************************
#pragma once

#include "DockablePane.h"

#using <mscorlib.dll>
#using <System.dll>
#using <MagicLibrary.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Threading;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents the dockable properties pane
	//*************************************************************************
	public __gc class PropertiesPane : public DockablePane
	{
	public:
		PropertiesPane();
		~PropertiesPane();
	private:
		PropertyGrid * propertyGrid;
		Threading::Thread * refreshGridThread; 
		void OnFocusChanged(DisplayableDataNode *dataNode);
		
		CloseProjectUIDelegate *CloseProjectUIHandler;
		void RefreshGridThreadFunc();
	};
}