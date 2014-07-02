//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TreeListNodeCollection.h
//
// DESCRIPTION: Contains definition for the class TreeListNodeCollection
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
#using <system.windows.forms.dll>

#include <stdio.h>

using namespace System;
using namespace System::Collections;
using namespace System::Windows::Forms;

namespace SIControls
{
	//*********************************************************************
	// Forward declaration of the class to avoid compile errors
	//*********************************************************************
	public __gc class TreeListNode;

	//*********************************************************************
	// Contains a collection of TreeListNode items
	//*********************************************************************
	public __gc class TreeListNodeCollection : public CollectionBase
	{
	public:
		MouseEventHandler *MouseDown;
		EventHandler *NodesChanged;

		//TreeListNodeCollection();
		TreeListNodeCollection(TreeListNode *owner);
		~TreeListNodeCollection();

		__property TreeListNode *get_Owner() { return owner; }
		__property void set_Owner(TreeListNode *value) { owner = value; }

		__property int get_TotalCount() { return totalCount; }
		__property void set_TotalCount(int value) { totalCount = value; }

		__property TreeListNode *get_Item(int index);

		int IndexOf(TreeListNode *item);
	protected:
		void OnMouseDown(Object *sender, MouseEventArgs *e);
		void OnNodesChanged();
		void OnNodesChanged(Object *sender, EventArgs *e);

		TreeListNode *owner;
		int totalCount;
	};
}