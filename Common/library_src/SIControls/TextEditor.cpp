//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		TextEditor.cpp
//
// DESCRIPTION: Contains implementation for the class TextEditor
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Jul 2004		 R. Wagner	 File created.
//*************************************************************************
#include "TextEditor.h"
#include "BasicTextEditorLineCollection.h"


namespace SIControls
{
	//*************************************************************************
	// Method:		TextEditor
	// Description: Constructor for the TextEditor class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TextEditor::TextEditor(): Control()
	{
		ShowScrollBar((HWND)Handle.ToPointer(), SB_HORZ, true);
		ShowScrollBar((HWND)Handle.ToPointer(), SB_VERT, true);

		redrawLineDelegate = new RedrawLineDelegate(this, OnRedrawLine);
		textChangedDelegate = new TextChangedDelegate(this, OnTextChanged);

		lines = new BasicTextEditorLineCollection();
		lines->OnRedrawLine += redrawLineDelegate;
		lines->OnTextChanged += textChangedDelegate;

		SetStyle(ControlStyles::UserPaint, true);
		SetStyle(ControlStyles::AllPaintingInWmPaint, true);
		SetStyle(ControlStyles::DoubleBuffer, true);
		SetStyle(ControlStyles::Selectable, true);
		SetStyle(ControlStyles::UserMouse, true);
		BackColor = System::Drawing::Color::White;
		TabStop = false;
		Cursor = Cursors::IBeam;

		cursorPos.X = 0;
		cursorPos.Y = 0;
		caretEnabled = false;
		caretVisible = false;

		Font = new System::Drawing::Font("Courier New", 10);

		RefreshLines();
	}


	//*************************************************************************
	// Method:		UpdateSizes
	// Description: Updates the page size
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::UpdateSizes()
	{
		pageSize.Width = ClientSize.Width / fontSize.Width;
		pageSize.Height = ClientSize.Height / fontSize.Height;
		SetScrollPageSize(pageSize.Width, pageSize.Height);
		ShowCursor();
		Invalidate();
	}


	//*************************************************************************
	// Method:		OnResize
	// Description: Called when the control is resized
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnResize(EventArgs* e)
	{
		Control::OnResize(e);
		UpdateSizes();
	}


	//*************************************************************************
	// Method:		set_Font
	// Description: Sets the active font
	//
	// Parameters:
	//	font - New font
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::set_Font(System::Drawing::Font* font)
	{
		Control::set_Font(font);

		Graphics* g = CreateGraphics();
		SizeF size = g->MeasureString("XXXXXX", font);
		fontSize.Width = (int)size.Width / 6;
		fontSize.Height = (int)size.Height + 1;

		UpdateSizes();
	}


	//*************************************************************************
	// Method:		get_CharWidth
	// Description: Gets the width of one character
	//
	// Parameters:
	//	None
	//
	// Return Value: Character width
	//*************************************************************************
	int TextEditor::get_CharWidth()
	{
		return fontSize.Width;
	}


	//*************************************************************************
	// Method:		get_CharHeight
	// Description: Gets the height of one character
	//
	// Parameters:
	//	None
	//
	// Return Value: Character height
	//*************************************************************************
	int TextEditor::get_CharHeight()
	{
		return fontSize.Height;
	}


	//*************************************************************************
	// Method:		HideCursor
	// Description: Hides the cursor
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::HideCursor()
	{
		if (!caretEnabled)
			return;
		if (!caretVisible)
			return;

		HideCaret((HWND)Handle.ToPointer());
		caretVisible = false;
	}


	//*************************************************************************
	// Method:		ShowCursor
	// Description: Displays the cursor
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::ShowCursor()
	{
		if (!caretEnabled)
			return;
		if (!lines)
			return;

		if (caretVisible)
		{
			HideCaret((HWND)Handle.ToPointer());
			caretVisible = false;
		}

		int scrollX = GetScrollPosition(SB_HORZ);
		int scrollY = GetScrollPosition(SB_VERT);

		int row = cursorPos.Y - scrollY;
		int col = lines->GetCharacterCol(cursorPos.X, cursorPos.Y) - scrollX;
		if (row < 0)
			return;
		if (row >= pageSize.Height)
			return;
		if (col < 0)
			return;
		if (col >= pageSize.Width)
			return;

		SetCaretPos(col * fontSize.Width, row * fontSize.Height);
		ShowCaret((HWND)Handle.ToPointer());
		caretVisible = true;
	}


	//*************************************************************************
	// Method:		OnGotFocus
	// Description: Called when the control gains the focus
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnGotFocus(EventArgs* e)
	{
		EnableCursor();
		ShowCursor();
		Control::OnGotFocus(e);
	}


	//*************************************************************************
	// Method:		OnLostFocus
	// Description: Called when the control loses the focus
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnLostFocus(EventArgs* e)
	{
		HideCursor();
		DisableCursor();
		Control::OnGotFocus(e);
	}


	//*************************************************************************
	// Method:		DisableCursor
	// Description: Disables the cursor.  Calls to ShowCursor will be ignored.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::DisableCursor()
	{
		if (currentCursorWindow == Handle)
		{
			DestroyCaret();
			currentCursorWindow = IntPtr::Zero;
		}
		caretEnabled = false;
	}


	//*************************************************************************
	// Method:		EnableCursor
	// Description: Enables the cursor
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::EnableCursor()
	{
		if ((currentCursorWindow != Handle) && (currentCursorWindow != IntPtr::Zero))
			DestroyCaret();

		currentCursorWindow = Handle;
		CreateCaret((HWND)Handle.ToPointer(), NULL, 2, fontSize.Height);
		caretEnabled = true;
	}


	//*************************************************************************
	// Method:		IsInputKey
	// Description: Called to determine if a key is accepted input for
	//				this control
	//
	// Parameters:
	//	key - Key to check
	//
	// Return Value: true if key is input for this control
	//*************************************************************************
	bool TextEditor::IsInputKey(System::Windows::Forms::Keys key)
	{
		if (key == System::Windows::Forms::Keys::Tab)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::Tab))
			return true;
		if (key == System::Windows::Forms::Keys::Up)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::Up))
			return true;
		if (key == System::Windows::Forms::Keys::Down)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::Down))
			return true;
		if (key == System::Windows::Forms::Keys::Left)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::Left))
			return true;
		if (key == System::Windows::Forms::Keys::Right)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::Right))
			return true;
		if (key == System::Windows::Forms::Keys::Home)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::Home))
			return true;
		if (key == System::Windows::Forms::Keys::End)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::End))
			return true;
		if (key == System::Windows::Forms::Keys::PageUp)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::PageUp))
			return true;
		if (key == System::Windows::Forms::Keys::PageDown)
			return true;
		if (key == (System::Windows::Forms::Keys::Shift | System::Windows::Forms::Keys::PageDown))
			return true;
		return Control::IsInputKey(key);
	}


	//*************************************************************************
	// Method:		OnKeyDown
	// Description: Called when a key is pressed down
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnKeyDown(KeyEventArgs* e)
	{
		Control::OnKeyDown(e);
	}


	//*************************************************************************
	// Method:		OnKeyUp
	// Description: Called when a key is released
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnKeyUp(KeyEventArgs* e)
	{
		Control::OnKeyUp(e);
	}


	//*************************************************************************
	// Method:		OnKeyPress
	// Description: Called when a key is pressed
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnKeyPress(KeyPressEventArgs* e)
	{
		Control::OnKeyPress(e);
	}


	//*************************************************************************
	// Method:		OnMouseWheel
	// Description: Called when the mouse wheel is moved
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnMouseWheel(MouseEventArgs* e)
	{
		int amount = e->Delta / 40; // 40 units per line
		if (amount > 0)
		{
			for (int i=0; i<amount; i++)
				VScroll(SB_LINEUP);
		}
		else if (amount < 0)
		{
			for (int i=0; i<(-amount); i++)
				VScroll(SB_LINEDOWN);
		}
		Control::OnMouseWheel(e);
	}


	//*************************************************************************
	// Method:		OnMouseDown
	// Description: Called when a mouse button is pressed down
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnMouseDown(MouseEventArgs* e)
	{
		Control::OnMouseDown(e);
	}


	//*************************************************************************
	// Method:		OnMouseUp
	// Description: Called when a mouse button is released
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnMouseUp(MouseEventArgs* e)
	{
		Control::OnMouseUp(e);
	}


	//*************************************************************************
	// Method:		OnMouseMove
	// Description: Called when the mouse is moved within the control
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnMouseMove(MouseEventArgs* e)
	{
		Control::OnMouseMove(e);
	}


	//*************************************************************************
	// Method:		OnMouseLeave
	// Description: Called when the mouse leaves the control
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnMouseLeave(MouseEventArgs* e)
	{
		Control::OnMouseLeave(e);
	}


	//*************************************************************************
	// Method:		OnPaint
	// Description: Called to paint the control contents
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnPaint(PaintEventArgs* e)
	{
		if (!lines)
			return;

		int scrollX = GetScrollPosition(SB_HORZ);
		int scrollY = GetScrollPosition(SB_VERT);

		RECT drawArea;
		drawArea.left = Math::Max((e->ClipRectangle.Left / fontSize.Width) - 1, 0);
		drawArea.right = Math::Min((e->ClipRectangle.Right / fontSize.Width) + 1, pageSize.Width);
		drawArea.top = Math::Max((e->ClipRectangle.Top / fontSize.Height) - 1, 0);
		drawArea.bottom = Math::Min((e->ClipRectangle.Bottom / fontSize.Height) + 1, pageSize.Height);

		for (int y=drawArea.top; y<=drawArea.bottom; y++)
		{
			UInt32 row = y + scrollY;

			if (row >= lines->Count)
				continue;

			TextToken* tokens __gc[] = lines->GetLine(row);

			if (tokens != NULL)
			{
				int x = 0;
				for (int i=0; i<tokens->Count; i++)
				{
					SolidBrush* brush = new SolidBrush(tokens[i]->color);
					for (int j=0; j<tokens[i]->text->Length; j++)
					{
						if ((tokens[i]->col + j - scrollX) < 0)
							continue;
						if ((tokens[i]->col + j - scrollX) > pageSize.Width)
							break;
						e->Graphics->DrawString(tokens[i]->text->Substring(j, 1), Font, brush,
							(float)((tokens[i]->col + j - scrollX) * fontSize.Width + x - 2), (float)(y * fontSize.Height));
					}
				}
			}
		}
	}


	//*************************************************************************
	// Method:		RefreshLines
	// Description: Updates any lines marked as dirty or changed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::RefreshLines()
	{
		if (lines)
		{
			lines->Refresh();
			SetScrollMaximum(lines->MaxWidth, lines->Count);
		}
	}


	//*************************************************************************
	// Method:		HScroll
	// Description: Called on a horizontal scroll bar event
	//
	// Parameters:
	//	type - Type of event
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::HScroll(int type)
	{
		int oldPos = GetScrollPosition(SB_HORZ);

		RECT scrollRect;
		scrollRect.left = 0;
		scrollRect.right = ClientSize.Width;
		scrollRect.top = 0;
		scrollRect.bottom = ClientSize.Height;

		HideCursor();
		switch (type)
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			SetScrollPosition(SB_HORZ, GetScrollTrackPosition(SB_HORZ));
			ScrollWindow((HWND)Handle.ToPointer(), (oldPos - GetScrollPosition(SB_HORZ)) * fontSize.Width, 0,
				&scrollRect, &scrollRect);
			break;
		case SB_LINERIGHT:
			SetScrollPosition(SB_HORZ, oldPos + 1);
			ScrollWindow((HWND)Handle.ToPointer(), (oldPos - GetScrollPosition(SB_HORZ)) * fontSize.Width, 0,
				&scrollRect, &scrollRect);
			break;
		case SB_LINELEFT:
			SetScrollPosition(SB_HORZ, oldPos - 1);
			ScrollWindow((HWND)Handle.ToPointer(), (oldPos - GetScrollPosition(SB_HORZ)) * fontSize.Width, 0,
				&scrollRect, &scrollRect);
			break;
		case SB_PAGERIGHT:
			SetScrollPosition(SB_HORZ, oldPos + pageSize.Width);
			Invalidate();
			break;
		case SB_PAGELEFT:
			SetScrollPosition(SB_HORZ, oldPos - pageSize.Width);
			Invalidate();
			break;
		case SB_RIGHT:
			SetScrollPosition(SB_HORZ, 0x7fffffff);
			Invalidate();
			break;
		case SB_LEFT:
			SetScrollPosition(SB_HORZ, 0);
			Invalidate();
			break;
		}
		ShowCursor();
	}


	//*************************************************************************
	// Method:		VScroll
	// Description: Called on a vertical scroll bar event
	//
	// Parameters:
	//	type - Type of event
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::VScroll(int type)
	{
		int oldPos = GetScrollPosition(SB_VERT);

		RECT scrollRect;
		scrollRect.left = 0;
		scrollRect.right = ClientSize.Width;
		scrollRect.top = 0;
		scrollRect.bottom = ClientSize.Height;

		HideCursor();
		switch (type)
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			SetScrollPosition(SB_VERT, GetScrollTrackPosition(SB_VERT));
			ScrollWindow((HWND)Handle.ToPointer(), 0, (oldPos - GetScrollPosition(SB_VERT)) * fontSize.Height,
				&scrollRect, &scrollRect);
			break;
		case SB_LINERIGHT:
			SetScrollPosition(SB_VERT, oldPos + 1);
			ScrollWindow((HWND)Handle.ToPointer(), 0, (oldPos - GetScrollPosition(SB_VERT)) * fontSize.Height,
				&scrollRect, &scrollRect);
			break;
		case SB_LINELEFT:
			SetScrollPosition(SB_VERT, oldPos - 1);
			ScrollWindow((HWND)Handle.ToPointer(), 0, (oldPos - GetScrollPosition(SB_VERT)) * fontSize.Height,
				&scrollRect, &scrollRect);
			break;
		case SB_PAGERIGHT:
			SetScrollPosition(SB_VERT, oldPos + pageSize.Width);
			Invalidate();
			break;
		case SB_PAGELEFT:
			SetScrollPosition(SB_VERT, oldPos - pageSize.Width);
			Invalidate();
			break;
		case SB_RIGHT:
			SetScrollPosition(SB_VERT, 0x7fffffff);
			Invalidate();
			break;
		case SB_LEFT:
			SetScrollPosition(SB_VERT, 0);
			Invalidate();
			break;
		}
		ShowCursor();
	}


	//*************************************************************************
	// Method:		GetScrollPosition
	// Description: Gets the current scroll bar position
	//
	// Parameters:
	//	bar - Scroll bar to get position of (SB_HORZ or SB_VERT)
	//
	// Return Value: Position of scroll bar
	//*************************************************************************
	int TextEditor::GetScrollPosition(int bar)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		GetScrollInfo((HWND)Handle.ToPointer(), bar, &si);
		return si.nPos;
	}


	//*************************************************************************
	// Method:		SetScrollPosition
	// Description: Sets the position of the scroll bar
	//
	// Parameters:
	//	bar - Scroll bar to set (SB_HORZ or SB_VERT)
	//  pos - Position of scroll bar
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::SetScrollPosition(int bar, int pos)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS;
		si.nPos = pos;
		SetScrollInfo((HWND)Handle.ToPointer(), bar, &si, TRUE);
	}


	//*************************************************************************
	// Method:		GetScrollTrackPosition
	// Description: Gets the position the scroll bar has been dragged to
	//
	// Parameters:
	//	bar - Scroll bar to query (SB_HORZ or SB_VERT)
	//
	// Return Value: Position
	//*************************************************************************
	int TextEditor::GetScrollTrackPosition(int bar)
	{
		SCROLLINFO si;
		si.cbSize = sizeof(si);
		si.fMask = SIF_TRACKPOS;
		GetScrollInfo((HWND)Handle.ToPointer(), bar, &si);
		return si.nTrackPos;
	}


	//*************************************************************************
	// Method:		SetScrollPageSize
	// Description: Sets the size of a page
	//
	// Parameters:
	//	width - Page width in characters
	//  height - Page height in characters
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::SetScrollPageSize(int width, int height)
	{
		SCROLLINFO si;

		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nPage = width;
		SetScrollInfo((HWND)Handle.ToPointer(), SB_HORZ, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nPage = height;
		SetScrollInfo((HWND)Handle.ToPointer(), SB_VERT, &si, TRUE);
	}


	//*************************************************************************
	// Method:		SetScrollMaximum
	// Description: Sets the maximum scroll position
	//
	// Parameters:
	//	width - Maximum width in characters
	//  height - Maximum height in characters
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::SetScrollMaximum(int width, int height)
	{
		SCROLLINFO si;

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE;
		GetScrollInfo((HWND)Handle.ToPointer(), SB_VERT, &si);
		bool curAtBottom = (GetScrollPosition(SB_VERT) >= (si.nMax - pageSize.Height + 1));

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nMin = 0;
		si.nMax = width + 2;
		si.nPage = pageSize.Width;
		SetScrollInfo((HWND)Handle.ToPointer(), SB_HORZ, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nMin = 0;
		si.nMax = height - 1;
		si.nPage = pageSize.Height;
		SetScrollInfo((HWND)Handle.ToPointer(), SB_VERT, &si, TRUE);

		if (autoScroll && curAtBottom)
		{
			SetScrollPosition(SB_VERT, height - 1);
			Invalidate();
		}
	}


	//*************************************************************************
	// Method:		WndProc
	// Description: Called when a window message is receieved
	//
	// Parameters:
	//	msg - Message
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::WndProc(Message* msg)
	{
		if (msg->Msg == WM_HSCROLL)
			HScroll(msg->WParam.ToInt32() & 0xffff);
		else if (msg->Msg == WM_VSCROLL)
			VScroll(msg->WParam.ToInt32() & 0xffff);
		else
			Control::WndProc(msg);
	}


	//*************************************************************************
	// Method:		set_Lines
	// Description: Sets the lines to be displayed
	//
	// Parameters:
	//	value - Set of lines
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::set_Lines(TextEditorLineCollection* value)
	{
		if (lines)
		{
			lines->OnRedrawLine -= redrawLineDelegate;
			lines->OnTextChanged -= textChangedDelegate;
		}

		HideCursor();

		lines = value;
		cursorPos = System::Drawing::Point(0, 0);

		if (lines)
		{
			lines->OnRedrawLine += redrawLineDelegate;
			lines->OnTextChanged += textChangedDelegate;
		}

		RefreshLines();
		Invalidate();
		ShowCursor();
	}


	//*************************************************************************
	// Method:		OnRedrawLine
	// Description: Called when a line needs to be redrawn
	//
	// Parameters:
	//	row - Line index
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnRedrawLine(UInt32 row)
	{
		UInt32 scrollY = GetScrollPosition(SB_VERT);
		if ((row >= scrollY) && (row <= (scrollY + pageSize.Height)))
		{
			System::Drawing::Rectangle redrawRect;
			redrawRect.X = 0;
			redrawRect.Width = pageSize.Width * fontSize.Width;
			redrawRect.Y = (row - scrollY) * fontSize.Height;
			redrawRect.Height = fontSize.Height;
			Invalidate(redrawRect);
		}
	}


	//*************************************************************************
	// Method:		OnTextChanged
	// Description: Called when the text has changed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TextEditor::OnTextChanged()
	{
		HideCursor();
		RefreshLines();
		ShowCursor();
	}
}
