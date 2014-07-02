//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SavedFileCorruptionPane.cpp
//
// DESCRIPTION: Contains some implementation for the class SavedFileCorruptionPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#include "SavedFileCorruptionPane.h"
#include "FileCorruptionChangesParser.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		SavedFileCorruptionPane
	// Description: Constructor for the SavedFileCorruptionPane class
	//
	// Parameters:
	//	dataArray	 - the dataArray that this pane should be associated with
	//
	// Return Value: None
	//*************************************************************************
	SavedFileCorruptionPane::SavedFileCorruptionPane(SavedFileCorruptionDataArray *dataArray)
	{
		this->dataArray = dataArray;

		addedDelegate = new ItemAddedDelegate(this, onItemAdded);
		deletedDelegate = new ItemDeletedDelegate(this, onItemDeleted);
		this->dataArray->onItemAdded += addedDelegate;
		this->dataArray->onItemDeleted += deletedDelegate;

		// create the ListView
		this->listView = new ListView();
		listView->View = System::Windows::Forms::View::Details;
		listView->Dock = DockStyle::Fill;
		listView->FullRowSelect = true;
		listView->Scrollable = true;
		listView->MultiSelect = false;
		listView->AllowColumnReorder = true;
		listView->HideSelection = false;

		toolTip = new ToolTip();
		toolTip->AutoPopDelay = 5000;
		toolTip->InitialDelay = 1000;
		toolTip->ReshowDelay = 500;
		toolTip->ShowAlways = true;
		toolTip->SetToolTip(this->listView, "Once a file referenced in a file fuzzing fault has been used by the application,\nselect it here to see the details in the File Fuzzing Details pane");

		allColumns = new ArrayList();

		origFileHeader = new ColumnHeader();
		origFileHeader->Text = "Original File";
		origFileHeader->Width = 350;

		corruptedFileHeader = new ColumnHeader();
		corruptedFileHeader->Text = "Fuzzed File";
		corruptedFileHeader->Width = 350;

		listView->Columns->Add(origFileHeader);
		listView->Columns->Add(corruptedFileHeader);

		allColumns->Add(origFileHeader);
		allColumns->Add(corruptedFileHeader);

		this->Controls->Add(listView);

		LoadSettings(PaneName);

		this->listView->MouseUp += new MouseEventHandler(this, onMouseUp);
		this->listView->KeyDown += new KeyEventHandler(this, onKeyDown);
		this->listView->DoubleClick += new EventHandler(this, onDoubleClick);
		this->listView->ContextMenu = new System::Windows::Forms::ContextMenu();
		this->listView->SelectedIndexChanged += new EventHandler(this, onSelectedIndexChanged);
	}

	//*************************************************************************
	// Method:		~SavedFileCorruptionPane
	// Description: Destructor for the SavedFileCorruptionPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SavedFileCorruptionPane::~SavedFileCorruptionPane()
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
	void SavedFileCorruptionPane::Dispose(bool disposing)
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
	ArrayList *SavedFileCorruptionPane::get_VisibleColumns()
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
	void SavedFileCorruptionPane::set_VisibleColumns(ArrayList *headers)
	{
		if (headers == NULL)
			return;

		IEnumerator *enumerator = allColumns->GetEnumerator();
		int i = 0;
		while (enumerator->MoveNext())
		{
			bool updateIndex = true;

			ColumnHeader *header = dynamic_cast<ColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (listView->Columns->Contains(header))
			{
				if (!headers->Contains(header))
				{
					int numCols = listView->Columns->Count;

					listView->Columns->Remove(header);

					// now we need to move columns to the right of this to the left 1,
					// but we need to make sure there is a column to move
					if (i < numCols)
					{
						// cycle through all the list items
						IEnumerator *itemEnumerator = listView->Items->GetEnumerator();
						while (itemEnumerator->MoveNext())
						{
							ListViewItem *currentItem = dynamic_cast<ListViewItem *>(itemEnumerator->Current);
							if (currentItem)
							{
								// if the one we are moving is first, we have to set the list item text
								if (i == 0)
								{
									if (currentItem->SubItems->Count > 0)
									{
										currentItem->Text = currentItem->SubItems->get_Item(i+1)->Text;
										currentItem->SubItems->RemoveAt(1);
									}
								}
								else if (i < currentItem->SubItems->Count)
									currentItem->SubItems->RemoveAt(i);
							}
						}
					}
					updateIndex = false;
				}
			}
			else
			{
				if (headers->Contains(header))
				{
					// try to put it back in the right place.  Note
					// reordering before the hide and re-show is ignored.

					// try to figure out where to insert the column
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
						// insert the column header
						listView->Columns->Insert(insertLocation, colHeader);

						// cycle through all the list items
						int currentItemIndex = 0;
						IEnumerator *itemEnumerator = listView->Items->GetEnumerator();
						while (itemEnumerator->MoveNext())
						{
							ListViewItem *currentNCEntry;
							SavedFileCorruptionDataNode *ncNode = NULL;

							// get the log entry from the data array to see what to fill in.
							if (dataArray != NULL)
								ncNode = dynamic_cast<SavedFileCorruptionDataNode *>(dataArray->GetDataNodeByIndex(currentItemIndex));

							ListViewItem *currentItem = dynamic_cast<ListViewItem *>(itemEnumerator->Current);
							if (currentItem)
							{
								// if the one we are inserting is first, 
								// we have to set the list item text
								if (insertLocation == 0)
								{
									currentItem->SubItems->Insert(0, new ListViewItem::ListViewSubItem());
									currentItem->SubItems->get_Item(0)->Text = currentItem->Text;

									// get the real value that goes in this spot
									if (ncNode != NULL)
										currentNCEntry = this->ConvertSavedFileCorruptionToListViewItem(ncNode);

									if (currentNCEntry)
										currentItem->Text = currentNCEntry->Text;
									else
										currentItem->Text = "";
								}
								else if (insertLocation >= currentItem->SubItems->Count)
								{
									ListViewItem::ListViewSubItem *newSubItem = new ListViewItem::ListViewSubItem();
									newSubItem->Text = "";
									currentItem->SubItems->Add(newSubItem);

									// get the real value that goes in this spot
									if (ncNode != NULL)
										currentNCEntry = this->ConvertSavedFileCorruptionToListViewItem(ncNode);

									if (currentNCEntry)
										newSubItem->Text = currentNCEntry->SubItems->get_Item(currentItem->SubItems->Count - 1)->Text;
									else
										newSubItem->Text = "";
								}
								else
								{
									ListViewItem::ListViewSubItem *newSubItem = new ListViewItem::ListViewSubItem();
									newSubItem->Text = "";
									currentItem->SubItems->Insert(insertLocation, newSubItem);

									// get the real value that goes in this spot
									if (ncNode != NULL)
										currentNCEntry = this->ConvertSavedFileCorruptionToListViewItem(ncNode);

									if (currentNCEntry)
										newSubItem->Text = currentNCEntry->SubItems->get_Item(insertLocation)->Text;
									else
										newSubItem->Text = "";
								}
							}

							currentItemIndex++;
						}
					}
				}
			}

			if (updateIndex)
				i++;
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
	void SavedFileCorruptionPane::AddListViewItemString(ListViewItem *item, String *text, bool asSubItem)
	{
		if (!asSubItem)
			item->Text = text;
		else
			item->SubItems->Add(text);
	}

	//*************************************************************************
	// Method:		ConvertSavedFileCorruptionToListViewItem
	// Description: Converts a saved file corruption node to a list view item object
	//
	// Parameters:
	//	node - the node to convert
	//
	// Return Value: the converted list view item object
	//*************************************************************************
	ListViewItem *SavedFileCorruptionPane::ConvertSavedFileCorruptionToListViewItem(SavedFileCorruptionDataNode *node)
	{
		bool asSubItem = false;
		ListViewItem *newListViewItem = new ListViewItem();

		if (listView->Columns->Contains(origFileHeader))
		{
			AddListViewItemString(newListViewItem, node->OriginalFile, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(corruptedFileHeader))
		{
			AddListViewItemString(newListViewItem, node->CorruptedFile, asSubItem);
			asSubItem = true;
		}

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
	void SavedFileCorruptionPane::onItemAdded(DisplayableDataNode *node)
	{
		SavedFileCorruptionDataNode *ncNode = dynamic_cast<SavedFileCorruptionDataNode *>(node);
		if ( (ncNode != NULL) && (ncNode->processID == this->ProcessID) )
		{
			ListViewItem *newListViewItem = this->ConvertSavedFileCorruptionToListViewItem(ncNode);
			ncNode->Element = newListViewItem;
			this->listView->Items->Add(newListViewItem);
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
	void SavedFileCorruptionPane::onItemDeleted(DisplayableDataNode *node)
	{
		SavedFileCorruptionDataNode *ncNode = dynamic_cast<SavedFileCorruptionDataNode *>(node);
		if (ncNode != NULL)
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
	void SavedFileCorruptionPane::onMouseUp(Object *sender, MouseEventArgs *e)
	{
		if (e->Button == MouseButtons::Right)
		{
			if (this->listView->SelectedIndices->Count != 0)
			{
				MenuItem *showMenuItem = new MenuItem("Show Details", new EventHandler(this, onDoubleClick));
				showMenuItem->DefaultItem = true;
				this->listView->ContextMenu->MenuItems->Add(showMenuItem);
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Save Original File", new EventHandler(this, SaveOriginalFile)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Save Fuzzed File", new EventHandler(this, SaveCorruptedFile)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Save RTF Details", new EventHandler(this, SaveRTFDetails)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Delete Fuzzed File", new EventHandler(this, DeleteCorruptedFile)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));
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
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************	
	void SavedFileCorruptionPane::onDoubleClick(Object *sender, EventArgs *e)
	{
		if (this->listView->SelectedIndices->Count != 0)
		{
			// Display HTML details (selected item has already been loaded into the pane)
			if (uiEventRegistry->OnShowFileCorruptionDetailsPane)
				uiEventRegistry->OnShowFileCorruptionDetailsPane->Invoke();
		}
	}

	//*************************************************************************
	// Method:		SaveOriginalFile
	// Description: Saves the original file
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void SavedFileCorruptionPane::SaveOriginalFile(Object *sender, EventArgs *e)
	{
		SavedFileCorruptionDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<SavedFileCorruptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));

			SaveFileDialog* sfd = new SaveFileDialog();
			sfd->FileName = dataNode->OriginalFile;
			sfd->Filter = "All files (*.*)|*.*";
			sfd->FilterIndex = 1;
			sfd->OverwritePrompt = true;

			if (UIEventRegistry::getInstance()->OnShowCommonDialog(sfd) == DialogResult::OK)
			{
				try
				{
					File::Copy(dataNode->OriginalFile, sfd->FileName, true);
				}
				catch (Exception* e)
				{
					MessageBox::Show(String::Concat(S"Could not save the original file:\n", e->Message), "Error");
				}
			}
		}
	}

	//*************************************************************************
	// Method:		SaveCorruptedFile
	// Description: Saves the corrupted file
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void SavedFileCorruptionPane::SaveCorruptedFile(Object *sender, EventArgs *e)
	{
		SavedFileCorruptionDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<SavedFileCorruptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));

			SaveFileDialog* sfd = new SaveFileDialog();
			sfd->FileName = dataNode->CorruptedFile;
			sfd->Filter = "All files (*.*)|*.*";
			sfd->FilterIndex = 1;
			sfd->OverwritePrompt = true;

			if (UIEventRegistry::getInstance()->OnShowCommonDialog(sfd) == DialogResult::OK)
			{
				try
				{
					File::Copy(dataNode->CorruptedFile, sfd->FileName, true);
				}
				catch (Exception* e)
				{
					MessageBox::Show(String::Concat(S"Could not save the fuzzed file:\n", e->Message), "Error");
				}
			}
		}
	}

	//*************************************************************************
	// Method:		SaveRTFDetails
	// Description: Saves the details of the corruption changes
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void SavedFileCorruptionPane::SaveRTFDetails(Object *sender, EventArgs *e)
	{
		SavedFileCorruptionDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<SavedFileCorruptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));

			SaveFileDialog* sfd = new SaveFileDialog();
			sfd->FileName = Path::ChangeExtension(dataNode->ChangesFile, ".html");
			sfd->Filter = "RTF files (*.rtf)|*.rtf";
			sfd->FilterIndex = 1;
			sfd->OverwritePrompt = true;

			if (UIEventRegistry::getInstance()->OnShowCommonDialog(sfd) == DialogResult::OK)
			{
				FileCorruptionChangesParser* parser = new FileCorruptionChangesParser();
				if (!parser->Load(dataNode->ChangesFile))
					MessageBox::Show("Could not save the RTF details:\nUnable to read changes file.", "Error");
				else
				{
					if (!parser->SaveRTFDetails(sfd->FileName))
						MessageBox::Show("Could not save the RTF details:\nUnable to write to output file.", "Error");
				}
			}
		}
	}

	//*************************************************************************
	// Method:		DeleteCorruptedFile
	// Description: Deletes a corrupted file
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void SavedFileCorruptionPane::DeleteCorruptedFile(Object *sender, EventArgs *e)
	{
		SavedFileCorruptionDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			dataNode = dynamic_cast<SavedFileCorruptionDataNode*>(dataArray->GetDataNodeByID(dynamic_cast <String *> (lvi->Tag)));
			dataArray->BeginDeleteNode(dataNode);
			try
			{
				File::Delete(dataNode->CorruptedFile);
				File::Delete(dataNode->ChangesFile);
			}
			catch (Exception* e)
			{
				MessageBox::Show(String::Concat(S"Could not delete all files:\n", e->Message), "Error");
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
	void SavedFileCorruptionPane::onKeyDown(Object* sender, KeyEventArgs *e)
	{
		if ((e->KeyData == Keys::Delete) || (e->KeyData == Keys::Back))
			DeleteCorruptedFile(NULL, NULL);
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
	void SavedFileCorruptionPane::onSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (this->listView->SelectedIndices->Count > 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			SavedFileCorruptionDataNode *node = dynamic_cast<SavedFileCorruptionDataNode *>(this->dataArray->GetDataNodeByID (dynamic_cast <String *> (lvi->Tag)));
			
			if (!node)
				return;

			Object *itemObject[] = new Object *[1];
			itemObject[0] = node;
			this->uiEventRegistry->OnFocusChanged->DynamicInvoke(itemObject);
		}
	}
		
	//*************************************************************************
	// Method:		onShowFieldChooser
	// Description: called when the user clicks the context menu to 
	//	show the field chooser
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void SavedFileCorruptionPane::onShowFieldChooser(Object *sender, EventArgs *args)
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
}
