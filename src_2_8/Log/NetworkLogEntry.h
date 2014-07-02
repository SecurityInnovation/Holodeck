//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogEntry.h
//
// DESCRIPTION: Contains definition for the network log entry class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <stdio.h>

#ifdef LogDll_File 
#define HolodeckLog_Api __declspec(dllexport)
#else
#define HolodeckLog_Api __declspec(dllimport)
#endif

namespace Log
{
	//*************************************************************************
	// class to hold a network log entry
	//*************************************************************************
	class HolodeckLog_Api NetworkLogEntry
	{
	private:
		// static members
		static HANDLE heap;

		// non string members
		unsigned int logEntrySize;
		bool incoming;
		int dataProtocol;
		unsigned long sourceIP;
		unsigned long destinationIP;
		unsigned short sourcePort;
		unsigned short destinationPort;

		// string lengths
		unsigned int packetDataLength;
		unsigned int originalPacketDataLength;

		// strings
		unsigned char *packetData;
		unsigned char *originalPacketData;

		void Cleanup();
		void RecomputeLogEntrySize();
	public:
		NetworkLogEntry();
		~NetworkLogEntry();

		void Initialize();

		void ConvertToByteSequence(unsigned char *buffer, unsigned int &bufferSize);
		void ConvertFromByteSequence(unsigned char *buffer);

		unsigned int GetLogSize() 
		{ 
			if (logEntrySize == 0)
				RecomputeLogEntrySize();
			return logEntrySize; 
		}
		
		unsigned int GetDataLength() { return this->packetDataLength; }
		unsigned int GetOriginalDataLength() { return this->originalPacketDataLength; }

		void SetIsIncomingMessage(bool inMsg) { incoming = inMsg; }
		bool GetIsIncomingMessage() { return incoming; }

		void SetProtocol(int protocol) { dataProtocol = protocol; }
		int GetProtocol() { return dataProtocol; }

		void SetDestinationIPAddress(unsigned long address) { destinationIP = address; }
		unsigned long GetDestinationIPAddress() { return destinationIP; }

		void SetDestinationPort(unsigned short port) { destinationPort = port; }
		unsigned short GetDestinationPort() { return destinationPort; }

		void SetSourceIPAddress(unsigned long address) { sourceIP = address; }
		unsigned long GetSourceIPAddress() { return sourceIP; }

		void SetSourcePort(unsigned short port) { sourcePort = port; }
		unsigned short GetSourcePort() { return sourcePort; }

		void SetData(unsigned char *data, unsigned int dataLength);
		void GetData(unsigned char *data, unsigned int &dataLength);

		void SetOriginalData(unsigned char *data, unsigned int dataLength);
		void GetOriginalData(unsigned char *data, unsigned int &dataLength);

	};
}