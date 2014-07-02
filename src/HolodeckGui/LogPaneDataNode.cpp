//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class LogPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 21 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#include "LogPaneDataNode.h"

namespace UIData
{
	//*************************************************************************
	// Method:		LogPaneDataNode
	// Description: Constructor for the LogPaneDataNode class
	//
	// Parameters:
	//	item - The log item that this data node encapsulates
	//
	// Return Value: None
	//*************************************************************************
	LogPaneDataNode::LogPaneDataNode(LogItem *item)
	{
		this->logItem = item;
		this->Type = Logs;

		errorCodeDB = ErrorCodeDB::GetInstance();
		if (!errorCodeDB->IsLoaded)
			errorCodeDB->LoadDatabase(RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath());
	}

	//*************************************************************************
	// Method:		~LogPaneDataNode
	// Description: Destructor for the LogPaneDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogPaneDataNode::~LogPaneDataNode()
	{
	}

	//*************************************************************************
	// Method:		getParamValue
	// Description: Gets the string value of the requested parameter, if valid
	//
	// Parameters:
	//	index		- the index of the param.
	//
	// Return Value: (string) if valid: value of param, else NULL
	//*************************************************************************
	String * LogPaneDataNode::getParamValue (int index)
	{
		if (logItem->Parameters->Count - 1 >= index)
			return logItem->Parameters->Item[index]->ToString();
		else
			return NULL;
	}

}