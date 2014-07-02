//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TreeListNode.cpp
//
// DESCRIPTION: Contains implemenation for the class TreeListNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include "TreeListNode.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		TreeListNode
	// Description: Constructor for the TreeListNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TreeListNode::TreeListNode()
	{
		imageIndex = 0;
		stateImageIndex = 0;
		index = 0;

		isRoot = false;
		selected = false;
		focused = false;
		styleAll = false;
		hovered = false;
		expanded = false;
		visible = true;

		backColor = SystemColors::Window;
		foreColor = SystemColors::WindowText;

		currChild = NULL;
		fullPath = "";

		subItems = new ContainerListViewSubItemCollection();
		subItems->ItemsChanged += new ItemsChangedEventHandler(this, OnSubItemsChanged);
	}

	//*************************************************************************
	// Method:		~TreeListNode
	// Description: Destructor for the TreeListNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TreeListNode::~TreeListNode()
	{
	}

	//*************************************************************************
	// Method:		get_ChildrenCount
	// Description: Gets the number of child nodes of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of child nodes of this node
	//*************************************************************************
	int TreeListNode::get_ChildrenCount()
	{
		if (GetChildNodeCount)
			return GetChildNodeCount->Invoke(this);

		return 0;
	}

	//*************************************************************************
	// Method:		set_IsExpanded
	// Description: Sets whether or not the node is expanded
	//
	// Parameters:
	//	value - true if the node is expanded, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::set_IsExpanded(bool value)
	{
		expanded = value;
		if (SetIsExpanded)
			SetIsExpanded->Invoke(this, value);
	}

	//*************************************************************************
	// Method:		set_Selected
	// Description: Sets whether or not the node is selected
	//
	// Parameters:
	//	value - true if the node is selected, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::set_Selected(bool value)
	{
		selected = value;
		if (SetIsSelected)
			SetIsSelected->Invoke(this, value);
	}

	//*************************************************************************
	// Method:		set_Focused
	// Description: Sets whether or not the node is focused
	//
	// Parameters:
	//	value - true if the node is focused, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::set_Focused(bool value)
	{
		focused = value;
		if (SetIsFocused)
			SetIsFocused->Invoke(this, value);
	}

	//*************************************************************************
	// Method:		Collapse
	// Description: Collapses the node
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::Collapse()
	{
		IsExpanded = false;
	}

	//*************************************************************************
	// Method:		CollapseAll
	// Description: Collapses this node and all child nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::CollapseAll()
	{
		TreeListNode *child = dynamic_cast<TreeListNode *>(this->FirstChild());
		while (child)
		{
			child->CollapseAll();
			child = dynamic_cast<TreeListNode *>(this->NextChild());
		}
		Collapse();
	}

	//*************************************************************************
	// Method:		Expand
	// Description: Expands this node
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::Expand()
	{
		IsExpanded = true;
	}

	//*************************************************************************
	// Method:		ExpandAll
	// Description: Expands this node and all child nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::ExpandAll()
	{
		Expand();
		TreeListNode *child = dynamic_cast<TreeListNode *>(this->FirstChild());
		while (child)
		{
			child->ExpandAll();
			child = dynamic_cast<TreeListNode *>(this->NextChild());
		}
	}

	//*************************************************************************
	// Method:		Toggle
	// Description: Toggles the state of the node.  If it is expanded it will be
	//	collapsed, and vice versa
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::Toggle()
	{
		if (expanded)
			Collapse();
		else
			Expand();
	}

	//*************************************************************************
	// Method:		IsNodeAt
	// Description: returns whether this node is at a certain visual location
	//
	// Parameters:
	//	firstVisibleYPixel - the absolute number of pixels in the Y axis to the
	//		first visible pixel
	//	yOffset - the number of pixels attributable to the other nodes visible
	//		above this one
	//	rowHeight - the height of each row
	//
	// Return Value: true if this node is at the specified location, false otherwise
	//*************************************************************************
	bool TreeListNode::IsNodeAt(int firstVisibleYPixel, int yOffset, int rowHeight)
	{
		return ((firstVisibleYPixel > yOffset) &&
			(yOffset + (VisibleNodeCount * rowHeight) + rowHeight <= firstVisibleYPixel));
	}

	//*************************************************************************
	// Method:		IsNodeAt
	// Description: returns whether this node is at a certain visual location
	//
	// Parameters:
	//	row - the row to check for this node
	//	prior - the number of nodes prior to this one
	//
	// Return Value: true if this node is at the specified location, false otherwise
	//*************************************************************************
	bool TreeListNode::IsNodeAt(int row, int prior)
	{
		return ((row - prior) <= VisibleNodeCount);
	}

	//*************************************************************************
	// Method:		GetNodeAt
	// Description: returns the node at a certain visual location
	//
	// Parameters:
	//	firstVisibleYPixel - the absolute number of pixels in the Y axis to the
	//		first visible pixel
	//	yOffset - the number of pixels attributable to the other nodes visible
	//		above this one
	//	rowHeight - the height of each row
	//	node (out) - the node at the specified location
	//
	// Return Value: true if it can get the node, false otherwise
	//*************************************************************************
	bool TreeListNode::GetNodeAt(int firstVisibleYPixel, int yOffset, int rowHeight, TreeListNode **node)
	{
		return GetNodeAt(firstVisibleYPixel / rowHeight, yOffset / rowHeight, node);
	}

	//*************************************************************************
	// Method:		GetNodeAt
	// Description: returns the node at a certain visual location
	//
	// Parameters:
	//	row - the row to check for the node
	//	prior - the number of nodes prior to this one
	//	node (out) - the node at the specified location
	//
	// Return Value: true if it can get the node, false otherwise
	//*************************************************************************
	bool TreeListNode::GetNodeAt(int row, int prior, TreeListNode **node)
	{
		*node = NULL;

		if (!this->isRoot)
		{
			int firstRow = row - prior;

			if (!IsNodeAt(firstRow, 0))
				return false;

			if (firstRow == 1)
			{
				*node = this;
				return true;
			}

			prior++;
		}

		// find the node
		int children = this->ChildrenCount;
		if (GetChildNodeAt)
		{
			for (int i = 0; i < children; i++)
			{
				TreeListNode *child = GetChildNodeAt->Invoke(this, i);
				if (child && (child->GetNodeAt(row, prior, node)))
					return true;

				if (child)
					prior += child->VisibleNodeCount;
			}
		}

		// should never get here
		*node = NULL;
		return false;
	}

	//*************************************************************************
	// Method:		Level
	// Description: returns the depth level of the node
	//
	// Parameters:
	//	None
	//
	// Return Value: the depth level of the node
	//*************************************************************************
	int TreeListNode::Level()
	{
		Stack *stack = new Stack();
		GetStackToVirtualParent(stack);

		// The stack will always contain the additional virtual node
		// and since the level should return a zero based reference, need
		// to subtract another 1
		return stack->Count - 2;
	}

	//*************************************************************************
	// Method:		RowNumber
	// Description: returns the row number of this node
	//
	// Parameters:
	//	visibleOnly - true if you want to only count visible rows, false for
	//		all rows
	//
	// Return Value: the row number of this node, -1 if failed
	//*************************************************************************
	int TreeListNode::RowNumber(bool visibleOnly)
	{
		if (GetRowNumber)
			return GetRowNumber->Invoke(this, visibleOnly);
		return -1;
	}

	//*************************************************************************
	// Method:		ParentNode
	// Description: returns the parent of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's parent node
	//*************************************************************************
	ParentChildList *TreeListNode::ParentNode()
	{
		if (GetParentNode)
			return GetParentNode->Invoke(this);
		return NULL;
	}

	//*************************************************************************
	// Method:		PreviousSibling
	// Description: returns the previous sibling of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's previous sibling node
	//*************************************************************************
	ParentChildList *TreeListNode::PreviousSibling()
	{
		if (GetPrevSibling)
			return GetPrevSibling->Invoke(this);
		return NULL;
	}

	//*************************************************************************
	// Method:		NextSibling
	// Description: returns the next sibling of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's next sibling node
	//*************************************************************************
	ParentChildList *TreeListNode::NextSibling()
	{
		if (GetNextSibling)
			return GetNextSibling->Invoke(this);
		return NULL;
	}

	//*************************************************************************
	// Method:		FirstChild
	// Description: returns the first child node of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's first child node
	//*************************************************************************
	ParentChildList *TreeListNode::FirstChild()
	{
		if (GetFirstChild)
		{
			currChild = dynamic_cast<TreeListNode *>(GetFirstChild->Invoke(this));
			return currChild;
		}
		return NULL;
	}

	//*************************************************************************
	// Method:		LastChild
	// Description: returns the last child node of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's last child node
	//*************************************************************************
	ParentChildList *TreeListNode::LastChild()
	{
		if (GetLastChild)
		{
			currChild = dynamic_cast<TreeListNode *>(GetLastChild->Invoke(this));
			return currChild;
		}
		return NULL;
	}

	//*************************************************************************
	// Method:		NextChild
	// Description: returns the next child node of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's next child node
	//*************************************************************************
	ParentChildList *TreeListNode::NextChild()
	{
		if (GetNextChild)
		{
			currChild = dynamic_cast<TreeListNode *>(GetNextChild->Invoke(this, currChild));
			return currChild;
		}
		return NULL;
	}

	//*************************************************************************
	// Method:		PreviousChild
	// Description: returns the previous child of this node
	//
	// Parameters:
	//	None
	//
	// Return Value: this node's previous child node
	//*************************************************************************
	ParentChildList *TreeListNode::PreviousChild()
	{
		if (GetPrevChild)
		{
			currChild = dynamic_cast<TreeListNode *>(GetPrevChild->Invoke(this, currChild));
			return currChild;
		}
		return NULL;
	}

	//*************************************************************************
	// Method:		CompareTo
	// Description: returns 0 if the nodes are equal, 1 if this node is after
	//	comparisonNode, -1 if this node is before the comparisonNode
	//
	// Parameters:
	//	comparisonNode - the node to compare this one to
	//
	// Return Value: 0 if the nodes are equal, 1 if this node is after
	//	comparisonNode, -1 if this node is before the comparisonNode
	//*************************************************************************
	int TreeListNode::CompareTo(TreeListNode *comparisonNode)
	{
		if (this == comparisonNode)
			return 0;

		if (this->IsAfter(comparisonNode))
			return 1;

		return -1;
	}

	//*************************************************************************
	// Method:		OnSubItemsChanged
	// Description: updates sub items when 1 is changed
	//
	// Parameters:
	//	sender - the sender of the change event
	//	e - the args representing the change event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::OnSubItemsChanged(Object *sender, ItemsChangedEventArgs *e)
	{
		subItems->get_Item(e->IndexChanged)->MouseDown += new MouseEventHandler(this, OnSubItemMouseDown);
	}

	//*************************************************************************
	// Method:		OnSubItemMouseDown
	// Description: invokes the MouseDown delegate for this object
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::OnSubItemMouseDown(Object *sender, MouseEventArgs *e)
	{
		if (MouseDown)
			MouseDown->Invoke(this, e);
	}

	//*************************************************************************
	// Method:		OnSubNodeMouseDown
	// Description: invokes the MouseDown delegate for this object
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::OnSubNodeMouseDown(Object *sender, MouseEventArgs *e)
	{
		if (MouseDown)
			MouseDown->Invoke(sender, e);
	}

	//*************************************************************************
	// Method:		GetStackToVirtualParent
	// Description: gets the stack of parent nodes up to the root node
	//
	// Parameters:
	//	stack - the stack to add the nodes to
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::GetStackToVirtualParent(Stack *stack)
	{
		stack->Push(this);
		if (this->ParentNode())
			GetStackToVirtualParent(stack, ParentNode());
	}

	//*************************************************************************
	// Method:		GetStackToVirtualParent
	// Description: gets the stack of the specified node's parent nodes up to 
	//	the root node
	//
	// Parameters:
	//	stack - the stack to add the nodes to
	//	node - the node to get the stack of
	//
	// Return Value: None
	//*************************************************************************
	void TreeListNode::GetStackToVirtualParent(Stack *stack, ParentChildList *node)
	{
		stack->Push(node);
		if (node->ParentNode())
			GetStackToVirtualParent(stack, node->ParentNode());
	}

	//*************************************************************************
	// Method:		IsAfter
	// Description: returns true if this node is after the specified node
	//
	// Parameters:
	//	node - the node to compare against
	//
	// Return Value: true if this node is after the specified node
	//*************************************************************************
	bool TreeListNode::IsAfter(TreeListNode *node)
	{
		TreeListNode *parent = dynamic_cast<TreeListNode *>(ParentNode());
		if (!parent || GetNodeIndex)
			return false;

		int thisIndex = GetNodeIndex->Invoke(parent, this);
		int nodeIndex = GetNodeIndex->Invoke(parent, node);

		return (thisIndex > nodeIndex);
	}

	//*************************************************************************
	// Method:		IsBefore
	// Description: returns true if this node is before the specified node
	//
	// Parameters:
	//	node - the node to compare against
	//
	// Return Value: true if this node is before the specified node
	//*************************************************************************
	bool TreeListNode::IsBefore(TreeListNode *node)
	{
		TreeListNode *parent = dynamic_cast<TreeListNode *>(ParentNode());
		if (!parent || GetNodeIndex)
			return false;

		int thisIndex = GetNodeIndex->Invoke(parent, this);
		int nodeIndex = GetNodeIndex->Invoke(parent, node);

		return (thisIndex < nodeIndex);
	}

	//*************************************************************************
	// Method:		get_VisibleNodeCount
	// Description: returns the number of visible child nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of visible child nodes
	//*************************************************************************
	int TreeListNode::get_VisibleNodeCount()
	{
		if (this->GetVisibleNodeCount)
			return GetVisibleNodeCount->Invoke(this);
		return 0;
	}

	//*************************************************************************
	// Method:		GetListViewItem
	// Description: Converts this ContainerListViewItem to a ListViewItem type
	//				(Only text is copied)
	//
	// Parameters:
	//	None
	//
	// Return Value: A ListView type copy of this item
	//*************************************************************************
	ListViewItem *TreeListNode::GetListViewItem()
	{
		ListViewItem * lvi = new ListViewItem(this->Text);
		
		if (this->subItems != NULL)
		{
			for (int i=0; i < this->subItems->Count; i++)
			{
				ContainerListViewSubItem * clvsi = this->subItems->Item[i];
				lvi->SubItems->Add (clvsi->Text);
			}
		}
		
		return lvi;
	}
}