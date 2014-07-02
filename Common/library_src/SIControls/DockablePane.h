//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DockablePane.h
//
// DESCRIPTION: Contains definition for the abstract class DockablePane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "Pane.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all dockable panes inherit from.  Provides some common
	// attributes to dockable panes
	//*************************************************************************
	public __abstract __gc class DockablePane : public Pane
	{
	public:
		DockablePane();
		~DockablePane();
	};
}