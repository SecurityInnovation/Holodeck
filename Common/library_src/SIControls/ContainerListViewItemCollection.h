//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewItemCollection.h
//
// DESCRIPTION: Contains definition for the class ContainerListViewItemCollection
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

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Forward declaration to avoid compiler errors
	//*********************************************************************
	public __gc class ContainerListViewItem;

	public __delegate ContainerListViewItem *QueryContainerListViewItemDelegate(unsigned int index);
	public __delegate unsigned int QueryContainerListViewItemIndexDelegate(ContainerListViewItem *item);

	//*********************************************************************
	// Contains a set of ContainerListViewItems
	//*********************************************************************
	public __gc class ContainerListViewItemCollection : public CollectionBase
	{
	public:
		MouseEventHandler *MouseDown;
		QueryContainerListViewItemDelegate *QueryListViewItem;
		QueryContainerListViewItemIndexDelegate *QueryListViewItemIndex;

		ContainerListViewItemCollection();

		__property ContainerListViewItem *get_Item(int index);

		__property unsigned int get_ItemCount() { return itemCount; }
		__property void set_ItemCount(unsigned int value) { itemCount = value; }

		int IndexOf(ContainerListViewItem *item);

	private:
		void OnMouseDown(Object *sender, MouseEventArgs *e);

		unsigned int itemCount;
	};
}