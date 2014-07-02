//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TreeListView.h
//
// DESCRIPTION: Contains definition for the class TreeListView
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
#using <SiIconLib.dll>

#include <stdio.h>
#include "SelectedTreeListNodeCollection.h"
#include "ContainerListView.h"
#include "TreeListNode.h"

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Reflection;
using namespace System::Collections::Specialized;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace SiIconLib;

namespace SIControls
{
	public __delegate void FilterNodeColumnsDelegate(TreeListNode *item);
	public __delegate TreeListNode *QueryTreeListNodeDelegate(unsigned int index);
	public __delegate TreeListNode *QueryTreeListNodeAtRowDelegate(unsigned int row);
	public __delegate void UnselectAllNodesDelegate();

	//*********************************************************************
	// A tree control where the nodes can be multi column list view items
	//*********************************************************************
	public __gc class TreeListView : public ContainerListView
	{
	public:
		EventHandler *SelectedItemChanged;
		QueryTreeListNodeDelegate *QueryTreeListNode;
		QueryTreeListNodeAtRowDelegate *QueryTreeListNodeAtRow;
		FilterNodeColumnsDelegate *FilterNodeColumns;
		UnselectAllNodesDelegate *UnselectAllNodes;

		TreeListView();
		~TreeListView();

		void CollapseAll();
		void ExpandAll();
		SelectedTreeListNodeCollection *GetSelectedNodes(TreeListNode *node);

		__property SelectedTreeListNodeCollection *get_SelectedNodes();
		__property virtual void set_SelectedIndex(int index);
		__property virtual void set_SelectedNode(TreeListNode *node);
		__property virtual void set_SelectedRow(int index);

		__property int get_VisibleNodeCount();

		__property TreeListNode *get_RootNode() { return virtualParent; }
		__property void set_RootNode(TreeListNode *value);

		__property bool get_UseVisualStyle() { return visualStyle; }
		__property void set_UseVisualStyle(bool value) { visualStyle = value; }

		__property bool get_MouseActivate() { return mouseActivate; }
		__property void set_MouseActivate(bool value) { mouseActivate = value; }

		__property bool get_AlwaysShowPlusMinus() { return alwaysShowPM; }
		__property void set_AlwaysShowPlusMinus(bool value) { alwaysShowPM = value; }

		__property int get_Indent() { return indent; }
		__property void set_Indent(int value) { indent = value; }

		__property virtual unsigned int get_ItemCount() { return totalNodeCount; }
		__property virtual void set_ItemCount(unsigned int value);

		__property bool get_ShowLines() { return showLines; }
		__property void set_ShowLines(bool value) { showLines = value; }

		__property bool get_ShowRootLines() { return showRootLines; }
		__property void set_ShowRootLines(bool value) { showRootLines = value; }

		__property bool get_ShowPlusMinus() { return showPlusMinus; }
		__property void set_ShowPlusMinus(bool value) { showPlusMinus = value; }

		__property bool get_FullBackground() { return fullBackground; }
		__property void set_FullBackground(bool value) { fullBackground = value; }

		__property Keys get_ExpandAllKey() { return expandAllKey; }
		__property void set_ExpandAllKey(Keys value) { expandAllKey = value; }

		bool GetPriorNode(TreeListNode **curNode);
		bool GetNextNode(TreeListNode **curNode);
		void GetCurrentNode(TreeListNode **curNode) { *curNode = currNode; }

	protected:
		void OnSubControlMouseDown(Object *sender, MouseEventArgs *e);
		virtual void OnNodesChanged(Object *sender, EventArgs *e);
		void OnSelectedItemChanged();
		void OnSelectedIndexChanged();

		void MakeSelectedVisible();
		void OnKeyDown(KeyEventArgs *e);
		void OnLeftRightKeys(KeyEventArgs *e);
		void OnUpDownKeys(KeyEventArgs *e);
		void ShowSelectedItems();
		void OnResize(EventArgs *e);
		void OnMouseDown(MouseEventArgs *e);
		void OnMouseDownColumns(MouseEventArgs *e);
		void OnMouseDownBody(MouseEventArgs *e);
		void OnKeyUp(KeyEventArgs *e);
		void DrawRows(Graphics *g, System::Drawing::Rectangle r);
		void Dispose(bool disposing);
		void AdjustScrollBars();
		void AutoSetColWidth(TreeListNode *node, int *mWid, int *tWid, int i);
		void UnfocusNodes(TreeListNode *node);
		void UnselectNodes(TreeListNode *node);
		TreeListNode *NodeInNodeRow(MouseEventArgs *e);
		TreeListNode *NodeAtYPos(int yPos);
		TreeListNode *NodePlusClicked(MouseEventArgs *e);
		void RenderNodeRows(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int *totalRend);
		int GetIconBufferForRowRender(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int *totalRend, int lb, int eb, int hb);
		void RenderPerItemBackground(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int *totalRend, int ib, int lb, int eb, int hb, int colWidth);
		void RenderItemSelectionAndFocus(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r);
		void RenderRootLines(TreeListNode *node, Graphics *g, Pen *linePen, System::Drawing::Rectangle r, int eb, int hb, int level, int index, int count, int childCount, int *totalRend);
		void RenderChildLines(TreeListNode *node, Graphics *g, Pen *linePen, System::Drawing::Rectangle r, int lb, int eb, int hb, int level, int index, int count, int childCount, int *totalRend);
		void RenderPlusAndMinusLines(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int eb, int lb, int ib, int hb, int *totalRend);
		void RenderSubItems(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int hb, int *totalRend);
		void RenderPlus(Graphics *g, int x, int y, int w, int h, TreeListNode *node);
		int FirstNodeRelativeToCurrentNode();
		void DrawBackground(Graphics *g, System::Drawing::Rectangle r);

		ListDictionary *pmRects;
		ListDictionary *nodeRowRects;
		Bitmap *minusBitmap;
		Bitmap *plusBitmap;
		TreeListNode *currNode;
		TreeListNode *firstSelectedNode;
		TreeListNode *virtualParent;
		System::ComponentModel::Container *components;
		QueryTreeListNodeDelegate *treeListNodeQueryHandler;
		Keys expandAllKey;
		
		int indent;
		int vSize;
		int hSize;
		int rendCount;
		unsigned int totalNodeCount;

		bool showLines;
		bool showRootLines;
		bool showPlusMinus;
		bool alwaysShowPM;
		bool mouseActivate;
		bool allCollapsed;
		bool visualStyle;
		bool fullBackground;
	};
}