//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIToolBar.cpp
//
// DESCRIPTION: Contains implementation for the class SIToolBar
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#include <windows.h>
#include <commctrl.h>
#include "SIToolBar.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SIToolBar
	// Description: Constructor for the SIToolBar class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIToolBar::SIToolBar()
	{
		groups = new ArrayList();
		this->ImageList = new System::Windows::Forms::ImageList();

		this->ButtonClick += new ToolBarButtonClickEventHandler(this, OnToolBarButtonClicked);
	}

	//*************************************************************************
	// Method:		~SIToolBar
	// Description: Destructor for the SIToolBar class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIToolBar::~SIToolBar()
	{
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the toolbar's image list
	//
	// Parameters:
	//	fileName - the name of the file that holds the image
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int SIToolBar::AddImageToList(String *fileName)
	{
		try
		{
			Bitmap *bitmap = new Bitmap(fileName);
			return this->ImageList->Images->Add(bitmap, Color::Empty);
		}
		catch(...)
		{
		}

		return -1;
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the toolbar's image list
	//
	// Parameters:
	//	image - Image to add
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int SIToolBar::AddImageToList(Bitmap *image)
	{
		return this->ImageList->Images->Add(image, Color::Empty);
	}

	//*************************************************************************
	// Method:		AddGroup
	// Description: Adds a button group to the control
	//
	// Parameters:
	//	groupName - the name of the group to add
	//	position - the relative position to add the group
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::AddGroup(String *groupName, SIToolBarButtonPosition position)
	{
		SIToolBarButtonGroup* group;

		groupName = String::Concat(groupName, S"->");

		// Call ProcessMenuName to force group creation with correct position info
		ProcessButtonName(true, groupName, group, position);
	}

	//*************************************************************************
	// Method:		AddButton
	// Description: Adds a button to the control
	//
	// Parameters:
	//	itemName - the internal name for the item
	//	position - the relative position to add the item in the group
	//	displayString - the text to display as the button text
	//	helpString - the text to use as a help string for the button
	//	imageIndex - the index of the image in the image list to display for the button
	//	stateQueryHandler - the handler to get the state of the button
	//	clickedHandler - the delegate to invoke when the button is clicked
	//	tag - any user defined object you want
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::AddButton(String *itemName, SIToolBarButtonPosition position, String *displayString,
			String *helpString, int imageIndex, ToolBarButtonQueryStateDelegate *stateQueryHandler,
			ToolBarButtonClickedDelegate *clickedHandler, Object *tag)
	{
		SIToolBarButtonGroup* group;

		ProcessButtonName(true, itemName, group, SIToolBarButtonPosition::Normal);
		if (!group)
			return;

		// Try to find an existing item by this name
		for (int i = 0; i < group->buttons->Count; i++)
		{
			SIToolBarButton* item = dynamic_cast<SIToolBarButton*>(group->buttons->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, update info and exit
				item->displayName = displayString;
				item->helpString = helpString;
				item->position = position;
				item->imageIndex = imageIndex;
				item->OnQueryState = stateQueryHandler;
				item->OnClick = clickedHandler;
				item->tag = tag;
				return;
			}
		}

		// No menu by this name, add it to the list
		SIToolBarButton* item = new SIToolBarButton;
		item->name = itemName;
		item->displayName = displayString;
		item->helpString = helpString;
		item->imageIndex = imageIndex;
		item->style = ToolBarButtonStyle::PushButton;
		item->dropDownMenu = NULL;
		item->position = position;
		item->parent = group;
		item->OnQueryState = stateQueryHandler;
		item->OnClick = clickedHandler;
		item->tag = tag;
		AddToItemsList(group->buttons, item);

		Refresh();
	}

	//*************************************************************************
	// Method:		SetButtonOptionDisplayString
	// Description: changes the display string for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the display string for
	//	displayString - the string to set as the displayed menu item text
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionDisplayString(String *itemName, String *displayString)
	{
		SIToolBarButton *item = FindButton(itemName);
		if (item)
		{
			item->displayName = displayString;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetButtonOptionHelpString
	// Description: changes the help string for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the help string for
	//	helpString - the string to set as the button's help text
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionHelpString(String *itemName, String *helpString)
	{
		SIToolBarButton *item = FindButton(itemName);
		if (item)
		{
			item->helpString = helpString;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetButtonOptionTag
	// Description: changes the user-defined tag object for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	tag - the user-defined object to assign to the button
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionTag(String *itemName, Object *tag)
	{
		SIToolBarButton* item = FindButton(itemName);
		if (item)
		{
			item->tag = tag;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetButtonOptionImageIndex
	// Description: changes the image for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	imageIndex - the image to assign to the button
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionImageIndex(String *itemName, int imageIndex)
	{
		SIToolBarButton* item = FindButton(itemName);
		if (item)
		{
			item->imageIndex = imageIndex;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetButtonOptionStyle
	// Description: changes the style for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	style - the style to assign to the button
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionStyle(String *itemName, ToolBarButtonStyle style)
	{
		SIToolBarButton* item = FindButton(itemName);
		if (item)
		{
			item->style = style;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetButtonOptionContextMenu
	// Description: changes the context menu for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	menu - the context menu to assign to the button
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionContextMenu(String *itemName, Menu *menu)
	{
		SIToolBarButton* item = FindButton(itemName);
		if (item)
		{
			item->dropDownMenu = menu;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetButtonOptionIconType
	// Description: changes the icon type for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	type - the icon type to assign to the button
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::SetButtonOptionIconType(String *itemName, IconType type)
	{
		SIToolBarButton* item = FindButton(itemName);
		if (item)
		{
			item->imageIconType = type;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		FindButton
	// Description: returns an item with the specified name
	//
	// Parameters:
	//	itemName - the name of the item to find
	//
	// Return Value: the found item, or NULL if not found
	//*************************************************************************
	SIToolBarButton *SIToolBar::FindButton(String *itemName)
	{
		SIToolBarButtonGroup* group;

		if (!ProcessButtonName(false, itemName, group, SIToolBarButtonPosition::Normal))
			return NULL;

		// Try to find an existing item by this name
		for (int i = 0; i < group->buttons->Count; i++)
		{
			SIToolBarButton* item = dynamic_cast<SIToolBarButton*>(group->buttons->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, return to caller
				return item;
			}
		}

		return NULL;
	}
	
	//*************************************************************************
	// Method:		GetButtonFullName
	// Description: gets the full name of a specified item
	//
	// Parameters:
	//	item - the item to get the name of
	//
	// Return Value: the full name of the specified item
	//*************************************************************************
	String *SIToolBar::GetButtonFullName(SIToolBarButton *item)
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
	// Description: Updates the states of all items in the toolbar control
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::Refresh()
	{
		SIToolBarButton* lastVisible = NULL;

		// Refresh each group
		for (int i = 0; i < groups->Count; i++)
		{
			SIToolBarButtonGroup *curr = dynamic_cast<SIToolBarButtonGroup *>(groups->get_Item(i));
			if (!curr)
				continue;

			RefreshButtonGroup(curr, lastVisible);
		}

		// Go back through the groups and add separators as necessary
		for (int i = 0; i < groups->Count; i++)
		{
			SIToolBarButtonGroup *curr = dynamic_cast<SIToolBarButtonGroup *>(groups->get_Item(i));
			if (curr->visibleCount == 0)
				continue;

			bool needSeparator = false;
			for (int j = i + 1; j < groups->Count; j++)
			{
				SIToolBarButtonGroup *other = dynamic_cast<SIToolBarButtonGroup *>(groups->get_Item(j));
				if (other->visibleCount == 0)
					continue;

				// Found non-empty group to the right, need a separator
				needSeparator = true;
				break;
			}

			if (needSeparator && (!curr->separatorInserted))
			{
				// Separator required but not yet visible
				if (!curr->separatorToolBarButton)
				{
					curr->separatorToolBarButton = new ToolBarButton();
					curr->separatorToolBarButton->Style = ToolBarButtonStyle::Separator;
				}

				lastVisible = NULL;
				for (int j = 0; j < curr->buttons->Count; j++)
				{
					SIToolBarButton* item = dynamic_cast<SIToolBarButton*>(curr->buttons->Item[j]);
					if (item->itemInserted)
						lastVisible = item;
				}

				if (lastVisible != NULL)
				{
					Buttons->Insert(Buttons->IndexOf(lastVisible->button) + 1, curr->separatorToolBarButton);
					curr->separatorInserted = true;
				}
			}
			else if ((!needSeparator) && curr->separatorInserted)
			{
				// Separator not required but currently visible
				Buttons->Remove(curr->separatorToolBarButton);
				curr->separatorInserted = false;
			}
		}
	}

	//*************************************************************************
	// Method:		ProcessButtonName
	// Description: parses a button name and processes it
	//
	// Parameters:
	//	addIfNotFound - if true and the name is not found, an item will be added
	//	buttonName - the name of the button to process
	//	group - the group of the button to process
	//	position - the relative position of the item to process
	//
	// Return Value: None
	//*************************************************************************
	bool SIToolBar::ProcessButtonName(bool addIfNotFound, String *&buttonName, SIToolBarButtonGroup *&group, SIToolBarButtonPosition groupPosition)
	{
		String *groupName = "None";
		if (buttonName->IndexOf("->") != -1)
		{
			groupName = buttonName->Substring(0, buttonName->IndexOf("->"));
			buttonName = buttonName->Substring(buttonName->IndexOf("->") + 2);
		}

		// find the parent group if we can
		bool found = false;
		SIToolBarButtonGroup *parentGroup = NULL;
		for (int i = 0; i < groups->Count; i++)
		{
			SIToolBarButtonGroup *curr = dynamic_cast<SIToolBarButtonGroup *>(groups->get_Item(i));
			if (!curr || !curr->name)
				continue;

			if (curr->name->Equals(groupName))
			{
				parentGroup = curr;
				found = true;
				break;
			}
		}

		// we can't find the parent group, do we want to add it?
		if (!found)
		{
			if (!addIfNotFound)
				return false;

			SIToolBarButtonGroup *newGroup = new SIToolBarButtonGroup;
			newGroup->name = groupName;
			newGroup->buttons = new ArrayList();
			newGroup->position = groupPosition;
			newGroup->separatorInserted = false;
			newGroup->separatorToolBarButton = NULL;
			newGroup->visibleCount = 0;
			parentGroup = newGroup;
			AddToGroupsList(parentGroup);
		}

		group = parentGroup;

		// look for the item in the group
		found = false;
		for (int i = 0; i < parentGroup->buttons->Count; i++)
		{
			SIToolBarButton *curr = dynamic_cast<SIToolBarButton *>(parentGroup->buttons->get_Item(i));
			if (!curr || !curr->name)
				continue;

			if (curr->name->Equals(buttonName))
			{
				found = true;
				break;
			}
		}

		// button not found, do we want to add it?
		if (!found)
		{
			if (!addIfNotFound)
				return false;

			SIToolBarButton *button = new SIToolBarButton;
			button->name = buttonName;
			button->position = SIToolBarButtonPosition::Normal;
			button->parent = parentGroup;
		}

		return true;
	}

	//*************************************************************************
	// Method:		RefreshButtonGroup
	// Description: refreshes the state of all items in a menu group
	//
	// Parameters:
	//	group - the group to refresh
	//	lastVisibleItem - the last visible item refreshed
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::RefreshButtonGroup(SIToolBarButtonGroup *group, SIToolBarButton *&lastVisibleItem)
	{
		int visibleCount = 0;

		for (int i = 0; i < group->buttons->Count; i++)
		{
			SIToolBarButton* item = dynamic_cast<SIToolBarButton*>(group->buttons->Item[i]);

			SIToolBarButtonState state = SIToolBarButtonState::Enabled;
			if (item->OnQueryState)
				state = item->OnQueryState->Invoke(GetButtonFullName(item), item->tag);

			if (item->button == NULL)
			{
				ToolBarButton* button = new ToolBarButton();
				button->Text = item->displayName;
				button->Style = item->style;
				button->DropDownMenu = item->dropDownMenu;
				button->ToolTipText = item->helpString;
				button->ImageIndex = item->imageIndex;
				button->Tag = item;
				item->button = button;
				item->itemInserted = false;
			}

			if (state == SIToolBarButtonState::Invisible)
			{
				if (item->itemInserted)
				{
					this->Buttons->Remove(item->button);
					item->itemInserted = false;
				}
			}
			else
			{
				visibleCount++;

				if (!item->itemInserted)
				{
					if (lastVisibleItem == NULL)
						Buttons->Insert(0, item->button);
					else
						Buttons->Insert(Buttons->IndexOf(lastVisibleItem->button) + 1, item->button);

					item->itemInserted = true;
				}

				if (!item->button->Text->Equals(item->displayName))
					item->button->Text = item->displayName;
				if (item->button->ToolTipText != item->helpString)
					item->button->ToolTipText = item->helpString;
				if (item->button->ImageIndex != item->imageIndex)
					item->button->ImageIndex = item->imageIndex;
				if (item->button->Style != item->style)
					item->button->Style = item->style;
				if (item->button->DropDownMenu != item->dropDownMenu)
					item->button->DropDownMenu = item->dropDownMenu;

				// see if we have to resize the button
				if (item->imageIndex != -1)
				{
					Bitmap *originalImage = IconLibrary::Instance->GetBitmapForType(item->imageIconType);
					ToolBarButton *button = Buttons->get_Item(Buttons->IndexOf(item->button));
					HWND hwnd = (HWND)this->Handle.ToPointer();

					TBBUTTONINFO oldInfo;
					oldInfo.dwMask = TBIF_BYINDEX | TBIF_SIZE;
					oldInfo.cbSize = sizeof(TBBUTTONINFO);
					int id = (int)Buttons->IndexOf(item->button);
					LRESULT result = SendMessage(hwnd, TB_GETBUTTONINFO, (WPARAM)id, (LPARAM)&oldInfo);

					int newWidth = oldInfo.cx + (originalImage->Width - this->ImageSize.Width);

					oldInfo.cx = newWidth;

					result = SendMessage(hwnd, TB_SETBUTTONINFO, (WPARAM)id, (LPARAM)&oldInfo);
				}

				switch (state)
				{
				case SIToolBarButtonState::Disabled:
					if (item->button->Enabled)
						item->button->Enabled = false;
					break;
				case SIToolBarButtonState::Enabled:
					if (!item->button->Enabled)
						item->button->Enabled = true;
					if (item->button->Pushed)
						item->button->Pushed = false;
					break;
				case SIToolBarButtonState::Pushed:
					if (!item->button->Enabled)
						item->button->Enabled = true;
					if (!item->button->Pushed)
						item->button->Pushed = true;
					break;
				}

				lastVisibleItem = item;
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
	void SIToolBar::AddToItemsList(ArrayList *list, SIToolBarButton *item)
	{
		int insertPos;
		for (insertPos = 0; insertPos < list->Count; insertPos++)
		{
			SIToolBarButton* curItem = dynamic_cast<SIToolBarButton*>(list->Item[insertPos]);
			bool insertNow = false;
			switch (item->position)
			{
			case SIToolBarButtonPosition::First:
				// Insert before all "normal" and "last" items
				if ((curItem->position == SIToolBarButtonPosition::Normal) || (curItem->position == SIToolBarButtonPosition::Last))
					insertNow = true;
				break;
			case SIToolBarButtonPosition::Normal:
				// Insert before all "last" items
				if (curItem->position == SIToolBarButtonPosition::Last)
					insertNow = true;
				break;
			case SIToolBarButtonPosition::Last:
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
	//	group - the group to add to the list
	//
	// Return Value: None
	//*************************************************************************
	void SIToolBar::AddToGroupsList(SIToolBarButtonGroup *group)
	{
		int insertPos;
		for (insertPos = 0; insertPos < groups->Count; insertPos++)
		{
			SIToolBarButtonGroup* curGroup = dynamic_cast<SIToolBarButtonGroup*>(groups->Item[insertPos]);
			bool insertNow = false;
			switch (group->position)
			{
			case SIToolBarButtonPosition::First:
				// Insert before all "normal" and "last" groups
				if ((curGroup->position == SIToolBarButtonPosition::Normal) || (curGroup->position == SIToolBarButtonPosition::Last))
					insertNow = true;
				break;
			case SIToolBarButtonPosition::Normal:
				// Insert before all "last" groups
				if (curGroup->position == SIToolBarButtonPosition::Last)
					insertNow = true;
				break;
			case SIToolBarButtonPosition::Last:
				// Always at end, never break out early
				break;
			}
			if (insertNow)
				break;
		}
		groups->Insert(insertPos, group);
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
	void SIToolBar::OnToolBarButtonClicked(Object *sender, ToolBarButtonClickEventArgs *args)
	{
		ToolBarButton *button = args->Button;
		if (!button)
			return;

		SIToolBarButton* item = dynamic_cast<SIToolBarButton*>(button->Tag);
		if (item == NULL)
			return;

		if (item->OnClick == NULL)
			return;

		item->OnClick->Invoke(GetButtonFullName(item), item->tag);
	}
}