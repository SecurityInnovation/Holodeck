//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		SIContextMenu.cpp
//
// DESCRIPTION: Contains implementation for the class SIContextMenu
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 14 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#include <stdio.h>
#include "SIContextMenu.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SIContextMenu
	// Description: Constructor for the SIContextMenu class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIContextMenu::SIContextMenu()
	{
		groups = new ArrayList();

		imageList = new ImageList();
		// add an empty item
		imageList->Images->Add(new System::Drawing::Bitmap(1, 1));
	}
	
	//*************************************************************************
	// Method:		~SIContextMenu
	// Description: Destructor for the SIContextMenu class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIContextMenu::~SIContextMenu()
	{
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
	int SIContextMenu::AddImageToList(String *fileName)
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
	int SIContextMenu::AddImageToList(Bitmap *image)
	{
		return imageList->Images->Add(image, Color::Empty);
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
	void SIContextMenu::AddMenuGroup(String *groupName, SIMenuItemPosition position)
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
	//	stateQueryHandler - the handler to get the state of the menu item
	//	clickedHandler - the delegate to invoke when the item is clicked
	//	tag - any user defined object you want
	//
	// Return Value: None
	//*************************************************************************
	void SIContextMenu::AddMenuItem(String *itemName, SIMenuItemPosition position, Shortcut shortcut,
						String *displayString, int imageIndex, MenuItemQueryStateDelegate *stateQueryHandler,
						MenuItemClickedDelegate *clickedHandler, Object *tag)
	{
		SIMenuGroup* group;

		ProcessMenuName(true, itemName, group, SIMenuItemPosition::Normal);
		if (!group)
			return;

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
	void SIContextMenu::SetMenuOptionDisplayString(String *itemName, String *displayString)
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
	void SIContextMenu::SetMenuOptionTag(String *itemName, Object *tag)
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
	void SIContextMenu::SetMenuOptionImageIndex(String *itemName, int imageIndex)
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
	SIMenuItem *SIContextMenu::FindMenuItem(String *itemName)
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
	String *SIContextMenu::GetItemFullName(SIMenuItem *item)
	{
		String* name = item->name;

		if (item->parent)
		{
			String* groupName = item->parent->name;
			name = String::Concat(groupName, S"->", name);
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
	void SIContextMenu::Refresh()
	{
		MenuCommand* lastVisible = NULL;

		// refresh each group
		for (int i = 0; i < groups->Count; i++)
		{
			SIMenuGroup *group = dynamic_cast<SIMenuGroup *>(groups->get_Item(i));
			if (!group)
				continue;

			RefreshMenuGroup(group, lastVisible);
		}

		for (int i = 0; i < groups->Count; i++)
		{
			SIMenuGroup *curr = dynamic_cast<SIMenuGroup *>(groups->get_Item(i));
			if (curr->visibleCount == 0)
				continue;

			bool needSeparator = false;
			for (int j = i + 1; j < groups->Count; j++)
			{
				SIMenuGroup *other = dynamic_cast<SIMenuGroup *>(groups->get_Item(j));
				if (other->visibleCount == 0)
					continue;

				// found non-empty group, need a separator
				needSeparator = true;
				break;
			}

			if (needSeparator && !curr->separatorInserted)
			{
				// separator required but not yet visible
				if (!curr->separatorMenuItem)
					curr->separatorMenuItem = new MenuCommand("-");

				lastVisible = NULL;
				for (int j = 0; j < curr->menuItems->Count; j++)
				{
					SIMenuItem *item = dynamic_cast<SIMenuItem *>(curr->menuItems->get_Item(j));
					if (item && item->itemInserted)
						lastVisible = item->menuItem;
				}

				if (lastVisible)
				{
					MenuCommands->Insert(MenuCommands->IndexOf(lastVisible) + 1, curr->separatorMenuItem);
					curr->separatorInserted = true;
				}
			}
			else if (!needSeparator && curr->separatorInserted)
			{
				MenuCommands->Remove(curr->separatorMenuItem);
				curr->separatorInserted = false;
			}
		}
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
	bool SIContextMenu::ProcessMenuName(bool addIfNotFound, String *&menuName, SIMenuGroup *&group, SIMenuItemPosition groupPosition)
	{
		String *groupName = "None";
		ArrayList *currGroups = groups;

		ArrayList *splitStringList = new ArrayList();
		while (menuName->IndexOf("->") != -1)
		{
			String *leftName = menuName->Substring(0, menuName->IndexOf("->"));
			menuName = menuName->Substring(menuName->IndexOf("->") + 2);
			splitStringList->Add(leftName);
		}
		if (!menuName->Trim()->Equals(""))
			splitStringList->Add(menuName);

		while (splitStringList->Count > 0)
		{
			groupName = splitStringList->get_Item(0)->ToString();
			splitStringList->RemoveAt(0);

			// find the parent group
			bool found = false;
			SIMenuGroup *parentGroup = NULL;
			for (int i = 0; i < currGroups->Count; i++)
			{
				SIMenuGroup* currGroup = dynamic_cast<SIMenuGroup*>(currGroups->Item[i]);
				if (currGroup->name->Equals(groupName))
				{
					parentGroup = currGroup;
					found = true;
					break;
				}
			}

			// we can't find the parent group, do we want to add it?
			if (!found)
			{
				if (!addIfNotFound)
					return false;

				SIMenuGroup* newGroup = new SIMenuGroup;
				newGroup->name = groupName;
				newGroup->position = groupPosition;
				newGroup->menuItems = new ArrayList();
				newGroup->parent = NULL;
				parentGroup = newGroup;
				AddToGroupsList(currGroups, parentGroup);
			}

			group = parentGroup;

			if (splitStringList->Count == 0)
				return true;

			menuName = splitStringList->get_Item(0)->ToString();
			splitStringList->RemoveAt(0);

			found = false;
			for (int i = 0; i < parentGroup->menuItems->Count; i++)
			{
				SIMenuItem *curr = dynamic_cast<SIMenuItem *>(parentGroup->menuItems->get_Item(i));
				if (!curr || !curr->name)
					continue;

				if (curr->name->Equals(menuName))
				{
					found = true;
					currGroups = curr->groups;
					break;
				}
			}

			// item not found, do we want to add it?
			if (!found)
			{
				if (!addIfNotFound)
					return false;

				SIMenuItem* item = new SIMenuItem;
				item->name = menuName;
				item->displayName = menuName;
				item->position = SIMenuItemPosition::Normal;
				item->imageIndex = -1;
				item->subMenu = (splitStringList->Count > 0);
				item->parent = group;
				item->groups = new ArrayList();
				AddToItemsList(group->menuItems, item);
				currGroups = item->groups;
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
	void SIContextMenu::RefreshMenuGroup(SIMenuGroup *group, MenuCommand *&lastVisibleItem)
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
	void SIContextMenu::AddToItemsList(ArrayList *list, SIMenuItem *item)
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
	void SIContextMenu::AddToGroupsList(ArrayList *list, SIMenuGroup *group)
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
	void SIContextMenu::OnMenuItemClicked(Object *sender, EventArgs *args)
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
	int SIContextMenu::GetMenuItemIndex(MenuCommandCollection *items, MenuCommand *item)
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