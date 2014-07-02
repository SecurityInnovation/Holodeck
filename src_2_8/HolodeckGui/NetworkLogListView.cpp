//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogListView.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkLogListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#include "NetworkLogListView.h"
#include "defines.h"
#include "NetworkLogPane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkLogListView
	// Description: Constructor for the NetworkLogListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogListView::NetworkLogListView()
	{
		this->FullRowSelect = true;
		this->MultiSelect = false;
		this->AllowColumnReorder = true;
		this->HideSelection = false;
		this->Visible = true;

		Filter_OKClick = new EventHandler (this, filter_OKClick);

		allColumns = new ArrayList();

		timeStampHeader = new ToggleColumnHeader();
		timeStampHeader->Text = TIME_STAMP_HEADER_TEXT;
		timeStampHeader->Width = 160;

		threadIDHeader = new ToggleColumnHeader();
		threadIDHeader->Text = THREAD_ID_HEADER_TEXT;
		threadIDHeader->Width = 50;

		incomingHeader = new ToggleColumnHeader();
		incomingHeader->Text = DIRECTION_HEADER_TEXT;
		incomingHeader->Width = 60;

		protocolHeader = new ToggleColumnHeader();
		protocolHeader->Text = PROTOCOL_HEADER_TEXT;
		protocolHeader->Width = 140;

		msgLengthHeader = new ToggleColumnHeader();
		msgLengthHeader->Text = MSG_LENGTH_HEADER_TEXT;
		msgLengthHeader->Width = 75;

		destIPHeader = new ToggleColumnHeader();
		destIPHeader->Text = DESTINATION_IP_HEADER_TEXT;
		destIPHeader->Width = 100;

		destPortHeader = new ToggleColumnHeader();
		destPortHeader->Text = DESTINATION_PORT_HEADER_TEXT;
		destPortHeader->Width = 90;

		sourceIPHeader = new ToggleColumnHeader();
		sourceIPHeader->Text = SOURCE_IP_HEADER_TEXT;
		sourceIPHeader->Width = 100;

		sourcePortHeader = new ToggleColumnHeader();
		sourcePortHeader->Text = SOURCE_PORT_HEADER_TEXT;
		sourcePortHeader->Width = 80;

		dataPreviewHeader = new ToggleColumnHeader();
		dataPreviewHeader->Text = MSG_DATA_HEADER_TEXT;
		dataPreviewHeader->Width = 100;

		this->AddColumnHeader (timeStampHeader, new EventHandler (this, customfilter_TimeStamp));
		this->AddColumnHeader (threadIDHeader, new EventHandler (this, customfilter_ThreadID));
		this->AddColumnHeader (incomingHeader, new EventHandler (this, customfilter_Incoming));
		this->AddColumnHeader (protocolHeader, new EventHandler (this, customfilter_Protocol));
		this->AddColumnHeader (msgLengthHeader, new EventHandler(this, customfilter_MsgLength));
		this->AddColumnHeader (destIPHeader, new EventHandler (this, customfilter_DestinationIP));
		this->AddColumnHeader (destPortHeader, new EventHandler (this, customfilter_DestinationPort));
		this->AddColumnHeader (sourceIPHeader, new EventHandler (this, customfilter_SourceIP));
		this->AddColumnHeader (sourcePortHeader, new EventHandler (this, customfilter_SourcePort));
		this->AddColumnHeader (dataPreviewHeader, NULL);

		allColumns->Add(timeStampHeader);
		allColumns->Add(threadIDHeader);
		allColumns->Add(incomingHeader);
		allColumns->Add(protocolHeader);
		allColumns->Add(msgLengthHeader);
		allColumns->Add(destIPHeader);
		allColumns->Add(destPortHeader);
		allColumns->Add(sourceIPHeader);
		allColumns->Add(sourcePortHeader);
		allColumns->Add(dataPreviewHeader);
		
		// call the custom filter methods so that they create the form objects
		this->customfilter_TimeStamp(NULL, EventArgs::Empty);
		this->customfilter_ThreadID(NULL, EventArgs::Empty);
		this->customfilter_Incoming(NULL, EventArgs::Empty);
		this->customfilter_Protocol(NULL, EventArgs::Empty);
		this->customfilter_MsgLength(NULL, EventArgs::Empty);
		this->customfilter_DestinationIP(NULL, EventArgs::Empty);
		this->customfilter_DestinationPort(NULL, EventArgs::Empty);
		this->customfilter_SourceIP(NULL, EventArgs::Empty);
		this->customfilter_SourcePort(NULL, EventArgs::Empty);
	}

	//*************************************************************************
	// Method:		~NetworkLogListView
	// Description: Destructor for the NetworkLogListView class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogListView::~NetworkLogListView()
	{
	}

	//*************************************************************************
	// Method:		customfilter_timeStamp
	// Description: The custom filter method for TimeStamp
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_TimeStamp(System::Object * sender, EventArgs * e)
	{
		if (timeStampFilterForm == NULL)
			timeStampFilterForm = new TimeStampFilterForm(Filter_OKClick, NULL);

		if (sender != NULL)
		{
			DialogResult result = timeStampFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				timeStampHeader->TextColor = timeStampFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_ThreadID
	// Description: The custom filter method for ThreadID
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_ThreadID(System::Object * sender, EventArgs * e)
	{
		if (threadIDFilterForm == NULL)
			threadIDFilterForm = new GenericListFilterForm (NULL, NULL, "Available Threads", "Include only these",
													false, "Thread Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_ThreadID_RefreshValues));
		if (sender != NULL)
		{
			DialogResult result = threadIDFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				threadIDHeader->TextColor = threadIDFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_Incoming
	// Description: The custom filter method for Incoming
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_Incoming(System::Object * sender, EventArgs * e)
	{
		if (incomingFilterForm == NULL)
			incomingFilterForm = new GenericListFilterForm (NULL, NULL, "Available Directions", "Include only these",
													false, "Direction Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_Incoming_RefreshValues));
		if (sender != NULL)
		{
			DialogResult result = incomingFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				incomingHeader->TextColor = incomingFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_Protocol
	// Description: The custom filter method for Protocol
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_Protocol(System::Object * sender, EventArgs * e)
	{
		if (protocolFilterForm == NULL)
			protocolFilterForm = new GenericListFilterForm (NULL, NULL, "Available Protocols", "Include only these",
													false, "Protocol Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_Protocol_RefreshValues));
		if (sender != NULL)
		{
			DialogResult result = protocolFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				protocolHeader->TextColor = protocolFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_MsgLength
	// Description: The custom filter method for Message Length
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_MsgLength(System::Object * sender, EventArgs * e)
	{
		if (msgLengthFilterForm == NULL)
		{
			msgLengthFilterForm = new NumericRangeFilterForm (Filter_OKClick, NULL);
			msgLengthFilterForm->MinNumericValue = 0;
			msgLengthFilterForm->MaxNumericValue = 4294967295;
			msgLengthFilterForm->Text = "Network Message Length Filter";
		}

		if (sender != NULL)
		{
			DialogResult result = msgLengthFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				msgLengthHeader->TextColor = msgLengthFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_DestinationIP
	// Description: The custom filter method for DestinationIP
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_DestinationIP(System::Object * sender, EventArgs * e)
	{
		if (destinationIPFilterForm == NULL)
			destinationIPFilterForm = new GenericListFilterForm (NULL, NULL, "Available IP Addresses", "Include only these",
													false, "IP Address Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_DestinationIP_RefreshValues));
		if (sender != NULL)
		{
			DialogResult result = destinationIPFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				destIPHeader->TextColor = destinationIPFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_DestinationPort
	// Description: The custom filter method for DestinationPort
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_DestinationPort(System::Object * sender, EventArgs * e)
	{
		if (destinationPortFilterForm == NULL)
		{
			destinationPortFilterForm = new NumericRangeFilterForm (Filter_OKClick, NULL);
			destinationPortFilterForm->MinNumericValue = 0;
			destinationPortFilterForm->MaxNumericValue = 65535;
			destinationPortFilterForm->Text = "Destination Port Filter";
		}

		if (sender != NULL)
		{
			DialogResult result = destinationPortFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				destPortHeader->TextColor = destinationPortFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_SourceIP
	// Description: The custom filter method for Source IP
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_SourceIP(System::Object * sender, EventArgs * e)
	{
		if (sourceIPFilterForm == NULL)
			sourceIPFilterForm = new GenericListFilterForm (NULL, NULL, "Available IP Addresses", "Include only these",
													false, "IP Address Filter", Filter_OKClick, NULL, new EventHandler (this, customfilter_SourceIP_RefreshValues));
		if (sender != NULL)
		{
			DialogResult result = sourceIPFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				sourceIPHeader->TextColor = sourceIPFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//*************************************************************************
	// Method:		customfilter_SourcePort
	// Description: The custom filter method for Source Port
	//				Called by FilterListView
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_SourcePort(System::Object * sender, EventArgs * e)
	{
		if (sourcePortFilterForm == NULL)
		{
			sourcePortFilterForm = new NumericRangeFilterForm (Filter_OKClick, NULL);
			sourcePortFilterForm->MinNumericValue = 0;
			sourcePortFilterForm->MaxNumericValue = 65535;
			sourcePortFilterForm->Text = "Destination Port Filter";
		}

		if (sender != NULL)
		{
			DialogResult result = sourcePortFilterForm->ShowDialog(this);
			if (result == DialogResult::OK)
				sourcePortHeader->TextColor = sourcePortFilterForm->FilterEnabled ? Color::Blue : Color::Empty;
		}
	}

	//Event Handlers

	//*************************************************************************
	// Method:		filter_OKClick
	// Description: Called whenever an OK button is clicked on any filter form
	//				Responsible for invoking the FilterUpdate delegate.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::filter_OKClick(System::Object * sender, EventArgs * e)
	{
		this->OnFilterUpdate->Invoke();
	}

	//*************************************************************************
	// Method:		customfilter_ThreadID_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_ThreadID_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->threadIDFilterForm->Visible)
		{
			NetworkLogPane *logPane = dynamic_cast <NetworkLogPane *> (this->Parent);
			ArrayList *threadIDList = new ArrayList();
			String *itemText;

			//Get the index of the threadID header
			int tidColumnIndex = this->Columns->IndexOf (this->threadIDHeader);

			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; threadIDList->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				ContainerListViewItem *item = logPane->OnQueryContainerListViewItem(i);
				if (!item)
					continue;

				// minus 1 because of the text field of the item whereas tidColumnIndex is the column id
				itemText = item->SubItems->get_Item(tidColumnIndex - 1)->ToString();
				if (!threadIDList->Contains (itemText))
					threadIDList->Add (itemText);
			}
			
			// sort the values
			threadIDList->Sort ();
			
			// convert to an array
			Array *tidArray = threadIDList->ToArray();

			this->threadIDFilterForm->AvailableList = tidArray;
		}
	}

	//*************************************************************************
	// Method:		customfilter_Incoming_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_Incoming_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->incomingFilterForm->Visible)
		{
			NetworkLogPane *logPane = dynamic_cast <NetworkLogPane *> (this->Parent);
			ArrayList *incomingList = new ArrayList();
			String *itemText;

			//Get the index of the incoming header
			int incomingColumnIndex = this->Columns->IndexOf (this->incomingHeader);

			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; incomingList->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				ContainerListViewItem *item = logPane->OnQueryContainerListViewItem(i);
				if (!item)
					continue;

				// minus 1 because of the text field of the item whereas the column index is the column id
				itemText = item->SubItems->get_Item(incomingColumnIndex - 1)->ToString();
				if (!incomingList->Contains (itemText))
					incomingList->Add (itemText);
			}
			
			// sort the values
			incomingList->Sort ();
			
			// convert to an array
			Array *incomingArray = incomingList->ToArray();

			this->incomingFilterForm->AvailableList = incomingArray;
		}
	}

	//*************************************************************************
	// Method:		customfilter_Protocol_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_Protocol_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->protocolFilterForm->Visible)
		{
			NetworkLogPane *logPane = dynamic_cast <NetworkLogPane *> (this->Parent);
			ArrayList *protocolList = new ArrayList();
			String *itemText;

			//Get the index of the incoming header
			int protocolColumnIndex = this->Columns->IndexOf (this->protocolHeader);

			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; protocolList->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				ContainerListViewItem *item = logPane->OnQueryContainerListViewItem(i);
				if (!item)
					continue;

				// minus 1 because of the text field of the item whereas the column index is the column id
				itemText = item->SubItems->get_Item(protocolColumnIndex - 1)->ToString();
				if (!protocolList->Contains (itemText))
					protocolList->Add (itemText);
			}
			
			// sort the values
			protocolList->Sort ();
			
			// convert to an array
			Array *protocolArray = protocolList->ToArray();

			this->protocolFilterForm->AvailableList = protocolArray;
		}
	}

	//*************************************************************************
	// Method:		customfilter_SourceIP_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_SourceIP_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->sourceIPFilterForm->Visible)
		{
			NetworkLogPane *logPane = dynamic_cast <NetworkLogPane *> (this->Parent);
			ArrayList *sourceIPList = new ArrayList();
			String *itemText;

			//Get the index of the incoming header
			int sourceIPColumnIndex = this->Columns->IndexOf (this->sourceIPHeader);

			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; sourceIPList->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				ContainerListViewItem *item = logPane->OnQueryContainerListViewItem(i);
				if (!item)
					continue;

				// minus 1 because of the text field of the item whereas the column index is the column id
				itemText = item->SubItems->get_Item(sourceIPColumnIndex - 1)->ToString();
				if (!sourceIPList->Contains (itemText))
					sourceIPList->Add (itemText);
			}
			
			// sort the values
			sourceIPList->Sort ();
			
			// convert to an array
			Array *sourceIPArray = sourceIPList->ToArray();

			this->sourceIPFilterForm->AvailableList = sourceIPArray;
		}
	}

	//*************************************************************************
	// Method:		customfilter_DestinationIP_RefreshValues
	// Description: Called whenever the visible state of the form is changed.
	//				Used to update "Available" values in the form.
	//
	// Parameters:
	//		sender	- the calling object
	//		e		- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogListView::customfilter_DestinationIP_RefreshValues(System::Object * sender, EventArgs * e)
	{
		if (this->destinationIPFilterForm->Visible)
		{
			NetworkLogPane *logPane = dynamic_cast <NetworkLogPane *> (this->Parent);
			ArrayList *destinationIPList = new ArrayList();
			String *itemText;

			//Get the index of the incoming header
			int destinationIPColumnIndex = this->Columns->IndexOf (this->destIPHeader);

			//Get the last MAX_DISPLAY_RETURNVALS return values from the current log
			for (int i=0; destinationIPList->Count < MAX_DISPLAY_RETURNVALS && i < logPane->LogCount; i++)
			{
				ContainerListViewItem *item = logPane->OnQueryContainerListViewItem(i);
				if (!item)
					continue;

				// minus 1 because of the text field of the item whereas the column index is the column id
				itemText = item->SubItems->get_Item(destinationIPColumnIndex - 1)->ToString();
				if (!destinationIPList->Contains (itemText))
					destinationIPList->Add (itemText);
			}
			
			// sort the values
			destinationIPList->Sort ();
			
			// convert to an array
			Array *destinationIPArray = destinationIPList->ToArray();

			this->destinationIPFilterForm->AvailableList = destinationIPArray;
		}
	}
}