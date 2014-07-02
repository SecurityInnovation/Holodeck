//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ScheduledTestPane.cpp
//
// DESCRIPTION: Contains some implementation for the class ScheduledTestPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "ScheduledTestPane.h"
#include "FieldChooser.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ScheduledTestPane
	// Description: Constructor for the ScheduledTestPane class
	//
	// Parameters:
	//	dataArray	 - the dataArray that this pane should be associated with
	//	threadID - the thread which the pane belongs to, 0 signifies an all
	//							process pane.
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestPane::ScheduledTestPane(ScheduledTestDataArray *dataArray, unsigned int ThreadID)
	{
		uiEventRegistry = UIEventRegistry::getInstance();

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
		ItemCheckEventHandler *listView_ItemCheckEventHandler =  new ItemCheckEventHandler (this, listView_ItemCheckEvent);

		// create the ListView
		listView = new CheckBoxListView();
		listView->Dock = DockStyle::Fill;
		listView->FullRowSelect = true;
		listView->Scrollable = true;
		listView->MultiSelect = false;
		listView->AllowColumnReorder = true;

		allColumns = new ArrayList();

		onOffHeader = new ColumnHeader();
		onOffHeader->Text = "On/Off";
		onOffHeader->Width = 50;

		targetThreadHeader = new ColumnHeader();
		targetThreadHeader->Text = "Target Thread";
		targetThreadHeader->Width = 80;

		functionNameHeader = new ColumnHeader();
		functionNameHeader->Text = "Function";
		functionNameHeader->Width = 140;

		returnValueHeader = new ColumnHeader();
		returnValueHeader->Text = "Return Value";
		returnValueHeader->Width = 80;

		errorCodeHeader = new ColumnHeader();
		errorCodeHeader->Text = "Error Code";
		errorCodeHeader->Width = 140;

		matchParamHeader = new ColumnHeader();
		matchParamHeader->Text = "In Parameters To Match";
		matchParamHeader->Width = 200;

		changeParamHeader = new ColumnHeader();
		changeParamHeader->Text = "Out Parameters To Change";
		changeParamHeader->Width = 200;

		testOwnerHeader = new ColumnHeader();
		testOwnerHeader->Text = "Owner";
		testOwnerHeader->Width = 200;

		pauseAfterTestHeader = new ColumnHeader();
		pauseAfterTestHeader->Text = "Pause";
		pauseAfterTestHeader->Width = 80;

		fireConditionHeader = new ColumnHeader();
		fireConditionHeader->Text = "Fire Condition";
		fireConditionHeader->Width = 210;

		listView->Columns->Add(onOffHeader);
		listView->Columns->Add(targetThreadHeader);
		listView->Columns->Add(functionNameHeader);
		listView->Columns->Add(returnValueHeader);
		listView->Columns->Add(errorCodeHeader);
		listView->Columns->Add(matchParamHeader);
		listView->Columns->Add(changeParamHeader);
		listView->Columns->Add(testOwnerHeader);
		listView->Columns->Add(pauseAfterTestHeader);
		listView->Columns->Add(fireConditionHeader);

		allColumns->Add(onOffHeader);
		allColumns->Add(targetThreadHeader);
		allColumns->Add(functionNameHeader);
		allColumns->Add(returnValueHeader);
		allColumns->Add(errorCodeHeader);
		allColumns->Add(matchParamHeader);
		allColumns->Add(changeParamHeader);
		allColumns->Add(testOwnerHeader);
		allColumns->Add(pauseAfterTestHeader);
		allColumns->Add(fireConditionHeader);

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

		catchCheckEvent = true;
		this->listView->ItemCheck += listView_ItemCheckEventHandler;
	}

	//*************************************************************************
	// Method:		~ScheduledTestPane
	// Description: Destructor for the ScheduledTestPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ScheduledTestPane::~ScheduledTestPane()
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
	void ScheduledTestPane::Dispose(bool disposing)
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
	ArrayList *ScheduledTestPane::get_VisibleColumns()
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
	void ScheduledTestPane::set_VisibleColumns(ArrayList *headers)
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
	void ScheduledTestPane::AddListViewItemString(ListViewItem *item, String *text, bool asSubItem)
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
	void ScheduledTestPane::AddStringsToListViewItem(ListViewItem *item, ScheduledTestDataNode *node)
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

		if (listView->Columns->Contains(targetThreadHeader))
		{
			AddListViewItemString(item, node->threadID.ToString(), asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(functionNameHeader))
		{
			AddListViewItemString(item, node->Function, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(returnValueHeader))
		{
			AddListViewItemString(item, node->ReturnValue, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(errorCodeHeader))
		{
			AddListViewItemString(item, node->ErrorCode, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(matchParamHeader))
		{
			String *matchString = NULL;
			
			if (node->MatchParameters)
			{
				int i = 0;
				IEnumerator *enumerator = node->MatchParameters->GetEnumerator();
				while (enumerator->MoveNext())
				{
					String * paramMatchString = "";
					InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
					if (!param || param->TestCasesCount == 0)
						continue;

					if (i > 0)
						paramMatchString = String::Concat(paramMatchString, " && ");

					for (int mps=0; mps < param->TestCasesCount; mps++)
					{
						ParamTestCase * ptc = dynamic_cast <ParamTestCase *> (param->TestCase (mps));
						//If any of the params has an {Any} as a match, skip it from displaying
						if (ptc->TestValue->CompareTo (IN_PARAM_MATCH_ANY_STRING) == 0)
						{
							paramMatchString = S"skip";
							continue;
						}
						if ( (param->TestCasesCount > 1) && (mps == 0) )
							paramMatchString = String::Concat(paramMatchString, "(");

						paramMatchString = String::Concat(paramMatchString, "(", param->Name, " ");
						paramMatchString = String::Concat(paramMatchString, ptc->TestOperator, " ");
						paramMatchString = String::Concat(paramMatchString, ptc->TestValue, ")");

						if ( (param->TestCasesCount > 1) && (mps < (param->TestCasesCount-1)) )
							paramMatchString = String::Concat(paramMatchString, " OR ");

						if ( (param->TestCasesCount > 1) && (mps == (param->TestCasesCount-1)) )
							paramMatchString = String::Concat(paramMatchString, ")");

					}

					//Concat the string to the master string if skip is not specified
					if (String::Compare (paramMatchString, S"skip") != 0)
					{
						matchString = String::Concat (matchString, paramMatchString);
						i++;
					}
				}
			}

			AddListViewItemString(item, matchString, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(changeParamHeader))
		{
			String *changeString = NULL;
			
			if (node->ChangeParameters)
			{
				int i = 0;
				IEnumerator *enumerator = node->ChangeParameters->GetEnumerator();
				while (enumerator->MoveNext())
				{
					InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(enumerator->Current);
					if (!param || (param->ChangeValue->CompareTo(OUT_PARAM_NO_CHANGE_STRING) == 0))
						continue;

					if (i > 0)
						changeString = String::Concat(changeString, ", ");

					changeString = String::Concat(changeString, param->Name, " => ");
					changeString = String::Concat(changeString, param->ChangeValue);

					i++;
				}
			}

			AddListViewItemString(item, changeString, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(testOwnerHeader))
		{
			AddListViewItemString(item, node->TestOwner, asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(pauseAfterTestHeader))
		{
			AddListViewItemString(item, node->PauseAppOnTestFire? "yes" : "no", asSubItem);
			asSubItem = true;
		}

		if (listView->Columns->Contains(fireConditionHeader))
		{
			String * displayString;

			switch (node->FiringOption)
			{
				case 1:
						displayString = String::Concat ("Run ", dynamic_cast <String *> (node->FiringDetails), "% of the time");
					break;
				case 2:
						displayString = "When called by a function from a list";
					break;
				case 3:
						displayString = "When the callstack matches the specified pattern";
					break;
				case 4:
						displayString = "When any one from defined list is in the callstack";
					break;
				default:
						displayString = "";
					break;
			}

			AddListViewItemString(item, displayString, asSubItem);
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
	void ScheduledTestPane::UpdateListViewItemStrings(ListViewItem *item)
	{
		if (!item)
			return;

		ScheduledTestDataNode *node = dynamic_cast<ScheduledTestDataNode *>(dataArray->GetDataNodeByID(dynamic_cast<String *>(item->Tag)));
		if (!node)
			return;

		item->Text = "";
		item->SubItems->Clear();

		AddStringsToListViewItem(item, node);
		
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
	ListViewItem *ScheduledTestPane::ConvertScheduledTestToListViewItem(ScheduledTestDataNode *node)
	{
		bool asSubItem = false;
		
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
	void ScheduledTestPane::onItemAdded(DisplayableDataNode *node)
	{
		ListViewItem *lvItem;
		ScheduledTestDataNode *testNode = dynamic_cast <ScheduledTestDataNode *> (node);

		//Refresh is called whenever a new testpane is created, ensure no duplicates are added to the view
		for (int i=0; i < this->listView->Items->Count; i++)
		{
			lvItem = this->listView->Items->Item[i];
			if (node->ID->CompareTo(dynamic_cast <String*> (lvItem->Tag)) == 0)
				return;
		}

		if ( (testNode != NULL) && ( (node->threadID == threadID) || (threadID == 0)) )
		{
			this->catchCheckEvent = false;
			lvItem = this->ConvertScheduledTestToListViewItem(testNode);
			testNode->Element = lvItem;
			this->listView->Items->Add(lvItem);
			this->catchCheckEvent = true;
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
	void ScheduledTestPane::onItemDeleted(DisplayableDataNode *node)
	{
		ScheduledTestDataNode *testNode = dynamic_cast <ScheduledTestDataNode *> (node);
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
	void ScheduledTestPane::onMouseUp(Object *sender, MouseEventArgs *e)
	{
		if (e->Button == MouseButtons::Right)
		{
			
			if (this->listView->SelectedIndices->Count != 0)
			{
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Copy", new EventHandler(this, onCopyToClipboard)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Modify Test...", new EventHandler(this, ModifyTest)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("Delete Test", new EventHandler(this, DeleteTest)));
				this->listView->ContextMenu->MenuItems->Add(new MenuItem("-"));
			}
			this->listView->ContextMenu->MenuItems->Add(new MenuItem("Create a new Test", new EventHandler(this, CreateTest)));
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
	void ScheduledTestPane::onDoubleClick(Object *sender, EventArgs *e)
	{
		if (this->listView->SelectedIndices->Count != 0)
			ModifyTest(NULL, NULL);
	}

	//*************************************************************************
	// Method:		CreateTest
	// Description: Creates a test
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ScheduledTestPane::CreateTest(Object *sender, EventArgs *e)
	{
		DisplayableDataNode *dataNode = new DisplayableDataNode("", UIData::Test, this->ProcessID, this->ThreadID);
		uiEventRegistry->OnStartCreateTestWizard(dataNode);
	}

	//*************************************************************************
	// Method:		DeleteTest
	// Description: Deletes a test
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ScheduledTestPane::DeleteTest(Object *sender, EventArgs *e)
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
	// Method:		ModifyTest
	// Description: Modifies a test
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************	
	void ScheduledTestPane::ModifyTest(Object *sender, EventArgs *e)
	{
		DisplayableDataNode *dataNode;

		if (this->listView->SelectedIndices->Count != 0)
		{
			int index = this->listView->SelectedIndices->Item[0];
			ListViewItem *lvItem = this->listView->Items->Item[index];
			String *nodeID = dynamic_cast <String*> (lvItem->Tag);
			dataNode = dynamic_cast<DisplayableDataNode*>(dataArray->GetDataNodeByID(nodeID));
			uiEventRegistry->OnStartModifyTestWizard(dataNode);
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
	void ScheduledTestPane::onKeyDown(Object* sender, KeyEventArgs *e)
	{
		if ((e->KeyData == Keys::Delete) || (e->KeyData == Keys::Back))
			DeleteTest(NULL, NULL);
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
	void ScheduledTestPane::onSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (this->listView->SelectedIndices->Count > 0)
		{
			ListViewItem * lvi = this->listView->SelectedItems->Item[0];
			ScheduledTestDataNode *node = dynamic_cast<ScheduledTestDataNode *>(this->dataArray->GetDataNodeByID (dynamic_cast <String *> (lvi->Tag)));
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
	void ScheduledTestPane::onCopyToClipboard(Object *sender, EventArgs *args)
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
	void ScheduledTestPane::onShowFieldChooser(Object *sender, EventArgs *args)
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
	void ScheduledTestPane::listView_ItemCheckEvent(Object *sender, ItemCheckEventArgs * e)
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

		ScheduledTestDataNode *schDataNode = dynamic_cast<ScheduledTestDataNode*> (dataNode);
		if (!schDataNode)
			return;

		schDataNode->IsTestOn = e->NewValue == CheckState::Checked;

		if (dataNode->ToggleDelegate)
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;

			dataNode->ToggleDelegate->DynamicInvoke(args);
		}
	}
}
