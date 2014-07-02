//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogPaneDataNode.h
//
// DESCRIPTION: Contains definition for the class NetworkLogPaneDataNode
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 29 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "DisplayableDataNode.h"
#include "defines.h"

#using <System.dll>
#using <mscorlib.dll>
#using <HoloScript.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace GuiSupport;
using namespace System::Collections;
using namespace System::ComponentModel;

using namespace HoloScript;

namespace UIData
{
	public __gc class NetworkLogPaneDataNode : public DisplayableDataNode
	{
	protected:
		NetworkLogItem *logItem;

	public:
		NetworkLogPaneDataNode(NetworkLogItem *item);
		~NetworkLogPaneDataNode();

		[Browsable(false)]
		__property NetworkLogItem *get_LogEntry() { return logItem; }

		//bugbug: need to figure out how to write a TypeConverter in managed C++ to expand LogEntry property automatically
		[ReadOnly(true), Description("The time at which the data was handled"), Category("Data Info")]
		__property String *get_TimeStamp() { return logItem->TimeStamp; }
		
		[ReadOnly(true), Description("The ID of the thread from which the data was handled"), Category("Data Info")]
		__property String *get_CallerThreadID() { return logItem->ThreadID; }

		[ReadOnly(true), Description("The IP address the data was going to"), Category("Data Info")]
		__property String *get_DestinationIPAddress() { return logItem->DestinationIPAddressString; }

		[ReadOnly(true), Description("The Port the data was going to"), CategoryAttribute("Data Info")]
		__property String *get_DestinationPort() { return logItem->DestinationPort.ToString(); }

		[ReadOnly(true), Description("The IP address the data was coming from"), Category("Data Info")]
		__property String *get_SourceIPAddress() { return logItem->SourceIPAddressString; }

		[ReadOnly(true), Description("The Port the data was coming from"), CategoryAttribute("Data Info")]
		__property String *get_SourcePort() { return logItem->SourcePort.ToString(); }

		[ReadOnly(true), Description("The Direction the network data was heading")]
		__property String *get_Direction() { return IsIncomingMessage ? INCOMING_MESSAGE_TEXT : OUTGOING_MESSAGE_TEXT; }

		[Browsable(false), ReadOnly(true), Description("True if the message was received by this computer, False if it was sent by this computer"), CategoryAttribute("Data Info")]
		__property bool get_IsIncomingMessage() { return logItem->IsIncomingMessage; }

		[ReadOnly(true), Description("The protocol used in the message"), CategoryAttribute("Data Info")]
		__property String *get_Protocol() { return logItem->Protocol.ToString(); }
	};
}