//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ServiceListItemSorter.cpp
//
// DESCRIPTION: Contains implementation for the class ServiceListItemSorter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 DEC 2003		 B. Shirey   File created.
//*************************************************************************
#include "ServiceListItemSorter.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ServiceListItemSorter
	// Description: Constructor for the ServiceListItemSorter class
	//
	// Parameters:
	//	sortColumn - the column to sort on
	//
	// Return Value: None
	//*************************************************************************
	ServiceListItemSorter::ServiceListItemSorter(int sortColumn)
	{
		ascending = true;
		columnToSort = sortColumn;
	}

	//*************************************************************************
	// Method:		Compare
	// Description: Compares 2 objects to determine how to sort them
	//
	// Parameters:
	//	a - the first object to compare
	//	b - the second object to compare
	//
	// Return Value: < 0 if b is greater than a, > 0 if a is greater than b
	//	or 0 if they are equal
	//*************************************************************************
	int ServiceListItemSorter::Compare(Object *a, Object *b)
	{
		ListViewItem *itemA = dynamic_cast<ListViewItem *>(a);
		ListViewItem *itemB = dynamic_cast<ListViewItem *>(b);

		if (!a || !b)
			return 0;

		CaseInsensitiveComparer *c = new CaseInsensitiveComparer();
		if (ascending)
			return c->Compare(itemA->SubItems->get_Item(columnToSort)->Text, itemB->SubItems->get_Item(columnToSort)->Text);
		return c->Compare(itemB->SubItems->get_Item(columnToSort)->Text, itemA->SubItems->get_Item(columnToSort)->Text);
	}
}