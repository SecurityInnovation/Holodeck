//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SelectedTreeListNodeCollection.h
//
// DESCRIPTION: Contains definition for the class SelectedTreeListNodeCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <system.dll>

#include "TreeListNode.h"
#include <stdio.h>

using namespace System;
using namespace System::Collections;

namespace SIControls
{
	//*********************************************************************
	// Contains a set of selected TreeListNode objects
	//*********************************************************************
	public __gc class SelectedTreeListNodeCollection : public CollectionBase
	{
	public:
		SelectedTreeListNodeCollection();
		~SelectedTreeListNodeCollection();
		__property TreeListNode *get_Item(int index);
		__property void set_Item(int index, TreeListNode *value);

		int IndexOf(TreeListNode *item);
		int Add(TreeListNode *item);
		void AddRange(TreeListNode *items[]);
		void Remove(TreeListNode *item);
		void ClearAll();
	};
}