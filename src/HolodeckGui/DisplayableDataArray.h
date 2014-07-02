//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DisplayableDataArray.h
//
// DESCRIPTION: Contains definition for the class DisplayableDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Windows.Forms.dll>

#include "DisplayableDataNode.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace UIData
{
	public __delegate void ItemAddedDelegate(DisplayableDataNode *Node);
	public __delegate void ItemDeletedDelegate(DisplayableDataNode *Node);
	public __delegate void ItemModifiedDelegate(DisplayableDataNode *Node, String *propertyName);

	public __abstract __gc class DisplayableDataArray
	{
	public:

		[Browsable(false)]
		__property int get_Count() {return itemList->Count;}

		[Description("The Process ID this item is associated with"), Category("Owner Info")]
		__property unsigned int get_ProcessID() { return processID; }
		[Description("The Thread ID this item is associated with"), Category("Owner Info")]
		__property unsigned int get_ThreadID() { return threadID; }

		DisplayableDataArray();
		~DisplayableDataArray();
		virtual void CleanUp();


		DisplayableDataNode *GetDataNodeByIndex(int Index);
		DisplayableDataNode *GetDataNodeByID(String* ID);
		ItemAddedDelegate *onItemAdded;
		ItemDeletedDelegate *onItemDeleted;
		ItemModifiedDelegate *onItemModified;

		bool BeginDeleteNode(DisplayableDataNode *dataNode);
		bool BeginCreateNode(DisplayableDataNode *dataNode);
		bool BeginModifyNode(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
		bool SaveNode(DisplayableDataNode *dataNode);
		void Refresh();
		void Clear();

	private:
		int idIndex;

		NodeModifiedDelegate *NodeModifiedHandler;
		CloseProjectDelegate *CloseProjectHandler;

	protected:
		SortedList *itemList;							//data array list
		DataEventRegistry *dataEventRegistry;
		unsigned int processID;
		unsigned int threadID;

		void DeleteDataNode(DisplayableDataNode *dataNode);
		void CreateDataNode(DisplayableDataNode *dataNode);
		void ModifyDataNode(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
		void OnNodeModified(DisplayableDataNode *Node, String *propertyName);
		void OnCloseProject();
	};
}