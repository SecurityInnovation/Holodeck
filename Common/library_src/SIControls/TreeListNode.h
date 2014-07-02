//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TreeListNode.h
//
// DESCRIPTION: Contains definition for the class TreeListNode
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

#include "ParentChildList.h"
#include "ContainerListViewSubItemCollection.h"
#include "TreeListNodeCollection.h"

using namespace System;
using namespace System::Drawing;

namespace SIControls
{
	public __delegate ParentChildList *GetParentNodeDelegate(TreeListNode *node);
	public __delegate ParentChildList *GetFirstChildDelegate(TreeListNode *node);
	public __delegate ParentChildList *GetLastChildDelegate(TreeListNode *node);
	public __delegate ParentChildList *GetNextChildDelegate(TreeListNode *node, TreeListNode *curChild);
	public __delegate ParentChildList *GetPrevChildDelegate(TreeListNode *node, TreeListNode *curChild);
	public __delegate ParentChildList *GetNextSiblingDelegate(TreeListNode *node);
	public __delegate ParentChildList *GetPrevSiblingDelegate(TreeListNode *node);
	public __delegate TreeListNode *GetChildNodeAtDelegate(TreeListNode *node, unsigned int index);

	public __delegate void SetIsExpandedDelegate(TreeListNode *node, bool expanded);
	public __delegate void SetIsSelectedDelegate(TreeListNode *node, bool selected);
	public __delegate void SetIsFocusedDelegate(TreeListNode *node, bool focused);

	public __delegate int GetRowNumberDelegate(TreeListNode *node, bool visibleOnly);
	public __delegate int GetNodeIndexDelegate(TreeListNode *parent, TreeListNode *child);
	
	public __delegate int GetChildNodeCountDelegate(TreeListNode *node);
	public __delegate int GetVisibleNodeCountDelegate(TreeListNode *node);

	//*********************************************************************
	// Contains a single node of a tree in the TreeListView
	//*********************************************************************
	public __gc class TreeListNode : public ParentChildList
	{
	public:
		MouseEventHandler *MouseDown;

		GetParentNodeDelegate *GetParentNode;
		GetFirstChildDelegate *GetFirstChild;
		GetLastChildDelegate *GetLastChild;
		GetNextChildDelegate *GetNextChild;
		GetPrevChildDelegate *GetPrevChild;
		GetNextSiblingDelegate *GetNextSibling;
		GetPrevSiblingDelegate *GetPrevSibling;
		GetChildNodeAtDelegate *GetChildNodeAt;

		SetIsExpandedDelegate *SetIsExpanded;
		SetIsSelectedDelegate *SetIsSelected;
		SetIsFocusedDelegate *SetIsFocused;

		GetRowNumberDelegate *GetRowNumber;
		GetNodeIndexDelegate *GetNodeIndex;

		GetChildNodeCountDelegate *GetChildNodeCount;
		GetVisibleNodeCountDelegate *GetVisibleNodeCount;

		TreeListNode();
		~TreeListNode();

		__property bool get_IsExpanded() { return expanded; }
		__property void set_IsExpanded(bool value);

		__property bool get_Focused() { return focused; }
		__property void set_Focused(bool value);

		__property bool get_Selected() { return selected; }
		__property void set_Selected(bool value);

		__property bool get_IsVisible() { return visible; }
		__property void set_IsVisible(bool value) { visible = value; }

		__property int get_DescendentsCount() { return (GetChildNodeCount ? GetChildNodeCount->Invoke(this) : 0); }

		__property String *get_FullPath() { return fullPath; }

		__property Color get_BackColor() { return backColor; }
		__property void set_BackColor(Color value) { backColor = value; }

		__property Font *get_Font() { return font; }
		__property void set_Font(System::Drawing::Font *value) { font = value; }

		__property Color get_ForeColor() { return foreColor; }
		__property void set_ForeColor(Color value) { foreColor = value; }

		__property int get_ImageIndex() { return imageIndex; }
		__property void set_ImageIndex(int value) { imageIndex = value; }

		__property int get_Index() { return index; }
		__property void set_Index(int value) { index = value; }

		__property ContainerListViewSubItemCollection *get_SubItems() { return subItems; }

		__property int get_StateImageIndex() { return stateImageIndex; }
		__property void set_StateImageIndex(int value) { stateImageIndex = value; }

		__property Object *get_Tag() { return tag; }
		__property void set_Tag(Object *value) { tag = value; }

		__property String *get_Text() { return text; }
		__property void set_Text(String *value) { text = value; }

		__property bool get_UseItemStyleForSubItems() { return styleAll; }
		__property void set_UseItemStyleForSubItems(bool value) { styleAll = value; }

		__property bool get_Hovered() { return hovered; }
		__property void set_Hovered(bool value) { hovered = value; }

		__property bool get_IsRoot() { return isRoot; }
		__property void set_IsRoot(bool value) { isRoot = value; }

		__property int get_ChildrenCount();

		__property int get_VisibleNodeCount();

		void Collapse();
		void CollapseAll();
		void Expand();
		void ExpandAll();
		void Toggle();
		bool IsNodeAt(int firstVisibleYPixel, int yOffset, int rowHeight);
		bool IsNodeAt(int row, int prior);
		bool GetNodeAt(int firstVisibleYPixel, int yOffset, int rowHeight, TreeListNode **node);
		bool GetNodeAt(int row, int prior, TreeListNode **node);
		int Level();
		int RowNumber(bool visibleOnly);

		ParentChildList *ParentNode();
		ParentChildList *PreviousSibling();
		ParentChildList *NextSibling();
		ParentChildList *FirstChild();
		ParentChildList *LastChild();
		ParentChildList *NextChild();
		ParentChildList *PreviousChild();

		int CompareTo(TreeListNode *comparisonNode);
		void GetStackToVirtualParent(Stack *stack);
		ListViewItem *GetListViewItem();

	protected:
		void OnSubItemsChanged(Object *sender, ItemsChangedEventArgs *e);
		void OnSubItemMouseDown(Object *sender, MouseEventArgs *e);
		void OnSubNodeMouseDown(Object *sender, MouseEventArgs *e);

		void GetStackToVirtualParent(Stack *stack, ParentChildList *node);

		bool IsAfter(TreeListNode *node);
		bool IsBefore(TreeListNode *node);

		//int descendentsCount;
		//int descendentsVisibleCount;
		int imageIndex;
		int stateImageIndex;
		int index;

		bool isRoot;
		bool selected;
		bool focused;
		bool styleAll;
		bool hovered;
		bool expanded;
		bool visible;

		Color backColor;
		Color foreColor;

		System::Drawing::Font *font;
		ContainerListViewSubItemCollection *subItems;
		Object *tag;
		String *text;
		TreeListNode *currChild;
		String *fullPath;
	};
}