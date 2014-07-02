//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIMenus.h
//
// DESCRIPTION: Contains definitions for classes common to SIMenu classes
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 14 Jul 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>

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
	// forward declaration so we compile ok
	public __gc struct SIMenuItem;

	// possible menu item states
	public __value enum SIMenuItemState
	{
		Invisible,
		Disabled,
		Enabled,
		Checked
	};

	// possible menu item positions
	public __value enum SIMenuItemPosition
	{
		First,
		Normal,
		Last
	};

	// some delegates used by the menu items
	public __delegate SIMenuItemState MenuItemQueryStateDelegate(String *itemName, Object *tag);
	public __delegate void MenuItemClickedDelegate(String *itemName, Object *tag);

	// a group of menu items
	public __gc struct SIMenuGroup
	{
		SIMenuItem *parent;
		String *name;
		SIMenuItemPosition position;
		ArrayList *menuItems;
		bool separatorInserted;
		int visibleCount;
		MenuCommand *separatorMenuItem;
		MenuCommand *parentMenuItem;
	};

	// a single menu item
	public __gc struct SIMenuItem
	{
		SIMenuGroup *parent;
		String *name;
		String *displayName;
		bool subMenu;
		bool itemInserted;
		SIMenuItemPosition position;
		int imageIndex;
		Shortcut shortcut;
		Object *tag;
		ArrayList *groups;
		MenuCommand *menuItem;
		MenuCommand *parentMenuItem;
		MenuItemQueryStateDelegate *OnQueryState;
		MenuItemClickedDelegate *OnClick;
	};
}