//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		BasicTextEditorLineCollection.h
//
// DESCRIPTION: Contains definition for the class BasicTextEditorLineCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Jul 2004		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <System.Drawing.dll>

#include "TextEditor.h"


namespace SIControls
{
	public __gc class BasicTextEditorLineCollection: public TextEditorLineCollection
	{
		ArrayList* lines;
		UInt32 maxWidth;
		UInt32 tabWidth;

	public:
		BasicTextEditorLineCollection();
		~BasicTextEditorLineCollection();

		UInt32 get_Count() { return lines->Count; }
		UInt32 get_MaxWidth() { return maxWidth; }

		UInt32 get_TabWidth() { return tabWidth; }
		void set_TabWidth(UInt32 value);

		TextToken* GetLine(UInt32 index) __gc[];

		int GetCharacterCol(int x, int y);
		int GetColCharacter(int col, int y);

		void Refresh();
	};
}
