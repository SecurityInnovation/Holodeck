//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ExceptionPane.cpp
//
// DESCRIPTION: Contains some implementation for the class ExceptionPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 25 Feb 2004		 R. Wagner	 File created.
//*************************************************************************
#include <windows.h>
#include <shellapi.h>
#include "ExceptionPane.h"
#include "UserNotification.h"
#include "SiString.h"
#include "FieldChooser.h"

using namespace SiUtils;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ExceptionPane
	// Description: Constructor for the ExceptionPane class
	//
	// Parameters:
	//	dataArray	 - the dataArray that this pane should be associated with
	//	threadID - the thread which the pane belongs to, 0 signifies an all
	//							process pane.
	//
	// Return Value: None
	//*************************************************************************
	ExceptionPane::ExceptionPane(ExceptionDataArray *dataArray, unsigned int ThreadID)
	{
		ArrayList *visibleHeaders;
		ColumnHeader *header;

		this->dataArray = dataArray;
		this->threadID = ThreadID;

		if (threadID != 0)
			this->perThreadMode = true;
		else
			this->perThreadMode = false;
		

		addedDelegate = new ItemAddedDelegate(this, onItemAdded);
		deletedDelegate = new ItemDeletedDelegate(this, onItemDeleted);
		this->dataArray->onItemAdded += addedDelegate;
		this->dataArray->onItemDeleted += deletedDelegate;

		// create the ListView
		listView = new ListView();
		listView->View = System::Windows::Forms::View::Details;
		listView->Dock = DockStyle::Fill;
		listView->FullRowSelect = true;
		listView->Scrollable = true;
		listView->MultiSelect = false;
		listView->AllowColumnReorder = true;

		allColumns = new ArrayList();

		timeStampHeader = new ColumnHeader();
		timeStampHeader->Text = "Time Stamp";
		timeStampHeader->Width = 144;

		exeNameHeader = new ColumnHeader();
		exeNameHeader->Text = "Executable Name";
		exeNameHeader->Width = 144;

		targetThreadHeader = new ColumnHeader();
		targetThreadHeader->Text = "Thread";
		targetThreadHeader->Width = 50;

		exceptionHeader = new ColumnHeader();
		exceptionHeader->Text = "Exception Description";
		exceptionHeader->Width = 400;

		miniDumpHeader = new ColumnHeader();
		miniDumpHeader->Text = "Mini Dump File";
		miniDumpHeader->Width = 400;

		listView->Columns->Add(timeStampHeader);
		listView->Columns->Add(exeNameHeader);
		listView->Columns->Add(targetThreadHeader);
		listView->Columns->Add(exceptionHeader);
		listView->Columns->Add(miniDumpHeader);

		allColumns->Add(timeStampHeader);
		allColumns->Add(exeNameHeader);
		allColumns->Add(targetThreadHeader);
		allColumns->Add(exceptionHeader);
		allColumns->Add(miniDumpHeader);

		this->Controls->Add(listView);

		LoadSettings(PaneName);

		visibleHeaders = this->VisibleColumns;
		if (!visibleHeaders)
			return;

		for (int i=0; i<visibleHeaders->Count; i++)
		{
			header = dynamic_cast <ColumnHeader*> (visibleHeaders->Item[i]);
			// if per-thread setting, then hide threadID column
			if ( (perThreadMode) && (header == targetThreadHeader) )
				visibleHeaders->Remove(header);
		}
		this->VisibleColumns = visibleHeaders;
			
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
	}

	//*************************************************************************
	// Method:		~ExceptionPane
	// Description: Destructor for the ExceptionPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ExceptionPane::~ExceptionPane()
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
	void ExceptionPane::Dispose(bool disposing)
	{
		dataArray->onItemAdded -= addedDelegate;
		dataArray->onItemDeleted -= deletedDelegate;
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
	ArrayList *ExceptionPane::get_VisibleColumns()
	{
		ArrayList *returnValue = new ArrayList();

		IEnumerator *enumerator = listView->Columns->GetEnumerator();
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
	void ExceptionPane::set_VisibleColumns(ArrayList *headers)
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
	// Method:		AddStringsToListViewItem
	// Description: Adds strings to a list view item based on node data
	//
	// Parameters:
	//	item - the item to update
	//	node - the node to get the data from
	//
	// Return Value: None
	//*************************************************************************
	void ExceptionPane::AddStringsToListViewItem(ListViewItem *item, ExceptionDataNode *node)
	{
		bool asSubItem = false;
		if (listView->Columns->Contains(timeStampHeader))
		{
			AddListViewItemString(item, node->TimeStamp, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(exeNameHeader))
		{
			AddListViewItemString(item, node->ExeName, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(targetThreadHeader))
		{
			AddListViewItemString(item, __box(node->threadID)->ToString(), asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(exceptionHeader))
		{
			AddListViewItemString(item, node->Exception, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(miniDumpHeader))
		{
			AddListViewItemString(item, node->MiniDumpFile, asSubItem);
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
	void ExceptionPane::UpdateListViewItemStrings(ListViewItem *item)
	{
		if (!item)
			return;

		ExceptionDataNode *node = dynamic_cast<ExceptionDataNode *>(dataArray->GetDataNodeByID(dynamic_cast<String *>(item->Tag)));
		if (!node)
			return;

		item->Text = "";
		item->SubItems->Clear();

		AddStringsToListViewItem(item, node);
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
	void ExceptionPane::AddListViewItemString(ListViewItem *item, String *text, bool asSubItem)
	{
		if (!asSubItem)
			item->Text = text;
		else
			item->SubItems->Add(text);
	}

	//*************************************************************************
	// Method:		ConvertScheduledTestToListViewItem
	// Description: Converts a scheduled test node to a list view item object
	//
	// Parameters:
	//	node - the node to convert
	//
	// Return Value: the converted list view item object
	//*************************************************************************
	ListViewItem *ExceptionPane::ConvertExceptionToListViewItem(ExceptionDataNode *node)
	{
		ListViewItem *newListViewItem = new ListViewItem();

		AddStringsToListViewItem(newListViewItem, node);
		
		newListViewItem->Tag = node->ID; // Store the data node as a tag so we can find it later

		return newListViewItem;
	}

	//*************************************************************************
	// Method:		onItemAdded
	// Description: called when a new log item is added in the data array
	//
	// Parameters:
	//	node - the node representing 
	//
	// Return Value: None
	//*************************************************************************
	void ExceptionPane::onItemAdded(DisplayableDataNode *node)
	{
		ListViewItem *lvItem;
		ExceptionDataNode *testNode = dynamic_cast <ExceptionDataNode *> (node);

		//Refresh is called whenever a new testpane is created, ensure no duplicates are added to the view
		for (int i=0; i < this->listView->Items->Count; i++)
		{
			lvItem = this->listView->Items->Item[i];
			if (node->ID->CompareTo(dynamic_cast <String*> (lvItem->Tag)) == 0)
				return;
		}

		if ( (testNode != NULL) && ( (node->threadID == threadID) || (threadID == 0)) )
		{
			lvItem = this->ConvertExceptionToListViewItem(testNode);
			testNode->Element = lvItem;
			this->listView->Items->Add(lvItem);
		}
	}

	//*************************************************************************
	// Method:		onItemDeleted
	// Description: called when a new log item is removed from the data array
	//
	// Parameters:
	//	node - the node representing
	//
	// Return Value: None
	//*************************************************************************
	void ExceptionPane::onItemDeleted(DisplayableDataNode *node)
	{
		ExceptionDataNode *testNode = dynamic_cast <ExceptionDataNode *> (node);
		if (testNode != NULL)
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
	void ExceptionPane::onMouseUp(Object *sender, MouseEventArgs *e)	
	{
		if (e->Button == MouseButtons::Right)
		{
			if (this->listView->SelectedIndices->Count != 0)
			{
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Copy", new EventHandler(this, onCopyToClipboard)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));

				MenuItem *viewMenuItem = new MenuItem("View Exception in Debugger", new EventHandler(this, onDoubleClick));
				viewMenuItem->DefaultItem = true;
				this->listView->ContextMenu->MenuItems->Add(viewMenuItem);
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Save Mini Dump As...", new EventHandler(this, SaveException)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Delete Exception", new EventHandler(this, DeleteException)));
			}
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
	void ExceptionPane::onDoubleClick(Object *sender, EventArgs *e)
	{
		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			ExceptionDataNode* exceptionNode = dynamic_cast<ExceptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));
			if (exceptionNode != NULL)
			{
				SiString fileNameString;
				Holodeck::StringConverter::GetInstance()->ConvertToSiString(exceptionNode->MiniDumpFile, &fileNameString);

				ShellExecuteW(NULL, L"open", (wchar_t*)fileNameString, NULL, NULL, SW_SHOW);
			}
		}
	}

	//*************************************************************************
	// Method:		DeleteException
	// Description: Deletes an exception
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ExceptionPane::DeleteException(Object *sender, EventArgs *e)
	{
		ExceptionDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<ExceptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));
			dataArray->BeginDeleteNode(dataNode);
		}
	}

	//*************************************************************************
	// Method:		SaveException
	// Description: Saves an exception to another location
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ExceptionPane::SaveException(Object *sender, EventArgs *e)
	{
		ExceptionDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<ExceptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));

			if (dataNode != NULL)
			{
				SaveFileDialog* sfd = new SaveFileDialog();
				sfd->CheckFileExists = false;
				sfd->CheckPathExists = true;
				sfd->DefaultExt = "dmp";
				sfd->FileName = dataNode->MiniDumpFile;
				sfd->Filter = "Mini dump files (*.dmp)|*.dmp|All files (*.*)|*.*";
				sfd->FilterIndex = 1;
				sfd->OverwritePrompt = true;
				sfd->ValidateNames = true;

				if (UIEventRegistry::getInstance()->OnShowCommonDialog(sfd) == DialogResult::OK)
				{
					try
					{
						File::Copy(dataNode->MiniDumpFile, sfd->FileName, true);
					}
					catch (Exception*)
					{
						UserNotification::ErrorNotify("The mini dump file could not be saved to this location.");
					}
				}
			}
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
	void ExceptionPane::onKeyDown(Object* sender, KeyEventArgs *e)
	{
		if ((e->KeyData == Keys::Delete) || (e->KeyData == Keys::Back))
			DeleteException(NULL, NULL);
	}
	//*************************************************************************
	// Method:		onSelectedIndexChanged
	// Description: called when a new test item is selected
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ExceptionPane::onSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (this->listView->SelectedIndices->Count > 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			ExceptionDataNode *node = dynamic_cast<ExceptionDataNode *>(this->dataArray->GetDataNodeByID (dynamic_cast <String *> (lvi->Tag)));
			if (!node)
				return;

			Object *itemObject[] = new Object *[1];
			itemObject[0] = node;
			this->uiEventRegistry->OnFocusChanged->DynamicInvoke(itemObject);
		}
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
	void ExceptionPane::onShowFieldChooser(Object *sender, EventArgs *args)
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
	// Method:		onCopyToClipboard
	// Description: Event handler for the Copy context menuitem
	//*************************************************************************	
	void ExceptionPane::onCopyToClipboard(Object *sender, EventArgs *args)
	{
		if (uiEventRegistry->OnCopyToClipboard)
			uiEventRegistry->OnCopyToClipboard->Invoke();
	}
}
