//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProcessListItemSorter.h
//
// DESCRIPTION: Contains definition for the class ProcessListItemSorter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 DEC 2003		 B. Shirey   File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that sorts the columns of the process list view in the new application
	// wizard page
	//*************************************************************************
	public __gc class ProcessListItemSorter : public IComparer
	{
	public:
		ProcessListItemSorter(int sortColumn);
		int Compare(Object *a, Object *b);

		__property bool get_Ascending() { return ascending; }
		__property void set_Ascending(bool value) { ascending = value; }

	protected:
		int ComparePID(Object *a, Object *b);
		int CompareProcess(Object *a, Object *b);

		bool ascending;
		int columnToSort;
	};
}