//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogPane.cpp
//
// DESCRIPTION: Contains implementation for the class LogPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogPane.h"
#include "FieldChooser.h"
#include "ImageListManager.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		LogPane
	// Description: Constructor for the LogPane class
	//
	// Parameters:
	//	data - The LogPaneDataArray object that this pane is a view of
	//
	// Return Value: None
	//*************************************************************************
	LogPane::LogPane(LogPaneDataArray *data)
	{
		uiEventRegistry = UIEventRegistry::getInstance();
		dataEventRegistry = DataEventRegistry::getInstance();

		numVisibleEntries = 0;

		this->dataArray = data;
		this->dataArray->LogPaneAttach();

		InitializeUI();

		FilterLogItemHandler = new AutoFilterDelegate (this, FilterLogItem);
		this->dataArray->FilterLogItem += FilterLogItemHandler;
		LogEntrySetReceivedHandler = new LogEntrySetReceivedDelegate(this, onLogEntrySetReceived);
		this->dataArray->LogEntrySetReceived += LogEntrySetReceivedHandler;
		
		statusTickHandler = new EventHandler(this, onStatusUpdateTimerTick);
		statusUpdateTimer = new Timer(new System::ComponentModel::Container());
		statusUpdateTimer->Enabled = true;
		statusUpdateTimer->Interval = 500;
		statusUpdateTimer->Tick += statusTickHandler;

		errorCodeDB = ErrorCodeDB::GetInstance();
		if (!errorCodeDB->IsLoaded)
			errorCodeDB->LoadDatabase(RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath());

		treeListView->SmallImageList = ImageListManager::getInstance()->LogTreeViewImageList;
		treeListView->ColumnSortEventHandler += new ColumnClickEventHandler(this, onColumnSort);
		treeListView->MouseUp += new MouseEventHandler(this, onListViewMouseUp);
		treeListView->OnFilterUpdate += new LogFilterUpdateDelegate (this, onFilterUpdate);
		treeListView->DoubleClick += new EventHandler(this, onListViewDoubleClick);
		treeListView->RootNode = dataArray->RootNode;
		treeListView->SelectedIndexChanged += new EventHandler(this, onSelectedIndexChanged);
		treeListView->FilterNodeColumns += new FilterNodeColumnsDelegate(this, onFilterNodeColumns);
		treeListView->QueryTreeListNode += new QueryTreeListNodeDelegate(this, onTreeListViewQueryItem);
		treeListView->QueryTreeListNodeAtRow += new QueryTreeListNodeAtRowDelegate(this, onTreeListViewQueryRow);
		treeListView->UnselectAllNodes += new UnselectAllNodesDelegate(this, onTreeListViewUnselectAll);
		
		LoadSettings(PaneName);

		if (this->ThreadID != 0)
		{
			ArrayList *cols = VisibleColumns;
			cols->Remove(treeListView->ThreadIDHeader);
			VisibleColumns = cols;
		}

		numVisibleEntries = this->dataArray->Count - 1;
		this->treeListView->ItemCount = numVisibleEntries;

		ProcessTerminatedHandler = new ProcessTerminatedDelegate(this, onProcessTerminated);
		uiEventRegistry->OnProcessTerminated += ProcessTerminatedHandler;
		ProcessPausedHandler = new ProcessPausedDelegate(this, onProcessPaused);
		uiEventRegistry->OnProcessPaused += ProcessPausedHandler;
		ProcessResumedHandler = new ProcessResumedDelegate(this, onProcessResumed);
		uiEventRegistry->OnProcessResumed += ProcessResumedHandler;
		SelectLogItemHandler = new SelectLogItemDelegate(this, onSelectLogItem);
		uiEventRegistry->OnSelectLogItem += SelectLogItemHandler;
		CloseProjectUIHandler = new CloseProjectUIDelegate(this, onCloseProjectUI);
		uiEventRegistry->OnCloseProjectUI += CloseProjectUIHandler;

		allColumnWidths = new ArrayList();
		IEnumerator *enumerator = treeListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			header->ImageAlign = HorizontalAlignment::Right;
			allColumnWidths->Add(__box(header->Width));
		}

		if (VisibleColumns->Contains(treeListView->TimeStampHeader))
		{
			treeListView->TimeStampHeader->Image = ImageListManager::getInstance()->SortAscendingIcon;
		}
	}

	//*************************************************************************
	// Method:		~LogPane
	// Description: Destructor for the LogPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogPane::~LogPane()
	{	
		CleanUp();
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Perform common cleanup tasks
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::CleanUp()
	{
		if (treeListView)
			treeListView->CleanUp();

		if (dataArray)
		{
			this->dataArray->FilterLogItem -= FilterLogItemHandler;
			this->dataArray->LogEntrySetReceived -= LogEntrySetReceivedHandler;
		}

		if (statusUpdateTimer && statusTickHandler)
			statusUpdateTimer->Tick -= statusTickHandler;

		uiEventRegistry->OnProcessTerminated -= ProcessTerminatedHandler;
		uiEventRegistry->OnProcessPaused -= ProcessPausedHandler;
		uiEventRegistry->OnProcessResumed -= ProcessResumedHandler;
		uiEventRegistry->OnSelectLogItem -= SelectLogItemHandler;
		uiEventRegistry->OnCloseProjectUI -= CloseProjectUIHandler;
	}

	//*************************************************************************
	// Method:		onCloseProjectUI
	// Description: Called when project is closing
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onCloseProjectUI()
	{
		CleanUp();
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
	void LogPane::InitializeUI()
	{
		treeListView = new LogTreeListView();
		treeListView->Visible = true;
		treeListView->HideSelection = true;
		treeListView->ShowLines = true;
		treeListView->ShowPlusMinus = true;
		treeListView->ColumnTracking = false;
		treeListView->ColumnSortColor = treeListView->BackColor;
		treeListView->ShowRootLines = false;
		treeListView->AllowColumnReorder = true;
		treeListView->FullRowSelect = true;
		treeListView->HeaderStyle = ColumnHeaderStyle::Clickable;
		treeListView->Dock = DockStyle::Fill;

		descriptionPanel = new StatusBarPanel();
		descriptionPanel->AutoSize = StatusBarPanelAutoSize::Spring;
		descriptionPanel->Text = String::Concat(dataArray->FullExecutableName, " - Process ", dataArray->ProcessID.ToString());

		activeOrSavedPanel = new StatusBarPanel();
		activeOrSavedPanel->AutoSize = StatusBarPanelAutoSize::Contents;

		if (dataArray->HoloScriptApplicationInstance->SavedLog)
			activeOrSavedPanel->Text =  "Saved";
		else if (dataArray->HoloScriptApplicationInstance->IsPaused)
			activeOrSavedPanel->Text =  "Paused";
		else
			activeOrSavedPanel->Text =  "Active";
		
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

		this->Controls->Add(treeListView);
		this->Controls->Add(statusBar);
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
	ArrayList *LogPane::get_VisibleColumns()
	{
		ArrayList *returnValue = new ArrayList();

		IEnumerator *enumerator = treeListView->Columns->GetEnumerator();
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
	void LogPane::set_VisibleColumns(ArrayList *headers)
	{
		if (headers == NULL)
			return;

		// remove the columns that should not be here
		IEnumerator *enumerator = treeListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (treeListView->Columns->Contains(header))
			{
				if (!headers->Contains(header))
				{
					if (allColumnWidths)
						allColumnWidths->set_Item(treeListView->AllColumns->IndexOf(header), __box(header->Width));

					// listview has the column, new columns array does not.  remove the column
					treeListView->Columns->Remove(header);
				}
			}
		}

		// add the columns that should be here
		enumerator = treeListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (header == NULL)
				continue;

			if (!treeListView->Columns->Contains(header))	
			{
				if (headers->Contains(header))
				{
					// listview does not have the column, but new columns array does.  add the column

					// try to put it back in the right place.  Note
					// reordering before the hide and re-show is ignored.

					// try to figure out where to insert the column
					int i = treeListView->AllColumns->IndexOf(header);
					int insertLocation = i;
					int count = 0;

					ToggleColumnHeader *colHeader = NULL;
					IEnumerator *colEnumerator = treeListView->AllColumns->GetEnumerator();
					while ((colEnumerator->MoveNext()) && (count <= i))
					{
						colHeader = dynamic_cast<ToggleColumnHeader *>(colEnumerator->Current);
						if (((colHeader == NULL) || (!treeListView->Columns->Contains(colHeader))) && (count != i))
							insertLocation--;

						count++;
					}

					if (colHeader != NULL)
					{
						if (allColumnWidths)
							colHeader->Width = *dynamic_cast<__box int *>(allColumnWidths->get_Item(i));

						// insert the column header
						treeListView->Columns->Insert(insertLocation, colHeader);
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
	void LogPane::onSelectedIndexChanged(Object *sender, EventArgs *args)
	{
		if (this->treeListView->SelectedIndices->Count > 0)
		{
			int id = *dynamic_cast<__box int *>(this->treeListView->SelectedIndices->get_Item(0));
			LogPaneDataNode *node = dynamic_cast<LogPaneDataNode *>(this->dataArray->GetSortedDataNodeAt(id));
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
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onStatusUpdateTimerTick(Object *sender, EventArgs *args)
	{
		if (!dataArray)
		{
			visibleCountPanel->Text = "Visible: 0";
			countPanel->Text = "Entries: 0";
			return;
		}
	
		unsigned long numVisible = this->numVisibleEntries;
		int totalEntries = dataArray->TotalLogEntries;

		if (numVisible > totalEntries)
			numVisible = totalEntries;

		visibleCountPanel->Text = String::Concat("Visible: ", numVisible.ToString());
		countPanel->Text = String::Concat("Entries: ", totalEntries.ToString());
	}

	//*************************************************************************
	// Method:		onProcessTerminated
	// Description: called when a process is terminated
	//
	// Parameters:
	//	processID - process that was terminated
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onProcessTerminated(unsigned int processID)
	{
		if (processID == this->ProcessID)
			activeOrSavedPanel->Text = "Terminated";
	}

	//*************************************************************************
	// Method:		onProcessPaused
	// Description: called when a process is paused
	//
	// Parameters:
	//	processID - process that was paused
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onProcessPaused(unsigned int processID)
	{
		if (processID == this->ProcessID)
			activeOrSavedPanel->Text = "Paused";
	}

	//*************************************************************************
	// Method:		onProcessResumed
	// Description: called when a process is resumed
	//
	// Parameters:
	//	processID - process that was resumed
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onProcessResumed(unsigned int processID)
	{
		if (processID == this->ProcessID)
			activeOrSavedPanel->Text = "Active";
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
	void LogPane::onColumnSort(Object *sender, ColumnClickEventArgs *args)
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

		if (headerToSort == treeListView->TimeStampHeader)  // timestamp
			dataArrayColumn = UIData::LogItemFields::TimeStampField;
		else if (headerToSort == treeListView->ThreadIDHeader)
			dataArrayColumn = UIData::LogItemFields::ThreadField;
		else if (headerToSort == treeListView->CategoryHeader)
			dataArrayColumn = UIData::LogItemFields::CategoryField;
		else if (headerToSort == treeListView->DllHeader)
			dataArrayColumn = UIData::LogItemFields::DllNameField;
		else if (headerToSort == treeListView->FunctionHeader)
			dataArrayColumn = UIData::LogItemFields::FunctionField;
		else if (headerToSort == treeListView->ReturnValueHeader)
			dataArrayColumn = UIData::LogItemFields::ReturnValueField;
		else if (headerToSort == treeListView->ErrorCodeHeader)
			dataArrayColumn = UIData::LogItemFields::ErrorCodeField;
		else if (headerToSort == treeListView->ExceptionHeader)
			dataArrayColumn = UIData::LogItemFields::ExceptionField;
		else
		{
			for (int i = 0; i < MAX_PARAMETER_COLUMNS; i++)
			{
				if (headerToSort == treeListView->GetParameterHeader(i))
					dataArrayColumn = UIData::LogItemFields::ParameterField + i;
			}
		}

		if ((dataArray->SortColumn == dataArrayColumn) && (dataArray->SortOrder == SortOrder::Ascending))
		{
			dataArray->SortOrder = SortOrder::Descending;
			headerToSort->Image = ImageListManager::getInstance()->SortDescendingIcon;
		}
		else
		{
			dataArray->SortOrder = SortOrder::Ascending;
			dataArray->SortColumn = dataArrayColumn;
			headerToSort->Image = ImageListManager::getInstance()->SortAscendingIcon;
		}

		dataArray->SortItems();
		treeListView->Invalidate();
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
	void LogPane::onListViewMouseUp(Object *sender, MouseEventArgs *args)
	{
		System::Windows::Forms::ContextMenu *contextMenu;
		
		if ((args->Button == MouseButtons::Right) && (!treeListView->IsPointInColumnHeaders(System::Drawing::Point(args->X, args->Y))))
		{
			contextMenu = new System::Windows::Forms::ContextMenu();
			if ((treeListView->SelectedIndices->Count > 0) && (*dynamic_cast<__box int *>(this->treeListView->SelectedIndices->get_Item(0)) != -1))
			{
				contextMenu->MenuItems->Add(new MenuItem("Copy", new EventHandler(this, onCopyToClipboard)));
			}
			contextMenu->MenuItems->Add(new MenuItem("Export Log to File...", new EventHandler(this, onExportToFile)));
			contextMenu->MenuItems->Add(new MenuItem("-"));

			if (numVisibleEntries > 0)
			{
				contextMenu->MenuItems->Add(new MenuItem("Expand All", new EventHandler(this, onExpandAll)));
				contextMenu->MenuItems->Add(new MenuItem("Collapse All", new EventHandler(this, onCollapseAll)));
				contextMenu->MenuItems->Add(new MenuItem("-"));
			}

			if ((treeListView->SelectedIndices->Count > 0) && (*dynamic_cast<__box int *>(this->treeListView->SelectedIndices->get_Item(0)) != -1))
			{
				MenuItem *createScheduledTestMenuItem = new MenuItem("Create Scheduled Test...", new EventHandler(this, onCreateScheduledTest));
				createScheduledTestMenuItem->DefaultItem = true;
				contextMenu->MenuItems->Add(createScheduledTestMenuItem);
			}

			contextMenu->MenuItems->Add(new MenuItem("Select Visible Columns...", new EventHandler(this, onShowFieldChooser)));
			
			Point point(args->X, args->Y);
			contextMenu->Show(this, point);
			contextMenu->MenuItems->Clear();
		}
	}

	//*************************************************************************
	// Method:		onListViewDoubleClick
	// Description: called when the a the mouse button is double-clicked
	//	over the list view
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onListViewDoubleClick(Object *sender, EventArgs *args)
	{
		onCreateScheduledTest(sender, args);
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
	void LogPane::onShowFieldChooser(Object *sender, EventArgs *args)
	{
		Cursor::Current = Cursors::WaitCursor;
		bool resume = false;

		HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataArray->ProcessID);
		if (holoScriptApp && !holoScriptApp->ApplicationPaused)
		{
			holoScriptApp->PauseApplication();
			resume = true;
		}

		FieldChooser *fieldChooser = new FieldChooser(treeListView, treeListView->AllColumns);

		int result = UIEventRegistry::getInstance()->OnShowDialog(fieldChooser);
		if (result == DialogResult::OK)
		{
			this->VisibleColumns = fieldChooser->SelectedColumns;
			SaveSettings(PaneName);
			treeListView->Invalidate();
		}

		if (holoScriptApp && resume)
			holoScriptApp->ResumeApplication();

		Cursor::Current = Cursors::Default;
	}
	//*************************************************************************
	// Method:		FilterLogItem
	// Description: determines whether the given LogItem should be filtered out
	//
	// Parameters:
	//	item -		the log item which is to be checked for exclusion
	//
	// Return Value: (bool) true if logItem should be filtered, else false
	//*************************************************************************
	bool LogPane::FilterLogItem(LogItem * item)
	{
		// TIMESTAMP FILTER
		if ((this->treeListView->timeStampFilterForm) && (this->treeListView->timeStampFilterForm->FilterEnabled))
		{
			//Format is: "MM/dd/yyyy  hh:mm:ss:nnn"
			DateTime dtLogEntry = ConvertTimeStampToDateTime(item->TimeStamp);

			if ((dtLogEntry >= ConvertTimeStampToDateTime(this->treeListView->timeStampFilterForm->RangeFrom)) &&
				(dtLogEntry <= ConvertTimeStampToDateTime(this->treeListView->timeStampFilterForm->RangeTo)))
			{
				if (!treeListView->timeStampFilterForm->ShowRange)
					return true;
			}
			else if (treeListView->timeStampFilterForm->ShowRange)
				return true;
		}

		// THREAD-ID FILTER
		if ((this->treeListView->threadIDFilterForm) && (this->treeListView->threadIDFilterForm->FilterEnabled))
		{
			if (! this->treeListView->threadIDFilterForm->SelectedList->Contains (item->ThreadID))
				return true;
		}

		// CATEGORY FILTER
		if ((this->treeListView->categoryFilterForm) && (this->treeListView->categoryFilterForm->FilterEnabled))
		{
			if (! this->treeListView->categoryFilterForm->SelectedList->Contains (item->Category))
				return true;
		}

		// DLL FILTER
		if ((this->treeListView->dllFilterForm) && (this->treeListView->dllFilterForm->FilterEnabled))
		{
			if (! this->treeListView->dllFilterForm->SelectedList->Contains (item->DllName))
				return true;
		}

		// FUNCTIONS FILTER
		if ((this->treeListView->functionsFilterForm) && (this->treeListView->functionsFilterForm->FilterEnabled))
		{
			System::Collections::Hashtable * disabledList = this->treeListView->functionsFilterForm->FunctionsTable;
			if (disabledList->ContainsKey (item->Category))
			{
				ArrayList * funcList = static_cast <ArrayList *> (disabledList->Item[item->Category]);
				if ((funcList) && (funcList->Contains (item->Function)))
					return true;
			}
		}

		// RETURN VALUE FILTER
		if ((this->treeListView->returnValueFilterForm) && (this->treeListView->returnValueFilterForm->FilterEnabled))
		{
			if (! this->treeListView->returnValueFilterForm->SelectedList->Contains (item->ReturnValue))
				return true;
		}

		// ERROR CODE FILTER
		if ((this->treeListView->errorCodeFilterForm) && (this->treeListView->errorCodeFilterForm->FilterEnabled))
		{
			if (! this->treeListView->errorCodeFilterForm->SelectedList->Contains (errorCodeDB->GetStringFromCode(item->ErrorCode)))
				return true;
		}

		// EXCEPTION FILTER
		if ((this->treeListView->exceptionFilterForm) && (this->treeListView->exceptionFilterForm->FilterEnabled))
		{
			if (! this->treeListView->exceptionFilterForm->SelectedList->Contains (item->Exception))
				return true;
		}

		// PARAMETER FILTER
		if ((this->treeListView->paramFilterForm) && (this->treeListView->paramFilterForm->FilterEnabled))
		{
			ListView::ListViewItemCollection * filterList = this->treeListView->paramFilterForm->Filters;
			// cycle through all the filters
			
			for (int i= filterList->Count - 1; i >= 0; i--)
			{
				ListViewItem * filter = filterList->Item[i];
				ArrayList * params = item->Parameters;
				
				// if match is specified for a param# then check there
				if (String::Compare (filter->SubItems->Item[2]->Text, "{Any}", true != 0))
				{
					// parse the target param., subtract 1 since UI is 1-index based, whereas array is 0-indexed
					int targetParam = Int32::Parse (filter->SubItems->Item[2]->Text) - 1;
					// if the specified param# does not exist for this call then do not include in log
					if (targetParam >= item->Parameters->Count)
						return true;
					else
					{
						//if match is found, then do not filter this log entry
						if (matchString (item->Parameters->Item[targetParam]->ToString()->Trim(), filter->SubItems->Item[0]->Text, filter->SubItems->Item[1]->Text->Trim()))
							return false;
					}
				}
				// otherwise check in all parameters
				else
				{
					for (int j=0; j< params->Count; j++)
					{
						//if match is found, then do not filter this log entry
						if (matchString (item->Parameters->Item[j]->ToString()->Trim(), filter->SubItems->Item[0]->Text, filter->SubItems->Item[1]->Text->Trim()))
							return false;
					}
				}
			}
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		ConvertTimeStampToDateTime
	// Description: Converts a TimeStamp to a DateTime type
	//
	// Parameters:
	//	timeStamp - the string to convert
	//
	// Return Value: (DateTime) The DateTime type equivalent of the time string
	//*************************************************************************	
	DateTime LogPane::ConvertTimeStampToDateTime(String * timeStamp)
	{
		int lastColon = 0;
		for (int i=0; i<3; i++)
			lastColon = timeStamp->IndexOf (":", lastColon + 1);
		
		//store the sub-second value in a string
		String * milliSecond = timeStamp->Substring (lastColon + 1);
		//separate the sub-second part from the string, to make it DateTime compatible
		String * dtString = timeStamp->Substring (0, lastColon);
		//parse rest of string into DateTime type
		DateTime dt = DateTime::Parse (dtString, NULL, Globalization::DateTimeStyles::AllowWhiteSpaces);
		//create a DateTime type with date time information including sub-second info.
		DateTime dtLogEntry = DateTime (dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second, Int32::Parse (milliSecond));
		return dtLogEntry;
	}

	//*************************************************************************
	// Method:		matchString
	// Description: Compares two strings based on the matchType
	//
	// Parameters:
	//	baseString		- the string against which the comparison is being made
	//	compareString	- the string for which the comparison is being made
	//	matchType		- valid values  {"SubString", "Starts With", "Ends With", "Exact"}	
	//
	// Return Value: (bool) returns the result of the comparison, if matchType is invalid then returns false
	//*************************************************************************	
	bool LogPane::matchString(String * baseString, String * compareString, String * matchType)
	{
		//make match case insensitive
		baseString = baseString->ToLower();
		compareString = compareString->ToLower();
		
		if (String::Compare (matchType, "contains", true) == 0)
		{
			if (baseString->IndexOf (compareString) > 0)
				return true;
			else
				return baseString->Equals (compareString);
		}

		if (String::Compare (matchType, "does not contain", true) == 0)
		{
			return (baseString->IndexOf(compareString) == -1);
		}

		if (String::Compare (matchType, "starts with", true) == 0)
		{
			return baseString->StartsWith (compareString);
		}

		if (String::Compare (matchType, "ends with", true) == 0)
		{
			return baseString->EndsWith (compareString);
		}

		if (String::Compare (matchType, "equals", true) == 0)
		{
			return baseString->Equals (compareString);
		}

		if (String::Compare (matchType, "does not equal", true) == 0)
		{
			return !baseString->Equals (compareString);
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
	void LogPane::onFilterUpdate()
	{
		unsigned int filteredCount = dataArray->FilterCurrentItems();
		dataArray->SortItems();

		numVisibleEntries = dataArray->VisibleNodeCount;
		treeListView->ItemCount = numVisibleEntries;

		treeListView->Refresh();
	}

	//*************************************************************************
	// Method:		onTreeListViewQueryItem
	// Description: called when the ui needs to get the tree list node at a 
	//	particular index
	//
	// Parameters:
	//	index - the index to get the node for
	//
	// Return Value: the tree list node at the specified index
	//*************************************************************************
	TreeListNode *LogPane::onTreeListViewQueryItem(unsigned int index)
	{
		return dataArray->GetTreeListNodeAtIndex(index);
	}

	//*************************************************************************
	// Method:		onTreeListViewQueryRow
	// Description: called when the ui needs to get the tree list node at a 
	//	particular row
	//
	// Parameters:
	//	row - the row index to get the node for
	//
	// Return Value: the tree list node at the specified row
	//*************************************************************************
	TreeListNode *LogPane::onTreeListViewQueryRow(unsigned int row)
	{
		return dataArray->GetTreeListNodeAtRow(row);
	}

	//*************************************************************************
	// Method:		onTreeListViewUnselectAll
	// Description: called when the ui needs to unselect all tree nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onTreeListViewUnselectAll()
	{
		dataArray->UnselectAllNodes();
	}

	//*************************************************************************
	// Method:		onFilterNodeColumns
	// Description: called when the ui needs to know what columns to draw for a
	//	node
	//
	// Parameters:
	//	node - the node to column filter
	//
	// Return Value: None
	//*************************************************************************	
	void LogPane::onFilterNodeColumns(TreeListNode *node)
	{
		// remove columns we don't want to display
		int i = 0;
		bool hasText = false;
		ArrayList *itemsToRemove = new ArrayList();
		IEnumerator *enumerator = treeListView->AllColumns->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ToggleColumnHeader *header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
			if (!header)
				continue;

			// different case for timestamp header
			if (header == treeListView->TimeStampHeader)
			{
				if (treeListView->Columns->Contains(treeListView->TimeStampHeader))
					hasText = true;
			}
			else
			{
				if (treeListView->Columns->Contains(header))
				{
					if (!hasText)
					{
						ContainerListViewSubItem *subItem = node->SubItems->get_Item(i);
						if (subItem)
						{
							node->Text = subItem->Text;
							itemsToRemove->Add(subItem);
						}
						else
							node->Text = NULL;

						hasText = true;
					}
				}
				else
				{
					ContainerListViewSubItem *subItem = node->SubItems->get_Item(i);
					if (subItem)
						itemsToRemove->Add(node->SubItems->get_Item(i));
				}

				i++;
			}
		}

		enumerator = itemsToRemove->GetEnumerator();
		while (enumerator->MoveNext())
		{
			node->SubItems->Remove(enumerator->Current);
		}
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
	void LogPane::onLogEntrySetReceived(unsigned int numEntriesInSet)
	{
		numVisibleEntries = dataArray->VisibleNodeCount;
		this->treeListView->ItemCount += numEntriesInSet;
		this->treeListView->Invalidate();
	}

	//*************************************************************************
	// Method:		get_PaneLogListView
	// Description: make LogListView accessible to enable storing filters
	//
	// Parameters:
	//	None
	//
	// Return Value: LogListView for the LogPane
	//*************************************************************************	
	LogTreeListView* LogPane::get_PaneLogListView()
	{
		return treeListView;
	}

	//*************************************************************************
	// Method:		ConvertLogItemToListViewItem
	// Description: Converts a LogItem to a ListViewItem
	//
	// Parameters:
	//	item - the LogItem to convert
	//
	// Return Value: the converted ListViewItem
	//*************************************************************************
	ListViewItem * LogPane::ConvertLogItemToListViewItem(LogItem *item)
	{
		ListViewItem * lvi;
		int paramCount = 0;

		for (int i=0; i < this->treeListView->Columns->Count; i++)
		{
			String * columnValue = "";
			if (this->treeListView->Columns->Item[i] == this->treeListView->TimeStampHeader)
				columnValue = item->TimeStamp ? item->TimeStamp : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->ThreadIDHeader)
				columnValue = item->ThreadID ? item->ThreadID : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->CategoryHeader)
				columnValue = item->Category ? item->Category : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->DllHeader)
				columnValue = item->DllName ? item->DllName : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->FunctionHeader)
				columnValue = item->Function ? item->Function : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->ErrorCodeHeader)
				columnValue = item->ErrorCode ? item->ErrorCode : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->ExceptionHeader)
				columnValue = item->Exception ? item->Exception : "";
			else if (this->treeListView->Columns->Item[i] == this->treeListView->ReturnValueHeader)
				columnValue = item->ReturnValue ? item->ReturnValue : "";
			else	// if none of the above headers, then it's a parameter header
			{
				columnValue = item->Parameters->Count > paramCount ? item->Parameters->Item[paramCount]->ToString() : NULL;
				paramCount++;
			}
				
			if (i == 0)
				lvi = new ListViewItem (columnValue);
			else
			{
				if (columnValue)
					lvi->SubItems->Add (columnValue);
			}
		}
		return lvi;
	}

	//*************************************************************************
	// Method:		onCopyToClipboard
	// Description: Event handler for the Copy context menuitem
	//*************************************************************************	
	void LogPane::onCopyToClipboard(Object *sender, EventArgs *args)
	{
		if (uiEventRegistry->OnCopyToClipboard)
			uiEventRegistry->OnCopyToClipboard->Invoke();
	}

	//*************************************************************************
	// Method:		onExpandAll
	// Description: called when the user wants to expand all log entries
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onExpandAll(Object *sender, EventArgs *args)
	{
		treeListView->ExpandAll();
	}
	
	//*************************************************************************
	// Method:		onCollapseAll
	// Description: called when the user wants to collapse all log entries
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onCollapseAll(Object *sender, EventArgs *args)
	{
		treeListView->CollapseAll();
	}

	//*************************************************************************
	// Method:		onCreateScheduledTest
	// Description: called when the user wants to create a scheduled test
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onCreateScheduledTest(Object *sender, EventArgs *args)
	{
		if (numVisibleEntries == 0)
			return;

		DisplayableDataNode *logNode = new DisplayableDataNode("", UIData::NodeType::ActiveLogPane, this->ProcessID, this->ThreadID);

		if (this->treeListView->SelectedIndices->Count == 0)
			return;

		unsigned int id = *dynamic_cast<__box int *>(this->treeListView->SelectedIndices->get_Item(0));
		LogPaneDataNode *node = dynamic_cast<LogPaneDataNode *>(this->dataArray->GetSortedDataNodeAt(id));
		if (!node)
			return;

		if ((id < 0) || ((int)id > dataArray->TotalLogEntries))
			return;

		logNode->Element = node->LogEntry;
		uiEventRegistry->OnStartCreateTestWizard(logNode);
	}

	//*************************************************************************
	// Method:		onSelectLogItem
	// Description: Selects a log item given a file position
	//
	// Parameters:
	//	filePosition - file position of the node to select
	//
	// Return Value: None
	//*************************************************************************
	void LogPane::onSelectLogItem(int pid, unsigned long filePosition)
	{
		if (this->ProcessID != pid)
			return;

		int index = dataArray->GetIndexFromFilePosition(filePosition);
		if (index != -1)
			this->SelectedTreeListViewIndex = index;
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
	void LogPane::onExportToFile(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *logNode = new DisplayableDataNode("", UIData::NodeType::ActiveLogPane, this->ProcessID, this->ThreadID);

		if (dataEventRegistry->OnSaveLogPane)
			dataEventRegistry->OnSaveLogPane->Invoke(logNode);
	}

}