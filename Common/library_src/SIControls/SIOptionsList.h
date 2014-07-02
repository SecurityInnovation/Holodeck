//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIOptionsList.h
//
// DESCRIPTION: Contains definition for the class SIOptionsList
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jun 2004		 R. Wagner	 File created.
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
	public __gc class SIOptionsList;
	public __gc class SIOptionsListView;

	// possible option states
	public __value enum SIOptionState
	{
		Invisible,
		Disabled,
		Enabled
	};

	// possible option positions
	public __value enum SIOptionPosition
	{
		First,
		Normal,
		Last
	};

	// possible option types
	public __value enum SIOptionType
	{
		BooleanOption,
		IntegerOption,
		StringOption
	};

	// some delegates used by the options
	public __delegate SIOptionState OptionQueryStateDelegate(String *itemName, Object *tag, SIOptionsListView* context);
	public __delegate String* OptionQueryValueDelegate(String *itemName, Object *tag, SIOptionsListView* context);
	public __delegate bool OptionValidateValueDelegate(String *itemName, Object *tag, SIOptionsListView* context, String* value);
	public __delegate void OptionCommitValueDelegate(String *itemName, Object *tag, SIOptionsListView* context, String* value);

	public __delegate void OptionsUpdateDelegate(SIOptionsList* optionsList);

	// a group of options
	public __gc struct SIOptionGroup
	{
		String *name;
		String *displayName;
		SIOptionPosition position;
		ArrayList *options;
	};

	// an option
	public __gc struct SIOption
	{
		SIOptionGroup *parent;
		String *name;
		String *displayName;
		String *helpString;
		SIOptionType type;
		SIOptionPosition position;
		Object *tag;
		OptionQueryStateDelegate *OnQueryState;
		OptionQueryValueDelegate *OnQueryValue;
		OptionValidateValueDelegate *OnValidateValue;
		OptionCommitValueDelegate *OnCommitValue;
	};

	//*********************************************************************
	// A list of option controls that can be dynamically built by string names
	//*********************************************************************
	public __gc class SIOptionsList
	{
	public:
		SIOptionsList();
		~SIOptionsList();

		void AddGroup(String *groupName, String* displayString, SIOptionPosition position);
		void AddOption(String *itemName, SIOptionPosition position, String *displayString,
			String *helpString, SIOptionType type, OptionQueryStateDelegate *stateQueryHandler,
			OptionQueryValueDelegate *valueQueryHandler, OptionValidateValueDelegate *validateHandler,
			OptionCommitValueDelegate *valueCommitHandler, Object *tag);

		void SetOptionDisplayString(String *itemName, String *displayString);
		void SetOptionHelpString(String *itemName, String *helpString);
		void SetOptionTag(String *itemName, Object *tag);

		SIOption *FindOption(String *itemName);
		String *GetOptionFullName(SIOption *item);

		SIOptionsListView* CreateListView(Object* context);

		__property ArrayList* get_Groups() { return groups; }

		OptionsUpdateDelegate* OnOptionsUpdate;

	private:
		bool ProcessOptionName(bool addIfNotFound, String *&optionName, SIOptionGroup *&group,
			SIOptionPosition groupPosition, String* groupDisplayName);
		void AddToItemsList(ArrayList *list, SIOption *button);
		void AddToGroupsList(SIOptionGroup *group);

		ArrayList *groups;
	};
}