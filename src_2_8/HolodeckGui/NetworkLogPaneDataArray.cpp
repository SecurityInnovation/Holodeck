//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogPaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkLogPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#include "NetworkLogPaneDataArray.h"
#include "ImageListManager.h"

namespace UIData
{
	//*************************************************************************
	// Method:		NetworkLogPaneDataArray
	// Description: Constructor for the NetworkLogPaneDataArray class
	//
	// Parameters:
	//	procID - The process id that this log represents
	//	tID - the thread id that this log represents
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPaneDataArray::NetworkLogPaneDataArray(unsigned int procID, unsigned int tID)
	{
		InternalConstructor(procID, tID, HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID), NULL);
	}

	//*************************************************************************
	// Method:		NetworkLogPaneDataArray
	// Description: Constructor for the NetworkLogPaneDataArray class
	//
	// Parameters:
	//	procID - The process id that this log represents
	//	tID - the thread id that this log represents
	//	hsApp - the holoScriptApplication instance to use for log generation
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPaneDataArray::NetworkLogPaneDataArray(unsigned int processID, unsigned int threadID, HoloScriptApplication *hsApp)
	{
		InternalConstructor(processID, threadID, hsApp, NULL);
	}

	//*************************************************************************
	// Method:		NetworkLogPaneDataArray
	// Description: Constructor for the NetworkLogPaneDataArray class
	//
	// Parameters:
	//	procID - The process id that this log represents
	//	tID - the thread id that this log represents
	//	logFilePath - the path of the saved log file
	//	hsApp - the holoScriptApplication instance to use for log generation
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPaneDataArray::NetworkLogPaneDataArray(unsigned int processID, unsigned int threadID, String *logFilePath, HoloScriptApplication *hsApp)
	{
		InternalConstructor(processID, threadID, hsApp, logFilePath);
	}

	//*************************************************************************
	// Method:		InternalConstructor
	// Description: Internal Constructor for the NetworkLogPaneDataArray class
	//
	// Parameters:
	//	procID - The process id that this log represents
	//	tID - the thread id that this log represents
	//	hsApp - the holoScriptApplication instance to use for log generation
	//	savedNetworkFileName - the name and path of the saved network logfile
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::InternalConstructor(unsigned int processID, unsigned int threadID, HoloScriptApplication *hsApp, String * savedNetworkFileName)
	{
		this->processID = processID;
		this->threadID = threadID;
		totalEntries = 0;
		otherEntries = 0;
		filteredOutList = new ArrayList();
		logIndexNodes = new ArrayList();

		if (savedNetworkFileName != NULL)
		{
			isSavedLog = true;
			this->savedLogFilePath = savedNetworkFileName;
		}
		else
		{
			isSavedLog = false;
			this->savedLogFilePath = S"";
		}
		
		logHandler = new HoloScript::LogsGeneratedDelegate(this, onLogsGenerated);
		this->holoScriptApp = hsApp;
		this->holoScriptApp->AddOnLogGeneratedHandler(logHandler);
	}

	//*************************************************************************
	// Method:		NetworkLogPaneDataArray
	// Description: Destructor for the NetworkLogPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogPaneDataArray::~NetworkLogPaneDataArray()
	{
		CleanUp();
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Cleans up all dataarray delegates
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void NetworkLogPaneDataArray::CleanUp()
	{
		holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		IsSelected
	// Description: returns whether the item at the specified position is selected
	//
	// Parameters:
	//	listViewIndex - the position to check for selection
	//
	// Return Value: true if selected, false otherwise
	//*************************************************************************
	bool NetworkLogPaneDataArray::IsSelected(int listViewIndex)
	{
		listMutex->WaitOne();
		NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(listViewIndex));
		listMutex->ReleaseMutex();
		if (!node)
			return false;

		return node->selected;
	}

	//*************************************************************************
	// Method:		SetIsSelected
	// Description: sets whether the item at the specified position is selected
	//
	// Parameters:
	//	listViewIndex - the position to set for selection
	//	selected - true if the row is selected, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::SetIsSelected(int listViewIndex, bool selected)
	{
		listMutex->WaitOne();
		NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(listViewIndex));
		listMutex->ReleaseMutex();
		if (!node)
			return;

		node->selected = selected;
	}
	
	//*************************************************************************
	// Method:		IsFocused
	// Description: returns whether the item at the specified position is focused
	//
	// Parameters:
	//	listViewIndex - the position to check for focus
	//
	// Return Value: true if selected, false otherwise
	//*************************************************************************
	bool NetworkLogPaneDataArray::IsFocused(int listViewIndex)
	{
		listMutex->WaitOne();
		NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(listViewIndex));
		listMutex->ReleaseMutex();
		if (!node)
			return false;

		return node->focused;
	}

	//*************************************************************************
	// Method:		SetIsFocused
	// Description: sets whether the item at the specified position is selected
	//
	// Parameters:
	//	listViewIndex - the position to set for selection
	//	focused - true if the row is focused, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::SetIsFocused(int listViewIndex, bool focused)
	{
		listMutex->WaitOne();
		NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(listViewIndex));
		listMutex->ReleaseMutex();
		if (!node)
			return;

		node->focused = focused;
	}

	//*************************************************************************
	// Method:		OnLogsGenerated
	// Description: Called when logs are generated for the process
	//
	// Parameters:
	//	numNewLogs - the number of new logs generated
	//	newLogItems - the new log item objects
	//	processID - the processID for the logs
	//	threadID - the threadID for the logs
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID,
		unsigned int threadID)
	{
		unsigned long originalTotalEntries = totalEntries;

		if (numNewLogs > 0)
		{
			listMutex->WaitOne();

			unsigned int numAdded = 0;
			unsigned long originalCurrentEntries = currentEntries;
			unsigned int filtered = 0;
			for (int count = 0; count < numNewLogs; count++)
			{
				// Get the log entry from Holoscript
				LogItem *item = newLogItems[count];
				if (!item)
					continue;

				// Check for a log events
				for (int j = 0; j < item->Events->Count; j++)
				{
					LogEvent* event = dynamic_cast<LogEvent*>(item->Events->Item[j]);
					if (!event)
						continue;

					// add the log file positions for entries with network logs
					if (event->Type == Holodeck::LogEventType::NetworkLogEvent)
					{
						NetworkLogItem *netItem = dynamic_cast<NetworkLogItem *>(item->NetworkLogItems->get_Item(0));

						NetworkLogIndexNode *node = new NetworkLogIndexNode();
						node->filePos = item->FilePosition;
						node->focused = false;
						node->selected = false;
						UpdateSortColumnCache(node, item, netItem);

						if ((FilterLogItem && (FilterLogItem->Invoke (netItem))) || 
							((this->ThreadID != 0) && (this->ThreadID != Convert::ToUInt32(netItem->ThreadID))))
						{
							// log entry is filtered out
							if ((this->threadID != 0) && (this->threadID != Convert::ToUInt32(netItem->ThreadID)))
								continue;

							filteredOutList->Add(node);
							currentEntries--;
							filtered++;
						}
						else
						{
							numAdded++;
							AddSortedNode(node);
						}
					}
				}
			}

			listMutex->ReleaseMutex();

			if (LogEntrySetReceived)
				LogEntrySetReceived->Invoke(numAdded);
		}
	}

	//*************************************************************************
	// Method:		FilterCurrentItems
	// Description: filters all the items in the list
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of items filtered out
	//*************************************************************************
	unsigned int NetworkLogPaneDataArray::FilterCurrentItems()
	{
		listMutex->WaitOne();

		// filter out the items needed
		ArrayList *removeList = new ArrayList();
		IEnumerator *enumerator = logIndexNodes->GetEnumerator();
		while (enumerator->MoveNext())
		{
			NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(enumerator->Current);
			// if the node isn't valid or it isn't a level 0 node, skip it
			if (!node)
				continue;

			// make sure it is a valid file position or we hang
			if (node->filePos == 0)
				continue;

			LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePos);
			if (!item || !item->NetworkLogItems || (item->NetworkLogItems->Count == 0))
				continue;

			NetworkLogItem *netItem = dynamic_cast<NetworkLogItem *>(item->NetworkLogItems->get_Item(0));
			if (!netItem)
				continue;

			if (FilterLogItem && (FilterLogItem->Invoke (netItem)))
				removeList->Add(node);

			delete item;
		}

		// add the filtered out items that need to be added again
		ArrayList *addList = new ArrayList();
		enumerator = filteredOutList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(enumerator->Current);
			// if the node isn't valid or it isn't a level 0 node, skip it
			if (!node)
				continue;

			LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePos);
			if (!item || !item->NetworkLogItems || (item->NetworkLogItems->Count == 0))
				continue;

			NetworkLogItem *netItem = dynamic_cast<NetworkLogItem *>(item->NetworkLogItems->get_Item(0));
			if (!netItem)
				continue;

			if (!FilterLogItem || (!FilterLogItem->Invoke(netItem)))
				addList->Add(node);

			delete item;
		}

		// actually remove the items
		unsigned int returnValue = 0;
		enumerator = removeList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(enumerator->Current);
			logIndexNodes->Remove(node);
			filteredOutList->Add(node);
			currentEntries--;
			returnValue++;
		}

		// actually add the items
		enumerator = addList->GetEnumerator();
		while (enumerator->MoveNext())
		{
			NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(enumerator->Current);
			logIndexNodes->Add(node);
			filteredOutList->Remove(node);
			currentEntries++;
			returnValue--;
		}

		listMutex->ReleaseMutex();

		return returnValue;
	}

	//*************************************************************************
	// Method:		SortItems
	// Description: Sorts all the items in the list
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::SortItems()
	{
		listMutex->WaitOne();
		cachedLogItemIndex = -1;
		logIndexNodes->Sort(this);
		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		GetLogItemAt
	// Description: Gets the nth visible log item from the indexer
	//
	// Parameters:
	//	listViewIndex - the index of the item to get
	//
	// Return Value: the NetworkLogItem object at the listViewIndex position, null if
	//	not found
	//*************************************************************************
	NetworkLogItem *NetworkLogPaneDataArray::GetLogItemAt(int listViewIndex)
	{
		listMutex->WaitOne();

		if (logIndexNodes->Count <= 0)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		if (listViewIndex >= logIndexNodes->Count)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(listViewIndex));
		if (!node)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePos);
		if (!item || (item->NetworkLogItems->Count <= 0))
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		NetworkLogItem *retVal = dynamic_cast<NetworkLogItem *>(item->NetworkLogItems->get_Item(0));
		listMutex->ReleaseMutex();
		return retVal;
	}

	//*************************************************************************
	// Method:		GetSortedDataNodeAt
	// Description: Gets the nth data node from the indexer
	//
	// Parameters:
	//	index - the index of the node to get
	//
	// Return Value: the data node object at the index position, null if
	//	not found
	//*************************************************************************
	NetworkLogPaneDataNode *NetworkLogPaneDataArray::GetSortedDataNodeAt(int index)
	{
		NetworkLogItem *item = GetLogItemAt(index);

		NetworkLogPaneDataNode *dataNode = new NetworkLogPaneDataNode(item);
		dataNode->Name = index.ToString();
		dataNode->processID = this->get_ProcessID();
		dataNode->threadID = this->threadID;

		return dataNode;
	}

	//*************************************************************************
	// Method:		GetFilePositionAt
	// Description: Gets the nth data node's position in the file
	//
	// Parameters:
	//	listViewIndex - the index of the node to get the position of
	//
	// Return Value: the file position of the data node object at the index 
	//	position, null if not found
	//*************************************************************************
	unsigned long NetworkLogPaneDataArray::GetFilePositionAt(int listViewIndex)
	{
		listMutex->WaitOne();
		NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(listViewIndex));
		listMutex->ReleaseMutex();
		return node->filePos;
	}

	//*************************************************************************
	// Method:		GetIndexOfFilePosition
	// Description: Gets index of the file position
	//
	// Parameters:
	//	filePos - the file position to get the index of
	//
	// Return Value: the index of the specified file position
	//*************************************************************************
	int NetworkLogPaneDataArray::GetIndexOfFilePosition(unsigned long filePos)
	{
		listMutex->WaitOne();

		IEnumerator *enumerator = logIndexNodes->GetEnumerator();
		int i = -1;
		while (enumerator->MoveNext())
		{
			i++;

			NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(enumerator->Current);
			if (!node)
				continue;

			if (node->filePos = filePos)
			{
				listMutex->ReleaseMutex();
				return i;
			}
		}
		listMutex->ReleaseMutex();
		return -1;
	}

	//*************************************************************************
	// Method:		Compare
	// Description: Compares two indexer node objects.  This method is used in
	//	the Sort method of the indexer.
	//
	// Parameters:
	//	obj1 - the first item to compare's file position
	//	obj2 - the second item to compare's file position
	//
	// Return Value: < 0 if the first is less than the second, > 0 if the second
	//	is greater than the first, 0 if they are equal
	//*************************************************************************
	int NetworkLogPaneDataArray::Compare(Object *obj1, Object *obj2)
	{
		NetworkLogIndexNode *node1 = dynamic_cast<NetworkLogIndexNode *>(obj1);
		NetworkLogIndexNode *node2 = dynamic_cast<NetworkLogIndexNode *>(obj2);

		if ((node1 == NULL) || (node2 == NULL))
			return 0;

		Int64 compareResult = 0;

		// Timestamp column has a special field in NetworkLogIndexNode, only use a string compare if it is a different column
		if (sortColumn != NetworkLogItemFields::TimeStampField)
		{
			String *string1 = node1->cachedSortColumnEntry;
			String *string2 = node2->cachedSortColumnEntry;

			// Ensure that neither string is NULL
			if (!string1)
				string1 = "";
			if (!string2)
				string2 = "";

			bool string1IsString = LogEntryIndexer::hasAlpha(string1);
			bool string2IsString = LogEntryIndexer::hasAlpha(string2);

			if (string1IsString && string2IsString)
			{
				// they are both strings - not numbers.  compare them
				compareResult = (sortOrder == SortOrder::Ascending) ? string1->CompareTo(string2) : string2->CompareTo(string1);
			}
			else if (string1IsString)
			{
				// the second string is a number, so it should be less
				compareResult = (sortOrder == SortOrder::Ascending) ? 1 : -1;
			}
			else if (string2IsString)
			{
				// the first is a number, so it should be less
				compareResult = (sortOrder == SortOrder::Ascending) ? -1 : 1;
			}
			else
			{
				// both are numbers
				Int64 num1 = Int64::Parse(string1);
				Int64 num2 = Int64::Parse(string2);

				if (sortOrder == SortOrder::Ascending)
				{
					if (num1 > num2)
						compareResult = 1;
					else if (num1 < num2)
						compareResult = -1;
					else
						compareResult = 0;
				}
				else
				{
					if (num1 > num2)
						compareResult = -1;
					else if (num1 < num2)
						compareResult = 1;
					else
						compareResult = 0;
				}
			}
		}

		if (compareResult == 0)
		{
			// Log entries are the same, now compare by timestamp to preserve the original order
			if (sortOrder == SortOrder::Ascending)
			{
				if (node1->timeStamp > node2->timeStamp)
					compareResult = 1;
				else if (node1->timeStamp < node2->timeStamp)
					compareResult = -1;
				else
					compareResult = 0;
			}
			else
			{
				if (node1->timeStamp > node2->timeStamp)
					compareResult = -1;
				else if (node1->timeStamp < node2->timeStamp)
					compareResult = 1;
				else
					compareResult = 0;
			}
		}
		if (compareResult < 0)
			return -1;
		if (compareResult > 0)
			return 1;
		return 0;
	}

	//*************************************************************************
	// Method:		set_SortColumn
	// Description: Changes the sort column
	//
	// Parameters:
	//	value - New sort column
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::set_SortColumn(unsigned int value)
	{
		listMutex->WaitOne();
		sortColumn = value;

		// Recompute the cached sort column for all indexer nodes
		for (int i=0; i<logIndexNodes->Count; i++)
		{
			NetworkLogIndexNode *node = dynamic_cast<NetworkLogIndexNode *>(logIndexNodes->get_Item(i));
			if (node->filePos == 0)
				continue;
			LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePos);
			if (!item || (item->NetworkLogItems->Count <= 0))
			{
				listMutex->ReleaseMutex();
				continue;
			}

			NetworkLogItem *netItem = dynamic_cast<NetworkLogItem *>(item->NetworkLogItems->get_Item(0));

			UpdateSortColumnCache(node, item, netItem);
		}

		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		UpdateSortColumnCache
	// Description: Updates the cachedSortColumnEntry field for a NetworkLogIndexNode
	//
	// Parameters:
	//	node - Node to update
	//	logItem - the log item associated with the node
	//  netLogItem - network log item associated with the node
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::UpdateSortColumnCache(NetworkLogIndexNode* node, LogItem *logItem, NetworkLogItem* netLogItem)
	{
		node->timeStamp = logItem->UInt64TimeStamp;
		switch (sortColumn)
		{
		case NetworkLogItemFields::TimeStampField:
			break;  // do nothing
		case NetworkLogItemFields::ThreadField:
			node->cachedSortColumnEntry = netLogItem->ThreadID;
			break;
		case NetworkLogItemFields::IncomingField:
			node->cachedSortColumnEntry = new String(netLogItem->IsIncomingMessage ? INCOMING_MESSAGE_TEXT : OUTGOING_MESSAGE_TEXT);
			break;
		case NetworkLogItemFields::ProtocolField:
			node->cachedSortColumnEntry = netLogItem->ProtocolString;
			break;
		case NetworkLogItemFields::MessageLengthField:
			node->cachedSortColumnEntry = netLogItem->ActualDataLength.ToString();
			break;
		case NetworkLogItemFields::OriginalMessageLengthField:
			node->cachedSortColumnEntry = netLogItem->ActualOriginalDataLength.ToString();
			break;
		case NetworkLogItemFields::DestinationIPField:
			node->cachedSortColumnEntry = netLogItem->DestinationIPAddressString;
			break;
		case NetworkLogItemFields::DestinationPortField:
			node->cachedSortColumnEntry = netLogItem->DestinationPort.ToString();
			break;
		case NetworkLogItemFields::SourceIPField:
			node->cachedSortColumnEntry = netLogItem->SourceIPAddressString;
			break;
		case NetworkLogItemFields::SourcePortField:
			node->cachedSortColumnEntry = netLogItem->SourcePort.ToString();
			break;
		}
	}

	//*************************************************************************
	// Method:		AddSortedNode
	// Description: Adds a node in the correct location
	//
	// Parameters:
	//	node - Node to insert
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::AddSortedNode(NetworkLogIndexNode *node)
	{
		int i = 0;
		if (logIndexNodes->Count > 0) // Only search if there are actually elements in the list
		{
			// Place the new log entry in the correct location to leave the list sorted.  Use a
			// binary search to find the location fast.
			int min, max;
			min = 0;
			max = logIndexNodes->Count - 1;
			for (i = (min + max) / 2; min != max; i = (min + max) / 2)
			{
				int compareResult = Compare(logIndexNodes->Item[i], node);

				if (compareResult > 0)
				{
					// Past where we want to be
					if (max == i)
						break; // Didn't update anything, we're about to loop indefinitely
					max = i;
					continue;
				}

				// Our spot is later on in the list
				if (min == i)
					break; // Didn't update anything, we're about to loop indefinitely
				min = i;
			}

			// i is now approximately where we need to be (it won't be exact as we aren't going to find our
			// log entry already in the list, our location depends on which side we came from).  Check the element
			// right before min just in case we got placed just after the ideal insertion location
			i = min - 1;
			if (i < 0)
				i = 0;
			for (; i<logIndexNodes->Count; i++)
			{
				int compareResult = Compare(logIndexNodes->Item[i], node);

				if (compareResult >= 0)
				{
					// We need to insert right here, break out now
					break;
				}
			}
		}

		logIndexNodes->Insert(i, node);
	}

	//*************************************************************************
	// Method:		ExportToFile
	// Description: Exports the log to the specified filename
	//
	// Parameters:
	//	exportFileName - The filename to export the log file to
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogPaneDataArray::ExportToFile(String * exportFileName)
	{
		StreamWriter *sw = NULL;

		try
		{
			sw = new StreamWriter(exportFileName, false);
			if (!sw)
				return;

			//write column headers
			sw->Write("TimeStamp");
			sw->Write(",");
			sw->Write("ThreadID");
			sw->Write(",");
			sw->Write("Incoming");
			sw->Write(",");
			sw->Write("Protocol");
			sw->Write(",");
			sw->Write("DestinationIP:Port");
			sw->Write(",");
			sw->Write("SourceIP:Port");
			sw->Write(",");
			sw->Write("Data");
			sw->Write("\r\n");

			for (int i = 0; i < logIndexNodes->Count; i++)
			{
				NetworkLogItem *item = this->GetLogItemAt(i);
				unsigned int currentDataSize = item->ActualDataLength;
				unsigned char *currentData = new unsigned char[currentDataSize];
				unsigned int sizeGot;
				item->GetData(currentData, sizeGot);
				String *data = new String((signed char*)currentData, 0, sizeGot);
				data = data->Replace("\n", " ");
				data = data->Replace("\r", " ");
				data = data->Replace("\t", "    ");
				data = data->Replace("\"", "&quot");
				
				if (!item) continue;

				sw->Write("\"");
				sw->Write(item->TimeStamp);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ThreadID);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->IsIncomingMessage? "Yes":"No");
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(item->ProtocolString);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(getFormattedAddressString(item->DestinationIPAddressString, item->DestinationPort.ToString()));
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(getFormattedAddressString(item->SourceIPAddressString, item->SourcePort.ToString()));
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(data);
				sw->Write("\"");
				sw->Write("\r\n");
			}
			sw->Close();
		}
		catch (Exception *)
		{
			if (sw)
				sw->Close();

			MessageBox::Show ("There was a problem exporting the network log to the specified location. Please verify that the specified location is not read-only and that you have adequate permissions to write to it.", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return;
		}
	}

	//*************************************************************************
	// Method:		DeleteSavedLog
	// Description: Deletes the saved log file
	//
	// Parameters:
	//	none
	//
	// Return Value: true if success, flase otherwise
	//*************************************************************************
	bool NetworkLogPaneDataArray::DeleteSavedLog()
	{
		return holoScriptApp->DeleteSavedLogFile(savedLogFilePath);
	}

	//*************************************************************************
	// Method:		getFormattedAddressString
	// Description: Adds the ip and port in one string
	//
	// Parameters:
	//	ip - the ip address
	//	port - the port
	//
	// Return Value: formatted address
	//*************************************************************************
	String * NetworkLogPaneDataArray::getFormattedAddressString(String *ip, String * port)
	{
		String * returnStr = String::Concat (ip->Trim(), S":", port->Trim());
		return returnStr;
	}
}