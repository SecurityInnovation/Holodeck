//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ColumnHeaderCollection.h
//
// DESCRIPTION: Contains definition for the class ColumnHeaderCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <system.dll>
#using <system.windows.forms.dll>

#include "ToggleColumnHeader.h"

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Contains ToggleColumnHeader objects for a ContainerListView
	//*********************************************************************
	public __gc class ColumnHeaderCollection : public CollectionBase
	{
	public:
		EventHandler *WidthResized;

		ColumnHeaderCollection();
		~ColumnHeaderCollection();

		__property ToggleColumnHeader *get_Item(int index);
		__property void set_Item(int index, ToggleColumnHeader *value);

		virtual int Add(ToggleColumnHeader *item);
		virtual ToggleColumnHeader *Add(String *str, int width, HorizontalAlignment textAlign);
		virtual void AddRange(ToggleColumnHeader *items[]);
	private:
		void OnWidthResized(Object *sender, EventArgs *e);
	};
}