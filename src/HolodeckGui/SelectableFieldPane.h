//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SelectableFieldPane.h
//
// DESCRIPTION: Contains definition for the abstract class SelectableFieldPane.h
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "Pane.h"
#include "defines.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <SIControls.dll>

using namespace SIControls;
using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace Microsoft::Win32;

namespace HolodeckGui
{
	public __abstract __gc class SelectableFieldPane : public Pane
	{
	public:
		SelectableFieldPane();
		~SelectableFieldPane();

		[Browsable(false)]
		__property virtual int get_ProcessID() = 0;
		[Browsable(false)]
		__property virtual String *get_PaneName() = 0;

		[Browsable(false)]
		__property virtual ListView *get_PaneListView() { return NULL; };
		[Browsable(false)]
		__property virtual TreeListView *get_PaneTreeListView() { return NULL; };
		[Browsable(false)]
		__property virtual ContainerListView *get_PaneContainerListView() { return NULL; }

		[Browsable(false)]
		__property virtual ArrayList *get_AllColumns() = 0;

		[Browsable(false)]
		__property virtual ArrayList *get_VisibleColumns() = 0;
		__property virtual void set_VisibleColumns(ArrayList *headers) = 0;

		void SaveSettings(String *paneName);
		void LoadSettings(String *paneName);
	};
}