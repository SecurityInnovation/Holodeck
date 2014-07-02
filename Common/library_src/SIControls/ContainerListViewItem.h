//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewItem.h
//
// DESCRIPTION: Contains definition for the class ContainerListViewItem
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
#using <system.drawing.dll>

#include "ContainerListViewSubItemCollection.h"
#include "Enums.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Forward declaration to avoid compiler errors
	//*********************************************************************
	public __gc class ContainerListView;

	//*********************************************************************
	// Represents an item in a ContainerListView
	//*********************************************************************
	public __gc class ContainerListViewItem : public ICloneable
	{
	public:
		MouseEventHandler *MouseDown;

		ContainerListViewItem();
		ContainerListViewItem(ContainerListView *lv, int i);

		Object *Clone();
		ListViewItem *GetListViewItem();

		__property Color get_BackColor() { return backColor; }
		__property void set_BackColor(Color value) { backColor = value; }

		__property Color get_ForeColor() { return foreColor; }
		__property void set_ForeColor(Color value) { foreColor = value; }

		__property bool get_Checked() { return isChecked; }
		__property void set_Checked(bool value) { isChecked = value; }

		__property bool get_Focused() { return isFocused; }
		__property void set_Focused(bool value) { isFocused = value; }

		__property Font *get_ItemFont() { return font; }
		__property void set_ItemFont(Font *value) { font = value; }

		__property int get_ImageIndex() { return imageIndex; }
		__property void set_ImageIndex(int value) { imageIndex = value; }

		__property int get_Index() { return index; }
		__property void set_Index(int value) { index = value; }

		__property ContainerListView *get_ListView() { return listView; }

		__property bool get_Selected() { return isSelected; }
		__property void set_Selected(bool value) { isSelected = value; }

		__property ContainerListViewSubItemCollection *get_SubItems() { return subItems; }

		__property int get_StateImageIndex() { return stateImageIndex; }
		__property void set_StateImageIndex(int value) { stateImageIndex = value; }

		__property Object *get_Tag() { return tag; }
		__property void set_Tag(Object *value) { tag = value; }

		__property String *get_Text() { return text; }
		__property void set_Text(String *value) { text = value; }

		__property bool get_UseItemStyleForSubItems() { return styleAll; }
		__property void set_UseItemStyleForSubItems(bool value) { styleAll = value; }

		__property bool get_Hovered() { return isHovered; }
		__property void set_Hovered(bool value) { isHovered = value; }

	private:
		Color backColor;
		Color foreColor;
		
		bool isChecked;
		bool isFocused;
		bool isSelected;
		bool styleAll;
		bool isHovered;

		Font *font;
		
		int imageIndex;
		int stateImageIndex;
		int index;
		
		ContainerListView *listView;
		ContainerListViewSubItemCollection *subItems;
		Object *tag;
		String *text;

		void OnSubItemsChanged(Object *sender, ItemsChangedEventArgs *e);
		void OnSubItemMouseDown(Object *sender, MouseEventArgs *e);
	};
}