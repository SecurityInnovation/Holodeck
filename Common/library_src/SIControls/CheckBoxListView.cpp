//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		CheckBoxListView.cpp
//
// DESCRIPTION: Contains implementation for the class CheckBoxListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 May 2004		 B. Shirey	 File created.
//*************************************************************************
#include "CheckBoxListView.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		CheckBoxListView
	// Description: Constructor for the CheckBoxListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	CheckBoxListView::CheckBoxListView() : ListView()
	{
		isDoubleClicking = false;

		View = System::Windows::Forms::View::Details;
		CheckBoxes = true;
	}

	//*************************************************************************
	// Method:		~CheckBoxListView
	// Description: Destructor for the CheckBoxListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	CheckBoxListView::~CheckBoxListView()
	{
	}

	//*************************************************************************
	// Method:		OnItemCheck
	// Description: Called when the check box selection changes
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void CheckBoxListView::OnItemCheck(ItemCheckEventArgs *args)
	{
		if (isDoubleClicking) 
        {
            args->NewValue = args->CurrentValue;
            isDoubleClicking = false;
        }
        else
			__super::OnItemCheck(args);
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: Called when the mouse button is pressed
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void CheckBoxListView::OnMouseDown(MouseEventArgs *args)
	{
		if ((args->X >= 0) && (args->X < Columns->get_Item(0)->Width))
			isDoubleClicking = false;
		else
			isDoubleClicking = true;

		__super::OnMouseDown(args);
	}

	//*************************************************************************
	// Method:		OnKeyDown
	// Description: Called when the key button is pressed
	//
	// Parameters:
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void CheckBoxListView::OnKeyDown(KeyEventArgs *args)
	{
		isDoubleClicking = false;
		__super::OnKeyDown(args);
	}
}