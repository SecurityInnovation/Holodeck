//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ServiceListItemSorter.h
//
// DESCRIPTION: Contains definition for the class ServiceListItemSorter
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
	// class that sorts the columns of the service list view in the new application
	// wizard page
	//*************************************************************************
	public __gc class ServiceListItemSorter : public IComparer
	{
	public:
		ServiceListItemSorter(int sortColumn);
		int Compare(Object *a, Object *b);

		__property bool get_Ascending() { return ascending; }
		__property void set_Ascending(bool value) { ascending = value; }

	protected:
		bool ascending;
		int columnToSort;
	};
}