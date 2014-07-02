//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogEntryIndexer.h
//
// DESCRIPTION: Contains definition for the class LogEntryIndexer
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include "defines.h"
#include "LogPaneDataNode.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>
#using <HoloScript.dll>

#define INDEXER_SECTION_BITS		10
#define INDEXER_SECTION_SIZE		(1 << INDEXER_SECTION_BITS)
#define INDEXER_SECTION_MASK		(INDEXER_SECTION_SIZE - 1)

#define ROW_INDEX_BITS 25
#define INVALID_ROW_INDEX ((1 << ROW_INDEX_BITS) - 1)


using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace HoloScript;

namespace SiUtils
{
	template <class T> class SiArray;
}

namespace UIData
{
	public __value enum LogItemFields
	{
		TimeStampField,
		ThreadField,
		CategoryField,
		DllNameField,
		FunctionField,
		ReturnValueField,
		ErrorCodeField,
		ExceptionField,
		ParameterField
	};

	// Declare bit field structure to save memory, use same type for all fields or the savings will be negated
	__nogc struct IndexerNodeInfo
	{
		unsigned long rowIndex : 25;
		unsigned long level : 2;
		unsigned long expanded : 1;
		unsigned long selected : 1;
		unsigned long focused : 1;
		unsigned long filtered : 1;
		unsigned long valid : 1;
	};

	__nogc struct IndexerNode
	{
		IndexerNode* parent;
		SiUtils::SiArray<IndexerNode*>* children;
		unsigned long filePosition;
		IndexerNodeInfo nodeInfo;

		IndexerNode();
		~IndexerNode();
	};

	__nogc class IndexerNodeList
	{
		SiUtils::SiArray<IndexerNode*>* sections;
		int count;

	public:
		IndexerNodeList();
		~IndexerNodeList();

		int Add(IndexerNode& node);
		int GetCount();
		IndexerNode* GetNodeAt(int index);
		int IndexOf(IndexerNode* node);
	};

	//*************************************************************************
	// class that indexes and sorts log entries
	//*************************************************************************
	public __gc class LogEntryIndexer : public IComparer
	{
	public:
		LogEntryIndexer(HoloScriptApplication *holoScriptApplication);
		~LogEntryIndexer();

		__property unsigned int get_Count() { return sortedList->GetCount(); }
		__property unsigned int get_SortOrder() { return sortOrder; }
		__property void set_SortOrder(unsigned int value) { sortOrder = value; }
		__property unsigned int get_SortColumn() { return sortColumn; }
		__property void set_SortColumn(unsigned int value);
		__property unsigned int get_VisibleAndExpandedCount();

		void SortItems(IndexerNode *rootNode);
		static bool hasAlpha(String *value);
		IndexerNode* AddLogEntry(IndexerNode* node, unsigned long filePos, LogItem* logItem);
		IndexerNode* AddNode(IndexerNode* node);
		void FillInIndexerNodeData(IndexerNode *node, unsigned long filePos, LogItem *item);
		LogItem *GetLogEntry(unsigned long index);
		LogItem *GetLogEntryFromNode(IndexerNode *node);
		LogItem *GetLogEntryFromFilePosition(unsigned long filePos);
		int GetIndexFromFilePosition(unsigned long filePos);
		IndexerNode *GetNodeAt(unsigned int index);
		IndexerNode *GetNodeAtRow(unsigned int index);
		int Compare(Object *obj1, Object *obj2);
		int Compare(IndexerNode *node1, IndexerNode *node2);
		void AddSortedChildNode(IndexerNode *parentNode, IndexerNode *childNode);
		void ExpandNode(IndexerNode* node);
		void CollapseNode(IndexerNode* node);
		void UnselectAllNodes();
		int IndexOf(IndexerNode* node);

	private:
		HoloScriptApplication *holoScriptApp;
		IndexerNodeList* sortedList;
		ArrayList *cachedSortColumn;
		SiUtils::SiArray<IndexerNode*>* rowCache;
		unsigned int sortColumn;
		unsigned int sortOrder;
		static System::Threading::Mutex* listMutex = new System::Threading::Mutex(false);

		void UpdateSortColumnCache(IndexerNode* node, LogItem* logItem);
		void AddNodeToRowCache(IndexerNode* node);
		void ResetRowIndex(IndexerNode* node);
	};
}