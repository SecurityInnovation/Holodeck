//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GraphNode.h
//
// DESCRIPTION: Contains definition for the class GraphNode
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

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace SIControls
{
	public __gc class GraphNode
	{
	private:
		Object* id;
		ArrayList* items;
		Color backColor, foreColor;

	public:
		GraphNode();
		~GraphNode();

		__property Object* get_NodeId() { return id; }
		__property void set_NodeId(Object* value) { id = value; }

		__property ArrayList* get_DisplayItems() { return items; }

		__property Color get_BackColor() { return backColor; }
		__property void set_BackColor(Color value) { backColor = value; }

		__property Color get_ForeColor() { return foreColor; }
		__property void set_ForeColor(Color value) { foreColor = value; }
	};
}
