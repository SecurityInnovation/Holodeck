//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FieldChooser.h
//
// DESCRIPTION: Contains definition for the class FieldChooser
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "Pane.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <SIControls.dll>

using namespace SIControls;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the dialog for choosing fields in a list view
	//*************************************************************************
	public __gc class FieldChooser : public Form
	{
	public:
		FieldChooser(ListView *lv, ArrayList *allColumnsHeaders);
		FieldChooser(TreeListView *lv, ArrayList *allColumnsHeaders);
		FieldChooser(ContainerListView *lv, ArrayList *allColumnsHeaders);
		~FieldChooser();

		__property ArrayList *get_SelectedColumns();

	private:
		ListView *listView;
		TreeListView *treeListView;
		ContainerListView *containerListView;
		ArrayList *allHeaders;
		CheckedListBox *fieldListBox;

		void Construct(ArrayList *allColumnsHeaders);
		void fieldListBox_ItemCheckEventHandler(Object * sender, ItemCheckEventArgs * e);
	};
}