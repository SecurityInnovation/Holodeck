//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GraphNodeCollection.h
//
// DESCRIPTION: Contains definition for the class GraphNodeCollection
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 23 Jul 2004		 R. Wagner	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>

#include "GraphNode.h"

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Reflection;
using namespace System::Collections::Specialized;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	public __delegate void GraphNodeRemovedDelegate(GraphNode* node);
	public __delegate void GraphNodeAddedDelegate(GraphNode* node);

	public __gc class GraphNodeCollection
	{
	private:
		Hashtable* nodeTable;

	public:
		GraphNodeRemovedDelegate* OnGraphNodeRemoved;
		GraphNodeAddedDelegate* OnGraphNodeAdded;

		GraphNodeCollection();
		~GraphNodeCollection();

		__property GraphNode* get_Item(Object* id);
		__property void set_Item(Object* id, GraphNode* node);
		__property ICollection* get_NodeList();

		void Add(GraphNode* node);
		void Remove(GraphNode* node);
	};
}
