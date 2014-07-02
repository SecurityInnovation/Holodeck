//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIMenuControl.cpp
//
// DESCRIPTION: Contains implementation for the class SIMenuControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#include "SIMenuControl.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SIMenuControl
	// Description: Constructor for the SIMenuControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIMenuControl::SIMenuControl()
	{
		menuItems = new SIMenuGroup;
		menuItems->menuItems = new ArrayList();

		imageList = new ImageList();
		// add an empty item
		imageList->Images->Add(new System::Drawing::Bitmap(1, 1));
	}

	//*************************************************************************
	// Method:		~SIMenuControl
	// Description: Destructor for the SIMenuControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIMenuControl::~SIMenuControl()
	{
		menuItems = NULL;
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the treeview's image list
	//
	// Parameters:
	//	fileName - the name of the file that holds the image
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int SIMenuControl::AddImageToList(String *fileName)
	{
		try
		{
			Bitmap *bitmap = new Bitmap(fileName);
			return imageList->Images->Add(bitmap, Color::Empty);
		}
		catch(...)
		{
		}

		return -1;
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the treeview's image list
	//
	// Parameters:
	//	image - image to add
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int SIMenuControl::AddImageToList(Bitmap *image)
	{
		return imageList->Images->Add(image, Color::Empty);
	}

	//*************************************************************************
	// Method:		AddMenu
	// Description: Adds a menu to the menu control
	//
	// Parameters:
	//	menuName - the internal name for the menu
	//	position - the position to add the menu to
	//	displayString - the string to display as the menu text
	//  imageIndex - index of menu icon
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::AddMenu(String *menuName, SIMenuItemPosition position, String *displayString, int imageIndex)
	{
		SIMenuGroup *group;

		ProcessMenuName(true, menuName, group, SIMenuItemPosition::Normal);

		// try to find an existing item by this name
		for (int i = 0; i < group->menuItems->Count; i++)
		{
			SIMenuItem* item = dynamic_cast<SIMenuItem*>(group->menuItems->Item[i]);
			if (item->name->Equals(menuName))
			{
				// Found, update info and exit
				item->displayName = displayString;
				item->position = position;
				item->imageIndex = imageIndex;
				return;
			}
		}

		// No menu by this name, add it to the list
		SIMenuItem* item = new SIMenuItem;
		item->name = menuName;
		item->displayName = displayString;
		item->subMenu = true;
		item->position = position;
		item->imageIndex = imageIndex;
		item->groups = new ArrayList();
		AddToItemsList(group->menuItems, item);
	}

	//*************************************************************************
	// Method:		AddMenuGroup
	// Description: Adds a menu group to the control
	//
	// Parameters:
	//	groupName - the name of the group to add
	//	position - the relative position to add the group
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::AddMenuGroup(String *groupName, SIMenuItemPosition position)
	{
		SIMenuGroup* group;

		groupName = String::Concat(groupName, S"->");

		// Call ProcessMenuName to force group creation with correct position info
		ProcessMenuName(true, groupName, group, position);
	}

	//*************************************************************************
	// Method:		AddMenuItem
	// Description: Adds a menu item to the control
	//
	// Parameters:
	//	itemName - the internal name for the item
	//	position - the relative position to add the item in the group
	//	shortcut - the shortcut to assign to the item
	//	displayString - the text to display as the menu item text
	//  imageIndex - index of menu item icon
	//	stateQueryHandler - the handler to get the state of the menu item
	//	clickedHandler - the delegate to invoke when the item is clicked
	//	tag - any user defined object you want
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::AddMenuItem(String *itemName, SIMenuItemPosition position, Shortcut shortcut,
						String *displayString, int imageIndex, MenuItemQueryStateDelegate *stateQueryHandler,
						MenuItemClickedDelegate *clickedHandler, Object *tag)
	{
		SIMenuGroup* group;

		ProcessMenuName(true, itemName, group, SIMenuItemPosition::Normal);

		// Try to find an existing item by this name
		for (int i = 0; i < group->menuItems->Count; i++)
		{
			SIMenuItem* item = dynamic_cast<SIMenuItem*>(group->menuItems->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, update info and exit
				item->displayName = displayString;
				item->position = position;
				item->imageIndex = imageIndex;
				item->shortcut = shortcut;
				item->OnQueryState = stateQueryHandler;
				item->OnClick = clickedHandler;
				item->tag = tag;
				return;
			}
		}

		// No menu by this name, add it to the list
		SIMenuItem* item = new SIMenuItem;
		item->name = itemName;
		item->displayName = displayString;
		item->subMenu = false;
		item->position = position;
		item->imageIndex = imageIndex;
		item->shortcut = shortcut;
		item->OnQueryState = stateQueryHandler;
		item->OnClick = clickedHandler;
		item->tag = tag;
		item->groups = new ArrayList();
		AddToItemsList(group->menuItems, item);

		Refresh();
	}

	//*************************************************************************
	// Method:		SetMenuOptionDisplayString
	// Description: changes the display string for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the display string for
	//	displayString - the string to set as the displayed menu item text
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::SetMenuOptionDisplayString(String *itemName, String *displayString)
	{
		SIMenuItem* item = FindMenuItem(itemName);
		if (item)
		{
			item->displayName = displayString;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetMenuOptionTag
	// Description: changes the user-defined tag object for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	tag - the user-defined object to assign to the menu item
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::SetMenuOptionTag(String *itemName, Object *tag)
	{
		SIMenuItem* item = FindMenuItem(itemName);
		if (item)
		{
			item->tag = tag;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetMenuOptionImageIndex
	// Description: changes the image for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	imageIndex - the image to assign to the menu item
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::SetMenuOptionImageIndex(String *itemName, int imageIndex)
	{
		SIMenuItem* item = FindMenuItem(itemName);
		if (item)
		{
			item->imageIndex = imageIndex;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		FindMenuItem
	// Description: returns an item with the specified name
	//
	// Parameters:
	//	itemName - the name of the item to find
	//
	// Return Value: the found item, or NULL if not found
	//*************************************************************************
	SIMenuItem *SIMenuControl::FindMenuItem(String *itemName)
	{
		SIMenuGroup* group;

		if (!ProcessMenuName(false, itemName, group, SIMenuItemPosition::Normal))
			return NULL;

		// Try to find an existing item by this name
		for (int i = 0; i < group->menuItems->Count; i++)
		{
			SIMenuItem* item = dynamic_cast<SIMenuItem*>(group->menuItems->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, return to caller
				return item;
			}
		}

		return NULL;
	}
	
	//*************************************************************************
	// Method:		GetItemFullName
	// Description: gets the full name of a specified item
	//
	// Parameters:
	//	item - the item to get the name of
	//
	// Return Value: the full name of the specified item
	//*************************************************************************
	String *SIMenuControl::GetItemFullName(SIMenuItem *item)
	{
		String* name = item->name;

		if (item->parent && item->parent->parent)
		{
			String* groupName = item->parent->name;
			String* parentName = GetItemFullName(item->parent->parent);
			if (groupName->Equals("None"))
				name = String::Concat(parentName, S"->", name);
			else
				name = String::Concat(parentName, S"[", groupName, S"]->", name);
		}

		return name;
	}

	//*************************************************************************
	// Method:		Refresh
	// Description: Updates the states of all items in the menu control
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::Refresh()
	{
		MenuCommand* lastVisible = NULL;
		RefreshMenuGroup(menuItems, lastVisible);
	}

	//*************************************************************************
	// Method:		ProcessMenuName
	// Description: parses a menu name and processes it
	//
	// Parameters:
	//	addIfNotFound - if true and the name is not found, an item will be added
	//	menuName - the name of the menu item to process
	//	group - the group of the menu item to process
	//	position - the relative position of the item to process
	//
	// Return Value: None
	//*************************************************************************
	bool SIMenuControl::ProcessMenuName(bool addIfNotFound, String *&menuName, SIMenuGroup *&group, SIMenuItemPosition groupPosition)
	{
		group = menuItems;

		while (menuName->IndexOf("->") != -1)
		{
			String* subName = menuName->Substring(0, menuName->IndexOf("->"));
			menuName = menuName->Substring(menuName->IndexOf("->") + 2);

			String* groupName = "None";
			if (subName->IndexOf("[") != -1)
			{
				groupName = subName->Substring(subName->IndexOf("[") + 1);
				if (groupName->IndexOf("]") != -1)
					groupName = groupName->Substring(0, groupName->IndexOf("]"));
				subName = subName->Substring(0, subName->IndexOf("["));
			}

			bool found = false;
			SIMenuItem* parent;
			for (int i = 0; i < group->menuItems->Count; i++)
			{
				SIMenuItem* item = dynamic_cast<SIMenuItem*>(group->menuItems->Item[i]);
				if (item->name->Equals(subName))
				{
					parent = item;
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (!addIfNotFound)
					return false;

				SIMenuItem* item = new SIMenuItem;
				item->name = subName;
				item->displayName = subName;
				item->position = SIMenuItemPosition::Normal;
				item->imageIndex = -1;
				item->subMenu = true;
				item->parent = group;
				item->groups = new ArrayList();
				parent = item;
				AddToItemsList(group->menuItems, item);
			}

			found = false;
			for (int i = 0; i < parent->groups->Count; i++)
			{
				SIMenuGroup* cur = dynamic_cast<SIMenuGroup*>(parent->groups->Item[i]);
				if (cur->name->Equals(groupName))
				{
					group = cur;
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (!addIfNotFound)
					return false;

				SIMenuGroup* newGroup = new SIMenuGroup;
				newGroup->name = groupName;
				newGroup->position = groupPosition;
				newGroup->menuItems = new ArrayList();
				newGroup->parent = parent;
				group = newGroup;
				AddToGroupsList(parent->groups, newGroup);
			}
		}

		return true;
	}

	//*************************************************************************
	// Method:		RefreshMenuGroup
	// Description: refreshes the state of all items in a menu group
	//
	// Parameters:
	//	group - the group to refresh
	//	lastVisibleItem - the last visible item refreshed
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::RefreshMenuGroup(SIMenuGroup *group, MenuCommand *&lastVisibleItem)
	{
		int visibleCount = 0;

		for (int i = 0; i < group->menuItems->Count; i++)
		{
			SIMenuItem* item = dynamic_cast<SIMenuItem*>(group->menuItems->Item[i]);

			SIMenuItemState state = SIMenuItemState::Enabled;
			if (item->OnQueryState)
				state = item->OnQueryState->Invoke(GetItemFullName(item), item->tag);

			if (item->menuItem == NULL)
			{
				MenuCommand* command = new MenuCommand();
				command->Text = item->displayName;
				command->Shortcut = item->shortcut;
				command->Tag = item;
				command->ImageList = imageList;
				command->ImageIndex = item->imageIndex;
				command->Click += new EventHandler(this, OnMenuItemClicked);
				item->menuItem = command;
				item->itemInserted = false;
			}

			if (item->groups->Count > 0)
			{
				MenuCommand* lastGroupVisible = NULL;
				for (int j = 0; j < item->groups->Count; j++)
				{
					SIMenuGroup* cur = dynamic_cast<SIMenuGroup*>(item->groups->Item[j]);
					cur->parentMenuItem = item->menuItem;
					for (int k = 0; k < cur->menuItems->Count; k++)
					{
						SIMenuItem* subItem = dynamic_cast<SIMenuItem*>(cur->menuItems->Item[k]);
						subItem->parentMenuItem = item->menuItem;
					}
					RefreshMenuGroup(cur, lastGroupVisible);
					if (cur->separatorInserted)
						lastGroupVisible = cur->separatorMenuItem;
				}

				// Go back through groups and add seperators if needed
				for (int j = 0; j < item->groups->Count; j++)
				{
					SIMenuGroup* cur = dynamic_cast<SIMenuGroup*>(item->groups->Item[j]);
					SIMenuItem* lastVisible = NULL;

					bool showBreak = false;
					if (cur->visibleCount > 0)
					{
						for (int k = j + 1; k < item->groups->Count; k++)
						{
							SIMenuGroup* next = dynamic_cast<SIMenuGroup*>(item->groups->Item[k]);
							if (next->visibleCount > 0)
								showBreak = true;
						}

						for (int k = 0; k < cur->menuItems->Count; k++)
						{
							SIMenuItem* curItem = dynamic_cast<SIMenuItem*>(cur->menuItems->Item[k]);
							if (curItem->itemInserted)
								lastVisible = curItem;
						}
					}

					if (showBreak)
					{
						if (cur->separatorMenuItem == NULL)
							cur->separatorMenuItem = new MenuCommand("-");

						if (!cur->separatorInserted)
						{
							if (cur->parentMenuItem)
							{
								cur->parentMenuItem->MenuCommands->Insert(GetMenuItemIndex(
									cur->parentMenuItem->MenuCommands, lastVisible->menuItem) + 1,
									cur->separatorMenuItem);
							}
							else
							{
								this->MenuCommands->Insert(GetMenuItemIndex(this->MenuCommands,
									lastVisible->menuItem) + 1, cur->separatorMenuItem);
							}
							cur->separatorInserted = true;
						}
					}
					else
					{
						if (cur->separatorInserted)
						{
							if (cur->parentMenuItem)
								cur->parentMenuItem->MenuCommands->Remove(cur->separatorMenuItem);
							else
								this->MenuCommands->Remove(cur->separatorMenuItem);
							cur->separatorInserted = false;
						}
					}
				}
			}

			if (item->subMenu)
			{
				int totalVisible = 0;
				for (int j = 0; j < item->groups->Count; j++)
				{
					SIMenuGroup* cur = dynamic_cast<SIMenuGroup*>(item->groups->Item[j]);
					totalVisible += cur->visibleCount;
				}

				if (totalVisible == 0)
					state = SIMenuItemState::Invisible;
			}

			if (state == SIMenuItemState::Invisible)
			{
				if (item->itemInserted)
				{
					if (item->parentMenuItem != NULL)
						item->parentMenuItem->MenuCommands->Remove(item->menuItem);
					else
						this->MenuCommands->Remove(item->menuItem);
					item->itemInserted = false;
				}
			}
			else
			{
				visibleCount++;

				if (!item->itemInserted)
				{
					if (item->parentMenuItem)
					{
						if (lastVisibleItem == NULL)
							item->parentMenuItem->MenuCommands->Insert(0, item->menuItem);
						else
						{
							item->parentMenuItem->MenuCommands->Insert(GetMenuItemIndex(
								item->parentMenuItem->MenuCommands, lastVisibleItem) + 1,
								item->menuItem);
						}
					}
					else
					{
						if (lastVisibleItem == NULL)
							this->MenuCommands->Insert(0, item->menuItem);
						else
						{
							this->MenuCommands->Insert(GetMenuItemIndex(this->MenuCommands,
								lastVisibleItem) + 1, item->menuItem);
						}
					}
					item->itemInserted = true;
				}

				if (!item->menuItem->Text->Equals(item->displayName))
					item->menuItem->Text = item->displayName;
				if (item->menuItem->Shortcut != item->shortcut)
					item->menuItem->Shortcut = item->shortcut;
				if (item->menuItem->ImageIndex != item->imageIndex)
					item->menuItem->ImageIndex = item->imageIndex;

				switch (state)
				{
				case SIMenuItemState::Disabled:
					if (item->menuItem->Enabled)
						item->menuItem->Enabled = false;
					break;
				case SIMenuItemState::Enabled:
					if (!item->menuItem->Enabled)
						item->menuItem->Enabled = true;
					if (item->menuItem->Checked)
						item->menuItem->Checked = false;
					break;
				case SIMenuItemState::Checked:
					if (!item->menuItem->Enabled)
						item->menuItem->Enabled = true;
					if (!item->menuItem->Checked)
						item->menuItem->Checked = true;
					break;
				}

				lastVisibleItem = item->menuItem;
			}
		}

		group->visibleCount = visibleCount;
	}

	//*************************************************************************
	// Method:		AddToItemsList
	// Description: Adds a menu item to an internal list
	//
	// Parameters:
	//	list - the list to add the item to
	//	item - the item to add to the list
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::AddToItemsList(ArrayList *list, SIMenuItem *item)
	{
		int insertPos;
		for (insertPos = 0; insertPos < list->Count; insertPos++)
		{
			SIMenuItem* curItem = dynamic_cast<SIMenuItem*>(list->Item[insertPos]);
			bool insertNow = false;
			switch (item->position)
			{
			case SIMenuItemPosition::First:
				// Insert before all "normal" and "last" items
				if ((curItem->position == SIMenuItemPosition::Normal) || (curItem->position == SIMenuItemPosition::Last))
					insertNow = true;
				break;
			case SIMenuItemPosition::Normal:
				// Insert before all "last" items
				if (curItem->position == SIMenuItemPosition::Last)
					insertNow = true;
				break;
			case SIMenuItemPosition::Last:
				// Always at end, never break out early
				break;
			}
			if (insertNow)
				break;
		}
		list->Insert(insertPos, item);
	}

	//*************************************************************************
	// Method:		AddToGroupsList
	// Description: Adds a group to an internal list
	//
	// Parameters:
	//	list - the list to add the group to
	//	group - the group to add to the list
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::AddToGroupsList(ArrayList *list, SIMenuGroup *group)
	{
		int insertPos;
		for (insertPos = 0; insertPos < list->Count; insertPos++)
		{
			SIMenuGroup* curGroup = dynamic_cast<SIMenuGroup*>(list->Item[insertPos]);
			bool insertNow = false;
			switch (group->position)
			{
			case SIMenuItemPosition::First:
				// Insert before all "normal" and "last" groups
				if ((curGroup->position == SIMenuItemPosition::Normal) || (curGroup->position == SIMenuItemPosition::Last))
					insertNow = true;
				break;
			case SIMenuItemPosition::Normal:
				// Insert before all "last" groups
				if (curGroup->position == SIMenuItemPosition::Last)
					insertNow = true;
				break;
			case SIMenuItemPosition::Last:
				// Always at end, never break out early
				break;
			}
			if (insertNow)
				break;
		}
		list->Insert(insertPos, group);
	}

	//*************************************************************************
	// Method:		OnMenuItemClicked
	// Description: Called when a menu item is clicked.  Invokes the clicked
	//	delegate for the clicked menu item if applicable
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void SIMenuControl::OnMenuItemClicked(Object *sender, EventArgs *args)
	{
		MenuCommand* command = dynamic_cast<MenuCommand*>(sender);
		if (command == NULL)
			return;

		SIMenuItem* item = dynamic_cast<SIMenuItem*>(command->Tag);
		if (item == NULL)
			return;

		if (item->OnClick == NULL)
			return;

		item->OnClick->Invoke(GetItemFullName(item), item->tag);
	}

	//*************************************************************************
	// Method:		GetMenuItemIndex
	// Description: Gets the index of a menu item in a collection
	//
	// Parameters:
	//	items - the group to find the item in
	//	item - the item to get the index of
	//
	// Return Value: the index of the found item, or -1 if not found
	//*************************************************************************
	int SIMenuControl::GetMenuItemIndex(MenuCommandCollection *items, MenuCommand *item)
	{
		for (int i = 0; i < items->Count; i++)
		{
			MenuCommand* cur = dynamic_cast<MenuCommand*>(items->Item[i]);
			if (cur == item)
				return i;
		}
		return -1;
	}
}