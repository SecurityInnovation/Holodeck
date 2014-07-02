//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogItem.h
//
// DESCRIPTION: Contains definition for the network log item class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <memory.h>
#include <stdio.h>
#include "ManagedEnums.h"
#include "NetworkLogEntry.h"

#using <mscorlib.dll>

using namespace Holodeck;
using namespace System;
using namespace System::Collections;

namespace HoloScript
{

	//*************************************************************************
	// managed class to hold a network log item
	//*************************************************************************
	public __gc class NetworkLogItem
	{
	private:
		Log::NetworkLogEntry *entry;
		UInt64 uint64TimeStamp;
		String *timeStamp;
		String *threadID;
		String * ConvertToTextFormat();

	public:
		NetworkLogItem();
		~NetworkLogItem();

		__property UInt64 get_UInt64TimeStamp() { return uint64TimeStamp; }
		__property void set_UInt64TimeStamp(UInt64 value) { uint64TimeStamp = value; }

		__property String *get_TimeStamp() { return timeStamp; }
		__property void set_TimeStamp(String * value) { timeStamp = value; }

		__property String *get_ThreadID() { return threadID; }
		__property void set_ThreadID(String * value) { threadID = value; }

		__property void set_IsIncomingMessage(bool value) { entry->SetIsIncomingMessage(value); }
		__property bool get_IsIncomingMessage() { return entry->GetIsIncomingMessage(); }

		__property void set_Protocol(int value) { entry->SetProtocol(value); }
		__property int get_Protocol() { return entry->GetProtocol(); }

		__property String *get_ProtocolString();

		__property void set_DestinationIPAddress(unsigned long value) { entry->SetDestinationIPAddress(value); }
		__property unsigned long get_DestinationIPAddress() { return entry->GetDestinationIPAddress(); }

		__property String *get_DestinationIPAddressString();

		__property void set_DestinationPort(unsigned short value) { entry->SetDestinationPort(value); }
		__property unsigned short get_DestinationPort() { return entry->GetDestinationPort(); }

		__property void set_SourceIPAddress(unsigned long value) { entry->SetSourceIPAddress(value); }
		__property unsigned long get_SourceIPAddress() { return entry->GetSourceIPAddress(); }

		__property String *get_SourceIPAddressString();

		__property void set_SourcePort(unsigned short value) { entry->SetSourcePort(value); }
		__property unsigned short get_SourcePort() { return entry->GetSourcePort(); }

		__property unsigned int get_ActualDataLength() { return entry->GetDataLength(); }
		__property unsigned int get_ActualOriginalDataLength() { return entry->GetOriginalDataLength(); }

		__property String * get_ClipboardText() {return ConvertToTextFormat();}

		void SetData(unsigned char *data, unsigned int dataLength) { entry->SetData(data, dataLength); }
		void GetData(unsigned char *data, unsigned int &dataLength) { entry->GetData(data, dataLength); }
		void SetOriginalData(unsigned char *data, unsigned int dataLength) { entry->SetOriginalData(data, dataLength); }
		void GetOriginalData(unsigned char *data, unsigned int &dataLength) { entry->GetOriginalData(data, dataLength); }

		void ConvertFromByteSequence(unsigned char *buffer) { entry->ConvertFromByteSequence(buffer); }
	};
}