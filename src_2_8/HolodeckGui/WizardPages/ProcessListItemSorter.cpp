//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProcessListItemSorter.cpp
//
// DESCRIPTION: Contains implementation for the class ProcessListItemSorter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 03 DEC 2003		 B. Shirey   File created.
//*************************************************************************
#include "ProcessListItemSorter.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ProcessListItemSorter
	// Description: Constructor for the ProcessListItemSorter class
	//
	// Parameters:
	//	sortColumn - the column to sort on
	//
	// Return Value: None
	//*************************************************************************
	ProcessListItemSorter::ProcessListItemSorter(int sortColumn)
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
	int ProcessListItemSorter::Compare(Object *a, Object *b)
	{
		ListViewItem *itemA = dynamic_cast<ListViewItem *>(a);
		ListViewItem *itemB = dynamic_cast<ListViewItem *>(b);

		if (!a || !b)
			return 0;

		switch(columnToSort)
		{
		case 0:
			return CompareProcess(itemA->SubItems->get_Item(0)->Text, itemB->SubItems->get_Item(0)->Text);
		case 1:
			return ComparePID(itemA->SubItems->get_Item(1)->Text, itemB->SubItems->get_Item(1)->Text);
		default:
			CaseInsensitiveComparer *c = new CaseInsensitiveComparer();
			return c->Compare(a, b);
		}
	}

	//*************************************************************************
	// Method:		ComparePID
	// Description: Compares 2 pid objects to determine how to sort them
	//
	// Parameters:
	//	a - the first object to compare
	//	b - the second object to compare
	//
	// Return Value: < 0 if b is greater than a, > 0 if a is greater than b
	//	or 0 if they are equal
	//*************************************************************************
	int ProcessListItemSorter::ComparePID(Object *a, Object *b)
	{
		int aInt = Int32::Parse(a->ToString());
		int bInt = Int32::Parse(b->ToString());

		if (ascending)
			return aInt - bInt;
		return bInt - aInt;
	}

	//*************************************************************************
	// Method:		CompareProcess
	// Description: Compares 2 process objects to determine how to sort them
	//
	// Parameters:
	//	a - the first object to compare
	//	b - the second object to compare
	//
	// Return Value: < 0 if b is greater than a, > 0 if a is greater than b
	//	or 0 if they are equal
	//*************************************************************************
	int ProcessListItemSorter::CompareProcess(Object *a, Object *b)
	{
		CaseInsensitiveComparer *c = new CaseInsensitiveComparer();

		if (ascending)
			return c->Compare(a, b);
		return c->Compare(b, a);
	}
}