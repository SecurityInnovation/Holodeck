//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogPane.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkLogPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2003		 B. Shirey	 File created.
//*************************************************************************
#include <windows.h>
#include <vfw.h>
#include "LogPane.h"
#include "NetworkLogPane.h"
#include "FieldChooser.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkLogPane
	// Description: Constructor for the NetworkLogPane class
	//
	// Parameters:
	//	data - The NetworkLogPaneDataArray object that this pane is a view of
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPane::NetworkLogPane(NetworkLogPaneDataArray *data)
	{
		uiEventRegistry = UIEventRegistry::getInstance();
		dataEventRegistry = DataEventRegistry::getInstance();

		numVisibleEntries = 0;

		this->dataArray = data;

		InitializeUI();

		AutoNetworkLogFilterDelegate * filterDelegate = new AutoNetworkLogFilterDelegate (this, FilterLogItem);
		this->dataArray->FilterLogItem += filterDelegate;
		this->dataArray->LogEntrySetReceived += new LogEntrySetReceivedDelegate(this, onLogEntrySetReceived);

		ColumnClickEventArgs *args = new ColumnClickEventArgs(0);
		onColumnSort(this, args);
		this->dataArray->SortOrder = SortOrder::Ascending;
		
		statusTickHandler = new System::Threading::TimerCallback(this, onStatusUpdateTimerTick);
		statusUpdateTimer = new System::Threading::Timer(statusTickHandler, NULL, 500, 500);

		logListView->SmallImageList = ImageListManager::getInstance()->LogTreeViewImageList;
		
		LoadSettings(PaneName);

		if (this->ThreadID != 0)
		{
			ArrayList *cols = VisibleColumns;
			cols->Remove(logListView->ThreadIDHeader);
			VisibleColumns = cols;
		}

		numVisibleEntries = this->dataArray->TotalLogEntries;
	
		this->logListView->ItemCount = numVisibleEntries;

		allColumnWidths = new ArrayList();
		IEnumerator *enumerator = AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			header->ImageAlign = HorizontalAlignment::Right;
			allColumnWidths->Add(__box(header->Width));
		}

		if (VisibleColumns->Contains(logListView->TimeStampHeader))
		{
			logListView->TimeStampHeader->Image = ImageListManager::getInstance()->SortAscendingIcon;
		}
	}

	//*************************************************************************
	// Method:		~NetworkLogPane
	// Description: Destructor for the NetworkLogPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPane::~NetworkLogPane()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes all UI components for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::InitializeUI()
	{
		logListView = new NetworkLogListView();
		logListView->Visible = true;
		logListView->HideSelection = true;
		logListView->ColumnTracking = false;
		logListView->ColumnSortColor = logListView->BackColor;
		logListView->AllowColumnReorder = true;
		logListView->FullRowSelect = true;
		logListView->HeaderStyle = ColumnHeaderStyle::Clickable;
		logListView->Dock = DockStyle::Fill;
		logListView->RowTracking = false;
		logListView->Items->QueryListViewItem += new QueryContainerListViewItemDelegate(this, OnQueryContainerListViewItem);
		logListView->Items->QueryListViewItemIndex += new QueryContainerListViewItemIndexDelegate(this, OnQueryContainerListViewItemIndex);
		logListView->SetListViewItemFocused += new SetListViewItemFocusedDelegate(this, OnSetListViewItemFocused);
		logListView->SetListViewItemSelected += new SetListViewItemSelectedDelegate(this, OnSetListViewItemSelected);
		logListView->SelectedIndexChanged += new EventHandler(this, onSelectedIndexChanged);
		logListView->MouseUp += new MouseEventHandler(this, onListViewMouseUp);
		logListView->FilterItemColumns += new FilterItemColumnsDelegate(this, onFilterItemColumns);
		logListView->OnFilterUpdate += new LogFilterUpdateDelegate (this, onFilterUpdate);
		logListView->ColumnSortEventHandler += new ColumnClickEventHandler(this, onColumnSort);
		logListView->DoubleClick += new EventHandler(this, onDoubleClick);

		descriptionPanel = new StatusBarPanel();
		descriptionPanel->AutoSize = StatusBarPanelAutoSize::Spring;
		descriptionPanel->Text = String::Concat("Network Logs - Process ", dataArray->ProcessID.ToString());

		activeOrSavedPanel = new StatusBarPanel();
		activeOrSavedPanel->AutoSize = StatusBarPanelAutoSize::Contents;
		activeOrSavedPanel->Text = "Active";
		
		countPanel = new StatusBarPanel();
		countPanel->AutoSize = StatusBarPanelAutoSize::Contents;
		countPanel->Text = "Entries: 0000";

		visibleCountPanel = new StatusBarPanel();
		visibleCountPanel->AutoSize = StatusBarPanelAutoSize::Contents;
		visibleCountPanel->Text = "Visible: 0000";

		StatusBar *statusBar = new StatusBar();
		statusBar->Dock = DockStyle::Bottom;
		statusBar->ShowPanels = true;
		statusBar->SizingGrip = false;
		statusBar->Panels->Add(descriptionPanel);
		statusBar->Panels->Add(activeOrSavedPanel);
		statusBar->Panels->Add(countPanel);
		statusBar->Panels->Add(visibleCountPanel);

		toolTip = new ToolTip();
		toolTip->AutoPopDelay = 5000;
		toolTip->InitialDelay = 1000;
		toolTip->ReshowDelay = 500;
		toolTip->ShowAlways = true;
		toolTip->SetToolTip(logListView, "Once the application has network traffic select a packet from this pane,\nand view the packet details in the Network Message Details pane");

		this->Controls->Add(logListView);
		this->Controls->Add(statusBar);
	}

	//*************************************************************************
	// Method:		OnSetListViewItemSelected
	// Description: called when the listview's selection changes
	//
	// Parameters:
	//	index - the row number that changed
	//	selected - true if it is selected, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::OnSetListViewItemSelected(int index, bool selected)
	{
		dataArray->SetIsSelected(index, selected);
	}
	
	//*************************************************************************
	// Method:		OnSetListViewItemFocused
	// Description: called when the listview's focus changes
	//
	// Parameters:
	//	index - the row number that changed
	//	focused - true if it is focused, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::OnSetListViewItemFocused(int index, bool focused)
	{
		dataArray->SetIsFocused(index, focused);
	}

	//*************************************************************************
	// Method:		OnQueryContainerListViewItem
	// Description: called to display a particular item in the list view
	//
	// Parameters:
	//	index - the row number to get the item of
	//
	// Return Value: the list view item at the specified index
	//*************************************************************************
	ContainerListViewItem *NetworkLogPane::OnQueryContainerListViewItem(unsigned int index)
	{
		if ((dataArray->TotalLogEntries == 0) || (index >= (unsigned int)dataArray->TotalLogEntries))
			return NULL;

		NetworkLogItem *item = dataArray->GetLogItemAt(index);
		if (!item)
			return NULL;

		ContainerListViewItem *retVal = new ContainerListViewItem();
		retVal->Tag = __box(dataArray->GetFilePositionAt(index));
		retVal->Text = item->TimeStamp;
		retVal->Selected = dataArray->IsSelected(index);
		retVal->Focused = dataArray->IsFocused(index);
		retVal->SubItems->Add(item->ThreadID);
		if (item->IsIncomingMessage)
			retVal->SubItems->Add(INCOMING_MESSAGE_TEXT);
		else
			retVal->SubItems->Add(OUTGOING_MESSAGE_TEXT);
		retVal->SubItems->Add(item->ProtocolString);
		retVal->SubItems->Add(item->ActualDataLength.ToString());
		retVal->SubItems->Add(item->DestinationIPAddressString);
		retVal->SubItems->Add(item->DestinationPort.ToString());
		retVal->SubItems->Add(item->SourceIPAddressString);
		retVal->SubItems->Add(item->SourcePort.ToString());

		unsigned char *data = new unsigned char[item->ActualDataLength];
		if (data)
		{
			unsigned int sizeGot;
			item->GetData(data, sizeGot);

			int sizeToDisplay = min(8, sizeGot);
			String *asciiString = "";
			for (int i = 0; i < sizeToDisplay; i++)
			{
				unsigned char value = data[i];

				if ((value > 32) && (value < 127))
					asciiString = String::Concat(asciiString, new String((wchar_t)value, 1));
				else
					asciiString = String::Concat(asciiString, ".");
			}

			retVal->SubItems->Add(asciiString);
			delete[] data;
		}

		return retVal;
	}
	
	//*************************************************************************
	// Method:		OnQueryContainerListViewItemIndex
	// Description: called to get the index of a particular item in the list view
	//
	// Parameters:
	//	item - the item to get the row number of
	//
	// Return Value: the index of the specified item
	//*************************************************************************
	unsigned int NetworkLogPane::OnQueryContainerListViewItemIndex(ContainerListViewItem *item)
	{
		unsigned long filePos = *dynamic_cast<__box unsigned long *>(item->Tag);
		int index = dataArray->GetIndexOfFilePosition(filePos);
		return index;
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
	ArrayList *NetworkLogPane::get_VisibleColumns()
	{
		ArrayList *returnValue = new ArrayList();

		IEnumerator *enumerator = logListView->Columns->GetEnumerator();
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
	void NetworkLogPane::set_VisibleColumns(ArrayList *headers)
	{
		if (headers == NULL)
			return;

		// remove the columns that should not be here
		IEnumerator *enumerator = logListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (logListView->Columns->Contains(header))
			{
				if (!headers->Contains(header))
				{
					if (allColumnWidths)
						allColumnWidths->set_Item(AllColumns->IndexOf(header), __box(header->Width));

					// listview has the column, new columns array does not.  remove the column
					logListView->Columns->Remove(header);
				}
			}
		}

		// add the columns that should be here
		enumerator = logListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (!logListView->Columns->Contains(header))	
			{
				if (headers->Contains(header))
				{
					// listview does not have the column, but new columns array does.  add the column

					// try to put it back in the right place.  Note
					// reordering before the hide and re-show is ignored.

					// try to figure out where to insert the column
					int i = logListView->AllColumns->IndexOf(header);
					int insertLocation = i;
					int count = 0;

					ToggleColumnHeader *colHeader = NULL;
					IEnumerator *colEnumerator = logListView->AllColumns->GetEnumerator();
					while ((colEnumerator->MoveNext()) && (count <= i))
					{
						colHeader = dynamic_cast<ToggleColumnHeader *>(colEnumerator->Current);
						if (((colHeader == NULL) || (!logListView->Columns->Contains(colHeader))) && (count != i))
							insertLocation--;

						count++;
					}

					if (colHeader != NULL)
					{
						if (allColumnWidths)
							colHeader->Width = *dynamic_cast<__box int *>(allColumnWidths->get_Item(i));

						// insert the column header
						logListView->Columns->Insert(insertLocation, colHeader);
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		onSelectedIndexChanged
	// Description: called when a new log item is selected
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::onSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (this->logListView->SelectedIndices->Count > 0)
		{
			int id = *dynamic_cast<__box int *>(this->logListView->SelectedIndices->get_Item(0));
			NetworkLogPaneDataNode *node = dynamic_cast<NetworkLogPaneDataNode *>(this->dataArray->GetSortedDataNodeAt(id));
			if (!node)
				return;

			Object *itemObject[] = new Object *[1];
			itemObject[0] = node;
			this->uiEventRegistry->OnFocusChanged->DynamicInvoke(itemObject);
		}
	}

	//*************************************************************************
	// Method:		onStatusUpdateTimerTick
	// Description: called when the status timer is ready to update the status bar
	//
	// Parameters:
	//	state - a parameter to store extra state info, always NULL in this case
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::onStatusUpdateTimerTick(Object *state)
	{
		if (!dataArray)
		{
			visibleCountPanel->Text = "Visible: 0";
			countPanel->Text = "Entries: 0";
			activeOrSavedPanel->Text = "Terminated";
			return;
		}
	
		visibleCountPanel->Text = String::Concat("Visible: ", this->numVisibleEntries.ToString());
		countPanel->Text = String::Concat("Entries: ", dataArray->TotalLogEntries.ToString());
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
	void NetworkLogPane::onShowFieldChooser(Object *sender, EventArgs *args)
	{
		Cursor::Current = Cursors::WaitCursor;
		bool resume = false;

		HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataArray->ProcessID);
		if (holoScriptApp && !holoScriptApp->ApplicationPaused)
		{
			holoScriptApp->PauseApplication();
			resume = true;
		}

		FieldChooser *fieldChooser = new FieldChooser(logListView, logListView->AllColumns);

		int result = UIEventRegistry::getInstance()->OnShowDialog(fieldChooser);
		if (result == DialogResult::OK)
		{
			this->VisibleColumns = fieldChooser->SelectedColumns;
			SaveSettings(PaneName);
			logListView->Invalidate();
		}

		if (holoScriptApp && resume)
			holoScriptApp->ResumeApplication();

		Cursor::Current = Cursors::Default;
	}

	//*************************************************************************
	// Method:		onListViewMouseUp
	// Description: called when the a the mouse button is release
	//	over the list view
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::onListViewMouseUp(Object *sender, MouseEventArgs *args)
	{
		System::Windows::Forms::ContextMenu *contextMenu;
		
		if ((args->Button == MouseButtons::Right) && (!logListView->IsPointInColumnHeaders(System::Drawing::Point(args->X, args->Y))))
		{
			contextMenu = new System::Windows::Forms::ContextMenu();
			if ((logListView->SelectedIndices->Count > 0) && (*dynamic_cast<__box int *>(this->logListView->SelectedIndices->get_Item(0)) != -1))
			{
				MenuItem *showMenuItem = new MenuItem("Show Details", new EventHandler(this, onDoubleClick));
				showMenuItem->DefaultItem = true;
				contextMenu->MenuItems->Add(showMenuItem);
				contextMenu->MenuItems->Add(new MenuItem("-"));
				contextMenu->MenuItems->Add(new MenuItem("Copy", new EventHandler(this, onCopyToClipboard)));
			}
			contextMenu->MenuItems->Add(new MenuItem("Export Log to File...", new EventHandler(this, onExportToFile)));
			contextMenu->MenuItems->Add(new MenuItem("-"));
			contextMenu->MenuItems->Add(new MenuItem("Select Visible Columns...", new EventHandler(this, onShowFieldChooser)));
			
			Point point(args->X, args->Y);
			contextMenu->Show(this, point);
			contextMenu->MenuItems->Clear();
		}
	}

	//*************************************************************************
	// Method:		onFilterItemColumns
	// Description: called when the ui needs to know what columns to draw for a
	//	item
	//
	// Parameters:
	//	item - the node to column filter
	//
	// Return Value: None
	//*************************************************************************	
	void NetworkLogPane::onFilterItemColumns(ContainerListViewItem *item)
	{
		// remove columns we don't want to display
		int i = 0;
		bool hasText = false;
		ArrayList *itemsToRemove = new ArrayList();
		IEnumerator *enumerator = logListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (!header)
				continue;

			// different case for timestamp header
			if (header == logListView->TimeStampHeader)
			{
				if (logListView->Columns->Contains(logListView->TimeStampHeader))
					hasText = true;
			}
			else
			{
				if (logListView->Columns->Contains(header))
				{
					if (!hasText)
					{
						ContainerListViewSubItem *subItem = item->SubItems->get_Item(i);
						if (subItem)
						{
							item->Text = subItem->Text;
							itemsToRemove->Add(subItem);
							hasText = true;
						}
					}
				}
				else
				{
					ContainerListViewSubItem *subItem = item->SubItems->get_Item(i);
					if (subItem)
						itemsToRemove->Add(item->SubItems->get_Item(i));
				}

				i++;
			}
		}

		enumerator = itemsToRemove->GetEnumerator();
		while (enumerator->MoveNext())
		{
			item->SubItems->Remove(enumerator->Current);
		}
	}

	//*************************************************************************
	// Method:		FilterLogItem
	// Description: determines whether the given NetworkLogItem should be filtered out
	//
	// Parameters:
	//	item - the network log item which is to be checked for exclusion
	//
	// Return Value: (bool) true if item should be filtered, else false
	//*************************************************************************
	bool NetworkLogPane::FilterLogItem(NetworkLogItem *item)
	{
		// TIMESTAMP FILTER
		if ((logListView->timeStampFilterForm) && (logListView->timeStampFilterForm->FilterEnabled))
		{
			//Format is: "MM/dd/yyyy  hh:mm:ss:nnn"
			DateTime dtLogEntry = LogPane::ConvertTimeStampToDateTime(item->TimeStamp);

			if ((dtLogEntry >= LogPane::ConvertTimeStampToDateTime(logListView->timeStampFilterForm->RangeFrom)) &&
				(dtLogEntry <= LogPane::ConvertTimeStampToDateTime(logListView->timeStampFilterForm->RangeTo)))
			{
				if (!logListView->timeStampFilterForm->ShowRange)
					return true;
			}
			else if (logListView->timeStampFilterForm->ShowRange)
				return true;
		}

		// THREAD-ID FILTER
		if ((logListView->threadIDFilterForm) && (logListView->threadIDFilterForm->FilterEnabled))
		{
			if (!logListView->threadIDFilterForm->SelectedList->Contains (item->ThreadID))
				return true;
		}

		// DIRECTION FILTER
		if ((logListView->incomingFilterForm) && (logListView->incomingFilterForm->FilterEnabled))
		{
			if (item->IsIncomingMessage && !logListView->incomingFilterForm->SelectedList->Contains(new String(INCOMING_MESSAGE_TEXT)))
				return true;
			else if (!item->IsIncomingMessage && !logListView->incomingFilterForm->SelectedList->Contains(new String(OUTGOING_MESSAGE_TEXT)))
				return true;
		}

		// PROTOCOL FILTER
		if ((logListView->protocolFilterForm) && (logListView->protocolFilterForm->FilterEnabled))
		{
			if (!logListView->protocolFilterForm->SelectedList->Contains (item->ProtocolString))
				return true;
		}

		// MESSAGE LENGTH FILTER
		NumericRangeFilterForm *msgLengthForm = logListView->msgLengthFilterForm;
		if (msgLengthForm && msgLengthForm->FilterEnabled)
		{
			if (msgLengthForm->UseSingleValue)
			{
				unsigned int value = msgLengthForm->SingleValue;
				if (item->ActualDataLength != value)
				{
					if (msgLengthForm->IncludeValues)
						return true;
				}
				else if (!msgLengthForm->IncludeValues)
					return true;
			}
			else if (msgLengthForm->UseRange)
			{
				unsigned int lowValue = msgLengthForm->LowerRangeValue;
				unsigned int highValue = msgLengthForm->UpperRangeValue;
				if ((item->ActualDataLength > highValue) || (item->ActualDataLength < lowValue))
				{
					if (msgLengthForm->IncludeValues)
						return true;
				}
				else if (!msgLengthForm->IncludeValues)
					return true;
			}
			else if (msgLengthForm->UseLessThan)
			{
				unsigned int value = msgLengthForm->LessThanValue;
				if (item->ActualDataLength >= value)
				{
					if (msgLengthForm->IncludeValues)
						return true;
				}
				else if (!msgLengthForm->IncludeValues)
					return true;
			}
			else if (msgLengthForm->UseGreaterThan)
			{
				unsigned int value = msgLengthForm->GreaterThanValue;
				if (item->ActualDataLength <= value)
				{
					if (msgLengthForm->IncludeValues)
						return true;
				}
				else if (!msgLengthForm->IncludeValues)
					return true;
			}
		}

		// DESTINATION IP FILTER
		if ((logListView->destinationIPFilterForm) && (logListView->destinationIPFilterForm->FilterEnabled))
		{
			if (!logListView->destinationIPFilterForm->SelectedList->Contains (item->DestinationIPAddressString))
				return true;
		}

		// DESTINATION PORT FILTER
		NumericRangeFilterForm *destPortForm = logListView->destinationPortFilterForm;
		if (destPortForm && destPortForm->FilterEnabled)
		{
			if (destPortForm->UseSingleValue)
			{
				unsigned int value = destPortForm->SingleValue;
				if (item->DestinationPort != value)
				{
					if (destPortForm->IncludeValues)
						return true;
				}
				else if (!destPortForm->IncludeValues)
					return true;
			}
			else if (destPortForm->UseRange)
			{
				unsigned int lowValue = destPortForm->LowerRangeValue;
				unsigned int highValue = destPortForm->UpperRangeValue;
				if ((item->DestinationPort > highValue) || (item->DestinationPort < lowValue))
				{
					if (destPortForm->IncludeValues)
						return true;
				}
				else if (!destPortForm->IncludeValues)
					return true;
			}
			else if (destPortForm->UseLessThan)
			{
				unsigned int value = destPortForm->LessThanValue;
				if (item->DestinationPort >= value)
				{
					if (destPortForm->IncludeValues)
						return true;
				}
				else if (!destPortForm->IncludeValues)
					return true;
			}
			else if (destPortForm->UseGreaterThan)
			{
				unsigned int value = destPortForm->GreaterThanValue;
				if (item->DestinationPort <= value)
				{
					if (destPortForm->IncludeValues)
						return true;
				}
				else if (!destPortForm->IncludeValues)
					return true;
			}
		}

		// SOURCE IP FILTER
		if ((logListView->sourceIPFilterForm) && (logListView->sourceIPFilterForm->FilterEnabled))
		{
			if (!logListView->sourceIPFilterForm->SelectedList->Contains (item->SourceIPAddressString))
				return true;
		}

		// SOURCE PORT FILTER
		NumericRangeFilterForm *sourcePortForm = logListView->sourcePortFilterForm;
		if (sourcePortForm && sourcePortForm->FilterEnabled)
		{
			if (sourcePortForm->UseSingleValue)
			{
				unsigned int value = sourcePortForm->SingleValue;
				if (item->SourcePort != value)
				{
					if (sourcePortForm->IncludeValues)
						return true;
				}
				else if (!sourcePortForm->IncludeValues)
					return true;
			}
			else if (sourcePortForm->UseRange)
			{
				unsigned int lowValue = sourcePortForm->LowerRangeValue;
				unsigned int highValue = sourcePortForm->UpperRangeValue;
				if ((item->SourcePort > highValue) || (item->SourcePort < lowValue))
				{
					if (sourcePortForm->IncludeValues)
						return true;
				}
				else if (!sourcePortForm->IncludeValues)
					return true;
			}
			else if (sourcePortForm->UseLessThan)
			{
				unsigned int value = sourcePortForm->LessThanValue;
				if (item->SourcePort >= value)
				{
					if (sourcePortForm->IncludeValues)
						return true;
				}
				else if (!sourcePortForm->IncludeValues)
					return true;
			}
			else if (sourcePortForm->UseGreaterThan)
			{
				unsigned int value = sourcePortForm->GreaterThanValue;
				if (item->SourcePort <= value)
				{
					if (sourcePortForm->IncludeValues)
						return true;
				}
				else if (!sourcePortForm->IncludeValues)
					return true;
			}
		}

		return false;
	}

	//*************************************************************************
	// Method:		onFilterUpdate
	// Description: Event gets called whenever any custom filter is updated
	//				Is responsible for calling the SortItems method
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void NetworkLogPane::onFilterUpdate()
	{
		unsigned int filteredCount = dataArray->FilterCurrentItems();
		dataArray->SortItems();

		numVisibleEntries = dataArray->VisibleLogEntries;
		logListView->ItemCount = numVisibleEntries;

		logListView->Refresh();
	}

	//*************************************************************************
	// Method:		onLogEntrySetReceived
	// Description: called when the data array is done processing a log entry set
	//
	// Parameters:
	//	numEntriesInSet - the number of entries in this log entry set
	//
	// Return Value: None
	//*************************************************************************	
	void NetworkLogPane::onLogEntrySetReceived(unsigned int numEntriesInSet)
	{
		if (numEntriesInSet > 0)
		{
			numVisibleEntries = dataArray->VisibleLogEntries;
			this->logListView->ItemCount += numEntriesInSet;
			this->logListView->Invalidate();
		}
	}

	//*************************************************************************
	// Method:		onColumnSort
	// Description: called when the a column in the listview is clicked and "Sort" is selected
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::onColumnSort(Object *sender, ColumnClickEventArgs *args)
	{
		unsigned int dataArrayColumn = 0;
		ToggleColumnHeader *headerToSort = dynamic_cast<ToggleColumnHeader *>(this->VisibleColumns->get_Item(args->Column));
		if (!headerToSort)
			return;

		// make sure the sort icons are only in 1 column
		IEnumerator *allColsEnumerator = this->AllColumns->GetEnumerator();
		while (allColsEnumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(allColsEnumerator->Current);
			if (!header)
				continue;

			header->Image = NULL;
		}

		if (headerToSort == logListView->TimeStampHeader)  // timestamp
			dataArrayColumn = UIData::NetworkLogItemFields::TimeStampField;
		else if (headerToSort == logListView->ThreadIDHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::ThreadField;
		else if (headerToSort == logListView->IncomingHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::IncomingField;
		else if (headerToSort == logListView->ProtocolHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::ProtocolField;
		else if (headerToSort == logListView->MessageLengthHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::MessageLengthField;
		else if (headerToSort == logListView->DestinationIPHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::DestinationIPField;
		else if (headerToSort == logListView->DestinationPortHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::DestinationPortField;
		else if (headerToSort == logListView->SourceIPHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::SourceIPField;
		else if (headerToSort == logListView->SourcePortHeader)
			dataArrayColumn = UIData::NetworkLogItemFields::SourcePortField;

		if ((dataArray->SortColumn == dataArrayColumn) && (dataArray->SortOrder == SortOrder::Ascending))
		{
			headerToSort->Image = ImageListManager::getInstance()->SortDescendingIcon;
			dataArray->SortOrder = SortOrder::Descending;
		}
		else
		{
			dataArray->SortOrder = SortOrder::Ascending;
			dataArray->SortColumn = dataArrayColumn;
			headerToSort->Image = ImageListManager::getInstance()->SortAscendingIcon;
		}

		dataArray->SortItems();
		logListView->Invalidate();
	}

	//*************************************************************************
	// Method:		onCopyToClipboard
	// Description: Event handler for the Copy context menuitem
	//*************************************************************************	
	void NetworkLogPane::onCopyToClipboard(Object *sender, EventArgs *args)
	{
		if (uiEventRegistry->OnCopyToClipboard)
			uiEventRegistry->OnCopyToClipboard->Invoke();
	}

	//*************************************************************************
	// Method:		onExportToFile
	// Description: called when the user wants to export the log to a file
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPane::onExportToFile(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *logNode = new DisplayableDataNode("", UIData::NodeType::ActiveNetworkLogPane, this->ProcessID, this->ThreadID);

		if (dataEventRegistry->OnSaveNetworkLogPane)
			dataEventRegistry->OnSaveNetworkLogPane->Invoke(logNode);
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
	void NetworkLogPane::onDoubleClick(Object *sender, EventArgs *e)
	{
		if (logListView->SelectedIndices->Count != 0)
		{
			// Display packet details (selected item has already been loaded into the pane)
			if (uiEventRegistry->OnShowNetworkCorruptionDetailsPane)
				uiEventRegistry->OnShowNetworkCorruptionDetailsPane->Invoke();
		}
	}
}