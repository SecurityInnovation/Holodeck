//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogPaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class LogPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "SiArray.h"
#include "LogPaneDataArray.h"
#include "ImageListManager.h"

namespace UIData
{
	//*************************************************************************
	// Method:		LogPaneDataArray
	// Description: Constructor for the LogPaneDataArray class
	//
	// Parameters:
	//  executableFileName - name of the executable
	//	logFilePath - path & file name of the file to open
	//	oldProcId - process id for the old log
	//
	// Return Value: None
	//*************************************************************************
	LogPaneDataArray::LogPaneDataArray(String *executableFileName, String *logFilePath, int oldProcId, String *exeAndLogSaveTime)
	{
		Initialize(executableFileName, logFilePath, oldProcId, 0, exeAndLogSaveTime, new HoloScriptApplication());

		process = NULL;
		holoScriptApp->SavedLog = true;
	}

	//*************************************************************************
	// Method:		LogPaneDataArray
	// Description: Constructor for the LogPaneDataArray class
	//
	// Parameters:
	//	executableFileName - the filename that represents this log pane's process
	//	procID - The process id that this log represents
	//
	// Return Value: None
	//*************************************************************************
	LogPaneDataArray::LogPaneDataArray(String *executableFileName, unsigned int procID, unsigned int tID)
	{
		Initialize(executableFileName, "", procID, tID, "", HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID));

		try
		{
			process = System::Diagnostics::Process::GetProcessById(processID);
		}
		catch (...)
		{
			//process has already terminated
		}

		holoScriptApp->SavedLog = false;
	}

	//*************************************************************************
	// Method:		LogPaneDataArray
	// Description: Destructor for the LogPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogPaneDataArray::~LogPaneDataArray()
	{
		CleanUp();
	}

	//*************************************************************************
	// Method:		Initialize
	// Description: Initializes the class's members
	//
	// Parameters:
	//	executableFileName - the executable file name to log
	//	logFilePath - the path to the log file
	//	processID - the process id of the logged process
	//	threadID - the thread id of the logged thread
	//	oldProcId - the original procid of this log
	//	exeLogAndSaveTime - the saved log time stamp
	//	hsApp - the holoscriptapplication used to log the application
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::Initialize(String *executableFileName, String *logFilePath, unsigned int processID, unsigned int threadID, String *exeAndLogSaveTime, HoloScriptApplication *hsApp)
	{
		logPaneAttached = false;
		isLoaded = false;

		savedLogFilePath = logFilePath;
		this->exeAndLogSavedTime = exeAndLogSaveTime;
		this->executableFileName = executableFileName;
		this->processID = processID;
		this->threadID = threadID;
		totalEntries = 0;
		otherEntries = 0;
		cachedLogItemIndex = -1;
		internalLevelZeroNode = NULL;

		threadIDHashTable = new Hashtable();

		GetParentNodeHandler = new GetParentNodeDelegate(this, onGetParentNode);
		GetFirstChildHandler = new GetFirstChildDelegate(this, onGetFirstChildNode);
		GetLastChildHandler = new GetLastChildDelegate(this, onGetLastChildNode);
		GetNextChildHandler = new GetNextChildDelegate(this, onGetNextChildNode);
		GetPrevChildHandler = new GetPrevChildDelegate(this, onGetPrevChildNode);
		GetNextSiblingHandler = new GetNextSiblingDelegate(this, onGetNextSiblingNode);
		GetPrevSiblingHandler = new GetPrevSiblingDelegate(this, onGetPrevSiblingNode);
		GetChildNodeAtHandler = new GetChildNodeAtDelegate(this, onGetChildNodeAt);

		SetIsSelectedHandler = new SetIsSelectedDelegate(this, onSetIsSelected);
		SetIsExpandedHandler = new SetIsExpandedDelegate(this, onSetIsExpanded);
		SetIsFocusedHandler = new SetIsFocusedDelegate(this, onSetIsFocused);

		GetRowNumberHandler = new GetRowNumberDelegate(this, onGetRowNumber);
		GetNodeIndexHandler = new GetNodeIndexDelegate(this, onGetNodeIndex);

		GetChildNodeCountHandler = new GetChildNodeCountDelegate(this, onGetChildNodeCount);
		GetVisibleNodeCountHandler = new GetVisibleNodeCountDelegate(this, onGetVisibleNodeCount);
		
		IndexerNode rootNodeData;
		rootNodeData.nodeInfo.expanded = true;
		rootNodeData.nodeInfo.selected = false;
		rootNodeData.nodeInfo.focused = false;
		rootNodeData.nodeInfo.valid = true;
		rootNodeData.parent = NULL;
		rootNodeData.filePosition = 0;
		rootNodeData.nodeInfo.level = 0;

		this->holoScriptApp = hsApp;
		logHandler = new HoloScript::LogsGeneratedDelegate(this, onLogsGenerated);
		if (this->holoScriptApp)
		{
			indexer = new LogEntryIndexer(this->holoScriptApp);
			rootNode = indexer->AddNode(&rootNodeData);
			rootNode->children = new SiUtils::SiArray<IndexerNode*>();
		}

		try
		{
			rootTreeListNode = GetTreeListNodeAtIndex(0);
			rootTreeListNode->IsRoot = true;
		}
		catch(...)
		{
			// we won't be able to display logs here, but that's better than crashing
		}
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
	void LogPaneDataArray::CleanUp()
	{
		if (logPaneAttached)
			holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		OpenSavedLog
	// Description: opens and reads the saved log file
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void LogPaneDataArray::OpenSavedLog()
	{
		if (!holoScriptApp->SavedLog || isLoaded)
			return;

		holoScriptApp->OpenLogFile(savedLogFilePath);
		isLoaded = true;
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
	bool LogPaneDataArray::DeleteSavedLog()
	{
		return holoScriptApp->DeleteSavedLogFile(savedLogFilePath);
	}

	//*************************************************************************
	// Method:		onGetParentNode
	// Description: called when the pane needs to get a node's parent node
	//
	// Parameters:
	//	node - the node to get the parent of
	//
	// Return Value: the parent of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetParentNode(TreeListNode *node)
	{
		try
		{
			IndexerNode *indexerNode = (IndexerNode*)(dynamic_cast<UIntPtr*>(node->Tag)->ToPointer());
			if (!indexerNode)
				return NULL;

			if (indexerNode->parent)
				return GetTreeListNodeFromIndexerNode(indexerNode->parent);

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetFirstChildNode
	// Description: called when the pane needs to get a node's first child node
	//
	// Parameters:
	//	node - the node to get the first child of
	//
	// Return Value: the first child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetFirstChildNode(TreeListNode *node)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			if (!indexerNode)
				return NULL;

			if (indexerNode->children && (indexerNode->children->GetLength() > 0))
			{
				IndexerNode *firstChildNode = (*indexerNode->children)[0];
				if (!firstChildNode)
					return NULL;

				return GetTreeListNodeFromIndexerNode(firstChildNode);
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetLastChildNode
	// Description: called when the pane needs to get a node's last child node
	//
	// Parameters:
	//	node - the node to get the last child of
	//
	// Return Value: the last child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetLastChildNode(TreeListNode *node)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			if (!indexerNode)
				return NULL;

			if (indexerNode->children && (indexerNode->children->GetLength() > 0))
			{
				IndexerNode *lastChildNode = (*indexerNode->children)[indexerNode->children->GetLength() - 1];
				if (!lastChildNode)
					return NULL;

				return GetTreeListNodeFromIndexerNode(lastChildNode);
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetNextChildNode
	// Description: called when the pane needs to get a node's next child node
	//
	// Parameters:
	//	node - the node to get the next child of
	//	curChild - the current child of the node
	//
	// Return Value: the next child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetNextChildNode(TreeListNode *node, TreeListNode *curChild)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			IndexerNode *curChildIndexerNode = TagToIndexerNode(curChild->Tag);

			if (!indexerNode || !curChildIndexerNode)
				return NULL;

			if (indexerNode->children && (indexerNode->children->GetLength() > 0))
			{
				int thisIndex = indexerNode->children->IndexOf(curChildIndexerNode);
				if (thisIndex < (int)indexerNode->children->GetLength() - 1)
				{
					IndexerNode *nextChildNode = (*indexerNode->children)[thisIndex + 1];
					if (!nextChildNode)
						return NULL;

					return GetTreeListNodeFromIndexerNode(nextChildNode);
				}
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetPrevChildNode
	// Description: called when the pane needs to get a node's previous child node
	//
	// Parameters:
	//	node - the node to get the previous child of
	//	curChild - the current child of the node
	//
	// Return Value: the previous child of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetPrevChildNode(TreeListNode *node, TreeListNode *curChild)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			IndexerNode *curChildIndexerNode = TagToIndexerNode(curChild->Tag);

			if (!indexerNode || !curChildIndexerNode)
				return NULL;

			if (indexerNode->children && (indexerNode->children->GetLength() > 0))
			{
				int thisIndex = indexerNode->children->IndexOf(curChildIndexerNode);
				if (thisIndex > 0)
				{
					IndexerNode *prevChildNode = (*indexerNode->children)[thisIndex - 1];
					if (!prevChildNode)
						return NULL;

					return GetTreeListNodeFromIndexerNode(prevChildNode);
				}
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetChildNodeAt
	// Description: called when the pane needs to get a node's child node
	//
	// Parameters:
	//	node - the node to get the child of
	//	index - the index of the child to get
	//
	// Return Value: the specified child of the node, NULL if failed
	//*************************************************************************
	TreeListNode *LogPaneDataArray::onGetChildNodeAt(TreeListNode *node, unsigned int index)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			if (!indexerNode)
				return NULL;

			if (indexerNode->children && ((unsigned int)indexerNode->children->GetLength() > index))
			{
				IndexerNode *childNode = (*indexerNode->children)[index];
				if (!childNode)
					return NULL;

				return GetTreeListNodeFromIndexerNode(childNode);
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetNextSiblingNode
	// Description: called when the pane needs to get a node's next sibling node
	//
	// Parameters:
	//	node - the node to get the next sibling of
	//
	// Return Value: the next sibling of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetNextSiblingNode(TreeListNode *node)
	{
		try
		{
			ParentChildList *p = onGetParentNode(node);
			if (p)
			{
				TreeListNode *parentNode = dynamic_cast<TreeListNode *>(p);

				IndexerNode *parentIndexerNode = TagToIndexerNode(parentNode->Tag);
				IndexerNode *indexerNode = TagToIndexerNode(node->Tag);

				if (!parentIndexerNode || !indexerNode)
					return NULL;

				if (parentIndexerNode->children && (parentIndexerNode->children->GetLength() > 0))
				{
					int thisIndex = parentIndexerNode->children->IndexOf(indexerNode);
					if (thisIndex < (int)parentIndexerNode->children->GetLength() - 1)
					{
						IndexerNode *nextSiblingNode = (*parentIndexerNode->children)[thisIndex + 1];
						if (!nextSiblingNode)
							return NULL;

						return GetTreeListNodeFromIndexerNode(nextSiblingNode);
					}
				}
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onGetPrevSiblingNode
	// Description: called when the pane needs to get a node's previous sibling node
	//
	// Parameters:
	//	node - the node to get the previous sibling of
	//
	// Return Value: the previous sibling of the node, NULL if failed
	//*************************************************************************
	ParentChildList *LogPaneDataArray::onGetPrevSiblingNode(TreeListNode *node)
	{
		try
		{
			ParentChildList *p = onGetParentNode(node);
			if (p)
			{
				TreeListNode *parentNode = dynamic_cast<TreeListNode *>(p);

				IndexerNode *parentIndexerNode = TagToIndexerNode(parentNode->Tag);
				IndexerNode *indexerNode = TagToIndexerNode(node->Tag);

				if (!parentIndexerNode || !indexerNode)
					return NULL;

				if (parentIndexerNode->children && (parentIndexerNode->children->GetLength() > 0))
				{
					int thisIndex = parentIndexerNode->children->IndexOf(indexerNode);
					if (thisIndex > 0)
					{
						IndexerNode *prevSiblingNode = (*parentIndexerNode->children)[thisIndex - 1];
						if (!prevSiblingNode)
							return NULL;

						return GetTreeListNodeFromIndexerNode(prevSiblingNode);
					}
				}
			}

			return NULL;
		}
		catch(...)
		{
			return NULL;
		}
	}

	//*************************************************************************
	// Method:		onSetIsSelected
	// Description: called when the pane needs to set whether or not a node is
	//	selected
	//
	// Parameters:
	//	node - the node to set
	//	selected - true for selected, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::onSetIsSelected(TreeListNode *node, bool selected)
	{
		IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
		if (!indexerNode)
			return;

		indexerNode->nodeInfo.selected = selected;
	}

	//*************************************************************************
	// Method:		onSetIsExpanded
	// Description: called when the pane needs to set whether or not a node is
	//	expanded
	//
	// Parameters:
	//	node - the node to set
	//	expanded - true for expanded, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::onSetIsExpanded(TreeListNode *node, bool expanded)
	{
		IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
		if (!indexerNode || !indexer)
			return;

		if (expanded)
			indexer->ExpandNode(indexerNode);
		else
			indexer->CollapseNode(indexerNode);
	}

	//*************************************************************************
	// Method:		onSetIsFocused
	// Description: called when the pane needs to set whether or not a node is
	//	focused
	//
	// Parameters:
	//	node - the node to set
	//	focused - true for focused, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::onSetIsFocused(TreeListNode *node, bool focused)
	{
		IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
		if (!indexerNode)
			return;

		indexerNode->nodeInfo.focused = focused;
	}

	//*************************************************************************
	// Method:		onGetChildNodeCount
	// Description: called when the pane needs to get the number of child nodes
	//	of a particular node
	//
	// Parameters:
	//	node - the node to get the child count of
	//
	// Return Value: the child count of the node, 0 if failed
	//*************************************************************************
	int LogPaneDataArray::onGetChildNodeCount(TreeListNode *node)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			if (!indexerNode || !indexerNode->children)
				return 0;

			return GetChildNodeCount(indexerNode);
		}
		catch(...)
		{
			return 0;
		}
	}

	//*************************************************************************
	// Method:		GetVisibleNodeCount
	// Description: gets the number of visible nodes under the specified node
	//
	// Parameters:
	//	node - the node to get the visible count of
	//
	// Return Value: the visible count of the node (always at least 1)
	//*************************************************************************
	int LogPaneDataArray::GetVisibleNodeCount(IndexerNode *node)
	{
		// start at 1 for this node
		int result = 1;

		if (node->nodeInfo.expanded && node->children)
		{
			for (unsigned int i = 0; i < node->children->GetLength(); i++)
			{
				result += GetVisibleNodeCount((*node->children)[i]);
			}
		}
		return result;
	}

	//*************************************************************************
	// Method:		GetChildNodeCount
	// Description: gets the number of child nodes of a particular node
	//
	// Parameters:
	//	node - the node to get the child count of
	//
	// Return Value: the child count of the node, 0 if failed
	//*************************************************************************
	int LogPaneDataArray::GetChildNodeCount(IndexerNode *node)
	{
		int result = 0;

		if (node && node->children)
		{
			for (unsigned int i = 0; i < node->children->GetLength(); i++)
			{
				IndexerNode *childNode = (*node->children)[i];
				//don't count nodes that won't display
				if (childNode && (childNode->filePosition == 0))
					continue;

				result += GetChildNodeCount(childNode) + 1;
			}
		}
		return result;
	}

	//*************************************************************************
	// Method:		onGetVisibleNodeCount
	// Description: called when the pane needs to get the number of visible nodes
	//	of a particular node
	//
	// Parameters:
	//	node - the node to get the visible count of
	//
	// Return Value: the visible count of the node, 0 if failed
	//*************************************************************************
	int LogPaneDataArray::onGetVisibleNodeCount(TreeListNode *node)
	{
		try
		{
			IndexerNode *indexerNode = TagToIndexerNode(node->Tag);
			if (!indexerNode)
				return 0;

			if (!indexerNode->children)
				return 1;

			return GetVisibleNodeCount(indexerNode);
		}
		catch(...)
		{
			return 0;
		}
	}

	//*************************************************************************
	// Method:		GetTreeListNodeAtRow
	// Description: called when the pane needs to get the node at a specific
	//				row index
	//
	// Parameters:
	//	row - index of row
	//
	// Return Value: the node at the row index
	//*************************************************************************
	TreeListNode *LogPaneDataArray::GetTreeListNodeAtRow(unsigned int row)
	{
		if (!indexer)
			return NULL;

		return GetTreeListNodeFromIndexerNode(indexer->GetNodeAtRow(row));
	}

	//*************************************************************************
	// Method:		get_ExecutablePath
	// Description: ExecutablePath property get method
	//
	// Parameters:
	//	None
	//
	// Return Value: the string representing the path to the executable for this log
	//	data array
	//*************************************************************************
	String *LogPaneDataArray::get_ExecutablePath() 
	{
		try
		{
			String *fullPath = process->MainModule->FileName;
			int pos = fullPath->LastIndexOf("\\");
			if (pos == -1)
				fullPath = "";
			else
				fullPath = fullPath->Substring(0, pos);

			return fullPath;
		}
		catch(Exception *)
		{
			return "";
		}
	}

	//*************************************************************************
	// Method:		get_ExecutableSize
	// Description: ExecutableSize property get method
	//
	// Parameters:
	//	None
	//
	// Return Value: the size of the executable for this log
	//	data array
	//*************************************************************************
	__int64 LogPaneDataArray::get_ExecutableSize() 
	{
		try
		{
			FileStream *stream = File::OpenRead(FullExecutableName);
		
			return stream->Length;
		}
		catch(Exception *)
		{
			return 0;
		}
	}

	//*************************************************************************
	// Method:		get_FullExecutableName
	// Description: FullExecutableName property get method
	//
	// Parameters:
	//	None
	//
	// Return Value: the string representing the full path to the executable for this log
	//	data array
	//*************************************************************************
	String *LogPaneDataArray::get_FullExecutableName() 
	{
		String *applicationName = this->ExecutablePath;
		if (applicationName->CompareTo("") != 0)
			applicationName = String::Concat(applicationName, "\\", this->ExecutableFileName);
		else
			applicationName = this->ExecutableFileName;

		return applicationName;
	}

	//*************************************************************************
	// Method:		get_StartTime
	// Description: StartTime property get method
	//
	// Parameters:
	//	None
	//
	// Return Value: the datetime object representing the start time
	//*************************************************************************
	DateTime LogPaneDataArray::get_StartTime() 
	{
		return process->StartTime;
	}

	//*************************************************************************
	// Method:		get_ElapsedTime
	// Description: ElapsedTime property get method
	//
	// Parameters:
	//	None
	//
	// Return Value: the timespan object representing the elapsed time
	//*************************************************************************
	TimeSpan LogPaneDataArray::get_ElapsedTime() 
	{
		if (process->HasExited)
			return process->ExitTime.Subtract(process->StartTime);
		else
			return DateTime::Now.Subtract(process->StartTime);
	}

	//*************************************************************************
	// Method:		get_RootNode
	// Description: gets the root node of the nodes
	//
	// Parameters:
	//	None
	//
	// Return Value: the root node of the nodes
	//*************************************************************************
	TreeListNode *LogPaneDataArray::get_RootNode()
	{
		return rootTreeListNode;
	}

	//*************************************************************************
	// Method:		AddLogItemToBufferedTreeStructure
	// Description: Since logs are sent to the log file in reverse order (since
	//	we log after the API call we are logging we have a problem of needing to
	//	add nodes before we know the parent node).  The solution to this is
	//	implemented below and it flows basically like the following:
	//
	//	1. Get the log
	//	2. If it is not level 0, 
	//		i. Check for existence of empty level (n-1) node
	//			1) If it exists, add level n node as a child
	//			2) Else create the level n-1 node
	//			3) Repeat step a. with the level n-1 node
	//		ii. If there is an empty node with children at level n
	//			1) Fill in the empty node
	//	3. If it is level 0
	//		a. Fill in the level 0 data
	//		b. Recursively add the level 0 node and its children to the indexer
	//
	// Parameters:
	//	item - the item to add to the structure
	//  itemFiltered - true if the item is filtered out
	//
	// Return Value: true if the structure is ready to be written, false otherwise
	//*************************************************************************
	bool LogPaneDataArray::AddLogItemToBufferedTreeStructure(LogItem *item, bool itemFiltered)
	{
		if (!indexer)
			return false;

		int depth = (int)item->UInt32CallDepth;
		// if this is the level 0 node
		if (depth == 0)
		{
			// create it if it doesn't exist.  This will only happen here if it has no child nodes
			if (internalLevelZeroNode == NULL)
			{
				IndexerNode newNode;
				newNode.nodeInfo.valid = false;
				internalLevelZeroNode = indexer->AddLogEntry(&newNode, item->FilePosition, item);
			}
			
			indexer->FillInIndexerNodeData(internalLevelZeroNode, item->FilePosition, item);
			internalLevelZeroNode->parent = rootNode;
			internalLevelZeroNode->nodeInfo.filtered = itemFiltered;

			return true;
		}

		// this must be a subnode of some future level 0 node
		// the level zero node will need to exist, but make sure it doesn't already
		if (internalLevelZeroNode == NULL)
		{
			IndexerNode newNode;
			newNode.nodeInfo.valid = false;
			internalLevelZeroNode = indexer->AddLogEntry(&newNode, 0, NULL);
		}

		int currentLevel = 0;
		IndexerNode *parentNode = internalLevelZeroNode;
		IndexerNode *correctNode = NULL;
		while (currentLevel < depth)
		{
			// must contain children, so make sure the arraylist is initialized so we can stick 
			// the child in it
			if (!parentNode->children)
				parentNode->children = new SiUtils::SiArray<IndexerNode*>();

			// look for the empty node if there are any
			correctNode = NULL;
			for (unsigned int i = 0; i < parentNode->children->GetLength(); i++)
			{
				IndexerNode* currNode = (*parentNode->children)[i];
				if (currNode && (currNode->filePosition == 0))
				{
					correctNode = currNode;
					break;
				}
			}
			
			// if it was found in the search above
			if (correctNode)
				parentNode = correctNode;
			else
			{
				IndexerNode newNode;
				newNode.nodeInfo.valid = false;
				newNode.parent = parentNode;
				correctNode = indexer->AddLogEntry(&newNode, 0, NULL);
				parentNode->children->Add(correctNode);
				parentNode = correctNode;
			}
			currentLevel++;
		}

		indexer->FillInIndexerNodeData(correctNode, item->FilePosition, item);
		correctNode->nodeInfo.filtered = itemFiltered;

		return false;
	}

	//*************************************************************************
	// Method:		AddNodeToIndexer
	// Description: takes a single node from the buffered tree structure and 
	//	writes it to the indexer
	//
	// Parameters:
	//	node - the node to add to the indexer
	//	numAdded - the number of nodes added so far
	//	filtered - the number of filtered nodes so far
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::AddNodeToIndexer(IndexerNode *node, unsigned int *numAdded, unsigned int *filtered)
	{
		if (!indexer)
			return;

		if (node->nodeInfo.filtered)
			(*filtered)++;
		else
		{
			currentEntries++;
			(*numAdded)++;
		}
		node->nodeInfo.rowIndex = INVALID_ROW_INDEX; // -1 represents items that are not in the row cache
		node->nodeInfo.valid = true;
	}

	//*************************************************************************
	// Method:		AddBufferedTreeStructureToIndexer
	// Description: takes a buffered tree structure and writes it to the indexer
	//
	// Parameters:
	//	node - the top level node to add to the indexer
	//	numAdded - the number of nodes added so far
	//	filtered - the number of filtered nodes so far
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::AddBufferedTreeStructureToIndexer(IndexerNode *node, unsigned int *numAdded, unsigned int *filtered)
	{
		if (node->filePosition != 0)
			AddNodeToIndexer(node, numAdded, filtered);

		if (node->children)
		{
			for (unsigned int i = 0; i < node->children->GetLength(); i++)
			{
				if ((*node->children)[i]->filePosition == 0)
				{
					// Child is an empty node, remove it and add its children to this node (empty nodes
					// may be left behind by AddLogItemToBufferedTreeStructure in some multithreaded cases)
					IndexerNode* childNode = (*node->children)[i];
					node->children->Remove(i);

					if (childNode->children)
					{
						for (unsigned int j=0; j<childNode->children->GetLength(); j++)
						{
							// Update level (which is mod 3 + 1 in the structure) to reflect the new position
							if ((*childNode->children)[j]->nodeInfo.level == 1)
								(*childNode->children)[j]->nodeInfo.level = 3;
							else
								(*childNode->children)[j]->nodeInfo.level--;
							(*childNode->children)[j]->parent = node;
							node->children->Insert(i + j, (*childNode->children)[j]);
						}
					}

					i--;
					continue;
				}

				AddBufferedTreeStructureToIndexer((*node->children)[i], numAdded, filtered);
			}
		}
	}

	//*************************************************************************
	// Method:		GetLogItemAtIndex
	// Description: Returns a the log item for the specified tree list index
	//
	// Parameters:
	//	index - the index of the indexer to get the item for
	//
	// Return Value: the logitem at the index, NULL if unsuccessful
	//*************************************************************************
	LogItem *LogPaneDataArray::GetLogItemAtIndex(unsigned int index)
	{
		if (index == -1)
			return NULL;

		if (!indexer)
			return NULL;

		IndexerNode *currNode = indexer->GetNodeAt(index);
		if (!currNode)
			return NULL;

		if (currNode == rootNode)
			return NULL;

		return indexer->GetLogEntryFromNode(currNode);
	}

	//*************************************************************************
	// Method:		GetTreeListNodeAtIndex
	// Description: Returns a treelistnode object representing the specified 
	//	tree list index
	//
	// Parameters:
	//	index - the index of the indexer to get the node for
	//
	// Return Value: the treelistnode object at the index, NULL if unsuccessful
	//*************************************************************************
	TreeListNode *LogPaneDataArray::GetTreeListNodeAtIndex(unsigned int index)
	{
		if (index == -1)
			return NULL;

		if (!indexer)
			return NULL;

		IndexerNode *currNode = indexer->GetNodeAt(index);
		return GetTreeListNodeFromIndexerNode(currNode);
	}

	//*************************************************************************
	// Method:		GetTreeListNodeFromIndexerNode
	// Description: Returns a treelistnode object representing the specified 
	//	indexer node
	//
	// Parameters:
	//	currNode - the indexer node to get the treelistnode object for
	//
	// Return Value: the treelistnode object, NULL if unsuccessful
	//*************************************************************************
	TreeListNode *LogPaneDataArray::GetTreeListNodeFromIndexerNode(IndexerNode* currNode)
	{
		if (!currNode || !indexer)
			return NULL;

		if (currNode == rootNode)
		{
			TreeListNode *retVal = new TreeListNode();
			retVal->Text = "Root";
			retVal->IsExpanded = currNode->nodeInfo.expanded;
			retVal->Selected = currNode->nodeInfo.selected;
			retVal->Focused = currNode->nodeInfo.focused;
			retVal->IsVisible = true;
			retVal->Tag = __box(UIntPtr(currNode));
			InitTreeListNodeDelegates(retVal);
			return retVal;
		}

		LogItem *logItem = indexer->GetLogEntryFromNode(currNode);
		if (!logItem)
			return NULL;

		TreeListNode *retVal = new TreeListNode();
		retVal->Text = logItem->TimeStamp;
		retVal->SubItems->Add(logItem->ThreadID);
		retVal->SubItems->Add(logItem->Category);
		retVal->SubItems->Add(logItem->DllName);
		retVal->SubItems->Add(logItem->Function);
		retVal->SubItems->Add(logItem->ReturnValue->Replace("\n", " "));

		//.net gives UINT -1 for errorcode, this should be ignored
		if (logItem->ErrorCode->CompareTo("4294967295") != 0)
		{
			String *ecString = ErrorCodeDB::GetInstance()->GetStringFromCode(logItem->ErrorCode); 
			if (ecString)
				retVal->SubItems->Add(ecString);
			else
				retVal->SubItems->Add(logItem->ErrorCode);
		}
		else
		{
			// Add an empty string to the errorcode column for .NET
			retVal->SubItems->Add("");
		}

		retVal->SubItems->Add(logItem->Exception);
		for (int i = 0; i < logItem->Parameters->Count; i++)
		{
			retVal->SubItems->Add(dynamic_cast<String *>(logItem->Parameters->get_Item(i))->Replace("\n", " "));
		}

		retVal->Tag = __box(UIntPtr(currNode));
		retVal->IsExpanded = currNode->nodeInfo.expanded;
		retVal->Selected = currNode->nodeInfo.selected;
		retVal->Focused = currNode->nodeInfo.focused;
		retVal->UseItemStyleForSubItems = true;
		InitTreeListNodeDelegates(retVal);

		// don't color the top level nodes, just child nodes
		if (currNode->nodeInfo.level > 0)
		{
			int choice = currNode->nodeInfo.level - 1;
			switch(choice)
			{
			case 0:
				retVal->BackColor = Color::Pink;
				break;
			case 1:
				retVal->BackColor = Color::MistyRose;
				break;
			case 2:
				retVal->BackColor = Color::LightPink;
				break;
			}
		}

		if (logItem->NetworkLogItems->Count > 0)
			retVal->ImageIndex = HolodeckGui::LogListViewIconTypes::NetworkLogIcon;

		return retVal;
	}

	//*************************************************************************
	// Method:		InitTreeListNodeDelegates
	// Description: attaches delegates to a treelistnode item
	//
	// Parameters:
	//	node - the node to attach the delegates to
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::InitTreeListNodeDelegates(TreeListNode *node)
	{
		node->GetParentNode += GetParentNodeHandler;
		node->GetFirstChild += GetFirstChildHandler;
		node->GetLastChild += GetLastChildHandler;
		node->GetNextChild += GetNextChildHandler;
		node->GetPrevChild += GetPrevChildHandler;
		node->GetNextSibling += GetNextSiblingHandler;
		node->GetPrevSibling += GetPrevSiblingHandler;
		node->GetChildNodeAt += GetChildNodeAtHandler;
		node->SetIsExpanded += SetIsExpandedHandler;
		node->SetIsSelected += SetIsSelectedHandler;
		node->SetIsFocused += SetIsFocusedHandler;
		node->GetRowNumber += GetRowNumberHandler;
		node->GetNodeIndex += GetNodeIndexHandler;
		node->GetChildNodeCount += GetChildNodeCountHandler;
		node->GetVisibleNodeCount += GetVisibleNodeCountHandler;
	}

	//*************************************************************************
	// Method:		onGetRowNumber
	// Description: Called when pane needs to know the row number of a node
	//
	// Parameters:
	//	node - the node to get the row number of
	//	visibleOnly - true to only count the visible nodes, false to count them all
	//
	// Return Value: the row number of the node
	//*************************************************************************
	int LogPaneDataArray::onGetRowNumber(TreeListNode *node, bool visibleOnly)
	{
		if (!indexer)
			return -1;

		IndexerNode *curNode = TagToIndexerNode(node->Tag);
		if (!visibleOnly)
			return indexer->IndexOf(curNode);

		if (!curNode || !curNode->parent)
			return -1;

		if (curNode->nodeInfo.rowIndex == INVALID_ROW_INDEX)
			return -1;
		else
			return curNode->nodeInfo.rowIndex;
	}

	//*************************************************************************
	// Method:		onGetNodeIndex
	// Description: Called when pane needs to the index of a node in another node
	//
	// Parameters:
	//	parent - the node containing the child node
	//	child - the child node to get the index of 
	//
	// Return Value: the index of the child node in the parent node
	//*************************************************************************
	int LogPaneDataArray::onGetNodeIndex(TreeListNode *parent, TreeListNode *child)
	{
		IndexerNode *parentIndexerNode = TagToIndexerNode(parent->Tag);
		IndexerNode *childIndexerNode = TagToIndexerNode(child->Tag);
		if (!parentIndexerNode || !childIndexerNode || !parentIndexerNode->children)
			return 0;

		return parentIndexerNode->children->IndexOf(childIndexerNode);
	}

	//*************************************************************************
	// Method:		OnLogsGenerated
	// Description: Called when logs are generated for the process
	//
	// Parameters:
	//	numNewLogs - the number of new logs generated
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int processID,
		unsigned int threadID)
	{
		unsigned long originalTotalEntries = totalEntries;
		totalEntries += numNewLogs;
		
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

				bool itemFiltered = false;
				UInt32 itemThreadID = Convert::ToUInt32(item->ThreadID);
				// Check the log filter to see if we really should be add this to the list view.  If filtered or if threadID doesn't match don't add.
				if ( (FilterLogItem && (FilterLogItem->Invoke (item))) || ((this->ThreadID != 0) && (this->ThreadID != itemThreadID)) )
				{
					if ( (this->threadID != 0) && (this->threadID != itemThreadID) )
						continue;
					itemFiltered = true;
				}

				// check if this is a new thread id
				String *itemThreadIDString = itemThreadID.ToString();
				if (!threadIDHashTable->Contains(itemThreadIDString))
				{
					threadIDHashTable->Add(itemThreadIDString, itemThreadIDString);
					if (LoggedNewThreadID)
						LoggedNewThreadID->Invoke(this->ProcessID, itemThreadID);
				}

				if (!AddLogItemToBufferedTreeStructure(item, itemFiltered))
					continue;

				AddBufferedTreeStructureToIndexer(internalLevelZeroNode, &numAdded, &filtered);
				if (!itemFiltered && indexer)
					indexer->AddSortedChildNode(rootNode, internalLevelZeroNode);

				internalLevelZeroNode = NULL;
			}
			cachedLogItemIndex = -1; // Log item cache may no longer be valid

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
	unsigned int LogPaneDataArray::FilterCurrentItems()
	{
		if (!indexer)
			return 0;

		listMutex->WaitOne();

		// filter the top level items
		unsigned int returnValue = 0;
		for (unsigned int i=0; i<indexer->Count; i++)
		{
			IndexerNode *node = indexer->GetNodeAt(i);
			// if the node isn't valid or it isn't a level 0 node, skip it
			if (!node || (!node->nodeInfo.valid) || (node->nodeInfo.level != 0))
				continue;

			// make sure it is a valid file position or we hang
			if (node->filePosition == 0)
				continue;

			LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePosition);
			if (!item)
				continue;

			if (FilterLogItem && (FilterLogItem->Invoke (item)))
			{
				// Item should now be filtered out
				if (!node->nodeInfo.filtered)
				{
					returnValue++;
					node->nodeInfo.filtered = true;
				}
			}
			else
			{
				// Item should now be shown
				if (node->nodeInfo.filtered)
				{
					returnValue--;
					node->nodeInfo.filtered = false;
				}
			}

			delete item;
		}

		cachedLogItemIndex = -1; // Log item cache is no longer valid
		listMutex->ReleaseMutex();

		return returnValue;
	}

	//*************************************************************************
	// Method:		ExportSavedLogItemsToFile
	// Description: exports a saved log file to a text only file
	//
	// Parameters:
	//	fileName - the file to export to
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::ExportSavedLogItemsToFile(String *fileName)
	{
		if (!holoScriptApp || !indexer)
			return;

		ArrayList *filePositions = new ArrayList();
		for (unsigned int i=0; i<indexer->Count; i++)
		{
			IndexerNode *node = indexer->GetNodeAt(i);
			if (!node)
				continue;

			if (node->filePosition == 0)
				continue;

			filePositions->Add(__box(node->filePosition));
		}

		holoScriptApp->ExportToDiffFile(fileName, filePositions);
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
	void LogPaneDataArray::SortItems()
	{
		if (!indexer)
			return;

		listMutex->WaitOne();
		indexer->SortItems(rootNode);
		cachedLogItemIndex = -1; // Log item cache is no longer valid
		listMutex->ReleaseMutex();
	}

	//*************************************************************************
	// Method:		GetLogItemAt
	// Description: Gets the nth visible log item from the indexer
	//
	// Parameters:
	//	listViewIndex - the index of the item to get
	//
	// Return Value: the LogItem object at the listViewIndex position, null if
	//	not found
	//*************************************************************************
	LogItem *LogPaneDataArray::GetLogItemAt(int listViewIndex)
	{
		if (!indexer)
			return NULL;

		listMutex->WaitOne();

		if (indexer->Count <= 0)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		if (listViewIndex >= (int)indexer->Count)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		if (cachedLogItemIndex == listViewIndex)
		{
			LogItem* item = cachedLogItem;
			listMutex->ReleaseMutex();
			return item;
		}

		IndexerNode *node = indexer->GetNodeAt(listViewIndex);
		if (!node)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		if (node->filePosition == 0)
		{
			listMutex->ReleaseMutex();
			return NULL;
		}

		LogItem *item = holoScriptApp->GetLogEntryAtFilePosition(node->filePosition);

		cachedLogItemIndex = listViewIndex;
		cachedLogItem = item;

		listMutex->ReleaseMutex();
		return item;
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
	LogPaneDataNode *LogPaneDataArray::GetSortedDataNodeAt(int index)
	{
		if (index < 0)
			return NULL;

		LogItem *item = GetLogItemAt(index);
		if (!item)
			return NULL;

		LogPaneDataNode *dataNode = new LogPaneDataNode(item);
		dataNode->Name = this->get_ExecutableFileName();
		dataNode->processID = this->get_ProcessID();
		dataNode->threadID = this->threadID;

		return dataNode;
	}

	//*************************************************************************
	// Method:		GetParentIndex
	// Description: Gets the index of the parent of the node at the specified
	//	index
	//
	// Parameters:
	//	index - the index of the node to get the parent of
	//	parentIndex (out) - the index of the node's parent node
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LogPaneDataArray::GetParentIndex(unsigned int index, unsigned int *parentIndex)
	{
		if (!indexer)
			return false;

		IndexerNode *node = indexer->GetNodeAt(index);
		if (!node)
			return false;

		if (!node->parent)
			return false;

		*parentIndex = indexer->IndexOf(node->parent);
		return true;
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
	void LogPaneDataArray::UnselectAllNodes()
	{
		if (indexer)
			indexer->UnselectAllNodes();
	}

	//*************************************************************************
	// Method:		ExportDataToFile
	// Description: writes all data to a human readable text file
	//
	// Parameters:
	//	fileName - the name of the file to write the log data to
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::ExportDataToFile(String *fileName)
	{
		ArrayList *filePositions = new ArrayList();
		for (unsigned int i=0; i<indexer->Count; i++)
		{
			IndexerNode *node = indexer->GetNodeAt(i);
			if (!node)
				continue;

			if (node->filePosition == 0)
				continue;

			filePositions->Add(__box(node->filePosition));
		}

		holoScriptApp->ExportToDiffFile(fileName, filePositions);
	}

	//*************************************************************************
	// Method:		TagToIndexerNode
	// Description: Converts tag on a TreeListNode to an IndexerNode
	//
	// Parameters:
	//	tag - Node tag
	//
	// Return Value: Indexer node referenced by tag
	//*************************************************************************
	IndexerNode* LogPaneDataArray::TagToIndexerNode(Object* tag)
	{
		UIntPtr* ptr = dynamic_cast<UIntPtr*>(tag);
		if (!ptr)
			return NULL;
		return (IndexerNode*)(ptr->ToPointer());
	}

	//*************************************************************************
	// Method:		LogPaneAttach
	// Description: Notifies the data array that a log pane is ready to
	//				display the contents of it.  This will force the
	//				LogEntryIndexer to be up to date from now on.
	//
	// Parameters:
	//	fileName - the name of the file to write the log data to
	//
	// Return Value: None
	//*************************************************************************
	void LogPaneDataArray::LogPaneAttach()
	{
		if (!logPaneAttached)
		{
			logPaneAttached = true;
			holoScriptApp->LockOnLogGenerated();

			holoScriptApp->AddOnLogGeneratedHandler(logHandler);

			// Add log items to the indexer
			LogItem* item __gc[] = new LogItem* __gc[1];
			for (unsigned long i=0; i<holoScriptApp->LogEntryCount; i++)
			{
				unsigned long filePos;
				item[0] = holoScriptApp->GetLogEntryAt(i, filePos);
				item[0]->FilePosition = filePos;
				logHandler->Invoke(1, item, this->ProcessID, 0);
			}

			holoScriptApp->UnlockOnLogGenerated();
		}
	}
}