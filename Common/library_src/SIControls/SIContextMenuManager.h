//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		SIContextMenuManager.h
//
// DESCRIPTION: Contains definition for the class SIContextMenuManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 14 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#include "SIContextMenu.h"

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>
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
	// a class to manage a set of si context menus
	//*********************************************************************
	public __gc class SIContextMenuManager
	{
	public:
		__property static SIContextMenuManager *get_Instance();

		bool IsContextMenuRegistered(String *menuName);
		bool RegisterContextMenu(String *menuName, SIContextMenu *menu);
		SIContextMenu *GetContextMenu(String *menuName);
		String *GetRegisteredMenuNames()[];

	protected:
		Hashtable *contextMenuTable;

	private:
		SIContextMenuManager();
		~SIContextMenuManager();

		static SIContextMenuManager *instance = NULL;
	};
}