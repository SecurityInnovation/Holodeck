//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionPortSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkCorruptionPortSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "NetworkCorruptionPortSelectionPage.h"
#include "defines.h"
#include "..\UserNotification.h"
#include <stdio.h>

#using <GuiSupport.dll>

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkCorruptionPortSelectionPage
	// Description: Constructor for the NetworkCorruptionPortSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionPortSelectionPage::NetworkCorruptionPortSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select which data should be fuzzed.";

		receiveOnly = new RadioButton();
		receiveOnly->Text = "Fuzz data being received over the network";
		receiveOnly->Location = System::Drawing::Point(8, 12);
		receiveOnly->Size = System::Drawing::Size(360, 16);
		receiveOnly->Checked = true;

		sendOnly = new RadioButton();
		sendOnly->Text = "Fuzz data being sent over the network";
		sendOnly->Location = System::Drawing::Point(8, 36);
		sendOnly->Size = System::Drawing::Size(360, 16);

		receiveAndSend = new RadioButton();
		receiveAndSend->Text = "Fuzz both sent and received data";
		receiveAndSend->Location = System::Drawing::Point(8, 60);
		receiveAndSend->Size = System::Drawing::Size(360, 16);

		Label* excludeLabel = new Label();
		excludeLabel->Text = "Network ports not included for fuzzing:";
		excludeLabel->Location = System::Drawing::Point(8, 90);
		excludeLabel->Size = System::Drawing::Size(140, 32);

		Label* includeLabel = new Label();
		includeLabel->Text = "Network ports to be included for fuzzing:";
		includeLabel->Location = System::Drawing::Point(245, 90);
		includeLabel->Size = System::Drawing::Size(140, 32);

		ToggleColumnHeader* excludedPortNumHeader = new ToggleColumnHeader();
		excludedPortNumHeader->Text = "Port";
		excludedPortNumHeader->Width = 48;

		ToggleColumnHeader* excludedPortNameHeader = new ToggleColumnHeader();
		excludedPortNameHeader->Text = "Protocol";
		excludedPortNameHeader->Width = 64;

		excluded = new ContainerListView();
		excluded->MultiSelect = true;
		excluded->Location = System::Drawing::Point(8, 122);
		excluded->Size = System::Drawing::Size(140, 108);
		excluded->DoubleClick += new EventHandler(this, onAddClicked);
		excluded->SetListViewItemSelected += new SetListViewItemSelectedDelegate(this, onExcludedSetSelected);
		excluded->SetListViewItemFocused += new SetListViewItemFocusedDelegate(this, onExcludedSetFocused);
		excluded->Items->QueryListViewItem += new QueryContainerListViewItemDelegate(this, onExcludedQueryItem);
		excluded->Items->QueryListViewItemIndex += new QueryContainerListViewItemIndexDelegate(this, onExcludedQueryItemIndex);
		excluded->Columns->Add(excludedPortNumHeader);
		excluded->Columns->Add(excludedPortNameHeader);

		ToggleColumnHeader* includedPortNumHeader = new ToggleColumnHeader();
		includedPortNumHeader->Text = "Port";
		includedPortNumHeader->Width = 48;

		ToggleColumnHeader* includedPortNameHeader = new ToggleColumnHeader();
		includedPortNameHeader->Text = "Protocol";
		includedPortNameHeader->Width = 64;

		included = new ContainerListView();
		excluded->MultiSelect = true;
		included->Location = System::Drawing::Point(245, 122);
		included->Size = System::Drawing::Size(140, 108);
		included->DoubleClick += new EventHandler(this, onRemoveClicked);
		included->SetListViewItemSelected += new SetListViewItemSelectedDelegate(this, onIncludedSetSelected);
		included->SetListViewItemFocused += new SetListViewItemFocusedDelegate(this, onIncludedSetFocused);
		included->Items->QueryListViewItem += new QueryContainerListViewItemDelegate(this, onIncludedQueryItem);
		included->Items->QueryListViewItemIndex += new QueryContainerListViewItemIndexDelegate(this, onIncludedQueryItemIndex);
		included->Columns->Add(includedPortNumHeader);
		included->Columns->Add(includedPortNameHeader);

		addButton = new Button();
		addButton->Text = "&Add ->";
		addButton->Location = System::Drawing::Point(158, 122);
		addButton->Size = System::Drawing::Size(77, 24);
		addButton->Click += new EventHandler(this, onAddClicked);

		removeButton = new Button();
		removeButton->Text = "<- &Remove";
		removeButton->Location = System::Drawing::Point(158, 150);
		removeButton->Size = System::Drawing::Size(77, 24);
		removeButton->Click += new EventHandler(this, onRemoveClicked);

		addAllButton = new Button();
		addAllButton->Text = "A&dd All";
		addAllButton->Location = System::Drawing::Point(158, 178);
		addAllButton->Size = System::Drawing::Size(77, 24);
		addAllButton->Click += new EventHandler(this, onAddAllClicked);

		removeAllButton = new Button();
		removeAllButton->Text = "R&emove All";
		removeAllButton->Location = System::Drawing::Point(158, 206);
		removeAllButton->Size = System::Drawing::Size(77, 24);
		removeAllButton->Click += new EventHandler(this, onRemoveAllClicked);

		// Load the network port database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		portDB = PortDB::GetInstance();
		if (!portDB->IsLoaded)
		{
			try
			{
				portDB->LoadDatabase(functionFolder);
			}
			catch (Exception* e)
			{
				UserNotification::ErrorNotify(e->Message);
			}
		}

		excludedPorts = new ArrayList();
		includedPorts = new ArrayList();
		for (int i=1; i<=65534; i++) // Ports 1 to 65534 are valid
		{
			PortNode* node = new PortNode();
			node->port = i;
			node->selected = false;
			node->focused = false;
			includedPorts->Add(node);
		}

		// Fill in the port list boxes
		UpdateListBoxItemCounts();

		this->Controls->Add(receiveOnly);
		this->Controls->Add(sendOnly);
		this->Controls->Add(receiveAndSend);
		this->Controls->Add(excludeLabel);
		this->Controls->Add(includeLabel);
		this->Controls->Add(excluded);
		this->Controls->Add(included);
		this->Controls->Add(addButton);
		this->Controls->Add(removeButton);
		this->Controls->Add(addAllButton);
		this->Controls->Add(removeAllButton);
	}

	//*************************************************************************
	// Method:		~NetworkCorruptionPortSelectionPage
	// Description: Destructor for the NetworkCorruptionPortSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkCorruptionPortSelectionPage::~NetworkCorruptionPortSelectionPage()
	{
	}

	//*************************************************************************
	// Method:		UpdateListBoxItemCounts
	// Description: Updates the item counts for the 2 list views
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::UpdateListBoxItemCounts()
	{
		included->ItemCount = includedPorts->Count;
		excluded->ItemCount = excludedPorts->Count;

		included->Invalidate();
		excluded->Invalidate();
	}

	
	//*************************************************************************
	// Method:		get_PortList
	// Description: Gets the list of ports to be corrupted
	//
	// Parameters:
	//	None
	//
	// Return Value: List of ports
	//*************************************************************************
	ArrayList *NetworkCorruptionPortSelectionPage::get_PortList()
	{
		ArrayList* result = new ArrayList();
		for (int i=0; i<includedPorts->Count; i++)
		{
			PortNode* node = dynamic_cast<PortNode*>(includedPorts->Item[i]);
			result->Add(__box(node->port));
		}
		return result;
	}


	//*************************************************************************
	// Method:		set_PortList
	// Description: Sets the list of ports to be corrupted
	//
	// Parameters:
	//	value - List of ports to corrupt
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::set_PortList(ArrayList *value)
	{
		bool portIncluded __gc[] = new bool __gc[65536];

		// Set the included port boolean array based on the list in value
		for (int i=0; i<65536; i++)
			portIncluded[i] = false;
		for (int i=0; i<value->Count; i++)
			portIncluded[*dynamic_cast<__box int*>(value->Item[i])] = true;

		// Reconstruct the included and excluded list boxes from this boolean array
		includedPorts->Clear();
		excludedPorts->Clear();
		for (int i=1; i<=65534; i++) // Ports 1 to 65534 are valid
		{
			PortNode* node = new PortNode();
			node->port = i;
			node->selected = false;
			node->focused = false;

			if (portIncluded[i])
				includedPorts->Add(node);
			else
				excludedPorts->Add(node);
		}
		UpdateListBoxItemCounts();
	}


	//*************************************************************************
	// Method:		get_SentData
	// Description: Determines if sent data should be corrupted
	//
	// Parameters:
	//	None
	//
	// Return Value: true if sent data should be corrupted
	//*************************************************************************
	bool NetworkCorruptionPortSelectionPage::get_SentData()
	{
		if (sendOnly->Checked)
			return true;
		if (receiveAndSend->Checked)
			return true;
		return false;
	}


	//*************************************************************************
	// Method:		get_ReceivedData
	// Description: Determines if received data should be corrupted
	//
	// Parameters:
	//	None
	//
	// Return Value: true if received data should be corrupted
	//*************************************************************************
	bool NetworkCorruptionPortSelectionPage::get_ReceivedData()
	{
		if (receiveOnly->Checked)
			return true;
		if (receiveAndSend->Checked)
			return true;
		return false;
	}

	
	//*************************************************************************
	// Method:		SetSentAndReceived
	// Description: Sets if sent data, received data, or both should be corrupted
	//
	// Parameters:
	//	sent - true if sent data should be corrupted
	//  received - true if received data should be corrupted
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::SetSentAndReceived(bool sent, bool received)
	{
		if (sent && received)
		{
			receiveAndSend->Checked = true;
			receiveOnly->Checked = false;
			sendOnly->Checked = false;
		}
		else if (sent)
		{
			sendOnly->Checked = true;
			receiveAndSend->Checked = false;
			receiveOnly->Checked = false;
		}
		else
		{
			receiveOnly->Checked = true;
			receiveAndSend->Checked = false;
			sendOnly->Checked = false;
		}
	}

	
	//*************************************************************************
	// Method:		AddPortToList
	// Description: Adds a port to a port list box, keeping the list sorted
	//
	// Parameters:
	//	list - List box to add to
	//  port - Port to add
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::AddPortToList(ArrayList* list, int insertPortNum)
	{
		int i = 0;
		if (list->Count > 0) // Only search if there are actually elements in the list
		{
			// Place the port number in the correct location to leave the list sorted.  Use a
			// binary search to find the location fast.
			int min, max;
			min = 0;
			max = list->Count - 1;
			for (i = (min + max) / 2; min != max; i = (min + max) / 2)
			{
				PortNode* curNode = dynamic_cast<PortNode*>(list->Item[i]);

				if (curNode->port > insertPortNum)
				{
					// Past where we want to be
					if (max == i)
						break; // Didn't update anything, we're about to loop indefinitely
					max = i;
					continue;
				}
				// Our spot is later on in the list
				if (min == i)
					break; // Didn't update anything, we're about to loop indefinitely
				min = i;
			}

			// i is now approximately where we need to be (it won't be exact as we aren't going to find our
			// port already in the list, our location depends on which side we came from).  Check the element
			// right before min just in case we got placed just after the ideal insertion location
			i = min - 1;
			if (i < 0)
				i = 0;
			for (; i<list->Count; i++)
			{
				PortNode* curNode = dynamic_cast<PortNode*>(list->Item[i]);
				if (curNode->port > insertPortNum)
				{
					// We need to insert right here, break out now
					break;
				}
			}
		}

		PortNode* node = new PortNode();
		node->port = insertPortNum;
		node->selected = false;
		node->focused = false;
		list->Insert(i, node);
	}


	//*************************************************************************
	// Method:		onAddClicked
	// Description: Called when the "Add" button is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onAddClicked(Object *sender, EventArgs *e)
	{
		for (int i=0; i<excludedPorts->Count; i++)
		{
			PortNode* node = dynamic_cast<PortNode*>(excludedPorts->Item[i]);
			if (node->selected)
			{
				AddPortToList(includedPorts, node->port);
				excludedPorts->RemoveAt(i);
				i--;
			}
		}
		UpdateListBoxItemCounts();
	}


	//*************************************************************************
	// Method:		onRemoveClicked
	// Description: Called when the "Remove" button is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onRemoveClicked(Object *sender, EventArgs *e)
	{
		for (int i=0; i<includedPorts->Count; i++)
		{
			PortNode* node = dynamic_cast<PortNode*>(includedPorts->Item[i]);
			if (node->selected)
			{
				AddPortToList(excludedPorts, node->port);
				includedPorts->RemoveAt(i);
				i--;
			}
		}
		UpdateListBoxItemCounts();
	}


	//*************************************************************************
	// Method:		onAddAllClicked
	// Description: Called when the "Add All" button is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onAddAllClicked(Object *sender, EventArgs *e)
	{
		excludedPorts->Clear();
		includedPorts->Clear();
		for (int i=1; i<=65534; i++) // Ports 1 to 65534 are valid
		{
			PortNode* node = new PortNode();
			node->port = i;
			node->selected = false;
			node->focused = false;
			includedPorts->Add(node);
		}
		UpdateListBoxItemCounts();
	}


	//*************************************************************************
	// Method:		onRemoveAllClicked
	// Description: Called when the "Remove All" button is clicked
	//
	// Parameters:
	//	sender - Object that triggered the event
	//  e - Event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onRemoveAllClicked(Object *sender, EventArgs *e)
	{
		excludedPorts->Clear();
		includedPorts->Clear();
		for (int i=1; i<=65534; i++) // Ports 1 to 65534 are valid
		{
			PortNode* node = new PortNode();
			node->port = i;
			node->selected = false;
			node->focused = false;
			excludedPorts->Add(node);
		}
		UpdateListBoxItemCounts();
	}


	//*************************************************************************
	// Method:		get_PortString
	// Description: Returns the ports selected as a readable string
	//
	// Parameters:
	//	None
	//
	// Return Value: String representing the selected ports
	//*************************************************************************
	String* NetworkCorruptionPortSelectionPage::get_PortString()
	{
		// Convert the port list to a readable string
		bool portIncluded __gc[] = new bool __gc[65536];
		for (int i=0; i<65536; i++)
			portIncluded[i] = 0;
		ArrayList* portList = this->PortList;
		for (int i=0; i<portList->Count; i++)
			portIncluded[*dynamic_cast<Int32*>(portList->Item[i])] = true;

		String *portStr;

		if (SentData && ReceivedData)
			portStr = "Send and Recv on ";
		else if (SentData)
			portStr = "Send on ";
		else
			portStr = "Recv on ";

		bool enumerateExcludes = false;

		if (portList->Count == 65534) // All possible ports
			return String::Concat(portStr, "All");
		else if (portList->Count > 32768) // More than half
		{
			portStr = String::Concat(portStr, "All Except ");
			enumerateExcludes = true;
		}

		int sequenceStart = 1;
		int sequenceEnd = 0;
		bool firstSeq = true;
		for (int i=1; i<=65534; i++) // Ports 1 to 65534 are valid
		{
			if (((!enumerateExcludes) && (!portIncluded[i])) ||
				(enumerateExcludes && portIncluded[i]))
			{
				// Not included in list.  Add the past included sequence to the string
				if (sequenceStart > sequenceEnd)
				{
					// Not a valid sequence, this is a run of excluded ports
					sequenceStart = i + 1; // Next sequence will start at next port, if it's included
					continue;
				}

				if (!firstSeq)
					portStr = String::Concat(portStr, S", ");
				firstSeq = false;

				if (sequenceStart == sequenceEnd)
				{
					// Single port
					portStr = String::Concat(portStr, __box(sequenceStart)->ToString());
				}
				else
				{
					// Multiple ports
					portStr = String::Concat(portStr, __box(sequenceStart)->ToString(), S"-", __box(sequenceEnd)->ToString());
				}

				sequenceStart = i + 1; // Next sequence will start at next port, if it's included
				continue;
			}
			else
				sequenceEnd = i;
		}
		if (sequenceStart <= sequenceEnd)
		{
			// Add the last sequence to the string
			if (!firstSeq)
				portStr = portStr->Concat(S", ");
			firstSeq = false;

			if (sequenceStart == sequenceEnd)
			{
				// Single port
				portStr = String::Concat(portStr, __box(sequenceStart)->ToString());
			}
			else
			{
				// Multiple ports
				portStr = String::Concat(portStr, __box(sequenceStart)->ToString(), S"-", __box(sequenceEnd)->ToString());
			}
		}

		return portStr;
	}

	//*************************************************************************
	// Method:		onExcludedSetSelected
	// Description: Called when the excluded ports are selected
	//
	// Parameters:
	//	index - the index of the selected node
	//	selected - true if the node is to be selected, false to unselect
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onExcludedSetSelected(int index, bool selected)
	{
		if (index >= excludedPorts->Count)
			return;

		PortNode* node = dynamic_cast<PortNode*>(excludedPorts->Item[index]);
		node->selected = selected;
	}

	//*************************************************************************
	// Method:		onExcludedSetFocused
	// Description: Called when an item in the excluded list gets focus
	//
	// Parameters:
	//	index - the index of the focused node
	//	selected - true if the node is to be focused, false to unfocus
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onExcludedSetFocused(int index, bool focused)
	{
		if (index >= excludedPorts->Count)
			return;

		PortNode* node = dynamic_cast<PortNode*>(excludedPorts->Item[index]);
		node->focused = focused;
	}

	//*************************************************************************
	// Method:		onExcludedQueryItem
	// Description: called to get an item in the excluded list
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: The list view item at that index
	//*************************************************************************
	ContainerListViewItem *NetworkCorruptionPortSelectionPage::onExcludedQueryItem(unsigned int index)
	{
		if (index >= (unsigned int)excludedPorts->Count)
			return NULL;

		PortNode* node = dynamic_cast<PortNode*>(excludedPorts->Item[index]);

		ContainerListViewItem* item = new ContainerListViewItem();
		item->Focused = node->focused;
		item->Selected = node->selected;
		item->Text = __box(node->port)->ToString();
		item->Tag = __box(node->port);
		item->Index = index;

		String* portName = portDB->GetNameFromNumber(node->port);
		if (portName != NULL)
			item->SubItems->Add(portName);
		else
			item->SubItems->Add(S"");

		return item;
	}

	//*************************************************************************
	// Method:		onExcludedQueryItemIndex
	// Description: Called to get the index of a specified item in the excluded list
	//
	// Parameters:
	//	item - the item to get the index of 
	//
	// Return Value: The index of the item
	//*************************************************************************
	unsigned int NetworkCorruptionPortSelectionPage::onExcludedQueryItemIndex(ContainerListViewItem *item)
	{
		int port = *dynamic_cast<Int32*>(item->Tag);
		for (int i=0; i<excludedPorts->Count; i++)
		{
			PortNode* node = dynamic_cast<PortNode*>(excludedPorts->Item[i]);
			if (node->port == port)
				return i;
		}
		return -1;
	}

	//*************************************************************************
	// Method:		onIncludedSetSelected
	// Description: Called when the included ports are selected
	//
	// Parameters:
	//	index - the index of the selected node
	//	selected - true if the node is to be selected, false to unselect
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onIncludedSetSelected(int index, bool selected)
	{
		if (index >= includedPorts->Count)
			return;

		PortNode* node = dynamic_cast<PortNode*>(includedPorts->Item[index]);
		node->selected = selected;
	}

	//*************************************************************************
	// Method:		onIncludedSetFocused
	// Description: Called when an item in the included list gets focus
	//
	// Parameters:
	//	index - the index of the focused node
	//	selected - true if the node is to be focused, false to unfocus
	//
	// Return Value: None
	//*************************************************************************
	void NetworkCorruptionPortSelectionPage::onIncludedSetFocused(int index, bool focused)
	{
		if (index >= includedPorts->Count)
			return;

		PortNode* node = dynamic_cast<PortNode*>(includedPorts->Item[index]);
		node->focused = focused;
	}

	//*************************************************************************
	// Method:		onIncludedQueryItem
	// Description: called to get an item in the included list
	//
	// Parameters:
	//	index - the index of the item to get
	//
	// Return Value: The list view item at that index
	//*************************************************************************
	ContainerListViewItem *NetworkCorruptionPortSelectionPage::onIncludedQueryItem(unsigned int index)
	{
		if (index >= (unsigned int)includedPorts->Count)
			return NULL;

		PortNode* node = dynamic_cast<PortNode*>(includedPorts->Item[index]);

		ContainerListViewItem* item = new ContainerListViewItem();
		item->Focused = node->focused;
		item->Selected = node->selected;
		item->Text = __box(node->port)->ToString();
		item->Tag = __box(node->port);
		item->Index = index;

		String* portName = portDB->GetNameFromNumber(node->port);
		if (portName != NULL)
			item->SubItems->Add(portName);
		else
			item->SubItems->Add(S"");

		return item;
	}

	//*************************************************************************
	// Method:		onIncludedQueryItemIndex
	// Description: Called to get the index of a specified item in the included list
	//
	// Parameters:
	//	item - the item to get the index of 
	//
	// Return Value: The index of the item
	//*************************************************************************
	unsigned int NetworkCorruptionPortSelectionPage::onIncludedQueryItemIndex(ContainerListViewItem *item)
	{
		int port = *dynamic_cast<Int32*>(item->Tag);
		for (int i=0; i<includedPorts->Count; i++)
		{
			PortNode* node = dynamic_cast<PortNode*>(includedPorts->Item[i]);
			if (node->port == port)
				return i;
		}
		return -1;
	}
}
