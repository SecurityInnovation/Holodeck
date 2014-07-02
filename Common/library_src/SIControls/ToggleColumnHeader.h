//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ToggleColumnHeader.h
//
// DESCRIPTION: Contains definition for the class ToggleColumnHeader
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
#using <system.drawing.dll>
#using <system.windows.forms.dll>

#include "Enums.h"
#include <stdio.h>

using namespace System;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Forward declaration of the class to avoid compiler errors
	//*********************************************************************
	public __gc class ContainerListView;

	//*********************************************************************
	// Represents a column header in the ContainerListView class
	//*********************************************************************
	public __gc class ToggleColumnHeader : public ICloneable
	{
	public:
		EventHandler *WidthResized;

		ToggleColumnHeader();

		__property bool get_Selected() { return isSelected; }
		__property void set_Selected(bool value) { isSelected = value; }

		__property Bitmap *get_Image() { return image; }
		__property void set_Image(Bitmap *value) { image = value; }

		__property ColumnScaleStyle get_ScaleStyle() { return scaleStyle; }
		__property void set_ScaleStyle(ColumnScaleStyle value) { scaleStyle = value; }

		__property int get_Index() { return index; }
		__property void set_Index(int value) { index = value; }

		__property String *get_Text() { return text; }
		__property void set_Text(String *value) { text = value; }

		__property Color get_TextColor() { return textColor; }
		__property void set_TextColor(Color value) { textColor = value; }

		__property HorizontalAlignment get_TextAlign() { return textAlign; }
		__property void set_TextAlign(HorizontalAlignment value) { textAlign = value; }

		__property HorizontalAlignment get_ImageAlign() { return imageAlign; }
		__property void set_ImageAlign(HorizontalAlignment value) { imageAlign = value; }

		__property int get_Width() { return width; }
		__property void set_Width(int value);

		__property bool get_Visible() { return isVisible; }
		__property void set_Visible(bool value) { isVisible = value; }

		__property bool get_Hovered() { return isHovered; }
		__property void set_Hovered(bool value) { isHovered = value; }

		__property bool get_Pressed() { return isPressed; }
		__property void set_Pressed(bool value) { isPressed = value; }

		Object *Clone();
		String *ToString();

	private:
		void OnWidthResized();

		int index;
		int width;

		ContainerListView *listView;
		HorizontalAlignment textAlign;
		HorizontalAlignment imageAlign;
		String *text;
		ColumnScaleStyle scaleStyle;
		Bitmap *image;
		Color textColor;

		bool isVisible;
		bool isHovered;
		bool isPressed;
		bool isSelected;
	};
}