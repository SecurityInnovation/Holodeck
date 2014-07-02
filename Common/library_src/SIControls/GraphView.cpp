//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GraphView.cpp
//
// DESCRIPTION: Contains implemenation for the class GraphView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 23 Jul 2004		 R. Wagner	  File created.
//*************************************************************************
#include <windows.h>
#include "GraphView.h"
#include "ContainerListViewItem.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		GraphView
	// Description: Constructor for the GraphView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GraphView::GraphView()
	{
		ShowScrollBar((HWND)Handle.ToPointer(), SB_HORZ, true);
		ShowScrollBar((HWND)Handle.ToPointer(), SB_VERT, true);

		nodes = new GraphNodeCollection();
		nodes->OnGraphNodeAdded += new GraphNodeAddedDelegate(this, OnGraphNodeAdded);
		nodes->OnGraphNodeRemoved += new GraphNodeRemovedDelegate(this, OnGraphNodeRemoved);

		dispInfo = new Hashtable();

		SetStyle((ControlStyles)(ControlStyles::AllPaintingInWmPaint | ControlStyles::Opaque |
			ControlStyles::UserPaint | ControlStyles::DoubleBuffer | ControlStyles::Selectable |
			ControlStyles::UserMouse), true);

		defaultBackColor = Color::MistyRose;
		defaultForeColor = Color::Black;
		borderPad = 16;
		nodePad = 2;
		colPad = 6;
		rowHeight = 13;

		AdjustScrollBars();
	}

	//*************************************************************************
	// Method:		~GraphView
	// Description: Destructor for the GraphView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	GraphView::~GraphView()
	{
	}

	//*************************************************************************
	// Method:		OnGraphNodeRemoved
	// Description: Called when a node is removed from the graph
	//
	// Parameters:
	//	node - Removed node
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::OnGraphNodeRemoved(GraphNode* node)
	{
		if (dispInfo->ContainsKey(node))
			dispInfo->Remove(node);

		UpdateLayout(false);
	}

	//*************************************************************************
	// Method:		OnGraphNodeAdded
	// Description: Called when a node is added to the graph
	//
	// Parameters:
	//	node - Added node
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::OnGraphNodeAdded(GraphNode* node)
	{
		GraphNodeDisplayInfo* info = new GraphNodeDisplayInfo();
		info->dirty = true;

		dispInfo->Item[node] = info;

		UpdateLayout(false);
	}

	//*************************************************************************
	// Method:		OnPaint
	// Description: Paints the control
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::OnPaint(PaintEventArgs* e)
	{
		int scrollX = GetScrollPosition(SB_HORZ);
		int scrollY = GetScrollPosition(SB_VERT);

		int minX = e->ClipRectangle.Left + scrollX;
		int maxX = e->ClipRectangle.Right + scrollX;
		int minY = e->ClipRectangle.Top + scrollY;
		int maxY = e->ClipRectangle.Bottom + scrollY;

		e->Graphics->FillRectangle(SystemBrushes::Window, e->ClipRectangle);

		IEnumerator* enumerator = nodes->NodeList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			GraphNode* node = dynamic_cast<GraphNode*>(enumerator->Current);
			if (!node)
				continue;

			GraphNodeDisplayInfo* info = dynamic_cast<GraphNodeDisplayInfo*>(dispInfo->Item[node]);
			if (info)
			{
				// Don't draw the node if it's not visible
				if (info->x > maxX)
					continue;
				if ((info->x + info->w) < minX)
					continue;
				if (info->y > maxY)
					continue;
				if ((info->y + info->h) < minY)
					continue;

				DrawNode(e->Graphics, node, info, scrollX, scrollY);
			}
		}
	}

	//*************************************************************************
	// Method:		MeasureNode
	// Description: Gets the size of a node in pixels
	//
	// Parameters:
	//	node - Node to measure
	//  info - Receives measurements
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::MeasureNode(GraphNode* node, GraphNodeDisplayInfo* info)
	{
		Graphics* g = CreateGraphics();

		// Find number of columns
		IEnumerator* enumerator = node->DisplayItems->GetEnumerator();
		int colCount = 1;
		while (enumerator->MoveNext())
		{
			if (__typeof(ContainerListViewItem)->IsInstanceOfType(enumerator->Current))
			{
				ContainerListViewItem* item = dynamic_cast<ContainerListViewItem*>(enumerator->Current);
				if ((item->SubItems->Count + 1) > colCount)
					colCount = item->SubItems->Count + 1;
			}
		}

		// Find column widths and total height
		enumerator = node->DisplayItems->GetEnumerator();
		info->colWidth = new int __gc[colCount];
		info->h = 0;
		while (enumerator->MoveNext())
		{
			if (__typeof(String)->IsInstanceOfType(enumerator->Current))
			{
				String* s = dynamic_cast<String*>(enumerator->Current);
				System::Drawing::SizeF size = g->MeasureString(s, Font);
				if ((int)size.Width > info->colWidth[0])
					info->colWidth[0] = (int)size.Width;
			}
			else if (__typeof(ContainerListViewItem)->IsInstanceOfType(enumerator->Current))
			{
				ContainerListViewItem* item = dynamic_cast<ContainerListViewItem*>(enumerator->Current);
				System::Drawing::SizeF size = g->MeasureString(item->Text, item->ItemFont ? item->ItemFont : Font);
				if ((int)size.Width > info->colWidth[0])
					info->colWidth[0] = (int)size.Width;

				for (int i=0; i<item->SubItems->Count; i++)
				{
					ContainerListViewSubItem* subItem = item->SubItems->Item[i];
					if (subItem->ItemControl)
					{
						if (subItem->ItemControl->Width > info->colWidth[i + 1])
							info->colWidth[i + 1] = subItem->ItemControl->Width;
					}
					else
					{
						size = g->MeasureString(subItem->Text, item->ItemFont ? item->ItemFont : Font);
						if ((int)size.Width > info->colWidth[i + 1])
							info->colWidth[i + 1] = (int)size.Width;
					}
				}
			}

			info->h += rowHeight;
		}

		// Find maximum total width
		enumerator = node->DisplayItems->GetEnumerator();
		info->w = 0;
		for (int i=0; i<info->colWidth->Count; i++)
		{
			if ((i + 1) < info->colWidth->Count)
				info->w += colPad;
			info->w += info->colWidth[i];
		}

		info->w += nodePad * 2 + 2;
		info->h += nodePad * 2 + 2;
	}

	//*************************************************************************
	// Method:		DrawNode
	// Description: Draws a node in the graph
	//
	// Parameters:
	//	node - Node to draw
	//  info - Display parameters for the node
	//  scrollX - Scroll amount in X direction
	//  scrollY - Scroll amount in Y direction
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::DrawNode(Graphics* g, GraphNode* node, GraphNodeDisplayInfo* info, int scrollX, int scrollY)
	{
		g->FillRectangle(new SolidBrush((node->BackColor == Color::Empty) ? defaultBackColor : node->BackColor),
			info->x - scrollX, info->y - scrollY, info->w, info->h);
		g->DrawRectangle(new Pen(new SolidBrush(Color::Black)), info->x - scrollX, info->y - scrollY, info->w, info->h);

		int y = info->y + nodePad + 1 - scrollY;

		IEnumerator* enumerator = node->DisplayItems->GetEnumerator();
		while (enumerator->MoveNext())
		{
			int x = info->x + nodePad + 1 - scrollX;

			if (__typeof(String)->IsInstanceOfType(enumerator->Current))
			{
				String* s = dynamic_cast<String*>(enumerator->Current);
				g->DrawString(s, Font, new SolidBrush((node->ForeColor == Color::Empty) ? defaultForeColor :
					node->ForeColor), (float)x, (float)y);
			}
			else if (__typeof(ContainerListViewItem)->IsInstanceOfType(enumerator->Current))
			{
				ContainerListViewItem* item = dynamic_cast<ContainerListViewItem*>(enumerator->Current);

				Color foreColor = defaultForeColor;
				if (item->ForeColor != Color::Empty)
					foreColor = item->ForeColor;
				else if (node->ForeColor != Color::Empty)
					foreColor = node->ForeColor;

				g->DrawString(item->Text, item->ItemFont ? item->ItemFont : Font, new SolidBrush(foreColor),
					(float)x, (float)y);

				x += info->colWidth[0] + colPad;

				for (int i=0; i<item->SubItems->Count; i++)
				{
					ContainerListViewSubItem* subItem = item->SubItems->Item[i];

					Color foreColor = defaultForeColor;
					if (subItem->ForeColor != Color::Empty)
						foreColor = subItem->ForeColor;
					else if (item->ForeColor != Color::Empty)
						foreColor = item->ForeColor;
					else if (node->ForeColor != Color::Empty)
						foreColor = node->ForeColor;

					if (!subItem->ItemControl)
					{
						g->DrawString(subItem->Text, item->ItemFont ? item->ItemFont : Font, new SolidBrush(foreColor),
							(float)x, (float)y);
					}

					x += info->colWidth[i + 1] + colPad;
				}
			}

			y += rowHeight;
		}
	}

	//*************************************************************************
	// Method:		AdjustScrollBars
	// Description: Adjusts the scoll bars according to the current graph
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::AdjustScrollBars()
	{
		IEnumerator* enumerator = dispInfo->Values->GetEnumerator();
		int minX = Int32::MaxValue;
		int maxX = Int32::MinValue;
		int minY = Int32::MaxValue;
		int maxY = Int32::MinValue;
		bool containsNodes = false;
		while (enumerator->MoveNext())
		{
			GraphNodeDisplayInfo* info = dynamic_cast<GraphNodeDisplayInfo*>(enumerator->Current);
			if (info)
			{
				if (info->x < minX)
					minX = info->x;
				if ((info->x + info->w) > maxX)
					maxX = info->x + info->w;
				if (info->y < minY)
					minY = info->y;
				if ((info->y + info->h) > maxY)
					maxY = info->y + info->h;
				containsNodes = true;
			}
		}

		if (!containsNodes)
			SetScrollRange(0, 0, 0, 0);
		else
			SetScrollRange(minX - borderPad, maxX + borderPad, minY - borderPad, maxY + borderPad);
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
	void GraphView::OnResize(EventArgs* e)
	{
		SetScrollPageSize(ClientSize.Width, ClientSize.Height);
		Control::OnResize(e);
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
	int GraphView::GetScrollPosition(int bar)
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
	void GraphView::SetScrollPosition(int bar, int pos)
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
	int GraphView::GetScrollTrackPosition(int bar)
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
	void GraphView::SetScrollPageSize(int width, int height)
	{
		SCROLLINFO si;

		pageSize.Width = width;
		pageSize.Height = height;

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
	// Method:		SetScrollRange
	// Description: Sets the scroll ranges
	//
	// Parameters:
	//	minX - Minimum X in pixels
	//	maxX - Maximum X in pixels
	//	minY - Minimum Y in pixels
	//	maxY - Maximum Y in pixels
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::SetScrollRange(int minX, int maxX, int minY, int maxY)
	{
		SCROLLINFO si;

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nMin = minX;
		si.nMax = maxX;
		si.nPage = pageSize.Width;
		SetScrollInfo((HWND)Handle.ToPointer(), SB_HORZ, &si, TRUE);

		si.cbSize = sizeof(si);
		si.fMask = SIF_RANGE | SIF_PAGE | SIF_DISABLENOSCROLL;
		si.nMin = minY;
		si.nMax = maxY;
		si.nPage = pageSize.Height;
		SetScrollInfo((HWND)Handle.ToPointer(), SB_VERT, &si, TRUE);
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
	void GraphView::WndProc(Message* msg)
	{
		if (msg->Msg == WM_HSCROLL)
			HScroll(msg->WParam.ToInt32() & 0xffff);
		else if (msg->Msg == WM_VSCROLL)
			VScroll(msg->WParam.ToInt32() & 0xffff);
		else
			Control::WndProc(msg);
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
	void GraphView::HScroll(int type)
	{
		int oldPos = GetScrollPosition(SB_HORZ);

		RECT scrollRect;
		scrollRect.left = 0;
		scrollRect.right = ClientSize.Width;
		scrollRect.top = 0;
		scrollRect.bottom = ClientSize.Height;

		switch (type)
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			SetScrollPosition(SB_HORZ, GetScrollTrackPosition(SB_HORZ));
			ScrollWindow((HWND)Handle.ToPointer(), oldPos - GetScrollPosition(SB_HORZ), 0, &scrollRect, &scrollRect);
			break;
		case SB_LINERIGHT:
			SetScrollPosition(SB_HORZ, oldPos + 1);
			ScrollWindow((HWND)Handle.ToPointer(), oldPos - GetScrollPosition(SB_HORZ), 0, &scrollRect, &scrollRect);
			break;
		case SB_LINELEFT:
			SetScrollPosition(SB_HORZ, oldPos - 1);
			ScrollWindow((HWND)Handle.ToPointer(), oldPos - GetScrollPosition(SB_HORZ), 0, &scrollRect, &scrollRect);
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
			SetScrollPosition(SB_HORZ, Int32::MaxValue);
			Invalidate();
			break;
		case SB_LEFT:
			SetScrollPosition(SB_HORZ, Int32::MinValue);
			Invalidate();
			break;
		}
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
	void GraphView::VScroll(int type)
	{
		int oldPos = GetScrollPosition(SB_VERT);

		RECT scrollRect;
		scrollRect.left = 0;
		scrollRect.right = ClientSize.Width;
		scrollRect.top = 0;
		scrollRect.bottom = ClientSize.Height;

		switch (type)
		{
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
			SetScrollPosition(SB_VERT, GetScrollTrackPosition(SB_VERT));
			ScrollWindow((HWND)Handle.ToPointer(), 0, oldPos - GetScrollPosition(SB_VERT), &scrollRect, &scrollRect);
			break;
		case SB_LINERIGHT:
			SetScrollPosition(SB_VERT, oldPos + 1);
			ScrollWindow((HWND)Handle.ToPointer(), 0, oldPos - GetScrollPosition(SB_VERT), &scrollRect, &scrollRect);
			break;
		case SB_LINELEFT:
			SetScrollPosition(SB_VERT, oldPos - 1);
			ScrollWindow((HWND)Handle.ToPointer(), 0, oldPos - GetScrollPosition(SB_VERT), &scrollRect, &scrollRect);
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
			SetScrollPosition(SB_VERT, Int32::MaxValue);
			Invalidate();
			break;
		case SB_LEFT:
			SetScrollPosition(SB_VERT, Int32::MinValue);
			Invalidate();
			break;
		}
	}

	//*************************************************************************
	// Method:		OnFontChanged
	// Description: Called when the default font changes
	//
	// Parameters:
	//	e - Event args
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::OnFontChanged(EventArgs* e)
	{
		UpdateLayout(true);
		Control::OnFontChanged(e);
	}

	//*************************************************************************
	// Method:		UpdateLayout
	// Description: Updates the graph layout
	//
	// Parameters:
	//	invalidateInfo - Set to true to relayout the entire graph
	//
	// Return Value: None
	//*************************************************************************
	void GraphView::UpdateLayout(bool invalidateInfo)
	{
		if (invalidateInfo)
		{
			IEnumerator* enumerator = dispInfo->Values->GetEnumerator();
			while (enumerator->MoveNext())
			{
				GraphNodeDisplayInfo* info = dynamic_cast<GraphNodeDisplayInfo*>(enumerator->Current);
				if (info)
					info->dirty = true;
			}
		}

		bool changed = false;

		IEnumerator* enumerator = nodes->NodeList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			GraphNode* node = dynamic_cast<GraphNode*>(enumerator->Current);
			if (!node)
				continue;

			GraphNodeDisplayInfo* info = dynamic_cast<GraphNodeDisplayInfo*>(dispInfo->Item[node]);
			if (!info)
				continue;
			if (!info->dirty)
				continue;

			MeasureNode(node, info);

			info->x = 0;
			info->y = 0;

			changed = true;
		}

		if (changed)
			AdjustScrollBars();
	}
}
