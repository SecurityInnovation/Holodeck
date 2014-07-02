//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SelectedContainerListViewItemCollection.h
//
// DESCRIPTION: Contains definition for the class SelectedContainerListViewItemCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <system.dll>

#include "ContainerListViewItem.h"

using namespace System;
using namespace System::Collections;

namespace SIControls
{
	//*********************************************************************
	// Contains a set of selected list view items
	//*********************************************************************
	public __gc class SelectedContainerListViewItemCollection : public CollectionBase
	{
	public:
		__property ContainerListViewItem *get_Item(int index);
		__property void set_Item(int index, ContainerListViewItem *value);

		int IndexOf(ContainerListViewItem *item);
		int Add(ContainerListViewItem *item);
		void AddRange(ContainerListViewItem *items[]);
		void Remove(ContainerListViewItem *item);
		void ClearAll();
	};
}