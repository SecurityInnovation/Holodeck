//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		SIContextMenu.h
//
// DESCRIPTION: Contains definition for the class SIContextMenu
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 14 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#include "SIMenus.h"

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>
#using <SiIconLib.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Text;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;
using namespace Crownwood::Magic::Menus;

namespace SIControls
{
	//*********************************************************************
	// Represents a context menu
	//*********************************************************************
	public __gc class SIContextMenu : public PopupMenu
	{
	public:
		SIContextMenu();
		~SIContextMenu();

		int AddImageToList(String *fileName);
		int AddImageToList(Bitmap *image);

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

		ArrayList *groups;
		ImageList *imageList;
	};
}