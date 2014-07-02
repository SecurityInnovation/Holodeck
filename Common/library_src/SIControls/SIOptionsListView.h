//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIOptionsListView.h
//
// DESCRIPTION: Contains definition for the class SIOptionsListView
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
#include "SIOptionsList.h"
#include "ContainerListView.h"

using namespace System;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;
using namespace Crownwood::Magic::Menus;
using namespace Crownwood::Magic::Collections;

namespace SIControls
{
	public __gc struct SIOptionGroupInstance
	{
		bool headerInserted;
		ContainerListViewItem* headerItem;
		int visibleCount;
	};

	public __gc struct SIOptionInstance
	{
		ContainerListViewItem* item;
		Control* control;
		String* value;
		bool itemInserted;
	};

	public __gc class SIOptionsListView: public ContainerListView
	{
	public:
		SIOptionsListView(SIOptionsList* options, Object* context);
		~SIOptionsListView();

		__property Object* get_Context() { return context; }
		__property void set_Context(Object* value) { context = value; }

		bool Validate();
		void Commit();
		String* GetOptionValue(String* itemName);
		void SetOptionValue(String* itemName, String* value);
		ArrayList* GetOptionNames();

		void Refresh();

	private:
		void RefreshOptionGroup(SIOptionGroup *group, SIOptionInstance *&lastVisibleItem);
		ContainerListViewItem *OnQueryItem(unsigned int index);
		unsigned int OnQueryItemIndex(ContainerListViewItem *item);
		void OnSetListViewItemSelected(int index, bool selected);
		void OnSetListViewItemFocused(int index, bool focused);
		void OnItemTextChanged(Object* sender, EventArgs* args);
		void OnOptionsUpdate(SIOptionsList* optionsList);
		SIOptionGroupInstance* GetGroupInstance(SIOptionGroup* group);
		SIOptionInstance* GetItemInstance(SIOption* option);

		SIOptionsList* optionsList;
		Object* context;

		ArrayList *listItems;
		ToggleColumnHeader* descHeader;
		ToggleColumnHeader* controlHeader;
		Hashtable *groupInstance;
		Hashtable *itemInstance;
	};
}
