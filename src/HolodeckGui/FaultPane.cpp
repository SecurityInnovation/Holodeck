//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FaultPane.cpp
//
// DESCRIPTION: Contains implementation for the class FaultPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include "RegistrationMgr.h"
#include "FaultPane.h"

using namespace HolodeckRegistration;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FaultPane
	// Description: Constructor for the FaultPane class
	//
	// Parameters:
	//	data - The FaultPaneDataArray object that this pane is a view of
	//
	// Return Value: None
	//*************************************************************************
	FaultPane::FaultPane(FaultPaneDataArray *data, unsigned int ThreadID)
	{
		this->dataArray = data;
		this->threadID = ThreadID;
		deletedDelegate = new ItemDeletedDelegate(this, onItemDeleted);
		dataArray->onItemDeleted += deletedDelegate;

		addedDelegate = new ItemAddedDelegate(this, onItemAdded);
		dataArray->onItemAdded += addedDelegate;

		InitializeUI();
	}

	//*************************************************************************
	// Method:		~FaultPane
	// Description: Destructor for the FaultPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FaultPane::~FaultPane()
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
	void FaultPane::Dispose(bool disposing)
	{
		dataArray->onItemDeleted -= deletedDelegate;
		dataArray->onItemAdded -= addedDelegate;
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
	void FaultPane::InitializeUI()
	{	
		int width = 0, height = 0;
		GroupBox *biggestGroupBox, *currentGroupBox;
		FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
		IList *diskFaults = faultDB->DiskFaults;
		IList *memoryFaults = faultDB->MemoryFaults;
		IList *networkFaults = faultDB->NetworkFaults;
		IList *registryFaults = faultDB->RegistryFaults;
		IList *processFaults = faultDB->ProcessFaults;
		IList *customFaults = faultDB->CustomFaults;
		
		DisplayableDataNode *dataNode;
		panel = new Panel();
		System::Drawing::Size size;

		memoryBox = new GroupBox();
		diskBox = new GroupBox();
		networkBox = new GroupBox();
		registryBox = new GroupBox();
		processBox = new GroupBox();
		customBox = new GroupBox();
		RadioButton *radioButton;

		memoryBox->FlatStyle = FlatStyle::Popup;
		diskBox->FlatStyle = FlatStyle::Popup;
		networkBox->FlatStyle = FlatStyle::Popup;
		registryBox->FlatStyle = FlatStyle::Popup;
		processBox->FlatStyle = FlatStyle::Popup;
		customBox->FlatStyle = FlatStyle::Popup;

		panel->AutoScroll = true;
		panel->BackColor = Color::LightSteelBlue;
		size.Height = 10;
		size.Width = 10;
		panel->AutoScrollMargin = size;
		panel->Dock = DockStyle::Fill;
		this->Controls->Add(panel);

		panel->Layout += new LayoutEventHandler(this, LayoutChanged);

		//Disk fault radio buttons
		for (int i=0; i<diskFaults->Count; i++)
		{
			radioButton = new RadioButton();
			Fault *fault = dynamic_cast <Fault*> (diskFaults->Item[i]);
			radioButton->TabIndex = diskFaults->Count - i;
			radioButton->Text = fault->Name;
			radioButton->Dock = DockStyle::Top;
			radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
			dataNode = dataArray->GetDataNodeByID(Convert::ToString(fault->FaultID));
			if ( dataNode && (dataNode->threadID == threadID) ) 
				radioButton->Checked = true;

			diskBox->Controls->Add(radioButton);
		}

		radioButton = new RadioButton();
    	radioButton->TabIndex = 0;
		radioButton->Text = "No Disk Fault";
		radioButton->Dock = DockStyle::Top;
		radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
		dataNode = NULL;
		if (!dataArray->FindFaultNode(NodeType::DiskFault, threadID))
			radioButton->Checked = true;

		diskBox->Controls->Add(radioButton);

		//Memory radio buttons
		for (int i=0; i<memoryFaults->Count; i++)
		{
			radioButton = new RadioButton();
			Fault *fault = dynamic_cast <Fault*> (memoryFaults->Item[i]);
			radioButton->TabIndex = memoryFaults->Count - i;
			radioButton->Text = fault->Name;
			radioButton->Dock = DockStyle::Top;
			radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
			dataNode = dataArray->GetDataNodeByID(Convert::ToString(fault->FaultID));
			if ( dataNode && (dataNode->threadID == threadID) ) 
				radioButton->Checked = true;

			memoryBox->Controls->Add(radioButton);
		}

		radioButton = new RadioButton();
    	radioButton->TabIndex = 0;
		radioButton->Text = "No Memory Fault";
		radioButton->Dock = DockStyle::Top;
		radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
		dataNode = NULL;
		if (!dataArray->FindFaultNode(NodeType::MemoryFault, threadID))
			radioButton->Checked = true;

		memoryBox->Controls->Add(radioButton);

		//Network radio buttons
		for (int i=0; i<networkFaults->Count; i++)
		{
			radioButton = new RadioButton();
			Fault *fault = dynamic_cast <Fault*> (networkFaults->Item[i]);
			radioButton->TabIndex = networkFaults->Count - i;
			radioButton->Text = fault->Name;
			radioButton->Dock = DockStyle::Top;
			radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
			dataNode = dataArray->GetDataNodeByID(Convert::ToString(fault->FaultID));
			if ( dataNode && (dataNode->threadID == threadID) ) 
				radioButton->Checked = true;

			networkBox->Controls->Add(radioButton);
		}

		radioButton = new RadioButton();
    	radioButton->TabIndex = 0;
		radioButton->Text = "No Network Fault";
		radioButton->Dock = DockStyle::Top;
		radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
		dataNode = NULL;
		if (!dataArray->FindFaultNode(NodeType::NetworkFault, threadID))
			radioButton->Checked = true;

		networkBox->Controls->Add(radioButton);

		//Registry radio buttons
		for (int i=0; i<registryFaults->Count; i++)
		{
			radioButton = new RadioButton();
			Fault *fault = dynamic_cast <Fault*> (registryFaults->Item[i]);
			radioButton->TabIndex = registryFaults->Count - i;
			radioButton->Text = fault->Name;
			radioButton->Dock = DockStyle::Top;
			radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
			dataNode = dataArray->GetDataNodeByID(Convert::ToString(fault->FaultID));
			if ( dataNode && (dataNode->threadID == threadID) ) 
				radioButton->Checked = true;

			registryBox->Controls->Add(radioButton);
		}

		radioButton = new RadioButton();
    	radioButton->TabIndex = 0;
		radioButton->Text = "No Registry Fault";
		radioButton->Dock = DockStyle::Top;
		radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
		dataNode = NULL;
		if (!dataArray->FindFaultNode(NodeType::RegistryFault, threadID))
			radioButton->Checked = true;

		registryBox->Controls->Add(radioButton);

		//Process radio buttons
		for (int i=0; i<processFaults->Count; i++)
		{
			radioButton = new RadioButton();
			Fault *fault = dynamic_cast <Fault*> (processFaults->Item[i]);
			radioButton->TabIndex = processFaults->Count - i;
			radioButton->Text = fault->Name;
			radioButton->Dock = DockStyle::Top;
			radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
			dataNode = dataArray->GetDataNodeByID(Convert::ToString(fault->FaultID));
			if ( dataNode && (dataNode->threadID == threadID) ) 
				radioButton->Checked = true;

			processBox->Controls->Add(radioButton);
		}

		radioButton = new RadioButton();
		radioButton->TabIndex = 0;
    	radioButton->Text = "No Process/Library Fault";
		radioButton->Dock = DockStyle::Top;
		radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
		dataNode = NULL;
		if (!dataArray->FindFaultNode(NodeType::ProcessFault, threadID))
			radioButton->Checked = true;

		processBox->Controls->Add(radioButton);

		//Custom radio buttons
		for (int i=0; i<customFaults->Count; i++)
		{
			radioButton = new RadioButton();
			Fault *fault = dynamic_cast <Fault*> (customFaults->Item[i]);
			radioButton->TabIndex = customFaults->Count - i;
			radioButton->Text = fault->Name;
			radioButton->Dock = DockStyle::Top;
			radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
			dataNode = dataArray->GetDataNodeByID(Convert::ToString(fault->FaultID));
			if ( dataNode && (dataNode->threadID == threadID) ) 
				radioButton->Checked = true;

			customBox->Controls->Add(radioButton);
		}

		radioButton = new RadioButton();
    	radioButton->TabIndex = 0;
		radioButton->Text = "No Custom Fault";
		radioButton->Dock = DockStyle::Top;
		radioButton->CheckedChanged += new EventHandler(this, onCheckedChanged);
		dataNode = NULL;
		if (!dataArray->FindFaultNode(NodeType::CustomFault, threadID))
			radioButton->Checked = true;

		customBox->Controls->Add(radioButton);

		//add help link for how to add a custom fault
		//bugbug - need the real help content to link to, using a dummy for now.
		LinkLabel *customFaultsHelpLink = new LinkLabel();
		radioButton = dynamic_cast <RadioButton*> (customBox->Controls->Item[0]);
		customFaultsHelpLink->Location = Point(radioButton->Location.X, radioButton->Location.Y + 45);
		customFaultsHelpLink->Size = System::Drawing::Size(175, 30);
		customFaultsHelpLink->TabStop = true;
		customFaultsHelpLink->Text = "Click here to learn how to add a custom fault scenario.";
		customFaultsHelpLink->LinkClicked += new LinkLabelLinkClickedEventHandler(this, helpLinkClicked);

		customBox->Controls->Add(customFaultsHelpLink);

		//turn off some faults if this is a trial
		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			for (int i=2; i<diskBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (diskBox->Controls->Item[i]);
				radioButton->Enabled = false;
			}
			for (int i=2; i<memoryBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (memoryBox->Controls->Item[i]);
				radioButton->Enabled = false;
			}
			for (int i=2; i<networkBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (networkBox->Controls->Item[i]);
				radioButton->Enabled = false;
			}
			for (int i=2; i<registryBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (registryBox->Controls->Item[i]);
				radioButton->Enabled = false;
			}
			for (int i=2; i<processBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (processBox->Controls->Item[i]);
				radioButton->Enabled = false;
			}
			for (int i=2; i<customBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (customBox->Controls->Item[i]);
				// not all these controls are radio buttons now that we added a link label
				if (radioButton)
					radioButton->Enabled = false;
			}
		}
		
		diskBox->Text = "Disk Faults";
		memoryBox->Text = "Memory Faults";
		networkBox->Text = "Network Faults";
		registryBox->Text = "Registry Faults";
		processBox->Text = "Process/Library Faults";
		customBox->Text = "Custom Faults";
	
		panel->Controls->Add(customBox);
		panel->Controls->Add(processBox);
		panel->Controls->Add(registryBox);
		panel->Controls->Add(networkBox);
		panel->Controls->Add(memoryBox);
		panel->Controls->Add(diskBox);

		//set up the groupbox size and position
		diskBox->Top = diskBox->Top + 1;
		memoryBox->Top = memoryBox->Top + 1;
		networkBox->Top = networkBox->Top + 1;
		registryBox->Top = networkBox->Top + 1;
		processBox->Top = networkBox->Top + 1;
		customBox->Top = customBox->Top + 1;
		diskBox->Left = diskBox->Left + 1;

		biggestGroupBox = dynamic_cast <GroupBox*> (panel->Controls->Item[0]);
		if (!biggestGroupBox)
			return;

		for (int i=1; i<panel->Controls->Count; i++)
		{
			currentGroupBox = dynamic_cast <GroupBox*> (panel->Controls->Item[i]);
			if (!currentGroupBox)
				continue;

			//find tallest box
			if (currentGroupBox->Controls->Count > biggestGroupBox->Controls->Count)
				biggestGroupBox = currentGroupBox;

			//find widest button
			for (int i=0; i<currentGroupBox->Controls->Count; i++)
			{
				radioButton = dynamic_cast <RadioButton*> (currentGroupBox->Controls->get_Item(i));
				if (!radioButton)
					continue;

				if (radioButton->Width > width)
					width = radioButton->Width;
			}
		}

		//get radiobutton height
		radioButton = dynamic_cast <RadioButton*> (biggestGroupBox->Controls->Item[0]);
		if (!radioButton)
			return;
		height = radioButton->Height;

		diskBox->Height = height * (biggestGroupBox->Controls->Count + 1);
		diskBox->Width = width;
		memoryBox->Height = diskBox->Height;
		memoryBox->Width = diskBox->Width;
		networkBox->Height = diskBox->Height;
		networkBox->Width = diskBox->Width;
		registryBox->Height = diskBox->Height;
		registryBox->Width = diskBox->Width;
		processBox->Height = diskBox->Height;
		processBox->Width = diskBox->Width;
		customBox->Height = diskBox->Height;
		customBox->Width = diskBox->Width;

		memoryBox->Left = diskBox->Left + diskBox->Width + 10;
		networkBox->Left = diskBox->Left + (diskBox->Width + 10) * 2;
		registryBox->Left = diskBox->Left + (diskBox->Width + 10) * 3;
		processBox->Left = diskBox->Left + (diskBox->Width + 10) * 4;
		customBox->Left = diskBox->Left + (diskBox->Width + 10) * 5;
	}

	//*************************************************************************
	// Method:		onItemDeleted
	// Description: called when a fault is deleted
	//
	// Parameters:
	//	dataNode - the node representing the deleted fault
	//
	// Return Value: None
	//*************************************************************************
	void FaultPane::onItemDeleted(DisplayableDataNode *dataNode)
	{
		if ( (dataNode->Type == UIData::DiskFault) && (dataNode->threadID == threadID) )
		{
			int faultCount = diskBox->Controls->Count;
			RadioButton *radioButton = dynamic_cast <RadioButton*> (diskBox->Controls->Item[faultCount-1]);
			radioButton->Checked = true;
		}
		else if ( (dataNode->Type == UIData::MemoryFault) && (dataNode->threadID == threadID) )
		{
			int faultCount = memoryBox->Controls->Count;
			RadioButton *radioButton = dynamic_cast <RadioButton*> (memoryBox->Controls->Item[faultCount-1]);
			radioButton->Checked = true;
		}
		else if ( (dataNode->Type == UIData::NetworkFault) && (dataNode->threadID == threadID) )
		{
			int faultCount = networkBox->Controls->Count;
			RadioButton *radioButton = dynamic_cast <RadioButton*> (networkBox->Controls->Item[faultCount-1]);
			radioButton->Checked = true;
		}
		else if ( (dataNode->Type == UIData::RegistryFault) && (dataNode->threadID == threadID) )
		{
			int faultCount = registryBox->Controls->Count;
			RadioButton *radioButton = dynamic_cast <RadioButton*> (registryBox->Controls->Item[faultCount-1]);
			radioButton->Checked = true;
		}
		else if ( (dataNode->Type == UIData::ProcessFault) && (dataNode->threadID == threadID) )
		{
			int faultCount = processBox->Controls->Count;
			RadioButton *radioButton = dynamic_cast <RadioButton*> (processBox->Controls->Item[faultCount-1]);
			radioButton->Checked = true;
		}
		else if ( (dataNode->Type == UIData::CustomFault) && (dataNode->threadID == threadID) )
		{
			int faultCount = customBox->Controls->Count;
			RadioButton *radioButton = dynamic_cast <RadioButton*> (customBox->Controls->Item[faultCount-2]);
			radioButton->Checked = true;
		}
	}

	//*************************************************************************
	// Method:		onItemAdded
	// Description: called when a fault is added
	//
	// Parameters:
	//	dataNode - the node representing the deleted fault
	//
	// Return Value: None
	//*************************************************************************
	void FaultPane::onItemAdded(DisplayableDataNode *dataNode)
	{
		RadioButton *radioButton = FindRadioButton(dataNode);

		if ( radioButton && (dataNode->threadID == threadID) )
		{
			radioButton->Checked = true;
		}
	}

	//*************************************************************************
	// Method:		onCheckChanged
	// Description: called when a radio is modified
	//
	// Parameters:
	//	sender - object that sent the event
	//	e - event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FaultPane::onCheckedChanged(Object *sender, EventArgs *e)
	{
		FaultPaneDataNode *faultNode;
		NodeType type;
		RadioButton *radioButton = dynamic_cast <RadioButton*> (sender);
		GroupBox *groupBox = dynamic_cast <GroupBox*> (radioButton->Parent);

		if (radioButton && groupBox && radioButton->Checked)
		{
			if (groupBox->Text->StartsWith("Disk"))
			{
				type = UIData::DiskFault;
			}
			else if (groupBox->Text->StartsWith("Memory"))
			{
				type = UIData::MemoryFault;
			}
			else if (groupBox->Text->StartsWith("Network"))
			{
				type = UIData::NetworkFault;
			}
			else if (groupBox->Text->StartsWith("Registry"))
			{
				type = UIData::RegistryFault;
			}
			else if (groupBox->Text->StartsWith("Process"))
			{
				type = UIData::ProcessFault;
			}
			else if (groupBox->Text->StartsWith("Custom"))
			{
				type = UIData::CustomFault;
			}
			else
				return;

			faultNode = new FaultPaneDataNode(radioButton->Text, type, dataArray->ProcessID, threadID);
			faultNode->Element = radioButton;
			radioButton->Tag = faultNode;

			int diskCount = diskBox->Controls->Count;
			int memoryCount = memoryBox->Controls->Count;
			int networkCount = networkBox->Controls->Count;
			int registryCount = registryBox->Controls->Count;
			int processCount = processBox->Controls->Count;
			int customCount = customBox->Controls->Count;
			if ( (radioButton != diskBox->Controls->Item[diskCount-1]) && (radioButton != memoryBox->Controls->Item[memoryCount-1]) && (radioButton != networkBox->Controls->Item[networkCount-1]) && (radioButton != registryBox->Controls->Item[registryCount-1]) && (radioButton != processBox->Controls->Item[processCount-1]) && (radioButton != customBox->Controls->Item[customCount-2]) )
				dataArray->BeginCreateNode(faultNode);
			else
				dataArray->BeginDeleteNode(faultNode);
		}

		if (faultNode)
			uiEventRegistry->OnFocusChanged->Invoke(faultNode);
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
	void FaultPane::LayoutChanged(Object *sender, LayoutEventArgs *e)
	{
		diskBox->Focus();
	}

	//*************************************************************************
	// Method:		FindRadioButton
	// Description: Returns the radio associated with the given datanode
	//
	// Parameters:
	//	dataNode - dataNode that represents checkbox to search for
	//
	// Return Value: checkbox if found, otherwise null
	//*************************************************************************
	RadioButton *FaultPane::FindRadioButton(DisplayableDataNode *dataNode)
	{
		RadioButton *radioButton;
		GroupBox *groupBox;

		switch (dataNode->Type)
		{
		case NodeType::DiskFault:
			groupBox = diskBox;
			break;
		case NodeType::MemoryFault:
			groupBox = memoryBox;
			break;
		case NodeType::NetworkFault:
			groupBox = networkBox;
			break;
		case NodeType::ProcessFault:
			groupBox = processBox;
			break;
		case NodeType::RegistryFault:
			groupBox = registryBox;
			break;
		case NodeType::CustomFault:
			groupBox = customBox;
			break;
		}

		for (int i=0; (groupBox) && (i<groupBox->Controls->Count); i++)
		{
			radioButton = dynamic_cast <RadioButton*> (groupBox->Controls->get_Item(i));
			if (radioButton->Text->CompareTo(dataNode->Name) == 0)
				return radioButton;
		}

	
		return NULL;
	}

	//****************************************************************************
	// Method:		helpLinkClicked
	// Description: Handles the click of the custom faults help link
	//
	// Parameters:
	//	sender - the object that sent the event
	//	args - the event arguments representing the click event
	//
	// Return Value: None
	//*************************************************************************
	void FaultPane::helpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args)
	{
		String *url;
		try
		{
			if (UIEventRegistry::getInstance()->OnURLClicked)
			{
				url = String::Concat("file://", GuiSupport::RegistryHelper::getInstance()->GetInstallationPath(), "/");
				url = String::Concat(url, CUSTOM_FAULTS_HELP_FILE);
				UIEventRegistry::getInstance()->OnURLClicked->Invoke(url);
			}
		}
		catch (Exception *e)
		{
			String *msg = String::Concat("Could not open the help browser to the location: ", url, " - \nReason: ", e->Message->ToString());
			UserNotification::ErrorNotify(msg);
		}
	}

}