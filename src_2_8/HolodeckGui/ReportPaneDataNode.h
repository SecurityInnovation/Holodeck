//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReportPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class ReportPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 9 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"

#using <System.dll>
#using <mscorlib.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;

namespace UIData
{
	public __gc class ReportPaneDataNode : public DisplayableDataNode
	{
	public:
		[Browsable(false)]
		__property int get_ColumnCount() { return reportColumns->Count; }
		[Browsable(false)]
		__property ArrayList *get_Columns() { return reportColumns; }
		[Browsable(false)]
		__property int get_RowCount() { return reportRows->Count; }
		[Browsable(false)]
		__property ArrayList *get_Rows() { return reportRows; }
		[Browsable(false)]
		__property String *get_XMLPath() { return xmlPath; }

		ReportPaneDataNode(String *reportName, String *XMLPath, NodeType type, int pID, int tID);
		~ReportPaneDataNode();

		void addColumn(String *colName);
		void addRow(ArrayList *row);

	private:
		ArrayList *reportColumns;
		ArrayList *reportRows;
		String *xmlPath;

	};
}