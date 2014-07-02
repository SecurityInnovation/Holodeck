//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ContainerListView.h
//
// DESCRIPTION: Contains definition for the class ContainerListView
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
#include "ToggleColumnHeader.h"
#include "ColumnHeaderCollection.h"
#include "ContainerListViewItem.h"
#include "ContainerListViewItemCollection.h"
#include "SelectedContainerListViewItemCollection.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	public __delegate void FilterItemColumnsDelegate(ContainerListViewItem *item);
	public __delegate void SetListViewItemSelectedDelegate(int index, bool selected);
	public __delegate void SetListViewItemFocusedDelegate(int index, bool focused);

	//*********************************************************************
	// Represents an onwer-drawn virtualized multi column list view
	//*********************************************************************
	public __gc class ContainerListView : public Control
	{
	public:
		ContainerListView();
		~ContainerListView();

		LabelEditEventHandler *AfterLabelEdit;
		LabelEditEventHandler *BeforeLabelEdit;
		ColumnClickEventHandler *ColumnClick;
		EventHandler *ItemActivate;
		EventHandler *SelectedIndexChanged;
		SetListViewItemSelectedDelegate *SetListViewItemSelected;
		SetListViewItemFocusedDelegate *SetListViewItemFocused;
		FilterItemColumnsDelegate *FilterItemColumns;

		ContextMenuEventHandler *ContextMenuEvent;
		ItemMenuEventHandler *ItemMenuEvent;
		HeaderMenuEventHandler *HeaderMenuEvent;

		__property bool get_EnsureVisible() { return ensureVisible; }
		__property void set_EnsureVisible(bool value) { ensureVisible = value; }

		__property bool get_CaptureFocusClick() { return captureFocusClick; }
		__property void set_CaptureFocusClick(bool value) { captureFocusClick = value; }

		__property System::Windows::Forms::ContextMenu *get_HeaderMenu() { return headerMenu; }
		__property void set_HeaderMenu(System::Windows::Forms::ContextMenu *value) { headerMenu = value; }

		__property System::Windows::Forms::ContextMenu *get_ItemMenu() { return itemMenu; }
		__property void set_ItemMenu(System::Windows::Forms::ContextMenu *value) { itemMenu = value; }

		__property System::Windows::Forms::ContextMenu *get_ContextMenu() { return contextMenu; }
		__property void set_ContextMenu(System::Windows::Forms::ContextMenu *value) { contextMenu = value; }

		__property ColumnHeaderCollection *get_Columns() { return columns; }

		__property ContainerListViewItemCollection *get_Items() { return items; }

		__property virtual unsigned int get_ItemCount() { return items->ItemCount; }
		__property virtual void set_ItemCount(unsigned int value);

		__property void set_Visible(bool value);
		__property bool get_Visible();

		__property ItemActivation get_Activation() { return activation; }
		__property void set_Activation(ItemActivation value) { activation = value; }

		__property bool get_AllowColumnReorder() { return allowColumnReorder; }
		__property void set_AllowColumnReorder(bool value) { allowColumnReorder = value; }

		__property System::Windows::Forms::BorderStyle get_BorderStyle() { return borderStyle; }
		__property void set_BorderStyle(System::Windows::Forms::BorderStyle value);

		__property ColumnHeaderStyle get_HeaderStyle() { return headerStyle; }
		__property void set_HeaderStyle(ColumnHeaderStyle value);

		__property bool get_ColumnTracking() { return doColTracking; }
		__property void set_ColumnTracking(bool value) { doColTracking = value; }

		__property Color get_ColumnTrackColor() { return colTrackColor; }
		__property void set_ColumnTrackColor(Color value) { colTrackColor = value; }

		__property Color get_ColumnSortColor() { return colSortColor; }
		__property void set_ColumnSortColor(Color value) { colSortColor = value; }

		__property int get_RowHeight() { return rowHeight; }
		__property void set_RowHeight(int value);

		__property bool get_RowTracking() { return doRowTracking; }
		__property void set_RowTracking(bool value) { doRowTracking = value; }

		__property Color get_RowTrackColor() { return rowTrackColor; }
		__property void set_RowTrackColor(Color value) { rowTrackColor = value; }

		__property Color get_RowSelectColor() { return rowSelectColor; }
		__property void set_RowSelectColor(Color value) { rowSelectColor = value; }

		__property bool get_FullRowSelect() { return fullRowSelect; }
		__property void set_FullRowSelect(bool value) { fullRowSelect = value; }

		__property Color get_GridLineColor() { return gridLineColor; }
		__property void set_GridLineColor(Color value) { gridLineColor = value; }

		__property bool get_GridLines() { return gridLines; }
		__property void set_GridLines(bool value);

		__property ImageList *get_SmallImageList() { return smallImageList; }
		__property void set_SmallImageList(ImageList *value);

		__property ImageList *get_StateImageList() { return stateImageList; }
		__property void set_StateImageList(ImageList *value) { stateImageList = value; }

		__property bool get_LabelEdit() { return labelEdit; }
		__property void set_LabelEdit(bool value) { labelEdit = value; }

		__property bool get_HideSelection() { return hideSelection; }
		__property void set_HideSelection(bool value) { hideSelection = value; }

		__property bool get_DisableSelection() { return disableSelection; }
		__property void set_DisableSelection(bool value) { disableSelection = value; }

		__property bool get_FullHighlightOnHideSelection() { return fullHighlightOnHideSelection; }
		__property void set_FullHighlightOnHideSelection(bool value) { fullHighlightOnHideSelection = value; }

		__property bool get_HoverSelection() { return hoverSelection; }
		__property void set_HoverSelection(bool value) { hoverSelection = value; }

		__property bool get_MultiSelect() { return multiSelect; }
		__property void set_MultiSelect(bool value) { multiSelect = value; }

		__property bool get_VisualStyles();
		__property void set_VisualStyles(bool value) { visualStyles = value; }

		__property ArrayList *get_SelectedIndices() { return selectedIndices; }

		__property int get_SelectedIndex();
		__property virtual void set_SelectedIndex(int index);

		__property int get_SelectedRow() { return get_SelectedIndex(); }
		__property virtual void set_SelectedRow(int row);
		
		__property SelectedContainerListViewItemCollection *get_SelectedItems() { return selectedItems; }

		__property bool get_InUpdateTransaction() { return updateTransactions->Count > 0; }

		void BeginUpdate();
		void EndUpdate();
		void MoveToIndex(int index);
		bool IsPointInColumnHeaders(System::Drawing::Point p);

	protected:
		void OnAfterLabelEdit(LabelEditEventArgs *e);
		void OnBeforeLabelEdit(LabelEditEventArgs *e);
		void OnColumnClick(ColumnClickEventArgs *e);
		void OnItemActivate(EventArgs *e);
		void OnSelectedIndexChanged(EventArgs *e);
		virtual void OnSubControlMouseDown(Object *sender, MouseEventArgs *e);
		void OnContextMenuEvent(MouseEventArgs *e);
		void OnItemMenuEvent(MouseEventArgs *e);
		void OnHeaderMenuEvent(MouseEventArgs *e);
		void PopMenu(System::Windows::Forms::ContextMenu *menu, MouseEventArgs *e);
		void OnScroll(Object *sender, EventArgs *e);
		void OnColumnWidthResize(Object *sender, EventArgs *e);

		void OnPaint(PaintEventArgs *e);
		void OnResize(EventArgs *e);
		void WndProc(Message *msg);
		void OnMouseMove(MouseEventArgs *e);
		void OnMouseDown(MouseEventArgs *e);
		void OnMouseUp(MouseEventArgs *e);
		void OnMouseWheel(MouseEventArgs *e);
		void OnKeyDown(KeyEventArgs *e);
		virtual void OnPageKeys(KeyEventArgs *e);
		virtual void OnUpDownKeys(KeyEventArgs *e);
		virtual void OnCheckShiftState(KeyEventArgs *e);
		void OnKeyUp(KeyEventArgs *e);
		void OnGotFocus(EventArgs *e);
		void OnLostFocus(EventArgs *e);

		virtual void Attach();
		virtual void Detach();
		void GenerateColumnRects();
		void GenerateHeaderRect();
		void GenerateRowsRect();
		virtual void AdjustScrollBars();
		void UnselectAll();
		bool MouseInRect(MouseEventArgs *me, System::Drawing::Rectangle rect);
		void MakeSelectedVisible();
		int GetStringWidth(String *s, System::Drawing::Font* font);
		String *TruncatedString(String *s, System::Drawing::Font* font, int width, int offset, Graphics *g);
		void ShowSelectedItems();
		void ShowSelectedItem(int index);
		void SelectiveSelection(int index);
		void CalcSpringWidths(System::Drawing::Rectangle rect);
		virtual void DrawBorder(Graphics *g, System::Drawing::Rectangle r);
		virtual void DrawBackground(Graphics *g, System::Drawing::Rectangle r);
		virtual void DrawHeaders(Graphics *g, System::Drawing::Rectangle r);
		virtual void DrawRows(Graphics *g, System::Drawing::Rectangle r);
		virtual void DrawExtra(Graphics *g, System::Drawing::Rectangle r);

		unsigned int GetTopVisibleIndex();
		unsigned int GetBottomVisibleIndex();

		ItemActivation activation;
		System::Windows::Forms::BorderStyle borderStyle;
		ColumnHeaderStyle headerStyle;
		MultiSelectMode multiSelectMode;
		SortOrder sorting;
		Color colTrackColor;
		Color colSortColor;
		Color rowTrackColor;
		Color rowSelectColor;
		Color gridLineColor;
		Point lastClickedPoint;
		System::Drawing::Rectangle headerRect;
		System::Drawing::Rectangle rowsRect;
		System::Drawing::Rectangle columnRects[];
		System::Drawing::Rectangle columnSizeRects[];

		ColumnHeaderCollection *columns;
		SelectedContainerListViewItemCollection *selectedItems;
		ContainerListViewItemCollection *items;
		ArrayList *selectedIndices;
		ContainerListViewItem *topItem;
		ContainerListViewItem *focusedItem;
		ImageList *smallImageList;
		ImageList *stateImageList;
		Comparer *sortComparer;
		String *text;
		System::Windows::Forms::ContextMenu *headerMenu;
		System::Windows::Forms::ContextMenu *itemMenu;
		System::Windows::Forms::ContextMenu *contextMenu;
		HScrollBar *hScrollBar;
		VScrollBar *vScrollBar;
		Stack *updateTransactions;

		bool allowColumnReorder;
		bool hideSelection;
		bool disableSelection;
		bool fullHighlightOnHideSelection;
		bool hoverSelection;
		bool multiSelect;
		bool labelEdit;
		bool scrollable;
		bool visualStyles;
		bool isFocused;
		bool doColTracking;
		bool colScaleMode;
		bool doRowTracking;
		bool fullRowSelect;
		bool gridLines;
		bool captureFocusClick;
		bool ensureVisible;
		bool visible;
		
		int borderWidth;
		int headerBuffer;
		int focusedIndex;
		int lastColHovered;
		int lastColPressed;
		int lastColSelected;
		int allColsWidth;
		int colScaleWidth;
		int colScalePos;
		int scaledCol;
		int lastRowHovered;
		int rowHeight;
		int allRowsHeight;
		int firstSelected;
		int lastSelected;
		int mouseMoveTicks;
		int vSize;
		int hSize;
		int springWidth;
		int springCount;
		int lastSelectedIndex;
	};
}