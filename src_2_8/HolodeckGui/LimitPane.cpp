//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LimitPane.cpp
//
// DESCRIPTION: Contains implementation for the class LimitPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include "RegistrationMgr.h"
#include "LimitPane.h"
#include "UserNotification.h"

using namespace HolodeckRegistration;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		LimitPane
	// Description: Constructor for the LimitPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LimitPane::LimitPane()
	{
		dataArrayList = new SortedList();
		dataEventRegistry = DataEventRegistry::getInstance();

		timer = new Timer();
		timer->Interval = 500;
		timer->Tick += new EventHandler(this, onTimerTick);
		timer->Start();

		Layout += new LayoutEventHandler(this, LayoutChanged);
		FocusChangedHandler = new FocusChangedDelegate(this, onFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;
		CloseProjectUIHandler = new CloseProjectUIDelegate(this, onCloseProjectUI);
		uiEventRegistry->OnCloseProjectUI += CloseProjectUIHandler;

		InitializeUI();

		diskTrackBar->MouseDown += new MouseEventHandler(this, diskTrackBarMouseDown);
		diskTrackBar->MouseUp += new MouseEventHandler(this, diskTrackBarValueChanged);
		diskLimitTextBox->GotFocus += new EventHandler(this, diskLimitTextBoxGotFocus);
		diskLimitTextBox->LostFocus += new EventHandler(this, diskLimitTextBoxTextChanged);
		diskLimitTextBox->KeyDown += new KeyEventHandler(this, diskLimitTextBoxKeyDown);
		diskUnitsComboBox->SelectedIndexChanged += new EventHandler(this, diskUnitsComboBoxIndexChanged);
		memoryTrackBar->MouseDown += new MouseEventHandler(this, memoryTrackBarMouseDown);
		memoryTrackBar->MouseUp += new MouseEventHandler(this, memoryTrackBarValueChanged);
		memoryLimitTextBox->GotFocus += new EventHandler(this, memoryLimitTextBoxGotFocus);
		memoryLimitTextBox->LostFocus += new EventHandler(this, memoryLimitTextBoxTextChanged);
		memoryLimitTextBox->KeyDown += new KeyEventHandler(this, memoryLimitTextBoxKeyDown);
		memoryUnitsComboBox->SelectedIndexChanged += new EventHandler(this, memoryUnitsComboBoxIndexChanged);
		networkUpTrackBar->MouseDown += new MouseEventHandler(this, networkUpTrackBarMouseDown);
		networkUpTrackBar->MouseUp += new MouseEventHandler(this, networkUpTrackBarValueChanged);
		networkUpLimitTextBox->GotFocus += new EventHandler(this, networkUpLimitTextBoxGotFocus);
		networkUpLimitTextBox->LostFocus += new EventHandler(this, networkUpLimitTextBoxTextChanged);
		networkUpLimitTextBox->KeyDown += new KeyEventHandler(this, networkUpLimitTextBoxKeyDown);
		networkUpUnitsComboBox->SelectedIndexChanged += new EventHandler(this, networkUpUnitsComboBoxIndexChanged);
		networkDownTrackBar->MouseDown += new MouseEventHandler(this, networkDownTrackBarMouseDown);
		networkDownTrackBar->MouseUp += new MouseEventHandler(this, networkDownTrackBarValueChanged);
		networkDownLimitTextBox->GotFocus += new EventHandler(this, networkDownLimitTextBoxGotFocus);
		networkDownLimitTextBox->LostFocus += new EventHandler(this, networkDownLimitTextBoxTextChanged);
		networkDownLimitTextBox->KeyDown += new KeyEventHandler(this, networkDownLimitTextBoxKeyDown);
		networkDownUnitsComboBox->SelectedIndexChanged += new EventHandler(this, networkDownUnitsComboBoxIndexChanged);

		memoryLimitTextBoxFocus = false;
		memoryTrackBarFocus = false;
		diskLimitTextBoxFocus = false;
		diskTrackBarFocus = false;
		networkUpLimitTextBoxFocus = false;
		networkUpTrackBarFocus = false;
		networkDownLimitTextBoxFocus = false;
		networkDownTrackBarFocus = false;

		diskLimitEnabled = false;
		memoryLimitEnabled = false;
		networkUpLimitEnabled = false;
		networkDownLimitEnabled = false;
	}

	//*************************************************************************
	// Method:		~LimitPane
	// Description: Destructor for the LimitPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LimitPane::~LimitPane()
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
	void LimitPane::Dispose(bool disposing)
	{
		timer->Stop();
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
	void LimitPane::InitializeUI()
	{
		System::Drawing::Size size;
		diskPanel = new Panel();
		diskProgressBar = new ProgressBar();
		diskTrackBar = new TrackBar();
		diskLabel = new Label();
		diskLimitLabel = new Label();
		diskUsedLabel = new Label();
		diskUnitsLabel = new Label();
		diskUsedTextBox = new TextBox();
		diskLimitTextBox = new TextBox();
		diskUnitsComboBox = new ComboBox();

		memoryPanel = new Panel();
		memoryProgressBar = new ProgressBar();
		memoryTrackBar = new TrackBar();
		memoryLabel = new Label();
		memoryLimitLabel = new Label();
		memoryUsedLabel = new Label();
		memoryUnitsLabel = new Label();
		memoryUsedTextBox = new TextBox();
		memoryLimitTextBox = new TextBox();
		memoryUnitsComboBox = new ComboBox();

		networkUpPanel = new Panel();
		networkUpProgressBar = new ProgressBar();
		networkUpTrackBar = new TrackBar();
		networkUpLabel = new Label();
		networkUpLimitLabel = new Label();
		networkUpUsedLabel = new Label();
		networkUpUnitsLabel = new Label();
		networkUpUsedTextBox = new TextBox();
		networkUpLimitTextBox = new TextBox();
		networkUpUnitsComboBox = new ComboBox();

		networkDownPanel = new Panel();
		networkDownProgressBar = new ProgressBar();
		networkDownTrackBar = new TrackBar();
		networkDownLabel = new Label();
		networkDownLimitLabel = new Label();
		networkDownUsedLabel = new Label();
		networkDownUnitsLabel = new Label();
		networkDownUsedTextBox = new TextBox();
		networkDownLimitTextBox = new TextBox();
		networkDownUnitsComboBox = new ComboBox();

		this->BorderStyle = BorderStyle::Fixed3D;
		this->AutoScroll = true;
		size.Height = 5;
		size.Width = 5;
		this->AutoScrollMargin = size;
		this->BackColor = Color::Gainsboro;

		diskPanel->BorderStyle = BorderStyle::Fixed3D;
		memoryPanel->BorderStyle = BorderStyle::Fixed3D;
		networkUpPanel->BorderStyle = BorderStyle::Fixed3D;
		networkDownPanel->BorderStyle = BorderStyle::Fixed3D;
	
		diskPanel->BackColor = Color::LightSteelBlue;
		diskPanel->Top = 5;
		diskPanel->Left = 0;
		diskProgressBar->Text = "Disk";
		diskTrackBar->TickStyle = TickStyle::None;
		diskTrackBar->Minimum = 0;
		diskTrackBar->Maximum = 100;
		diskTrackBar->SmallChange = 1;
		diskTrackBar->LargeChange = 1;
		diskTrackBar->AutoSize = false;
		diskTrackBar->Enabled = false;
		diskLabel->Text = "Disk Space";
		diskLimitLabel->Text = "Limit:";
		diskUsedLabel->Text = "Used:";
		diskUnitsLabel->Text = "Units:";
		diskLabel->AutoSize = true;
		diskLimitLabel->AutoSize = true;
		diskUsedLabel->AutoSize = true;
		diskUnitsLabel->AutoSize = true;
		diskUsedTextBox->ReadOnly = true;
		diskLimitTextBox->Enabled = false;
		diskUnitsComboBox->MaxDropDownItems = 4;
		diskUnitsComboBox->Items->Add(new String("Bytes"));
		diskUnitsComboBox->Items->Add(new String("KB"));
		diskUnitsComboBox->Items->Add(new String("MB"));
		diskUnitsComboBox->Items->Add(new String("GB"));
		diskUnitsComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		diskUnitsComboBox->SelectedIndex = 1;
		diskUnitsComboBox->Enabled = false;
		diskUnits = 1024;
		this->Controls->Add(diskPanel);
		diskPanel->Controls->Add(diskLabel);
		diskPanel->Controls->Add(diskProgressBar);
		diskPanel->Controls->Add(diskTrackBar);
		diskPanel->Controls->Add(diskUsedLabel);
		diskPanel->Controls->Add(diskUsedTextBox);
		diskPanel->Controls->Add(diskLimitLabel);
		diskPanel->Controls->Add(diskLimitTextBox);
		diskPanel->Controls->Add(diskUnitsLabel);
		diskPanel->Controls->Add(diskUnitsComboBox);

		memoryPanel->BackColor = Color::LightSteelBlue;
		memoryProgressBar->Text = "Memory";
		memoryTrackBar->TickStyle = TickStyle::None;
		memoryTrackBar->Minimum = 0;
		memoryTrackBar->Maximum = 100;
		memoryTrackBar->SmallChange = 1;
		memoryTrackBar->LargeChange = 1;
		memoryTrackBar->AutoSize = false;
		memoryTrackBar->Enabled = false;
		memoryLabel->Text = "Memory Space";
		memoryLimitLabel->Text = "Limit:";
		memoryUsedLabel->Text = "Used:";
		memoryUnitsLabel->Text = "Units:";
		memoryLabel->AutoSize = true;
		memoryLimitLabel->AutoSize = true;
		memoryUsedLabel->AutoSize = true;
		memoryUnitsLabel->AutoSize = true;
		memoryUsedTextBox->ReadOnly = true;
		memoryLimitTextBox->Enabled = false;
		memoryUnitsComboBox->MaxDropDownItems = 3;
		memoryUnitsComboBox->Items->Add(new String("Bytes"));
		memoryUnitsComboBox->Items->Add(new String("KB"));
		memoryUnitsComboBox->Items->Add(new String("MB"));
		memoryUnitsComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		memoryUnitsComboBox->SelectedIndex = 1;
		memoryUnitsComboBox->Enabled = false;
		memoryUnits = 1024;
		this->Controls->Add(memoryPanel);
		memoryPanel->Controls->Add(memoryLabel);
		memoryPanel->Controls->Add(memoryProgressBar);
		memoryPanel->Controls->Add(memoryTrackBar);
		memoryPanel->Controls->Add(memoryUsedLabel);
		memoryPanel->Controls->Add(memoryUsedTextBox);
		memoryPanel->Controls->Add(memoryLimitLabel);
		memoryPanel->Controls->Add(memoryLimitTextBox);
		memoryPanel->Controls->Add(memoryUnitsLabel);
		memoryPanel->Controls->Add(memoryUnitsComboBox);

		networkUpPanel->BackColor = Color::LightSteelBlue;
		networkUpProgressBar->Text = "networkUp";
		networkUpTrackBar->TickStyle = TickStyle::None;
		networkUpTrackBar->Minimum = 0;
		networkUpTrackBar->Maximum = 100;
		networkUpTrackBar->SmallChange = 1;
		networkUpTrackBar->LargeChange = 1;
		networkUpTrackBar->AutoSize = false;
		networkUpTrackBar->Enabled = false;
		networkUpLabel->Text = "Network Upload Bandwidth";
		networkUpLimitLabel->Text = "Limit:";
		networkUpUsedLabel->Text = "Used:";
		networkUpUnitsLabel->Text = "Units:";
		networkUpLabel->AutoSize = true;
		networkUpLimitLabel->AutoSize = true;
		networkUpUsedLabel->AutoSize = true;
		networkUpUnitsLabel->AutoSize = true;
		networkUpUsedTextBox->ReadOnly = true;
		networkUpLimitTextBox->Enabled = false;
		networkUpUnitsComboBox->MaxDropDownItems = 3;
		networkUpUnitsComboBox->Items->Add(new String("Bits/Sec"));
		networkUpUnitsComboBox->Items->Add(new String("KBits/Sec"));
		networkUpUnitsComboBox->Items->Add(new String("MBits/Sec"));
		networkUpUnitsComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		networkUpUnitsComboBox->SelectedIndex = 1;
		networkUpUnitsComboBox->Enabled = false;
		networkUpUnits = 1000;
		this->Controls->Add(networkUpPanel);
		networkUpPanel->Controls->Add(networkUpLabel);
		networkUpPanel->Controls->Add(networkUpProgressBar);
		networkUpPanel->Controls->Add(networkUpTrackBar);
		networkUpPanel->Controls->Add(networkUpUsedLabel);
		networkUpPanel->Controls->Add(networkUpUsedTextBox);
		networkUpPanel->Controls->Add(networkUpLimitLabel);
		networkUpPanel->Controls->Add(networkUpLimitTextBox);
		networkUpPanel->Controls->Add(networkUpUnitsLabel);
		networkUpPanel->Controls->Add(networkUpUnitsComboBox);

		networkDownPanel->BackColor = Color::LightSteelBlue;
		networkDownProgressBar->Text = "networkDown";
		networkDownTrackBar->TickStyle = TickStyle::None;
		networkDownTrackBar->Minimum = 0;
		networkDownTrackBar->Maximum = 100;
		networkDownTrackBar->SmallChange = 1;
		networkDownTrackBar->LargeChange = 1;
		networkDownTrackBar->AutoSize = false;
		networkDownTrackBar->Enabled = false;
		networkDownLabel->Text = "Network Download Bandwidth";
		networkDownLimitLabel->Text = "Limit:";
		networkDownUsedLabel->Text = "Used:";
		networkDownUnitsLabel->Text = "Units:";
		networkDownLabel->AutoSize = true;
		networkDownLimitLabel->AutoSize = true;
		networkDownUsedLabel->AutoSize = true;
		networkDownUnitsLabel->AutoSize = true;
		networkDownUsedTextBox->ReadOnly = true;
		networkDownLimitTextBox->Enabled = false;
		networkDownUnitsComboBox->MaxDropDownItems = 3;
		networkDownUnitsComboBox->Items->Add(new String("Bits/Sec"));
		networkDownUnitsComboBox->Items->Add(new String("KBits/Sec"));
		networkDownUnitsComboBox->Items->Add(new String("MBits/Sec"));
		networkDownUnitsComboBox->DropDownStyle = ComboBoxStyle::DropDownList;
		networkDownUnitsComboBox->SelectedIndex = 1;
		networkDownUnitsComboBox->Enabled = false;
		networkDownUnits = 1000;
		this->Controls->Add(networkDownPanel);
		networkDownPanel->Controls->Add(networkDownLabel);
		networkDownPanel->Controls->Add(networkDownProgressBar);
		networkDownPanel->Controls->Add(networkDownTrackBar);
		networkDownPanel->Controls->Add(networkDownUsedLabel);
		networkDownPanel->Controls->Add(networkDownUsedTextBox);
		networkDownPanel->Controls->Add(networkDownLimitLabel);
		networkDownPanel->Controls->Add(networkDownLimitTextBox);
		networkDownPanel->Controls->Add(networkDownUnitsLabel);
		networkDownPanel->Controls->Add(networkDownUnitsComboBox);

		if (RegistrationMgr::GetInstance()->IsFeatureDisabledTrial())
		{
			networkUpPanel->Enabled = false;
			networkDownPanel->Enabled = false;
			diskPanel->Enabled = false;
		}
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
	void LimitPane::LayoutChanged(Object *sender, LayoutEventArgs *e)
	{
		diskPanel->Width = this->Width - 26;
		if (diskPanel->Width < 470)		//for readability, never go smaller than 470
			diskPanel->Width = 470;
		diskLabel->Top = 10;
		diskLabel->Left = 10;
		diskProgressBar->Top = diskLabel->Bottom + 5;
		diskProgressBar->Left = diskLabel->Left;
		diskProgressBar->Height = 15;
		diskProgressBar->Width = diskPanel->Width/2;
		diskTrackBar->Top = diskProgressBar->Bottom + 10;
		diskTrackBar->Left = diskProgressBar->Left;
		diskTrackBar->Height = diskProgressBar->Height*2;
		diskTrackBar->Width = diskProgressBar->Width;
		diskUsedLabel->Top = diskProgressBar->Top;
		diskUsedLabel->Left = diskProgressBar->Right + 10;
		diskUsedTextBox->Top = diskUsedLabel->Top;
		diskUsedTextBox->Left = diskUsedLabel->Right;
		diskUsedTextBox->Width = 65;
		diskLimitLabel->Top = diskTrackBar->Top;
		diskLimitLabel->Left = diskUsedLabel->Left;
		diskLimitTextBox->Top = diskLimitLabel->Top;
		diskLimitTextBox->Left = diskLimitLabel->Right;
		diskLimitTextBox->Width = 65;
		diskUnitsLabel->Top = diskUsedTextBox->Top;
		diskUnitsLabel->Left = diskUsedTextBox->Right + 10;
		diskUnitsComboBox->Top = diskUnitsLabel->Top;
		diskUnitsComboBox->Left = diskUnitsLabel->Right;
		diskUnitsComboBox->Width = 65;
		diskPanel->Height = diskTrackBar->Bottom + 10;

		memoryPanel->Top = diskPanel->Bottom + 5;
		memoryPanel->Left = diskPanel->Left;
		memoryPanel->Width = diskPanel->Width;
		memoryLabel->Top = diskLabel->Top;
		memoryLabel->Left = diskLabel->Left;
		memoryProgressBar->Top = diskProgressBar->Top;
		memoryProgressBar->Left = diskProgressBar->Left;
		memoryProgressBar->Height = diskProgressBar->Height;
		memoryProgressBar->Width = diskProgressBar->Width;
		memoryTrackBar->Top = memoryProgressBar->Bottom + 10;
		memoryTrackBar->Left = memoryProgressBar->Left;
		memoryTrackBar->Height = memoryProgressBar->Height*2;
		memoryTrackBar->Width = memoryProgressBar->Width;
		memoryUsedLabel->Top = memoryProgressBar->Top;
		memoryUsedLabel->Left = memoryProgressBar->Right + 10;
		memoryUsedTextBox->Top = memoryUsedLabel->Top;
		memoryUsedTextBox->Left = memoryUsedLabel->Right;
		memoryUsedTextBox->Width = diskUsedTextBox->Width;
		memoryLimitLabel->Top = memoryTrackBar->Top;
		memoryLimitLabel->Left = memoryUsedLabel->Left;
		memoryLimitTextBox->Top = memoryLimitLabel->Top;
		memoryLimitTextBox->Left = memoryLimitLabel->Right;
		memoryLimitTextBox->Width = diskLimitTextBox->Width;
		memoryUnitsLabel->Top = memoryUsedTextBox->Top;
		memoryUnitsLabel->Left = memoryUsedTextBox->Right + 10;
		memoryUnitsComboBox->Top = memoryUnitsLabel->Top;
		memoryUnitsComboBox->Left = memoryUnitsLabel->Right;
		memoryUnitsComboBox->Width = diskUnitsComboBox->Width;
		memoryPanel->Height = diskPanel->Height;

		networkUpPanel->Top = memoryPanel->Bottom + 5;
		networkUpPanel->Left = diskPanel->Left;
		networkUpPanel->Width = memoryPanel->Width;
		networkUpLabel->Top = memoryLabel->Top;
		networkUpLabel->Left = memoryLabel->Left;
		networkUpProgressBar->Top = memoryProgressBar->Top;
		networkUpProgressBar->Left = memoryProgressBar->Left;
		networkUpProgressBar->Height = memoryProgressBar->Height;
		networkUpProgressBar->Width = memoryProgressBar->Width;
		networkUpTrackBar->Top = networkUpProgressBar->Bottom + 10;
		networkUpTrackBar->Left = networkUpProgressBar->Left;
		networkUpTrackBar->Height = networkUpProgressBar->Height*2;
		networkUpTrackBar->Width = networkUpProgressBar->Width;
		networkUpUsedLabel->Top = networkUpProgressBar->Top;
		networkUpUsedLabel->Left = networkUpProgressBar->Right + 10;
		networkUpUsedTextBox->Top = networkUpUsedLabel->Top;
		networkUpUsedTextBox->Left = networkUpUsedLabel->Right;
		networkUpUsedTextBox->Width = memoryUsedTextBox->Width;
		networkUpLimitLabel->Top = networkUpTrackBar->Top;
		networkUpLimitLabel->Left = networkUpUsedLabel->Left;
		networkUpLimitTextBox->Top = networkUpLimitLabel->Top;
		networkUpLimitTextBox->Left = networkUpLimitLabel->Right;
		networkUpLimitTextBox->Width = memoryLimitTextBox->Width;
		networkUpUnitsLabel->Top = networkUpUsedTextBox->Top;
		networkUpUnitsLabel->Left = networkUpUsedTextBox->Right + 10;
		networkUpUnitsComboBox->Top = networkUpUnitsLabel->Top;
		networkUpUnitsComboBox->Left = networkUpUnitsLabel->Right;
		networkUpUnitsComboBox->Width = diskUnitsComboBox->Width + 10;
		networkUpPanel->Height = memoryPanel->Height;

		networkDownPanel->Top = networkUpPanel->Bottom + 5;
		networkDownPanel->Left = diskPanel->Left;
		networkDownPanel->Width = networkUpPanel->Width;
		networkDownLabel->Top = networkUpLabel->Top;
		networkDownLabel->Left = networkUpLabel->Left;
		networkDownProgressBar->Top = networkUpProgressBar->Top;
		networkDownProgressBar->Left = networkUpProgressBar->Left;
		networkDownProgressBar->Height = networkUpProgressBar->Height;
		networkDownProgressBar->Width = networkUpProgressBar->Width;
		networkDownTrackBar->Top = networkDownProgressBar->Bottom + 10;
		networkDownTrackBar->Left = networkDownProgressBar->Left;
		networkDownTrackBar->Height = networkDownProgressBar->Height*2;
		networkDownTrackBar->Width = networkDownProgressBar->Width;
		networkDownUsedLabel->Top = networkDownProgressBar->Top;
		networkDownUsedLabel->Left = networkDownProgressBar->Right + 10;
		networkDownUsedTextBox->Top = networkDownUsedLabel->Top;
		networkDownUsedTextBox->Left = networkDownUsedLabel->Right;
		networkDownUsedTextBox->Width = networkUpUsedTextBox->Width;
		networkDownLimitLabel->Top = networkDownTrackBar->Top;
		networkDownLimitLabel->Left = networkDownUsedLabel->Left;
		networkDownLimitTextBox->Top = networkDownLimitLabel->Top;
		networkDownLimitTextBox->Left = networkDownLimitLabel->Right;
		networkDownLimitTextBox->Width = networkUpLimitTextBox->Width;
		networkDownUnitsLabel->Top = networkDownUsedTextBox->Top;
		networkDownUnitsLabel->Left = networkDownUsedTextBox->Right + 10;
		networkDownUnitsComboBox->Top =networkDownUnitsLabel->Top;
		networkDownUnitsComboBox->Left = networkDownUnitsLabel->Right;
		networkDownUnitsComboBox->Width = diskUnitsComboBox->Width + 10;
		networkDownPanel->Height = networkUpPanel->Height;

	}

	//*************************************************************************
	// Method:		onFocusChanged
	// Description: Called when focus changes in any ui pane
	//
	// Parameters:
	//	dataNode - node representing object that got focus
	//
	// Return Value: none
	//*************************************************************************
	void LimitPane::onFocusChanged(DisplayableDataNode *dataNode)
	{

		if (dataArray)
		{
			dataArray->onItemAdded -= itemAddedHandler;
			dataArray->onItemModified -= itemModifiedHandler;
			dataArray->onItemDeleted -= itemDeletedHandler;
		}

		if (dataNode && dataNode->processID != 0) 
		{
			processID = dataNode->processID;
			int index = dataArrayList->IndexOfKey(__box(processID.GetHashCode()));
			if (index >= 0)
			{
				dataArray = dynamic_cast <LimitPaneDataArray*> (dataArrayList->GetByIndex(index));
				
				itemAddedHandler = new ItemAddedDelegate(this, onItemAdded);
				dataArray->onItemAdded += itemAddedHandler;
				itemModifiedHandler = new ItemModifiedDelegate(this, onItemModified);
				dataArray->onItemModified += itemModifiedHandler;
				itemDeletedHandler = new ItemDeletedDelegate(this, onItemDeleted);
				dataArray->onItemDeleted += itemDeletedHandler;
				dataArray->Refresh();
			}
			else
				dataArray = NULL;

			diskTrackBar->Enabled = true;
			diskLimitTextBox->Enabled = true;
			diskUnitsComboBox->Enabled = true;
			memoryTrackBar->Enabled = true;
			memoryLimitTextBox->Enabled = true;
			memoryUnitsComboBox->Enabled = true;
			networkUpTrackBar->Enabled = true;
			networkUpLimitTextBox->Enabled = true;
			networkUpUnitsComboBox->Enabled = true;
			networkDownTrackBar->Enabled = true;
			networkDownLimitTextBox->Enabled = true;
			networkDownUnitsComboBox->Enabled = true;
		}
		else
		{
			diskTrackBar->Enabled = false;
			diskTrackBar->Value = 0;
			diskLimitTextBox->Enabled = false;
			diskLimitTextBox->Text = "";
			diskUsedTextBox->Text = "";
			diskUnitsComboBox->Enabled = false;
			memoryTrackBar->Enabled = false;
			memoryTrackBar->Value = 0;
			memoryLimitTextBox->Enabled = false;
			memoryLimitTextBox->Text = "";
			memoryUsedTextBox->Text = "";
			memoryUnitsComboBox->Enabled = false;
			networkUpTrackBar->Enabled = false;
			networkUpTrackBar->Value = 0;
			networkUpLimitTextBox->Enabled = false;
			networkUpLimitTextBox->Text = "";
			networkUpUsedTextBox->Text = "";
			networkUpUnitsComboBox->Enabled = false;
			networkDownTrackBar->Enabled = false;
			networkDownTrackBar->Value = 0;
			networkDownLimitTextBox->Enabled = false;
			networkDownLimitTextBox->Text = "";
			networkDownUsedTextBox->Text = "";
			networkDownUnitsComboBox->Enabled = false;
		}
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
	void LimitPane::onCloseProjectUI()
	{
		diskProgressBar->Value = 0;
		diskTrackBar->Enabled = false;
		diskTrackBar->Value = 0;
		diskLimitTextBox->Enabled = false;
		diskLimitTextBox->Text = "";
		diskUnitsComboBox->Enabled = false;
		diskUsedTextBox->Text = "";

		memoryProgressBar->Value = 0;
		memoryTrackBar->Enabled = false;
		memoryTrackBar->Value = 0;
		memoryLimitTextBox->Enabled = false;
		memoryLimitTextBox->Text = "";
		memoryUnitsComboBox->Enabled = false;
		memoryUsedTextBox->Text = "";

		networkUpProgressBar->Value = 0;
		networkUpTrackBar->Enabled = false;
		networkUpTrackBar->Value = 0;
		networkUpLimitTextBox->Enabled = false;
		networkUpLimitTextBox->Text = "";
		networkUpUnitsComboBox->Enabled = false;
		networkUpUsedTextBox->Text = "";

		networkDownProgressBar->Value = 0;
		networkDownTrackBar->Enabled = false;
		networkDownTrackBar->Value = 0;
		networkDownLimitTextBox->Enabled = false;
		networkDownLimitTextBox->Text = "";
		networkDownUnitsComboBox->Enabled = false;
		networkDownUsedTextBox->Text = "";

		// Clear out the data arrays
		if (dataArray)
		{
			dataArray->onItemAdded -= itemAddedHandler;
			dataArray->onItemModified -= itemModifiedHandler;
			dataArray->onItemDeleted -= itemDeletedHandler;
		}
		dataArray = NULL;
		ClearAll();
	}

	//*************************************************************************
	// Method:		diskTrackBarMouseDown
	// Description: Event handler for when a user clicks on the trackbar
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::diskTrackBarMouseDown(Object *sender, MouseEventArgs *e)
	{
		diskTrackBarFocus = true;
	}

	//*************************************************************************
	// Method:		diskTrackBarValueChanged
	// Description: Event handler for when the value of the disk trackbar changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::diskTrackBarValueChanged(Object *sender, MouseEventArgs *e)
	{
		Decimal multiplier;
		int trackValue = diskTrackBar->Value;
		UInt64 newValue, currentValue;
		
		if (diskLimitTextBox->Text->Length > 0)
		{
			try
			{
				currentValue = Convert::ToInt64(diskLimitTextBox->Text);
			}
			catch(...)
			{
				return;
			}

			if (trackValue == 100)
				newValue = realMaxDisk/ diskUnits;
			else if (trackValue == 0)
				newValue = 0;
			else
			{
				multiplier = Decimal::Divide(trackValue, 100);
				newValue = Convert::ToInt64(Decimal::Round((realMaxDisk * multiplier) / diskUnits, 0));
				//if units are large it may take several ticks to change the value
				while ( (newValue == currentValue) && (trackValue < 100) && (trackValue > 0) )
				{
					if ( (realMaxDisk * multiplier) > (maximumDiskUsage) )
						trackValue++;
					else
						trackValue--;
					multiplier = Decimal::Divide(trackValue, 100);
					newValue =  Convert::ToInt64(Decimal::Round((realMaxDisk * multiplier) / diskUnits, 0));
				}
				diskTrackBar->Value = trackValue;
			}

			diskLimitTextBox->Text = Convert::ToString(newValue);
			diskTrackBarFocus = false;
			diskLimitTextBoxTextChanged(sender, e);
		}

	}

	//*************************************************************************
	// Method:		diskLimitTextBoxGotFocus
	// Description: Event handler for when the limit box gets focus
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::diskLimitTextBoxGotFocus(Object *sender, EventArgs *e)
	{
		diskLimitTextBoxFocus = true;
	}

	//*************************************************************************
	// Method:		diskLimitTextBoxTextChanged
	// Description: Event handler for when the value of the disk limit box changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::diskLimitTextBoxTextChanged(Object *sender, EventArgs *e)
	{
		LimitPaneDataNode *dataNode = new LimitPaneDataNode("Disk Limit", UIData::DiskLimit, processID, 0);
		Object *args[] = __gc new Object*[1];

		try
		{
			if (diskLimitTextBox->Text->Length == 0)
				throw "Invalid Entry";
			maximumDiskUsage = Convert::ToUInt64(diskLimitTextBox->Text) * diskUnits;
		}
		catch(...)
		{
			diskLimitTextBox->Text = Convert::ToString(maximumDiskUsage/diskUnits);
			UserNotification::ErrorNotify("Please enter a number from 0 to the size of your disk drive");
			diskLimitTextBoxFocus = false;
			return;
		}
		if (maximumDiskUsage > realMaxDisk)
		{
			maximumDiskUsage = realMaxDisk;
		}
		else if (maximumDiskUsage < currentDiskUsage)
		{
			maximumDiskUsage = currentDiskUsage;
		}

		if (maximumDiskUsage == realMaxDisk)
		{
			if (dataNode->DeleteDelegate)
			{
				dataNode->LimitValue = realMaxDisk;
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeleteDelegate->DynamicInvoke(args);
			}
		}
		else 
		{
			if (!diskLimitEnabled)
			{
				if (dataNode->CreateDelegate)
				{
					dataNode->LimitValue = maximumDiskUsage;
					Object *args[] = __gc new Object*[1];
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(dataNode);
				}
			}
			else
			{
				if (dataNode->ModifyDelegate)
				{
					String *propertyName = "LimitValue";
					LimitPaneDataNode *modifiedDataNode = new LimitPaneDataNode(dataNode);
					modifiedDataNode->LimitValue = maximumDiskUsage;
					Object *args[] = __gc new Object*[3];
					args[0] = dataNode;
					args[1] = modifiedDataNode;
					args[2] = propertyName;
					dataNode->ModifyDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(modifiedDataNode);
				}
			}
		}
		diskLimitTextBoxFocus = false;
	}

	//*************************************************************************
	// Method:		diskLimitTextBoxKeyDown
	// Description: Event handler for when a key is pressed in a limit box
	//
	// Parameters:
	//	sender - object that sent the event
	//	e - event args representing the key press event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::diskLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e)
	{
		if (e->KeyCode == System::Windows::Forms::Keys::Return)
		{
			e->Handled = true;
			if (diskLimitTextBoxFocus)
			{
				diskLimitTextBoxTextChanged(sender, NULL);
				UpdateResourceMonitors();
				diskLimitTextBoxFocus = true;
			}
		}
	}

	//*************************************************************************
	// Method:		diskUnitsComboBoxIndexChanged
	// Description: Event handler for when the index of the diskunits combo changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::diskUnitsComboBoxIndexChanged(Object *sender, EventArgs *e)
	{
		switch (diskUnitsComboBox->SelectedIndex)
		{
		case 0:
			diskUnits = 1;	//2^0
			break;
		case 1:
			diskUnits = 1024;  //2^10
			break;
		case 2:
			diskUnits = 1024 * 1024; //2^20
			break;
		case 3:
			diskUnits = 1024 * 1024 * 1024;  //2^30;
			break;
		}
		UpdateResourceMonitors();
	}

	//*************************************************************************
	// Method:		memoryTrackBarMouseDown
	// Description: Event handler for when a user clicks on the trackbar
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::memoryTrackBarMouseDown(Object *sender, MouseEventArgs *e)
	{
		memoryTrackBarFocus = true;
	}

	//*************************************************************************
	// Method:		memoryTrackBarValueChanged
	// Description: Event handler for when the value of the memory trackbar changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::memoryTrackBarValueChanged(Object *sender, MouseEventArgs *e)
	{
		Decimal multiplier;
		int trackValue = memoryTrackBar->Value;
		UInt64 newValue, currentValue;
		
		if (memoryLimitTextBox->Text->Length > 0)
		{
			try
			{
				currentValue = Convert::ToInt64(memoryLimitTextBox->Text);
			}
			catch(...)
			{
				return;
			}

			if (trackValue == 100)
				newValue = realMaxMemory/ memoryUnits;
			else if (trackValue == 0)
				newValue = 0;
			else
			{
				multiplier = Decimal::Divide(trackValue, 100);
				newValue = Convert::ToInt64(Decimal::Round((realMaxMemory * multiplier) / memoryUnits, 0));
				//if units are large it may take several ticks to change the value
				while ( (newValue == currentValue) && (trackValue < 100) && (trackValue > 0) )
				{
					if ( (realMaxMemory * multiplier) > (maximumMemoryUsage) )
						trackValue++;
					else
						trackValue--;
					multiplier = Decimal::Divide(trackValue, 100);
					newValue =  Convert::ToInt64(Decimal::Round((realMaxMemory * multiplier) / memoryUnits, 0));
				}
				memoryTrackBar->Value = trackValue;
			}

			memoryLimitTextBox->Text = Convert::ToString(newValue);
			memoryTrackBarFocus = false;
			memoryLimitTextBoxTextChanged(sender, e);
		}
	}

	//*************************************************************************
	// Method:		memoryLimitTextBoxGotFocus
	// Description: Event handler for when the value of the memory limit gains focus
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::memoryLimitTextBoxGotFocus(Object *sender, EventArgs *e)
	{
		memoryLimitTextBoxFocus = true;
	}

	//*************************************************************************
	// Method:		memoryLimitTextBoxTextChanged
	// Description: Event handler for when the value of the memory limit box changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::memoryLimitTextBoxTextChanged(Object *sender, EventArgs *e)
	{
		LimitPaneDataNode *dataNode = new LimitPaneDataNode("Memory Limit", UIData::MemoryLimit, processID, 0);

		try
		{
			if (memoryLimitTextBox->Text->Length == 0)
				throw "Invalid Entry";
			maximumMemoryUsage = Convert::ToUInt64(memoryLimitTextBox->Text) * memoryUnits;
		}
		catch(...)
		{
			memoryLimitTextBox->Text = Convert::ToString(maximumMemoryUsage/memoryUnits);
			UserNotification::ErrorNotify("Please enter a number from 0 to the amount of available memory");
			memoryLimitTextBoxFocus = false;
			return;
		}
		if (maximumMemoryUsage > realMaxMemory)
		{
			maximumMemoryUsage = realMaxMemory;
		}
		else if (maximumMemoryUsage < currentMemoryUsage)
		{
			maximumMemoryUsage = currentMemoryUsage;
		}

		if (maximumMemoryUsage == realMaxMemory)
		{
  			if (dataNode->DeleteDelegate)
			{
				dataNode->LimitValue = realMaxMemory;
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeleteDelegate->DynamicInvoke(args);
			}
		}
		else 
		{
			if (!memoryLimitEnabled)
			{
				if (dataNode->CreateDelegate)
				{
					dataNode->LimitValue = maximumMemoryUsage;
					Object *args[] = __gc new Object*[1];
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(dataNode);
				}
			}
			else
			{
				if (dataNode->ModifyDelegate)
				{
					String *propertyName = "LimitValue";
					LimitPaneDataNode *modifiedDataNode = new LimitPaneDataNode(dataNode);
					modifiedDataNode->LimitValue = maximumMemoryUsage;
					Object *args[] = __gc new Object*[3];
					args[0] = dataNode;
					args[1] = modifiedDataNode;
					args[2] = propertyName;
					dataNode->ModifyDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(modifiedDataNode);
				}
			}
			
		}
		memoryLimitTextBoxFocus = false;
	}

	//*************************************************************************
	// Method:		memoryLimitTextBoxKeyDown
	// Description: Event handler for when a key is pressed in a limit box
	//
	// Parameters:
	//	sender - object that sent the event
	//	e - event args representing the key press event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::memoryLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e)
	{
		if (e->KeyCode == System::Windows::Forms::Keys::Return)
		{
			e->Handled = true;
			if (memoryLimitTextBoxFocus)
			{
				memoryLimitTextBoxTextChanged(sender, NULL);
				UpdateResourceMonitors();
				memoryLimitTextBoxFocus = true;
			}
		}
	}

	//*************************************************************************
	// Method:		memoryUnitsComboBoxIndexChanged
	// Description: Event handler for when the index of the memory units combo changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::memoryUnitsComboBoxIndexChanged(Object *sender, EventArgs *e)
	{
		switch (memoryUnitsComboBox->SelectedIndex)
		{
		case 0:
			memoryUnits = 1;	//2^1
			break;
		case 1:
			memoryUnits = 1024;  //2^10
			break;
		case 2:
			memoryUnits = 1024 * 1024; //2^20
			break;
		case 3:
			memoryUnits = 1024 * 1024 * 1024;  //2^30;
			break;
		}
		UpdateResourceMonitors();
	}


	//*************************************************************************
	// Method:		networkUpTrackBarMouseDown
	// Description: Event handler for when a user clicks on the trackbar
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkUpTrackBarMouseDown(Object *sender, MouseEventArgs *e)
	{
		networkUpTrackBarFocus = true;
	}

	//*************************************************************************
	// Method:		networkDownTrackBarMouseDown
	// Description: Event handler for when a user clicks on the trackbar
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkDownTrackBarMouseDown(Object *sender, MouseEventArgs *e)
	{
		networkDownTrackBarFocus = true;
	}

	//*************************************************************************
	// Method:		networkUpTrackBarValueChanged
	// Description: Event handler for when the value of the network trackbar changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkUpTrackBarValueChanged(Object *sender, MouseEventArgs *e)
	{
		Decimal multiplier;
		int trackValue = networkUpTrackBar->Value;
		UInt64 newValue, currentValue;
		
		if (networkUpLimitTextBox->Text->Length > 0)
		{
			try
			{
				currentValue = Convert::ToInt64(networkUpLimitTextBox->Text);
			}
			catch(...)
			{
				return;
			}

			if (trackValue == 100)
				newValue = realMaxNetworkUp/ networkUpUnits;
			else if (trackValue == 0)
				newValue = 0;
			else
			{
				multiplier = Decimal::Divide(trackValue, 100);
				newValue = Convert::ToInt64(Decimal::Round((realMaxNetworkUp * multiplier) / networkUpUnits, 0));
				//if units are large it may take several ticks to change the value
				while ( (newValue == currentValue) && (trackValue < 100) && (trackValue > 0) )
				{
					if ( (realMaxNetworkUp * multiplier) > (maximumNetworkUpUsage) )
						trackValue++;
					else
						trackValue--;
					multiplier = Decimal::Divide(trackValue, 100);
					newValue =  Convert::ToInt64(Decimal::Round((realMaxNetworkUp * multiplier) / networkUpUnits, 0));
				}
				networkUpTrackBar->Value = trackValue;
			}

			networkUpLimitTextBox->Text = Convert::ToString(newValue);
			networkUpTrackBarFocus = false;
			networkUpLimitTextBoxTextChanged(sender, e);
		}
	}

	//*************************************************************************
	// Method:		networkDownTrackBarValueChanged
	// Description: Event handler for when the value of the networkDown trackbar changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkDownTrackBarValueChanged(Object *sender, MouseEventArgs *e)
	{
		Decimal multiplier;
		int trackValue = networkDownTrackBar->Value;
		UInt64 newValue, currentValue;
		
		if (networkDownLimitTextBox->Text->Length > 0)
		{
			try
			{
				currentValue = Convert::ToInt64(networkDownLimitTextBox->Text);
			}
			catch(...)
			{
				return;
			}

			if (trackValue == 100)
				newValue = realMaxNetworkDown / networkDownUnits;
			else if (trackValue == 0)
				newValue = 0;
			else
			{
				multiplier = Decimal::Divide(trackValue, 100);
				newValue = Convert::ToInt64(Decimal::Round((realMaxNetworkDown * multiplier) / networkDownUnits, 0));
				//if units are large it may take several ticks to change the value
				while ( (newValue == currentValue) && (trackValue < 100) && (trackValue > 0) )
				{
					if ( (realMaxNetworkDown * multiplier) > (maximumNetworkDownUsage) )
						trackValue++;
					else
						trackValue--;
					multiplier = Decimal::Divide(trackValue, 100);
					newValue =  Convert::ToInt64(Decimal::Round((realMaxNetworkDown * multiplier) / networkDownUnits, 0));
				}
				networkDownTrackBar->Value = trackValue;
			}

			networkDownLimitTextBox->Text = Convert::ToString(newValue);
			networkDownTrackBarFocus = false;
			networkDownLimitTextBoxTextChanged(sender, e);
		}
	}

	//*************************************************************************
	// Method:		networkUpLimitTextBoxGotFocus
	// Description: Event handler for when the limit box gets focus
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkUpLimitTextBoxGotFocus(Object *sender, EventArgs *e)
	{
		networkUpLimitTextBoxFocus = true;
	}

	//*************************************************************************
	// Method:		networkDownLimitTextBoxGotFocus
	// Description: Event handler for when the limit box gets focus
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkDownLimitTextBoxGotFocus(Object *sender, EventArgs *e)
	{
		networkDownLimitTextBoxFocus = true;
	}

	//*************************************************************************
	// Method:		networkUpLimitTextBoxTextChanged
	// Description: Event handler for when the value of the networkUp limit box changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkUpLimitTextBoxTextChanged(Object *sender, EventArgs *e)
	{
		LimitPaneDataNode *dataNode = new LimitPaneDataNode("Network Upload Limit", UIData::NetworkUpLimit, processID, 0);

		try
		{
			if (networkUpLimitTextBox->Text->Length == 0)
				throw "Invalid Entry";
			maximumNetworkUpUsage = Convert::ToUInt64(networkUpLimitTextBox->Text) * networkUpUnits;
		}
		catch(...)
		{
			networkUpLimitTextBox->Text = Convert::ToString(maximumNetworkUpUsage/networkUpUnits);
			UserNotification::ErrorNotify("Please enter a number from 0 to the amount of available network upload bandwidth");
			networkUpLimitTextBoxFocus = false;
			return;
		}
		if (maximumNetworkUpUsage > realMaxNetworkUp)
		{
			maximumNetworkUpUsage = realMaxNetworkUp;
		}

		if (maximumNetworkUpUsage == realMaxNetworkUp)
		{
  			if (dataNode->DeleteDelegate)
			{
				dataNode->LimitValue = realMaxNetworkUp;
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeleteDelegate->DynamicInvoke(args);
			}
		}
		else 
		{
			if (!networkUpLimitEnabled)
			{
				if (dataNode->CreateDelegate)
				{
					dataNode->LimitValue = maximumNetworkUpUsage;
					Object *args[] = __gc new Object*[1];
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(dataNode);
				}
			}
			else
			{
				if (dataNode->ModifyDelegate)
				{
					String *propertyName = "LimitValue";
					LimitPaneDataNode *modifiedDataNode = new LimitPaneDataNode(dataNode);
					modifiedDataNode->LimitValue = maximumNetworkUpUsage;
					Object *args[] = __gc new Object*[3];
					args[0] = dataNode;
					args[1] = modifiedDataNode;
					args[2] = propertyName;
					dataNode->ModifyDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(modifiedDataNode);
				}
			}
			
		}
		networkUpLimitTextBoxFocus = false;
	}

	//*************************************************************************
	// Method:		networkUpLimitTextBoxKeyDown
	// Description: Event handler for when a key is pressed in a limit box
	//
	// Parameters:
	//	sender - object that sent the event
	//	e - event args representing the key press event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkUpLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e)
	{
		if (e->KeyCode == System::Windows::Forms::Keys::Return)
		{
			e->Handled = true;
			if (networkUpLimitTextBoxFocus)
			{
				networkUpLimitTextBoxTextChanged(sender, NULL);
				UpdateResourceMonitors();
				networkUpLimitTextBoxFocus = true;
			}
		}
	}

	//*************************************************************************
	// Method:		networkUpUnitsComboBoxIndexChanged
	// Description: Event handler for when the index of the network upload units combo changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkUpUnitsComboBoxIndexChanged(Object *sender, EventArgs *e)
	{
		switch (networkUpUnitsComboBox->SelectedIndex)
		{
		case 0:
			networkUpUnits = 1;	//Bits
			break;
		case 1:
			networkUpUnits = 1000;  //KBits
			break;
		case 2:
			networkUpUnits = 1000 * 1000; //MBits
			break;
		}
		UpdateResourceMonitors();
	}

	//*************************************************************************
	// Method:		networkDownLimitTextBoxTextChanged
	// Description: Event handler for when the value of the networkDown limit box changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkDownLimitTextBoxTextChanged(Object *sender, EventArgs *e)
	{
		LimitPaneDataNode *dataNode = new LimitPaneDataNode("Network Download Limit", UIData::NetworkDownLimit, processID, 0);

		try
		{
			if (networkDownLimitTextBox->Text->Length == 0)
				throw "Invalid Entry";
			maximumNetworkDownUsage = Convert::ToUInt64(networkDownLimitTextBox->Text) * networkDownUnits;
		}
		catch(...)
		{
			networkDownLimitTextBox->Text = Convert::ToString(maximumNetworkDownUsage/networkDownUnits);
			UserNotification::ErrorNotify("Please enter a number from 0 to the amount of available network download bandwidth");
			networkDownLimitTextBoxFocus = false;
			return;
		}
		if (maximumNetworkDownUsage > realMaxNetworkDown)
		{
			maximumNetworkDownUsage = realMaxNetworkDown;
		}

		if (maximumNetworkDownUsage == realMaxNetworkDown)
		{
  			if (dataNode->DeleteDelegate)
			{
				dataNode->LimitValue = realMaxNetworkDown;
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeleteDelegate->DynamicInvoke(args);
			}
		}
		else 
		{
			if (!networkDownLimitEnabled)
			{
				if (dataNode->CreateDelegate)
				{
					dataNode->LimitValue = maximumNetworkDownUsage;
					Object *args[] = __gc new Object*[1];
					args[0] = dataNode;
					dataNode->CreateDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(dataNode);
				}
			}
			else
			{
				if (dataNode->ModifyDelegate)
				{
					String *propertyName = "LimitValue";
					LimitPaneDataNode *modifiedDataNode = new LimitPaneDataNode(dataNode);
					modifiedDataNode->LimitValue = maximumNetworkDownUsage;
					Object *args[] = __gc new Object*[3];
					args[0] = dataNode;
					args[1] = modifiedDataNode;
					args[2] = propertyName;
					dataNode->ModifyDelegate->DynamicInvoke(args);
					uiEventRegistry->OnFocusChanged->Invoke(modifiedDataNode);
				}
			}
			
		}
		networkDownLimitTextBoxFocus = false;
	}

	//*************************************************************************
	// Method:		networkDownLimitTextBoxKeyDown
	// Description: Event handler for when a key is pressed in a limit box
	//
	// Parameters:
	//	sender - object that sent the event
	//	e - event args representing the key press event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkDownLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e)
	{
		if (e->KeyCode == System::Windows::Forms::Keys::Return)
		{
			e->Handled = true;
			if (networkDownLimitTextBoxFocus)
			{
				networkDownLimitTextBoxTextChanged(sender, NULL);
				UpdateResourceMonitors();
				networkDownLimitTextBoxFocus = true;
			}
		}
	}

	//*************************************************************************
	// Method:		networkDownUnitsComboBoxIndexChanged
	// Description: Event handler for when the index of the network upload units combo changes
	//
	// Parameters:
	//	sender - object that sent the event
	//	LayoutEventArgs - event args representing the layout change event
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::networkDownUnitsComboBoxIndexChanged(Object *sender, EventArgs *e)
	{
		switch (networkDownUnitsComboBox->SelectedIndex)
		{
		case 0:
			networkDownUnits = 1;	//Bits
			break;
		case 1:
			networkDownUnits = 1000;  //KBits
			break;
		case 2:
			networkDownUnits = 1000 * 1000; //MBits
			break;
		}
		UpdateResourceMonitors();
	}

	//*************************************************************************
	// Method:		UpdateResourceMonitors
	// Description: called to update the resource monitors with new information
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::UpdateResourceMonitors()
	{
		Decimal Numerator, Denominator, Result;
		RegistrationMgr *regMgr = RegistrationMgr::GetInstance();

		try
		{
			//set trackbars and limit edit boxes
			if (dataArray)
			{
				dataArray->GetRealSpaceInformation(realMaxDisk, realMaxMemory);
				dataArray->GetMonitorInformation(maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage);

				if (!regMgr->IsFeatureDisabledTrial())
				{
					if ( (!diskTrackBarFocus) && (!diskLimitTextBoxFocus) )
					{
						if (realMaxDisk == 0)
						{
							diskTrackBar->Value = 0;
							diskLimitTextBox->Text = "0";
						}
						else 
						{
							if (realMaxDisk < maximumDiskUsage)							
								maximumDiskUsage = realMaxDisk;
								
							Numerator = maximumDiskUsage;
							Denominator = realMaxDisk;
							Result = Decimal::Divide(Numerator, Denominator);
							Result = Decimal::Round(Result * 100, 0);
							diskTrackBar->Value = Decimal::ToInt16(Result);
							diskLimitTextBox->Text = Convert::ToString(maximumDiskUsage/diskUnits);
						}
					}
				}

				if ( (!memoryTrackBarFocus) && (!memoryLimitTextBoxFocus) )
				{
					if (realMaxMemory == 0)
					{
						memoryTrackBar->Value = 0;
						memoryLimitTextBox->Text = "0";
					}
					else 
					{
						if (realMaxMemory < maximumMemoryUsage)
							maximumMemoryUsage = realMaxMemory;
						Numerator = maximumMemoryUsage;
						Denominator = realMaxMemory;
						Result = Decimal::Divide(Numerator, Denominator);
						Result = Decimal::Round(Result * 100, 0);
						memoryTrackBar->Value = Decimal::ToInt16(Result);
						memoryLimitTextBox->Text = Convert::ToString(maximumMemoryUsage/memoryUnits);
					}
				}

				if (!regMgr->IsFeatureDisabledTrial())
				{
					dataArray->GetRealMaxNetwork(realMaxNetworkUp, realMaxNetworkDown);
					if (!dataArray->GetNetworkUploadBandwidth(maximumNetworkUpUsage))
						maximumNetworkUpUsage = realMaxNetworkUp;

					if (!dataArray->GetNetworkDownloadBandwidth(maximumNetworkDownUsage))
						maximumNetworkDownUsage = realMaxNetworkDown;

					if (!dataArray->GetNetworkMonitorInformation(currentNetworkUpUsage, currentNetworkDownUsage))
					{
						currentNetworkUpUsage = 0;
						currentNetworkDownUsage = 0;
					}

					if ( (!networkUpTrackBarFocus) && (!networkUpLimitTextBoxFocus) )
					{
						if ((realMaxNetworkUp == 0) || (realMaxNetworkUp < maximumNetworkUpUsage))
						{
							networkUpTrackBar->Value = 0;
							networkUpLimitTextBox->Text = 0;
						}
						else 
						{
							Numerator = maximumNetworkUpUsage;
							Denominator = realMaxNetworkUp;
							Result = Decimal::Divide(Numerator, Denominator);
							Result = Decimal::Round(Result * 100, 0);
							networkUpTrackBar->Value = Decimal::ToInt16(Result);
							networkUpLimitTextBox->Text = Convert::ToString(maximumNetworkUpUsage/networkUpUnits);
						}
					}
						
					if ( (!networkDownTrackBarFocus) && (!networkDownLimitTextBoxFocus) )
					{
						if ((realMaxNetworkDown == 0) || (realMaxNetworkDown < maximumNetworkDownUsage))
						{
							networkDownTrackBar->Value = 0;
							networkDownLimitTextBox->Text = "0";
						}
						else 
						{
							Numerator = maximumNetworkDownUsage;
							Denominator = realMaxNetworkDown;
							Result = Decimal::Divide(Numerator, Denominator);
							Result = Decimal::Round(Result * 100, 0);
							networkDownTrackBar->Value = Decimal::ToInt16(Result);
							networkDownLimitTextBox->Text = Convert::ToString(maximumNetworkDownUsage/networkDownUnits);
						}
					}

					//set progressbar and used edit box
					if ((maximumDiskUsage == 0) || (maximumDiskUsage < currentDiskUsage))
					{
						diskProgressBar->Value = 0;
						diskUsedTextBox->Text = "0";
					}
					else
					{
						Numerator = currentDiskUsage;
						Denominator = maximumDiskUsage;
						Result = Decimal::Divide(Numerator, Denominator);
						Result = Decimal::Round(Result * 100, 0);
						diskProgressBar->Value = Decimal::ToInt16(Result);
						diskUsedTextBox->Text = Convert::ToString(currentDiskUsage/diskUnits);
					}
				}

				if ((maximumMemoryUsage == 0) || (maximumMemoryUsage < currentMemoryUsage))
				{
					memoryProgressBar->Value = 0;
					memoryUsedTextBox->Text = "0";
				}
				else
				{
					Numerator = currentMemoryUsage;
					Denominator = maximumMemoryUsage;
					Result = Decimal::Divide(Numerator, Denominator);
					Result = Decimal::Round(Result * 100, 0);
					memoryProgressBar->Value = Decimal::ToInt16(Result);
					memoryUsedTextBox->Text = Convert::ToString(currentMemoryUsage/memoryUnits);
				}

				if (!regMgr->IsFeatureDisabledTrial())
				{
					if ((realMaxNetworkUp == 0) || (realMaxNetworkUp < currentNetworkUpUsage))
					{
						networkUpProgressBar->Value = 0;
						networkUpUsedTextBox->Text = "0";
					}
					else
					{
						Numerator = currentNetworkUpUsage;
						Denominator = maximumNetworkUpUsage;
						Result = Decimal::Divide(Numerator, Denominator);
						Result = Decimal::Round(Result * 100, 0);
						networkUpProgressBar->Value = Decimal::ToInt16(Result);
						networkUpUsedTextBox->Text = Convert::ToString(currentNetworkUpUsage/networkUpUnits);
					}

					if ((realMaxNetworkDown == 0) || (realMaxNetworkDown < currentNetworkDownUsage))
					{
						networkDownProgressBar->Value = 0;
						networkDownUsedTextBox->Text = "0";
					}
					else
					{
						Numerator = currentNetworkDownUsage;
						Denominator = maximumNetworkDownUsage;
						Result = Decimal::Divide(Numerator, Denominator);
						Result = Decimal::Round(Result * 100, 0);
						networkDownProgressBar->Value = Decimal::ToInt16(Result);
						networkDownUsedTextBox->Text = Convert::ToString(currentNetworkDownUsage/networkDownUnits);
					}
				}
			}

			if (!regMgr->IsFeatureDisabledTrial())
			{
				if (realMaxDisk == maximumDiskUsage)
					diskLimitEnabled = false;
				else
					diskLimitEnabled = true;
			}

			if (realMaxMemory == maximumMemoryUsage)
				memoryLimitEnabled = false;
			else
				memoryLimitEnabled = true;

			if (!regMgr->IsFeatureDisabledTrial())
			{
				if (realMaxNetworkUp == maximumNetworkUpUsage)
					networkUpLimitEnabled = false;
				else
					networkUpLimitEnabled = true;

				if (realMaxNetworkDown == maximumNetworkDownUsage)
					networkDownLimitEnabled = false;
				else
					networkDownLimitEnabled = true;
			}
		}
		catch (...)
		{
			//do nothing
		}
	}

	//*************************************************************************
	// Method:		onTimerTick
	// Description: Called to update the monitors
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::onTimerTick(Object *sender, EventArgs *e)
	{
		UpdateResourceMonitors();
	}

	//*************************************************************************
	// Method:		onItemDeleted
	// Description: Item deleted handler
	//
	// Parameters:
	//	dataNode - represents the limit that was deleted
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::onItemDeleted(DisplayableDataNode *dataNode)
	{
		UpdateResourceMonitors();
	}

	//*************************************************************************
	// Method:		onItemAdded
	// Description: Item added handler
	//
	// Parameters:
	//	dataNode - represents the limit that was added
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::onItemAdded(DisplayableDataNode *dataNode)
	{
		UpdateResourceMonitors();
	}

	//*************************************************************************
	// Method:		onItemModified
	// Description: Item modified handler
	//
	// Parameters:
	//	dataNode - represents the limit that was modified
	//
	// Return Value: None
	//*************************************************************************
	void LimitPane::onItemModified(DisplayableDataNode *dataNode, String *propertyName)
	{
		UpdateResourceMonitors();
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
	void LimitPane::AddDataArray(LimitPaneDataArray *data)
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
	void LimitPane::ClearAll()
	{
		dataArrayList->Clear();
	}
}