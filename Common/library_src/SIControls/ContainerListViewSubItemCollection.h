//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewSubItemCollection.h
//
// DESCRIPTION: Contains definition for the class ContainerListViewSubItemCollection
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
#using <system.windows.forms.dll>

#include "ContainerListViewSubItem.h"
#include "Enums.h"
#include <stdio.h>

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Contains a set of subitems in a ContainerListViewItem
	//*********************************************************************
	public __gc class ContainerListViewSubItemCollection : public CollectionBase
	{
	public:
		ItemsChangedEventHandler *ItemsChanged;

		__property ContainerListViewSubItem *get_Item(int index);
		__property void set_Item(int index, ContainerListViewSubItem *value);

		int Add(ContainerListViewSubItem *item);
		ContainerListViewSubItem *Add(Control *control);
		ContainerListViewSubItem *Add(String *str);
		void AddRange(ContainerListViewSubItem *items[]);
	protected:
		void OnItemsChanged(ItemsChangedEventArgs *e);
	};
}