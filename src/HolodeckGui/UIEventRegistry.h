//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		UIEventRegistry.h
//
// DESCRIPTION: Contains definition for the singleton class UIEventRegistry
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
#include "DisplayableDataNode.h"
#include "ResourceTestDataNode.h"

#using <mscorlib.dll>
#using <system.windows.forms.dll>

using namespace System;
using namespace System::Windows::Forms;
using namespace UIData;

namespace HolodeckGui
{
	//process events
	public __delegate void PauseProcessDelegate(unsigned int processID);
	public __delegate void ProcessPausedDelegate(unsigned int processID);
	public __delegate void ResumeProcessDelegate(unsigned int processID);
	public __delegate void ProcessResumedDelegate(unsigned int processID);
	public __delegate void TerminateProcessDelegate(unsigned int processID);
	public __delegate void ProcessTerminatedDelegate(unsigned int processID);
	public __delegate void RestartProcessDelegate(unsigned int processID);
	public __delegate void RemoveProcessDelegate(unsigned int processID);
	public __delegate void ProcessRemovedDelegate(unsigned int processID);

	//focus events
	public __delegate void ProjectPaneFocusChangedDelegate(DisplayableDataNode *dataNode);
	public __delegate void FocusChangedDelegate(DisplayableDataNode *dataNode);
	
	//misc events
	public __delegate void PerThreadChangedDelegate(bool perThread);
	public __delegate void DoubleClickDelegate(DisplayableDataNode *dataNode);
	public __delegate void CloseProjectUIDelegate();
	public __delegate void URLClickedDelegate(String *url);
	public __delegate void CopyToClipboardDelegate();
	public __delegate void SelectLogItemDelegate(int pid, unsigned long filePos);

	// dialog events
	public __delegate DialogResult ShowDialogDelegate(Form *formToDisplay);
	public __delegate DialogResult ShowCommonDialogDelegate(CommonDialog *formToDisplay);
	
	//recorded session events
	public __delegate void RecordedSessionReplayStartDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionReplayStopDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionStartDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionStopDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionReplayStartedDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionReplayStoppedDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionStartedDelegate(DisplayableDataNode *dataNode);
	public __delegate void RecordedSessionStoppedDelegate(DisplayableDataNode *dataNode);
	
	//test wizard events
	public __delegate void StartCreateTestWizardDelegate(DisplayableDataNode *dataNode);
	public __delegate void StartModifyTestWizardDelegate(DisplayableDataNode *dataNode);
	
	//net corruption events
	public __delegate void StartNetworkCorruptionWizardDelegate(DisplayableDataNode *dataNode);
	public __delegate void StartModifyNetworkCorruptionWizardDelegate(DisplayableDataNode *dataNode);
	public __delegate void ShowNetworkCorruptionDetailsPaneDelegate();
	
	//file corruption events
	public __delegate void StartFileCorruptionWizardDelegate(DisplayableDataNode *dataNode);
	public __delegate void StartModifyFileCorruptionWizardDelegate(DisplayableDataNode *dataNode);
	public __delegate void ShowFileCorruptionDetailsPaneDelegate();
	
	//resource test events
	public __delegate void StartCreateResourceTestWizardDelegate(ResourceTestDataNode *dataNode);
	public __delegate void StartModifyResourceTestWizardDelegate(ResourceTestDataNode *dataNode);
	
	//*************************************************************************
	// class that all coordinates registering for and receiving events between
	// the UI's panes' UI classes.
	//*************************************************************************
	public __gc class UIEventRegistry
	{
	private:
		static UIEventRegistry *instance = NULL;

		UIEventRegistry();
		~UIEventRegistry();
	public:
		static UIEventRegistry *getInstance();

		//process events
		PauseProcessDelegate			*OnPauseProcess;
		ProcessPausedDelegate			*OnProcessPaused;
		ResumeProcessDelegate			*OnResumeProcess;
		ProcessResumedDelegate			*OnProcessResumed;
		TerminateProcessDelegate		*OnTerminateProcess;
		ProcessTerminatedDelegate		*OnProcessTerminated;
		RestartProcessDelegate			*OnRestartProcess;
		RemoveProcessDelegate			*OnRemoveProcess;	
		ProcessRemovedDelegate			*OnProcessRemoved;	

		//focus events
		ProjectPaneFocusChangedDelegate		*OnProjectPaneFocusChanged;	//fired whenever the selection in the project pane changes. Used by MainFrame
		FocusChangedDelegate				*OnFocusChanged;	//fired whenever ui focus changes.  Will be used by property pane.
		
		//misc events
		CopyToClipboardDelegate				*OnCopyToClipboard;
		PerThreadChangedDelegate			*OnPerThreadChanged;//fired when the per-thread setting changes in the ui.
		DoubleClickDelegate					*OnDoubleClick;		//fired when an element in a ui pane is double-clicked.
		CloseProjectUIDelegate				*OnCloseProjectUI;	//invoked when the project is closing.
		URLClickedDelegate					*OnURLClicked;		//invoked when a link label url is clicked
		ShowDialogDelegate					*OnShowDialog;		//invoked to display a dialog
		ShowCommonDialogDelegate			*OnShowCommonDialog;//invoked to display a common dialog
		SelectLogItemDelegate				*OnSelectLogItem;	//invoked to select a log item in the log pane
		
		//recorded session events
		RecordedSessionStartDelegate		*OnRecordedSessionStart;	//invoked when a session is recorded
		RecordedSessionStopDelegate			*OnRecordedSessionStop;		//invoked when a session recording is stopped
		RecordedSessionReplayStartDelegate	*OnRecordedSessionReplayStart;   //invoked when replaying a recorded session
		RecordedSessionReplayStopDelegate	*OnRecordedSessionReplayStop;  //invoked when a recorded session replay stops
		RecordedSessionStartedDelegate		*OnRecordedSessionStarted;	//invoked when a session is recorded
		RecordedSessionStoppedDelegate			*OnRecordedSessionStopped;		//invoked when a session recording is stopped
		RecordedSessionReplayStartedDelegate	*OnRecordedSessionReplayStarted;   //invoked when replaying a recorded session
		RecordedSessionReplayStoppedDelegate	*OnRecordedSessionReplayStopped;  //invoked when a recorded session replay stops
		
		//test wizard events
		StartCreateTestWizardDelegate		*OnStartCreateTestWizard;	// invoked when we need to start the create test wizard
		StartModifyTestWizardDelegate		*OnStartModifyTestWizard;	// invoked when we need to start the modify test wizard
		
		//network corruption wizard events
		StartNetworkCorruptionWizardDelegate	*OnStartNetworkCorruptionWizard;	// invoked when we need to start the network corruption wizard
		StartModifyNetworkCorruptionWizardDelegate	*OnStartModifyNetworkCorruptionWizard;	// invoked when we need to start the modify network corruption wizard
		ShowNetworkCorruptionDetailsPaneDelegate	*OnShowNetworkCorruptionDetailsPane;	// invoked when we need to show the network corruption details pane
		
		//file corruption wizard events
		StartFileCorruptionWizardDelegate	*OnStartFileCorruptionWizard;	// invoked when we need to start the file corruption wizard
		StartModifyFileCorruptionWizardDelegate	*OnStartModifyFileCorruptionWizard;	// invoked when we need to start the modify file corruption wizard
		ShowFileCorruptionDetailsPaneDelegate	*OnShowFileCorruptionDetailsPane;	// invoked when we need to show the file corruption details pane
		
		//resource test wizard events
		StartCreateResourceTestWizardDelegate		*OnStartCreateResourceTestWizard;	// invoked when we need to start the create resource test wizard
		StartModifyResourceTestWizardDelegate		*OnStartModifyResourceTestWizard;	// invoked when we need to start the modify resource test wizard
	};
}