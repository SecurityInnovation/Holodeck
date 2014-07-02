//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectPaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ProjectPaneDataArray.h"

namespace UIData
{

	//*************************************************************************
	// Method:		ProjectPaneDataArray
	// Description: Constructor for the ProjectPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectPaneDataArray::ProjectPaneDataArray()
	{
		TestDeletedHandler = new TestDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnTestDeleted += TestDeletedHandler;
		ReportDeletedHandler = new ReportDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnReportDeleted += ReportDeletedHandler;
		RecordedSessionDeletedHandler = new RecordedSessionDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnRecordedSessionDeleted += RecordedSessionDeletedHandler;
		LogPaneDeletedHandler = new LogPaneDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnLogPaneDeleted += LogPaneDeletedHandler;
		NetworkLogPaneDeletedHandler = new NetworkLogPaneDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnNetworkLogPaneDeleted += NetworkLogPaneDeletedHandler;
		LimitDeletedHandler = new LimitDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnLimitDeleted += LimitDeletedHandler;
		FaultDeletedHandler = new FaultDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnFaultDeleted += FaultDeletedHandler;
		ResourceTestDeletedHandler = new ResourceTestDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnResourceTestDeleted += ResourceTestDeletedHandler;
		NetworkCorruptionDeletedHandler = new NetworkCorruptionDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnNetworkCorruptionDeleted += NetworkCorruptionDeletedHandler;
		FileCorruptionDeletedHandler = new FileCorruptionDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnFileCorruptionDeleted += FileCorruptionDeletedHandler;
		SavedFileCorruptionDeletedHandler = new SavedFileCorruptionDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnSavedFileCorruptionDeleted += SavedFileCorruptionDeletedHandler;
		ExceptionDeletedHandler = new ExceptionDeletedDelegate(this, DeleteDataNode);
		dataEventRegistry->OnExceptionDeleted += ExceptionDeletedHandler;

		TestCreatedHandler = new TestCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnTestCreated += TestCreatedHandler;
		ReportCreatedHandler = new ReportCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnReportCreated += ReportCreatedHandler;
		RecordedSessionCreatedHandler = new RecordedSessionCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnRecordedSessionCreated += RecordedSessionCreatedHandler;
		LogPaneCreatedHandler = new LogPaneCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnLogPaneCreated += LogPaneCreatedHandler;
		NetworkLogPaneCreatedHandler = new NetworkLogPaneCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnNetworkLogPaneCreated += NetworkLogPaneCreatedHandler;
		LimitCreatedHandler = new LimitCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnLimitCreated += LimitCreatedHandler;
		FaultCreatedHandler = new FaultCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnFaultCreated += FaultCreatedHandler;
		ResourceTestCreatedHandler = new ResourceTestCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnResourceTestCreated += ResourceTestCreatedHandler;
		NetworkCorruptionCreatedHandler = new NetworkCorruptionCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnNetworkCorruptionCreated += NetworkCorruptionCreatedHandler;
		FileCorruptionCreatedHandler = new FileCorruptionCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnFileCorruptionCreated += FileCorruptionCreatedHandler;
		SavedFileCorruptionCreatedHandler = new SavedFileCorruptionCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnSavedFileCorruptionCreated += SavedFileCorruptionCreatedHandler;
		ExceptionCreatedHandler = new ExceptionCreatedDelegate(this, CreateDataNode);
		dataEventRegistry->OnExceptionCreated += ExceptionCreatedHandler;

		TestModifiedHandler = new TestModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnTestModified += TestModifiedHandler;
		ReportModifiedHandler = new ReportModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnReportModified += ReportModifiedHandler;
		RecordedSessionModifiedHandler = new RecordedSessionModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnRecordedSessionModified += RecordedSessionModifiedHandler;
		LogPaneModifiedHandler = new LogPaneModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnLogPaneModified += LogPaneModifiedHandler;
		NetworkLogPaneModifiedHandler = new NetworkLogPaneModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnNetworkLogPaneModified += NetworkLogPaneModifiedHandler;
		FaultModifiedHandler = new FaultModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnFaultModified += FaultModifiedHandler;
		ResourceTestModifiedHandler = new ResourceTestModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnResourceTestModified += ResourceTestModifiedHandler;
		NetworkCorruptionModifiedHandler = new NetworkCorruptionModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnNetworkCorruptionModified += NetworkCorruptionModifiedHandler;
		FileCorruptionModifiedHandler = new FileCorruptionModifiedDelegate(this, ModifyDataNode);
		dataEventRegistry->OnFileCorruptionModified += FileCorruptionModifiedHandler;
	}

	//*************************************************************************
	// Method:		~ProjectPaneDataArray
	// Description: Destructor for the ProjectPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectPaneDataArray::~ProjectPaneDataArray()
	{
		CleanUp();
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Cleans up all dataarray delegates
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPaneDataArray::CleanUp()
	{
		dataEventRegistry->OnTestDeleted -= TestDeletedHandler;
		dataEventRegistry->OnReportDeleted -= ReportDeletedHandler;
		dataEventRegistry->OnRecordedSessionDeleted -= RecordedSessionDeletedHandler;
		dataEventRegistry->OnLogPaneDeleted -= LogPaneDeletedHandler;
		dataEventRegistry->OnNetworkLogPaneDeleted -= NetworkLogPaneDeletedHandler;
		dataEventRegistry->OnLimitDeleted -= LimitDeletedHandler;
		dataEventRegistry->OnFaultDeleted -= FaultDeletedHandler;
		dataEventRegistry->OnNetworkCorruptionDeleted -= NetworkCorruptionDeletedHandler;
		dataEventRegistry->OnFileCorruptionDeleted -= FileCorruptionDeletedHandler;
		dataEventRegistry->OnSavedFileCorruptionDeleted -= SavedFileCorruptionDeletedHandler;
		dataEventRegistry->OnExceptionDeleted -= ExceptionDeletedHandler;

		dataEventRegistry->OnTestCreated -= TestCreatedHandler;
		dataEventRegistry->OnReportCreated -= ReportCreatedHandler;
		dataEventRegistry->OnRecordedSessionCreated -= RecordedSessionCreatedHandler;
		dataEventRegistry->OnLogPaneCreated -= LogPaneCreatedHandler;
		dataEventRegistry->OnNetworkLogPaneCreated -= NetworkLogPaneCreatedHandler;
		dataEventRegistry->OnLimitCreated -= LimitCreatedHandler;
		dataEventRegistry->OnFaultCreated -= FaultCreatedHandler;
		dataEventRegistry->OnNetworkCorruptionCreated -= NetworkCorruptionCreatedHandler;
		dataEventRegistry->OnFileCorruptionCreated -= FileCorruptionCreatedHandler;
		dataEventRegistry->OnSavedFileCorruptionCreated -= SavedFileCorruptionCreatedHandler;
		dataEventRegistry->OnExceptionCreated -= ExceptionCreatedHandler;

		dataEventRegistry->OnTestModified -= TestModifiedHandler;
		dataEventRegistry->OnReportModified -= ReportModifiedHandler;
		dataEventRegistry->OnRecordedSessionModified -= RecordedSessionModifiedHandler;
		dataEventRegistry->OnLogPaneModified -= LogPaneModifiedHandler;
		dataEventRegistry->OnNetworkLogPaneModified -= NetworkLogPaneModifiedHandler;
		dataEventRegistry->OnFaultModified -= FaultModifiedHandler;
		dataEventRegistry->OnNetworkCorruptionModified -= NetworkCorruptionModifiedHandler;
		dataEventRegistry->OnFileCorruptionModified -= FileCorruptionModifiedHandler;

		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		CreateDataNode
	// Description: Override create data node so that we ensure we are adding a 
	//	copy instead of a reference
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPaneDataArray::CreateDataNode(DisplayableDataNode *dataNode)
	{
		DisplayableDataNode *newNode = new DisplayableDataNode(dataNode);
		DisplayableDataArray::CreateDataNode(newNode);
	}

	//*************************************************************************
	// Method:		RemoveProcess
	// Description: Removes nodes belonging to the specified process from the pane
	//
	// Parameters:
	//	processID - the pid of the process to remove
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPaneDataArray::RemoveProcess(unsigned int processID)
	{
		ArrayList * nodesToRemove = new ArrayList();
		for (int i=0 ; i < this->itemList->Count; i++)
		{
			DisplayableDataNode * ppDataNode = dynamic_cast <DisplayableDataNode *> (this->itemList->GetByIndex(i));
			if (ppDataNode->processID == processID)
				nodesToRemove->Add (ppDataNode);
		}

		for (int j=0; j < nodesToRemove->Count; j++)
		{
			Object *args[] = __gc new Object*[1];
			DisplayableDataNode * dataNode = dynamic_cast <DisplayableDataNode *> (nodesToRemove->Item[j]);
			args[0] = dataNode;
			dataNode->DeleteDelegate->DynamicInvoke(args);
		}
	}
}