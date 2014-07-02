//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIOptionsList.cpp
//
// DESCRIPTION: Contains implementation for the class SIOptionsList
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jun 2004		 R. Wagner	 File created.
//*************************************************************************
#include "SIOptionsList.h"
#include "SIOptionsListView.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SIOptionsList
	// Description: Constructor for the SIOptionsList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIOptionsList::SIOptionsList()
	{
		groups = new ArrayList();
	}

	//*************************************************************************
	// Method:		~SIOptionsList
	// Description: Destructor for the SIOptionsList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIOptionsList::~SIOptionsList()
	{
	}

	//*************************************************************************
	// Method:		AddGroup
	// Description: Adds an option group to the control
	//
	// Parameters:
	//	groupName - the name of the group to add
	//  displayName - name displayed in the control
	//	position - the relative position to add the group
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsList::AddGroup(String *groupName, String* displayName, SIOptionPosition position)
	{
		SIOptionGroup* group;

		groupName = String::Concat(groupName, S"->");

		// Call ProcessOptionName to force group creation with correct position info
		ProcessOptionName(true, groupName, group, position, displayName);
	}

	//*************************************************************************
	// Method:		AddOption
	// Description: Adds an option to the control
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
	void SIOptionsList::AddOption(String *itemName, SIOptionPosition position, String *displayString,
			String *helpString, SIOptionType type, OptionQueryStateDelegate *stateQueryHandler,
			OptionQueryValueDelegate *valueQueryHandler, OptionValidateValueDelegate *validateHandler,
			OptionCommitValueDelegate* valueCommitHandler, Object *tag)
	{
		SIOptionGroup* group;

		ProcessOptionName(true, itemName, group, SIOptionPosition::Normal, NULL);
		if (!group)
			return;

		// Try to find an existing item by this name
		for (int i = 0; i < group->options->Count; i++)
		{
			SIOption* item = dynamic_cast<SIOption*>(group->options->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, update info and exit
				item->displayName = displayString;
				item->helpString = helpString;
				item->position = position;
				item->OnQueryState = stateQueryHandler;
				item->OnQueryValue = valueQueryHandler;
				item->OnValidateValue = validateHandler;
				item->OnCommitValue = valueCommitHandler;
				item->tag = tag;
				return;
			}
		}

		// No menu by this name, add it to the list
		SIOption* item = new SIOption;
		item->name = itemName;
		item->displayName = displayString;
		item->helpString = helpString;
		item->position = position;
		item->OnQueryState = stateQueryHandler;
		item->OnQueryValue = valueQueryHandler;
		item->OnValidateValue = validateHandler;
		item->OnCommitValue = valueCommitHandler;
		item->type = type;
		item->tag = tag;
		item->parent = group;
		AddToItemsList(group->options, item);

		if (OnOptionsUpdate)
			OnOptionsUpdate->Invoke(this);
	}

	//*************************************************************************
	// Method:		SetOptionDisplayString
	// Description: changes the display string for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the display string for
	//	displayString - the string to set as the displayed menu item text
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsList::SetOptionDisplayString(String *itemName, String *displayString)
	{
		SIOption *item = FindOption(itemName);
		if (item)
		{
			item->displayName = displayString;
			if (OnOptionsUpdate)
				OnOptionsUpdate->Invoke(this);
		}
	}

	//*************************************************************************
	// Method:		SetOptionHelpString
	// Description: changes the help string for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the help string for
	//	helpString - the string to set as the button's help text
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsList::SetOptionHelpString(String *itemName, String *helpString)
	{
		SIOption *item = FindOption(itemName);
		if (item)
		{
			item->helpString = helpString;
			if (OnOptionsUpdate)
				OnOptionsUpdate->Invoke(this);
		}
	}

	//*************************************************************************
	// Method:		SetOptionTag
	// Description: changes the user-defined tag object for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	tag - the user-defined object to assign to the button
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsList::SetOptionTag(String *itemName, Object *tag)
	{
		SIOption* item = FindOption(itemName);
		if (item)
		{
			item->tag = tag;
			if (OnOptionsUpdate)
				OnOptionsUpdate->Invoke(this);
		}
	}

	//*************************************************************************
	// Method:		FindOption
	// Description: returns an item with the specified name
	//
	// Parameters:
	//	itemName - the name of the item to find
	//
	// Return Value: the found item, or NULL if not found
	//*************************************************************************
	SIOption *SIOptionsList::FindOption(String *itemName)
	{
		SIOptionGroup* group;

		if (!ProcessOptionName(false, itemName, group, SIOptionPosition::Normal, NULL))
			return NULL;

		// Try to find an existing item by this name
		for (int i = 0; i < group->options->Count; i++)
		{
			SIOption* item = dynamic_cast<SIOption*>(group->options->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, return to caller
				return item;
			}
		}

		return NULL;
	}
	
	//*************************************************************************
	// Method:		GetOptionFullName
	// Description: gets the full name of a specified item
	//
	// Parameters:
	//	item - the item to get the name of
	//
	// Return Value: the full name of the specified item
	//*************************************************************************
	String *SIOptionsList::GetOptionFullName(SIOption *item)
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
	// Method:		ProcessOptionName
	// Description: parses a button name and processes it
	//
	// Parameters:
	//	addIfNotFound - if true and the name is not found, an item will be added
	//	buttonName - the name of the button to process
	//	group - the group of the button to process
	//	position - the relative position of the item to process
	//  groupDisplayName - name of group to display in control
	//
	// Return Value: None
	//*************************************************************************
	bool SIOptionsList::ProcessOptionName(bool addIfNotFound, String *&optionName, SIOptionGroup *&group,
		SIOptionPosition groupPosition, String* groupDisplayName)
	{
		String *groupName = "None";
		if (optionName->IndexOf("->") != -1)
		{
			groupName = optionName->Substring(0, optionName->IndexOf("->"));
			optionName = optionName->Substring(optionName->IndexOf("->") + 2);
		}

		// find the parent group if we can
		bool found = false;
		SIOptionGroup *parentGroup = NULL;
		for (int i = 0; i < groups->Count; i++)
		{
			SIOptionGroup *curr = dynamic_cast<SIOptionGroup *>(groups->get_Item(i));
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

			SIOptionGroup *newGroup = new SIOptionGroup;
			newGroup->name = groupName;
			newGroup->options = new ArrayList();
			newGroup->position = groupPosition;
			if (groupDisplayName)
				newGroup->displayName = groupDisplayName;
			else
				newGroup->displayName = groupName;
			parentGroup = newGroup;
			AddToGroupsList(parentGroup);
		}

		group = parentGroup;

		// look for the item in the group
		found = false;
		for (int i = 0; i < parentGroup->options->Count; i++)
		{
			SIOption *curr = dynamic_cast<SIOption *>(parentGroup->options->get_Item(i));
			if (!curr || !curr->name)
				continue;

			if (curr->name->Equals(optionName))
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

			SIOption *button = new SIOption;
			button->name = optionName;
			button->position = SIOptionPosition::Normal;
			button->parent = parentGroup;
		}

		return true;
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
	void SIOptionsList::AddToItemsList(ArrayList *list, SIOption *item)
	{
		int insertPos;
		for (insertPos = 0; insertPos < list->Count; insertPos++)
		{
			SIOption* curItem = dynamic_cast<SIOption*>(list->Item[insertPos]);
			bool insertNow = false;
			switch (item->position)
			{
			case SIOptionPosition::First:
				// Insert before all "normal" and "last" items
				if ((curItem->position == SIOptionPosition::Normal) || (curItem->position == SIOptionPosition::Last))
					insertNow = true;
				break;
			case SIOptionPosition::Normal:
				// Insert before all "last" items
				if (curItem->position == SIOptionPosition::Last)
					insertNow = true;
				break;
			case SIOptionPosition::Last:
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
	void SIOptionsList::AddToGroupsList(SIOptionGroup *group)
	{
		int insertPos;
		for (insertPos = 0; insertPos < groups->Count; insertPos++)
		{
			SIOptionGroup* curGroup = dynamic_cast<SIOptionGroup*>(groups->Item[insertPos]);
			bool insertNow = false;
			switch (group->position)
			{
			case SIOptionPosition::First:
				// Insert before all "normal" and "last" groups
				if ((curGroup->position == SIOptionPosition::Normal) || (curGroup->position == SIOptionPosition::Last))
					insertNow = true;
				break;
			case SIOptionPosition::Normal:
				// Insert before all "last" groups
				if (curGroup->position == SIOptionPosition::Last)
					insertNow = true;
				break;
			case SIOptionPosition::Last:
				// Always at end, never break out early
				break;
			}
			if (insertNow)
				break;
		}
		groups->Insert(insertPos, group);
	}

	//*************************************************************************
	// Method:		CreateListView
	// Description: Creates a new list view with these options
	//
	// Parameters:
	//	context - Context of options
	//
	// Return Value: List view object
	//*************************************************************************
	SIOptionsListView* SIOptionsList::CreateListView(Object* context)
	{
		SIOptionsListView* listView = new SIOptionsListView(this, context);
		return listView;
	}
}