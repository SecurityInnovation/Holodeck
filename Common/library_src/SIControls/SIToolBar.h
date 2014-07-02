//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SIToolBar.h
//
// DESCRIPTION: Contains definition for the class SIToolBar
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

#using <MSCorLib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <SIIconLib.dll>

using namespace System;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;
using namespace Crownwood::Magic::Menus;
using namespace Crownwood::Magic::Collections;
using namespace SiIconLib;

namespace SIControls
{
	// possible toolbar button states
	public __value enum SIToolBarButtonState
	{
		Invisible,
		Disabled,
		Enabled,
		Pushed
	};

	// possible toolbar button positions
	public __value enum SIToolBarButtonPosition
	{
		First,
		Normal,
		Last
	};

	// some delegates used by the toolbar buttons
	public __delegate SIToolBarButtonState ToolBarButtonQueryStateDelegate(String *itemName, Object *tag);
	public __delegate void ToolBarButtonClickedDelegate(String *itemName, Object *tag);

	// a group of toolbar buttons
	public __gc struct SIToolBarButtonGroup
	{
		String *name;
		SIToolBarButtonPosition position;
		ArrayList *buttons;
		bool separatorInserted;
		int visibleCount;
		ToolBarButton *separatorToolBarButton;
	};

	// a toolbar button
	public __gc struct SIToolBarButton
	{
		SIToolBarButtonGroup *parent;
		String *name;
		String *displayName;
		String *helpString;
		int imageIndex;
		IconType imageIconType;
		bool itemInserted;
		SIToolBarButtonPosition position;
		Object *tag;
		ToolBarButtonStyle style;
		Menu *dropDownMenu;
		ToolBarButton *button;
		ToolBarButtonQueryStateDelegate *OnQueryState;
		ToolBarButtonClickedDelegate *OnClick;
	};

	//*********************************************************************
	// A toolbar control that can be dynamically built by string names
	//*********************************************************************
	public __gc class SIToolBar : public ToolBar
	{
	public:
		SIToolBar();
		~SIToolBar();

		int AddImageToList(String *fileName);
		int AddImageToList(Bitmap *image);

		void AddGroup(String *groupName, SIToolBarButtonPosition position);
		void AddButton(String *itemName, SIToolBarButtonPosition position, String *displayString,
			String *helpString, int imageIndex, ToolBarButtonQueryStateDelegate *stateQueryHandler,
			ToolBarButtonClickedDelegate *clickedHandler, Object *tag);

		void SetButtonOptionDisplayString(String *itemName, String *displayString);
		void SetButtonOptionHelpString(String *itemName, String *helpString);
		void SetButtonOptionTag(String *itemName, Object *tag);
		void SetButtonOptionImageIndex(String *itemName, int imageIndex);
		void SetButtonOptionStyle(String *itemName, ToolBarButtonStyle style);
		void SetButtonOptionContextMenu(String *itemName, Menu *menu);
		void SetButtonOptionIconType(String *itemName, IconType type);

		SIToolBarButton *FindButton(String *itemName);
		String *GetButtonFullName(SIToolBarButton *button);

		void Refresh();

	private:
		bool ProcessButtonName(bool addIfNotFound, String *&buttonName, SIToolBarButtonGroup *&group, SIToolBarButtonPosition groupPosition);
		void RefreshButtonGroup(SIToolBarButtonGroup *group, SIToolBarButton *&lastVisibleItem);
		void AddToItemsList(ArrayList *list, SIToolBarButton *button);
		void AddToGroupsList(SIToolBarButtonGroup *group);
		void OnToolBarButtonClicked(Object *sender, ToolBarButtonClickEventArgs *args);

		ArrayList *groups;
	};
}