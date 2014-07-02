//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		enums.h
//
// DESCRIPTION: Contains enums and constants for the SIControls project
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#using <system.dll>
#using <system.windows.forms.dll>

using namespace System;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Contains styles of column scaling
	//*********************************************************************
	public __value enum ColumnScaleStyle
	{
		Slide,
		Spring
	};

	//*********************************************************************
	// Contains modes of selection
	//*********************************************************************
	public __value enum MultiSelectMode
	{
		Single,
		Range,
		Selective
	};

	//*********************************************************************
	// Contains a way to track items being changed
	//*********************************************************************
	public __gc class ItemsChangedEventArgs : public EventArgs
	{
	public:
		static int IndexChanged = -1;
		
		ItemsChangedEventArgs(int indexChanged)
		{
			IndexChanged = indexChanged;
		}
	};

	//*********************************************************************
	// some common delegates
	//*********************************************************************
	public __delegate void ItemsChangedEventHandler(Object *sender, ItemsChangedEventArgs *e);
	public __delegate void ContextMenuEventHandler(Object *sender, MouseEventArgs *e);
	public __delegate void ItemMenuEventHandler(Object *sender, MouseEventArgs *e);
	public __delegate void HeaderMenuEventHandler(Object *sender, MouseEventArgs *e);
}