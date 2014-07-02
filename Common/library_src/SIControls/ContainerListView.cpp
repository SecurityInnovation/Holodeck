//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListView.cpp
//
// DESCRIPTION: Contains implemenation for the class ContainerListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include <windows.h>
#include "ContainerListView.h"
#include "StringTools.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		ContainerListView
	// Description: Constructor for the ContainerListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ContainerListView::ContainerListView()
	{
		borderStyle = BorderStyle::Fixed3D;
		headerStyle = ColumnHeaderStyle::Nonclickable;
		colTrackColor = Color::WhiteSmoke;
		colSortColor = Color::Gainsboro;
		gridLineColor = Color::WhiteSmoke;
		multiSelectMode = MultiSelectMode::Single;
		rowTrackColor = Color::WhiteSmoke;
		rowSelectColor = SystemColors::Highlight;
	
		allowColumnReorder = false;
		hideSelection = true;
		hoverSelection = false;
		multiSelect = false;
		labelEdit = false;
		scrollable = true;
		visualStyles = false;
		isFocused = false;
		doColTracking = false;
		colScaleMode = false;
		doRowTracking = true;
		fullRowSelect = true;
		gridLines = false;
		captureFocusClick = false;
		ensureVisible = true;
		visible = false;

		focusedIndex = -1;
		lastSelectedIndex = -1;
		lastColSelected = -1;
		lastColHovered = -1;
		lastColPressed = -1;
		allColsWidth = 0;
		colScalePos = 0;
		colScaleWidth = 0;
		scaledCol = -1;
		lastRowHovered = -1;
		rowHeight = 18;
		allRowsHeight = 0;
		firstSelected = -1;
		lastSelected = -1;
		borderWidth = 2;
		headerBuffer = 20;
		mouseMoveTicks = 0;
		hSize = 0;
		vSize = 0;
		springWidth = 0;
		springCount = 0;

		SetStyle((ControlStyles)(ControlStyles::AllPaintingInWmPaint | ControlStyles::ResizeRedraw |
			ControlStyles::Opaque | ControlStyles::UserPaint | ControlStyles::DoubleBuffer |
			ControlStyles::Selectable | ControlStyles::UserMouse), true);

		BackColor = SystemColors::Window;

		columns = new ColumnHeaderCollection();
		selectedIndices = new ArrayList();
		selectedItems = new SelectedContainerListViewItemCollection();
		updateTransactions = new Stack();
		items = new ContainerListViewItemCollection();

		hScrollBar = new HScrollBar();
		hScrollBar->Parent = this;
		hScrollBar->Minimum = 0;
		hScrollBar->Maximum = 0;
		hScrollBar->SmallChange = 10;
		hScrollBar->Hide();

		vScrollBar = new VScrollBar();
		vScrollBar->Parent = this;
		vScrollBar->Minimum = 0;
		vScrollBar->Maximum = 0;
		vScrollBar->SmallChange = rowHeight;
		vScrollBar->Hide();

		Attach();

		GenerateColumnRects();
		GenerateHeaderRect();
	}

	//*************************************************************************
	// Method:		~ContainerListView
	// Description: Destructor for the ContainerListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ContainerListView::~ContainerListView()
	{
	}

	//*************************************************************************
	// Method:		set_Visible
	// Description: sets whether or not the list view is visible
	//
	// Parameters:
	//	value - true if the list is to be visible, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_Visible(bool value)
	{
		bool visible = true;
		__super::set_Visible(value);
	}
	
	//*************************************************************************
	// Method:		get_Visible
	// Description: gets whether or not the list view is visible
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the list is visible, false otherwise
	//*************************************************************************
	bool ContainerListView::get_Visible()
	{
		visible = false;
		return __super::get_Visible();
	}

	//*************************************************************************
	// Method:		get_SelectedIndex
	// Description: gets the selected index
	//
	// Parameters:
	//	None
	//
	// Return Value: the selected index
	//*************************************************************************
	int ContainerListView::get_SelectedIndex()
	{
		if (selectedIndices->Count > 0)
			return *dynamic_cast<__box int *>(selectedIndices->get_Item(0));

		return -1;
	}

	//*************************************************************************
	// Method:		set_SelectedIndex
	// Description: sets the selected index
	//
	// Parameters:
	//	index - the index of the item to select
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_SelectedIndex(int index)
	{
		IEnumerator *enumerator = selectedIndices->GetEnumerator();
		while (enumerator->MoveNext())
		{
			int item = *dynamic_cast<__box int *>(enumerator->Current);
			ContainerListViewItem *lvi = Items->get_Item(item);
			if (!lvi)
				continue;

			if (this->SetListViewItemFocused && (lvi->Focused))
				SetListViewItemFocused->Invoke(item, false);
			if (this->SetListViewItemSelected && (lvi->Selected))
				SetListViewItemSelected->Invoke(item, false);
		}

		selectedIndices->Clear();
		selectedItems->ClearAll();

		ContainerListViewItem *thisItem = Items->get_Item(index);
		if (!thisItem)
			return;

		selectedIndices->Add(__box(index));
		focusedIndex = index;
		thisItem->Focused = true;
		thisItem->Selected = true;
		selectedItems->Add(thisItem);

		if (this->SetListViewItemFocused)
			SetListViewItemFocused->Invoke(index, true);
		if (this->SetListViewItemSelected)
			SetListViewItemSelected->Invoke(index, true);

		MakeSelectedVisible();
		OnSelectedIndexChanged(EventArgs::Empty);
		Invalidate();
	}

	//*************************************************************************
	// Method:		OnAfterLabelEdit
	// Description: Invokes the AfterLabelEdit delegate for the class
	//
	// Parameters:
	//	e - the args representing the label edit event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnAfterLabelEdit(LabelEditEventArgs *e)
	{
		if (AfterLabelEdit)
		{
			try
			{
				// this should be the label
				AfterLabelEdit->Invoke(this, e);
			}
			catch(Exception *)
			{
			}
		}
	}

	//*************************************************************************
	// Method:		OnBeforeLabelEdit
	// Description: Invokes the BeforeLabelEdit delegate for the class
	//
	// Parameters:
	//	e - the args representing the label edit event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnBeforeLabelEdit(LabelEditEventArgs *e)
	{
		if (BeforeLabelEdit)
		{
			try
			{
				// this should be the label
				BeforeLabelEdit->Invoke(this, e);
			}
			catch(Exception *)
			{
			}
		}
	}

	//*************************************************************************
	// Method:		OnColumnClick
	// Description: Invokes the ColumnClick delegate for the class
	//
	// Parameters:
	//	e - the args representing the column click event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnColumnClick(ColumnClickEventArgs *e)
	{
		if (ColumnClick)
		{
			try
			{
				// this should be the label
				ColumnClick->Invoke(this, e);
			}
			catch(Exception *)
			{
			}
		}
	}

	//*************************************************************************
	// Method:		OnItemActivate
	// Description: Invokes the ItemActivate delegate for the class
	//
	// Parameters:
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnItemActivate(EventArgs *e)
	{
		if (ItemActivate)
		{
			try
			{
				// this should be the label
				ItemActivate->Invoke(this, e);
			}
			catch(Exception *)
			{
			}
		}
	}

	//*************************************************************************
	// Method:		OnSelectedIndexChanged
	// Description: Invokes the SelectedIndexChanged delegate for the class
	//
	// Parameters:
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnSelectedIndexChanged(EventArgs *e)
	{
		if (SelectedIndexChanged)
		{
			try
			{
				// this should be the label
				SelectedIndexChanged->Invoke(this, e);
			}
			catch(Exception *)
			{
			}
		}
	}

	//*************************************************************************
	// Method:		OnSubControlMouseDown
	// Description: Handles mouse clicks on sub controls
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnSubControlMouseDown(Object *sender, MouseEventArgs *e)
	{
		ContainerListViewItem *item = dynamic_cast<ContainerListViewItem *>(sender);
		if (!item)
			return;

		if (multiSelectMode == MultiSelectMode::Single)
		{
			selectedIndices->Clear();
			selectedItems->Clear();

			for (int i = 0; i < (int)ItemCount; i++)
			{
				ContainerListViewItem *thisItem = items->get_Item(i);
				if (!thisItem)
					continue;
	
				thisItem->Focused = false;
				thisItem->Selected = false;

				if (thisItem->Equals(item))
				{
					thisItem->Focused = true;
					thisItem->Selected = true;
					firstSelected = i;
					focusedIndex = i;

					selectedIndices->Add(__box(i));
					selectedItems->Add(thisItem);
				}
			}
			OnSelectedIndexChanged(new EventArgs());
		}
		else if (multiSelectMode == MultiSelectMode::Range)
		{
		}
		else if (multiSelectMode == MultiSelectMode::Selective)
		{
			// unfocus the prev item
			for (int i = 0; i < (int)ItemCount; i++)
			{
				ContainerListViewItem *thisItem = items->get_Item(i);
				if (!thisItem)
					continue;
				thisItem->Focused = false;
			}

			// if the item is already selected, unselect it
			if (item->Selected)
			{
				item->Focused = false;
				item->Selected = false;

				selectedIndices->Remove(__box(items->IndexOf(item)));
				selectedItems->Remove(item);
				OnSelectedIndexChanged(new EventArgs());
			}
			else
			{
				item->Focused = true;
				item->Selected = true;

				selectedIndices->Add(__box(items->IndexOf(item)));
				selectedItems->Add(item);
				OnSelectedIndexChanged(new EventArgs());
			}

			focusedIndex = items->IndexOf(item);
		}

		Invalidate(this->ClientRectangle);
	}

	//*************************************************************************
	// Method:		OnContextMenuEvent
	// Description: Invokes the ContextMenuEvent delegate for the class
	//
	// Parameters:
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnContextMenuEvent(MouseEventArgs *e)
	{
		if (ContextMenuEvent)
			ContextMenuEvent->Invoke(this, e);
		PopMenu(contextMenu, e);
	}

	//*************************************************************************
	// Method:		OnItemMenuEvent
	// Description: Invokes the ItemMenuEvent delegate for the class
	//
	// Parameters:
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnItemMenuEvent(MouseEventArgs *e)
	{
		if (ItemMenuEvent)
			ItemMenuEvent->Invoke(this, e);
		else if (!itemMenu)
			OnContextMenuEvent(e);
		else
			PopMenu(itemMenu, e);
	}

	//*************************************************************************
	// Method:		OnHeaderMenuEvent
	// Description: Invokes the HeaderMenuEvent delegate for the class
	//
	// Parameters:
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnHeaderMenuEvent(MouseEventArgs *e)
	{
		if (HeaderMenuEvent)
			HeaderMenuEvent->Invoke(this, e);
		else if (!headerMenu)
			OnContextMenuEvent(e);
		else
			PopMenu(itemMenu, e);
	}

	//*************************************************************************
	// Method:		PopMenu
	// Description: displays a popup menu
	//
	// Parameters:
	//	menu - the menu to display
	//	e - the args representing the click event that triggered the popup
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::PopMenu(System::Windows::Forms::ContextMenu *menu, MouseEventArgs *e)
	{
		if (menu)
		{
			if (hScrollBar && hScrollBar->Visible)
				menu->Show(this, Point(e->X - hScrollBar->Value, e->Y));
			else
				menu->Show(this, Point(e->X, e->Y));
		}
	}

	//*************************************************************************
	// Method:		OnScroll
	// Description: called when the user scrolls.  Updates the UI
	//
	// Parameters:
	//	sender - the sender of the scroll event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnScroll(Object *sender, EventArgs *e)
	{
		GenerateColumnRects();
		GenerateHeaderRect();
		Invalidate();
	}

	//*************************************************************************
	// Method:		OnColumnWidthResize
	// Description: called when a column is resized
	//
	// Parameters:
	//	sender - the sender of the resize event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnColumnWidthResize(Object *sender, EventArgs *e)
	{
		GenerateColumnRects();
	}

	//*************************************************************************
	// Method:		BeginUpdate
	// Description: Starts a refresh pause so that many UI updates can be done
	//	before the redraw
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::BeginUpdate()
	{
		updateTransactions->Push(this);
	}

	//*************************************************************************
	// Method:		EndUpdate
	// Description: Resumes a refresh pause so that many UI updates can be applied
	//	and the UI redrawn
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::EndUpdate()
	{
		if (updateTransactions->Count > 0)
			updateTransactions->Pop();
		if (!InUpdateTransaction)
			this->Invalidate();
	}

	//*************************************************************************
	// Method:		set_ItemCount
	// Description: sets the number of items in the listview
	//
	// Parameters:
	//	value - the number of items in the list view
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_ItemCount(unsigned int value)
	{
		items->ItemCount = value;
		if (visible)
		{
			AdjustScrollBars();
			Invalidate();
			vScrollBar->Show();
		}
	}

	//*************************************************************************
	// Method:		set_BorderStyle
	// Description: sets the border style of the list view
	//
	// Parameters:
	//	value - the border style to set
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_BorderStyle(System::Windows::Forms::BorderStyle value)
	{
		borderStyle = value;
		if (borderStyle == System::Windows::Forms::BorderStyle::Fixed3D)
			borderWidth = 2;
		else
			borderWidth = 1;
		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		set_HeaderStyle
	// Description: sets the header style of the list view
	//
	// Parameters:
	//	value - the header style to set
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_HeaderStyle(ColumnHeaderStyle value)
	{
		headerStyle = value;
		Invalidate(this->ClientRectangle);
		if (headerStyle == ColumnHeaderStyle::None)
			headerBuffer = 0;
		else
			headerBuffer = 20;
	}

	//*************************************************************************
	// Method:		set_RowHeight
	// Description: sets the row height of the list view
	//
	// Parameters:
	//	value - the row height to set
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_RowHeight(int value)
	{
		rowHeight = value;
		vScrollBar->SmallChange = rowHeight;
		Invalidate();
	}

	//*************************************************************************
	// Method:		set_GridLines
	// Description: sets whether or not to display grid lines
	//
	// Parameters:
	//	value - true to display grid lines, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_GridLines(bool value)
	{
		gridLines = value;
		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		set_SmallImageList
	// Description: sets the small image list of the list view
	//
	// Parameters:
	//	value - the image list to set
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_SmallImageList(ImageList *value)
	{
		smallImageList = value;
		Invalidate(this->ClientRectangle);
	}

	//*************************************************************************
	// Method:		get_VisualStyles
	// Description: gets whether or not the list view is using the xp visual styles
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the list view is using visual styles, false otherwise
	//*************************************************************************
	bool ContainerListView::get_VisualStyles()
	{
		bool val;
		try
		{
			//bugbug - this can be extended to use the UXTheme api
			val = visualStyles;
		}
		catch(Exception *)
		{
			val = visualStyles;
		}
		return val;
	}

	//*************************************************************************
	// Method:		OnPaint
	// Description: called when the UI needs to be redrawn
	//
	// Parameters:
	//	e - the args representing the paint event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnPaint(PaintEventArgs *e)
	{
		System::Drawing::Rectangle r = ClientRectangle;
		Graphics *g = e->Graphics;

		DrawBackground(g, r);
		DrawRows(g, r);
		DrawHeaders(g, r);
		DrawExtra(g, r);
		DrawBorder(g, r);
	}

	//*************************************************************************
	// Method:		OnResize
	// Description: called when the control is resized
	//
	// Parameters:
	//	e - the args representing the resize event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnResize(EventArgs *e)
	{
		Control::OnResize(e);
		GenerateHeaderRect();
		GenerateRowsRect();
		AdjustScrollBars();

		// invalidate subitem controls
		int topItemIndex = GetTopVisibleIndex();
		int bottomItemIndex = GetBottomVisibleIndex();
		for (int i = topItemIndex; i < bottomItemIndex; i++)
		{
			ContainerListViewItem *lvi = items->get_Item(i);
			if (!lvi)
				continue;

			for (int j = 0; j < lvi->SubItems->Count; j++)
			{
				ContainerListViewSubItem *slvi = lvi->SubItems->get_Item(j);
				if (slvi->ItemControl)
					slvi->ItemControl->Invalidate(slvi->ItemControl->ClientRectangle);
			}
		}
		Invalidate();
	}

	//*************************************************************************
	// Method:		WndProc
	// Description: called when a windows message is received for the list view
	//
	// Parameters:
	//	msg - the message received
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::WndProc(Message *msg)
	{
		Control::WndProc(msg);
		if (msg->Msg == WM_GETDLGCODE)
		{
			// this list makes arrow and tab key events cause OnKeyXXX events to fire
			msg->Result = IntPtr(DLGC_WANTCHARS | DLGC_WANTARROWS | msg->Result.ToInt32());
		}
	}

	//*************************************************************************
	// Method:		OnMouseMove
	// Description: called when the mouse moves in the list view
	//
	// Parameters:
	//	e - the args representing the mouse move event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnMouseMove(MouseEventArgs *e)
	{
		int i;
		Control::OnMouseMove(e);

		if ((headerStyle == ColumnHeaderStyle::None) && (!doRowTracking) && (!doColTracking))
		{
			// Don't process mouse move if nothing enabled that requires it
			return;
		}

		lastRowHovered = -1;
		// see if we are trying to move a column
		if (lastColPressed >= 0 && allowColumnReorder)
		{
			if (Math::Abs(e->X - lastClickedPoint.X) > 3)
			{
				// set rect for drag pos
			}
		}
		else if (colScaleMode)
		{
			lastColHovered = -1;
			Cursor::Current = Cursors::VSplit;
			colScalePos = e->X - lastClickedPoint.X;

			ToggleColumnHeader *column = columns->get_Item(scaledCol);
			if (colScalePos + colScaleWidth <= 0)
				column->Width = 1;
			else
				column->Width = colScalePos + colScaleWidth;

			Invalidate();
		}
		else
		{
			if (columns->Count > 0)
			{
				// is the mouse is in the header region
				if (headerStyle != ColumnHeaderStyle::None)
				{
					Cursor::Current = Cursors::Default;
					if (MouseInRect(e, headerRect))
					{
						if (columnRects->Length < columns->Count)
							GenerateColumnRects();

						for (i = 0; i < columns->Count; i++)
						{
							if (MouseInRect(e, columnRects[i]))
							{
								columns->get_Item(i)->Hovered = true;
								lastColHovered = i;
							}
							else
								columns->get_Item(i)->Hovered = false;

							if (MouseInRect(e, columnSizeRects[i]))
								Cursor::Current = Cursors::VSplit;
						}
						Invalidate();
						mouseMoveTicks++;
						if (mouseMoveTicks > 10)
						{
							mouseMoveTicks = 0;
							System::Threading::Thread::Sleep(1);
						}
						return;
					}
				}
			}

			if (lastColHovered >= 0)
			{
				columns->get_Item(lastColHovered)->Hovered = false;
				lastColHovered = -1;
				Invalidate();
			}

			if (ItemCount > 0)
			{
				GenerateRowsRect();
				if (MouseInRect(e, rowsRect))
				{
					int topItemIndex = GetTopVisibleIndex();
					int bottomItemIndex = GetBottomVisibleIndex();

					for (i = topItemIndex; (i < bottomItemIndex); i++)
					{
						ContainerListViewItem *item = items->get_Item(i);
						if (!item)
							continue;

						System::Drawing::Rectangle rect;
						rect.X = ClientRectangle.Left + 2;
						rect.Y = 2 + headerBuffer - vScrollBar->Value + (i * rowHeight);
						rect.Width = ClientRectangle.Width - 4;
						rect.Height = rowHeight - 1;

						if (MouseInRect(e, rect))
						{
							item->Hovered = true;
							lastRowHovered = i;
							break;
						}
						item->Hovered = false;
					}
					Invalidate();
				}
			}
		}

		mouseMoveTicks++;
		if (mouseMoveTicks > 10)
		{
			mouseMoveTicks = 0;
			System::Threading::Thread::Sleep(1);
		}
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: called when the mouse is pressed in the list view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnMouseDown(MouseEventArgs *e)
	{
		try
		{
			lastSelectedIndex = SelectedIndex;

			Control::OnMouseDown(e);
			if (!isFocused)
			{
				this->Focus();
				if (!captureFocusClick)
					return;
			}

			lastClickedPoint = Point(e->X, e->Y);

			// determine if a header was pressed
			if (headerStyle != ColumnHeaderStyle::None)
			{
				if (MouseInRect(e, headerRect) && (e->Button == MouseButtons::Left))
				{
					for (int i = 0; i < columns->Count; i++)
					{
						ToggleColumnHeader *column = columns->get_Item(i);
						column->Pressed = false;
						if (MouseInRect(e, columnSizeRects[i]))
						{
							if (column->ScaleStyle == ColumnScaleStyle::Slide)
							{
								// autosize the column
								if ((e->Clicks == 2) && (e->Button == MouseButtons::Left))
								{
									int mWid = 0;
									int tWid = 0;
									int topItemIndex = GetTopVisibleIndex();
									int bottomItemIndex = GetBottomVisibleIndex();

									for (int j = topItemIndex; j < bottomItemIndex; j++)
									{
										ContainerListViewItem *item = items->get_Item(j);
										if (!item)
											continue;

										System::Drawing::Font* font = Font;
										if (item->ItemFont != NULL)
											font = item->ItemFont;

										if ((i > 0) && (item->SubItems->Count > 0))
											tWid = GetStringWidth(item->SubItems->get_Item(i - 1)->Text, font) + 10;
										else if (i == 0)
											tWid = GetStringWidth(item->Text, font) + 10;
										tWid += 5;
										if (tWid > mWid)
											mWid = tWid;
									}

									tWid = GetStringWidth(columns->get_Item(i)->Text, Font);
									if (tWid > mWid)
										mWid = tWid;

									mWid += 5;
									columns->get_Item(i)->Width = mWid;
								}
								else // scale the column
								{
									colScaleMode = true;
									colScaleWidth = columnRects[i].Width;
									scaledCol = i;
								}
							}
						}
						else if ((MouseInRect(e, columnRects[i])) && (!MouseInRect(e, columnSizeRects[i])))
						{
							columns->get_Item(i)->Pressed = true;
							lastColPressed = i;
						}
					}

					Invalidate();
					return;
				}
			}

			if (e->Button == MouseButtons::Left || e->Button == MouseButtons::Right)
			{
				if (MouseInRect(e, rowsRect) && (ItemCount > 0))
				{
					int topItemIndex = GetTopVisibleIndex();
					int bottomItemIndex = GetBottomVisibleIndex();
					for (int i = topItemIndex; (i <= bottomItemIndex) && (i < (int)ItemCount); i++)
					{
						System::Drawing::Rectangle rect;
						rect.X = ClientRectangle.Left + 2;
						rect.Y = 2 + headerBuffer - vScrollBar->Value + (i * rowHeight);
						rect.Width = ClientRectangle.Width - 4;
						rect.Height = rowHeight - 1;

						if (MouseInRect(e, rect))
						{
							SelectedRow = i;

							switch(multiSelectMode)
							{
							case MultiSelectMode::Single:
								break;
							case MultiSelectMode::Range:
								MoveToIndex(SelectedIndex);
								break;
							case MultiSelectMode::Selective:
								SelectiveSelection(SelectedIndex);
								break;
							}
						}
					}
				}
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: called when the mouse is released in the list view
	//
	// Parameters:
	//	e - the args representing the release event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnMouseUp(MouseEventArgs *e)
	{
		int i;
		Control::OnMouseUp(e);

		lastClickedPoint = Point::Empty;
		if (colScaleMode)
		{
			colScaleMode = false;
			colScalePos = 0;
			scaledCol = -1;
			colScaleWidth = 0;

			AdjustScrollBars();
		}

		if (lastColPressed >= 0)
		{
			ToggleColumnHeader *column = columns->get_Item(lastColPressed);
			column->Pressed = false;
			if ((MouseInRect(e, columnRects[lastColPressed])) && (!MouseInRect(e, columnSizeRects[lastColPressed])) && (e->Button == MouseButtons::Left))
			{
				OnColumnClick(new ColumnClickEventArgs(lastColPressed));

				// change currently selected column
				if (lastColSelected >= 0)
					columns->get_Item(lastColSelected)->Selected = false;
				column->Selected = true;
				lastColSelected = lastColPressed;
			}
		}
		lastColPressed = -1;

		// check for context click
		if (e->Button == MouseButtons::Right)
		{
			if (MouseInRect(e, headerRect))
				OnHeaderMenuEvent(e);
			else if (MouseInRect(e, rowsRect))
			{
				int topItemIndex = GetTopVisibleIndex();
				int bottomItemIndex = GetBottomVisibleIndex();

				for (i = topItemIndex; (i < bottomItemIndex); i++)
				{
					System::Drawing::Rectangle rect;
					rect.X = ClientRectangle.Left + 2;
					rect.Y = 2 + headerBuffer - vScrollBar->Value + (i * rowHeight);
					rect.Width = ClientRectangle.Width - 4;
					rect.Height = rowHeight - 1;

					if (MouseInRect(e, rect))
					{
						OnItemMenuEvent(e);
						break;
					}
				}
				if (i >= (int)ItemCount)
					OnContextMenuEvent(e);
			}
			else
				OnContextMenuEvent(e);
		}
	}

	//*************************************************************************
	// Method:		OnMouseWheel
	// Description: called when the mouse scroll wheel is scrolled in the list view
	//
	// Parameters:
	//	e - the args representing the scroll wheel event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnMouseWheel(MouseEventArgs *e)
	{
		if (e->Delta > 0)
		{
			if (vScrollBar->Visible)
			{
				int newVal = (vScrollBar->Value - vScrollBar->SmallChange * (e->Delta / 100));
				vScrollBar->Value = (newVal < 0) ? 0 : newVal;
			}
			else if (hScrollBar->Visible)
			{
				int newVal = (hScrollBar->Value - hScrollBar->SmallChange * (e->Delta / 100));
				hScrollBar->Value = (newVal < 0) ? 0 : newVal;
			}
		}
		else if (e->Delta < 0)
		{
			if (vScrollBar->Visible)
			{
				int newVal = (vScrollBar->Value - vScrollBar->SmallChange * (e->Delta / 100));
				int max = vScrollBar->Maximum - vScrollBar->LargeChange;
				vScrollBar->Value = (newVal > max) ? max : newVal;
			}
			else if (hScrollBar->Visible)
			{
				int newVal = (hScrollBar->Value - hScrollBar->SmallChange * (e->Delta / 100));
				int max = hScrollBar->Maximum - hScrollBar->LargeChange;
				hScrollBar->Value = (newVal > max) ? max : newVal;
			}
		}
	}

	//*************************************************************************
	// Method:		OnKeyDown
	// Description: called when a key is pressed in the list view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnKeyDown(KeyEventArgs *e)
	{
		OnCheckShiftState(e);
		switch(e->KeyCode)
		{
		case Keys::Home:
		case Keys::End:
		case Keys::PageUp:
		case Keys::PageDown:  // fall thru intentional
			OnPageKeys(e);
			break;
		case Keys::Up:
		case Keys::Down: // fall thru intentional
			OnUpDownKeys(e);
			break;
		default:
			Control::OnKeyDown(e);
			break;
		}

		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		OnPageKeys
	// Description: called when on of the page keys is pressed
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnPageKeys(KeyEventArgs *e)
	{
		switch(e->KeyCode)
		{
		case Keys::Home:
			if (vScrollBar->Visible)
				vScrollBar->Value = 0;
			else
				MoveToIndex(0);

			if (hScrollBar->Visible)
				hScrollBar->Value = 0;

			e->Handled = true;
			break;
		case Keys::End:
			if (vScrollBar->Visible)
				vScrollBar->Value = vScrollBar->Maximum - vScrollBar->LargeChange;
			else
				MoveToIndex(ItemCount - 1);

			if (hScrollBar->Visible)
				hScrollBar->Value = hScrollBar->Maximum - hScrollBar->LargeChange;

			e->Handled = true;
			break;
		case Keys::PageUp:
			if (vScrollBar->Visible)
				vScrollBar->Value = (vScrollBar->LargeChange > vScrollBar->Value) ? 0 : vScrollBar->Value - vScrollBar->LargeChange;
			else
				MoveToIndex(0);

			e->Handled = true;
			break;
		case Keys::PageDown:
			if (vScrollBar->Visible)
			{
				if (vScrollBar->Value + vScrollBar->LargeChange > vScrollBar->Maximum - vScrollBar->LargeChange)
					vScrollBar->Value = vScrollBar->Maximum - vScrollBar->LargeChange;
				else
					vScrollBar->Value = vScrollBar->Value + vScrollBar->LargeChange;
			}
			else
				MoveToIndex(ItemCount - 1);

			e->Handled = true;
			break;
		}
	}

	//*************************************************************************
	// Method:		OnUpDownKeys
	// Description: called when one of the up or down keys is pressed in the list view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnUpDownKeys(KeyEventArgs *e)
	{
		if (ItemCount > 0)
		{
			switch(e->KeyCode)
			{
			case Keys::Down:
				focusedIndex++;
				if (focusedIndex >= (int)ItemCount)
					focusedIndex = ItemCount - 1;
				break;
			case Keys::Up:
				focusedIndex--;
				if (focusedIndex < 0)
					focusedIndex = 0;
				break;
			}

			SelectedRow = focusedIndex;

			MakeSelectedVisible();
			OnSelectedIndexChanged(EventArgs::Empty);
			Invalidate();
			e->Handled = true;
		}
	}

	//*************************************************************************
	// Method:		OnCheckShiftState
	// Description: checks to see if the shift key was pressed when another key
	//	was pressed
	//
	// Parameters:
	//	e - the args representing the key press event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnCheckShiftState(KeyEventArgs *e)
	{
		if (multiSelect)
		{
			if (e->KeyCode == Keys::ControlKey)
				multiSelectMode = MultiSelectMode::Selective;
			else if (e->KeyCode == Keys::ShiftKey)
				multiSelectMode = MultiSelectMode::Range;
		}

		if (!multiSelect && (e->KeyCode == Keys::Return))
		{
			OnItemActivate(new EventArgs());
		}
	}

	//*************************************************************************
	// Method:		OnKeyUp
	// Description: called when the up key is pressed in the list view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnKeyUp(KeyEventArgs *e)
	{
		Control::OnKeyUp(e);
		if (!e->Shift)
			multiSelectMode = MultiSelectMode::Single;
	}

	//*************************************************************************
	// Method:		OnGotFocus
	// Description: called when the list view gets focus
	//
	// Parameters:
	//	e - the args representing the focus event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnGotFocus(EventArgs *e)
	{
		Control::OnGotFocus(e);
		isFocused = true;
		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		OnLostFocus
	// Description: called when the list view loses focus
	//
	// Parameters:
	//	e - the args representing the focus event
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::OnLostFocus(EventArgs *e)
	{
		Control::OnLostFocus(e);
		isFocused = false;
		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		Attach
	// Description: attaches various delegates to this class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::Attach()
	{
		columns->WidthResized += new EventHandler(this, OnColumnWidthResize);

		hScrollBar->ValueChanged += new EventHandler(this, OnScroll);
		vScrollBar->ValueChanged += new EventHandler(this, OnScroll);
	}

	//*************************************************************************
	// Method:		Detach
	// Description: Detaches various delegates from this class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::Detach()
	{
		columns->WidthResized -= new EventHandler(this, OnColumnWidthResize);

		hScrollBar->ValueChanged -= new EventHandler(this, OnScroll);
		vScrollBar->ValueChanged -= new EventHandler(this, OnScroll);
	}

	//*************************************************************************
	// Method:		GenerateColumnRects
	// Description: calculates the sizes needed for the columns and column headers
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::GenerateColumnRects()
	{
		columnRects = new System::Drawing::Rectangle[columns->Count];
		columnSizeRects = new System::Drawing::Rectangle[columns->Count];

		int lWidth = 2 - hScrollBar->Value;
		int colWidth = 0;
		allColsWidth = 0;

		CalcSpringWidths(ClientRectangle);
		for (int i = 0; i < columns->Count; i++)
		{
			colWidth = (columns->get_Item(i)->ScaleStyle == ColumnScaleStyle::Spring) ? springWidth : columns->get_Item(i)->Width;
			columnRects[i] = System::Drawing::Rectangle(lWidth, 2, colWidth, 20);
			columnSizeRects[i] = System::Drawing::Rectangle(lWidth + colWidth - 4, ClientRectangle.Top + 2, 4, headerBuffer);
			lWidth += colWidth;
			allColsWidth += colWidth;
		}
	}

	//*************************************************************************
	// Method:		GenerateHeaderRect
	// Description: calculates the size of the column headers
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::GenerateHeaderRect()
	{
		headerRect = System::Drawing::Rectangle(ClientRectangle.Left + 2, ClientRectangle.Top + 2, ClientRectangle.Width - 4, 20);
	}

	//*************************************************************************
	// Method:		GenerateRowsRect
	// Description: calculates the size of the area that holds list view rows
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::GenerateRowsRect()
	{
		int headerSize = (headerStyle == ColumnHeaderStyle::None) ? 2 : 22;
		rowsRect = System::Drawing::Rectangle(ClientRectangle.Left + 2, ClientRectangle.Top + headerSize, ClientRectangle.Width - 4, ClientRectangle.Height - headerSize);
	}

	//*************************************************************************
	// Method:		AdjustScrollBars
	// Description: resizes the scrollbars to take into account new items, items
	//	removed, and resizing
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::AdjustScrollBars()
	{
		try
		{
			if ((ItemCount > 0) || ((columns->Count > 0) && (!colScaleMode)))
			{
				allColsWidth = 0;
				for (int i = 0; i < columns->Count; i++)
					allColsWidth += columns->get_Item(i)->Width;

				allRowsHeight = ItemCount * rowHeight;
				vSize = vScrollBar->Width;
				hSize = hScrollBar->Height;

				hScrollBar->Left = ClientRectangle.Left + 2;
				hScrollBar->Width = ClientRectangle.Width - vSize - 4;
				hScrollBar->Top = ClientRectangle.Top + ClientRectangle.Height - hScrollBar->Height - 2;
				hScrollBar->Maximum = allColsWidth;
				hScrollBar->LargeChange = (ClientRectangle.Width - vSize - 4 > 0) ? ClientRectangle.Width - vSize - 4 : 0;
				if (allColsWidth > ClientRectangle.Width - 4 - vSize)
				{
					hScrollBar->Show();
					hSize = hScrollBar->Height;
				}
				else
				{
					hScrollBar->Hide();
					hSize = 0;
					hScrollBar->Value = 0;
				}

				vScrollBar->Left = ClientRectangle.Left + ClientRectangle.Width - vScrollBar->Width - 2;
				vScrollBar->Top = ClientRectangle.Top + headerBuffer + 2;
				vScrollBar->Height = ClientRectangle.Height - hSize - headerBuffer - 4;
				//vScrollBar->Maximum = max(0, allRowsHeight - ClientRectangle.Height + headerBuffer + 4 + rowHeight);
				vScrollBar->SmallChange = rowHeight;
				vScrollBar->Maximum = allRowsHeight + headerBuffer + 4 + rowHeight;
				vScrollBar->LargeChange = (int)(ClientRectangle.Height / rowHeight) * vScrollBar->SmallChange;

				if (allRowsHeight > ClientRectangle.Height - headerBuffer - 4 - hSize)
				{
					vScrollBar->Show();
					vSize = vScrollBar->Width;
				}
				else
				{
					vScrollBar->Hide();
					vSize = 0;
					vScrollBar->Value = 0;
				}

				hScrollBar->Width = ClientRectangle.Width - vSize - 4;
				hScrollBar->Top = ClientRectangle.Top + ClientRectangle.Height - hScrollBar->Height - 2;
				hScrollBar->LargeChange = (ClientRectangle.Width - vSize - 4 > 0) ? ClientRectangle.Width - vSize - 4 : 0;
				if (allColsWidth > ClientRectangle.Width - vSize - 4)
					hScrollBar->Show();
				else
				{
					hScrollBar->Hide();
					hSize = 0;
					hScrollBar->Value = 0;
				}
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		UnselectAll
	// Description: unselects all items in the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::UnselectAll()
	{
		for (int i = 0; i < (int)ItemCount; i++)
		{
			ContainerListViewItem *lvi = items->get_Item(i);
			if (!lvi)
				continue;

			lvi->Focused = false;
			lvi->Selected = false;
		}
	}

	//*************************************************************************
	// Method:		MouseInRect
	// Description: determines whether or not the mouse is in a certain rectangle
	//
	// Parameters:
	//	me - the args representing the mouse position
	//	rect - the rect to test the mouse position against
	//
	// Return Value: true if the mouse is in the rectangle, false otherwise
	//*************************************************************************
	bool ContainerListView::MouseInRect(MouseEventArgs *me, System::Drawing::Rectangle rect)
	{
		return ((me->X >= rect.Left) && (me->X <= rect.Left + rect.Width) && (me->Y >= rect.Top) && (me->Y <= rect.Top + rect.Height));
	}

	//*************************************************************************
	// Method:		MakeSelectedVisible
	// Description: makes the selected item visible in the UI
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::MakeSelectedVisible()
	{
		if ((focusedIndex > -1) && ensureVisible)
		{
			ContainerListViewItem *item = items->get_Item(focusedIndex);
			if (!item)
				return;

			if (item && item->Focused && item->Selected)
			{
				System::Drawing::Rectangle r = ClientRectangle;

				int i = focusedIndex;
				try
				{
					int topVisible = this->GetTopVisibleIndex();
					int top = topVisible;
					int bottom = this->GetBottomVisibleIndex() - 1;

					// if it is already visible, don't move
					if ((i > top) && (i < bottom))
						return;

					if (i <= top)
						top = i - 1;
					else if (i >= bottom)
						top = i - (bottom - top) + 1;

					vScrollBar->Value = top * rowHeight;
				}
				catch(ArgumentException *)
				{
					if (vScrollBar->Value > vScrollBar->Maximum)
						vScrollBar->Value = vScrollBar->Maximum;
					else if (vScrollBar->Value < vScrollBar->Minimum)
						vScrollBar->Value = vScrollBar->Minimum;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		GetStringWidth
	// Description: gets the width of a string in pixels
	//
	// Parameters:
	//	s - the string to get the width of
	//
	// Return Value: the width of the string in pixels
	//*************************************************************************
	int ContainerListView::GetStringWidth(String *s, System::Drawing::Font* font)
	{
		Graphics *g = Graphics::FromImage(new Bitmap(32, 32));
		return StringTools::MeasureDisplayStringWidth(g, s, font);
	}

	//*************************************************************************
	// Method:		TruncatedString
	// Description: gets a string that will fit in a given width, truncating when
	//	necessary
	//
	// Parameters:
	//	s - the string to possibly truncate
	//	width - the width the string needs to fit into
	//	offset - the offset of the start of the string
	//	g - the Graphics object to use to truncate the string
	//
	// Return Value: The possibly truncated string.  s if no truncation needed.
	//*************************************************************************
	String *ContainerListView::TruncatedString(String *s, System::Drawing::Font* font, int width, int offset, Graphics *g)
	{
		String *str = "";
		int strWidth;
		int i;
		SizeF strSize;

		try
		{
			strSize = g->MeasureString(s, font);
			if ((int)strSize.Width <= (width - offset))
				return s;

			strWidth = 0;

			for (i = 1; (i <= s->Length) && (strWidth <= width - offset); i++)
			{
				strSize = g->MeasureString(s->Substring(0, i), font);
				strWidth = (int)strSize.Width;
			}
			i--;

			if (i < s->Length)
			{
				if (i <= 3)
					str = String::Concat(s->Substring(0, 1), "...");
				else
					str = String::Concat(s->Substring(0, i - 3), "...");
			}
			else
				str = s->Substring(0, i);
		}
		catch(Exception *)
		{
		}

		return str;
	}

	//*************************************************************************
	// Method:		ShowSelectedItems
	// Description: Shows the selected items in the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::ShowSelectedItems()
	{
		if (firstSelected == focusedIndex)
			ShowSelectedItem(firstSelected);
		else if (firstSelected > focusedIndex)
		{
			for (int i = firstSelected; i >= focusedIndex; i--)
			{
				ContainerListViewItem *item = items->get_Item(i);
				if (!item)
					continue;
				
				item->Selected = true;
				selectedIndices->Add(__box(i));
				selectedItems->Add(item);
			}
		}
		else if (firstSelected < focusedIndex)
		{
			for (int i = firstSelected; i <= focusedIndex; i++)
			{
				ContainerListViewItem *item = items->get_Item(i);
				if (!item)
					continue;

				item->Selected = true;
				selectedIndices->Add(__box(i));
				selectedItems->Add(item);
			}
		}
	}

	//*************************************************************************
	// Method:		ShowSelectedItem
	// Description: show a single selected item in the UI
	//
	// Parameters:
	//	index - the index of the selected item to show
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::ShowSelectedItem(int index)
	{
		ContainerListViewItem *item = items->get_Item(index);
		if (!item)
			return;

		item->Selected = true;
		selectedIndices->Add(__box(index));
		selectedItems->Add(item);
	}

	//*************************************************************************
	// Method:		MoveToIndex
	// Description: scrolls the UI to the specified index
	//
	// Parameters:
	//	index - the index to scroll to
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::MoveToIndex(int index)
	{
		if ((index < 0) || (index >= (int)ItemCount))
			return;

		if (index == focusedIndex)
			return;

		UnselectAll();

		selectedItems->Clear();
		selectedIndices->Clear();

		if (focusedItem)
		{
			focusedItem->Selected = false;
			focusedItem->Focused = false;
		}

		focusedIndex = index;

		if ((multiSelectMode == MultiSelectMode::Single) || (firstSelected == -1))
		{
			firstSelected = focusedIndex;
			ContainerListViewItem *item = items->get_Item(focusedIndex);
			if (!item)
				return;

			item->Focused = true;
			focusedItem = item;
		}

		OnSelectedIndexChanged(new EventArgs());
		ShowSelectedItems();
		MakeSelectedVisible();
		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		IsPointInColumnHeaders
	// Description: determines whether or not a point is in the header region
	//	or not
	//
	// Parameters:
	//	p - the point to check
	//
	// Return Value: true if the point is in the header region, false otherwise
	//*************************************************************************
	bool ContainerListView::IsPointInColumnHeaders(System::Drawing::Point p)
	{
		return ((p.X >= headerRect.Left) &&
				(p.X <= (headerRect.Left + headerRect.Width)) &&
				(p.Y >= headerRect.Top) &&
				(p.Y <= (headerRect.Top + headerRect.Height)));
	}

	//*************************************************************************
	// Method:		SelectiveSelection
	// Description: selects the row at the specified index
	//
	// Parameters:
	//	index - the index of the item to select
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::SelectiveSelection(int index)
	{
		// This is a special case and will be used when the user clicks on an item
		// while the control key is pressed or presses the space bar button while an
		// item has the focus.

		// unfocus the previously focused item
		if ((focusedIndex >= 0) && (focusedIndex < (int)ItemCount))
		{
			ContainerListViewItem *item = items->get_Item(focusedIndex);
			if (!item)
				return;
			item->Focused = false;
		}

		ContainerListViewItem *item = items->get_Item(index);
		if (!item)
			return;
		if (item->Selected)
		{
			item->Focused = false;
			item->Selected = false;

			int i = selectedItems->IndexOf(item);
			selectedIndices->Remove(__box(i));
			selectedItems->Remove(item);

			if (focusedItem == item)
				focusedItem = NULL;

			MakeSelectedVisible();
			OnSelectedIndexChanged(new EventArgs());
		}
		else
		{
			item->Focused = true;
			item->Selected = true;
			
			selectedIndices->Add(__box(index));
			selectedItems->Add(item);
			focusedItem = item;
			
			MakeSelectedVisible();
			OnSelectedIndexChanged(new EventArgs());
		}

		Invalidate();
	}

	//*************************************************************************
	// Method:		CalcSpringWidths
	// Description: calculates the column widths if spring style is selected
	//
	// Parameters:
	//	rect - the rectangle to calculate the widths based on
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::CalcSpringWidths(System::Drawing::Rectangle rect)
	{
		springCount = 0;
		springWidth = rect.Width - (borderWidth * 2);
		for (int i = 0; i < columns->Count; i++)
		{
			ToggleColumnHeader *column = columns->get_Item(i);
			if (column->ScaleStyle == ColumnScaleStyle::Slide)
				springWidth -= column->Width;
			else
				springCount++;
		}

		if ((springCount > 0) && (springWidth > 0))
			springWidth = springWidth / springCount;
	}

	//*************************************************************************
	// Method:		DrawBorder
	// Description: draws the list view's border
	//
	// Parameters:
	//	g - the Graphics object used to do the drawing
	//	r - the rectangle representing the area to draw in
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::DrawBorder(Graphics *g, System::Drawing::Rectangle r)
	{
		System::Drawing::Rectangle rect = ClientRectangle;
		if (borderStyle == BorderStyle::FixedSingle)
			g->DrawRectangle(SystemPens::ControlDarkDark, r.Left, r.Top, r.Width, r.Height);
		else if (borderStyle == BorderStyle::Fixed3D)
			ControlPaint::DrawBorder3D(g, r.Left, r.Top, r.Width, r.Height, Border3DStyle::Sunken);
		else if (borderStyle == BorderStyle::None)
		{
			// nothing to draw
		}
	}

	//*************************************************************************
	// Method:		DrawBackground
	// Description: draws the list view's background
	//
	// Parameters:
	//	g - the Graphics object used to do the drawing
	//	r - the rectangle representing the area to draw in
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::DrawBackground(Graphics *g, System::Drawing::Rectangle r)
	{
		int i;
		int lWidth = 2;
		int lHeight = 1;

		g->FillRectangle(new SolidBrush(BackColor), r);

		// selected column
		if (headerStyle == ColumnHeaderStyle::Clickable)
		{
			for (i = 0; i < columns->Count; i++)
			{
				ToggleColumnHeader *column = columns->get_Item(i);
				if (column->Selected)
				{
					g->FillRectangle(new SolidBrush(colSortColor), r.Left + lWidth - hScrollBar->Value, r.Top + 2 + headerBuffer, column->Width, r.Height - 4 - headerBuffer);
					break;
				}
				lWidth += column->Width;
			}
		}

		// hot tracked column
		if (doColTracking && (lastColHovered >= 0) && (lastColHovered < columns->Count))
			g->FillRectangle(new SolidBrush(colTrackColor), columnRects[lastColHovered].Left, 22, columnRects[lastColHovered].Width, r.Height - 22);

		// hot tracked row
		if (doRowTracking && (lastRowHovered >= 0) && (lastRowHovered < (int)ItemCount))
		{
			int top = 2 + headerBuffer - vScrollBar->Value + (lastRowHovered * rowHeight);
			g->FillRectangle(new SolidBrush(rowTrackColor), r.Left + 2, top, r.Left + r.Width - 4, rowHeight);
		}

		// grid lines
		if (gridLines)
		{
			Pen *p = new Pen(new SolidBrush(gridLineColor), 1.0);
			lWidth = 1;

			// vertical 
			for (i = 0; i < columns->Count; i++)
			{
				ToggleColumnHeader *column = columns->get_Item(i);
				if (r.Left + lWidth + column->Width > r.Left + r.Width - 2)
					break;

				g->DrawLine(p, r.Left + lWidth + column->Width - hScrollBar->Value, r.Top + 2 + headerBuffer, r.Left + lWidth + column->Width - hScrollBar->Value, r.Top + r.Height - 2);
				lWidth += column->Width;
			}

			unsigned int topItemIndex = GetTopVisibleIndex();
			unsigned int bottomItemIndex = GetBottomVisibleIndex();
			lHeight = topItemIndex * rowHeight + 1;

			// horizontal
			for (unsigned int j = topItemIndex; j < bottomItemIndex; j++)
			{
				g->DrawLine(p, r.Left + 2, r.Top + headerBuffer + rowHeight + lHeight - vScrollBar->Value, r.Left + r.Width, r.Top + headerBuffer + rowHeight + lHeight - vScrollBar->Value);
				lHeight += rowHeight;
			}
		}
	}

	//*************************************************************************
	// Method:		DrawHeaders
	// Description: draws the list view's column headers
	//
	// Parameters:
	//	g - the Graphics object used to do the drawing
	//	r - the rectangle representing the area to draw in
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::DrawHeaders(Graphics *g, System::Drawing::Rectangle r)
	{
		if (headerStyle != ColumnHeaderStyle::None)
		{
			g->FillRectangle(new SolidBrush(SystemColors::Control), r.Left + 2, r.Top + 2, r.Width - 2, headerBuffer);

			CalcSpringWidths(r);

			// render column headers and trailing column header
			int last = 2;
			int i;

			int left = r.Left - hScrollBar->Value;
			g->Clip = new System::Drawing::Region(System::Drawing::Rectangle(r.Left + 2, r.Top + 2, r.Width - 5, r.Top + headerBuffer));
			for (i = 0; i < columns->Count; i++)
			{
				ToggleColumnHeader *column = columns->get_Item(i);

				Brush *textBrush = (column->TextColor == Color::Empty) ? SystemBrushes::ControlText : new SolidBrush(column->TextColor);
				if ((left + last + column->Width > r.Left + 2) && (left + last < r.Left + r.Width - 2))
				{
					if ((headerStyle == ColumnHeaderStyle::Clickable) && (column->Pressed))
						ControlPaint::DrawButton(g, left + last, r.Top + 2, column->Width, r.Top + headerBuffer, ButtonState::Flat);
					else
						ControlPaint::DrawButton(g, left + last, r.Top + 2, column->Width, r.Top + headerBuffer, ButtonState::Normal);

					if (column->Image)
					{
						if (column->ImageAlign == HorizontalAlignment::Left)
						{
							g->DrawImage(column->Image, System::Drawing::Rectangle(left + last + 4, r.Top + 3, 16, 16));
							g->DrawString(TruncatedString(column->Text, Font, column->Width, 25, g), Font, textBrush, (float)(left + last + 22), (float)(r.Top + 5));
						}
						else
						{
							g->DrawString(TruncatedString(column->Text, Font, column->Width, 25, g), Font, textBrush, (float)(left + last + 4), (float)(r.Top + 5));
							g->DrawImage(column->Image, System::Drawing::Rectangle(left + last + column->Width - 20, r.Top + 3, 16, 16));
						}
					}
					else
					{
						String *str = TruncatedString(column->Text, Font, column->Width, 0, g);
						if (column->TextAlign == HorizontalAlignment::Left)
							g->DrawString(str, Font, textBrush, (float)(left + last + 4), (float)(r.Top + 5));
						else if (column->TextAlign == HorizontalAlignment::Right)
							g->DrawString(str, Font, textBrush, (float)(left + last + column->Width - StringTools::MeasureDisplayStringWidth(g, str, Font) - 4), (float)(r.Top + 5));
						else
							g->DrawString(str, Font, textBrush, (float)(left + last + (column->Width / 2) - (StringTools::MeasureDisplayStringWidth(g, str, Font) / 2)), (float)(r.Top + 5));
					}
				}

				last += column->Width;
			}

			// only render trailing column header if the end of the last column ends before the listview does
			if (left + last + 5 <= r.Left + r.Width)
			{
				g->Clip = new System::Drawing::Region(System::Drawing::Rectangle(r.Left + 2, r.Top + 2, r.Width - 5, r.Top + headerBuffer));
				ControlPaint::DrawButton(g, left + last, r.Top + 2, r.Width - (r.Left + last) - 3 + hScrollBar->Value, r.Top + headerBuffer, ButtonState::Normal);
			}
		}
	}

	//*************************************************************************
	// Method:		DrawRows
	// Description: draws the list view's rows
	//
	// Parameters:
	//	g - the Graphics object used to do the drawing
	//	r - the rectangle representing the area to draw in
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::DrawRows(Graphics *g, System::Drawing::Rectangle r)
	{
		// don't paint if in transaction
		if (InUpdateTransaction)
			return;

		AdjustScrollBars();

		CalcSpringWidths(r);

		if (columns->Count > 0)
		{
			// render list view item rows
			int last;
			int j;
			int i;

			// help perf by setting some commonly used vars
			int leftView = r.Left + 2;
			int left = leftView - hScrollBar->Value;
			int topView = r.Top + 2 + headerBuffer;
			int top = topView - vScrollBar->Value;

			int topItemIndex = GetTopVisibleIndex();
			int bottomItemIndex = GetBottomVisibleIndex();

			// Mark all controls as not visible (don't actually make them invisible yet or the
			// active control will lose focus)
			IEnumerator *enumerator = this->Controls->GetEnumerator();
			while (enumerator->MoveNext())
			{
				Control *control = dynamic_cast<Control *>(enumerator->Current);
				if ((control != hScrollBar) && (control != vScrollBar))
					control->Tag = __box(false);
			}

			for (i = topItemIndex; i <= bottomItemIndex; i++)
			{
				j = 0;
				last = 0;

				ToggleColumnHeader *column = columns->get_Item(j);
				int colWidth = (column->ScaleStyle == ColumnScaleStyle::Spring) ? springWidth : column->Width;

				if ((top + (rowHeight * (i + 1)) + 2 > r.Top + 2) && (top + (rowHeight * i) + 2 < r.Top + r.Height - 2 - hSize))
				{
					g->Clip = new System::Drawing::Region(System::Drawing::Rectangle(r.Left + 2, r.Top + headerBuffer + 2, r.Width - vSize - 5, r.Height - hSize - 5));

					int rowSelWidth;
					if ((allColsWidth < (r.Width - 5)) || hScrollBar->Visible)
						rowSelWidth = allColsWidth;
					else
						rowSelWidth = r.Width - 5;

					if (!fullRowSelect)
						rowSelWidth = colWidth;

					// render selected item highlights
					ContainerListViewItem *item = items->get_Item(i);
					if (!item)
						continue;

					System::Drawing::Font* font = Font;
					if (item->ItemFont != NULL)
						font = item->ItemFont;

					if (FilterItemColumns)
						FilterItemColumns->Invoke(item);

					// fill the back ground color
					if (item->BackColor != Color::Empty)
						g->FillRectangle(new SolidBrush(item->BackColor), left, top + (rowHeight * i), colWidth - (gridLines ? 1 : 0), rowHeight - (gridLines ? 1 : 0));

					if (!disableSelection)
					{
						if (item->Selected && isFocused)
							g->FillRectangle(new SolidBrush(rowSelectColor), left, top + (rowHeight * i), rowSelWidth, rowHeight);
						else if (item->Selected && !isFocused && hideSelection && !fullHighlightOnHideSelection)
							ControlPaint::DrawFocusRectangle(g, System::Drawing::Rectangle(left, top + (rowHeight * i), rowSelWidth, rowHeight));
						else if (item->Selected && !isFocused && hideSelection && fullHighlightOnHideSelection)
							g->FillRectangle(new SolidBrush(rowSelectColor), left, top + (rowHeight * i), rowSelWidth, rowHeight);
						else if (item->Selected && !isFocused && !hideSelection)
							g->FillRectangle(SystemBrushes::Control, leftView, top + (rowHeight * i), rowSelWidth, rowHeight);
					}
					else
						g->FillRectangle(SystemBrushes::Control, leftView, top + (rowHeight * i), rowSelWidth, rowHeight);

					if (item->Focused && multiSelect && isFocused)
						ControlPaint::DrawFocusRectangle(g, System::Drawing::Rectangle(left, top + (rowHeight * i), rowSelWidth, rowHeight));

					// render item
					if (left + 2 + colWidth > r.Left + 4)
					{
						int w = colWidth > r.Width ? r.Width - 6 : colWidth - 2;
						g->Clip = new System::Drawing::Region(System::Drawing::Rectangle(leftView + 2, topView, w, r.Height - hSize - 5));
						Color userColor = SystemColors::WindowText;
						if (item->ForeColor != Color::Empty)
							userColor = item->ForeColor;

						if (smallImageList && (item->ImageIndex >= 0) && (item->ImageIndex < smallImageList->Images->Count))
						{
							smallImageList->Draw(g, left + 4, top + (rowHeight * i) + 1, 16, 16, item->ImageIndex);
							Brush *brush = (item->Selected && (isFocused || fullHighlightOnHideSelection)) ? SystemBrushes::HighlightText : new SolidBrush(userColor);
							g->DrawString(TruncatedString(item->Text, font, colWidth, 18, g), font, brush, (float)(left + 22), (float)(top + (rowHeight * i) + 2));
						}
						else
						{
							Brush *brush = (item->Selected && (isFocused || fullHighlightOnHideSelection)) ? SystemBrushes::HighlightText : new SolidBrush(userColor);
							g->DrawString(TruncatedString(item->Text, font, colWidth, 0, g), font, brush, (float)(left + 4), (float)(top + (rowHeight * i) + 2));
						}
					}

					// render subitems
					if (columns->Count > 0)
					{
						for (j = 0; (j < item->SubItems->Count) && (j < columns->Count - 1); j++)
						{
							ContainerListViewSubItem *subItem = item->SubItems->get_Item(j);

							int colWidth = (columns->get_Item(j)->ScaleStyle == ColumnScaleStyle::Spring) ? springWidth : columns->get_Item(j)->Width;
							int colPlus1Width = (columns->get_Item(j + 1)->ScaleStyle == ColumnScaleStyle::Spring) ? springWidth : columns->get_Item(j + 1)->Width;
							last += colWidth;

							// don't render it unless it is in the viewport
							if ((left + 2 + last + colPlus1Width > r.Left + 4) &&
								(left + last < r.Left + r.Width - 4))
							{
								int w = colPlus1Width - 6;
								g->Clip = new System::Drawing::Region(System::Drawing::Rectangle(left + last, topView, w + 5, r.Height - hSize - 6));

								if (subItem->BackColor != Color::Empty)
									g->FillRectangle(new SolidBrush(subItem->BackColor), left + last, top + (rowHeight * i), w + 6 - (gridLines ? 1 : 0), rowHeight - (gridLines ? 1 : 0));

								Control *c = subItem->ItemControl;
								if (c)
								{
									c->Location = System::Drawing::Point(left + last + 2, top + (rowHeight * i) + 2);
									c->ClientSize = System::Drawing::Size(colPlus1Width - 6, rowHeight - 4);
									if ((c->Location.Y + c->Size.Height) > (headerBuffer + 2))
									{
										if (c->Location.Y < (headerBuffer + 2))
										{
											// Not entirely visible, set the region to prevent drawing over
											// the header or border
											c->Region = new System::Drawing::Region(System::Drawing::Rectangle(0,
												(headerBuffer + 2) - c->Location.Y, c->Size.Width, c->Size.Height -
												((headerBuffer + 2) - c->Location.Y)));
										}
										else
											c->Region = NULL;

										c->Visible = true;
									}
									else
										c->Visible = false;
									c->Parent = this;
									c->Tag = __box(true);
									c->Invalidate();
									c->Update();
								}
								else
								{
									Color userColor = SystemColors::WindowText;
									if (subItem->ForeColor != Color::Empty)
										userColor = subItem->ForeColor;

									String *str = TruncatedString(subItem->Text, font, colPlus1Width, 12, g);
									if (columns->get_Item(j + 1)->TextAlign == HorizontalAlignment::Left)
									{
										Brush *brush = item->Selected && (isFocused || fullHighlightOnHideSelection) ? SystemBrushes::HighlightText : new SolidBrush(userColor);
										g->DrawString(str, font, brush, (float)(left + last + 4), (float)(top + (rowHeight * i) + 2));
									}
									else if (columns->get_Item(j + 1)->TextAlign == HorizontalAlignment::Right)
									{
										Brush *brush = item->Selected && (isFocused || fullHighlightOnHideSelection) ? SystemBrushes::HighlightText : new SolidBrush(userColor);
										g->DrawString(str, font, brush, (float)(left + last + colPlus1Width - StringTools::MeasureDisplayStringWidth(g, str, this->Font) - 2), (float)(top + (rowHeight * i) + 2));
									}
									else
									{
										Brush *brush = item->Selected && (isFocused || fullHighlightOnHideSelection) ? SystemBrushes::HighlightText : new SolidBrush(userColor);
										g->DrawString(str, font, brush, (float)(left + last + (colPlus1Width / 2) - (StringTools::MeasureDisplayStringWidth(g, str, Font) / 2)), (float)(top + (rowHeight * i) + 2));
									}
								}
							}
							else
							{
								Control *c = subItem->ItemControl;
								if (c)
									c->Visible = false;
							}
						}
					}
				}
			}

			// All controls marked as not visible should now be made invisible
			enumerator = this->Controls->GetEnumerator();
			while (enumerator->MoveNext())
			{
				Control *control = dynamic_cast<Control *>(enumerator->Current);
				if ((control != hScrollBar) && (control != vScrollBar))
				{
					if (!(*dynamic_cast<Boolean*>(control->Tag)))
						control->Visible = false;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		DrawExtra
	// Description: draws the list view's extra data
	//
	// Parameters:
	//	g - the Graphics object used to do the drawing
	//	r - the rectangle representing the area to draw in
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::DrawExtra(Graphics *g, System::Drawing::Rectangle r)
	{
		if (hScrollBar->Visible && vScrollBar->Visible)
		{
			g->ResetClip();
			g->FillRectangle(SystemBrushes::Control, r.Width - vScrollBar->Width - borderWidth, r.Height - hScrollBar->Height - borderWidth, vScrollBar->Width, hScrollBar->Height);
		}
	}

	//*************************************************************************
	// Method:		GetTopVisibleIndex
	// Description: gets the first visible index in the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: the first visible index in the list view
	//*************************************************************************
	unsigned int ContainerListView::GetTopVisibleIndex()
	{
		// get the top visible item -1 (for partially visible items)
		int topItemIndex = (vScrollBar->Value / rowHeight);
		if ((topItemIndex > 0) && ((vScrollBar->Value % rowHeight) != 0))
			topItemIndex--;

		return topItemIndex;
	}


	//*************************************************************************
	// Method:		GetBottomVisibleIndex
	// Description: gets the last visible index in the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: the last visible index in the list view
	//*************************************************************************
	unsigned int ContainerListView::GetBottomVisibleIndex()
	{
		// get the bottom visible item +1 (for partially visible items)
		int bottomItemIndex = (vScrollBar->Value + ClientRectangle.Height - headerBuffer - 4) / rowHeight;
		if (bottomItemIndex < (int)(ItemCount - 1))
			bottomItemIndex++;

		return bottomItemIndex;
	}

	//*************************************************************************
	// Method:		set_SelectedRow
	// Description: sets the selected row by row index
	//
	// Parameters:
	//	row - row index to select
	//
	// Return Value: None
	//*************************************************************************
	void ContainerListView::set_SelectedRow(int row)
	{
		SelectedIndex = row;
	}
}
