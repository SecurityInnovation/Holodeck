//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		NonDockablePane.h
//
// DESCRIPTION: Contains definition for the abstract class NonDockablePane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Jun 2004		 B. Shirey	 File created.
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

namespace SIControls
{
	//*************************************************************************
	// class that all non-dockable panes inherit from.  Provides some common
	// attributes to non-dockable panes
	//*************************************************************************
	public __gc class NonDockablePane : public Pane
	{
	public:
		NonDockablePane();
		~NonDockablePane();

		virtual void OnClosed();
	};
}