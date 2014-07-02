//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SITreeView.h
//
// DESCRIPTION: Contains definition for the class SITreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "TreeListView.h"

namespace SIControls
{
	#define ROW_INDEX_BITS 25
	#define INVALID_ROW_INDEX ((1 << ROW_INDEX_BITS) - 1)

	// possible menu item positions
	public __value enum SITreeViewItemPosition
	{
		First,
		Normal,
		Last
	};

	// some delegates used by the tree view items
	public __delegate void TreeViewItemDoubleClickedDelegate(String *itemName, Object *tag);

	public __value struct SITreeViewItemInfo
	{
		unsigned long rowIndex;
		unsigned char level;
		bool expanded;
		bool selected;
		bool focused;
	};

	// a single menu item
	public __gc struct SITreeViewItem
	{
		SITreeViewItem *parent;
		ArrayList *children;
		SITreeViewItemInfo itemInfo;
		String *name;
		Object *displayObj;
		int imageIndex;
		SITreeViewItemPosition position;
		Object *tag;
		TreeViewItemDoubleClickedDelegate *OnDoubleClick;
	};

	// more item delegates
	public __delegate TreeListNode *ConvertDisplayObjectToTreeListNodeDelegate(SITreeViewItem *item);

	//*********************************************************************
	// A virtualized tree view control that can be dynamically built by string names
	//*********************************************************************
	public __gc class SITreeView : public TreeListView
	{
	public:
		SITreeView();
		~SITreeView();

		ConvertDisplayObjectToTreeListNodeDelegate *OnConvertItemToTreeListNode;

		int AddImageToList(String *fileName);
		int AddImageToList(Bitmap *image);

		void AddTreeViewItem(String *itemName, SITreeViewItemPosition position,
							 Object *displayObject, int imageIndex,
							 TreeViewItemDoubleClickedDelegate *clickedHandler, Object *tag);
		void RemoveTreeViewItem(String *itemName);

		void SetTreeViewItemOptionDisplayObject(String *itemName, Object *displayObject);
		void SetTreeViewItemOptionTag(String *itemName, Object *tag);
		void SetTreeViewItemOptionImageIndex(String *itemName, int imageIndex);

		SITreeViewItem *FindTreeViewItem(String *itemName);
		String *GetItemFullName(SITreeViewItem *item);

		void ExpandItem(SITreeViewItem *item);
		void CollapseItem(SITreeViewItem *item);
		void RemoveItem(SITreeViewItem *item);

		void Refresh();

		void BeginUpdate();
		void EndUpdate();

		__property int get_EmptyImageIndex() { return 0; }

	private:
		bool ProcessName(bool addIfNotFound, String *&itemName, SITreeViewItem *&groupItem, SITreeViewItemPosition groupPosition);
		void AddToItemsList(ArrayList *list, SITreeViewItem *item);

		void InitTreeListNodeDelegates(TreeListNode *node);
		TreeListNode *ConvertToTreeListNode(SITreeViewItem *item);
		int GetChildNodeCount(SITreeViewItem *item);
		int GetVisibleNodeCount(SITreeViewItem *item);
		void UnselectItems(SITreeViewItem *item);

		//virtual treeview delegates
		GetParentNodeDelegate *GetParentNodeHandler;
		GetFirstChildDelegate *GetFirstChildHandler;
		GetLastChildDelegate *GetLastChildHandler;
		GetNextChildDelegate *GetNextChildHandler;
		GetPrevChildDelegate *GetPrevChildHandler;
		GetNextSiblingDelegate *GetNextSiblingHandler;
		GetPrevSiblingDelegate *GetPrevSiblingHandler;
		GetChildNodeAtDelegate *GetChildNodeAtHandler;
		SetIsSelectedDelegate *SetIsSelectedHandler;
		SetIsFocusedDelegate *SetIsFocusedHandler;
		SetIsExpandedDelegate *SetIsExpandedHandler;
		GetRowNumberDelegate *GetRowNumberHandler;
		GetNodeIndexDelegate *GetNodeIndexHandler;
		GetChildNodeCountDelegate *GetChildNodeCountHandler;
		GetVisibleNodeCountDelegate *GetVisibleNodeCountHandler;

		// virtual treeview delegate handlers
		ParentChildList *OnGetParentNode(TreeListNode *node);
		ParentChildList *OnGetFirstChildNode(TreeListNode *node);
		ParentChildList *OnGetLastChildNode(TreeListNode *node);
		ParentChildList *OnGetNextChildNode(TreeListNode *node, TreeListNode *curChild);
		ParentChildList *OnGetPrevChildNode(TreeListNode *node, TreeListNode *curChild);
		TreeListNode *OnGetChildNodeAt(TreeListNode *node, unsigned int index);
		void OnSetIsSelected(TreeListNode *node, bool selected);
		void OnSetIsExpanded(TreeListNode *node, bool expanded);
		void OnSetIsFocused(TreeListNode *node, bool focused);
		ParentChildList *OnGetNextSiblingNode(TreeListNode *node);
		ParentChildList *OnGetPrevSiblingNode(TreeListNode *node);
		int OnGetChildNodeCount(TreeListNode *node);
		int OnGetRowNumber(TreeListNode *node, bool visibleOnly);
		int OnGetNodeIndex(TreeListNode *parent, TreeListNode *child);
		int OnGetVisibleNodeCount(TreeListNode *node);
		void OnTreeListViewUnselectAll();
		TreeListNode *OnTreeListViewQueryItem(unsigned int index);
		TreeListNode *OnTreeListViewQueryRow(unsigned int row);
		void OnListViewDoubleClick(Object *sender, EventArgs *args);

		SITreeViewItem *rootItem;
		TreeListNode *root;
		ArrayList *items;

		int updateModeCount;
	};
}