//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TreeListView.cpp
//
// DESCRIPTION: Contains implemenation for the class TreeListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#include <windows.h>
#include "TreeListView.h"
#include "StringTools.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		TreeListView
	// Description: Constructor for the TreeListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TreeListView::TreeListView()
	{
		indent = 19;
		rendCount = 0;

		showLines = true;
		showRootLines = false;
		showPlusMinus = true;
		alwaysShowPM = false;
		mouseActivate = false;
		allCollapsed = false;
		visualStyle = true;

		expandAllKey = Keys::F5;

		currNode = NULL;
		firstSelectedNode = NULL;
		components = NULL;

		virtualParent = NULL;

		SelectedItemChanged = NULL;

		nodeRowRects = new ListDictionary();
		pmRects = new ListDictionary();

		try
		{
			// use reflection to get the plus and minus bitmaps
			minusBitmap = IconLibrary::Instance->GetBitmapForType(IconType::TreeViewMinusIconType);
			plusBitmap = IconLibrary::Instance->GetBitmapForType(IconType::TreeViewPlusIconType);
		}
		catch(Exception *)
		{
			// couldn't find the files
			minusBitmap = NULL;
			plusBitmap = NULL;
		}
	}

	//*************************************************************************
	// Method:		~TreeListView
	// Description: Destructor for the TreeListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TreeListView::~TreeListView()
	{
	}

	//*************************************************************************
	// Method:		OnSubControlMouseDown
	// Description: handles a click on a subcontrol
	//
	// Parameters:
	//	sender - the sender of the click event
	//	e - the args representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnSubControlMouseDown(Object *sender, MouseEventArgs *e)
	{
		TreeListNode *node = dynamic_cast<TreeListNode *>(sender);
		UnselectNodes(node);

		node->Focused = true;
		node->Selected = true;

		if (e->Clicks >= 2)
			node->Toggle();

		Invalidate(ClientRectangle);
	}

	//*************************************************************************
	// Method:		OnNodesChanged
	// Description: handles a node change event
	//
	// Parameters:
	//	sender - the sender of the change event
	//	e - the args representing the change event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnNodesChanged(Object *sender, EventArgs *e)
	{
		AdjustScrollBars();
	}

	//*************************************************************************
	// Method:		OnSelectedItemChanged
	// Description: invokes the SelectedItemChanged delegate for this class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnSelectedItemChanged()
	{
		if (SelectedItemChanged)
			SelectedItemChanged->Invoke(this, new EventArgs());
	}

	//*************************************************************************
	// Method:		OnSelectedIndexChanged
	// Description: invokes the SelectedIndexChanged delegate for this class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnSelectedIndexChanged()
	{
		if (SelectedIndexChanged)
			SelectedIndexChanged->Invoke(this, new EventArgs());
	}

	//*************************************************************************
	// Method:		get_VisibleNodeCount
	// Description: returns a the number of visible nodes of the tree
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of visible nodes of the tree
	//*************************************************************************
	int TreeListView::get_VisibleNodeCount()
	{
		if (!virtualParent)
			return 0;

		if (virtualParent->GetVisibleNodeCount)
			return virtualParent->GetVisibleNodeCount->Invoke(virtualParent);

		return 0;
	}

	//*************************************************************************
	// Method:		get_SelectedNodes
	// Description: returns a collection containing all the selected nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: a collection containing all the selected nodes
	//*************************************************************************
	SelectedTreeListNodeCollection *TreeListView::get_SelectedNodes()
	{
		return GetSelectedNodes(virtualParent);
	}

	//*************************************************************************
	// Method:		set_ItemCount
	// Description: sets the number of items in the treelistview
	//
	// Parameters:
	//	value - the number of items in the tree list view
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::set_ItemCount(unsigned int value)
	{
		totalNodeCount = value;
		if (visible)
		{
			AdjustScrollBars();
			Invalidate();
			vScrollBar->Show();
		}
	}

	//*************************************************************************
	// Method:		set_SelectedIndex
	// Description: sets the selected item in the tree view
	//
	// Parameters:
	//	index - the index of the item to set
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::set_SelectedIndex(int index)
	{
		if (UnselectAllNodes != NULL)
			UnselectAllNodes->Invoke();
		else
			UnselectNodes(virtualParent);

		if (QueryTreeListNode)
		{
			currNode = QueryTreeListNode->Invoke(index);

			selectedIndices->Clear();

			if (currNode)
			{
				firstSelectedNode = currNode;
				firstSelectedNode->Selected = true;
				firstSelectedNode->Focused = true;
				selectedIndices->Add(__box(index));
			}
			
			MakeSelectedVisible();
			OnSelectedItemChanged();
			OnSelectedIndexChanged();
			Invalidate();
		}
	}

	//*************************************************************************
	// Method:		set_SelectedNode
	// Description: sets the selected item in the tree view
	//
	// Parameters:
	//	node - the node to select
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::set_SelectedNode(TreeListNode *node)
	{
		if (UnselectAllNodes != NULL)
			UnselectAllNodes->Invoke();
		else
			UnselectNodes(virtualParent);

		selectedIndices->Clear();

		currNode = node;
		if (currNode)
		{
			firstSelectedNode = currNode;
			firstSelectedNode->Selected = true;
			firstSelectedNode->Focused = true;
			selectedIndices->Add(__box(node->RowNumber(false)));
		}
	
		MakeSelectedVisible();
		OnSelectedItemChanged();
		OnSelectedIndexChanged();
		Invalidate();
	}

	//*************************************************************************
	// Method:		set_SelectedRow
	// Description: sets the selected item in the tree view by row index
	//
	// Parameters:
	//	row - the row index of the item to set
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::set_SelectedRow(int row)
	{
		if (UnselectAllNodes != NULL)
			UnselectAllNodes->Invoke();
		else
			UnselectNodes(virtualParent);

		if (QueryTreeListNodeAtRow)
		{
			currNode = QueryTreeListNodeAtRow->Invoke(row);

			selectedIndices->Clear();

			if (currNode)
			{
				firstSelectedNode = currNode;
				firstSelectedNode->Selected = true;
				firstSelectedNode->Focused = true;
				selectedIndices->Add(__box(currNode->Index));
			}
		
			MakeSelectedVisible();
			OnSelectedItemChanged();
			OnSelectedIndexChanged();
			Invalidate();
		}
	}

	//*************************************************************************
	// Method:		set_RootNode
	// Description: sets the root node of the tree view
	//
	// Parameters:
	//	value - the new root node of the tree list view
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::set_RootNode(TreeListNode *value) 
	{ 
		virtualParent = value; 
	}

	//*************************************************************************
	// Method:		CollapseAll
	// Description: collapses all nodes and their subnodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::CollapseAll()
	{
		if (!virtualParent)
			return;

		virtualParent->CollapseAll();

		allCollapsed = true;
		AdjustScrollBars();
		Invalidate();
	}

	//*************************************************************************
	// Method:		ExpandAll
	// Description: expands all nodes and their subnodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::ExpandAll()
	{
		if (!virtualParent)
			return;

		virtualParent->ExpandAll();

		allCollapsed = false;
		AdjustScrollBars();
		Invalidate();
	}

	//*************************************************************************
	// Method:		GetSelectedNodes
	// Description: returns a collection containing all the selected nodes under
	//	the specified node
	//
	// Parameters:
	//	node - the node to get the selected sub nodes for
	//
	// Return Value: a collection containing all the selected nodes under the
	//	specified node
	//*************************************************************************
	SelectedTreeListNodeCollection *TreeListView::GetSelectedNodes(TreeListNode *node)
	{
		SelectedTreeListNodeCollection *list = new SelectedTreeListNodeCollection();
		if (node->ChildrenCount == 0)
			return list;

		TreeListNode *child = dynamic_cast<TreeListNode *>(node->FirstChild());
		while (child)
		{
			if (child->Selected)
				list->Add(child);

			if (child->IsExpanded)
			{
				SelectedTreeListNodeCollection *list2 = GetSelectedNodes(child);
				for (int i = 0; i < list2->Count; i++)
					list->Add(list2->get_Item(i));
			}
		}

		return list;
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
	void TreeListView::MakeSelectedVisible()
	{
		int topVisible;
		int top;
		int bottom;

		if (currNode && ensureVisible)
		{
			if (currNode->Selected)
			{
				// need to figure out what row number the currNode is in the list
				int i = currNode->RowNumber(true);
				if (i == -1)
				{
					// row isn't visible, expand parent node(s) to make it visible
					ArrayList* parents = new ArrayList();
					TreeListNode* curParent = currNode;
					while (curParent->Level() > 0)
					{
						curParent = dynamic_cast<TreeListNode*>(curParent->ParentNode());
						parents->Insert(0, curParent);
					}

					for (int j=0; j<parents->Count; j++)
						dynamic_cast<TreeListNode*>(parents->Item[j])->Expand();

					i = currNode->RowNumber(true);
				}
				try
				{
					topVisible = this->GetTopVisibleIndex();
					top = topVisible;
					bottom = this->GetBottomVisibleIndex() - 3;

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
					if ( (top / rowHeight) > vScrollBar->Maximum)
						vScrollBar->Value = vScrollBar->Maximum;
					else if ( (top / rowHeight) < vScrollBar->Minimum)
						vScrollBar->Value = vScrollBar->Minimum;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		OnKeyDown
	// Description: called when a key is pressed in the tree view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnKeyDown(KeyEventArgs *e)
	{
		ContainerListView::OnCheckShiftState(e);

		if ((e->KeyCode == Keys::Left) || (e->KeyCode == Keys::Right))
			OnLeftRightKeys(e);
		else
			ContainerListView::OnKeyDown(e);
	}

	//*************************************************************************
	// Method:		OnLeftRightKeys
	// Description: called when one of the left or right keys is pressed in the 
	//	tree view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnLeftRightKeys(KeyEventArgs *e)
	{
		// left/right don't mean anything for childless nodes
		if ((ItemCount > 0) && currNode && (currNode->ChildrenCount > 0))
		{
			switch(e->KeyCode)
			{
			case Keys::Left:
				if (currNode->IsExpanded)
				{
					currNode->Collapse();
					AdjustScrollBars();
				}
				break;
			case Keys::Right:
				if (!currNode->IsExpanded)
				{
					currNode->Expand();
					AdjustScrollBars();
				}
				break;
			}

			Invalidate();
			e->Handled = true;
		}
	}

	//*************************************************************************
	// Method:		OnUpDownKeys
	// Description: called when one of the up or down keys is pressed in the tree view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnUpDownKeys(KeyEventArgs *e)
	{
		switch(e->KeyCode)
		{
		case Keys::Up:
			GetPriorNode(&currNode);
			break;
		case Keys::Down:
			GetNextNode(&currNode);
			break;
		}

		if (!currNode)
			return;

		if (UnselectAllNodes != NULL)
			UnselectAllNodes->Invoke();
		else
			UnselectNodes(virtualParent);
		currNode->Selected = true;
		currNode->Focused = true;

		if (currNode->GetRowNumber)
		{
			selectedIndices->Clear();
			selectedIndices->Add(__box(currNode->GetRowNumber->Invoke(currNode, false)));
		}

		if ((multiSelectMode == MultiSelectMode::Single) || (firstSelectedNode == NULL))
		{
			firstSelectedNode->Selected = false;
			firstSelectedNode->Focused = false;

			firstSelectedNode = currNode;

			firstSelectedNode->Selected = true;
			firstSelectedNode->Focused = true;
		}

		MakeSelectedVisible();
		OnSelectedItemChanged();
		OnSelectedIndexChanged();
		Invalidate();
		e->Handled = true;
	}

	//*************************************************************************
	// Method:		GetPriorNode
	// Description: gets the node prior to the current node
	//
	// Parameters:
	//	node (out) - the prior node
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool TreeListView::GetPriorNode(TreeListNode **curNode)
	{
		if (!*curNode)
			return false;

		if (!(*curNode)->PreviousSibling() && (*curNode)->ParentNode())
		{
			TreeListNode *t = dynamic_cast<TreeListNode *>((*curNode)->ParentNode());
			if (t->ParentNode())
			{
				*curNode = t;
				return true;
			}
		}
		else if ((*curNode)->PreviousSibling())
		{
			TreeListNode *t = dynamic_cast<TreeListNode *>((*curNode)->PreviousSibling());
			if ((t->ChildrenCount > 0) && t->IsExpanded)
			{
				do
				{
					t = dynamic_cast<TreeListNode *>(t->LastChild());
					if ((!t->IsExpanded) || (t->ChildrenCount == 0))
					{
						*curNode = t;
						return true;
					}
				} while ((t->ChildrenCount > 0) && t->IsExpanded);
			}
			else
			{
				*curNode = dynamic_cast<TreeListNode *>((*curNode)->PreviousSibling());
				return true;
			}
		}

		return false;
	}

	//*************************************************************************
	// Method:		GetNextNode
	// Description: gets the node next to the current node
	//
	// Parameters:
	//	node (out) - the next node
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool TreeListView::GetNextNode(TreeListNode **curNode)
	{
		if (!*curNode)
			return false;

		if (((*curNode)->IsExpanded) && ((*curNode)->ChildrenCount > 0))
		{
			*curNode = dynamic_cast<TreeListNode *>((*curNode)->FirstChild());
			return true;
		}
		else if ((!(*curNode)->NextSibling()) && ((*curNode)->ParentNode()))
		{
			TreeListNode *t = *curNode;
			do
			{
				t = dynamic_cast<TreeListNode *>(t->ParentNode());
				if (t->NextSibling())
				{
					*curNode = dynamic_cast<TreeListNode *>(t->NextSibling());
					return true;
				}
			} while ((!t->NextSibling()) && (t->ParentNode()));
		}
		else if ((*curNode)->NextSibling())
		{
			*curNode = dynamic_cast<TreeListNode *>((*curNode)->NextSibling());
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		ShowSelectedItems
	// Description: Shows the selected items in the tree view
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::ShowSelectedItems()
	{
		if (!currNode)
			return;

		if (UnselectAllNodes != NULL)
			UnselectAllNodes->Invoke();
		else
			UnselectNodes(virtualParent);

		// figure out the path between the first selected item and the current node
		// then select all visible child nodes
		if (firstSelectedNode == currNode)
		{
			currNode->Selected = true;
		}
		else
		{
			// set each node from the first to the current node, selecting as we go
			int currentNodeIsAbove = FirstNodeRelativeToCurrentNode();
			TreeListNode *node = firstSelectedNode;
			node->Selected = true;

			while (node != currNode)
			{
				if (currentNodeIsAbove == -1)
				{
					if (!GetPriorNode(&node))
						break;
				}
				else
				{
					if (!GetNextNode(&node))
						break;
				}

				node->Selected = true;
			}
		}

		MakeSelectedVisible();
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
	void TreeListView::OnResize(EventArgs *e)
	{
		ContainerListView::OnResize(e);

		AdjustScrollBars();
		Invalidate();
	}

	//*************************************************************************
	// Method:		OnMouseDown
	// Description: called when the mouse is pressed in the tree view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnMouseDown(MouseEventArgs *e)
	{
		ContainerListView::OnMouseDown(e);

		OnMouseDownColumns(e);
		OnMouseDownBody(e);
	}

	//*************************************************************************
	// Method:		OnMouseDownColumns
	// Description: called when the mouse is pressed in the tree view to handle
	//	column clicks
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnMouseDownColumns(MouseEventArgs *e)
	{
		for (int i = 0; i < columns->Count; i++)
		{
			if ((columnSizeRects->Length > 0) && (MouseInRect(e, columnSizeRects[i])))
			{
				// autosize column
				if ((e->Clicks == 2) && (e->Button == MouseButtons::Left))
				{
					int mWid = 0;
					int textWid = 0;

					AutoSetColWidth(virtualParent, &mWid, &textWid, i);

					textWid = GetStringWidth(columns->get_Item(i)->Text, Font);
					if (textWid > mWid)
						mWid = textWid;

					mWid += 5;

					if (columns->get_Item(i)->Image)
						mWid += 18;

					columns->get_Item(i)->Width = mWid;
					GenerateColumnRects();
				}
				else // scale column
				{
					colScaleMode = true;
					colScaleWidth = columnRects[i].Width;
					scaledCol = i;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		OnMouseDownBody
	// Description: called when the mouse is pressed in the tree view to handle
	//	non column clicks
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnMouseDownBody(MouseEventArgs *e)
	{
		if (MouseInRect(e, rowsRect))
		{
			TreeListNode *node;

			node = NodePlusClicked(e);
			if (node)
			{
				node->Toggle();
				AdjustScrollBars();
			}
			else
			{
				node = NodeInNodeRow(e);
				if (!node)
				{
					selectedIndices->Clear();
					SelectedIndex = -1;
					return;
				}

				switch(multiSelectMode)
				{
				case MultiSelectMode::Single:
					if (UnselectAllNodes != NULL)
						UnselectAllNodes->Invoke();
					else
						UnselectNodes(virtualParent);

					if ((e->Clicks == 2) && (!mouseActivate))
					{
						node->Toggle();
						AdjustScrollBars();
					}
					else if ((e->Clicks == 2) && mouseActivate)
					{
						//OnItemActivate(new EventArgs());
					}

					currNode = node;
					break;
				case MultiSelectMode::Range:
					if (UnselectAllNodes != NULL)
						UnselectAllNodes->Invoke();
					else
						UnselectNodes(virtualParent);
					currNode = node;
					break;
				case MultiSelectMode::Selective:
					if (UnselectAllNodes != NULL)
						UnselectAllNodes->Invoke();
					else
						UnselectNodes(virtualParent);

					if (node->Selected)
					{
						// unselect it
						node->Focused = false;
						node->Selected = false;
					}
					else
					{
						node->Focused = true;
						node->Selected = true;
						currNode = node;
					}

					if ((e->Clicks == 2) && !mouseActivate)
					{
						node->Toggle();
						AdjustScrollBars();
					}
					else if ((e->Clicks == 2) && mouseActivate)
					{
						//OnItemActivate(new EventArgs());
					}

					Invalidate();
					break;
				}
			}

			if (node->GetRowNumber)
			{
				selectedIndices->Clear();
				selectedIndices->Add(__box(node->GetRowNumber->Invoke(node, false)));
			}

			if ((multiSelectMode == MultiSelectMode::Single) || (!firstSelectedNode))
			{
				if (currNode)
				{
					firstSelectedNode = currNode;
					firstSelectedNode->Selected = true;
					firstSelectedNode->Focused = true;
				}
			}

			OnSelectedItemChanged();
			OnSelectedIndexChanged();
			Invalidate();
		}
		else
		{
			selectedIndices->Clear();
			selectedIndices->Add(__box(-1));
		}
	}

	//*************************************************************************
	// Method:		OnKeyUp
	// Description: called when the up key is pressed in the tree view
	//
	// Parameters:
	//	e - the args representing the press event
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::OnKeyUp(KeyEventArgs *e)
	{
		ContainerListView::OnKeyUp(e);
		if (e->Handled)
			return;

		if (e->KeyCode == expandAllKey)
		{
			if (allCollapsed)
				ExpandAll();
			else
				CollapseAll();
		}
	}

	//*************************************************************************
	// Method:		DrawRows
	// Description: draws the tree view's rows
	//
	// Parameters:
	//	g - the Graphics object used to do the drawing
	//	r - the rectangle representing the area to draw in
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::DrawRows(Graphics *g, System::Drawing::Rectangle r)
	{
		if (InUpdateTransaction)
			return;

		AdjustScrollBars();

		nodeRowRects->Clear();
		pmRects->Clear();

		if (ItemCount == 0)
			return;

		int totalRend = 0;
		int maxRend = (ClientRectangle.Height / rowHeight) + 1;
		int prior = vScrollBar->Value / rowHeight;
		if (prior < 0)
			prior = 0;

		TreeListNode *nodeToDraw = QueryTreeListNodeAtRow->Invoke(prior++);

		totalRend = 0;
		GenerateColumnRects();

		// Mark all controls as not visible (don't actually make them invisible yet or the
		// active control will lose focus)
		IEnumerator *enumerator = this->Controls->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Control *control = dynamic_cast<Control *>(enumerator->Current);
			if ((control != hScrollBar) && (control != vScrollBar))
				control->Tag = __box(false);
		}

		while (nodeToDraw && (maxRend > totalRend))
		{
			if (FilterNodeColumns)
				FilterNodeColumns->Invoke(nodeToDraw);
			RenderNodeRows(nodeToDraw, g, r, &totalRend);

			totalRend++;

			nodeToDraw = QueryTreeListNodeAtRow->Invoke(prior++);
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

	//*************************************************************************
	// Method:		Dispose
	// Description: disposes of the tree view
	//
	// Parameters:
	//	disposing - true if the object is being disposed, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::Dispose(bool disposing)
	{
		if (disposing)
		{
			if (components)
				components->Dispose();
		}
		ContainerListView::Dispose(disposing);
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
	void TreeListView::AdjustScrollBars()
	{
		try
		{
			if ((VisibleNodeCount > 0) && ((columns->Count > 0) && !colScaleMode))
			{
				allColsWidth = 0;
				for (int i = 0; i < columns->Count; i++)
					allColsWidth += columns->get_Item(i)->Width;

				allRowsHeight = rowHeight * VisibleNodeCount;
				vSize = vScrollBar->Width;
				hSize = hScrollBar->Height;

				hScrollBar->Left = ClientRectangle.Left + 2;
				hScrollBar->Maximum = allColsWidth;			

				if (allColsWidth > ClientRectangle.Width - 4 - vSize)
				{
					hScrollBar->Show();
					hSize = hScrollBar->Height;
				}
				else
				{
					hScrollBar->Hide();
					hScrollBar->Value = 0;
					hSize = 0;
				}

				vScrollBar->Left = ClientRectangle.Left + ClientRectangle.Width - vScrollBar->Width - 2;
				vScrollBar->Top = ClientRectangle.Top + headerBuffer + 2;
				vScrollBar->Height = ClientRectangle.Height - hSize - headerBuffer - 4;
				vScrollBar->SmallChange = rowHeight;
				vScrollBar->LargeChange = 10 * vScrollBar->SmallChange;
				vScrollBar->Maximum = allRowsHeight + (vScrollBar->LargeChange/2);

				if (allRowsHeight > ClientRectangle.Height - headerBuffer - 4 - hSize)
				{
					vScrollBar->Show();
					vSize = vScrollBar->Width;
				}
				else
				{
					vScrollBar->Hide();
					vScrollBar->Value = 0;
					vSize = 0;
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
	// Method:		AutoSetColWidth
	// Description: automatically sets the column widths
	//
	// Parameters:
	//	node - the root node to set the column width for
	//	mWid (in/out) - the max column width
	//	tWid (in/out) - the text width
	//	i - the column to set the width of
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::AutoSetColWidth(TreeListNode *node, int *mWid, int *tWid, int i)
	{
		TreeListNode *thisItem = dynamic_cast<TreeListNode *>(node->FirstChild());
		while (thisItem)
		{
			if (i > 0)
			{
				ContainerListViewSubItem *subItem = thisItem->SubItems->get_Item(i - 1);
				*tWid = (subItem == NULL ? 0 : GetStringWidth(subItem->Text, Font));
			}
			else
			{
				*tWid = GetStringWidth(thisItem->Text, Font);
			}
			*tWid += 5;

			if (*tWid > *mWid)
				*mWid = *tWid;

			if (thisItem->ChildrenCount > 0)
			{
				AutoSetColWidth(thisItem, mWid, tWid, i);
			}
			thisItem = dynamic_cast<TreeListNode *>(node->NextChild());
		}
	}

	//*************************************************************************
	// Method:		UnfocusNodes
	// Description: removes focus from all nodes in the collection
	//
	// Parameters:
	//	node - the root node to remove focus from
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::UnfocusNodes(TreeListNode *node)
	{
		if (!node)
			return;

		node->Focused = false;
		TreeListNode *child = dynamic_cast<TreeListNode *>(node->FirstChild());
		while (child)
		{
			UnfocusNodes(child);
			child = dynamic_cast<TreeListNode *>(node->NextChild());
		}
	}

	//*************************************************************************
	// Method:		UnselectNodes
	// Description: removes selection from all nodes in the collection
	//
	// Parameters:
	//	node - the root node to remove selection from
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::UnselectNodes(TreeListNode *node)
	{
		if (!node)
			return;

		node->Focused = false;
		node->Selected = false;
		TreeListNode *child = dynamic_cast<TreeListNode *>(node->FirstChild());
		while (child)
		{
			UnselectNodes(child);
			child = dynamic_cast<TreeListNode *>(node->NextChild());
		}
	}

	//*************************************************************************
	// Method:		NodeInNodeRow
	// Description: gets the node at a specified location
	//
	// Parameters:
	//	e - the args representing the position of the mouse
	//
	// Return Value: the node at the mouse location
	//*************************************************************************
	TreeListNode *TreeListView::NodeInNodeRow(MouseEventArgs *e)
	{
		IEnumerator *keyEnum = nodeRowRects->Keys->GetEnumerator();
		IEnumerator *valueEnum = nodeRowRects->Values->GetEnumerator();

		while (keyEnum->MoveNext() && valueEnum->MoveNext())
		{
			System::Drawing::Rectangle r = *dynamic_cast<System::Drawing::Rectangle *>(keyEnum->Current);

			if ((r.Left <= e->X) && (r.Left + r.Width >= e->X) &&
				(r.Top <= e->Y) && (r.Top + r.Height >= e->Y))
			{
				return dynamic_cast<TreeListNode *>(valueEnum->Current);
			}
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		NodeAtYPos
	// Description: gets the node at a specified location
	//
	// Parameters:
	//	yPos - the y offset to get the node at
	//
	// Return Value: the node at the y location
	//*************************************************************************
	TreeListNode *TreeListView::NodeAtYPos(int yPos)
	{
		IEnumerator *keyEnum = nodeRowRects->Keys->GetEnumerator();
		IEnumerator *valueEnum = nodeRowRects->Values->GetEnumerator();

		while (keyEnum->MoveNext() && valueEnum->MoveNext())
		{
			System::Drawing::Rectangle r = *dynamic_cast<System::Drawing::Rectangle *>(keyEnum->Current);

			if ((r.Top <= yPos) && (r.Top + r.Height >= yPos))
			{
				return dynamic_cast<TreeListNode *>(valueEnum->Current);
			}
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		NodePlusClicked
	// Description: called when the plus is clicked to expand or collapse a node
	//
	// Parameters:
	//	e - the args representing the mouse click
	//
	// Return Value: the node whose plus was clicked
	//*************************************************************************
	TreeListNode *TreeListView::NodePlusClicked(MouseEventArgs *e)
	{
		IEnumerator *keyEnum = pmRects->Keys->GetEnumerator();
		IEnumerator *valueEnum = pmRects->Values->GetEnumerator();

		while (keyEnum->MoveNext() && valueEnum->MoveNext())
		{
			System::Drawing::Rectangle r = *dynamic_cast<System::Drawing::Rectangle *>(keyEnum->Current);

			if ((r.Left <= e->X) && (r.Left + r.Width >= e->X) &&
				(r.Top <= e->Y) && (r.Top + r.Height >= e->Y))
			{
				return dynamic_cast<TreeListNode *>(valueEnum->Current);
			}
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		RenderNodeRows
	// Description: draws the node rows in the UI
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	r - the area to draw the rows in
	//	totalRend (in/out) - the total number of rows rendered so far
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderNodeRows(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int *totalRend)
	{
		if (!node)
			return;

		int childCount = 0;
		int count = 0;
		int index = -1;

		if (showRootLines)
		{
			TreeListNode *parentNode = dynamic_cast<TreeListNode *>(node->ParentNode());
			TreeListNode *prevSiblingNode = dynamic_cast<TreeListNode *>(node->PreviousSibling());

			if (parentNode)
				count = parentNode->ChildrenCount;

			if (parentNode && parentNode->GetNodeIndex)
				index = parentNode->GetNodeIndex->Invoke(parentNode, node);

			if (prevSiblingNode && prevSiblingNode->GetVisibleNodeCount)
				childCount = prevSiblingNode->GetVisibleNodeCount->Invoke(prevSiblingNode);
		}

		int level = node->Level();
		
		if (node->IsVisible)
		{
			int edgeBuffer = 2;
			if (showRootLines)
				edgeBuffer = 10;

			// only render the row if it is in the viewport
			if (((r.Top + (rowHeight * *totalRend) + (edgeBuffer / 4) + rowHeight) >= r.Top + 2) &&
				((r.Top + (rowHeight * *totalRend) + (edgeBuffer / 4)) < (r.Top + r.Height)))
			{
				rendCount++;
				int levelBuffer = 0;
				int iconBuffer = 0;
				int hBuffer = headerBuffer;
				Pen *linePen = new Pen(SystemBrushes::ControlDark, 1.0);
				Pen *PMPen = new Pen(SystemBrushes::ControlDark, 1.0);
				Pen *PMPen2 = new Pen(new SolidBrush(Color::Black), 1.0);

				linePen->DashStyle = System::Drawing::Drawing2D::DashStyle::Dot;

				// add space for plus/minus icons and root lines to the edge buffer
				if (showRootLines || showPlusMinus)
					edgeBuffer += 10;

				// set level buffer
				levelBuffer = indent * level;

				// set icon buffer
				iconBuffer = GetIconBufferForRowRender(node, g, r, totalRend, levelBuffer, edgeBuffer, hBuffer);

				// add a rectange to the node row rectangles
				System::Drawing::Rectangle sr = System::Drawing::Rectangle(r.Left + levelBuffer + iconBuffer + edgeBuffer + 4 - hScrollBar->Value, 
					r.Top + hBuffer + (rowHeight * *totalRend) + 2, 
					allColsWidth - (levelBuffer + iconBuffer + edgeBuffer + 4), rowHeight); 

				Object *tempSr = __box(sr);
				if (!nodeRowRects->Contains(tempSr))
					nodeRowRects->Add(tempSr, node);

				int colWidth = (columns->get_Item(0)->ScaleStyle == ColumnScaleStyle::Spring ? springWidth : columns->get_Item(0)->Width);

				RenderPerItemBackground(node, g, r, totalRend, iconBuffer, levelBuffer, edgeBuffer, hBuffer, colWidth);

				RenderItemSelectionAndFocus(node, g, sr);

				if (showRootLines)
				{
					RenderRootLines(node, g, linePen, r, edgeBuffer, hBuffer, level, index, count, childCount, totalRend);
					RenderChildLines(node, g, linePen, r, levelBuffer, edgeBuffer, hBuffer, level, index, count, childCount, totalRend);
				}

				RenderPlusAndMinusLines(node, g, r, edgeBuffer, levelBuffer, iconBuffer, hBuffer, totalRend);

				// render the text if visible
				if (r.Left + colWidth - hScrollBar->Value > r.Left)
				{
					if ((!disableSelection) && node->Selected && (isFocused || fullHighlightOnHideSelection))
					{
						g->DrawString(TruncatedString(node->Text, Font, columns->get_Item(0)->Width, 
							levelBuffer + edgeBuffer + iconBuffer + 4, g), Font, 
							SystemBrushes::HighlightText, 
							(float)(r.Left + levelBuffer + iconBuffer + edgeBuffer + 4 - hScrollBar->Value), 
							(float)(r.Top + hBuffer + (rowHeight * *totalRend) + (edgeBuffer / 4) + 1));
					}
					else
					{
						g->DrawString(TruncatedString(node->Text, Font, columns->get_Item(0)->Width, 
							levelBuffer + edgeBuffer + iconBuffer + 4, g), Font, 
							new SolidBrush(node->ForeColor), 
							(float)(r.Left + levelBuffer + iconBuffer + edgeBuffer + 4 - hScrollBar->Value), 
							(float)(r.Top + hBuffer + (rowHeight * *totalRend) + (edgeBuffer / 4) + 1));
					}
				}

				RenderSubItems(node, g, r, hBuffer, totalRend);
			}
		}
	}

	//*************************************************************************
	// Method:		GetIconBufferForRowRender
	// Description: returns the amount of space needed to draw an icon
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	r - the area to draw the rows in
	//	totalRend (in/out) - the total number of rows rendered so far
	//	lb - the level buffer of the node
	//	eb - the edge buffer of the node
	//	hb - the header buffer of the node
	//
	// Return Value: the space needed to draw the icon
	//*************************************************************************
	int TreeListView::GetIconBufferForRowRender(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int *totalRend, int lb, int eb, int hb)
	{
		if ((node->Selected || node->Focused) && stateImageList)
		{
			if ((node->ImageIndex >= 0) && (stateImageList->Images) && (node->ImageIndex < stateImageList->Images->Count))
			{
				if (showRootLines)
					stateImageList->Draw(g, r.Left + lb + eb + 4 - hScrollBar->Value, r.Top + hb + (rowHeight * *totalRend) + (eb / 4), 16, 16, node->ImageIndex);
				else
					stateImageList->Draw(g, r.Left + lb + 4 - hScrollBar->Value, r.Top + hb + (rowHeight * *totalRend) + (eb / 4), 16, 16, node->ImageIndex);
				return 18;
			}
		}
		else
		{
			if (smallImageList && (node->ImageIndex >= 0) && (smallImageList->Images) && (node->ImageIndex < smallImageList->Images->Count))
			{
				if (showRootLines)
					smallImageList->Draw(g, r.Left + lb + eb + 4 - hScrollBar->Value, r.Top + hb + (rowHeight * *totalRend) + (eb / 4), 16, 16, node->ImageIndex);
				else
					smallImageList->Draw(g, r.Left + lb + 4 - hScrollBar->Value, r.Top + hb + (rowHeight * *totalRend) + (eb / 4), 16, 16, node->ImageIndex);
				return 18;
			}
		}

		return 0;
	}

	//*************************************************************************
	// Method:		RenderPerItemBackground
	// Description: draws the background for each node and subitem
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	r - the area to draw the rows in
	//	totalRend (in/out) - the total number of rows rendered so far
	//	ib - the icon buffer of the node
	//	lb - the level buffer of the node
	//	eb - the edge buffer of the node
	//	hb - the header buffer of the node
	//	colWidth - the column width
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderPerItemBackground(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int *totalRend, int ib, int lb, int eb, int hb, int colWidth)
	{
		// render per-item background
		if (node->BackColor != BackColor)
		{
			if (node->UseItemStyleForSubItems)
			{
				if (fullBackground)
				{
					g->FillRectangle(new SolidBrush(node->BackColor), r.Left - hScrollBar->Value,
						r.Top + hb + (rowHeight * *totalRend) + 2, 
						allColsWidth + 2, rowHeight);
				}
				else
				{
					g->FillRectangle(new SolidBrush(node->BackColor), r.Left + lb + ib + eb + 4 - hScrollBar->Value,
						r.Top + hb + (rowHeight * *totalRend) + 2, 
						allColsWidth - (lb + ib + eb + 4), rowHeight);
				}
			}
			else
			{
				if (fullBackground)
				{
					g->FillRectangle(new SolidBrush(node->BackColor), r.Left - hScrollBar->Value,
						r.Top + hb + (rowHeight * *totalRend) + 2, 
						colWidth, rowHeight);
				}
				else
				{
					g->FillRectangle(new SolidBrush(node->BackColor), r.Left + lb + ib + eb + 4 - hScrollBar->Value,
						r.Top + hb + (rowHeight * *totalRend) + 2, 
						colWidth - (lb + ib + eb + 4), rowHeight);
				}
			}
		}
	}

	//*************************************************************************
	// Method:		RenderItemSelectionAndFocus
	// Description: draws the selection and focus boxes for each item
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	r - the area to draw the rows in
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderItemSelectionAndFocus(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r)
	{
		if (!disableSelection)
		{
			// render selection and focus
			if (node->Selected && isFocused)
			{
				g->FillRectangle(new SolidBrush(rowSelectColor), r);
			}
			else if (node->Selected && !isFocused && !hideSelection)
			{
				g->FillRectangle(SystemBrushes::Control, r);
			}
			else if (node->Selected && !isFocused && hideSelection && !fullHighlightOnHideSelection)
			{
				ControlPaint::DrawFocusRectangle(g, r);
			}
			else if (node->Selected && !isFocused && hideSelection && fullHighlightOnHideSelection)
			{
				g->FillRectangle(new SolidBrush(rowSelectColor), r);
			}

			if (node->Focused && ((isFocused && multiSelect) || !node->Selected))
			{
				ControlPaint::DrawFocusRectangle(g, r);
			}
		}
	}

	//*************************************************************************
	// Method:		RenderRootLines
	// Description: draws the lines from the node to the root nodes
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	linePen - the Pen object to draw the lines with
	//	r - the area to draw the rows in
	//	eb - the edge buffer of the node
	//	hb - the header buffer of the node
	//	level - the depth level of the node
	//	index - the index of the node
	//	count - the node count
	//	childCount - the number of child nodes of the root node
	//  totalRend (in/out) - the total number of rows rendered so far
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderRootLines(TreeListNode *node, Graphics *g, Pen *linePen, System::Drawing::Rectangle r, int eb, int hb, int level, int index, int count, int childCount, int *totalRend)
	{
		bool moreSiblingNodes = (node->NextSibling() != NULL);
		if (r.Left + eb - hScrollBar->Value > r.Left)
		{
			if (showRootLines && (level == 0))
			{
				if (index == 0)
				{
					// draw horizontal line with length eb/2
					g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value, r.Top + (eb / 2) + hb,
						r.Left + eb - hScrollBar->Value, r.Top + (eb / 2) + hb);
					if (moreSiblingNodes)
					{
						// draw vertical line with length of eb/2
						g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value, r.Top + (eb / 2) + hb,
							r.Left + (eb / 2) - hScrollBar->Value, r.Top + eb + hb);
					}
				}
				else if (index == count - 1)
				{
					// draw horizontal line with a length of eb / 2 offset vertically by rowHeight * totalRend
					g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend), 
						r.Left + eb - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend));
					// draw a vertical line with a length of eb / 2 offset vertically by rowHeight * totalRend
					g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value, 
						r.Top + hb + (rowHeight * *totalRend),
						r.Left + (eb / 2) - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend));
				}
				else
				{
					// draw horizontal line with a length of eb / 2 offset vertically by rowHeight * totalRend
					g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value, 
						r.Top + eb + hb + (rowHeight * *totalRend) - (eb / 2), 
						r.Left + eb - hScrollBar->Value, 
						r.Top + eb + hb + (rowHeight * *totalRend) - (eb / 2));

					// draw vertical line with a length of eb / 2 offset vertically by rowHeight * totalRend
					if (moreSiblingNodes)
					{
						g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value, 
							r.Top + eb + hb + (rowHeight * (*totalRend - 1)), 
							r.Left + (eb / 2) - hScrollBar->Value, 
							r.Top + eb + hb + (rowHeight * *totalRend));
					}
				}

				if (childCount > 0)
				{
					g->DrawLine(linePen, r.Left + (eb / 2) - hScrollBar->Value,
						r.Top + hb + (rowHeight * (*totalRend - childCount)),
						r.Left + (eb / 2) - hScrollBar->Value,
						r.Top + hb + (rowHeight  * *totalRend));
				}
			}
		}
	}

	//*************************************************************************
	// Method:		RenderChildLines
	// Description: draws the lines from the node to the child nodes
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	linePen - the Pen object to draw the lines with
	//	r - the area to draw the rows in
	//	lb - the level buffer of the node
	//	eb - the edge buffer of the node
	//	hb - the header buffer of the node
	//	level - the depth level of the node
	//	index - the index of the node
	//	count - the node count
	//	childCount - the number of child nodes of the root node
	//  totalRend (in/out) - the total number of rows rendered so far
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderChildLines(TreeListNode *node, Graphics *g, Pen *linePen, System::Drawing::Rectangle r, int lb, int eb, int hb, int level, int index, int count, int childCount, int *totalRend)
	{
		// render child lines if visible
		if (r.Left + lb + eb - hScrollBar->Value > r.Left)
		{
			if (showLines && (level > 0))
			{
				if (index == count - 1)
				{
					g->DrawLine(linePen, r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend), 
						r.Left + lb + eb - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend));
					g->DrawLine(linePen, r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + hb + (rowHeight * *totalRend), 
						r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend));
				}
				else
				{
					g->DrawLine(linePen, r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend), 
						r.Left + lb + eb - hScrollBar->Value, 
						r.Top + (eb / 2) + hb + (rowHeight * *totalRend));
					g->DrawLine(linePen, r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + hb + (rowHeight * *totalRend), 
						r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + eb + hb + (rowHeight * *totalRend));
				}

				if (childCount > 0)
				{
					g->DrawLine(linePen, r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + hb + (rowHeight * (*totalRend-childCount)), 
						r.Left + lb + (eb / 2) - hScrollBar->Value, 
						r.Top + hb + (rowHeight * *totalRend));
				}
			}
		}
	}

	//*************************************************************************
	// Method:		RenderPlusAndMinuses
	// Description: draws the plus or minus next to the node
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	r - the area to draw the rows in
	//	eb - the edge buffer of the node
	//	lb - the level buffer of the node
	//  ib - the icon buffer of the node
	//	hb - the header buffer of the node
	//  totalRend (in/out) - the total number of rows rendered so far
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderPlusAndMinusLines(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int eb, int lb, int ib, int hb, int *totalRend)
	{
		if (r.Left + lb + (eb / 2) + 5 - hScrollBar->Value > r.Left)
		{
			if (showPlusMinus && ((node->ChildrenCount > 0) || alwaysShowPM))
			{
				if (showRootLines)
				{
					RenderPlus(g, r.Left + lb + (eb / 2) - 4 - hScrollBar->Value, 
						r.Top + hb + (rowHeight * *totalRend) + (rowHeight / 2) - 3, 8, 8, node);
				}
				else
				{
					RenderPlus(g, r.Left + lb + ib + (eb / 2) - 2 - hScrollBar->Value, 
						r.Top + hb + (rowHeight * *totalRend) + (rowHeight / 2) - 3, 8, 8, node);
				}
			}
		}
	}

	//*************************************************************************
	// Method:		RenderSubItems
	// Description: draws the node's sub items
	//
	// Parameters:
	//	node - the node to draw
	//	g - the Graphics object to draw with
	//	r - the area to draw the rows in
	//	hb - the header buffer of the node
	//  totalRend (in/out) - the total number of rows rendered so far
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderSubItems(TreeListNode *node, Graphics *g, System::Drawing::Rectangle r, int hb, int *totalRend)
	{
		int j;
		int last = 0;
		if (columns->Count > 0)
		{
			for (j = 0; ((j < node->SubItems->Count) && (j < columns->Count)); j++)
			{
				int colWidth = (columns->get_Item(j)->ScaleStyle == ColumnScaleStyle::Spring ? springWidth : columns->get_Item(j)->Width);
				int colPlus1Width = (columns->get_Item(j + 1)->ScaleStyle == ColumnScaleStyle::Spring ? springWidth : columns->get_Item(j + 1)->Width);

				last += colWidth;

				Control *c = node->SubItems->get_Item(j)->ItemControl;
				if (c)
				{
					c->Location = System::Drawing::Point(r.Left + last + 4 - hScrollBar->Value,
						r.Top + (rowHeight * *totalRend) + hb + 4);
					c->ClientSize = System::Drawing::Size(colPlus1Width - 6, rowHeight - 4);
					c->Tag = __box(true);
					c->Visible = true;
					c->Parent = this;
					c->Invalidate();
					c->Update();
				}
				else
				{
					String *sp = TruncatedString(node->SubItems->get_Item(j)->Text, Font, colPlus1Width, 9, g);
					Brush *brush = (node->UseItemStyleForSubItems ? new SolidBrush(node->ForeColor) :
						new SolidBrush(node->SubItems->get_Item(j)->ForeColor));
					if (columns->get_Item(j + 1)->TextAlign == HorizontalAlignment::Left)
					{
						if (node->Selected && (isFocused || fullHighlightOnHideSelection))
						{
							g->DrawString(sp, Font, SystemBrushes::HighlightText, 
								(float)(last + 6 - hScrollBar->Value), 
								(float)(r.Top + (rowHeight * *totalRend) + hb + 4));
						}
						else
						{
							g->DrawString(sp, Font, 
								brush, (float)(last + 6 - hScrollBar->Value), 
								(float)(r.Top + (rowHeight * *totalRend) + hb + 4));
						}
					}
					else if (columns->get_Item(j + 1)->TextAlign == HorizontalAlignment::Right)
					{
						int sw = StringTools::MeasureDisplayStringWidth(g, sp, Font);
						if (node->Selected && (isFocused || fullHighlightOnHideSelection))
						{
							g->DrawString(sp, Font, SystemBrushes::HighlightText, 
								(float)(last + colPlus1Width - sw - 4 - hScrollBar->Value), 
								(float)(r.Top + (rowHeight * *totalRend) + hb + 4));
						}
						else
						{
							g->DrawString(sp, Font, brush, 
								(float)(last + colPlus1Width - sw - 4 - hScrollBar->Value), 
								(float)(r.Top + (rowHeight * *totalRend) + hb + 4));
						}
					}
					else
					{
						int sw = StringTools::MeasureDisplayStringWidth(g, sp, Font);
						if (node->Selected && (isFocused || fullHighlightOnHideSelection))
						{
							g->DrawString(sp, Font, SystemBrushes::HighlightText, 
								(float)(last + (colPlus1Width / 2) - (sw / 2) - hScrollBar->Value), 
								(float)(r.Top + (rowHeight * *totalRend) + hb + 4));
						}
						else
						{
							g->DrawString(sp, Font, brush, 
								(float)(last+(colPlus1Width / 2) - (sw / 2) - hScrollBar->Value), 
								(float)(r.Top + (rowHeight * *totalRend) + hb + 4));
						}
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		RenderPlus
	// Description: draws the plus or minus sign
	//
	// Parameters:
	//	g - the Graphics object to draw with
	//	x - the x coord to draw it at
	//	y - the y coord to draw it at
	//	w - the width of the box
	//	h - the height of the box
	//	node - the node to draw it next to
	//
	// Return Value: None
	//*************************************************************************
	void TreeListView::RenderPlus(Graphics *g, int x, int y, int w, int h, TreeListNode *node)
	{
		if (visualStyle && minusBitmap && plusBitmap)
		{
			if (node->IsExpanded)
			{
				g->DrawImage(minusBitmap, x, y);
			}
			else
			{
				g->DrawImage(plusBitmap, x, y);
			}
		}
		else
		{
			g->DrawRectangle(new Pen(SystemBrushes::ControlDark), x, y, w, h);
			g->FillRectangle(new SolidBrush(Color::White), x + 1, y + 1, w - 1, h - 1);
			g->DrawLine(new Pen(new SolidBrush(Color::Black)), x + 2, y + 4, x + w - 2, y + 4);

			if (!node->IsExpanded)
				g->DrawLine(new Pen(new SolidBrush(Color::Black)), x + 4, y + 2, x + 4, y + h - 2);
		}

		try
		{
			Object *r = __box(System::Drawing::Rectangle(x, y, w, h));
			if (!pmRects->Contains(r))
				pmRects->Add(r, node);
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		FirstNodeRelativeToCurrentNode
	// Description: returns the index of the first node relative to the current
	//	node
	//
	// Parameters:
	//	None
	//
	// Return Value: the index of the first node relative to the current
	//*************************************************************************
	int TreeListView::FirstNodeRelativeToCurrentNode()
	{
		Stack *firstSelectedNodeStack = new Stack();
		Stack *currNodeStack = new Stack();

		// find the root for both ends of the selected range
		firstSelectedNode->GetStackToVirtualParent(firstSelectedNodeStack);
		currNode->GetStackToVirtualParent(currNodeStack);

		// find the point of divergence
		while ((firstSelectedNodeStack->Count > 1) && (currNodeStack->Count > 1))
		{
			// look at the first item and pop them off if they are the same
			if (firstSelectedNodeStack->Peek() != currNodeStack->Peek())
				break;

			firstSelectedNodeStack->Pop();
			currNodeStack->Pop();
		}

		// At this point there are two nodes at the same level in the tree, one at the
		// top of each stack.  Use this information to determine which appears first in
		// its parents node collection.
		int result = (dynamic_cast<TreeListNode *>(firstSelectedNodeStack->Peek()))->CompareTo(dynamic_cast<TreeListNode *>(currNodeStack->Peek()));
		if (result != 0)
			return -result;

		// If iResult = 0, it's going to be because the curNode is the parent of the firstnode 
		// or because the firstnode is the parent of the curNode
		if (firstSelectedNodeStack->Count > currNodeStack->Count)
			return -1;
		
		return 1;
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
	void TreeListView::DrawBackground(Graphics *g, System::Drawing::Rectangle r)
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
			lHeight = 1;

			// horizontal
			for (unsigned int j = topItemIndex; j < bottomItemIndex; j++)
			{
				g->DrawLine(p, r.Left + 2, r.Top + headerBuffer + rowHeight + lHeight, r.Left + r.Width, r.Top + headerBuffer + rowHeight + lHeight);
				lHeight += rowHeight;
			}
		}
	}
}