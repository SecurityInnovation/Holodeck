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
#include "ReportPaneDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ReportPaneDataNode
	// Description: Constructor for the ReportPaneDataNode class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	ReportPaneDataNode::ReportPaneDataNode(String *reportName, String *XMLPath, NodeType type, int pID, int tID)
	{
		reportColumns = new ArrayList();
		reportRows = new ArrayList();

		InternalConstructor(reportName, type, pID, tID);
		xmlPath = XMLPath;
	}

	//*************************************************************************
	// Method:		~ReportPaneDataNode
	// Description: Destructor for the ReportPaneDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ReportPaneDataNode::~ReportPaneDataNode()
	{
	}

	//*************************************************************************
	// Method:		addColumn
	// Description: Adds a column to the column list
	//
	// Parameters:
	//	colName - name of the column
	//
	// Return Value: None
	//*************************************************************************
	void ReportPaneDataNode::addColumn(String *colName)
	{
		reportColumns->Add(colName);
	}

	//*************************************************************************
	// Method:		addRow
	// Description: Adds a row to the row list
	//
	// Parameters:
	//	row - array that contains value for all columns in this row
	//
	// Return Value: None
	//*************************************************************************
	void ReportPaneDataNode::addRow(ArrayList *row)
	{
		reportRows->Add(row);
	}
}