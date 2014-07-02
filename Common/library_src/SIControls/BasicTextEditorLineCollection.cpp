//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		BasicTextEditorLineCollection.cpp
//
// DESCRIPTION: Contains implementation for the class BasicTextEditorLineCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Jul 2004		 R. Wagner	 File created.
//*************************************************************************
#include "BasicTextEditorLineCollection.h"


namespace SIControls
{
	//*************************************************************************
	// Method:		BasicTextEditorLineCollection
	// Description: Constructor for the BasicTextEditorLineCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	BasicTextEditorLineCollection::BasicTextEditorLineCollection()
	{
		lines = new ArrayList();
		tabWidth = 4;
	}

    
	//*************************************************************************
	// Method:		~BasicTextEditorLineCollection
	// Description: Destructor for the BasicTextEditorLineCollection class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	BasicTextEditorLineCollection::~BasicTextEditorLineCollection()
	{
	}


	//*************************************************************************
	// Method:		GetLine
	// Description: Gets the contents of a line
	//
	// Parameters:
	//	index - Index of line
	//
	// Return Value: Tokens the line is composed of
	//*************************************************************************
	TextToken* BasicTextEditorLineCollection::GetLine(UInt32 index) __gc[]
	{
		if (index >= (UInt32)lines->Count)
			return NULL;

		TextToken* tokens __gc[] = new TextToken* __gc[1];

		tokens[0] = new TextToken();
		tokens[0]->text = dynamic_cast<String*>(lines->Item[index]);
		tokens[0]->color = Color::Black;
		tokens[0]->col = 0;

		return tokens;
	}


	//*************************************************************************
	// Method:		GetCharacterCol
	// Description: Gets the column index for a given character
	//
	// Parameters:
	//	x - Character index (tab is one character, but >=1 columns)
	//  y - Line index
	//
	// Return Value: Column index
	//*************************************************************************
	int BasicTextEditorLineCollection::GetCharacterCol(int x, int y)
	{
		if (y >= lines->Count)
			return 0;

		String* line = dynamic_cast<String*>(lines->Item[y]);
		int cur = 0;
		for (int i=0; (i<line->Length) && (i<x); i++)
		{
			if (line->Chars[i] == L'\t')
			{
				cur += tabWidth - (cur % tabWidth);
				continue;
			}
			cur++;
		}
		return cur;
	}


	//*************************************************************************
	// Method:		GetColCharacter
	// Description: Gets the character index of a specific column
	//
	// Parameters:
	//	col - Column index
	//  y - Line index
	//
	// Return Value: Character index (tab is one character, but >=1 columns)
	//*************************************************************************
	int BasicTextEditorLineCollection::GetColCharacter(int col, int y)
	{
		if (y >= lines->Count)
			return 0;

		String* line = dynamic_cast<String*>(lines->Item[y]);
		int i = 0;
		for (int cur=0; (cur<col) && (i<line->Length); i++)
		{
			if (line->Chars[i] == L'\t')
			{
				int width = tabWidth - (cur % tabWidth);
				if (((cur + width) > col) && ((col - cur) < (width / 2)))
					break;
				cur += width;
				continue;
			}
			cur++;
		}
		return i;
	}


	//*************************************************************************
	// Method:		Refresh
	// Description: Refreshes any dirty or changed lines
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void BasicTextEditorLineCollection::Refresh()
	{
		for (int i=0; i<lines->Count; i++)
		{
			String* line = dynamic_cast<String*>(lines->Item[i]);

			// Update width
			UInt32 width = 0;
			for (int j=0; j<line->Length; j++)
			{
				if (line->Chars[j] == L'\t')
				{
					width += tabWidth - (width % tabWidth);
					continue;
				}
				width++;
			}

			if (width > maxWidth)
				maxWidth = width;
		}
	}

	
	//*************************************************************************
	// Method:		set_TabWidth
	// Description: Sets the tab width for this instance
	//
	// Parameters:
	//	value - Tab width
	//
	// Return Value: None
	//*************************************************************************
	void BasicTextEditorLineCollection::set_TabWidth(UInt32 value)
	{
		tabWidth = value;
		Refresh();
	}
}
