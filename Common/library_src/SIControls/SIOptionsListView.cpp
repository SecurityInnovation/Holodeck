//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIOptionsListView.cpp
//
// DESCRIPTION: Contains implementation for the class SIOptionsListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jun 2004		 R. Wagner	 File created.
//*************************************************************************
#include "SIOptionsListView.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SIOptionsListView
	// Description: Constructor for the SIOptionsListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIOptionsListView::SIOptionsListView(SIOptionsList* options, Object* cxt): ContainerListView()
	{
		optionsList = options;
		context = cxt;

		listItems = new ArrayList();
		groupInstance = new Hashtable();
		itemInstance = new Hashtable();

		HeaderStyle = ColumnHeaderStyle::None;
		ColumnTracking = false;
		RowTracking = false;
		HideSelection = true;
		HoverSelection = false;
		Activation = ItemActivation::OneClick;
		RowHeight = 22;

		descHeader = new ToggleColumnHeader();
		descHeader->Text = "Description";
		descHeader->Width = 250;

		controlHeader = new ToggleColumnHeader();
		controlHeader->Text = "Setting";
		controlHeader->Width = 80;

		Columns->Add(descHeader);
		Columns->Add(controlHeader);

		Items->QueryListViewItem += new QueryContainerListViewItemDelegate(this, OnQueryItem);
		Items->QueryListViewItemIndex += new QueryContainerListViewItemIndexDelegate(this, OnQueryItemIndex);
		SetListViewItemSelected += new SetListViewItemSelectedDelegate(this, OnSetListViewItemSelected);
		SetListViewItemFocused += new SetListViewItemFocusedDelegate(this, OnSetListViewItemFocused);

		optionsList->OnOptionsUpdate += new OptionsUpdateDelegate(this, OnOptionsUpdate);

		// Load initial settings
		for (int i=0; i<options->Groups->Count; i++)
		{
			SIOptionGroup* group = dynamic_cast<SIOptionGroup*>(options->Groups->Item[i]);

			for (int j=0; j<group->options->Count; j++)
			{
				SIOption* item = dynamic_cast<SIOption*>(group->options->Item[j]);

				try
				{
					if (item->OnQueryValue)
					{
						GetItemInstance(item)->control->Text = item->OnQueryValue->Invoke(
							optionsList->GetOptionFullName(item), item->tag, this);
					}
				}
				catch (Exception*)
				{
					// Value load failed, try using the default context
					context = NULL;

					try
					{
						if (item->OnQueryValue)
						{
							GetItemInstance(item)->control->Text = item->OnQueryValue->Invoke(
								optionsList->GetOptionFullName(item), item->tag, this);
						}
					}
					catch (Exception*)
					{
						// Use a default according to type
						switch (item->type)
						{
						case SIOptionType::BooleanOption:
							GetItemInstance(item)->control->Text = "Disabled";
							break;
						case SIOptionType::IntegerOption:
							GetItemInstance(item)->control->Text = "0";
							break;
						case SIOptionType::StringOption:
							GetItemInstance(item)->control->Text = "";
							break;
						}
					}

					// Set context to correct value
					context = cxt;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		~SIOptionsListView
	// Description: Destructor for the SIOptionsListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIOptionsListView::~SIOptionsListView()
	{
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
	void SIOptionsListView::Refresh()
	{
		SIOptionInstance* lastVisible = NULL;

		// Refresh each group
		for (int i = 0; i < optionsList->Groups->Count; i++)
		{
			SIOptionGroup *curr = dynamic_cast<SIOptionGroup *>(optionsList->Groups->get_Item(i));
			if (!curr)
				continue;

			RefreshOptionGroup(curr, lastVisible);
		}

		// Go back through the groups and add headers as necessary
		for (int i = 0; i < optionsList->Groups->Count; i++)
		{
			SIOptionGroup *curr = dynamic_cast<SIOptionGroup *>(optionsList->Groups->get_Item(i));
			SIOptionGroupInstance *currInstance = GetGroupInstance(curr);

			if (currInstance->visibleCount == 0)
			{
				if (currInstance->headerInserted)
				{
					// Header not required but currently visible
					listItems->Remove(currInstance->headerItem);
					currInstance->headerInserted = false;
				}
			}
			else
			{
				if (!currInstance->headerInserted)
				{
					// Header required but not yet visible
					if (!currInstance->headerItem)
					{
						currInstance->headerItem = new ContainerListViewItem();
						currInstance->headerItem->Text = curr->displayName;
						currInstance->headerItem->ItemFont = new System::Drawing::Font(Font, FontStyle::Bold);
					}

					SIOptionInstance* firstVisible = NULL;
					for (int j = 0; j < curr->options->Count; j++)
					{
						SIOption* item = dynamic_cast<SIOption*>(curr->options->Item[j]);
						SIOptionInstance* itemInstance = GetItemInstance(item);

						if (itemInstance->itemInserted)
						{
							firstVisible = itemInstance;
							break;
						}
					}

					if (firstVisible != NULL)
					{
						listItems->Insert(listItems->IndexOf(firstVisible->item), currInstance->headerItem);
						currInstance->headerInserted = true;
					}
				}
			}
		}

		ItemCount = listItems->Count;
		ContainerListView::Refresh();
	}

	//*************************************************************************
	// Method:		RefreshOptionGroup
	// Description: refreshes the state of all items in a menu group
	//
	// Parameters:
	//	group - the group to refresh
	//	lastVisibleItem - the last visible item refreshed
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::RefreshOptionGroup(SIOptionGroup *group, SIOptionInstance *&lastVisibleItem)
	{
		int visibleCount = 0;

		for (int i = 0; i < group->options->Count; i++)
		{
			SIOption* item = dynamic_cast<SIOption*>(group->options->Item[i]);
			SIOptionInstance* itemInstance = GetItemInstance(item);

			SIOptionState state = SIOptionState::Enabled;
			if (item->OnQueryState)
				state = item->OnQueryState->Invoke(optionsList->GetOptionFullName(item), item->tag, this);

			if (itemInstance->item == NULL)
			{
				ContainerListViewItem* listItem = new ContainerListViewItem();
				listItem->Text = String::Concat(S"    ", item->displayName);
				listItem->SubItems->Add(itemInstance->control);
				listItem->Tag = item;
				itemInstance->item = listItem;
				itemInstance->itemInserted = false;
			}

			if (state == SIOptionState::Invisible)
			{
				if (itemInstance->itemInserted)
				{
					this->listItems->Remove(itemInstance->item);
					itemInstance->itemInserted = false;
				}
			}
			else
			{
				visibleCount++;

				if (!itemInstance->itemInserted)
				{
					if (lastVisibleItem == NULL)
						listItems->Insert(0, itemInstance->item);
					else
						listItems->Insert(listItems->IndexOf(lastVisibleItem->item) + 1, itemInstance->item);

					itemInstance->itemInserted = true;
				}

				if (!itemInstance->item->Text->Equals(String::Concat(S"    ", item->displayName)))
					itemInstance->item->Text = String::Concat(S"    ", item->displayName);

				switch (state)
				{
				case SIOptionState::Disabled:
					if (itemInstance->item->ForeColor == Color::FromKnownColor(KnownColor::WindowText))
						itemInstance->item->ForeColor = Color::FromKnownColor(KnownColor::GrayText);
					if (itemInstance->control->Enabled)
						itemInstance->control->Enabled = false;
					break;
				case SIOptionState::Enabled:
					if (itemInstance->item->ForeColor == Color::FromKnownColor(KnownColor::GrayText))
						itemInstance->item->ForeColor = Color::FromKnownColor(KnownColor::WindowText);
					if (!itemInstance->control->Enabled)
						itemInstance->control->Enabled = true;
					break;
				}

				lastVisibleItem = itemInstance;
			}
		}

		GetGroupInstance(group)->visibleCount = visibleCount;
	}

	//*************************************************************************
	// Method:		OnQueryItem
	// Description: Called when the list view needs to find an item
	//
	// Parameters:
	//	index - index of item
	//
	// Return Value: Item object
	//*************************************************************************
	ContainerListViewItem *SIOptionsListView::OnQueryItem(unsigned int index)
	{
		if ((index < 0) || (index >= (unsigned int)listItems->Count))
			return NULL;
		return dynamic_cast<ContainerListViewItem*>(listItems->Item[index]);
	}

	//*************************************************************************
	// Method:		OnQueryItemIndex
	// Description: Called when the list view needs to find the index of
	//				an item
	//
	// Parameters:
	//	item - item to find index of
	//
	// Return Value: Index of item
	//*************************************************************************
	unsigned int SIOptionsListView::OnQueryItemIndex(ContainerListViewItem *item)
	{
		return listItems->IndexOf(item);
	}

	//*************************************************************************
	// Method:		OnSetListViewItemSelected
	// Description: Called when the selected item is updated.  This function
	//				will prevent items from showing a selection.
	//
	// Parameters:
	//	index - index of item
	//  selected - true if selected
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::OnSetListViewItemSelected(int index, bool selected)
	{
		if ((index < 0) || (index >= listItems->Count))
			return;

		ContainerListViewItem* item = dynamic_cast<ContainerListViewItem*>(listItems->Item[index]);
		if (item)
			item->Selected = false;
	}

	//*************************************************************************
	// Method:		OnSetListViewItemFocused
	// Description: Called when the focused item is updated.  This function
	//				will prevent items from showing a focus.
	//
	// Parameters:
	//	index - index of item
	//  focused - true if focused
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::OnSetListViewItemFocused(int index, bool focused)
	{
		if ((index < 0) || (index >= listItems->Count))
			return;

		ContainerListViewItem* item = dynamic_cast<ContainerListViewItem*>(listItems->Item[index]);
		if (item)
			item->Focused = false;
	}

	//*************************************************************************
	// Method:		OnItemTextChanged
	// Description: Called when a setting is updated
	//
	// Parameters:
	//	sender - Control that sent the event
	//  args - Event args
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::OnItemTextChanged(Object* sender, EventArgs* args)
	{
		OnOptionsUpdate(optionsList);
	}

	//*************************************************************************
	// Method:		GetOptionValue
	// Description: Gets the value of an option as a string
	//
	// Parameters:
	//	itemName - full name of item
	//
	// Return Value: Value of option
	//*************************************************************************
	String* SIOptionsListView::GetOptionValue(String *itemName)
	{
		SIOption* opt = optionsList->FindOption(itemName);
		if (!opt)
			return NULL;

		SIOptionInstance* optInstance = GetItemInstance(opt);
		return optInstance->value;
	}

	//*************************************************************************
	// Method:		SetOptionValue
	// Description: Sets the value of an option as a string
	//
	// Parameters:
	//	itemName - full name of item
	//  value - new value of option
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::SetOptionValue(String* itemName, String* value)
	{
		SIOption* opt = optionsList->FindOption(itemName);
		if (!opt)
			return;

		SIOptionInstance* optInstance = GetItemInstance(opt);
		optInstance->value = value;
		if (optInstance->control && (!optInstance->control->IsDisposed))
			optInstance->control->Text = value;
	}

	//*************************************************************************
	// Method:		Validate
	// Description: Validates the current settings
	//
	// Parameters:
	//	None
	//
	// Return Value: true if input is valid, false otherwise
	//*************************************************************************
	bool SIOptionsListView::Validate()
	{
		for (int i=0; i<optionsList->Groups->Count; i++)
		{
			SIOptionGroup* group = dynamic_cast<SIOptionGroup*>(optionsList->Groups->Item[i]);

			for (int j=0; j<group->options->Count; j++)
			{
				SIOption* item = dynamic_cast<SIOption*>(group->options->Item[j]);
				SIOptionInstance* itemInstance = GetItemInstance(item);

				try
				{
					if (item->OnQueryState)
					{
						// Don't validate disabled or invisible options
						if (item->OnQueryState->Invoke(optionsList->GetOptionFullName(item), item->tag, this) !=
							SIOptionState::Enabled)
							continue;
					}

					if (item->type == SIOptionType::IntegerOption)
					{
						try
						{
							Int32::Parse(itemInstance->value);
						}
						catch (Exception*)
						{
							// Number parse failed, invalid integer
							return false;
						}
					}

					if (item->OnValidateValue)
					{
						if (!item->OnValidateValue->Invoke(optionsList->GetOptionFullName(item), item->tag, this,
							itemInstance->value))
							return false;
					}
				}
				catch (Exception*)
				{
					// Validation failed
					return false;
				}
			}
		}
		return true;
	}

	//*************************************************************************
	// Method:		Commit
	// Description: Commits the options to the given context
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::Commit()
	{
		for (int i=0; i<optionsList->Groups->Count; i++)
		{
			SIOptionGroup* group = dynamic_cast<SIOptionGroup*>(optionsList->Groups->Item[i]);

			for (int j=0; j<group->options->Count; j++)
			{
				SIOption* item = dynamic_cast<SIOption*>(group->options->Item[j]);
				SIOptionInstance* itemInstance = GetItemInstance(item);

				try
				{
					if (item->OnQueryState)
					{
						// Don't commit disabled or invisible options
						if (item->OnQueryState->Invoke(optionsList->GetOptionFullName(item), item->tag, this) !=
							SIOptionState::Enabled)
							continue;
					}

					if (item->OnCommitValue)
						item->OnCommitValue->Invoke(optionsList->GetOptionFullName(item), item->tag, this, itemInstance->value);
				}
				catch (Exception*)
				{
					// Ignore failed commits
				}
			}
		}
	}

	//*************************************************************************
	// Method:		GetOptionNames
	// Description: Gets the list of enabled options
	//
	// Parameters:
	//	None
	//
	// Return Value: List of enabled options
	//*************************************************************************
	ArrayList* SIOptionsListView::GetOptionNames()
	{
		ArrayList* list = new ArrayList();

		for (int i=0; i<optionsList->Groups->Count; i++)
		{
			SIOptionGroup* group = dynamic_cast<SIOptionGroup*>(optionsList->Groups->Item[i]);

			for (int j=0; j<group->options->Count; j++)
			{
				SIOption* item = dynamic_cast<SIOption*>(group->options->Item[j]);

				try
				{
					if (item->OnQueryState)
					{
						// Don't include disabled or invisible options
						if (item->OnQueryState->Invoke(optionsList->GetOptionFullName(item), item->tag, this) !=
							SIOptionState::Enabled)
							continue;
					}

					list->Add(optionsList->GetOptionFullName(item));
				}
				catch (Exception*)
				{
					// Don't add options that crash
				}
			}
		}

		return list;
	}

	//*************************************************************************
	// Method:		OnOptionsUpdate
	// Description: Called when the available options are updated
	//
	// Parameters:
	//	optionsList - SIOptionsList object that was updated
	//
	// Return Value: None
	//*************************************************************************
	void SIOptionsListView::OnOptionsUpdate(SIOptionsList* optionsList)
	{
		// Update value fields
		for (int i=0; i<optionsList->Groups->Count; i++)
		{
			SIOptionGroup* group = dynamic_cast<SIOptionGroup*>(optionsList->Groups->Item[i]);

			for (int j=0; j<group->options->Count; j++)
			{
				SIOption* item = dynamic_cast<SIOption*>(group->options->Item[j]);
				SIOptionInstance* itemInstance = GetItemInstance(item);

				itemInstance->value = itemInstance->control->Text;
			}
		}

		Refresh();
	}

	//*************************************************************************
	// Method:		GetGroupInstance
	// Description: Gets the SIOptionGroupInstance object for a given
	//				SIOptionGroup
	//
	// Parameters:
	//	group - Group to query
	//
	// Return Value: SIOptionGroupInstance object
	//*************************************************************************
	SIOptionGroupInstance* SIOptionsListView::GetGroupInstance(SIOptionGroup* group)
	{
		SIOptionGroupInstance* instance = dynamic_cast<SIOptionGroupInstance*>(groupInstance->Item[group]);
		if (instance)
			return instance;

		instance = new SIOptionGroupInstance();
		groupInstance->Add(group, instance);
		return instance;
	}

	//*************************************************************************
	// Method:		GetItemInstance
	// Description: Gets the SIOptionInstance object for a given SIOption
	//
	// Parameters:
	//	item - Item to query
	//
	// Return Value: SIOptionInstance object
	//*************************************************************************
	SIOptionInstance* SIOptionsListView::GetItemInstance(SIOption* item)
	{
		SIOptionInstance* instance = dynamic_cast<SIOptionInstance*>(itemInstance->Item[item]);
		if (instance)
			return instance;

		instance = new SIOptionInstance();

		// Create the control for editing the setting
		ComboBox* comboBox;
		TextBox* textBox;
		switch (item->type)
		{
		case SIOptionType::BooleanOption:
			comboBox = new ComboBox();
			comboBox->DropDownStyle = ComboBoxStyle::DropDownList;
			comboBox->Items->Add(S"Disabled");
			comboBox->Items->Add(S"Enabled");
			comboBox->SelectedIndex = 0;
			comboBox->SelectedIndexChanged += new EventHandler(this, OnItemTextChanged);
			instance->control = comboBox;
			break;
		case SIOptionType::IntegerOption:
			textBox = new TextBox();
			textBox->Text = "0";
			textBox->Select(0, 0);
			textBox->TextChanged += new EventHandler(this, OnItemTextChanged);
			instance->control = textBox;
			break;
		case SIOptionType::StringOption:
			textBox = new TextBox();
			textBox->Text = "";
			textBox->Select(0, 0);
			textBox->TextChanged += new EventHandler(this, OnItemTextChanged);
			instance->control = textBox;
			break;
		}

		instance->control->Location = System::Drawing::Point(0, 0);
		instance->control->Width = controlHeader->Width;

		itemInstance->Add(item, instance);
		return instance;
	}
}