//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		StringTools.cpp
//
// DESCRIPTION: Contains implemenation for the class StringTools
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "StringTools.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		StringTools
	// Description: Constructor for the StringTools class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	StringTools::StringTools()
	{
	}

	//*************************************************************************
	// Method:		MeasureDisplayString
	// Description: Gets the size of a string in pixels
	//
	// Parameters:
	//	graphics - the graphics object the string will be measured on
	//	test - the string to measure
	//	font - the font to measure the string in
	//
	// Return Value: the size of the string
	//*************************************************************************
	SizeF StringTools::MeasureDisplayString(Graphics *graphics, String *text, Font *font)
	{
		const int width = 32;

		Bitmap *bitmap = new Bitmap(width, 1, graphics);
		SizeF size = graphics->MeasureString(text, font);
		Graphics *g = Graphics::FromImage(bitmap);

		int measuredWidth = (int)size.Width;
		if (g)
		{
			g->Clear(Color::White);
			g->DrawString(String::Concat(text, "|"), font, Brushes::Black, (float)(width - measuredWidth), (float)(0 - (font->Height / 2)));

			for (int i = width - 1; i >= 0; i--)
			{
				measuredWidth--;
				if (bitmap->GetPixel(i, 0).R == 0)
				{
					break;
				}
			}
		}

		return SizeF((float)measuredWidth, size.Height);
	}

	//*************************************************************************
	// Method:		MeasureDisplayString
	// Description: Gets the width of a string in pixels
	//
	// Parameters:
	//	graphics - the graphics object the string will be measured on
	//	test - the string to measure
	//	font - the font to measure the string in
	//
	// Return Value: the width of the string in pixels
	//*************************************************************************
	int StringTools::MeasureDisplayStringWidth(Graphics *graphics, String *text, Font *font)
	{
		return (int)MeasureDisplayString(graphics, text, font).Width;
	}
}