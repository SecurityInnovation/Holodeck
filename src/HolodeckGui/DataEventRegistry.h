//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DataEventRegistry.h
//
// DESCRIPTION: Contains definition for the singleton class DataEventRegistry
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>

#using <mscorlib.dll>

using namespace System;

namespace UIData
{

	public __gc class DisplayableDataNode;

	//data events - detailed comments are below in the event registry class.
	
	//project events
	public __delegate void SaveProjectDelegate();
	public __delegate void CloseProjectDelegate();
	public __delegate void RestartProjectDelegate(bool pauseOnStart);
	public __delegate void ProjectRestartedDelegate();

	//test events
	public __delegate void ToggleTestDelegate(DisplayableDataNode *dataNode);
	public __delegate void CreateTestDelegate(DisplayableDataNode *dataNode);
	public __delegate void TestCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteTestDelegate(DisplayableDataNode *dataNode);
	public __delegate void TestDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyTestDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void TestModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveTestDelegate(DisplayableDataNode *dataNode);

	//Resource based test events
	public __delegate void ToggleResourceTestDelegate(DisplayableDataNode *dataNode);
	public __delegate void CreateResourceTestDelegate(DisplayableDataNode *dataNode);
	public __delegate void ResourceTestCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteResourceTestDelegate(DisplayableDataNode *dataNode);
	public __delegate void ResourceTestDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyResourceTestDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void ResourceTestModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);

	//report events
	public __delegate void CreateReportDelegate(DisplayableDataNode *dataNode);
	public __delegate void ReportCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteReportDelegate(DisplayableDataNode *dataNode);
	public __delegate void ReportDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyReportDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void ReportModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveReportDelegate(DisplayableDataNode *dataNode);

	//recorded session events
	public __delegate void CreateRecordedSessionDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteRecordedSessionDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyRecordedSessionDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void RecordedSessionModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveRecordedSessionDelegate(DisplayableDataNode *dataNode);

	//log events
	public __delegate void CreateLogPaneDelegate(DisplayableDataNode *dataNode);
	public __delegate void LogPaneCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteLogPaneDelegate(DisplayableDataNode *dataNode);
	public __delegate void LogPaneDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyLogPaneDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void LogPaneModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveLogPaneDelegate(DisplayableDataNode *dataNode);

	// network log events
	public __delegate void CreateNetworkLogPaneDelegate(DisplayableDataNode *dataNode);
	public __delegate void NetworkLogPaneCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteNetworkLogPaneDelegate(DisplayableDataNode *dataNode);
	public __delegate void NetworkLogPaneDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyNetworkLogPaneDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void NetworkLogPaneModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveNetworkLogPaneDelegate(DisplayableDataNode *dataNode);

	//limit events
	public __delegate void CreateLimitDelegate(DisplayableDataNode *dataNode);
	public __delegate void LimitCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteLimitDelegate(DisplayableDataNode *dataNode);
	public __delegate void LimitDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyLimitDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void LimitModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveLimitDelegate(DisplayableDataNode *dataNode);

	//resource events
	public __delegate void ResourceCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ResourceDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ResourceModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveResourcePaneDelegate();

	//fault events
	public __delegate void CreateFaultDelegate(DisplayableDataNode *dataNode);
	public __delegate void FaultCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteFaultDelegate(DisplayableDataNode *dataNode);
	public __delegate void FaultDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyFaultDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void FaultModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveFaultDelegate(DisplayableDataNode *dataNode);

	//network corruption fault events
	public __delegate void ToggleNetworkCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void CreateNetworkCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void NetworkCorruptionCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteNetworkCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void NetworkCorruptionDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyNetworkCorruptionDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void NetworkCorruptionModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveNetworkCorruptionDelegate(DisplayableDataNode *dataNode);

	// file corruption fault events
	public __delegate void ToggleFileCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void CreateFileCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void FileCorruptionCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteFileCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void FileCorruptionDeletedDelegate(DisplayableDataNode *dataNode);
	public __delegate void ModifyFileCorruptionDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void FileCorruptionModifiedDelegate(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName);
	public __delegate void SaveFileCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void CreateSavedFileCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void SavedFileCorruptionCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteSavedFileCorruptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void SavedFileCorruptionDeletedDelegate(DisplayableDataNode *dataNode);

	// exception events
	public __delegate void CreateExceptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void ExceptionCreatedDelegate(DisplayableDataNode *dataNode);
	public __delegate void DeleteExceptionDelegate(DisplayableDataNode *dataNode);
	public __delegate void ExceptionDeletedDelegate(DisplayableDataNode *dataNode);

	// test generation events
	public __delegate void StopStressTestGenerationDelegate();
	public __delegate void StopCodeCoverageTestGenerationDelegate();
	public __delegate void StressTestGenerationStoppedDelegate();
	public __delegate void CodeCoverageTestGenerationStoppedDelegate();

	//*************************************************************************
	// class that all coordinates registering for and receiving events between
	// the UI's panes' data classes.
	//*************************************************************************
	public __gc class DataEventRegistry
	{
	private:
		static DataEventRegistry *instance = NULL;

		DataEventRegistry();
		~DataEventRegistry();
	public:
		static DataEventRegistry *getInstance();

		//project events
		SaveProjectDelegate				*OnSaveProject;		//invoked by any ui data class that wants to save a project
		CloseProjectDelegate			*OnCloseProject;	//invoked when the project is closing.
		RestartProjectDelegate			*OnRestartProject;	//invoked to restart the project
		ProjectRestartedDelegate		*OnProjectRestarted;//invoked when the project has been restarted

		//test events
		ToggleTestDelegate				*OnToggleTest;		//invoked by any ui data class that wants to change the state of the test (on\off)
		CreateTestDelegate				*OnCreateTest;		//invoked by any ui data class that wants to create a test
		TestCreatedDelegate				*OnTestCreated;		//invoked by test pane data class when the test has been succesfully created
		DeleteTestDelegate				*OnDeleteTest;		//invoked by any ui data class that wants to delete a test
		TestDeletedDelegate				*OnTestDeleted;		//invoked by test pane data class when the test has been succesfully deleted
		ModifyTestDelegate				*OnModifyTest;		//invoked by any ui data class that wants to modify a test
		TestModifiedDelegate			*OnTestModified;	//invoked by test pane data class when the test has been succesfully modified
		SaveTestDelegate				*OnSaveTest;		//invoked by any ui data class that wants to save a test

		//resource test events
		ToggleResourceTestDelegate		*OnToggleResourceTest;		//invoked by any ui data class that wants to change the state of the test (on\off)
		CreateResourceTestDelegate		*OnCreateResourceTest;		//invoked by any ui data class that wants to create a test
		ResourceTestCreatedDelegate		*OnResourceTestCreated;		//invoked by test pane data class when the test has been succesfully created
		DeleteResourceTestDelegate		*OnDeleteResourceTest;		//invoked by any ui data class that wants to delete a test
		ResourceTestDeletedDelegate		*OnResourceTestDeleted;		//invoked by test pane data class when the test has been succesfully deleted
		ModifyResourceTestDelegate		*OnModifyResourceTest;		//invoked by any ui data class that wants to modify a test
		ResourceTestModifiedDelegate	*OnResourceTestModified;	//invoked by test pane data class when the test has been succesfully modified

		//resource  events
		ResourceCreatedDelegate			*OnResourceCreated;			//invoked by resource pane when a resource has been created
		ResourceDeletedDelegate			*OnResourceDeleted;		//invoked by resource pane when a resource has been deleted
		ResourceModifiedDelegate		*OnResourceModified;	//invoked by resource pane when a resource has been modified
		SaveResourcePaneDelegate		*OnSaveResourcePane;	//invokes when the resource pane should be exported to a file

		//report events
		CreateReportDelegate			*OnCreateReport;	//invoked by any ui data class that wants to create a report
		ReportCreatedDelegate			*OnReportCreated;	//invoked by report pane data class when the report has been succesfully created
		DeleteReportDelegate			*OnDeleteReport;	//invoked by any ui data class that wants to delete a report
		ReportDeletedDelegate			*OnReportDeleted;	//invoked by report pane data class when the report has been succesfully deleted
		ModifyReportDelegate			*OnModifyReport;	//invoked by any ui data class that wants to modify a report
		ReportModifiedDelegate			*OnReportModified;	//invoked by report pane data class when the report has been succesfully modified
		SaveReportDelegate				*OnSaveReport;		//invoked by any ui data class that wants to save a report

		//recorded session events
		CreateRecordedSessionDelegate	*OnCreateRecordedSession;	//invoked by any ui data class that wants to create a recorded session
		RecordedSessionCreatedDelegate	*OnRecordedSessionCreated;	//invoked by recorded session pane data class when the recorded session has been succesfully created
		DeleteRecordedSessionDelegate	*OnDeleteRecordedSession;	//invoked by any ui data class that wants to delete a recorded session
		RecordedSessionDeletedDelegate	*OnRecordedSessionDeleted;	//invoked by recorded session pane data class when the recorded session has been succesfully deleted
		ModifyRecordedSessionDelegate	*OnModifyRecordedSession;	//invoked by any ui data class that wants to modify a recorded session
		RecordedSessionModifiedDelegate	*OnRecordedSessionModified;	//invoked by recorded session pane data class when the recorded session has been succesfully modified
		SaveRecordedSessionDelegate		*OnSaveRecordedSession;		//invoked by any ui data class that wants to save a recorded session

		//log events
		CreateLogPaneDelegate			*OnCreateLogPane;	//invoked by any ui data class that wants to create a log
		LogPaneCreatedDelegate			*OnLogPaneCreated;	//invoked by log pane data class when the log has been succesfully created
		DeleteLogPaneDelegate			*OnDeleteLogPane;	//invoked by any ui data class that wants to delete a log
		LogPaneDeletedDelegate			*OnLogPaneDeleted;	//invoked by log pane data class when the log has been succesfully deleted
		ModifyLogPaneDelegate			*OnModifyLogPane;	//invoked by any ui data class that wants to modify a log
		LogPaneModifiedDelegate			*OnLogPaneModified;	//invoked by log pane data class when the log has been succesfully modified
		SaveLogPaneDelegate				*OnSaveLogPane;		//invoked by any ui data class that wants to save a log

		//network log events
		CreateNetworkLogPaneDelegate	*OnCreateNetworkLogPane;	//invoked by any ui data class that wants to create a network log
		NetworkLogPaneCreatedDelegate	*OnNetworkLogPaneCreated;	//invoked by network log pane data class when the log has been succesfully created
		DeleteNetworkLogPaneDelegate	*OnDeleteNetworkLogPane;	//invoked by any ui data class that wants to delete a network log
		NetworkLogPaneDeletedDelegate	*OnNetworkLogPaneDeleted;	//invoked by network log pane data class when the log has been succesfully deleted
		ModifyNetworkLogPaneDelegate	*OnModifyNetworkLogPane;	//invoked by any ui data class that wants to modify a network log
		NetworkLogPaneModifiedDelegate	*OnNetworkLogPaneModified;	//invoked by network log pane data class when the log has been succesfully modified
		SaveNetworkLogPaneDelegate		*OnSaveNetworkLogPane;		//invoked by any ui data class that wants to save a network log

		//limit events
		CreateLimitDelegate				*OnCreateLimit;		//invoked by any ui data class that wants to create a limit
		LimitCreatedDelegate			*OnLimitCreated;	//invoked by limit pane data class when the limit has been succesfully created
		DeleteLimitDelegate				*OnDeleteLimit;		//invoked by any ui data class that wants to delete a limit
		LimitDeletedDelegate			*OnLimitDeleted;	//invoked by limit pane data class when the limit has been succesfully deleted
		ModifyLimitDelegate				*OnModifyLimit;		//invoked by any ui data class that wants to modify a limit
		LimitModifiedDelegate			*OnLimitModified;	//invoked by limit pane data class when the limit has been succesfully modified
		SaveLimitDelegate				*OnSaveLimit;		//invoked by any ui data class that wants to save a limit

		//fault events
		CreateFaultDelegate				*OnCreateFault;		//invoked by any ui data class that wants to create a fault
		FaultCreatedDelegate			*OnFaultCreated;	//invoked by fault pane data class when the fault has been succesfully created
		DeleteFaultDelegate				*OnDeleteFault;		//invoked by any ui data class that wants to delete a fault
		FaultDeletedDelegate			*OnFaultDeleted;	//invoked by fault pane data class when the fault has been succesfully deleted
		ModifyFaultDelegate				*OnModifyFault;		//invoked by any ui data class that wants to modify a fault
		FaultModifiedDelegate			*OnFaultModified;	//invoked by fault pane data class when the fault has been succesfully modified
		SaveFaultDelegate				*OnSaveFault;		//invoked by any ui data class that wants to save a fault

		//network corruption events
		ToggleNetworkCorruptionDelegate		*OnToggleNetworkCorruption;		//invoked by any ui data class that wants to change the state of the test (on\off)
		CreateNetworkCorruptionDelegate		*OnCreateNetworkCorruption;		//invoked by any ui data class that wants to create a network corruption fault
		NetworkCorruptionCreatedDelegate	*OnNetworkCorruptionCreated;	//invoked by network corruption fault pane data class when the fault has been succesfully created
		DeleteNetworkCorruptionDelegate		*OnDeleteNetworkCorruption;		//invoked by any ui data class that wants to delete a network corruption fault
		NetworkCorruptionDeletedDelegate	*OnNetworkCorruptionDeleted;	//invoked by network corruption fault pane data class when the fault has been succesfully deleted
		ModifyNetworkCorruptionDelegate		*OnModifyNetworkCorruption;		//invoked by any ui data class that wants to modify a network corruption fault
		NetworkCorruptionModifiedDelegate	*OnNetworkCorruptionModified;	//invoked by network corruption fault pane data class when the fault has been succesfully modified
		SaveNetworkCorruptionDelegate		*OnSaveNetworkCorruption;		//invoked by any ui data class that wants to save a network corruption fault

		// file corruption fault events
		ToggleFileCorruptionDelegate	*OnToggleFileCorruption;				//invoked by any ui data class that wants to change the state of the test (on\off)
		CreateFileCorruptionDelegate	*OnCreateFileCorruption;				//invoked by any ui data class that wants to create a file corruption fault
		FileCorruptionCreatedDelegate	*OnFileCorruptionCreated;				//invoked by file corruption fault pane data class when the fault has been succesfully created
		DeleteFileCorruptionDelegate	*OnDeleteFileCorruption;				//invoked by any ui data class that wants to delete a file corruption fault
		FileCorruptionDeletedDelegate	*OnFileCorruptionDeleted;				//invoked by file corruption fault pane data class when the fault has been succesfully deleted
		ModifyFileCorruptionDelegate	*OnModifyFileCorruption;				//invoked by any ui data class that wants to modify a file corruption fault
		FileCorruptionModifiedDelegate	*OnFileCorruptionModified;				//invoked by file corruption fault pane data class when the fault has been succesfully modified
		SaveFileCorruptionDelegate		*OnSaveFileCorruption;					//invoked by any ui data class that wants to save a file corruption fault
		CreateSavedFileCorruptionDelegate	*OnCreateSavedFileCorruption;		//invoked by any ui data class that wants to create a saved corrupted file
		SavedFileCorruptionCreatedDelegate	*OnSavedFileCorruptionCreated;		//invoked by saved file corruption pane data class when the saved file has been succesfully created
		DeleteSavedFileCorruptionDelegate	*OnDeleteSavedFileCorruption;		//invoked by any ui data class that wants to delete a svaed corrupted file
		SavedFileCorruptionDeletedDelegate	*OnSavedFileCorruptionDeleted;		//invoked by saved file corruption pane data class when the saved file has been succesfully deleted

		// exception events
		CreateExceptionDelegate			*OnCreateException;				//invoked when a new unhandled exception is about to be added
		ExceptionCreatedDelegate		*OnExceptionCreated;			//invoked by exception pane data class when the exception has been succesfully created
		DeleteExceptionDelegate			*OnDeleteException;				//invoked by any ui data class that wants to delete an exception
		ExceptionDeletedDelegate		*OnExceptionDeleted;			//invoked by exception pane data class when the exception has been succesfully deleted
		
		// test generation events
		StopStressTestGenerationDelegate			*OnStopStressTestGeneration;				//invoked when a piece of UI wants to stop test generation
		StopCodeCoverageTestGenerationDelegate		*OnStopCodeCoverageTestGeneration;			//invoked when a piece of UI wants to stop test generation
		StressTestGenerationStoppedDelegate			*OnStressTestGenerationStopped;				//invoked when test generation has stopped
		CodeCoverageTestGenerationStoppedDelegate	*OnCodeCoverageTestGenerationStopped;		//invoked when test generation has stopped
	};
}