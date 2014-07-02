//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListViewSubItem.cpp
//
// DESCRIPTION: Contains implemenation for the class ContainerListViewSubItem
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "ContainerListViewSubItem.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		ContainerListViewSubItem
	// Description: Constuctor for the ContainerListViewSubItem class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewSubItem::ContainerListViewSubItem()
	{
		text = "SubItem";
		Construct(NULL);
	}

	//*************************************************************************
	// Method:		ContainerListViewSubItem
	// Description: Constuctor for the ContainerListViewSubItem class
	//
	// Parameters:
	//	control - the control representing the subitem
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewSubItem::ContainerListViewSubItem(Control *control)
	{
		text = "";
		Construct(control);
	}

	//*************************************************************************
	// Method:		ContainerListViewSubItem
	// Description: Constuctor for the ContainerListViewSubItem class
	//
	// Parameters:
	//	str - the text for the subitem
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewSubItem::ContainerListViewSubItem(String *str)
	{
		text = str;
		Construct(NULL);
	}

	//*************************************************************************
	// Method:		~ContainerListViewSubItem
	// Description: Destuctor for the ContainerListViewSubItem class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ContainerListViewSubItem::~ContainerListViewSubItem()
	{
	}

	//*************************************************************************
	// Method:		Construct
	// Description: Initializes the sub item class
	//
	// Parameters:
	//	control - the control representing the sub item
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewSubItem::Construct(Control *control)
	{
		childControl = control;

		if (childControl)
			childControl->MouseDown += new MouseEventHandler(this, OnMouseDown);

		BackColor = Color::Empty;
		ForeColor = Color::Empty;
	}

	//*************************************************************************
	// Method:		set_ItemControl
	// Description: sets the child control for the sub item
	//
	// Parameters:
	//	value - the control to use as the items control
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewSubItem::set_ItemControl(Control *value)
	{
		childControl = value;

		if (childControl)
			childControl->MouseDown += new MouseEventHandler(this, OnMouseDown);
	}

	//*************************************************************************
	// Method:		ToString
	// Description: returns a string representation of the sub item
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representation of the sub item
	//*************************************************************************
	String *ContainerListViewSubItem::ToString()
	{
		if (childControl)
			return childControl->ToString();
		return text;
	}

	//*************************************************************************
	// Method:		Clone
	// Description: returns a copy of this sub item
	//
	// Parameters:
	//	None
	//
	// Return Value: a copy of this sub item
	//*************************************************************************
	Object *ContainerListViewSubItem::Clone()
	{
		ContainerListViewSubItem *clvsi = new ContainerListViewSubItem();
		clvsi->ItemControl = NULL;
		clvsi->Text = text;

		return clvsi;
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: invokes the MouseDown delegate for the object
	//
	// Parameters:
	//	sender - the sender of this event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListViewSubItem::OnMouseDown(Object *sender, MouseEventArgs *e)
	{
		if (MouseDown)
			MouseDown->Invoke(sender, e);
	}
}