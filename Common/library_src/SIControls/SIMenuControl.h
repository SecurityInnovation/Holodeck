//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIMenuControl.h
//
// DESCRIPTION: Contains definition for the class SIMenuControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include "SIMenus.h"

#using <MSCorLib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;
using namespace Crownwood::Magic::Menus;
using namespace Crownwood::Magic::Collections;

namespace SIControls
{
	//*********************************************************************
	// A menu control that can be dynamically built by string names
	//*********************************************************************
	public __gc class SIMenuControl : public MenuControl
	{
	public:
		SIMenuControl();
		~SIMenuControl();

		int AddImageToList(String *fileName);
		int AddImageToList(Bitmap *image);

		void AddMenu(String *menuName, SIMenuItemPosition position, String *displayString, int imageIndex);
		void AddMenuGroup(String *groupName, SIMenuItemPosition position);
		void AddMenuItem(String *itemName, SIMenuItemPosition position, Shortcut shortcut,
						 String *displayString, int imageIndex, MenuItemQueryStateDelegate *stateQueryHandler,
						 MenuItemClickedDelegate *clickedHandler, Object *tag);

		void SetMenuOptionDisplayString(String *itemName, String *displayString);
		void SetMenuOptionTag(String *itemName, Object *tag);
		void SetMenuOptionImageIndex(String *itemName, int imageIndex);

		SIMenuItem *FindMenuItem(String *itemName);
		String *GetItemFullName(SIMenuItem *item);

		void Refresh();

		__property int get_EmptyImageIndex() { return 0; }

	private:
		bool ProcessMenuName(bool addIfNotFound, String *&menuName, SIMenuGroup *&group, SIMenuItemPosition groupPosition);
		void RefreshMenuGroup(SIMenuGroup *group, MenuCommand *&lastVisibleItem);
		void AddToItemsList(ArrayList *list, SIMenuItem *item);
		void AddToGroupsList(ArrayList *list, SIMenuGroup *group);
		void OnMenuItemClicked(Object *sender, EventArgs *args);
		int GetMenuItemIndex(MenuCommandCollection *items, MenuCommand *item);

		SIMenuGroup *menuItems;
		ImageList *imageList;
	};
}