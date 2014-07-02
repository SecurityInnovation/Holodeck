//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DisplayableDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class DisplayableDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include "DisplayableDataArray.h"
#include "UserNotification.h"

namespace UIData
{

	//*************************************************************************
	// Method:		DisplayableDataArray
	// Description: Constructor for the DisplayableDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DisplayableDataArray::DisplayableDataArray()
	{
		itemList = new SortedList();
		dataEventRegistry = dataEventRegistry->getInstance();

		NodeModifiedHandler = new NodeModifiedDelegate(this, OnNodeModified);
		CloseProjectHandler = new CloseProjectDelegate(this, OnCloseProject);
		dataEventRegistry->OnCloseProject += CloseProjectHandler;

		idIndex = 1000;
		processID = 0;
		threadID = 0;
	}

	//*************************************************************************
	// Method:		~DisplayableDataArray
	// Description: Destructor for the DisplayableDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	DisplayableDataArray::~DisplayableDataArray()
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
	void DisplayableDataArray::CleanUp()
	{
		dataEventRegistry->OnCloseProject -= CloseProjectHandler;
	}


	//*************************************************************************
	// Method:		BeginDeleteNode
	// Description: Starts the chain of messages that will delete the data node in each data class
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: true if succesful, false if not
	//*************************************************************************
	bool DisplayableDataArray::BeginDeleteNode(DisplayableDataNode *dataNode)
	{
		dataNode->UpdateDelegates();
		if (!dataNode->DeleteDelegate)
			return false;
		else
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeleteDelegate->DynamicInvoke(args);
		}

		return true;
	}

	//*************************************************************************
	// Method:		BeginCreateNode
	// Description: Starts the chain of messages that will create the data node in each data class
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: true if succesful, false if not
	//*************************************************************************
	bool DisplayableDataArray::BeginCreateNode(DisplayableDataNode *dataNode)
	{
		dataNode->UpdateDelegates();
		if (!dataNode->CreateDelegate)
			return false;
		else
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->CreateDelegate->DynamicInvoke(args);
		}

		return true;
	}

	//*************************************************************************
	// Method:		BeginModifyNode
	// Description: Starts the chain of messages that will modify the data node in each data class
	//
	// Parameters:
	//	dataNode - the node to be modified
	//	modifiedDataNode - the node containing a modified property
	//	propertyName - name of the property that was modified
	//
	// Return Value: true if succesful, false if not
	//*************************************************************************
	bool DisplayableDataArray::BeginModifyNode(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		dataNode->UpdateDelegates();
		if (!dataNode->ModifyDelegate)
			return false;
		else
		{
			Object *args[] = __gc new Object*[3];
			args[0] = dataNode;
			args[1] = modifiedDataNode;
			args[2] = propertyName;
			dataNode->ModifyDelegate->DynamicInvoke(args);
		}

		return true;
	}

	//*************************************************************************
	// Method:		SaveNode
	// Description: Called by ui class when it wants to save a node
	//
	// Parameters:
	//	dataNode - node to save
	//
	// Return Value: None
	//*************************************************************************
	bool DisplayableDataArray::SaveNode(DisplayableDataNode *dataNode)
	{
		dataNode->UpdateDelegates();
		if (!dataNode->SaveDelegate)
			return false;
		else
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->SaveDelegate->DynamicInvoke(args);
		}

		return true;
	}

	//*************************************************************************
	// Method:		CreateDataNode
	// Description: Adds an item to the array
	//
	// Parameters:
	//	dataNode - the data node item to add
	//
	// Return Value: true if added, false if not
	//*************************************************************************
	void DisplayableDataArray::CreateDataNode(DisplayableDataNode *dataNode)
	{
		try
		{
			dataNode->UpdateDelegates();
			if (dataNode->ID->Equals(""))
				dataNode->ID = __box(idIndex++)->ToString();

			if (!itemList->Contains(dataNode->ID))
			{
				dataNode->onNodeModified += NodeModifiedHandler;
				itemList->Add(dataNode->ID, dataNode);
				if (this->onItemAdded)
					onItemAdded->Invoke(dataNode);
			}
		}
		catch (...)
		{
			return;
		}
	}

	//*************************************************************************
	// Method:		DeleteDataNode
	// Description: event handler for all deleted events.  When this is recieved we are ready to delete the item
	//	from our list.
	//
	// Parameters:
	//	dataNode - node that will be deleted
	//
	// Return Value: true if succesfull, false if not
	//*************************************************************************
	void DisplayableDataArray::DeleteDataNode(DisplayableDataNode *dataNode)
	{
		DisplayableDataNode *deleteNode;

		try
		{
			for (int i=0; i<itemList->Count; i++)
			{
				deleteNode = dynamic_cast <DisplayableDataNode*> (itemList->GetByIndex(i));
				deleteNode->UpdateDelegates();
				if (((dataNode->ID->Equals(deleteNode->ID)) || (dataNode->ID->Equals(""))) && (dataNode->Type == deleteNode->Type) && (dataNode->processID == deleteNode->processID) && (dataNode->threadID == deleteNode->threadID))
				{
					itemList->Remove(deleteNode->ID);
					if (this->onItemDeleted)
						this->onItemDeleted->Invoke(deleteNode);
					break;
				}
			}
		}
		catch(...)
		{
			return;
		}
	}
	
	//*************************************************************************
	// Method:		ModifyDataNode
	// Description: event handler for all modify events.  When this is recieved we are ready to modify the item
	//	in our list.
	//
	// Parameters:
	//	dataNode - node that will be modified
	//	modifiedDataNode - node that contains the modification
	//	propertyName - name of the property that was modified
	//
	// Return Value: none
	//*************************************************************************
	void DisplayableDataArray::ModifyDataNode(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		DisplayableDataNode *listDataNode = GetDataNodeByID(dataNode->ID);
		
		if (propertyName->CompareTo("ID") == 0)
		{
			//if the key already exists exit.
			if (itemList->ContainsKey(modifiedDataNode->ID))
			{
				String *msg = "Internal Holodeck Error: '";
				msg = String::Concat(msg, modifiedDataNode->ID);
				msg = String::Concat(msg, "' cannot be added twice to the data array.");
				HolodeckGui::UserNotification::ErrorNotify(msg);
				return;
			}

			//since id is the key we need to re-insert
			itemList->Remove(listDataNode->ID);
			listDataNode->set_ID(modifiedDataNode->get_ID());
			itemList->Add(listDataNode->ID, listDataNode);
		}
		else if (propertyName->CompareTo("Element") == 0)
		{
			listDataNode->set_Element(modifiedDataNode->get_Element());
		}
		else if (propertyName->CompareTo("Type") == 0)
		{
			listDataNode->set_Type(modifiedDataNode->get_Type());
		}
		else if (propertyName->CompareTo("Name") == 0)
		{
			listDataNode->set_Name(modifiedDataNode->get_Name());
		}
		else if (propertyName->CompareTo("ProcessID") == 0)
		{
			listDataNode->set_processID(modifiedDataNode->get_processID());
		}
		else if (propertyName->CompareTo("ThreadID") == 0)
		{
			listDataNode->set_threadID(modifiedDataNode->get_threadID());
		}
	}


	//*************************************************************************
	// Method:		GetDataNode
	// Description: Gets a node from the array
	//
	// Parameters:
	//	Index - the index of the data node item to get
	//
	// Return Value: pointer to the data node retrieved
	//*************************************************************************
	DisplayableDataNode *DisplayableDataArray::GetDataNodeByIndex(int Index)
	{
		DisplayableDataNode *Item;
		try
		{
			Item = dynamic_cast <DisplayableDataNode*> (itemList->GetByIndex(Index));
			return Item;
		}
		catch(System::ArgumentOutOfRangeException*)
		{
			return 0;
		}
	}

	//*************************************************************************
	// Method:		GetDataNodeByID
	// Description: Gets a node from the array based on the node ID
	//
	// Parameters:
	//	ID - the ID of the data node item to get
	//
	// Return Value: pointer to the data node retrieved
	//*************************************************************************
	DisplayableDataNode *DisplayableDataArray::GetDataNodeByID(String* ID)
	{
		DisplayableDataNode *Item;
		int index;

		try
		{
			index = itemList->IndexOfKey(ID);
			Item = dynamic_cast <DisplayableDataNode*> (itemList->GetByIndex(index));
			return Item;
		}
		catch(System::ArgumentOutOfRangeException*)
		{
			return 0;
		}
	}

	//*************************************************************************
	// Method:		OnNodeModified
	// Description: Event handler for when a property on an item in the array is modified
	//
	// Parameters:
	//	Index - the index of the data node item to that was modified
	//	propertyName - the name of the property that was modified
	//
	// Return Value: none
	//*************************************************************************
	void DisplayableDataArray::OnNodeModified(DisplayableDataNode *Node, String *propertyName)
	{
		if (this->onItemModified)
			this->onItemModified->Invoke(Node, propertyName);
	}

	//*************************************************************************
	// Method:		OnCloseProject
	// Description: Event handler for when the project is closed
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void DisplayableDataArray::OnCloseProject()
	{
		while (itemList->Count > 0)
			itemList->RemoveAt(0);
	}

	//*************************************************************************
	// Method:		Refresh
	// Description: resend itemadded messages for each node in the list
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void DisplayableDataArray::Refresh()
	{
		if (onItemAdded)
		{
			for (int i=0; i<itemList->Count; i++)
			{
				onItemAdded->Invoke(dynamic_cast <DisplayableDataNode*> (itemList->GetByIndex(i)));
			}
		}
	}

	//*************************************************************************
	// Method:		Clear
	// Description: Deletes each node in the list
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void DisplayableDataArray::Clear()
	{
		itemList->Clear();
	}
}

