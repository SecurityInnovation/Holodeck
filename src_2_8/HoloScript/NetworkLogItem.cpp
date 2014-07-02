//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogItem.cpp
//
// DESCRIPTION: Contains implementation for the network log item class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#include <winsock2.h>
#include <rpc.h>
#include "NetworkLogItem.h"

namespace HoloScript
{
	//*************************************************************************
	// Method:		NetworkLogItem
	// Description: Constructor for the NetworkLogItem class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogItem::NetworkLogItem()
	{
		entry = new Log::NetworkLogEntry();
	}

	//*************************************************************************
	// Method:		~NetworkLogItem
	// Description: Destructor for the NetworkLogItem class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogItem::~NetworkLogItem()
	{
		if (entry)
		{
			delete entry;
			entry = NULL;
		}
	}

	//*************************************************************************
	// Method:		get_ProtocolString
	// Description: Takes the protocol and converts it to a "friendly" string
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representation of the protocol
	//*************************************************************************
	String *NetworkLogItem::get_ProtocolString()
	{
		WSADATA WSAData;
		int numProtocols;
		WSAPROTOCOL_INFO *protocolBuf = NULL;
		DWORD bufferLen = 0;

		int protocol = entry->GetProtocol();

		if (WSAStartup(MAKEWORD(2,2), &WSAData))
		{
			// error starting winsock.  nothing we can do
			return protocol.ToString();
		}

		LPINT protocols = new INT[2];
		protocols[0] = protocol;
		protocols[1] = 0;

		// we expect this to fail, we just need the bufferLength to be filled in
		numProtocols = WSAEnumProtocols(protocols, protocolBuf, &bufferLen);
		if ((numProtocols != SOCKET_ERROR) || (WSAGetLastError() != WSAENOBUFS))
		{
			WSACleanup();
			return protocol.ToString();
		}
		
		protocolBuf = (WSAPROTOCOL_INFO *)malloc(bufferLen);
		if (!protocolBuf)
		{
			WSACleanup();
			return protocol.ToString();
		}

        numProtocols = WSAEnumProtocols(protocols, protocolBuf, &bufferLen);
		String *retVal;
		if ((numProtocols == SOCKET_ERROR) || (numProtocols == 0))
			retVal = protocol.ToString();
		else
			retVal = new String(protocolBuf[0].szProtocol);

		free(protocolBuf);
		WSACleanup();

		return retVal;
	}

	//*************************************************************************
	// Method:		get_DestinationIPAddressString
	// Description: Takes the destinationIPAddress and converts it to a string
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representation of the destination IP address
	//*************************************************************************
	String *NetworkLogItem::get_DestinationIPAddressString()
	{
		char str[16];
		unsigned long addr = this->DestinationIPAddress;
		sprintf(str, "%d.%d.%d.%d", addr & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
		return new String(str);
	}

	//*************************************************************************
	// Method:		get_SourceIPAddressString
	// Description: Takes the sourceIPAddress and converts it to a string
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representation of the source IP address
	//*************************************************************************
	String *NetworkLogItem::get_SourceIPAddressString()
	{
		char str[16];
		unsigned long addr = this->SourceIPAddress;
		sprintf(str, "%d.%d.%d.%d", addr & 0xFF, (addr >> 8) & 0xFF, (addr >> 16) & 0xFF, (addr >> 24) & 0xFF);
		return new String(str);
	}

	//*************************************************************************
	// Method:		ConvertToTextFormat
	// Description: Converts the networklogitem to a string, used for pasting item
	//				into the clipboard
	//
	// Parameters:
	//	None
	//
	// Return Value: a string representation of the the networklogitem
	//*************************************************************************
	String * NetworkLogItem::ConvertToTextFormat()
	{
		String * text = S"";
		text = String::Concat (text, S"Type: ", this->IsIncomingMessage ? S"Incoming":S"Outgoing", S"\r\n");
		text = String::Concat (text, S"TimeStamp: ", this->TimeStamp, S"\r\n");
		text = String::Concat (text, S"ThreadID: ", this->ThreadID, S"\r\n");
		text = String::Concat (text, S"Protocol: ", this->ProtocolString, S"\r\n");
		text = String::Concat (text, S"Destination IP Address: ", this->DestinationIPAddressString, S"\r\n");
		text = String::Concat (text, S"Destination Port: ", this->DestinationPort.ToString(), S"\r\n");
		text = String::Concat (text, S"Source IP Address: ", this->SourceIPAddressString, S"\r\n");
		text = String::Concat (text, S"Source Port: ", this->SourcePort.ToString());
		
		return text;
	}
}