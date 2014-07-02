//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEntryIndexer.cpp
//
// DESCRIPTION: Contains implementation for the class LogEntryIndexer
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include <windows.h>

#include "SiArray.h"
#include "LogEntryIndexer.h"

namespace UIData
{
	static CRITICAL_SECTION rowCacheCritSec;
	static bool rowCacheCritSecInitialized = false;

	//*************************************************************************
	// Method:		IndexerNode
	// Description: Constructor for the IndexerNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	IndexerNode::IndexerNode()
	{
		parent = NULL;
		children = NULL;
		filePosition = 0;
		nodeInfo.rowIndex = INVALID_ROW_INDEX;
		nodeInfo.level = 0;
		nodeInfo.expanded = false;
		nodeInfo.selected = false;
		nodeInfo.focused = false;
		nodeInfo.filtered = false;
		nodeInfo.valid = false;
	}

	//*************************************************************************
	// Method:		~IndexerNode
	// Description: Destructor for the IndexerNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	IndexerNode::~IndexerNode()
	{
		parent = NULL;

		if (children)
			delete children;
		children = NULL;
	}

	//*************************************************************************
	// Method:		IndexerNodeList
	// Description: Constructor for the IndexerNodeList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	IndexerNodeList::IndexerNodeList()
	{
		count = 0;
		sections = new SiUtils::SiArray<IndexerNode*>();
	}

	//*************************************************************************
	// Method:		~IndexerNodeList
	// Description: Destructor for the IndexerNodeList class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	IndexerNodeList::~IndexerNodeList()
	{
		if (sections)
		{
			for (unsigned int i=0; i<sections->GetLength(); i++)
				delete[] (*sections)[i];
			delete sections;
			sections = NULL;
		}
	}

	//*************************************************************************
	// Method:		Add
	// Description: Adds a new node to the end of the list
	//
	// Parameters:
	//	node - Node to add
	//
	// Return Value: Index of added node
	//*************************************************************************
	int IndexerNodeList::Add(IndexerNode& node)
	{
		if ((count >> INDEXER_SECTION_BITS) >= (int)sections->GetLength())
			sections->Add(__nogc new IndexerNode[INDEXER_SECTION_SIZE]);

		int curIndex = count++;
		*GetNodeAt(curIndex) = node;
		return curIndex;
	}

	//*************************************************************************
	// Method:		GetCount
	// Description: Gets the number of nodes in the list
	//
	// Parameters:
	//	None
	//
	// Return Value: Number of nodes in the list
	//*************************************************************************
	int IndexerNodeList::GetCount()
	{
		return count;
	}

	//*************************************************************************
	// Method:		GetNodeAt
	// Description: Gets a pointer to a node by index
	//
	// Parameters:
	//	index - Index of node to get
	//
	// Return Value: Pointer to node.  This pointer is safe for caching.  As
	//	long as the IndexerNodeList is alive, the pointer will be valid.
	//*************************************************************************
	IndexerNode* IndexerNodeList::GetNodeAt(int index)
	{
		if (index >= count)
			return NULL;
		return &((*sections)[index >> INDEXER_SECTION_BITS])[index & INDEXER_SECTION_MASK];
	}

	//*************************************************************************
	// Method:		IndexOf
	// Description: Gets the index of a node
	//
	// Parameters:
	//	node - Pointer to node (returned by GetNodeAt) to get the index of
	//
	// Return Value: Index of node, or -1 if not found
	//*************************************************************************
	int IndexerNodeList::IndexOf(IndexerNode* node)
	{
		for (unsigned int i=0; i<sections->GetLength(); i++)
		{
			IndexerNode* begin = &(*sections)[i][0];
			IndexerNode* end = &(*sections)[i][INDEXER_SECTION_SIZE];

			// Is node within the range of memory allocated for this section?
			if ((node >= begin) && (node < end))
				return (i * INDEXER_SECTION_SIZE) + (node - begin);
		}
		return -1;
	}

	//*************************************************************************
	// Method:		LogEntryIndexer
	// Description: Constructor for the LogEntryIndexer class
	//
	// Parameters:
	//	holoScriptApplication - the HoloScriptApplication object to use
	//
	// Return Value: None
	//*************************************************************************
	LogEntryIndexer::LogEntryIndexer(HoloScriptApplication *holoScriptApplication) : 
		holoScriptApp(holoScriptApplication)
	{
		sortedList = new IndexerNodeList();
		sortOrder = SortOrder::Ascending;
		sortColumn = LogItemFields::TimeStampField;
		rowCache = new SiUtils::SiArray<IndexerNode*>();
		cachedSortColumn = new ArrayList();

		if (!rowCacheCritSecInitialized)
		{
			InitializeCriticalSection(&rowCacheCritSec);
			rowCacheCritSecInitialized = true;
		}
	}

	//*************************************************************************
	// Method:		~LogEntryIndexer
	// Description: Destructor for the LogEntryIndexer class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogEntryIndexer::~LogEntryIndexer()
	{
		if (rowCache)
		{
			delete rowCache;
			rowCache = NULL;
		}
		if (sortedList)
		{
			delete sortedList;
			sortedList = NULL;
		}
	}

	//*************************************************************************
	// Method:		hasAlpha
	// Description: determines if a string contains any alpha characters
	//
	// Parameters:
	//	value - string to examine
	//
	// Return Value: true if there is any alpha, false if there are digits.
	//*************************************************************************
	bool LogEntryIndexer::hasAlpha(String* value)
	{
		if (value == "")
		{
			// Trying to parse an empty string as a number will cause an exception.
			// Treat an empty string as having alpha.
			return true;
		}

		IEnumerator *enumerator = value->GetEnumerator();
		wchar_t character;

		while (enumerator->MoveNext())
		{
			character = Convert::ToChar(enumerator->Current);
			if (!iswdigit(character))
				return true;
		}

		return false;
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
	int LogEntryIndexer::Compare(Object *obj1, Object *obj2)
	{
		IndexerNode *node1 = (IndexerNode*)(dynamic_cast<UIntPtr*>(obj1)->ToPointer());
		IndexerNode *node2 = (IndexerNode*)(dynamic_cast<UIntPtr*>(obj2)->ToPointer());

		return Compare(node1, node2);
	}

	//*************************************************************************
	// Method:		Compare
	// Description: Compares two indexer node objects.  This method is used in
	//	the AddSortedChildNode method of the indexer.
	//
	// Parameters:
	//	node1 - the first item to compare's file position
	//	node2 - the second item to compare's file position
	//
	// Return Value: < 0 if the first is less than the second, > 0 if the second
	//	is greater than the first, 0 if they are equal
	//*************************************************************************
	int LogEntryIndexer::Compare(IndexerNode *node1, IndexerNode *node2)
	{
		if ((node1 == NULL) || (node2 == NULL))
			return 0;

		Int64 compareResult = 0;

		// Timestamp column has a special field in IndexerNode, only use a string compare if it is a different column
		if (sortColumn != LogItemFields::TimeStampField)
		{
			int node1Index = sortedList->IndexOf(node1);
			int node2Index = sortedList->IndexOf(node2);

			String *string1 = NULL;
			String *string2 = NULL;

			if (node1Index < cachedSortColumn->Count)
				string1 = dynamic_cast<String*>(cachedSortColumn->Item[node1Index]);
			if (node2Index < cachedSortColumn->Count)
				string2 = dynamic_cast<String*>(cachedSortColumn->Item[node2Index]);

			// Ensure that neither string is NULL
			if (!string1)
				string1 = "";
			if (!string2)
				string2 = "";

			bool string1IsString = hasAlpha(string1);
			bool string2IsString = hasAlpha(string2);

			if (string1IsString && string2IsString)
			{
				// they are both strings - not numbers.  compare them
				compareResult = (sortOrder == SortOrder::Ascending) ? string1->CompareTo(string2) : string2->CompareTo(string1);
			}
			else if (string1IsString)
			{
				// the second string is a number, so it should be less, unless string is empty
				if (string1->Length == 0)
					compareResult = (sortOrder == SortOrder::Ascending) ? -1 : 1;
				else
					compareResult = (sortOrder == SortOrder::Ascending) ? 1 : -1;
			}
			else if (string2IsString)
			{
				// the first is a number, so it should be less unless string is empty
				if (string2->Length == 0)
					compareResult = (sortOrder == SortOrder::Ascending) ? 1 : -1;
				else
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
			// Sorting by timestamp or Log entries are the same, now compare by file position since it is the same ordering (and
			// more precise) than the timestamp itself
			if (sortOrder == SortOrder::Ascending)
			{
				if (node1->filePosition > node2->filePosition)
					compareResult = 1;
				else if (node1->filePosition < node2->filePosition)
					compareResult = -1;
				else
					compareResult = 0;
			}
			else
			{
				if (node1->filePosition > node2->filePosition)
					compareResult = -1;
				else if (node1->filePosition < node2->filePosition)
					compareResult = 1;
				else
					compareResult = 0;
			}
		}

		return (int)compareResult;
	}

	//*************************************************************************
	// Method:		ResetRowIndex
	// Description: Sets the row index for a node to mark it as invisible,
	//				including child nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::ResetRowIndex(IndexerNode* node)
	{
		int rowCacheIndex = rowCache->IndexOf(node);
		if (rowCacheIndex != -1)
		{
			rowCache->Remove(rowCacheIndex);
			for (unsigned int i=0; i<rowCache->GetLength(); i++)
			{
				IndexerNode *indexerNode = (*rowCache)[i];
				indexerNode->nodeInfo.rowIndex -= 1;
			}
		}

		node->nodeInfo.rowIndex = INVALID_ROW_INDEX;

		if (!node->children)
			return;

		for (unsigned int i=0; i<node->children->GetLength(); i++)
			ResetRowIndex((*node->children)[i]);
	}

	//*************************************************************************
	// Method:		AddNodeToRowCache
	// Description: Adds a node to the end of the row cache, as well as any
	//				child nodes that are expanded
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::AddNodeToRowCache(IndexerNode* node)
	{
		EnterCriticalSection(&rowCacheCritSec);
		try
		{
			//don't add if they won't be displayed
			if (node->filePosition)
			{
				node->nodeInfo.rowIndex = rowCache->GetLength();
				rowCache->Add(node);
			}

			if (!node->children)
				return;

			if (node->nodeInfo.expanded)
			{
				for (unsigned int i=0; i<node->children->GetLength(); i++)
					AddNodeToRowCache((*node->children)[i]);
			}
			else
			{
				for (unsigned int i=0; i<node->children->GetLength(); i++)
					ResetRowIndex((*node->children)[i]);
			}
		}
		catch(Exception *)
		{
		}
		__finally
		{
			LeaveCriticalSection(&rowCacheCritSec);
		}
	}

	//*************************************************************************
	// Method:		SortItems
	// Description: Sorts all the items in the list
	//
	// Parameters:
	//	rootNode - the root node of all the other nodes.  don't sort it
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::SortItems(IndexerNode *rootNode)
	{
		listMutex->WaitOne();

		bool foundRootNode = false;

		// get all the level 0 nodes, as we only want to sort those
		ArrayList *levelZeroList = new ArrayList();
		for (int i=0; i<sortedList->GetCount(); i++)
		{
			IndexerNode *node = sortedList->GetNodeAt(i);
			if (!node || (!node->nodeInfo.valid) || (node->nodeInfo.filtered) || (node->nodeInfo.level > 0))
				continue;

			if (node == rootNode)
				foundRootNode = true;
			else
				levelZeroList->Add(__box(UIntPtr(node)));
		}
		levelZeroList->Sort(this);

		// make sure we found the root node
		if (!foundRootNode)
			return;

		// remove all the root node's children and add them back in sorted
		rootNode->children->Clear();
		for (int i=0; i<levelZeroList->Count; i++)
			rootNode->children->Add((IndexerNode*)dynamic_cast<UIntPtr*>(levelZeroList->Item[i])->ToPointer());

		EnterCriticalSection(&rowCacheCritSec);
		try
		{
			// Regenerate the row cache
			rowCache->Clear();
		}
		catch(Exception *)
		{
		}
		__finally
		{
			LeaveCriticalSection(&rowCacheCritSec);
		}
		
		for (unsigned int i=0; i<rootNode->children->GetLength(); i++)
			AddNodeToRowCache((*rootNode->children)[i]);

		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		UpdateSortColumnCache
	// Description: Updates the cachedSortColumnEntry field for an IndexerNode
	//
	// Parameters:
	//	node - Node to update
	//  logItem - Log item associated with the node
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::UpdateSortColumnCache(IndexerNode* node, LogItem* logItem)
	{
		if (sortColumn == LogItemFields::TimeStampField)
			return;

		int nodeIndex = sortedList->IndexOf(node);
		if (nodeIndex == -1)
			return;
		if (nodeIndex >= cachedSortColumn->Count)
			return;

		switch (sortColumn)
		{
		case LogItemFields::ThreadField:
			cachedSortColumn->Item[nodeIndex] = logItem->ThreadID;
			break;
		case LogItemFields::CategoryField:
			cachedSortColumn->Item[nodeIndex] = logItem->Category;
			break;
		case LogItemFields::DllNameField:
			cachedSortColumn->Item[nodeIndex] = logItem->DllName;
			break;
		case LogItemFields::FunctionField:
			cachedSortColumn->Item[nodeIndex] = logItem->Function;
			break;
		case LogItemFields::ErrorCodeField:
			cachedSortColumn->Item[nodeIndex] = logItem->ErrorCode;
			break;
		case LogItemFields::ExceptionField:
			cachedSortColumn->Item[nodeIndex] = logItem->Exception;
			break;
		case LogItemFields::ReturnValueField:
			cachedSortColumn->Item[nodeIndex] = logItem->ReturnValue;
			break;
		default:
			{
				int paramIndex = sortColumn - LogItemFields::ParameterField;
				bool paramExists = ((paramIndex >= 0) && (paramIndex < logItem->Parameters->Count));
				
				if (paramExists)
					cachedSortColumn->Item[nodeIndex] = dynamic_cast<String*>(logItem->Parameters->get_Item(paramIndex));
				else
					cachedSortColumn->Item[nodeIndex] = S"";
				break;
			}
		}
	}

	//*************************************************************************
	// Method:		FillInIndexerNodeData
	// Description: Fills in an empty indexernode with data from the logitem
	//
	// Parameters:
	//	node - the node to fill in
	//	filePos - the file position the log item came from
	//	item - the log item to fill the node with
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::FillInIndexerNodeData(IndexerNode *node, unsigned long filePos, LogItem *item)
	{
		node->filePosition = filePos;
		if (item->UInt32CallDepth > 0)
			node->nodeInfo.level = (item->UInt32CallDepth % 3) + 1;
		else
			node->nodeInfo.level = 0;
		node->nodeInfo.expanded = false;
		node->nodeInfo.selected = false;
		node->nodeInfo.focused = false;
		UpdateSortColumnCache(node, item);
	}

	//*************************************************************************
	// Method:		AddLogEntry
	// Description: Adds a log entry to the indexer
	//
	// Parameters:
	//	node - the node to add
	//  filePos - the file position to set for the log entry
	//	logItem - the log item to fill the data with (can be NULL if indexer
	//			node is already filled in with FillInIndexerNodeData)
	//
	// Return Value: Pointer to the permanent location of this log entry
	//*************************************************************************
	IndexerNode* LogEntryIndexer::AddLogEntry(IndexerNode *node, unsigned long filePos, LogItem* logItem)
	{
		listMutex->WaitOne();

		if ((node->filePosition == 0) && (logItem != NULL))
			FillInIndexerNodeData(node, filePos, logItem);
		
		int i = sortedList->Add(*node);
		IndexerNode* finalPtr = sortedList->GetNodeAt(i);

		if (sortColumn != LogItemFields::TimeStampField)
		{
			// Make room for sort column cache and update it, but only if not using timestamp sort
			cachedSortColumn->Add(NULL);
			if (logItem != NULL)
				UpdateSortColumnCache(finalPtr, logItem);
		}

		listMutex->ReleaseMutex();
		return finalPtr;
	}

	//*************************************************************************
	// Method:		AddNode
	// Description: Adds a node to the indexer
	//
	// Parameters:
	//	node - the node to add
	//
	// Return Value: Pointer to the permanent location of this log entry
	//*************************************************************************
	IndexerNode* LogEntryIndexer::AddNode(IndexerNode *node) 
	{ 
		if (sortColumn != LogItemFields::TimeStampField)
		{
			// Make room for sort column cache, but only if not using timestamp sort
			cachedSortColumn->Add(NULL);
		}

		int i = sortedList->Add(*node);
		IndexerNode* finalPtr = sortedList->GetNodeAt(i);
		return finalPtr; 
	}


	//*************************************************************************
	// Method:		AddSortedChildNode
	// Description: Adds a child node to the root node in a sort friendly location
	//
	// Parameters:
	//	parentNode - the node to add the child to
	//	childNode - the node to add to the parent node
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::AddSortedChildNode(IndexerNode *parentNode, IndexerNode *childNode)
	{
		// Don't add node if it's supposed to be filtered out
		if (childNode->nodeInfo.filtered)
			return;

		listMutex->WaitOne();

		// see if we need to insert the node sorted as a child of the parent node
		int i = 0;

		if (sortColumn == LogItemFields::TimeStampField)
		{
			if (sortOrder == SortOrder::Ascending)
				i = parentNode->children->GetLength();
			// If descending, correct value is zero (i is already zero)
		}
		// Only search if there are actually elements in the list		
		else if (parentNode->children->GetLength() > 0)
		{
			// Place the new log entry in the correct location to leave the list sorted.  Use a
			// binary search to find the location fast.
			int min, max;
			min = 0;
			max = parentNode->children->GetLength() - 1;
			for (i = (min + max) / 2; min != max; i = (min + max) / 2)
			{
				int compareResult = Compare((*parentNode->children)[i], childNode);

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
			for (; i<(int)parentNode->children->GetLength(); i++)
			{
				int compareResult = Compare((*parentNode->children)[i], childNode);

				if (compareResult >= 0)
				{
					// We need to insert right here, break out now
					break;
				}
			}
		}

		EnterCriticalSection(&rowCacheCritSec);
		try
		{
			if (i == 0)
				childNode->nodeInfo.rowIndex = 0;
			else if (i < (int)parentNode->children->GetLength())
				childNode->nodeInfo.rowIndex = (*parentNode->children)[i]->nodeInfo.rowIndex;
			else
				childNode->nodeInfo.rowIndex = rowCache->GetLength();

			for (unsigned int j=i; j<parentNode->children->GetLength(); j++)
				(*parentNode->children)[j]->nodeInfo.rowIndex++;

			parentNode->children->Insert(i, childNode);
			rowCache->Insert(childNode->nodeInfo.rowIndex, childNode);
		}
		catch(Exception *)
		{
		}
		__finally
		{
			LeaveCriticalSection(&rowCacheCritSec);
		}

		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		GetLogEntry
	// Description: Gets a log entry at the location
	//
	// Parameters:
	//	index - the index of the log entry to get
	//
	// Return Value: the log entry at the location
	//*************************************************************************
	LogItem *LogEntryIndexer::GetLogEntry(unsigned long index)
	{
		listMutex->WaitOne();

		if ((index >= Count) || (holoScriptApp == NULL))
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		IndexerNode *node = sortedList->GetNodeAt(index);
		if (node == NULL)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		LogItem* item = holoScriptApp->GetLogEntryAtFilePosition(node->filePosition);
		listMutex->ReleaseMutex();
		return item;
	}

	//*************************************************************************
	// Method:		GetLogEntryFromFilePosition
	// Description: Gets a log entry based on the specified file position
	//
	// Parameters:
	//	filePos - the file position to get the log item from
	//
	// Return Value: the log entry for the file position
	//*************************************************************************
	LogItem *LogEntryIndexer::GetLogEntryFromFilePosition(unsigned long filePos)
	{
		return holoScriptApp->GetLogEntryAtFilePosition(filePos);
	}

	//*************************************************************************
	// Method:		GetIndexFromFilePosition
	// Description: Gets an index based on the specified file position
	//
	// Parameters:
	//	filePos - the file position to get the log item from
	//
	// Return Value: the index of the indexernode corresponding to this file position, -1 if not found
	//*************************************************************************
	int LogEntryIndexer::GetIndexFromFilePosition(unsigned long filePos)
	{
		listMutex->WaitOne();

		for (int i=0; i<sortedList->GetCount(); i++)
		{
			IndexerNode* node = sortedList->GetNodeAt(i);
			if (node && (node->filePosition == filePos))
			{
				listMutex->ReleaseMutex();
				return i;
			}
		}

		listMutex->ReleaseMutex();
		return -1;
	}

	//*************************************************************************
	// Method:		GetLogEntryFromNode
	// Description: Gets a log entry based on the specified node
	//
	// Parameters:
	//	node - the node to get the logitem for
	//
	// Return Value: the log entry for the node
	//*************************************************************************
	LogItem *LogEntryIndexer::GetLogEntryFromNode(IndexerNode *node)
	{
		if (!node || !holoScriptApp)
			return NULL;

		return holoScriptApp->GetLogEntryAtFilePosition(node->filePosition);
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
	void LogEntryIndexer::set_SortColumn(unsigned int value)
	{
		// Don't regenerate info unless the column has changed
		if (sortColumn == value)
			return;

		listMutex->WaitOne();

		if (value > LogItemFields::ParameterField + MAX_PARAMETER_COLUMNS)
			sortColumn = LogItemFields::ParameterField + MAX_PARAMETER_COLUMNS;
		else if (value < 0)
			sortColumn = 0;
		else
			sortColumn = value;

		cachedSortColumn->Clear();

		if (sortColumn != LogItemFields::TimeStampField)
		{
			// Recompute the cached sort column for all indexer nodes
			for (int i=0; i<sortedList->GetCount(); i++)
			{
				IndexerNode *node = sortedList->GetNodeAt(i);
				cachedSortColumn->Add(NULL);
				if (node->filePosition == 0)
					continue;
				LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePosition);
				UpdateSortColumnCache(node, item);
			}
		}

		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		GetNodeAt
	// Description: Finds the node at a specified index
	//
	// Parameters:
	//	index - index to get the node at
	//
	// Return Value: Node at the index, or NULL if invalid index
	//*************************************************************************
	IndexerNode *LogEntryIndexer::GetNodeAt(unsigned int index)
	{
		IndexerNode *retVal = NULL;

		listMutex->WaitOne();

		if (index >= (unsigned int)sortedList->GetCount())
			retVal = NULL;
		else
			retVal = sortedList->GetNodeAt(index);

		listMutex->ReleaseMutex();

		return retVal;
	}

	//*************************************************************************
	// Method:		GetNodeAtRow
	// Description: Finds the node at a specified row index
	//
	// Parameters:
	//	index - Row index
	//
	// Return Value: Node at row index, or NULL if invalid index
	//*************************************************************************
	IndexerNode *LogEntryIndexer::GetNodeAtRow(unsigned int index)
	{
		IndexerNode *retVal = NULL;

		EnterCriticalSection(&rowCacheCritSec);
		try
		{
			if (index >= (unsigned int)rowCache->GetLength())
				retVal = NULL;
			else
				retVal = (*rowCache)[index];
		}
		catch(Exception *)
		{
		}
		__finally
		{
			LeaveCriticalSection(&rowCacheCritSec);
		}

		return retVal;
	}

	//*************************************************************************
	// Method:		ExpandNode
	// Description: Expands a tree node
	//
	// Parameters:
	//	node - Node to expand
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::ExpandNode(IndexerNode* node)
	{
		if (node->nodeInfo.expanded)
			return; // already expanded
		if (!node->children)
			return;

		listMutex->WaitOne();

		EnterCriticalSection(&rowCacheCritSec);
		try
		{
			node->nodeInfo.expanded = true;
			for (unsigned int i=node->nodeInfo.rowIndex + 1; i<rowCache->GetLength(); i++)
				(*rowCache)[i]->nodeInfo.rowIndex += node->children->GetLength();
			for (unsigned int i=0; i<node->children->GetLength(); i++)
				(*node->children)[i]->nodeInfo.rowIndex = node->nodeInfo.rowIndex + i + 1;

			rowCache->InsertRange(node->nodeInfo.rowIndex + 1, *node->children);
		}
		catch(Exception *)
		{
		}
		__finally
		{
			LeaveCriticalSection(&rowCacheCritSec);
		}

		listMutex->ReleaseMutex();
	}
	
	//*************************************************************************
	// Method:		CollapseNode
	// Description: Collapses a tree node
	//
	// Parameters:
	//	value - Node to collapse
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::CollapseNode(IndexerNode* node)
	{
		if (!node->nodeInfo.expanded)
			return; // already collapsed
		if (!node->children)
			return; // no kids to collapse
		if (node->filePosition == 0)
			return;	// root node

		listMutex->WaitOne();

		// Collapse children that are still expanded
		for (unsigned int i=0; i<node->children->GetLength(); i++)
		{
			IndexerNode* childNode = (*node->children)[i];
			if (childNode->nodeInfo.expanded)
				CollapseNode(childNode);
			childNode->nodeInfo.rowIndex = INVALID_ROW_INDEX; // mark node as invisible
		}

		node->nodeInfo.expanded = false;

		EnterCriticalSection(&rowCacheCritSec);
		try
		{
			// Remove collapsed nodes from the row cache
			for (unsigned int i=node->nodeInfo.rowIndex + node->children->GetLength() + 1; i<rowCache->GetLength(); i++)
				(*rowCache)[i]->nodeInfo.rowIndex -= node->children->GetLength();
			rowCache->RemoveRange(node->nodeInfo.rowIndex + 1, node->children->GetLength());
		}
		catch(Exception *)
		{
		}
		__finally
		{
			LeaveCriticalSection(&rowCacheCritSec);
		}

		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		UnselectAllNodes
	// Description: Unselects all selected nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void LogEntryIndexer::UnselectAllNodes()
	{
		listMutex->WaitOne();

		for (int i=0; i<sortedList->GetCount(); i++)
		{
			sortedList->GetNodeAt(i)->nodeInfo.selected = false;
			sortedList->GetNodeAt(i)->nodeInfo.focused = false;
		}

		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		get_VisibleAndExpandedCount
	// Description: Gets the number nodes currenly visible and expanded in the
	//				tree.  Collapsed nodes that are not visible are not counted
	//
	// Parameters:
	//	None
	//
	// Return Value: Number of visible and expanded nodes
	//*************************************************************************
	unsigned int LogEntryIndexer::get_VisibleAndExpandedCount()
	{
		return rowCache->GetLength();
	}

	//*************************************************************************
	// Method:		IndexOf
	// Description: Gets the index of a node
	//
	// Parameters:
	//	node - Node to get index of (must have been returned by this
	//		LogEntryIndexer object)
	//
	// Return Value: Index of node, or -1 if not found
	//*************************************************************************
	int LogEntryIndexer::IndexOf(IndexerNode* node)
	{
		return sortedList->IndexOf(node);
	}
}
