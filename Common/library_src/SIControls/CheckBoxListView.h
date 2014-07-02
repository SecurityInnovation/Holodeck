//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		CheckBoxListView.h
//
// DESCRIPTION: Contains definition for the class CheckBoxListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 May 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*************************************************************************
	// class that works just like a normal list view except it contains check
	// boxes and handles ItemCheck and DoubleClick events properly (see 
	// http://oldlook.experts-exchange.com:8080/Programming/Programming_Languages/C_Sharp/Q_20711571.html
	// for details
	//*************************************************************************
	public __gc class CheckBoxListView : public System::Windows::Forms::ListView
	{
	public:
		CheckBoxListView();
		~CheckBoxListView();
	protected:
		void OnItemCheck(ItemCheckEventArgs *args);
		void OnMouseDown(MouseEventArgs *args);
		void OnKeyDown(KeyEventArgs *args);

		bool isDoubleClicking;
	};
}