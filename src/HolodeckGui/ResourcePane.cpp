//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourcePane.cpp
//
// DESCRIPTION: Contains implementation for the class ResourcePane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include <psapi.h>
#include <vcclr.h>
#include "ResourcePane.h"
#include "SiString.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ResourcePane
	// Description: Constructor for the ResourcePane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourcePane::ResourcePane()
	{
		dataEventRegistry = DataEventRegistry::getInstance();

		dataArrayList = new SortedList();
		Menu::MenuItemCollection *menuItems;
		
		//fill in faults
		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
		IList *diskFaults = faultDB->DiskFaults;
		IList *registryFaults = faultDB->RegistryFaults;
		IList *processFaults = faultDB->ProcessFaults;
		IList *customFaults = faultDB->CustomFaults;

		threadID = 0;
		ignoreFocusChange = false;

		InitializeUI();

		resourceTreeView->ContextMenu = new System::Windows::Forms::ContextMenu();
		resourceTreeView->ContextMenu->MenuItems->Add(new MenuItem("Copy", new EventHandler(this, onCopyToClipboard)));
		resourceTreeView->ContextMenu->MenuItems->Add(new MenuItem("Export Resources to File...", new EventHandler(this, onExportToFile)));
		resourceTreeView->ContextMenu->MenuItems->Add(new MenuItem("-"));
		selectLogItemMenu = new MenuItem("Select most recent log entry", new EventHandler(this, SelectLogEntry));
		createFileCorruptionMenu = new MenuItem("Fuzz file", new EventHandler(this, StartCreateFileCorruptionWizard));
		createFaultMenu = new MenuItem("Create a fault", new EventHandler(this, StartCreateResourceTestWizard));
		createFaultMenu->DefaultItem = true;
		modifyFaultMenu = new MenuItem("Modify existing fault", new EventHandler(this, StartCreateResourceTestWizard));
		clearFaultMenu = new MenuItem("Clear existing fault", new EventHandler(this, DeleteResourceFault));
		fileFaultMenu = new MenuItem("Select a file fault");
		menuItems = fileFaultMenu->MenuItems;
		for (int i=0; i<diskFaults->Count; i++)
		{
			Fault *fault = dynamic_cast <Fault*> (diskFaults->Item[i]);
			String *faultName = fault->Name;
			menuItems->Add(new MenuItem (faultName, new EventHandler(this, InjectResourceFault)));
		}
		processFaultMenu = new MenuItem("Select a process fault");
		menuItems = processFaultMenu->MenuItems;
		for (int i=0; i<processFaults->Count; i++)
		{
			Fault *fault = dynamic_cast <Fault*> (processFaults->Item[i]);
			String *faultName = fault->Name;
			menuItems->Add(new MenuItem (faultName, new EventHandler(this, InjectResourceFault)));
		}
		registryFaultMenu = new MenuItem("Select a registry fault");
		menuItems = registryFaultMenu->MenuItems;
		for (int i=0; i<registryFaults->Count; i++)
		{
			Fault *fault = dynamic_cast <Fault*> (registryFaults->Item[i]);
			String *faultName = fault->Name;
			menuItems->Add(new MenuItem (faultName, new EventHandler(this, InjectResourceFault)));
		}
		customFaultMenu = new MenuItem("Select a custom fault");
		menuItems = customFaultMenu->MenuItems;
		for (int i=0; i<customFaults->Count; i++)
		{
			Fault *fault = dynamic_cast <Fault*> (customFaults->Item[i]);
			String *faultName = fault->Name;
			menuItems->Add(new MenuItem (faultName, new EventHandler(this, InjectResourceFault)));
		}


		focusChangedHandler = new FocusChangedDelegate(this, onFocusChanged);
		uiEventRegistry->OnFocusChanged += focusChangedHandler;
		closeProjectUIHandler = new CloseProjectUIDelegate(this, onCloseProjectUI);
		uiEventRegistry->OnCloseProjectUI += closeProjectUIHandler;
		processRemovedHandler = new ProcessRemovedDelegate (this, onProcessRemoved);
		uiEventRegistry->OnProcessRemoved += processRemovedHandler;
		
		resourceTreeView->SelectedIndexChanged += new EventHandler(this, onAfterSelect);
		resourceTreeView->DoubleClick += new EventHandler(this, StartCreateResourceTestWizard);
		resourceTreeView->MouseDown += new MouseEventHandler(this, onRightClick);
	}

	//*************************************************************************
	// Method:		~ResourcePane
	// Description: Destructor for the ResourcePane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourcePane::~ResourcePane()
	{
		uiEventRegistry->OnFocusChanged -= focusChangedHandler;	
		uiEventRegistry->OnCloseProjectUI -= closeProjectUIHandler;
		uiEventRegistry->OnProcessRemoved -= processRemovedHandler;
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
	void ResourcePane::Dispose(bool disposing)
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes all UI components for the class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::InitializeUI()
	{
		resourceTreeView = new VirtualResourceTreeView(false);
		resourceTreeView->Dock = DockStyle::Fill;
		resourceTreeView->BorderStyle = BorderStyle::Fixed3D;
		resourceTreeView->Visible = true;
		resourceTreeView->Height = this->Height;
		this->Controls->Add(resourceTreeView);
	}

	//*************************************************************************
	// Method:		AddDataArray
	// Description: Called when a new test application has been launched
	//
	// Parameters:
	//	data - the data array associated with the new application
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::AddDataArray(ResourcePaneDataArray *data)
	{
		int index = dataArrayList->IndexOfKey(__box(data->ProcessID.GetHashCode()));
		if (index >= 0)
			dataArrayList->SetByIndex(index, data);
		else
			dataArrayList->Add(__box(data->ProcessID.GetHashCode()), data);
	}

	//*************************************************************************
	// Method:		ClearAll
	// Description: Clears the dataArrayList
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::ClearAll()
	{
		dataArrayList->Clear();
	}

	//*************************************************************************
	// Method:		onFocusChanged
	// Description: Called when ui focus changes
	//
	// Parameters:
	//	dataNode - node associated with the focus change
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onFocusChanged(DisplayableDataNode *dataNode)
	{
		if (ignoreFocusChange)
			return;

		if (!dataNode)
		{
			resourceTreeView->Clear();
			return;
		}

		unsigned int processID = 0, localThreadID = 0;

		if ( dataNode && (dataNode->processID != 0) )
		{
			processID = dataNode->processID;
			localThreadID = dataNode->threadID;
		}
		else
			return;

		if ( (!dataArray) || (this->ProcessID != processID) || (localThreadID != this->threadID) )
		{
			this->threadID = localThreadID;
			if (dataArray)
			{
				dataArray->onItemAdded -= itemAddedHandler;
				dataArray->onItemModified -= itemModifiedHandler;
			}

			resourceTreeView->Clear();

			int index = dataArrayList->IndexOfKey(__box(processID.GetHashCode()));
			if (index >= 0)
			{
				dataArray = dynamic_cast <ResourcePaneDataArray*> (dataArrayList->GetByIndex(index));
			
				itemAddedHandler = new ItemAddedDelegate(this, onItemAdded);
				dataArray->onItemAdded += itemAddedHandler;
				itemModifiedHandler = new ItemModifiedDelegate(this, onItemModified);
				dataArray->onItemModified += itemModifiedHandler;
				dataArray->Refresh();
			}
			else
				dataArray = NULL;
		}
	}

	//*************************************************************************
	// Method:		onCloseProjectUI
	// Description: called when project is closing
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ResourcePane::onCloseProjectUI()
	{
		resourceTreeView->Clear();

		// Clear out the data arrays
		if (dataArray)
		{
			dataArray->onItemAdded -= itemAddedHandler;
			dataArray->onItemModified -= itemModifiedHandler;
		}
		dataArray = NULL;
		ClearAll();
	}

	//*************************************************************************
	// Method:		onItemAdded
	// Description: Called when an item is added to the underlying data array
	//
	// Parameters:
	//	dataNode - the data node that was added
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onItemAdded(DisplayableDataNode *dataNode)
	{
		TreeNode *treeNode = NULL;
		ResourcePaneDataNode *resourceDataNode = dynamic_cast <ResourcePaneDataNode*> (dataNode);

		//don't add if not displayable or not of the same thread affiliation as this pane.
		if ( (!resourceDataNode->Displayable) || ((threadID != 0) && (dataNode->threadID != threadID)) )
			return;

		switch (dataNode->Type)
		{
		case UIData::Process:
		case UIData::File:
		case UIData::RegistryEntry:
			{
				treeNode = resourceTreeView->AddNewResource(resourceDataNode->Type, resourceDataNode->Name);
				if (treeNode)
				{
					treeNode->Tag = resourceDataNode;
					resourceDataNode->Element = treeNode;
				}
				break;
			}
		case UIData::ComObject:
			{
				treeNode = resourceTreeView->AddNewResource(resourceDataNode->Type, resourceDataNode->ObjectDescription);
				if (treeNode)
				{
					treeNode->Tag = resourceDataNode;
					resourceDataNode->Element = treeNode;
				}
				break;
			}
		default:
			return;
		}

		if (treeNode && resourceDataNode->HasResourceTest)
			treeNode->ForeColor = Color::Red;

	}

	//*************************************************************************
	// Method:		onItemModified
	// Description: Called when an item is modified in the underlying data array
	//
	// Parameters:
	//	dataNode - the data node that was modified
	//	propertyName - the name of the modified property
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onItemModified(DisplayableDataNode *dataNode, String *propertyName)
	{
		if (!propertyName->Equals("ResourceTest"))
			return;

		ResourcePaneDataNode *resourceDataNode = dynamic_cast <ResourcePaneDataNode*> (dataNode);
		if (!resourceDataNode)
			return;

		TreeNode *treeNode = dynamic_cast <TreeNode*> (resourceDataNode->Element);

		if (!treeNode)
		{
			treeNode = resourceTreeView->GetNodeWithValue(dataNode->Name);
			if (treeNode)
			{
				treeNode->Tag = dataNode;
				dataNode->Element = treeNode;
			}
		}

		if (treeNode && resourceDataNode->HasResourceTest)
			treeNode->ForeColor = Color::Red;
		else if (treeNode && !resourceDataNode->HasResourceTest)
			treeNode->ForeColor = SystemColors::InfoText;


		resourceTreeView->Invalidate();
	}

	//*************************************************************************
	// Method:		onAfterSelect
	// Description: Called when an item is selected in the tree view
	//
	// Parameters:
	//	sender - object who sent this event
	//	e - arguments associated with this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onAfterSelect(Object *sender, EventArgs *e)
	{
		DisplayableDataNode *dataNode;
		selectedNode = resourceTreeView->SelectedNode;
		if (selectedNode && selectedNode->Tag)
		{
			dataNode = new DisplayableDataNode(dynamic_cast <DisplayableDataNode*> (selectedNode->Tag));
			if (dataNode)
			{
				ignoreFocusChange = true;
				uiEventRegistry->OnFocusChanged->Invoke(dataNode);
				ignoreFocusChange = false;
			}
		}	
	}

	//*************************************************************************
	// Method:		onRightClick
	// Description: Called when an item is right clicked in the list view
	//
	// Parameters:
	//	sender - object who sent this event
	//	e - arguments associated with this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onRightClick(Object *sender, MouseEventArgs *e)
	{
		//bugbug - can have nodes colored red but with no datanode associated, need to handle this case
		if (e->Button == MouseButtons::Right)
		{
			selectedNode = resourceTreeView->NodeAtMousePos(e);
			resourceTreeView->ContextMenu->MenuItems->Remove(selectLogItemMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(createFileCorruptionMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(createFaultMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(modifyFaultMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(clearFaultMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(fileFaultMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(processFaultMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(registryFaultMenu);
			resourceTreeView->ContextMenu->MenuItems->Remove(customFaultMenu);
			
			if (selectedNode && (!resourceTreeView->IsSpecialNode(selectedNode)))
			{
				if (selectedNode->Tag)
				{
					ResourcePaneDataNode *resourceNode = dynamic_cast <ResourcePaneDataNode*> (selectedNode->Tag);
					if (resourceNode && (resourceNode->FilePosition != -1) )
						resourceTreeView->ContextMenu->MenuItems->Add(selectLogItemMenu);
				}

				FaultType faultType = resourceTreeView->GetFaultType(selectedNode);
				switch (faultType)
				{
				case FaultType::DiskFault:
					{
						resourceTreeView->ContextMenu->MenuItems->Add(createFileCorruptionMenu);
						resourceTreeView->ContextMenu->MenuItems->Add(createFaultMenu);
						if (selectedNode->ForeColor == Color::Red)
						{
							resourceTreeView->ContextMenu->MenuItems->Add(modifyFaultMenu);
							resourceTreeView->ContextMenu->MenuItems->Add(clearFaultMenu);
						}
						resourceTreeView->ContextMenu->MenuItems->Add(fileFaultMenu);
						resourceTreeView->ContextMenu->MenuItems->Add(customFaultMenu);
						break;
					}
				case FaultType::ProcessFault:
					{
						resourceTreeView->ContextMenu->MenuItems->Add(createFaultMenu);
						if (selectedNode->ForeColor == Color::Red)
						{
							resourceTreeView->ContextMenu->MenuItems->Add(modifyFaultMenu);
							resourceTreeView->ContextMenu->MenuItems->Add(clearFaultMenu);
						}
						resourceTreeView->ContextMenu->MenuItems->Add(processFaultMenu);
						resourceTreeView->ContextMenu->MenuItems->Add(customFaultMenu);
						break;
					}
				case FaultType::RegistryFault:
					{
						resourceTreeView->ContextMenu->MenuItems->Add(createFaultMenu);
						if (selectedNode->ForeColor == Color::Red)
						{
							resourceTreeView->ContextMenu->MenuItems->Add(modifyFaultMenu);
							resourceTreeView->ContextMenu->MenuItems->Add(clearFaultMenu);
						}
						resourceTreeView->ContextMenu->MenuItems->Add(registryFaultMenu);
						resourceTreeView->ContextMenu->MenuItems->Add(customFaultMenu);
						break;
					}
				case FaultType::CustomFault:
					{
						resourceTreeView->ContextMenu->MenuItems->Add(createFaultMenu);
						if (selectedNode->ForeColor == Color::Red)
						{
							resourceTreeView->ContextMenu->MenuItems->Add(modifyFaultMenu);
							resourceTreeView->ContextMenu->MenuItems->Add(clearFaultMenu);
						}
						resourceTreeView->ContextMenu->MenuItems->Add(customFaultMenu);
						break;
					}
				}
			}
			else if (selectedNode && (resourceTreeView->ContextMenu->MenuItems->Count == 0) )
				resourceTreeView->ContextMenu->MenuItems->Add(createFaultMenu);
		}
	}	
	
	//*************************************************************************
	// Method:		onCopyToClipboard
	// Description: called when copy menu item is selected
	//
	// Parameters:
	//	sender - object who sent this event
	//	e - arguments associated with this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onCopyToClipboard(Object *sender, EventArgs *e)
	{
		if (uiEventRegistry->OnCopyToClipboard)
			uiEventRegistry->OnCopyToClipboard->Invoke();
	}

	//*************************************************************************
	// Method:		InjectResourceFault
	// Description: Creates a resource fault based on the currently selected node
	//
	// Parameters:
	//	sender - object who sent this event
	//	e - arguments associated with this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::InjectResourceFault(Object *sender, EventArgs *e)
	{
		String *resourcePath;
		MenuItem *menuItem = dynamic_cast <MenuItem*> (sender);
		TreeNode *treeNode = resourceTreeView->SelectedNode;
		if (!treeNode)
			return;

		DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (treeNode->Tag);
		//if a com object get the clsid not the converted name
		if (dataNode && (dataNode->Type == ComObject))
			resourcePath = dataNode->Name;
		else
			resourcePath = resourceTreeView->BuildResourcePath(treeNode, true);

		ResourceTestDataNode *resourceTestDataNode = new ResourceTestDataNode(resourcePath, menuItem->Text, resourceTreeView->GetFaultType(treeNode), dataArray->ProcessID, 0);

		dataArray->BeginCreateNode(resourceTestDataNode);
	}


	//*************************************************************************
	// Method:		DeleteResourceFault
	// Description: Deletes a resource fault based on the currently selected node
	//
	// Parameters:
	//	sender - object who sent this event
	//	e - arguments associated with this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::DeleteResourceFault(Object *sender, EventArgs *e)
	{
		ResourceTestDataNode *resourceTestDataNode;
		ResourcePaneDataNode *resourcePaneDataNode;
		TreeNode *treeNode = resourceTreeView->SelectedNode;
		if (!treeNode)
			return;
		
		resourcePaneDataNode = dynamic_cast <ResourcePaneDataNode*> (treeNode->Tag);
		if (!resourcePaneDataNode)
			return;
		
		resourceTestDataNode = dynamic_cast <ResourceTestDataNode*> (resourcePaneDataNode->ResourceTestNode);

		if (!resourceTestDataNode)
			return;

		dataArray->BeginDeleteNode(resourceTestDataNode);
	}

	//*************************************************************************
	// Method:		StartCreateFileCorruptionWizard
	// Description: starts the file corruption wizard with information from the currently selected node
	//
	// Parameters:
	//	standard event messages
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::StartCreateFileCorruptionWizard(Object *sender, EventArgs *e)
	{
		FileCorruptionDataNode *fileCorruptionDataNode;
		TreeNode *treeNode = resourceTreeView->SelectedNode;

		if (treeNode && (!resourceTreeView->IsSpecialNode(treeNode)))
		{
            fileCorruptionDataNode = new FileCorruptionDataNode(resourceTreeView->BuildResourcePath(treeNode, true), this->ProcessID);
			fileCorruptionDataNode->OriginalFile = fileCorruptionDataNode->Name;
		}
		else
			fileCorruptionDataNode = new FileCorruptionDataNode("", this->ProcessID);

		uiEventRegistry->OnStartFileCorruptionWizard(fileCorruptionDataNode);
	}

	//*************************************************************************
	// Method:		StartCreateResourceTestWizard
	// Description: starts the resource test wizard with information from the currently selected node
	//
	// Parameters:
	//	standard event messages
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::StartCreateResourceTestWizard(Object *sender, EventArgs *e)
	{
		bool modify = (sender == modifyFaultMenu);

		StartResourceTestWizard(resourceTreeView->SelectedNode, modify);
	}

	//*************************************************************************
	// Method:		StartResourceTestWizard
	// Description: starts the resource test wizard with information from the currently selected node
	//
	// Parameters:
	//	treeNode - treeNode that represents the resource to create a test for
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::StartResourceTestWizard(TreeNode *treeNode, bool modify)
	{
		ResourcePaneDataNode *resourcePaneDataNode;
		ResourceTestDataNode *resourceTestDataNode;

		if (treeNode)
		{
			resourcePaneDataNode = dynamic_cast <ResourcePaneDataNode*> (treeNode->Tag);
			if (resourcePaneDataNode && resourcePaneDataNode->ResourceTestNode)
				resourceTestDataNode = dynamic_cast <ResourceTestDataNode*> (resourcePaneDataNode->ResourceTestNode);
			else
				resourceTestDataNode = new ResourceTestDataNode(resourceTreeView->BuildResourcePath(treeNode, true), "", resourceTreeView->GetFaultType(treeNode), this->ProcessID, 0);
		}
		else
			resourceTestDataNode = new ResourceTestDataNode("", "", FaultType::CustomFault, this->ProcessID, 0);


		if (modify)
			uiEventRegistry->OnStartModifyResourceTestWizard(resourceTestDataNode);
		else
			uiEventRegistry->OnStartCreateResourceTestWizard(resourceTestDataNode);
	}

	//*************************************************************************
	// Method:		onProcessRemoved
	// Description: Refreshes the UI after a process is removed from the project
	//
	// Parameters:
	//	processID - the pid of the process which was removed
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onProcessRemoved(unsigned int processID)
	{
		//clear the pane ui
		resourceTreeView->Clear();

		//clear the data array out of the data array list
		int index = dataArrayList->IndexOfKey(__box(processID.GetHashCode()));
		if (index >= 0)
		{
			//if the removed process was the current, set current to NULL
			if (dataArray && (dataArray == dynamic_cast <ResourcePaneDataArray*> (dataArrayList->GetByIndex(index))))
			{
				dataArray->onItemAdded -= itemAddedHandler;
				dataArray->onItemModified -= itemModifiedHandler;
				dataArray = NULL;
			}

			dataArrayList->RemoveAt(index);
		}

		//refresh the pane ui
		this->Refresh();
	}

	//*************************************************************************
	// Method:		GetSelectedNode
	// Description: Retrieves the selected node from the pane
	//
	// Parameters:
	//	None
	//
	// Return Value: The selected ResourcePaneDataNode
	//*************************************************************************
	ResourcePaneDataNode* ResourcePane::GetSelectedNode()
	{
		TreeNode *selectedTreeNode = NULL;
		
		if (resourceTreeView->ContainsFocus)
			selectedTreeNode = resourceTreeView->SelectedNode;

		if (selectedTreeNode && selectedTreeNode->Tag)
		{
			ResourcePaneDataNode* returnNode  = dynamic_cast <ResourcePaneDataNode*> (selectedTreeNode->Tag);
			if (returnNode)
				return returnNode;
		}

		return NULL;
	}

	//*************************************************************************
	// Method:		SelectLogEntry
	// Description: selects the log pane entry that matches to this node
	//
	// Parameters:
	//	Standard event messages
	//
	// Return Value: none
	//*************************************************************************
	void ResourcePane::SelectLogEntry(Object *sender, EventArgs *e)
	{
		ResourcePaneDataNode *resourceNode;
		unsigned long filePos;

		if (selectedNode)
			resourceNode = dynamic_cast <ResourcePaneDataNode*> (selectedNode->Tag);

		if (resourceNode)
		{
			filePos = resourceNode->FilePosition;

			// First ensure the correct log pane is visible
			HoloScriptApplication* holoScriptApp = HoloScriptRegistry::getInstance()->
				getHoloScriptAppForProcess(resourceNode->processID);
			DisplayableDataNode *dataNode = new DisplayableDataNode(
				holoScriptApp ? holoScriptApp->ApplicationName : "", NodeType::ActiveLogPane,
				resourceNode->processID, 0);
			if (uiEventRegistry->OnDoubleClick)
				uiEventRegistry->OnDoubleClick->Invoke(dataNode);

			// Select the item in this log pane
			if (uiEventRegistry->OnSelectLogItem)
				uiEventRegistry->OnSelectLogItem->Invoke(resourceNode->processID, filePos);
		}
	}

	//*************************************************************************
	// Method:		GetSelectedNodeText
	// Description: Returns the text from the selected node
	//
	// Parameters:
	//	None
	//
	// Return Value: The text from the selected node
	//*************************************************************************
	String * ResourcePane::GetSelectedNodeText()
	{
		TreeNode *selectedTreeNode = NULL;
		
		if (resourceTreeView->ContainsFocus)
			selectedTreeNode = resourceTreeView->SelectedNode;

		if (selectedTreeNode)
			return selectedTreeNode->Text;
		else
			return NULL;
	}

	//*************************************************************************
	// Method:		onExportToFile
	// Description: called when the user wants to export the log to a file
	//
	// Parameters:
	//	sender - the sender of this event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePane::onExportToFile(Object *sender, EventArgs *args)
	{
		if (dataEventRegistry->OnSaveResourcePane)
			dataEventRegistry->OnSaveResourcePane->Invoke();
	}
}