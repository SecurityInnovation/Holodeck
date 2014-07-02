//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		TextEditor.h
//
// DESCRIPTION: Contains definition for the class TextEditor
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


using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Drawing::Drawing2D;
using namespace System::Text;


namespace SIControls
{
	public __delegate void RedrawLineDelegate(UInt32 row);
	public __delegate void TextChangedDelegate();

	public __gc struct TextToken
	{
		int col;
		Color color;
		String* text;
	};

	public __gc class TextEditorLineCollection
	{
	public:
		RedrawLineDelegate* OnRedrawLine;
		TextChangedDelegate* OnTextChanged;

		__property virtual UInt32 get_Count() = 0;
		__property virtual UInt32 get_MaxWidth() = 0;

		virtual TextToken* GetLine(UInt32 index) __gc[] = 0;

		virtual int GetCharacterCol(int x, int y) = 0;
		virtual int GetColCharacter(int col, int y) = 0;

		virtual void Refresh() = 0;
	};

	public __gc class TextEditor: public Control
	{
		TextEditorLineCollection* lines;

		System::Drawing::Size fontSize, pageSize;

		System::Drawing::Point cursorPos;
		bool caretEnabled, caretVisible;
		static IntPtr currentCursorWindow = IntPtr::Zero;
		bool readOnly, autoScroll;

		RedrawLineDelegate* redrawLineDelegate;
		TextChangedDelegate* textChangedDelegate;

		int GetScrollPosition(int bar);
		void SetScrollPosition(int bar, int pos);
		int GetScrollTrackPosition(int bar);
		void SetScrollPageSize(int width, int height);
		void SetScrollMaximum(int width, int height);

		void HScroll(int type);
		void VScroll(int type);

		void RefreshLines();
		void UpdateSizes();

		void HideCursor();
		void ShowCursor();
		void DisableCursor();
		void EnableCursor();

		void OnRedrawLine(UInt32 row);
		void OnTextChanged();

	protected:
		void WndProc(Message *msg);
		void OnPaint(PaintEventArgs* e);
		void OnResize(EventArgs* e);
		void OnGotFocus(EventArgs* e);
		void OnLostFocus(EventArgs* e);
		void OnKeyDown(KeyEventArgs* e);
		void OnKeyUp(KeyEventArgs* e);
		void OnKeyPress(KeyPressEventArgs* e);
		void OnMouseWheel(MouseEventArgs* e);
		void OnMouseDown(MouseEventArgs* e);
		void OnMouseUp(MouseEventArgs* e);
		void OnMouseMove(MouseEventArgs* e);
		void OnMouseLeave(MouseEventArgs* e);
		bool IsInputKey(System::Windows::Forms::Keys key);

	public:
		TextEditor();

		__property void set_Font(System::Drawing::Font* font);
		__property int get_CharWidth();
		__property int get_CharHeight();

		__property TextEditorLineCollection* get_Lines() { return lines; }
		__property void set_Lines(TextEditorLineCollection* value);

		__property bool get_ReadOnly() { return readOnly; }
		__property void set_ReadOnly(bool value) { readOnly = value; }

		__property bool get_AutoScroll() { return autoScroll; }
		__property void set_AutoScroll(bool value) { autoScroll = value; }
	};
}
