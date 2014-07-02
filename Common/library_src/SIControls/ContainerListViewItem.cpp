//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewItem.cpp
//
// DESCRIPTION: Contains implemenation for the class ContainerListViewItem
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "ContainerListViewItem.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		ContainerListViewItem
	// Description: Constructor for the ContainerListViewItem class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewItem::ContainerListViewItem()
	{
		subItems = new ContainerListViewSubItemCollection();
		subItems->ItemsChanged += new ItemsChangedEventHandler(this, OnSubItemsChanged);
		backColor = Color::Empty;
		foreColor = Color::Empty;
	}

	//*************************************************************************
	// Method:		ContainerListViewItem
	// Description: Constructor for the ContainerListViewItem class
	//
	// Parameters:
	//	lv - the ContainerListView that owns this item
	//	i - the index in the list view this item should be located at
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewItem::ContainerListViewItem(ContainerListView *lv, int i)
	{
		index = i;
		listView = lv;
		backColor = Color::Empty;
		foreColor = Color::Empty;
	}

	//*************************************************************************
	// Method:		OnSubItemsChanged
	// Description: Updates some properties of the subitems when they have changed
	//
	// Parameters:
	//	sender - the sender of this event
	//	e - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewItem::OnSubItemsChanged(Object *sender, ItemsChangedEventArgs *e)
	{
		subItems->get_Item(e->IndexChanged)->MouseDown += new MouseEventHandler(this, OnSubItemMouseDown);
	}

	//*************************************************************************
	// Method:		OnSubItemMouseDown
	// Description: Called when the mouse is clicked on a subitem control
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewItem::OnSubItemMouseDown(Object *sender, MouseEventArgs *e)
	{
		if (MouseDown)
			MouseDown->Invoke(this, e);
	}

	//*************************************************************************
	// Method:		Clone
	// Description: Copies this item into another item
	//
	// Parameters:
	//	None
	//
	// Return Value: A copy of this item
	//*************************************************************************
	Object *ContainerListViewItem::Clone()
	{
		ContainerListViewItem *lvi = new ContainerListViewItem();
		lvi->BackColor = backColor;
		lvi->Focused = isFocused;
		lvi->ItemFont = font;
		lvi->ForeColor = foreColor;
		lvi->ImageIndex = imageIndex;
		lvi->Selected = isSelected;
		lvi->Tag = tag;
		lvi->Text = text;
		lvi->UseItemStyleForSubItems = styleAll;

		return lvi;
	}

	//*************************************************************************
	// Method:		GetListViewItem
	// Description: Converts this ContainerListViewItem to a ListViewItem type
	//				(Only text is copied)
	//
	// Parameters:
	//	None
	//
	// Return Value: A ListView type copy of this item
	//*************************************************************************
	ListViewItem *ContainerListViewItem::GetListViewItem()
	{
		ListViewItem * lvi = new ListViewItem(this->Text);
		
		if (this->subItems != NULL)
		{
			for (int i=0; i < this->subItems->Count; i++)
			{
				ContainerListViewSubItem * clvsi = this->subItems->Item[i];
				lvi->SubItems->Add (clvsi->Text);
			}
		}
		
		return lvi;
	}
}