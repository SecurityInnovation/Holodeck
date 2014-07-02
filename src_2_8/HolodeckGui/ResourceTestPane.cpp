//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourceTestPane.cpp
//
// DESCRIPTION: Contains some implementation for the class ResourceTestPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Dec 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ResourceTestPane.h"
#include "FieldChooser.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ResourceTestPane
	// Description: Constructor for the ResourceTestPane class
	//
	// Parameters:
	//	dataArray	 - the dataArray that this pane should be associated with
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestPane::ResourceTestPane(ResourceTestDataArray *DataArray)
	{
		uiEventRegistry = UIEventRegistry::getInstance();

		this->dataArray = DataArray;

		addedDelegate = new ItemAddedDelegate(this, onItemAdded);
		deletedDelegate = new ItemDeletedDelegate(this, onItemDeleted);
		this->dataArray->onItemAdded += addedDelegate;
		this->dataArray->onItemDeleted += deletedDelegate;
		ItemCheckEventHandler *listView_ItemCheckEventHandler =  new ItemCheckEventHandler (this, listView_ItemCheckEvent);

		// create the ListView
		this->listView = new CheckBoxListView();
		listView->Dock = DockStyle::Fill;
		listView->FullRowSelect = true;
		listView->Scrollable = true;
		listView->MultiSelect = false;
		listView->AllowColumnReorder = true;

		allColumns = new ArrayList();

		onOffHeader = new ColumnHeader();
		onOffHeader->Text = "On/Off";
		onOffHeader->Width = 50;

		resourceNameHeader = new ColumnHeader();
		resourceNameHeader->Text = "Resource Name";
		resourceNameHeader->Width = 200;

		faultNameHeader = new ColumnHeader();
		faultNameHeader->Text = "Type";
		faultNameHeader->Width = 200;

		listView->Columns->Add(onOffHeader);
		listView->Columns->Add(resourceNameHeader);
		listView->Columns->Add(faultNameHeader);

		allColumns->Add(onOffHeader);
		allColumns->Add(resourceNameHeader);
		allColumns->Add(faultNameHeader);

		this->Controls->Add(listView);

		LoadSettings(PaneName);

		this->listView->MouseUp += new MouseEventHandler(this, onMouseUp);
		this->listView->KeyDown += new KeyEventHandler(this, onKeyDown);
		this->listView->DoubleClick += new EventHandler(this, onDoubleClick);
		this->listView->ContextMenu = new System::Windows::Forms::ContextMenu();
		this->listView->SelectedIndexChanged += new EventHandler(this, onSelectedIndexChanged);

		allColumnWidths = new ArrayList();
		IEnumerator *enumerator = AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ColumnHeader *header = dynamic_cast<ColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			allColumnWidths->Add(__box(header->Width));
		}

		catchCheckEvent = true;
		this->listView->ItemCheck += listView_ItemCheckEventHandler;
	}

	//*************************************************************************
	// Method:		~ResourceTestPane
	// Description: Destructor for the ResourceTestPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourceTestPane::~ResourceTestPane()
	{
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Called by destructor and any other code that wants to release resources
	//
	// Parameters:
	//	disposing - flag to specify managed or unmanaged resource release.
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::Dispose(bool disposing)
	{
		dataArray->onItemDeleted -= deletedDelegate;
		dataArray->onItemAdded -= addedDelegate;
	}

	//*************************************************************************
	// Method:		get_VisibleColumns
	// Description: get method of the VisibleColumns property
	//
	// Parameters:
	//	None
	//
	// Return Value: the visible column headers to get
	//*************************************************************************
	ArrayList *ResourceTestPane::get_VisibleColumns()
	{
		ArrayList *returnValue = new ArrayList();

		IEnumerator *enumerator = AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
			returnValue->Add(enumerator->Current);

		return returnValue;
	}

	//*************************************************************************
	// Method:		set_VisibleColumns
	// Description: set method of the VisibleColumns property
	//
	// Parameters:
	//	headers - the column headers to set
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::set_VisibleColumns(ArrayList *headers)
	{
		if (headers == NULL)
			return;

		// remove the columns that should not be here
		IEnumerator *enumerator = AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ColumnHeader *header = dynamic_cast<ColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (listView->Columns->Contains(header))
			{
				if (!headers->Contains(header))
				{
					if (allColumnWidths)
						allColumnWidths->set_Item(AllColumns->IndexOf(header), __box(header->Width));

					// listview has the column, new columns array does not.  remove the column
					listView->Columns->Remove(header);
				}
			}
		}

		// add the columns that should be here
		enumerator = AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ColumnHeader *header = dynamic_cast<ColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (!listView->Columns->Contains(header))	
			{
				if (headers->Contains(header))
				{
					// listview does not have the column, but new columns array does.  add the column

					// try to put it back in the right place.  Note
					// reordering before the hide and re-show is ignored.

					// try to figure out where to insert the column
					int i = AllColumns->IndexOf(header);
					int insertLocation = i;
					int count = 0;

					ColumnHeader *colHeader = NULL;
					IEnumerator *colEnumerator = AllColumns->GetEnumerator();
					while ((colEnumerator->MoveNext()) && (count <= i))
					{
						colHeader = dynamic_cast<ColumnHeader *>(colEnumerator->Current);
						if (((colHeader == NULL) || (!listView->Columns->Contains(colHeader))) && (count != i))
							insertLocation--;

						count++;
					}

					if (colHeader != NULL)
					{
						if (allColumnWidths)
							colHeader->Width = *dynamic_cast<__box int *>(allColumnWidths->get_Item(i));

						// insert the column header
						listView->Columns->Insert(insertLocation, colHeader);
					}
				}
			}
		}

		enumerator = listView->Items->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ListViewItem *item = dynamic_cast<ListViewItem *>(enumerator->Current);
			if (!item)
				continue;

			UpdateListViewItemStrings(item);
		}
	}

	//*************************************************************************
	// Method:		AddListViewItemString
	// Description: Adds a string to the list view item
	//
	// Parameters:
	//	item - the list view item to add the text to
	//	text - the text to set
	//  asSubItem - true to add the string as a sub item, false to set the list
	//	 view item's text
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::AddListViewItemString(ListViewItem *item, String *text, bool asSubItem)
	{
		if (!asSubItem)
			item->Text = text;
		else
			item->SubItems->Add(text);
	}

	//*************************************************************************
	// Method:		AddStringsToListViewItem
	// Description: Adds strings to a list view item based on node data
	//
	// Parameters:
	//	item - the item to update
	//	node - the node to get the data from
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::AddStringsToListViewItem(ListViewItem *item, ResourceTestDataNode *node)
	{
		bool asSubItem = false;
		if (listView->Columns->Contains(onOffHeader))
		{
			if (node->IsTestOn)
				item->Checked = true;
			else
				item->Checked = false;
			asSubItem = true;
		}

		if (listView->Columns->Contains(resourceNameHeader))
		{
			AddListViewItemString(item, node->Name, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(faultNameHeader))
		{
			AddListViewItemString(item, node->FaultName, asSubItem);
			asSubItem = true;
		}
	}

	//*************************************************************************
	// Method:		UpdateListViewItemStrings
	// Description: Updates a list view items with the correct strings after
	//	visible columns have changed
	//
	// Parameters:
	//	item - the item to update
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::UpdateListViewItemStrings(ListViewItem *item)
	{
		if (!item)
			return;

		ResourceTestDataNode *node = dynamic_cast<ResourceTestDataNode *>(dataArray->GetDataNodeByID(dynamic_cast<String *>(item->Tag)));
		if (!node)
			return;

		item->Text = "";
		item->SubItems->Clear();

		AddStringsToListViewItem(item, node);
	}

	//*************************************************************************
	// Method:		ConvertResourceTestToListViewItem
	// Description: Converts a resource test node to a list view item object
	//
	// Parameters:
	//	node - the node to convert
	//
	// Return Value: the converted list view item object
	//*************************************************************************
	ListViewItem *ResourceTestPane::ConvertResourceTestToListViewItem(ResourceTestDataNode *node)
	{
		bool asSubItem = false;
		ListViewItem *newListViewItem = new ListViewItem();

		AddStringsToListViewItem(newListViewItem, node);

		newListViewItem->Tag = node->ID; // Store the data node as a tag so we can find it later

		return newListViewItem;
	}

	//*************************************************************************
	// Method:		onItemAdded
	// Description: called when a new resource test is added in the data array
	//
	// Parameters:
	//	node - the node representing 
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::onItemAdded(DisplayableDataNode *node)
	{
		ResourceTestDataNode *rtNode = dynamic_cast<ResourceTestDataNode*>(node);
		if (rtNode != NULL)
		{
			this->catchCheckEvent = false;
			ListViewItem *newListViewItem = this->ConvertResourceTestToListViewItem(rtNode);
			rtNode->Element = newListViewItem;
			this->listView->Items->Add(newListViewItem);
			this->catchCheckEvent = true;
		}
	}

	//*************************************************************************
	// Method:		onItemDeleted
	// Description: called when a resource test is removed from the data array
	//
	// Parameters:
	//	node - the node representing
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::onItemDeleted(DisplayableDataNode *node)
	{
		ResourceTestDataNode *rtNode = dynamic_cast<ResourceTestDataNode *>(node);
		if (rtNode != NULL)
		{
			for (int i=0; i<listView->Items->Count; i++)
			{
				String* nodeID = dynamic_cast<String*>(listView->Items->Item[i]->Tag);
				if (nodeID && nodeID->Equals(node->ID))
				{
					listView->Items->RemoveAt(i);
					break;
				}
			}
		}
	}

	//*************************************************************************
	// Method:		onMouseUp
	// Description: called on mouse up event
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ResourceTestPane::onMouseUp(Object *sender, MouseEventArgs *e)
	{
		if (e->Button == MouseButtons::Right)
		{
			
			if (this->listView->SelectedIndices->Count != 0)
			{
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Copy", new EventHandler(this, onCopyToClipboard)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));

				MenuItem *modifyMenuItem = new MenuItem("Modify Resource Fault...", new EventHandler(this, ModifyResourceTest));
				modifyMenuItem->DefaultItem = true;
				this->listView->ContextMenu->MenuItems->Add(modifyMenuItem);
				
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Delete Resource Fault", new EventHandler(this, DeleteResourceTest)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));
			}
			this->listView->ContextMenu->MenuItems->Add(new MenuItem("Create a new Resource Fault", new EventHandler(this, CreateResourceTest)));
			this->listView->ContextMenu->MenuItems->Add(new MenuItem("Select Visible Columns...", new EventHandler(this, onShowFieldChooser)));

			Point point(e->get_X(), e->get_Y());
			this->listView->ContextMenu->Show(this->listView, point);
			this->listView->ContextMenu->MenuItems->Clear();
		}
	}

	//*************************************************************************
	// Method:		onDoubleClick
	// Description: called on double click event
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ResourceTestPane::onDoubleClick(Object *sender, EventArgs *e)
	{
		if (this->listView->SelectedIndices->Count != 0)
			ModifyResourceTest(NULL, NULL);
	}

	//*************************************************************************
	// Method:		DeleteResourceTest
	// Description: Deletes a test
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ResourceTestPane::DeleteResourceTest(Object *sender, EventArgs *e)
	{
		DisplayableDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<DisplayableDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));
			dataArray->BeginDeleteNode(dataNode);
		}
	}

	//*************************************************************************
	// Method:		CreateResourceTest
	// Description: Creates a test
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ResourceTestPane::CreateResourceTest(Object *sender, EventArgs *e)
	{
		ResourceTestDataNode *dataNode = new ResourceTestDataNode("", "", FaultType::CustomFault, this->ProcessID, 0);

		uiEventRegistry->OnStartCreateResourceTestWizard->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		ModifyResourceTest
	// Description: Modifies a test
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ResourceTestPane::ModifyResourceTest(Object *sender, EventArgs *e)
	{
		ResourceTestDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<ResourceTestDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));
			uiEventRegistry->OnStartModifyResourceTestWizard(dataNode);
		}
	}

	//*************************************************************************
	// Method:		onKeyDown
	// Description: called on key press event
	//
	// Parameters:
	//	sender - event originator
	//	e - event arguments
	//
	// Return Value: None
	//*************************************************************************	
	void ResourceTestPane::onKeyDown(Object* sender, KeyEventArgs *e)
	{
		if ((e->KeyData == Keys::Delete) || (e->KeyData == Keys::Back))
			DeleteResourceTest(NULL, NULL);
	}

	//*************************************************************************
	// Method:		onSelectedIndexChanged
	// Description: called when a new listview item is selected
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::onSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (this->listView->SelectedIndices->Count > 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			ResourceTestDataNode *node = dynamic_cast<ResourceTestDataNode *>(this->dataArray->GetDataNodeByID (dynamic_cast <String *> (lvi->Tag)));
			
			if (!node)
				return;

			Object *itemObject[] = new Object *[1];
			itemObject[0] = node;
			this->uiEventRegistry->OnFocusChanged->DynamicInvoke(itemObject);
		}
	}

	//*************************************************************************
	// Method:		onCopyToClipboard
	// Description: Event handler for the Copy context menuitem
	//*************************************************************************	
	void ResourceTestPane::onCopyToClipboard(Object *sender, EventArgs *args)
	{
		if (uiEventRegistry->OnCopyToClipboard)
			uiEventRegistry->OnCopyToClipboard->Invoke();
	}

	//*************************************************************************
	// Method:		onShowFieldChooser
	// Description: called when the a the user clicks the context menu to 
	//	show the field chooser
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourceTestPane::onShowFieldChooser(Object *sender, EventArgs *args)
	{
		Cursor::Current = Cursors::WaitCursor;
		bool resume = false;

		HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataArray->ProcessID);
		if (holoScriptApp && !holoScriptApp->ApplicationPaused)
		{
			holoScriptApp->PauseApplication();
			resume = true;
		}

		FieldChooser *fieldChooser = new FieldChooser(listView, AllColumns);

		int result = uiEventRegistry->OnShowDialog(fieldChooser);
		if (result == DialogResult::OK)
		{
			this->VisibleColumns = fieldChooser->SelectedColumns;
			SaveSettings(PaneName);
			listView->Invalidate();
		}

		if (holoScriptApp && resume)
			holoScriptApp->ResumeApplication();

		Cursor::Current = Cursors::Default;
	}

	//*************************************************************************
	// Method:		listView_ItemCheckEvent
	// Description: Event handler for the listview check changed event
	//*************************************************************************
	void ResourceTestPane::listView_ItemCheckEvent(Object *sender, ItemCheckEventArgs * e)
	{
		if (!this->catchCheckEvent)
			return;

		//select the item first
		listView->Items->Item[e->Index]->Selected = true;

		ListViewItem * lvi = this->listView->Items->Item[e->Index];
		if (!lvi)
			return;

		DisplayableDataNode *dataNode = dynamic_cast<DisplayableDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));
		if (!dataNode)
			return;

		ResourceTestDataNode *rtDataNode = dynamic_cast<ResourceTestDataNode*> (dataNode);
		if (!rtDataNode)
			return;

		rtDataNode->IsTestOn = e->NewValue == CheckState::Checked;

		if (dataNode->ToggleDelegate)
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;

			dataNode->ToggleDelegate->DynamicInvoke(args);
		}
	}
}
