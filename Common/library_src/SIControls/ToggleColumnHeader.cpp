//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ToggleColumnHeader.cpp
//
// DESCRIPTION: Contains implemenation for the class ToggleColumnHeader
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "ToggleColumnHeader.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		ToggleColumnHeader
	// Description: Constructor for the ToggleColumnHeader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ToggleColumnHeader::ToggleColumnHeader()
	{
		index = 0;
		listView = NULL;
		textAlign = HorizontalAlignment::Left;
		width = 90;
		isVisible = true;
		isHovered = false;
		isPressed = false;
		isSelected = false;
		scaleStyle = ColumnScaleStyle::Slide;
		textColor = Color::Empty;
	}

	//*************************************************************************
	// Method:		OnWidthResized
	// Description: Invokes the WidthResized delegate for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ToggleColumnHeader::OnWidthResized()
	{
		if (WidthResized)
			WidthResized->Invoke(this, new EventArgs());
	}

	//*************************************************************************
	// Method:		set_Width
	// Description: sets the width of the column header
	//
	// Parameters:
	//	value - the width to set
	//
	// Return Value: None
	//*************************************************************************
	void ToggleColumnHeader::set_Width(int value)
	{
		width = value;
		OnWidthResized();
	}

	//*************************************************************************
	// Method:		Clone
	// Description: copies this object to a new object
	//
	// Parameters:
	//	None
	//
	// Return Value: a copy of this object
	//*************************************************************************
	Object *ToggleColumnHeader::Clone()
	{
		ToggleColumnHeader *tch = new ToggleColumnHeader();
		tch->Index = index;
		tch->Text = text;
		tch->TextAlign = textAlign;
		tch->Width = width;

		return tch;
	}

	//*************************************************************************
	// Method:		ToString
	// Description: returns a string representation of this object
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representation of this object
	//*************************************************************************
	String *ToggleColumnHeader::ToString()
	{
		return text;
	}
}