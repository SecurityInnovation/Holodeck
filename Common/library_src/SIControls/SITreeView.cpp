//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		SITreeView.cpp
//
// DESCRIPTION: Contains implementation for the class SITreeView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#include "SITreeView.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SITreeView
	// Description: Constructor for the SITreeView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SITreeView::SITreeView()
	{
		rootItem = new SITreeViewItem;
		rootItem->displayObj = S"Root";
		rootItem->children = new ArrayList();
		rootItem->name = "Root";
		rootItem->tag = NULL;
		rootItem->itemInfo.expanded = true;
		rootItem->itemInfo.selected = false;
		rootItem->itemInfo.focused = false;

		this->SmallImageList = new ImageList();
		// add an empty item
		this->SmallImageList->Images->Add(new System::Drawing::Bitmap(1, 1));

		items = new ArrayList();

		// virtual tree view delegate handlers
		GetParentNodeHandler = new GetParentNodeDelegate(this, OnGetParentNode);
		GetFirstChildHandler = new GetFirstChildDelegate(this, OnGetFirstChildNode);
		GetLastChildHandler = new GetLastChildDelegate(this, OnGetLastChildNode);
		GetNextChildHandler = new GetNextChildDelegate(this, OnGetNextChildNode);
		GetPrevChildHandler = new GetPrevChildDelegate(this, OnGetPrevChildNode);
		GetNextSiblingHandler = new GetNextSiblingDelegate(this, OnGetNextSiblingNode);
		GetPrevSiblingHandler = new GetPrevSiblingDelegate(this, OnGetPrevSiblingNode);
		GetChildNodeAtHandler = new GetChildNodeAtDelegate(this, OnGetChildNodeAt);
		SetIsSelectedHandler = new SetIsSelectedDelegate(this, OnSetIsSelected);
		SetIsExpandedHandler = new SetIsExpandedDelegate(this, OnSetIsExpanded);
		SetIsFocusedHandler = new SetIsFocusedDelegate(this, OnSetIsFocused);
		GetRowNumberHandler = new GetRowNumberDelegate(this, OnGetRowNumber);
		GetNodeIndexHandler = new GetNodeIndexDelegate(this, OnGetNodeIndex);
		GetChildNodeCountHandler = new GetChildNodeCountDelegate(this, OnGetChildNodeCount);
		GetVisibleNodeCountHandler = new GetVisibleNodeCountDelegate(this, OnGetVisibleNodeCount);

		DoubleClick += new EventHandler(this, OnListViewDoubleClick);
		QueryTreeListNode += new QueryTreeListNodeDelegate(this, OnTreeListViewQueryItem);
		QueryTreeListNodeAtRow += new QueryTreeListNodeAtRowDelegate(this, OnTreeListViewQueryRow);
		UnselectAllNodes += new UnselectAllNodesDelegate(this, OnTreeListViewUnselectAll);

		root = ConvertToTreeListNode(rootItem);
		root->IsRoot = true;
		this->RootNode = root;
	}

	//*************************************************************************
	// Method:		~SITreeView
	// Description: Destructor for the SITreeView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SITreeView::~SITreeView()
	{
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the treeview's image list
	//
	// Parameters:
	//	fileName - the name of the file that holds the image
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int SITreeView::AddImageToList(String *fileName)
	{
		try
		{
			Bitmap *bitmap = new Bitmap(fileName);
			return this->SmallImageList->Images->Add(bitmap, Color::Empty);
		}
		catch(...)
		{
		}

		return -1;
	}

	//*************************************************************************
	// Method:		AddImageToList
	// Description: Adds an image to the treeview's image list
	//
	// Parameters:
	//	image - image to add
	//
	// Return Value: the index of the item added in the list
	//*************************************************************************
	int SITreeView::AddImageToList(Bitmap *image)
	{
		return this->SmallImageList->Images->Add(image, Color::Empty);
	}

	//*************************************************************************
	// Method:		AddTreeViewItem
	// Description: Adds a tree view item to the control
	//
	// Parameters:
	//	itemName - the internal name for the item
	//	position - the relative position to add the item in the group
	//	displayObject - the object to display as the tree view item data
	//	imageIndex - the index of the image to use for the item
	//	doubleClickedHandler - the delegate to invoke when the item is clicked
	//	tag - any user defined object you want
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::AddTreeViewItem(String *itemName, SITreeViewItemPosition position,
						Object *displayObject, int imageIndex,
						TreeViewItemDoubleClickedDelegate *doubleClickedHandler, Object *tag)
	{
		SITreeViewItem* groupItem;

		ProcessName(true, itemName, groupItem, SITreeViewItemPosition::Normal);

		// Try to find an existing item by this name
		for (int i = 0; i < groupItem->children->Count; i++)
		{
			SITreeViewItem* item = dynamic_cast<SITreeViewItem*>(groupItem->children->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, update info and exit
				item->displayObj = displayObject;
				item->position = position;
				item->itemInfo.selected = false;
				item->itemInfo.focused = false;
				item->imageIndex = imageIndex;
				item->OnDoubleClick = doubleClickedHandler;
				item->tag = tag;

				if (updateModeCount == 0)
					Refresh();
				return;
			}
		}

		// No item by this name, add it to the list
		SITreeViewItem* item = new SITreeViewItem;
		item->name = itemName;
		item->displayObj = displayObject;
		item->position = position;
		item->itemInfo.selected = false;
		item->itemInfo.focused = false;
		item->itemInfo.expanded = false;
		item->imageIndex = imageIndex;
		item->OnDoubleClick = doubleClickedHandler;
		item->tag = tag;
		item->children = new ArrayList();
		AddToItemsList(groupItem->children, item);

		if (updateModeCount == 0)
			Refresh();
	}

	//*************************************************************************
	// Method:		RemoveTreeViewItem
	// Description: removes an item and all subitems from the tree view
	//
	// Parameters:
	//	itemName - the name of the item to remove
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::RemoveTreeViewItem(String *itemName)
	{
		try
		{
			SITreeViewItem *item = FindTreeViewItem(itemName);
			if (item)
				RemoveItem(item);
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		SetTreeViewItemOptionDisplayObject
	// Description: changes the display object for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the display object for
	//	displayObject - the object to set as the displayed tree view item data
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::SetTreeViewItemOptionDisplayObject(String *itemName, Object *displayObject)
	{
		SITreeViewItem* item = FindTreeViewItem(itemName);
		if (item)
		{
			item->displayObj = displayObject;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetTreeViewItemOptionTag
	// Description: changes the user-defined tag object for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	tag - the user-defined object to assign to the tree view item
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::SetTreeViewItemOptionTag(String *itemName, Object *tag)
	{
		SITreeViewItem* item = FindTreeViewItem(itemName);
		if (item)
		{
			item->tag = tag;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		SetTreeViewItemOptionImageIndex
	// Description: changes the image for an existing item
	//
	// Parameters:
	//	itemName - the name of the item to set the tag for
	//	imageIndex - the image to assign to the tree view item
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::SetTreeViewItemOptionImageIndex(String *itemName, int imageIndex)
	{
		SITreeViewItem* item = FindTreeViewItem(itemName);
		if (item)
		{
			item->imageIndex = imageIndex;
			Refresh();
		}
	}

	//*************************************************************************
	// Method:		FindTreeViewItem
	// Description: returns an item with the specified name
	//
	// Parameters:
	//	itemName - the name of the item to find
	//
	// Return Value: the found item, or NULL if not found
	//*************************************************************************
	SITreeViewItem *SITreeView::FindTreeViewItem(String *itemName)
	{
		SITreeViewItem* groupItem;

		if (!ProcessName(false, itemName, groupItem, SITreeViewItemPosition::Normal))
			return NULL;

		// Try to find an existing item by this name
		for (int i = 0; i < groupItem->children->Count; i++)
		{
			SITreeViewItem* item = dynamic_cast<SITreeViewItem*>(groupItem->children->Item[i]);
			if (item->name->Equals(itemName))
			{
				// Found, return to caller
				return item;
			}
		}

		return NULL;
	}
	
	//*************************************************************************
	// Method:		GetItemFullName
	// Description: gets the full name of a specified item
	//
	// Parameters:
	//	item - the item to get the name of
	//
	// Return Value: the full name of the specified item
	//*************************************************************************
	String *SITreeView::GetItemFullName(SITreeViewItem *item)
	{
		String* name = item->name;

		if (item->parent)
		{
			String* groupName = GetItemFullName(item->parent);
			if (groupName && !groupName->Equals("Root"))
			{
				name = String::Concat(groupName, S"->", name);
			}
		}

		return name;
	}

	//*************************************************************************
	// Method:		Refresh
	// Description: Updates the states of all items in the tree view control
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::Refresh()
	{
		if (updateModeCount == 0)
		{
			// update the item count
			this->ItemCount = GetVisibleNodeCount(rootItem);
		}

		TreeListView::Refresh();
	}

	//*************************************************************************
	// Method:		ProcessName
	// Description: parses a tree view item name and processes it
	//
	// Parameters:
	//	addIfNotFound - if true and the name is not found, an item will be added
	//	itemName - the name of the item to process
	//	groupItem - the group of the item to process
	//	position - the relative position of the item to process
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SITreeView::ProcessName(bool addIfNotFound, String *&itemName, SITreeViewItem *&groupItem, SITreeViewItemPosition groupPosition)
	{
		groupItem = rootItem;

		// add all the group nodes
		while (itemName->IndexOf("->") != -1)
		{
			String* subName = itemName->Substring(0, itemName->IndexOf("->"));
			itemName = itemName->Substring(itemName->IndexOf("->") + 2);

			String* groupName = subName;

			bool found = false;
			SITreeViewItem* parent;
			for (int i = 0; i < groupItem->children->Count; i++)
			{
				SITreeViewItem* item = dynamic_cast<SITreeViewItem*>(groupItem->children->Item[i]);
				if (item->name->Equals(subName))
				{
					parent = item;
					found = true;
					break;
				}
			}

			if (!found)
			{
				if (!addIfNotFound)
					return false;

				SITreeViewItem* item = new SITreeViewItem;
				item->name = subName;
				item->displayObj = subName;
				item->position = SITreeViewItemPosition::Normal;
				item->itemInfo.expanded = false;
				item->parent = groupItem;
				item->children = new ArrayList();
				AddToItemsList(groupItem->children, item);
				groupItem = item;
			}
			else
				groupItem = parent;
		}

		// now add the leaf node if necessary
		bool found = false;
		for (int i = 0; i < groupItem->children->Count; i++)
		{
			SITreeViewItem* item = dynamic_cast<SITreeViewItem*>(groupItem->children->Item[i]);
			if (item->name->Equals(itemName))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			if (!addIfNotFound)
				return false;

			SITreeViewItem* item = new SITreeViewItem;
			item->name = itemName;
			item->displayObj = itemName;
			item->position = SITreeViewItemPosition::Normal;
			item->itemInfo.expanded = false;
			item->parent = groupItem;
			item->children = new ArrayList();
			AddToItemsList(groupItem->children, item);
		}

		return true;
	}

	//*************************************************************************
	// Method:		AddToItemsList
	// Description: Adds a tree view item to an internal list
	//
	// Parameters:
	//	list - the list to add the item to
	//	item - the item to add to the list
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::AddToItemsList(ArrayList *list, SITreeViewItem *item)
	{
		int insertPos = 0;
		int i = 0;
		for (i = 0; i < list->Count; i++)
		{
			SITreeViewItem* curItem = dynamic_cast<SITreeViewItem*>(list->Item[i]);
			bool insertNow = false;
			switch (item->position)
			{
			case SITreeViewItemPosition::First:
				// Insert before all "normal" and "last" items
				if ((curItem->position == SITreeViewItemPosition::Normal) || (curItem->position == SITreeViewItemPosition::Last))
					insertNow = true;
				break;
			case SITreeViewItemPosition::Normal:
				// Insert before all "last" items
				if (curItem->position == SITreeViewItemPosition::Last)
					insertNow = true;
				break;
			case SITreeViewItemPosition::Last:
				// Always at end, never break out early
				break;
			}
			if (insertNow)
				break;

			insertPos += GetVisibleNodeCount(curItem);
		}
		
		int parentPos = 0;
		if (item->parent)
			parentPos = item->parent->itemInfo.rowIndex;

		list->Insert(i, item);

		// now add to the global list
		if (item->parent && item->parent->itemInfo.expanded)
		{
			item->itemInfo.rowIndex = parentPos + insertPos + 1;
			if ((unsigned int)items->Count >= item->itemInfo.rowIndex)
			{
				items->Insert(item->itemInfo.rowIndex, item);

				for (unsigned int i = item->itemInfo.rowIndex + 1; i < (unsigned int)items->Count; i++)
				{
					SITreeViewItem *currItem = dynamic_cast<SITreeViewItem *>(items->get_Item(i));
					if (!currItem)
						continue;
					currItem->itemInfo.rowIndex++;
				}
			}
			else
			{
				item->itemInfo.rowIndex = items->Add(item);
			}
		}
		else
			item->itemInfo.rowIndex = INVALID_ROW_INDEX;
	}

	//*************************************************************************
	// Method:		InitTreeListNodeDelegates
	// Description: attaches delegates to a treelistnode item
	//
	// Parameters:
	//	node - the node to attach the delegates to
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::InitTreeListNodeDelegates(TreeListNode *node)
	{
		node->GetParentNode += GetParentNodeHandler;
		node->GetFirstChild += GetFirstChildHandler;
		node->GetLastChild += GetLastChildHandler;
		node->GetNextChild += GetNextChildHandler;
		node->GetPrevChild += GetPrevChildHandler;
		node->GetNextSibling += GetNextSiblingHandler;
		node->GetPrevSibling += GetPrevSiblingHandler;
		node->GetChildNodeAt += GetChildNodeAtHandler;
		node->SetIsExpanded += SetIsExpandedHandler;
		node->SetIsSelected += SetIsSelectedHandler;
		node->SetIsFocused += SetIsFocusedHandler;
		node->GetRowNumber += GetRowNumberHandler;
		node->GetNodeIndex += GetNodeIndexHandler;
		node->GetChildNodeCount += GetChildNodeCountHandler;
		node->GetVisibleNodeCount += GetVisibleNodeCountHandler;
	}

	//*************************************************************************
	// Method:		ConvertToTreeListNode
	// Description: converts an SITreeViewItem to a TreeListNode
	//
	// Parameters:
	//	item - the item to convert
	//
	// Return Value: the converted treelistnode object, or null
	//*************************************************************************
	TreeListNode *SITreeView::ConvertToTreeListNode(SITreeViewItem *item)
	{
		if (!item)
			return NULL;

		if (item == rootItem)
		{
			TreeListNode *retVal = new TreeListNode();
			retVal->Text = "Root";
			retVal->IsExpanded = item->itemInfo.expanded;
			retVal->Selected = item->itemInfo.selected;
			retVal->Focused = item->itemInfo.focused;
			retVal->IsVisible = true;
			retVal->Tag = item;
			InitTreeListNodeDelegates(retVal);
			return retVal;
		}

		TreeListNode *retVal = NULL;
		if (OnConvertItemToTreeListNode)
			retVal = OnConvertItemToTreeListNode->Invoke(item);

		else if (item->displayObj)
		{
			retVal = new TreeListNode();
			retVal->Text = item->displayObj->ToString();
		}

		if (retVal)
		{
			retVal->IsVisible = true;
			retVal->IsExpanded = item->itemInfo.expanded;
			retVal->Selected = item->itemInfo.selected;
			retVal->Focused = item->itemInfo.focused;
			retVal->ImageIndex = item->imageIndex;
			retVal->UseItemStyleForSubItems = true;
			retVal->Tag = item;
			InitTreeListNodeDelegates(retVal);
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		GetChildNodeCount
	// Description: recursively gets the number of child nodes of a particular node
	//
	// Parameters:
	//	item - the item to get the child count of
	//
	// Return Value: the child count of the node, 0 if failed
	//*************************************************************************
	int SITreeView::GetChildNodeCount(SITreeViewItem *item)
	{
		int result = 0;

		if (item && item->children)
		{
			for (unsigned int i = 0; i < (unsigned int)item->children->Count; i++)
			{
				SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(i));
				if (!childItem)
					continue;

				result += GetChildNodeCount(childItem) + 1;
			}
		}

		return result;
	}

	//*************************************************************************
	// Method:		GetVisibleNodeCount
	// Description: recursively gets the number of visible child nodes of a particular node
	//
	// Parameters:
	//	item - the item to get the visible child count of
	//
	// Return Value: the visible child count of the node, 0 if failed
	//*************************************************************************
	int SITreeView::GetVisibleNodeCount(SITreeViewItem *item)
	{
		int result = 1;

		if (item && item->children && item->itemInfo.expanded)
		{
			for (unsigned int i = 0; i < (unsigned int)item->children->Count; i++)
			{
				SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(i));
				if (!childItem)
					continue;

				result += GetVisibleNodeCount(childItem);
			}
		}

		return result;
	}

	//*************************************************************************
	// Method:		UnselectItems
	// Description: recursively unselects child nodes of a particular node
	//
	// Parameters:
	//	item - the item to unselect the children of
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::UnselectItems(SITreeViewItem *item)
	{
		if (item && item->children)
		{
			for (unsigned int i = 0; i < (unsigned int)item->children->Count; i++)
			{
				SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(i));
				if (!childItem)
					continue;

				childItem->itemInfo.selected = false;
				childItem->itemInfo.focused = false;
				UnselectItems(childItem);
			}
		}
	}

	//*************************************************************************
	// Method:		ExpandItem
	// Description: expands a particular node
	//
	// Parameters:
	//	item - the item to expand
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::ExpandItem(SITreeViewItem *item)
	{
		try
		{
			if (item->itemInfo.expanded)
				return;

			item->itemInfo.expanded = true;
			
			unsigned int childCount = 0;
			if (item->children)
				childCount = (unsigned int)item->children->Count;

			for (unsigned int i = item->itemInfo.rowIndex + 1; i < (unsigned int)items->Count; i++)
			{
				SITreeViewItem *currItem = dynamic_cast<SITreeViewItem *>(items->get_Item(i));
				if (!currItem)
					continue;
				currItem->itemInfo.rowIndex += childCount;
			}

			for (unsigned int i = 0; i < childCount; i++)
			{
				SITreeViewItem *currChild = dynamic_cast<SITreeViewItem *>(item->children->get_Item(i));
				if (!currChild)
					continue;

				currChild->itemInfo.rowIndex = item->itemInfo.rowIndex + i + 1;
				items->Insert(item->itemInfo.rowIndex + i + 1, currChild);
			}

			if (updateModeCount == 0)
				Refresh();
		}
		catch(...)
		{
		}
	}
	
	//*************************************************************************
	// Method:		CollapseItem
	// Description: recursively collapses a particular node and its children
	//
	// Parameters:
	//	item - the item to collapse
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::CollapseItem(SITreeViewItem *item)
	{
		try
		{
			if (!item->itemInfo.expanded)
				return;

			if (!item->children)
				return;

			unsigned int childCount = (unsigned int)item->children->Count;

			// collapse children that are still expanded
			for (unsigned int i = 0; i < childCount; i++)
			{
				SITreeViewItem *currChild = dynamic_cast<SITreeViewItem *>(item->children->get_Item(i));
				if (!currChild)
					continue;

				if (currChild->itemInfo.expanded)
					CollapseItem(currChild);
				currChild->itemInfo.rowIndex = INVALID_ROW_INDEX;
			}

			item->itemInfo.expanded = false;

			// update row indices
			for (unsigned int i = item->itemInfo.rowIndex + childCount + 1; i < (unsigned int)items->Count; i++)
			{
				SITreeViewItem *curr = dynamic_cast<SITreeViewItem *>(items->get_Item(i));
				if (!curr)
					continue;
				curr->itemInfo.rowIndex -= childCount;
			}

			// remove the children
			for (unsigned int i = 0; i < childCount; i++)
			{
				SITreeViewItem *currChild = dynamic_cast<SITreeViewItem *>(item->children->get_Item(i));
				if (!currChild)
					continue;
				items->Remove(currChild);
			}

			if (updateModeCount == 0)
				Refresh();
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		RemoveItem
	// Description: removes an item and all subitems from the treeview
	//
	// Parameters:
	//	node - the node to get the parent of
	//
	// Return Value: the parent of the node, NULL if failed
	//*************************************************************************
	void SITreeView::RemoveItem(SITreeViewItem *item)
	{
		try
		{
			// remove children
			while (item->children && (item->children->Count > 0))
			{
				SITreeViewItem *currChild = dynamic_cast<SITreeViewItem *>(item->children->get_Item(0));
				if (!currChild)
					continue;

				RemoveItem(currChild);
				currChild->itemInfo.rowIndex = INVALID_ROW_INDEX;
			}

			items->Remove(item);
			if (item->parent && item->parent->children)
				item->parent->children->Remove(item);

			if (item->itemInfo.rowIndex != INVALID_ROW_INDEX)
			{
				// update row indices
				for (unsigned int i = item->itemInfo.rowIndex + 1; i < (unsigned int)items->Count; i++)
				{
					SITreeViewItem *curr = dynamic_cast<SITreeViewItem *>(items->get_Item(i));
					if (!curr)
						continue;
					curr->itemInfo.rowIndex--;
				}
			}

			if (updateModeCount == 0)
				Refresh();
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnGetParentNode
	// Description: called when the pane needs to get a node's parent node
	//
	// Parameters:
	//	node - the node to get the parent of
	//
	// Return Value: the parent of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetParentNode(TreeListNode *node)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return NULL;

			if (item->parent)
				return ConvertToTreeListNode(item->parent);
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetFirstChildNode
	// Description: called when the pane needs to get a node's first child node
	//
	// Parameters:
	//	node - the node to get the first child of
	//
	// Return Value: the first child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetFirstChildNode(TreeListNode *node)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return NULL;

			if (item->children && (item->children->Count > 0))
			{
				SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(0));
				return ConvertToTreeListNode(childItem);
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetLastChildNode
	// Description: called when the pane needs to get a node's last child node
	//
	// Parameters:
	//	node - the node to get the last child of
	//
	// Return Value: the last child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetLastChildNode(TreeListNode *node)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return NULL;

			if (item->children && (item->children->Count > 0))
			{
				SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(item->children->Count - 1));
				return ConvertToTreeListNode(childItem);
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetNextChildNode
	// Description: called when the pane needs to get a node's next child node
	//
	// Parameters:
	//	node - the node to get the next child of
	//	curChild - the current child of the node
	//
	// Return Value: the next child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetNextChildNode(TreeListNode *node, TreeListNode *curChild)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(curChild->Tag);
			if (!item || !childItem)
				return NULL;

			if (item->children && (item->children->Count > 0))
			{
				int currIndex = item->children->IndexOf(childItem);
				if (currIndex < item->children->Count - 1)
				{
					SITreeViewItem *nextItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(currIndex + 1));
					return ConvertToTreeListNode(nextItem);
				}
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetPrevChildNode
	// Description: called when the pane needs to get a node's previous child node
	//
	// Parameters:
	//	node - the node to get the previous child of
	//	curChild - the current child of the node
	//
	// Return Value: the previous child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetPrevChildNode(TreeListNode *node, TreeListNode *curChild)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(curChild->Tag);
			if (!item || !childItem)
				return NULL;

			if (item->children && (item->children->Count > 0))
			{
				int currIndex = item->children->IndexOf(childItem);
				if (currIndex > 0)
				{
					SITreeViewItem *nextItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(currIndex - 1));
					return ConvertToTreeListNode(nextItem);
				}
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetChildNodeAt
	// Description: called when the pane needs to get a node's child node
	//
	// Parameters:
	//	node - the node to get the child of
	//	index - the index of the child to get
	//
	// Return Value: the specified child of the node, NULL if failed
	//*************************************************************************
	TreeListNode *SITreeView::OnGetChildNodeAt(TreeListNode *node, unsigned int index)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return NULL;

			if (item->children && ((unsigned int)item->children->Count > index))
			{
				SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(item->children->get_Item(index));
				return ConvertToTreeListNode(childItem);
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnSetIsSelected
	// Description: called when the pane needs to set if a node is selected
	//
	// Parameters:
	//	node - the node to set
	//	selected - true for selected, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::OnSetIsSelected(TreeListNode *node, bool selected)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return;

			item->itemInfo.selected = selected;
			Invalidate();
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnSetIsExpanded
	// Description: called when the pane needs to set if a node is expanded
	//
	// Parameters:
	//	node - the node to set
	//	expanded - true for expanded, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::OnSetIsExpanded(TreeListNode *node, bool expanded)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return;

			if (expanded)
				ExpandItem(item);
			else
				CollapseItem(item);
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnSetIsFocused
	// Description: called when the pane needs to set if a node is focused
	//
	// Parameters:
	//	node - the node to set
	//	focused - true for focused, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::OnSetIsFocused(TreeListNode *node, bool focused)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return;

			item->itemInfo.focused = focused;
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnGetNextSiblingNode
	// Description: called when the pane needs to get a node's next sibling node
	//
	// Parameters:
	//	node - the node to get the next sibling of
	//
	// Return Value: the next sibling of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetNextSiblingNode(TreeListNode *node)
	{
		try
		{
			ParentChildList *p = OnGetParentNode(node);
			if (!p)
				return NULL;

			TreeListNode *parentNode = dynamic_cast<TreeListNode *>(p);
			SITreeViewItem *parentItem = dynamic_cast<SITreeViewItem *>(parentNode->Tag);
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);

			if (!parentItem || !item)
				return NULL;

			if (parentItem->children && (parentItem->children->Count > 0))
			{
				int currIndex = parentItem->children->IndexOf(item);
				if (currIndex < parentItem->children->Count - 1)
				{
					SITreeViewItem *nextItem = dynamic_cast<SITreeViewItem *>(parentItem->children->get_Item(currIndex + 1));
					return ConvertToTreeListNode(nextItem);
				}
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetParentNode
	// Description: called when the pane needs to get a node's parent node
	//
	// Parameters:
	//	node - the node to get the parent of
	//
	// Return Value: the parent of the node, NULL if failed
	//*************************************************************************
	ParentChildList *SITreeView::OnGetPrevSiblingNode(TreeListNode *node)
	{
		try
		{
			ParentChildList *p = OnGetParentNode(node);
			if (!p)
				return NULL;

			TreeListNode *parentNode = dynamic_cast<TreeListNode *>(p);
			SITreeViewItem *parentItem = dynamic_cast<SITreeViewItem *>(parentNode->Tag);
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);

			if (!parentItem || !item)
				return NULL;

			if (parentItem->children && (parentItem->children->Count > 0))
			{
				int currIndex = parentItem->children->IndexOf(item);
				if (currIndex > 0)
				{
					SITreeViewItem *prevItem = dynamic_cast<SITreeViewItem *>(parentItem->children->get_Item(currIndex - 1));
					return ConvertToTreeListNode(prevItem);
				}
			}
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetChildNodeCount
	// Description: called when the pane needs to get a node's number of children
	//
	// Parameters:
	//	node - the node to get the number of children of
	//
	// Return Value: the number of children of the node, 0 if failed
	//*************************************************************************
	int SITreeView::OnGetChildNodeCount(TreeListNode *node)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item || !item->children)
				return 0;

			return GetChildNodeCount(item);
		}
		catch(...)
		{
		}

		return 0;
	}

	//*************************************************************************
	// Method:		OnGetRowNumber
	// Description: called when the pane needs to get a node's row number
	//
	// Parameters:
	//	node - the node to get the row number of
	//	visibleOnly - true to count only visible nodes, false to count them all
	//
	// Return Value: the row number of the node, -1 if failed
	//*************************************************************************
	int SITreeView::OnGetRowNumber(TreeListNode *node, bool visibleOnly)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item || !item->parent)
				return -1;

			if (item->itemInfo.rowIndex == INVALID_ROW_INDEX)
				return -1;

			return item->itemInfo.rowIndex;
		}
		catch(...)
		{
		}

		return 0;
	}

	//*************************************************************************
	// Method:		OnGetNodeIndex
	// Description: called when the pane needs to get a node's index in another node
	//
	// Parameters:
	//	node - the node containing the child node
	//	child - the child node to get the index of
	//
	// Return Value: the index of the child node, -1 if failed
	//*************************************************************************
	int SITreeView::OnGetNodeIndex(TreeListNode *parent, TreeListNode *child)
	{
		try
		{
			SITreeViewItem *parentItem = dynamic_cast<SITreeViewItem *>(parent->Tag);
			SITreeViewItem *childItem = dynamic_cast<SITreeViewItem *>(child->Tag);
			if (!parentItem || !childItem || !parentItem->children)
				return -1;

			return parentItem->children->IndexOf(childItem);
		}
		catch(...)
		{
		}

		return -1;
	}

	//*************************************************************************
	// Method:		OnGetVisibleNodeCount
	// Description: called when the pane needs to get a node's visible child count
	//
	// Parameters:
	//	node - the node to get the visible child count of
	//
	// Return Value: the visible child count of the node, 0 if failed
	//*************************************************************************
	int SITreeView::OnGetVisibleNodeCount(TreeListNode *node)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(node->Tag);
			if (!item)
				return 0;

			if (!item->children)
				return 1;

			return GetVisibleNodeCount(item);
		}
		catch(...)
		{
		}

		return 0;
	}

	//*************************************************************************
	// Method:		OnTreeListViewUnselectAll
	// Description: called when the pane needs to unselect all nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::OnTreeListViewUnselectAll()
	{
		try
		{
			UnselectItems(rootItem);
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		OnTreeListViewQueryItem
	// Description: Gets the item at a specified index so the tree view can 
	//	display it
	//
	// Parameters:
	//	index - the index of the item to display
	//
	// Return Value: the tree list node at the specified index, or null if none
	//*************************************************************************
	TreeListNode *SITreeView::OnTreeListViewQueryItem(unsigned int index)
	{
		try
		{
			SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(items->get_Item(index));
			TreeListNode *retVal = ConvertToTreeListNode(item);
			return retVal;
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnGetParentNode
	// Description: called when the pane needs to get a node's parent node
	//
	// Parameters:
	//	node - the node to get the parent of
	//
	// Return Value: the parent of the node, NULL if failed
	//*************************************************************************
	TreeListNode *SITreeView::OnTreeListViewQueryRow(unsigned int row)
	{
		try
		{
			return OnTreeListViewQueryItem(row);
		}
		catch(...)
		{
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		OnListViewDoubleClick
	// Description: Called when a tree view item is doublic clicked.  Invokes the clicked
	//	delegate for the clicked menu item if applicable
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::OnListViewDoubleClick(Object *sender, EventArgs *args)
	{
		try
		{
			if (SelectedIndices->Count > 0)
			{
				unsigned int id = *dynamic_cast<__box int *>(this->SelectedIndices->get_Item(0));
				SITreeViewItem *item = dynamic_cast<SITreeViewItem *>(items->get_Item(id));
				if (item && item->OnDoubleClick)
				{
					item->OnDoubleClick->Invoke(GetItemFullName(item), item->tag);
				}
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		BeginUpdate
	// Description: Prevents refreshes during a large update operation.  Must
	//				call EndUpdate after update is complete.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::BeginUpdate()
	{
		updateModeCount++;
	}

	//*************************************************************************
	// Method:		EndUpdate
	// Description: Finishes an update marked with a call to BeginUpdate.
	//				This will automatically refresh the view with the changes.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SITreeView::EndUpdate()
	{
		updateModeCount--;

		if (updateModeCount == 0)
			Refresh();
	}
}