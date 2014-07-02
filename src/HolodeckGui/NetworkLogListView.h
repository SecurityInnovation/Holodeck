//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogListView.h
//
// DESCRIPTION: Contains definition for the class NetworkLogListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <SIControls.dll>

#include <stdio.h>
#include "TimeStampFilterForm.h"
#include "GenericListFilterForm.h"
#include "NumericRangeFilterForm.h"

using namespace SIControls;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	public __delegate void NetworkLogFilterUpdateDelegate();

	//*********************************************************************
	// defines the network log panes' ListView control
	//*********************************************************************
	public __gc class NetworkLogListView : public SIControls::FilterListView
	{
	private:
		ArrayList *allColumns;
		ToggleColumnHeader *timeStampHeader;
		ToggleColumnHeader *threadIDHeader;
		ToggleColumnHeader *incomingHeader;
		ToggleColumnHeader *protocolHeader;
		ToggleColumnHeader *msgLengthHeader;
		ToggleColumnHeader *destIPHeader;
		ToggleColumnHeader *destPortHeader;
		ToggleColumnHeader *sourceIPHeader;
		ToggleColumnHeader *sourcePortHeader;
		ToggleColumnHeader *dataPreviewHeader;

		FunctionDatabase::InterceptedFunctionDB * interceptedFunctionDB;

		EventHandler *Filter_OKClick;

	public:
		NetworkLogListView();
		~NetworkLogListView();

		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_TimeStampHeader() { return timeStampHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_ThreadIDHeader() { return threadIDHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_IncomingHeader() { return incomingHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_ProtocolHeader() { return protocolHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_MessageLengthHeader() { return msgLengthHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_DestinationIPHeader() { return destIPHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_DestinationPortHeader() { return destPortHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_SourceIPHeader() { return sourceIPHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_SourcePortHeader() { return sourcePortHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_DataPreviewHeader() { return dataPreviewHeader; }

		void customfilter_TimeStamp(System::Object * sender, EventArgs * e);
		void customfilter_ThreadID(System::Object * sender, EventArgs * e);
		void customfilter_Incoming(System::Object * sender, EventArgs * e);
		void customfilter_Protocol(System::Object * sender, EventArgs * e);
		void customfilter_MsgLength(System::Object * sender, EventArgs * e);
		void customfilter_DestinationIP(System::Object * sender, EventArgs * e);
		void customfilter_DestinationPort(System::Object * sender, EventArgs * e);
		void customfilter_SourceIP(System::Object * sender, EventArgs * e);
		void customfilter_SourcePort(System::Object * sender, EventArgs * e);
		
		void filter_OKClick(System::Object * sender, EventArgs * e);
		void customfilter_ThreadID_RefreshValues(System::Object * sender, EventArgs * e);
		void customfilter_Incoming_RefreshValues(System::Object * sender, EventArgs * e);
		void customfilter_Protocol_RefreshValues(System::Object * sender, EventArgs * e);
		void customfilter_DestinationIP_RefreshValues(System::Object * sender, EventArgs * e);
		void customfilter_SourceIP_RefreshValues(System::Object * sender, EventArgs * e);

		bool EnableColumnSorting(ToggleColumnHeader *column) { return (column != dataPreviewHeader); }

		TimeStampFilterForm *timeStampFilterForm;
		GenericListFilterForm *threadIDFilterForm;
		GenericListFilterForm *incomingFilterForm;
		GenericListFilterForm *protocolFilterForm;
		NumericRangeFilterForm *msgLengthFilterForm;
		GenericListFilterForm *destinationIPFilterForm;
		NumericRangeFilterForm *destinationPortFilterForm;
		GenericListFilterForm *sourceIPFilterForm;
		NumericRangeFilterForm *sourcePortFilterForm;

		NetworkLogFilterUpdateDelegate * OnFilterUpdate;
	};
}