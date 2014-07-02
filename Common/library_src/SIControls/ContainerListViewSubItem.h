//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewSubItem.h
//
// DESCRIPTION: Contains definition for the class ContainerListViewSubItem
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
#using <system.drawing.dll>

#include <stdio.h>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Represents a single subitem in a ContainerListViewItem
	//*********************************************************************
	public __gc class ContainerListViewSubItem : public ICloneable
	{
	public:
		MouseEventHandler *MouseDown;

		ContainerListViewSubItem();
		ContainerListViewSubItem(Control *control);
		ContainerListViewSubItem(String *str);
		~ContainerListViewSubItem();

		__property Control *get_ItemControl() { return childControl; }
		__property void set_ItemControl(Control *value);

		__property String *get_Text() { return text; }
		__property void set_Text(String *value) { text = value; }

		__property Color get_ForeColor() { return foreColor; }
		__property void set_ForeColor(Color value) { foreColor = value; }

		__property Color get_BackColor() { return backColor; }
		__property void set_BackColor(Color value) { backColor = value; }

		String *ToString();

		Object *Clone();
	protected:
		void OnMouseDown(Object *sender, MouseEventArgs *e);
	private:
		String *text;
		Control *childControl;
		Color backColor;
		Color foreColor;

		void Construct(Control *control);
	};
}