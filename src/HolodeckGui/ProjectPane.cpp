//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectPane.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#define _WIN32_WINNT 0x400

#include "ProjectPane.h"
#include "UserNotification.h"
#include "RegistrationMgr.h"

using namespace HolodeckRegistration;
using namespace System::Threading;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ProjectPane
	// Description: Constructor for the ProjectPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectPane::ProjectPane(ProjectPaneDataArray *paneData)
	{
		processMutexList = new SortedList();
		threadMutexList = new SortedList();
		terminatedProcessTable = new Hashtable();
		pausedProcessTable = new Hashtable();

		projectTreeView = new TreeView();
		contextMenu = new System::Windows::Forms::ContextMenu();
		projectTreeView->ContextMenu = contextMenu;
		projectTreeView->HideSelection = false;

		this->Controls->Add(projectTreeView);
		projectTreeView->LabelEdit = true;
		projectPaneData = paneData;

		Layout += new LayoutEventHandler(this, LayoutChanged);
		projectTreeView->MouseUp += new MouseEventHandler(this, OnMouseUp);
		projectTreeView->KeyDown += new KeyEventHandler(this, OnKeyDown);
		projectTreeView->AfterSelect += new TreeViewEventHandler(this, OnSelect);
		projectTreeView->BeforeLabelEdit += new NodeLabelEditEventHandler(this, OnBeforeEdit);
		projectTreeView->AfterLabelEdit += new NodeLabelEditEventHandler(this, OnAfterEdit);
		projectTreeView->DoubleClick += new EventHandler(this, OnDoubleClick);
		
		//hook up to data class
		ItemAddedHandler = new ItemAddedDelegate(this, OnItemAdded);	
		projectPaneData->onItemAdded += ItemAddedHandler;

		ItemDeletedHandler = new ItemDeletedDelegate(this, OnItemDeleted);	
		projectPaneData->onItemDeleted += ItemDeletedHandler;

		ItemModifiedHandler = new ItemModifiedDelegate(this, OnItemModified);	
		projectPaneData->onItemModified += ItemModifiedHandler;

		//hook uievent registry delegates
		PerThreadChangedHandler = new PerThreadChangedDelegate(this, OnPerThreadChanged);
		uiEventRegistry->OnPerThreadChanged += PerThreadChangedHandler;
		CloseProjectUIHandler = new CloseProjectUIDelegate(this, OnCloseProjectUI);
		uiEventRegistry->OnCloseProjectUI += CloseProjectUIHandler;
		FocusChangedHandler = new FocusChangedDelegate(this, OnFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;
		RecordedSessionReplayStartedHandler = new RecordedSessionReplayStartedDelegate(this, RecordedSessionReplayStarted);
		uiEventRegistry->OnRecordedSessionReplayStarted += RecordedSessionReplayStartedHandler;
		RecordedSessionReplayStoppedHandler = new RecordedSessionReplayStoppedDelegate(this, RecordedSessionReplayStopped);
		uiEventRegistry->OnRecordedSessionReplayStopped += RecordedSessionReplayStoppedHandler;
		RecordedSessionStartedHandler = new RecordedSessionStartedDelegate(this, RecordedSessionStarted);
		uiEventRegistry->OnRecordedSessionStarted += RecordedSessionStartedHandler;
		RecordedSessionStoppedHandler = new RecordedSessionStoppedDelegate(this, RecordedSessionStopped);
		uiEventRegistry->OnRecordedSessionStopped += RecordedSessionStoppedHandler;
		ProcessRemovedHandler = new ProcessRemovedDelegate(this, onProcessRemoved);
		uiEventRegistry->OnProcessRemoved += ProcessRemovedHandler;
		
		//process events
		uiEventRegistry->OnProcessTerminated += new ProcessTerminatedDelegate(this, OnProcessTerminated);
		uiEventRegistry->OnProcessPaused += new ProcessPausedDelegate(this, OnProcessPaused);
		uiEventRegistry->OnProcessResumed += new ProcessResumedDelegate(this, OnProcessResumed);

		//load images
		projectTreeView->ImageList = ImageListManager::getInstance()->NodeTypeImageList;

		addNodeDelegate = new AddNodeDelegate(this, onAddNode);
		deleteNodeDelegate = new DeleteNodeDelegate(this, onDeleteNode);

		//menu items
		pauseResumeMenuItem = new MenuItem("Pause/Resume Application", new EventHandler(this, PauseProcess));
		stopMenuItem = new MenuItem("Stop Application", new EventHandler(this, TerminateProcess));
		replayMenuItemText = "Replay";
		recordMenuItemText = "Create Recorded Session";
		replayMenuItemEnabled = true;
		recordMenuItemEnabled = true;
		
		firstUse = true;
		ignoreFocusChange = false;
	}

	//*************************************************************************
	// Method:		~ProjectPane
	// Description: Destructor for the ProjectPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectPane::~ProjectPane()
	{
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Called by destructor and any other code that wants to release resources
	//
	// Parameters:
	//	disposing - flag to specify managed or unmanaged resource release.
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::Dispose(bool disposing)
	{
		projectPaneData->onItemAdded -= ItemAddedHandler;
		projectPaneData->onItemDeleted -= ItemDeletedHandler;
		projectPaneData->onItemModified -= ItemModifiedHandler;
	}

	//*************************************************************************
	// Method:		LayoutChanged
	// Description: Event handler for when the layout of the project pane changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::LayoutChanged(Object *sender, LayoutEventArgs *e)
	{
		projectTreeView->Size = this->Size;
	}

	//*************************************************************************
	// Method:		OnMouseUp
	// Description: Event handler for when the mouse button is released on the treeview
	//
	// Parameters:
	//	sender - object that sent the event
	//	MouseEventArgs - event args representing the mouse up event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnMouseUp(Object* sender, MouseEventArgs* e)
	{
		TreeNode *treeNode;
		Menu::MenuItemCollection *menuItems;
		DisplayableDataNode *dataNode;

		if (e->Button == MouseButtons::Right)
		{
			treeNode = projectTreeView->GetNodeAt(e->get_X(), e->get_Y());
			if (!treeNode)
				return;

			projectTreeView->set_SelectedNode(treeNode);
			dataNode = dynamic_cast <DisplayableDataNode*> (treeNode->Tag);
			menuItems = projectTreeView->ContextMenu->MenuItems;

			switch (dataNode->get_Type())
			{
			case UIData::Process:
				if (terminatedProcessTable->Contains(__box(dataNode->processID)))
				{
					pauseResumeMenuItem->Enabled = false;
					stopMenuItem->Enabled = false;
				}
				else if (pausedProcessTable->Contains(__box(dataNode->processID)))
				{
					pauseResumeMenuItem->Enabled = true;
					stopMenuItem->Enabled = true;
					pauseResumeMenuItem->Text = "Resume Application";
				}
				else
				{
					pauseResumeMenuItem->Enabled = true;
					stopMenuItem->Enabled = true;
					pauseResumeMenuItem->Text = "Pause Application";
				}
				menuItems->Add(pauseResumeMenuItem);
				menuItems->Add(stopMenuItem);
				menuItems->Add(new MenuItem("Restart Application", new EventHandler(this, RestartProcess)));
				menuItems->Add(new MenuItem("Remove Application", new EventHandler(this, RemoveProcess)));
				menuItems->Add(new MenuItem("-"));
				menuItems->Add(new MenuItem("Save Project", new EventHandler(this, saveProject)));
				//menuItems->Add(new MenuItem("Create a Report", new EventHandler(this, contextMenuCreateReport)));                
				break;
			case UIData::Thread:
				//menuItems->Add(new MenuItem("Create a Report", new EventHandler(this, contextMenuCreateReport))); 
				break;
			case UIData::RecordedSessions:
				{
					MenuItem *recordMenuItem = new MenuItem(recordMenuItemText, new EventHandler(this, contextMenuCreateRecordedSession));
					recordMenuItem->Enabled = recordMenuItemEnabled;
					menuItems->Add(recordMenuItem);
					menuItems->Add(new MenuItem("Delete Recorded Sessions", new EventHandler(this, contextMenuDelete)));

					if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
						recordMenuItem->Enabled = false;

					break;
				}
			case UIData::RecordedSession:
				{
					MenuItem *recordMenuItem = new MenuItem(recordMenuItemText, new EventHandler(this, contextMenuCreateRecordedSession));
					MenuItem *replayMenuItem = new MenuItem(replayMenuItemText, new EventHandler(this, contextMenuReplay));
					recordMenuItem->Enabled = recordMenuItemEnabled;
					replayMenuItem->Enabled = replayMenuItemEnabled;
					menuItems->Add(replayMenuItem);
					menuItems->Add(recordMenuItem);
					menuItems->Add(new MenuItem("Delete Recorded Session", new EventHandler(this, contextMenuDelete)));

					if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
					{
						recordMenuItem->Enabled = false;
						replayMenuItem->Enabled = false;
					}

					break;
				}
			case UIData::Reports:
				menuItems->Add(new MenuItem("Save All Reports", new EventHandler(this, contextMenuSave)));
				menuItems->Add(new MenuItem("Create a Report", new EventHandler(this, contextMenuCreateReport)));
				menuItems->Add(new MenuItem("Delete All Reports", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::Report:
				menuItems->Add(new MenuItem("Save Report", new EventHandler(this, contextMenuSave)));
				menuItems->Add(new MenuItem("Create Report", new EventHandler(this, contextMenuCreateReport)));
				menuItems->Add(new MenuItem("Delete Report", new EventHandler(this, contextMenuDelete)));
				menuItems->Add(new MenuItem("Rename Report", new EventHandler(this, contextMenuRename)));
				break;
			case UIData::SavedNetworkLogs:
				menuItems->Add(new MenuItem("Export All Logs to file", new EventHandler(this, contextMenuSave)));
				menuItems->Add(new MenuItem("Delete All Logs", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::SavedLogs:
				menuItems->Add(new MenuItem("Export All Logs to file", new EventHandler(this, contextMenuSave)));
				menuItems->Add(new MenuItem("Delete All Logs", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::Faults:
				menuItems->Add(new MenuItem("Create a Fault", new EventHandler(this, contextMenuCreateFault)));
				menuItems->Add(new MenuItem("Delete All Faults", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::ResourceFaults:
				menuItems->Add(new MenuItem("Create a Resource Fault", new EventHandler(this, contextMenuCreateResourceTest)));
				menuItems->Add(new MenuItem("Delete All Resource Faults", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::ResourceFault:
				menuItems->Add(new MenuItem("Create a Resource Fault", new EventHandler(this, contextMenuCreateResourceTest)));
				menuItems->Add(new MenuItem("Modify Resource Fault", new EventHandler(this, contextMenuModifyResourceTest)));
				menuItems->Add(new MenuItem("Delete Resource Fault", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::Limits:
				menuItems->Add(new MenuItem("Delete All Limits", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::Tests:
				menuItems->Add(new MenuItem("Create a Test", new EventHandler(this, contextMenuCreateTest)));
				menuItems->Add(new MenuItem("Delete All Tests", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::Test:
				menuItems->Add(new MenuItem("Create a Test", new EventHandler(this, contextMenuCreateTest)));
				menuItems->Add(new MenuItem("Modify Test", new EventHandler(this, contextMenuModifyTest)));
				menuItems->Add(new MenuItem("Delete Test", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::ActiveLogPane:
				menuItems->Add(new MenuItem("Create a Test", new EventHandler(this, contextMenuCreateTest)));
				menuItems->Add(new MenuItem("Export Log to File", new EventHandler(this, contextMenuSave)));
				break;
			case UIData::SavedLogPane:
				menuItems->Add(new MenuItem("Export Log to File", new EventHandler(this, contextMenuSave)));
				menuItems->Add(new MenuItem("Delete Log", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::Files:
			case UIData::Registry:
			case UIData::File:
			case UIData::RegistryEntry:
			case UIData::ActiveNetworkLogPane:
			case UIData::SavedNetworkLogPane:
				menuItems->Add(new MenuItem("Export Log to File", new EventHandler(this, contextMenuSave)));
				menuItems->Add(new MenuItem("Delete Log", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::MemoryFaultType:
			case UIData::DiskFaultType:
			case UIData::NetworkFaultType:
			case UIData::RegistryFaultType:
			case UIData::ProcessFaultType:
			case UIData::CustomFaultType:
			case UIData::MemoryFault:
			case UIData::DiskFault:
			case UIData::NetworkFault:
			case UIData::RegistryFault:
			case UIData::ProcessFault:
			case UIData::CustomFault:
				menuItems->Add(new MenuItem("Create a Fault", new EventHandler(this, contextMenuCreateFault)));
				menuItems->Add(new MenuItem("Delete Fault", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::NetworkCorruptionFaultType:
				menuItems->Add(new MenuItem("Create a Network Fuzzing Fault", new EventHandler(this, contextMenuCreateNetworkCorruption)));
				menuItems->Add(new MenuItem("Delete All Network Fuzzing Faults", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::NetworkCorruptionFault:
				menuItems->Add(new MenuItem("Create a Network Fuzzing Fault", new EventHandler(this, contextMenuCreateNetworkCorruption)));
				menuItems->Add(new MenuItem("Modify Network Fuzzing Fault", new EventHandler(this, contextMenuModifyNetworkCorruption)));
				menuItems->Add(new MenuItem("Delete Network Fuzzing Fault", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::FileCorruptionFaultType:
				menuItems->Add(new MenuItem("Create a File Fuzzing Fault", new EventHandler(this, contextMenuCreateFileCorruption)));
				menuItems->Add(new MenuItem("Delete All File Fuzzing Faults", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::FileCorruptionFault:
				menuItems->Add(new MenuItem("Create a File Fuzzing Fault", new EventHandler(this, contextMenuCreateFileCorruption)));
				menuItems->Add(new MenuItem("Modify File Fuzzing Fault", new EventHandler(this, contextMenuModifyFileCorruption)));
				menuItems->Add(new MenuItem("Delete File Fuzzing Fault", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::MemoryLimit:
			case UIData::DiskLimit:
			case UIData::NetworkUpLimit:
			case UIData::NetworkDownLimit:
				menuItems->Add(new MenuItem("Create a Limit", new EventHandler(this, OnDoubleClick)));
				menuItems->Add(new MenuItem("Delete Limits", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::ExceptionPane:
				menuItems->Add(new MenuItem("Delete All Exceptions", new EventHandler(this, contextMenuDelete)));
				break;
			case UIData::ExceptionInfo:
				menuItems->Add(new MenuItem("Delete Exception", new EventHandler(this, contextMenuDelete)));
				break;
			}
			Point point(e->get_X(), e->get_Y());
			projectTreeView->ContextMenu->Show(projectTreeView, point);
			menuItems->Clear();
		}
   	}

	//*************************************************************************
	// Method:		OnSelect
	// Description: Event handler for when the treenode selection changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	TreeViewEventArgs - event args representing the select event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnSelect(Object* sender, TreeViewEventArgs* e)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (e->Node->Tag);
		
		ignoreFocusChange = true;
		if ( (uiEventRegistry->OnProjectPaneFocusChanged) && (dataNode) )
			uiEventRegistry->OnProjectPaneFocusChanged->Invoke(dataNode);
		ignoreFocusChange = false;
	}

	//*************************************************************************
	// Method:		OnKeyPress
	// Description: Event handler for when a key is pressed in the treeview
	//
	// Parameters:
	//	sender - object that sent the event
	//	KeyPressEventArgs - event args representing the key press event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnKeyDown(Object* sender, KeyEventArgs* e)
	{
		TreeNode *treeNode;

		if ((e->KeyData == Keys::Delete) || (e->KeyData == Keys::Back))
		{
			treeNode = projectTreeView->SelectedNode;
			if (!treeNode)
				return;

			DeleteDataNodes(treeNode);
			
			e->Handled = true;
		}
		else if (e->KeyData == Keys::F2)
		{
			if (projectTreeView->SelectedNode)
				projectTreeView->SelectedNode->BeginEdit();
		}
	}

	//*************************************************************************
	// Method:		OnBeforeEdit
	// Description: Event handler called before a node is edited
	//
	// Parameters:
	//	sender - object that sent the event
	//	NodeLabelEditEventArgs - event args representing the label edit event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnBeforeEdit(Object *sender, NodeLabelEditEventArgs *e)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (e->Node->Tag);

		switch (dataNode->get_Type())
		{
		//case UIData::RecordedSession:
		case UIData::Report:
		//case UIData::Test:
			//these can be renamed
			break;
		default:
			//nothing else can be
			e->CancelEdit = true;
			break;
		}

	}

	//*************************************************************************
	// Method:		OnAfterEdit
	// Description: Event handler called before a node is edited
	//
	// Parameters:
	//	sender - object that sent the event
	//	NodeLabelEditEventArgs - event args representing the label edit event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnAfterEdit(Object *sender, NodeLabelEditEventArgs *e)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (e->Node->Tag);
		DisplayableDataNode *modifiedDataNode = new DisplayableDataNode(e->Label, dataNode->get_Type(), dataNode->processID, dataNode->threadID);

		if (!e->Label)
			return;

		if (dataNode->get_Type() == UIData::NodeType::Report)
		{
			Char illegalChars[] = { '.', '\\', '/', '*', '?', ':', '<', '>', '|', '\"' };
			if (e->Label->IndexOfAny(illegalChars) != -1)
			{
				UserNotification::ErrorNotify("You have entered an invalid report name.\n\nYou may not use any of the following characters in the report name: ., /, \\, *, ?, :, <, >, |, \"");
				e->CancelEdit = true;
				return;
			}
		}

		switch (dataNode->get_Type())
		{
//		case UIData::RecordedSession:
		case UIData::Report:
//		case UIData::Test:
			//these can be renamed
			e->CancelEdit = true;
			projectPaneData->BeginModifyNode(dataNode, modifiedDataNode, "Name");
			break;
		}
	}

	//*************************************************************************
	// Method:		OnDoubleClick
	// Description: Event handler called when a node is double clicked
	//
	// Parameters:
	//	sender - object that sent the event
	//	NodeLabelEditEventArgs - event args representing the label edit event
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnDoubleClick(Object *sender, EventArgs *e)
	{
		DisplayableDataNode *dataNode = new DisplayableDataNode(dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag));
		
		if (!dataNode)
			return;

		if (uiEventRegistry->OnDoubleClick)
			uiEventRegistry->OnDoubleClick->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		OnItemAdded
	// Description: Event handler for when a new item is added to the data array
	//
	// Parameters:
	//	Index - index in the data array for the item that was added
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnItemAdded(DisplayableDataNode *dataNode)
	{
		DisplayableDataNode *newNode;
		ArrayList *treeNodeArray;
		TreeNode *treeNode;
		String *processTitle, *threadTitle;
		char *idString;

		// get the hsa object and make sure it isn't the services.exe process on starting a service.
		HoloScriptRegistry *hsRegistry = HoloScriptRegistry::getInstance();
		HoloScriptApplication *thisHoloScriptApp = hsRegistry->getHoloScriptAppForProcess(dataNode->processID);
		if ( (dataNode->Type == UIData::Process) && (!thisHoloScriptApp || (thisHoloScriptApp == hsRegistry->ServicesExeApplication)) )
			return;

		//if this is the first insertion then setup session-level nodes
		if (firstUse && firstUseMutex->WaitOne(0, false))
		{
			firstUse = false;
			firstUseMutex->ReleaseMutex();
			newNode = new DisplayableDataNode("Saved Logs", UIData::SavedLogs, 0, 0);
			AddNode(newNode, 0);
			newNode = new DisplayableDataNode("Saved Network Logs", UIData::SavedNetworkLogs, 0, 0);
			AddNode(newNode, 1);
			newNode = new DisplayableDataNode("Saved Fuzzed Files", UIData::SavedCorruptedFiles, 0, 0);
			AddNode(newNode, 2);
			newNode = new DisplayableDataNode("Saved Exceptions", UIData::ExceptionPane, 0, 0);
			AddNode(newNode, 3);
			//newNode = new DisplayableDataNode("Reports", UIData::Reports, 0, 0);
			//AddNode(newNode, 4);
			newNode = new DisplayableDataNode("Recorded Sessions", UIData::RecordedSessions, 0, 0);
			AddNode(newNode, 4);
		}

		//if this is the first instance of this process then setup the process-level nodes
		if (IsNewProcess(dataNode->processID) && (dataNode->Type != UIData::SavedLogPane))
		{
			newNode = new DisplayableDataNode("Process", UIData::Process, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Network Logs", UIData::ActiveNetworkLogPane, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Faults", UIData::Faults, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Disk", UIData::DiskFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Memory", UIData::MemoryFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Network", UIData::NetworkFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Registry", UIData::RegistryFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Process", UIData::ProcessFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Custom", UIData::CustomFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Resource Faults", UIData::ResourceFaults, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Limits", UIData::Limits, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Network Fuzzing Faults", UIData::NetworkCorruptionFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("File Fuzzing Faults", UIData::FileCorruptionFaultType, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Fuzzed Files", UIData::CorruptedFiles, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Tests", UIData::Tests, dataNode->processID, 0);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Exceptions", UIData::ExceptionPane, dataNode->processID, 0);
			AddNode(newNode, -1);

			for (int i=0; i<projectTreeView->Nodes->Count; i++)
				projectTreeView->Nodes->get_Item(i)->Expand();
		}
		
		//if this is a new thread and we are in thread mode then setup the thread-level nodes
		if ( perThread && IsNewThread(dataNode->processID, dataNode->threadID) )
		{
			newNode = new DisplayableDataNode("Thread", UIData::Thread, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Network Logs", UIData::ActiveNetworkLogPane, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Faults", UIData::Faults, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Disk", UIData::DiskFaultType, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Memory", UIData::MemoryFaultType, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Network", UIData::NetworkFaultType, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Registry", UIData::RegistryFaultType, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Process", UIData::ProcessFaultType, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Custom", UIData::CustomFaultType, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Tests", UIData::Tests, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
			newNode = new DisplayableDataNode("Exceptions", UIData::ExceptionPane, dataNode->processID, dataNode->threadID);
			AddNode(newNode, -1);
		}

		//special case log panes so we don't show a node for each thread in per-process mode
		if ( (!perThread) && (dataNode->Type == UIData::ActiveLogPane) && (dataNode->threadID != 0) )
			return;

		if (dataNode->Type == UIData::ActiveLogPane)	//ensure active log pane is always first in the list
			AddNode(dataNode, 0);
		else
			AddNode(dataNode, -1);

		//set thread and process node names
		if (dataNode->Type == UIData::ActiveLogPane)
		{
			idString = new char[12];
			processTitle = dataNode->Name;

			HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(dataNode->processID);
			if (holoScriptApp && holoScriptApp->IsService)
			{
				processTitle = String::Concat(processTitle, " (");
				processTitle = String::Concat(processTitle, holoScriptApp->ServiceName);
				processTitle = String::Concat(processTitle, ", process - ");
			}
			else
				processTitle = String::Concat(processTitle, " (process - ");
			idString = itoa(dataNode->processID, idString, 10);
			processTitle = String::Concat(processTitle, idString);
			processTitle = String::Concat(processTitle, ")");

			treeNodeArray = GetNode(UIData::Process, dataNode->processID, 0, projectTreeView->Nodes->get_Item(0));
			if (!treeNodeArray)
				return;
	
			treeNode = dynamic_cast <TreeNode*> (treeNodeArray->get_Item(0));
			treeNode->Text = processTitle;

			if (perThread)
			{
				ZeroMemory(idString, sizeof(char)*12);
				threadTitle = String::Concat(threadTitle, "Thread - ");
				idString = itoa(dataNode->threadID, idString, 10);
				threadTitle = String::Concat(threadTitle, idString);

				treeNodeArray = GetNode(UIData::Thread, dataNode->processID, dataNode->threadID, projectTreeView->Nodes->get_Item(0));
				if (!treeNodeArray)
					return;
		
				treeNode = dynamic_cast <TreeNode*> (treeNodeArray->get_Item(0));
				treeNode->Text = threadTitle;
			}
			delete idString;
		}
	}

	//*************************************************************************
	// Method:		onDeleteNode
	// Description: delegate to handle cross thread marshalling
	//
	// Parameters:
	//	dataNode - data node that represents the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::onDeleteNode(DisplayableDataNode *dataNode)
	{		
		projectTreeView->Nodes->Remove(dynamic_cast <TreeNode*> (dataNode->Element));
	}

	//*************************************************************************
	// Method:		OnItemDeleted
	// Description: Event handler for when an item is about to be deleted from the data array
	//
	// Parameters:
	//	Index - index in the data array for the item that will be deleted
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnItemDeleted(DisplayableDataNode *dataNode)
	{
		Object *args[];
		args = __gc new Object*[1];
		args[0] = dataNode;
		Control::Invoke(deleteNodeDelegate, args);	
	}

	//*************************************************************************
	// Method:		OnItemModified
	// Description: Event handler for when an item's property is modified
	//
	// Parameters:
	//	Index - index in the data array for the item that was modified
	//	propertyName - name of the property that changed
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnItemModified(DisplayableDataNode *dataNode, String *propertyName)
	{
		TreeNode *treeNode;
		
		if (propertyName->CompareTo("Name") == 0)
		{
			treeNode = dynamic_cast <TreeNode*> (dataNode->Element);
			treeNode->Text = dataNode->get_Name();
		}
	}


	//*************************************************************************
	// Method:		GetNode
	// Description: Calls getnodehelper for each toplevel node.
	//
	// Parameters:
	//	Type - Type of node to return
	//	processID - processID of node to return
	//	threadID - threadID of node to return
	//	startNode - pointer to treenode to start searching from
	//
	// Return Value: Tree node array that was found, NULL if not found
	//*************************************************************************
	ArrayList *ProjectPane::GetNode(NodeType Type, int processID, int threadID, TreeNode *startNode)
	{
		ArrayList *treeNodeArray = new ArrayList();
		ArrayList *newNodeList;

		do
		{
			newNodeList = GetNodeHelper(Type, processID, threadID, startNode);
			if (newNodeList)
				treeNodeArray->AddRange(newNodeList);
			startNode = startNode->NextNode;
		} 
		while (startNode);

		if (treeNodeArray->Count > 0)
			return treeNodeArray;
		else 
			return NULL;
	}

	//*************************************************************************
	// Method:		GetNodeHelper
	// Description: Performs a depth first search and returns a node in the tree 
	//	that matches the type and processID requested
	//
	// Parameters:
	//	Type - Type of node to return
	//	processID - processID of node to return
	//	threadID - threadID of node to return
	//	startNode - pointer to treenode to start searching from
	//
	// Return Value: Tree node that was found, NULL if not found
	//*************************************************************************
	ArrayList* ProjectPane::GetNodeHelper(NodeType Type, int processID, int threadID, TreeNode *startNode)
	{			
		DisplayableDataNode *dataNode;
		TreeNode *treeNode;
		int Count = startNode->Nodes->Count;
		ArrayList *nodeList = new ArrayList();
		ArrayList *newNodeList;

		if (!startNode)
			return nodeList;

		//is this one it?
		dataNode = dynamic_cast <DisplayableDataNode*> (startNode->Tag);

		if (!dataNode)
			return nodeList;

		//special case saved logs.  They need to have different pID's than parent node, only thing matters to match is type
		if ( (Type == UIData::SavedLogs) && (dataNode->Type == Type) )
		{
			nodeList->Add(startNode);
		}
		else if ( (Type == UIData::SavedNetworkLogs) && (dataNode->Type == Type) )
		{
			nodeList->Add(startNode);
		}
		//if in per thread mode ensure process items also are children of thread node
		else if (perThread)
		{
			if (Type != UIData::Process)
			{
				if ( (dataNode->Type == Type) && (dataNode->processID == processID) && (dataNode->threadID == threadID) )
					nodeList->Add(startNode);
			}
			else
			{
				if ( (dataNode->Type == Type) && (dataNode->processID == processID) && ((dataNode->threadID == threadID) || dataNode->threadID == 0) )
					nodeList->Add(startNode);
			}
		}
		else 
		{
			if ( (dataNode->Type == Type) && (dataNode->processID == processID) && (dataNode->threadID == threadID) )
				nodeList->Add(startNode);
		}

		//look deeper
		for (int i=0; i<Count; i++)
		{
			treeNode = startNode->Nodes->get_Item(i);

			newNodeList = GetNodeHelper(Type, processID, threadID, treeNode);
			if (newNodeList)
				nodeList->AddRange(newNodeList);
		}

		return nodeList;
	}	

	//*************************************************************************
	// Method:		AddNode
	// Description: marshalls cross thread and calls onAddNode
	//
	// Parameters:
	//	dataNode - dataNode represting node to add
	//	position - Position in which to insert the node, -1 means add to the end.
	//
	// Return Value: Tree node that was found, NULL if not found
	//*************************************************************************
	void ProjectPane::AddNode(DisplayableDataNode *dataNode, int position)
	{
		Object *args[] = __gc new Object*[2];
		args[0] = dataNode;
		args[1] = __box(position);
		Control::Invoke(addNodeDelegate, args);
	}

	//*************************************************************************
	// Method:		onAddNode
	// Description: Adds a node to the tree
	//
	// Parameters:
	//	dataNode - dataNode represting node to add
	//	position - Position in which to insert the node, -1 means add to the end.
	//
	// Return Value: Tree node that was found, NULL if not found
	//*************************************************************************
	void ProjectPane::onAddNode(DisplayableDataNode *dataNode, int position)
	{
		NodeType parentType;
		TreeNode *treeNode;

		if (!dataNode)
			return;

		if ( (dataNode->threadID != 0) && (perThread) )
			parentType = UIData::Thread;
		else
			parentType = UIData::Process;

		switch (dataNode->get_Type())
		{
		case UIData::SavedLogs:
		case UIData::SavedNetworkLogs:
		case UIData::SavedCorruptedFiles:
		case UIData::Reports:  
		case UIData::RecordedSessions:  
		case UIData::Process:  //always top-level node
			treeNode = new TreeNode(dataNode->get_Name());
			if (terminatedProcessTable->Contains(__box(dataNode->processID)))
			{
				treeNode->ImageIndex = TERMINATED_PROCESS_INDEX;
				treeNode->SelectedImageIndex = TERMINATED_PROCESS_INDEX;
			}
			else if (pausedProcessTable->Contains(__box(dataNode->processID)))
			{
				treeNode->ImageIndex = PAUSED_PROCESS_INDEX;
				treeNode->SelectedImageIndex = PAUSED_PROCESS_INDEX;
			}
			else
			{
				treeNode->ImageIndex = dataNode->Type;
				treeNode->SelectedImageIndex = dataNode->Type;
			}
			treeNode->Tag = dataNode;		//treenode points to datanode
			dataNode->Element = treeNode;		//datanode points to treenode
			if (position == -1)
				projectTreeView->Nodes->Add(treeNode);
			else
				projectTreeView->Nodes->Insert(position, treeNode);
			break;
		case UIData::CorruptedFile:
			InsertNode(dataNode, UIData::CorruptedFiles, position);
			break;
		case UIData::SavedCorruptedFile:
			InsertNode(dataNode, UIData::SavedCorruptedFiles, position);
			break;
		case UIData::ExceptionPane:
			if (dataNode->processID == 0)
			{
				treeNode = new TreeNode(dataNode->get_Name());
				treeNode->ImageIndex = dataNode->Type;
				treeNode->SelectedImageIndex = dataNode->Type;
				treeNode->Tag = dataNode;		//treenode points to datanode
				dataNode->Element = treeNode;		//datanode points to treenode
				if (position == -1)
					projectTreeView->Nodes->Add(treeNode);
				else
					projectTreeView->Nodes->Insert(position, treeNode);
			}
			else
				InsertNode(dataNode, parentType, position);
			break;
		case UIData::Thread:
			InsertNode(dataNode, UIData::Process, position);
			break;
		case UIData::RecordedSession:
			InsertNode(dataNode, UIData::RecordedSessions, position);
			break;
		case UIData::Report:
			InsertNode(dataNode, UIData::Reports, position);
			break;
		case UIData::ActiveLogPane:
		case UIData::ActiveNetworkLogPane:
		case UIData::Faults:
		case UIData::ResourceFaults:
		case UIData::Limits:
		case UIData::Tests:
		case UIData::NetworkCorruptionFaultType:
		case UIData::FileCorruptionFaultType:
		case UIData::CorruptedFiles:
			InsertNode(dataNode, parentType, position);
			break;
		case UIData::Test:
			InsertNode(dataNode, UIData::Tests, position);
			break;
		case UIData::SavedLogPane:
			InsertNode(dataNode, UIData::SavedLogs, position);
			break;
		case UIData::SavedNetworkLogPane:
			InsertNode(dataNode, UIData::SavedNetworkLogs, position);
			break;
		case UIData::MemoryFaultType:
		case UIData::DiskFaultType:
		case UIData::NetworkFaultType:
		case UIData::RegistryFaultType:
		case UIData::ProcessFaultType:
		case UIData::CustomFaultType:
			InsertNode(dataNode, UIData::Faults, position);
			break;
		case UIData::MemoryFault:
			InsertNode(dataNode, UIData::MemoryFaultType, position);
			break;
		case UIData::DiskFault:
			InsertNode(dataNode, UIData::DiskFaultType, position);
			break;
		case UIData::NetworkFault:
			InsertNode(dataNode, UIData::NetworkFaultType, position);
			break;
		case UIData::RegistryFault:
			InsertNode(dataNode, UIData::RegistryFaultType, position);
			break;
		case UIData::ProcessFault:
			InsertNode(dataNode, UIData::ProcessFaultType, position);
			break;
		case UIData::CustomFault:
			InsertNode(dataNode, UIData::CustomFaultType, position);
			break;
		case UIData::ResourceFault:
			InsertNode(dataNode, UIData::ResourceFaults, position);
			break;
		case UIData::NetworkCorruptionFault:
			InsertNode(dataNode, UIData::NetworkCorruptionFaultType, position);
			break;
		case UIData::FileCorruptionFault:
			InsertNode(dataNode, UIData::FileCorruptionFaultType, position);
			break;
		case UIData::MemoryLimit:
		case UIData::DiskLimit:
		case UIData::NetworkUpLimit:
		case UIData::NetworkDownLimit:
			InsertNode(dataNode, UIData::Limits, position);
			break;
		case UIData::ExceptionInfo:
			InsertNode(dataNode, UIData::ExceptionPane, position);
			break;
		}
	}

	//*************************************************************************
	// Method:		InsertNode
	// Description: Inserts a new node into the right part of the tree
	//
	// Parameters:
	//	Node - data node that represents the node to insert
	//	parentNodeType - node type of the parent we will insert under
	//	position - Position in which to insert the node, -1 means add to the end.
	//
	// Return Value: true if succesful, false if not
	//*************************************************************************
	bool ProjectPane::InsertNode(DisplayableDataNode *dataNode, NodeType parentNodeType, int position)
	{		
		ArrayList *treeNodeArray;
		TreeNode *treeNode;
		IEnumerator *enumerator;
		TreeNode *newTreeNode;

		if (dataNode->Type == UIData::ActiveLogPane)
			newTreeNode = new TreeNode("API Log");
		else
			newTreeNode = new TreeNode(dataNode->Name);

		if (projectTreeView->Nodes->Count > 0)
		{				
			//don't add duplicate limit nodes
			if ( (dataNode->Type == UIData::DiskLimit) || (dataNode->Type == UIData::MemoryLimit) || (dataNode->Type == UIData::NetworkUpLimit) || (dataNode->Type == UIData::NetworkDownLimit) )
			{
				treeNodeArray = GetNode(dataNode->Type, dataNode->processID, dataNode->threadID, projectTreeView->Nodes->get_Item(0));
				if (treeNodeArray)
				{
					enumerator = treeNodeArray->GetEnumerator();
					while (enumerator->MoveNext())
					{
						treeNode = dynamic_cast <TreeNode*> (enumerator->Current);
						if (treeNode->Text->CompareTo(newTreeNode->Text) == 0)
							return false;
					}
				}
			}

			//now add to the tree
			if (perThread)
				treeNodeArray = GetNode(parentNodeType, dataNode->processID, dataNode->threadID, projectTreeView->Nodes->get_Item(0));
			else
				treeNodeArray = GetNode(parentNodeType, dataNode->processID, 0, projectTreeView->Nodes->get_Item(0));
			
			if (!treeNodeArray)
				return false;

			enumerator = treeNodeArray->GetEnumerator();
			while (enumerator->MoveNext())
			{
				treeNode = dynamic_cast <TreeNode*> (enumerator->Current);
				if (position == -1)
				{
					treeNode->Nodes->Add(newTreeNode);
					projectTreeView->SelectedNode = newTreeNode;
				}
				else
				{
					treeNode->Nodes->Insert(position, newTreeNode);
					projectTreeView->SelectedNode = newTreeNode;
				}

				newTreeNode->ImageIndex = dataNode->Type;
				newTreeNode->SelectedImageIndex = dataNode->Type;
				newTreeNode->Tag = dataNode;		//new treenode points to datanode
				dataNode->Element = newTreeNode;	//datanode points to new treenode
			}
		}
		else
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		DeleteDataNodes
	// Description: Deletes the data node associated with this tree node, and all subnodes 
	//
	// Parameters:
	//	startNode - node that will be deleted
	//
	// Return Value: true if at least one node was deleted, false if nothing was deleted
	//*************************************************************************
	bool ProjectPane::DeleteDataNodes(TreeNode *treeNode)
	{
		DisplayableDataNode *dataNode;
		bool returnVal = false;
		
		if (!treeNode)
			return returnVal;

		//dive in depth first
		int index = 0;
		int count = treeNode->Nodes->Count;
		while (treeNode->Nodes->Count > index)
		{
			if ( (!DeleteDataNodes(treeNode->Nodes->get_Item(index))) || (count == treeNode->Nodes->Count) )
				index++;
			else
			{
				returnVal = true;
				count = treeNode->Nodes->Count;
			}
		}

		//if it is a leaf then delete it
		if (treeNode->Nodes->Count == 0 )
		{
			dataNode = dynamic_cast <DisplayableDataNode*> (treeNode->Tag);

			switch (dataNode->get_Type())
			{
			case UIData::Process:
			case UIData::Thread:
			case UIData::RecordedSessions:
			case UIData::Reports:
			case UIData::SavedLogs:
			case UIData::SavedNetworkLogs:
			case UIData::ActiveLogPane:
			case UIData::ActiveNetworkLogPane:
			case UIData::Files:
			case UIData::Registry:
			case UIData::Faults:
			case UIData::MemoryFaultType:
			case UIData::DiskFaultType:
			case UIData::NetworkFaultType:
			case UIData::RegistryFaultType:
			case UIData::ProcessFaultType:
			case UIData::CustomFaultType:
			case UIData::ResourceFaults:
			case UIData::NetworkCorruptionFaultType:
			case UIData::FileCorruptionFaultType:
			case UIData::SavedCorruptedFiles:
			case UIData::Limits:
			case UIData::Tests:
			case UIData::ExceptionPane:
				//no delete operation for these
				break;
			default:
				//anything else gets deleted
				if (projectPaneData->BeginDeleteNode(dataNode))
					returnVal = true;
				break;
			}
		}

		return returnVal;
	}

	//*************************************************************************
	// Method:		contextMenuDelete
	// Description: handles the context menu delete click event
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuDelete(Object *sender, EventArgs *args)
	{
		DeleteDataNodes(projectTreeView->SelectedNode);
	}

	//*************************************************************************
	// Method:		contextMenuRename
	// Description: handles the context menu rename click event
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuRename(Object *sender, EventArgs *args)
	{
		projectTreeView->SelectedNode->BeginEdit();
	}

	//*************************************************************************
	// Method:		PauseProcess
	// Description: handles the pause application click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::PauseProcess(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		
		if (dataNode && uiEventRegistry->OnPauseProcess)
			uiEventRegistry->OnPauseProcess->Invoke(dataNode->processID);
	}

	//*************************************************************************
	// Method:		TerminateProcess
	// Description: handles the terminate application click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::TerminateProcess(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		
		if (dataNode && uiEventRegistry->OnTerminateProcess)
			uiEventRegistry->OnTerminateProcess->Invoke(dataNode->processID);
	}

	//*************************************************************************
	// Method:		RestartProcess
	// Description: handles the restart application click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::RestartProcess(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		
		if (dataNode && uiEventRegistry->OnRestartProcess)
			uiEventRegistry->OnRestartProcess->Invoke(dataNode->processID);
	}

	//*************************************************************************
	// Method:		RemoveProcess
	// Description: handles the remove application click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::RemoveProcess(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		
		if (dataNode && uiEventRegistry->OnRemoveProcess)
			uiEventRegistry->OnRemoveProcess->Invoke(dataNode->processID);
	}

	//*************************************************************************
	// Method:		saveProject
	// Description: handles the save project click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::saveProject(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		
		if ( (dataNode) && (dataNode->SaveDelegate) )
			dataNode->SaveDelegate->DynamicInvoke(NULL);
	}

	//*************************************************************************
	// Method:		contextMenuSave
	// Description: handles the context menu save click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuSave(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		TreeNode *treeNode = dynamic_cast <TreeNode*> (dataNode->Element);

		//save node and all subnodes
		projectPaneData->SaveNode(dataNode);
		if (treeNode->Nodes->Count > 0)
		{
			treeNode = treeNode->Nodes->get_Item(0);
			while (treeNode)
			{
				projectPaneData->SaveNode(dynamic_cast <DisplayableDataNode*> (treeNode->Tag));
				treeNode = treeNode->NextNode;
			}
		}
	}

	//*************************************************************************
	// Method:		contextMenuCreateRecordedSession
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateRecordedSession(Object *sender, EventArgs *args)
	{
		MenuItem *menuItem = dynamic_cast <MenuItem*> (sender);
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		DisplayableDataNode *createNode = new DisplayableDataNode("", UIData::RecordedSession, dataNode->processID, dataNode->threadID);

		if (menuItem->Text->CompareTo("Create Recorded Session") == 0)
		{
			projectPaneData->BeginCreateNode(createNode);
		}
		else
		{
			if (!uiEventRegistry->OnRecordedSessionStop)
				return;
            
			uiEventRegistry->OnRecordedSessionStop->Invoke(dataNode);
		}
	}

	//*************************************************************************
	// Method:		contextMenuReplay
	// Description: handles the context menu replay events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuReplay(Object *sender, EventArgs *args)
	{
		MenuItem *menuItem = dynamic_cast <MenuItem*> (sender);
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);

		if (menuItem->Text->CompareTo("Replay") == 0)
		{
			if (!uiEventRegistry->OnRecordedSessionReplayStart)
				return;
			else
				uiEventRegistry->OnRecordedSessionReplayStart->Invoke(dataNode);
		}
		else
		{
			if (!uiEventRegistry->OnRecordedSessionReplayStop)
				return;
			else
				uiEventRegistry->OnRecordedSessionReplayStop->Invoke(dataNode);
		}
	}
	//**************************************************************************
	// Method:		RecordedSessionStarted
	// Description: event handler that's called when a recorded session has started
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::RecordedSessionStarted(DisplayableDataNode *dataNode)
	{
			recordMenuItemText = "Stop Recording";
			replayMenuItemEnabled = false;
	}

	//**************************************************************************
	// Method:		RecordedSessionStopped
	// Description: event handler that's called when a recorded session has stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::RecordedSessionStopped(DisplayableDataNode *dataNode)
	{
			recordMenuItemText = "Create Recorded Session";
			replayMenuItemEnabled = true;
	}
	//**************************************************************************
	// Method:		RecordedSessionReplayStarted
	// Description: event handler that's called when a recorded session is replayed
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::RecordedSessionReplayStarted(DisplayableDataNode *dataNode)
	{
		replayMenuItemText = "Stop Replay";
		recordMenuItemEnabled = false;
	}

	//**************************************************************************
	// Method:		RecordedSessionReplayStopped
	// Description: event handler that's called when a recorded session replay has stopped
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::RecordedSessionReplayStopped(DisplayableDataNode *dataNode)
	{
		replayMenuItemText = "Replay";
		recordMenuItemEnabled = true;
	}

	//*************************************************************************
	// Method:		contextMenuCreateReport
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateReport(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		DisplayableDataNode *createNode = new DisplayableDataNode("", UIData::Report, dataNode->processID, dataNode->threadID);

		projectPaneData->BeginCreateNode(createNode);
	}

	//*************************************************************************
	// Method:		contextMenuCreateTest
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateTest(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		DisplayableDataNode *createNode = new DisplayableDataNode("", UIData::Test, dataNode->processID, dataNode->threadID);
		uiEventRegistry->OnStartCreateTestWizard->Invoke(createNode);
	}

	//*************************************************************************
	// Method:		contextMenuCreateFault
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateFault(Object *sender, EventArgs *args)
	{
		OnDoubleClick(sender, args);
	}

	//*************************************************************************
	// Method:		contextMenuCreateResourceTest
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateResourceTest(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		ResourceTestDataNode *createNode = new ResourceTestDataNode("", "", FaultType::CustomFault, dataNode->processID, dataNode->threadID);
		uiEventRegistry->OnStartCreateResourceTestWizard(createNode);
	}

	//*************************************************************************
	// Method:		contextMenuCreateNetworkCorruption
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateNetworkCorruption(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		DisplayableDataNode *createNode = new DisplayableDataNode("", UIData::NetworkCorruptionFault, dataNode->processID, 0);

		uiEventRegistry->OnStartNetworkCorruptionWizard->Invoke(createNode);
	}

	//*************************************************************************
	// Method:		contextMenuCreateFileCorruption
	// Description: handles the context menu create click events
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuCreateFileCorruption(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		DisplayableDataNode *createNode = new DisplayableDataNode("", UIData::FileCorruptionFault, dataNode->processID, 0);

		uiEventRegistry->OnStartFileCorruptionWizard->Invoke(createNode);
	}

	//*************************************************************************
	// Method:		contextMenuModifyTest
	// Description: handles the context menu modify test event
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuModifyTest(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		uiEventRegistry->OnStartModifyTestWizard->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		contextMenuModifyResourceTest
	// Description: handles the context menu modify resource test event
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuModifyResourceTest(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		uiEventRegistry->OnStartModifyResourceTestWizard->Invoke(dynamic_cast <ResourceTestDataNode*> (dataNode->DataArray));
	}

	//*************************************************************************
	// Method:		contextMenuModifyNetworkCorruption
	// Description: handles the context menu modify test event
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuModifyNetworkCorruption(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		uiEventRegistry->OnStartModifyNetworkCorruptionWizard->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		contextMenuModifyFileCorruption
	// Description: handles the context menu modify test event
	//
	// Parameters:
	//	sender - object that originated the event
	//	args - event details
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::contextMenuModifyFileCorruption(Object *sender, EventArgs *args)
	{
		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (projectTreeView->SelectedNode->Tag);
		uiEventRegistry->OnStartModifyFileCorruptionWizard->Invoke(dataNode);
	}

	//*************************************************************************
	// Method:		IsNewProcess
	// Description: Determines if the process is new to the treeview
	//
	// Parameters:
	//	processID - process ID to look for
	//
	// Return Value: true if not found, false if found
	//*************************************************************************
	bool ProjectPane::IsNewProcess(int processID)
	{
		String *pidString = Convert::ToString(processID);

		if (projectTreeView->Nodes->Count == 0)
			return true;

		if (processID == 0)
			return false;

		Mutex *newProcessMutex = dynamic_cast <Mutex*> (processMutexList->Item[pidString]);
		if (!newProcessMutex)
		{
			newProcessMutex = new Mutex(false, pidString);
			processMutexList->Add(pidString, newProcessMutex);
		}

		if (!newProcessMutex->WaitOne(0, false))
			return false;

		ArrayList *treeNodeArray = GetNode(UIData::Process, processID, 0, projectTreeView->Nodes->get_Item(0));
		if (treeNodeArray)
		{
			newProcessMutex->ReleaseMutex();
			return false;
		}
		else
		{
			treeNodeArray = GetNode(UIData::SavedLogPane, processID, 0, projectTreeView->Nodes->get_Item(0));
			if (treeNodeArray)
			{
				newProcessMutex->ReleaseMutex();
				return false;
			}
		}

		newProcessMutex->ReleaseMutex();
		return true;
	}

	//*************************************************************************
	// Method:		IsNewThread
	// Description: Determines if the process is new to the treeview
	//
	// Parameters:
	//	threadID - thread ID to look for
	//
	// Return Value: true if not found, false if found
	//*************************************************************************
	bool ProjectPane::IsNewThread(int processID, int threadID)
	{
		String *tidString = Convert::ToString(threadID);

		if (projectTreeView->Nodes->Count == 0)
			return true;

		if (threadID == 0)
			return false;

		Mutex *newThreadMutex = dynamic_cast <Mutex*> (threadMutexList->Item[tidString]);
		if (!newThreadMutex)
		{
			newThreadMutex = new Mutex(false, tidString);
			threadMutexList->Add(tidString, newThreadMutex);
		}

		if (!newThreadMutex->WaitOne(0, false))
			return false;

		ArrayList *treeNodeArray = GetNode(UIData::Thread, processID, threadID, projectTreeView->Nodes->get_Item(0));
		if (treeNodeArray)
		{
			newThreadMutex->ReleaseMutex();
			return false;
		}
		else
		{
			newThreadMutex->ReleaseMutex();
			return true;
		}
	}

	//*************************************************************************
	// Method:		OnPerThreadChanged
	// Description: called when perthread setting changes
	//
	// Parameters:
	//	perThreadValue - value of perThread setting
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::OnPerThreadChanged(bool perThreadValue)
	{
		perThread = perThreadValue;
		OnCloseProjectUI();
		projectPaneData->Refresh();
	}

	//*************************************************************************
	// Method:		OnCloseProjectUI
	// Description: called when project is closing
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::OnCloseProjectUI()
	{
		//remove all nodes
		while (projectTreeView->Nodes->Count > 0)
			projectTreeView->Nodes->RemoveAt(0);

		firstUse = true;
	}

	//*************************************************************************
	// Method:		OnFocusChanged
	// Description: Called when focus changes in any ui pane
	//
	// Parameters:
	//	dataNode - represents the object that got focus
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::OnFocusChanged(DisplayableDataNode *dataNode)
	{
		TreeNode *treeNode;
		ArrayList *treeNodeList;
		String *name;
		DisplayableDataNode *treeDataNode;

		//don't change selection made by the user
		if (ignoreFocusChange)
			return;

		if (!dataNode)
			return;

		if (projectTreeView->Nodes->Count == 0)
			return;

		//look for tree node
		treeNodeList = GetNode(dataNode->Type, dataNode->processID, dataNode->threadID, projectTreeView->Nodes->Item[0]);
		if (!treeNodeList)
			return;

		for (int i=0; i < treeNodeList->Count; i++)
		{
			treeNode = dynamic_cast <TreeNode*> (treeNodeList->Item[i]);
			treeDataNode = dynamic_cast <DisplayableDataNode*> (treeNode->Tag);
		
			if (treeDataNode)
				name = treeDataNode->Name;
			else
				name = treeNode->Text;

			if ( (name->CompareTo(dataNode->Name) == 0) && (treeDataNode->processID == dataNode->processID) && (treeDataNode->threadID == dataNode->threadID) )
			{
				//grab dataarray so we have more property information
				treeDataNode->DataArray = dataNode->DataArray;
				projectTreeView->SelectedNode = treeNode;
				break;
			}
		}
	}

	//*************************************************************************
	// Method:		onProcessRemoved
	// Description: Celled when a process is removed from the project
	//
	// Parameters:
	//	None
	//
	// Return Value: none
	//*************************************************************************
	void ProjectPane::onProcessRemoved(unsigned int processID)
	{
		//Remove the process data from the project pane data array
		projectPaneData->RemoveProcess (processID);
		//Refresh UI
		OnCloseProjectUI();
		projectPaneData->Refresh();
	}

	//*************************************************************************
	// Method:		onProcessTerminated
	// Description: called when a process is terminated
	//
	// Parameters:
	//	processID - process that was terminated
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnProcessTerminated(unsigned int processID)
	{
		if (projectTreeView->Nodes->Count == 0)
			return;

		ArrayList *treeNodeArray = GetNode(UIData::Process, processID, 0, projectTreeView->Nodes->get_Item(0));
		if (!treeNodeArray || (treeNodeArray->Count == 0))
			return;

		TreeNode *treeNode = dynamic_cast <TreeNode*> (treeNodeArray->get_Item(0));
		if (!treeNode)
			return;

		treeNode->ImageIndex = TERMINATED_PROCESS_INDEX;
		treeNode->SelectedImageIndex = TERMINATED_PROCESS_INDEX;
		if (!terminatedProcessTable->Contains(__box(processID)))
			terminatedProcessTable->Add(__box(processID), __box(processID));
	}

	//*************************************************************************
	// Method:		onProcessPaused
	// Description: called when a process is paused
	//
	// Parameters:
	//	processID - process that was paused
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnProcessPaused(unsigned int processID)
	{
		if (projectTreeView->Nodes->Count == 0)
			return;

		ArrayList *treeNodeArray = GetNode(UIData::Process, processID, 0, projectTreeView->Nodes->get_Item(0));
		if (!treeNodeArray || (treeNodeArray->Count == 0))
			return;

		TreeNode *treeNode = dynamic_cast <TreeNode*> (treeNodeArray->get_Item(0));
		if (!treeNode)
			return;

		treeNode->ImageIndex = PAUSED_PROCESS_INDEX;
		treeNode->SelectedImageIndex = PAUSED_PROCESS_INDEX;
		if (!pausedProcessTable->Contains(__box(processID)))
			pausedProcessTable->Add(__box(processID), __box(processID));
	}

	//*************************************************************************
	// Method:		onProcessResumed
	// Description: called when a process is resumed
	//
	// Parameters:
	//	processID - process that was resumed
	//
	// Return Value: None
	//*************************************************************************
	void ProjectPane::OnProcessResumed(unsigned int processID)
	{
		if (projectTreeView->Nodes->Count == 0)
			return;

		ArrayList *treeNodeArray = GetNode(UIData::Process, processID, 0, projectTreeView->Nodes->get_Item(0));
		if (!treeNodeArray || (treeNodeArray->Count == 0))
			return;

		TreeNode *treeNode = dynamic_cast <TreeNode*> (treeNodeArray->get_Item(0));
		if (!treeNode)
			return;

		treeNode->ImageIndex = UIData::Process;
		treeNode->SelectedImageIndex = UIData::Process;
		pausedProcessTable->Remove(__box(processID));
	}
}
