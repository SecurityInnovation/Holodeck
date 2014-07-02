//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DisplayableDataNode.cpp
//
// DESCRIPTION: Contains implementation for the class DisplayableDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include "displayabledatanode.h"
#using <mscorlib.dll>

namespace UIData
{
	//*************************************************************************
	// Method:		DisplayableDataNode
	// Description: Constructor for the DisplayableDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DisplayableDataNode::DisplayableDataNode()
	{
		nodeID = "";
		perThereadMode = false;
	}


	//*************************************************************************
	// Method:		DisplayableDataNode
	// Description: Constructor for the DisplayableDataNode class
	//
	// Parameters:
	//	dataNode - node to copy
	//
	// Return Value: None
	//*************************************************************************
	DisplayableDataNode::DisplayableDataNode(DisplayableDataNode *dataNode)
	{
		InternalConstructor(dataNode);
	}

	//*************************************************************************
	// Method:		DisplayableDataNode
	// Description: Constructor for the DisplayableDataNode class
	//
	// Parameters:
	//	Name - name of the node
	//	Type - type of the node
	//	pID - process ID for the node
	//	tID - thread ID for the node
	//
	// Return Value: None
	//*************************************************************************
	DisplayableDataNode::DisplayableDataNode(String *Name, NodeType Type, int pID, int tID)
	{
		InternalConstructor(Name, Type, pID, tID);
	}

	//*************************************************************************
	// Method:		InternalConstructor
	// Description: Internal constructor for the DisplayableDataNode class
	//
	// Parameters:
	//	Name - name of the node
	//	Type - type of the node
	//	pID - process ID for the node
	//	tID - thread ID for the node
	//
	// Return Value: None
	//*************************************************************************
	void DisplayableDataNode::InternalConstructor(String *Name, NodeType Type, int pID, int tID)
	{
		dataEventRegistry =  dataEventRegistry->getInstance();
		nodeName = Name;
		nodeType = Type;
		processID = pID;
		threadID = tID;
		nodeID = "";
		perThereadMode = false;

		UpdateDelegates();

	}

	//*************************************************************************
	// Method:		UpdateDelegates
	// Description: Ensures the delegates are all up to date
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void DisplayableDataNode::UpdateDelegates()
	{
		switch (nodeType)
		{
			case UIData::Process:
				saveDelegate = dataEventRegistry->OnSaveProject;
				break;
			case UIData::Test:
				toggleDelegate = dataEventRegistry->OnToggleTest;
				createDelegate = dataEventRegistry->OnCreateTest;
				deleteDelegate = dataEventRegistry->OnDeleteTest;
				modifyDelegate = dataEventRegistry->OnModifyTest;
				createdDelegate = dataEventRegistry->OnTestCreated;
				deletedDelegate = dataEventRegistry->OnTestDeleted;
				modifiedDelegate = dataEventRegistry->OnTestModified;
				break;
			case UIData::ResourceFault:
				toggleDelegate = dataEventRegistry->OnToggleResourceTest;
				createDelegate = dataEventRegistry->OnCreateResourceTest;
				deleteDelegate = dataEventRegistry->OnDeleteResourceTest;
				modifyDelegate = dataEventRegistry->OnModifyResourceTest;
				createdDelegate = dataEventRegistry->OnResourceTestCreated;
				deletedDelegate = dataEventRegistry->OnResourceTestDeleted;
				modifiedDelegate = dataEventRegistry->OnResourceTestModified;
				break;
			case UIData::Report:
				createDelegate = dataEventRegistry->OnCreateReport;
				deleteDelegate = dataEventRegistry->OnDeleteReport;
				modifyDelegate = dataEventRegistry->OnModifyReport;
				saveDelegate = dataEventRegistry->OnSaveReport;
				createdDelegate = dataEventRegistry->OnReportCreated;
				deletedDelegate = dataEventRegistry->OnReportDeleted;
				modifiedDelegate = dataEventRegistry->OnReportModified;
				break;
			case UIData::RecordedSession:
				createDelegate = dataEventRegistry->OnCreateRecordedSession;
				deleteDelegate = dataEventRegistry->OnDeleteRecordedSession;
				modifyDelegate = dataEventRegistry->OnModifyRecordedSession;
				saveDelegate = dataEventRegistry->OnSaveRecordedSession;
				createdDelegate = dataEventRegistry->OnRecordedSessionCreated;
				deletedDelegate = dataEventRegistry->OnRecordedSessionDeleted;
				modifiedDelegate = dataEventRegistry->OnRecordedSessionModified;
				break;
			case UIData::ActiveLogPane:
			case UIData::SavedLogPane:
				createDelegate = dataEventRegistry->OnCreateLogPane;
				deleteDelegate = dataEventRegistry->OnDeleteLogPane;
				saveDelegate = dataEventRegistry->OnSaveLogPane;
				createdDelegate = dataEventRegistry->OnLogPaneCreated;
				deletedDelegate = dataEventRegistry->OnLogPaneDeleted;
				break;
			case UIData::ActiveNetworkLogPane:
			case UIData::SavedNetworkLogPane:
				deleteDelegate = dataEventRegistry->OnDeleteNetworkLogPane;
				saveDelegate = dataEventRegistry->OnSaveNetworkLogPane;
				deletedDelegate = dataEventRegistry->OnNetworkLogPaneDeleted;
				break;
			case UIData::MemoryLimit:
			case UIData::DiskLimit:
			case UIData::NetworkUpLimit:
			case UIData::NetworkDownLimit:
				createDelegate = dataEventRegistry->OnCreateLimit;
				deleteDelegate = dataEventRegistry->OnDeleteLimit;
				modifyDelegate = dataEventRegistry->OnModifyLimit;
				createdDelegate = dataEventRegistry->OnLimitCreated;
				deletedDelegate = dataEventRegistry->OnLimitDeleted;
				modifiedDelegate = dataEventRegistry->OnLimitModified;
				break;
			case UIData::MemoryResource:
			case UIData::DiskResource:
			case UIData::NetworkResource:
			case UIData::File:
			case UIData::RegistryEntry:
				createdDelegate = dataEventRegistry->OnResourceCreated;
				deletedDelegate = dataEventRegistry->OnResourceDeleted;
				modifiedDelegate = dataEventRegistry->OnResourceModified;
				break;
			case UIData::MemoryFault:
			case UIData::DiskFault:
			case UIData::NetworkFault:
			case UIData::RegistryFault:
			case UIData::ProcessFault:
			case UIData::CustomFault:
				createDelegate = dataEventRegistry->OnCreateFault;
				deleteDelegate = dataEventRegistry->OnDeleteFault;
				createdDelegate = dataEventRegistry->OnFaultCreated;
				deletedDelegate = dataEventRegistry->OnFaultDeleted;
				break;
			case UIData::NetworkCorruptionFault:
				toggleDelegate = dataEventRegistry->OnToggleNetworkCorruption;
				createDelegate = dataEventRegistry->OnCreateNetworkCorruption;
				deleteDelegate = dataEventRegistry->OnDeleteNetworkCorruption;
				modifyDelegate = dataEventRegistry->OnModifyNetworkCorruption;
				createdDelegate = dataEventRegistry->OnNetworkCorruptionCreated;
				deletedDelegate = dataEventRegistry->OnNetworkCorruptionDeleted;
				modifiedDelegate = dataEventRegistry->OnNetworkCorruptionModified;
				break;
			case UIData::FileCorruptionFault:
				toggleDelegate = dataEventRegistry->OnToggleFileCorruption;
				createDelegate = dataEventRegistry->OnCreateFileCorruption;
				deleteDelegate = dataEventRegistry->OnDeleteFileCorruption;
				modifyDelegate = dataEventRegistry->OnModifyFileCorruption;
				createdDelegate = dataEventRegistry->OnFileCorruptionCreated;
				deletedDelegate = dataEventRegistry->OnFileCorruptionDeleted;
				modifiedDelegate = dataEventRegistry->OnFileCorruptionModified;
				break;
			case UIData::CorruptedFile:
			case UIData::SavedCorruptedFile:
				createDelegate = dataEventRegistry->OnCreateSavedFileCorruption;
				deleteDelegate = dataEventRegistry->OnDeleteSavedFileCorruption;
				createdDelegate = dataEventRegistry->OnSavedFileCorruptionCreated;
				deletedDelegate = dataEventRegistry->OnSavedFileCorruptionDeleted;
				break;
			case UIData::ExceptionInfo:
				createDelegate = dataEventRegistry->OnCreateException;
				deleteDelegate = dataEventRegistry->OnDeleteException;
				createdDelegate = dataEventRegistry->OnExceptionCreated;
				deletedDelegate = dataEventRegistry->OnExceptionDeleted;
				break;
		}
	}

	//*************************************************************************
	// Method:		InternalConstructor
	// Description: Internal copy constructor for the DisplayableDataNode class
	//
	// Parameters:
	//	Name - name of the node
	//	Type - type of the node
	//	pID - process ID for the node
	//	tID - thread ID for the node
	//
	// Return Value: None
	//*************************************************************************
	void DisplayableDataNode::InternalConstructor(DisplayableDataNode *dataNode)
	{
		dataEventRegistry =  dataEventRegistry->getInstance();
		this->toggleDelegate = dataNode->toggleDelegate;
		this->createdDelegate = dataNode->createdDelegate;
		this->createDelegate = dataNode->createDelegate;
		this->dataEventRegistry = dataNode->dataEventRegistry;
		this->deletedDelegate = dataNode->deletedDelegate;
		this->deleteDelegate = dataNode->deleteDelegate;
		this->modifiedDelegate = dataNode->modifiedDelegate;
		this->modifyDelegate = dataNode->modifyDelegate;
		this->nodeID = dataNode->nodeID;
		this->nodeName = dataNode->nodeName;
		this->nodeProcessID = dataNode->nodeProcessID;
		this->nodeThreadID = dataNode->nodeThreadID;
		this->nodeType = dataNode->nodeType;
		this->saveDelegate = dataNode->saveDelegate;
		this->uiElement = dataNode->uiElement;
		this->PerThreadMode = dataNode->PerThreadMode;
		this->DataArray = dataNode->DataArray;
	}

	//*************************************************************************
	// Method:		~DisplayableDataNode
	// Description: Destructor for the DisplayableDataNode class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DisplayableDataNode::~DisplayableDataNode()
	{
	}
}
