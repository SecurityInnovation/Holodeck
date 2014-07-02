//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Pane.h
//
// DESCRIPTION: Contains definition for the abstract class Pane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include "UIEventRegistry.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all panes inherit from.  Provides some common
	// attributes to panes
	//*************************************************************************
	public __abstract __gc class Pane : public Panel
	{
	public:
		Pane();
		~Pane();

	protected:
		UIEventRegistry *uiEventRegistry;
	};
}