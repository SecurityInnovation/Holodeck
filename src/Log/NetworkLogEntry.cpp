//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkNetworkLogEntry.cpp
//
// DESCRIPTION: Contains implementation for the log entry class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Jan 2004		 B. Shirey	 File created.
//*************************************************************************
#define LogDll_File

#include "NetworkLogEntry.h"
#include "defines.h"

namespace Log
{
	HANDLE NetworkLogEntry::heap = NULL;

	//*************************************************************************
	// Method:		NetworkLogEntry
	// Description: Constructor for the NetworkLogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogEntry::NetworkLogEntry()
	{
		Initialize();
	}

	//*************************************************************************
	// Method:		~NetworkLogEntry
	// Description: Destructor for the NetworkLogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogEntry::~NetworkLogEntry()
	{
		Cleanup();
	}

	//*************************************************************************
	// Method:		Initialize
	// Description: Initializes memory for the NetworkLogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::Initialize()
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		logEntrySize = 0;
		packetDataLength = 0;
		originalPacketDataLength = 0;
		incoming = false;
		dataProtocol = 0;
		sourceIP = 0;
		destinationIP = 0;
		sourcePort = 0;
		destinationPort = 0;
		packetData = NULL;
		originalPacketData = NULL;
	}

	//*************************************************************************
	// Method:		Cleanup
	// Description: Cleans up memory for the NetworkLogEntry class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::Cleanup()
	{
		if (packetData != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, packetData);

		if (originalPacketData != NULL)
			HeapFree(heap, HEAP_ZERO_MEMORY, originalPacketData);
	}

	//*************************************************************************
	// Method:		RecomputeLogEntrySize
	// Description: Sets the logEntrySize member to the correct value
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::RecomputeLogEntrySize()
	{
		logEntrySize = 0;
		logEntrySize += (sizeof(unsigned long) * 2); // for string lengths
		logEntrySize += (sizeof(unsigned long)); // data entry size
		logEntrySize += (sizeof(unsigned long)); // original data entry size
		logEntrySize += (sizeof(bool));	// for incoming
		logEntrySize += (sizeof(int)); // for protocol
		logEntrySize += (sizeof(unsigned long) * 2); // for the ip addresses
		logEntrySize += (sizeof(unsigned short) * 2); // for the ports
		logEntrySize += (sizeof(unsigned char) * packetDataLength); // for the corrupted data member
		logEntrySize += (sizeof(unsigned char) * originalPacketDataLength); // for the original data member
	}

	//*************************************************************************
	// Method:		SetData
	// Description: Sets the data for the log entry
	//
	// Parameters:
	//	data - the string representing the data for the log entry
	//	dataLength - the length of the data param
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::SetData(unsigned char *data, unsigned int dataLength)
	{
		packetDataLength = dataLength;

		if (packetDataLength > 0)
		{
			if (packetData == NULL)
				packetData = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (packetDataLength));
			else
				packetData = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, packetData, sizeof(unsigned char) * (packetDataLength));

			CopyMemory(packetData, data, packetDataLength);
		}
	}

	//*************************************************************************
	// Method:		GetData
	// Description: Gets the data for the log entry
	//
	// Parameters:
	//	data - the buffer to put the error code into.  Must be allocated by caller.
	//	dataLength - on entering the function, must be the maximum number of chars
	//		the data buffer will hold. on leaving the function it will be the 
	//		number of characters written into data.
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::GetData(unsigned char *data, unsigned int &dataLength)
	{
		if (data == NULL)
		{
			dataLength = 0;
			return;
		}

		dataLength = packetDataLength;

		if (dataLength > 0)
			CopyMemory(data, packetData, packetDataLength);
	}

	//*************************************************************************
	// Method:		SetOriginalData
	// Description: Sets the original data for the log entry
	//
	// Parameters:
	//	data - the string representing the data for the log entry
	//	dataLength - the length of the data param
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::SetOriginalData(unsigned char *data, unsigned int dataLength)
	{
		originalPacketDataLength = dataLength;

		if (originalPacketDataLength > 0)
		{
			if (originalPacketData == NULL)
				originalPacketData = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, sizeof(unsigned char) * (originalPacketDataLength));
			else
				originalPacketData = (unsigned char *)HeapReAlloc(heap, HEAP_ZERO_MEMORY, originalPacketData, sizeof(unsigned char) * (originalPacketDataLength));

			CopyMemory(originalPacketData, data, originalPacketDataLength);
		}
	}
	
	//*************************************************************************
	// Method:		GetOriginalData
	// Description: Gets the original data for the log entry
	//
	// Parameters:
	//	data - the buffer to put the error code into.  Must be allocated by caller.
	//	dataLength - on entering the function, must be the maximum number of chars
	//		the data buffer will hold. on leaving the function it will be the 
	//		number of characters written into data.
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::GetOriginalData(unsigned char *data, unsigned int &dataLength)
	{
		if (data == NULL)
		{
			dataLength = 0;
			return;
		}

		dataLength = originalPacketDataLength;
		if (dataLength > 0)
			CopyMemory(data, originalPacketData, originalPacketDataLength);
	}

	//*************************************************************************
	// Method:		ConvertToByteSequence
	// Description: Serializes the data in the log entry for sending across
	//	processes through a pipe or other method
	//
	// Parameters:
	//	buffer - the buffer to hold the byte sequence
	//	bufferSize - on entering the function, must be the maximum number of chars
	//		the param buffer will hold.  On exiting will hold the number of chars
	//		written to
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::ConvertToByteSequence(unsigned char *buffer, unsigned int &bufferSize)
	{
		if (bufferSize < logEntrySize)
		{
			bufferSize = 0;
			return;
		}

		unsigned int offset = 0;
		unsigned int i = 0;

		// copy the log size in
		CopyMemory((buffer + offset), &logEntrySize, sizeof(logEntrySize));
		offset += sizeof(logEntrySize);

		// copy the data length in
		CopyMemory((buffer + offset), &packetDataLength, sizeof(packetDataLength));
		offset += sizeof(packetDataLength);

		// copy the original data length in
		CopyMemory((buffer + offset), &originalPacketDataLength, sizeof(originalPacketDataLength));
		offset += sizeof(originalPacketDataLength);

		// copy the incoming flag in
		CopyMemory((buffer + offset), &incoming, sizeof(incoming));
		offset += sizeof(incoming);

		// copy the protocol in
		CopyMemory((buffer + offset), &dataProtocol, sizeof(dataProtocol));
		offset += sizeof(dataProtocol);

		// copy the destination ip in
		CopyMemory((buffer + offset), &destinationIP, sizeof(destinationIP));
		offset += sizeof(destinationIP);

		// copy the destination port in
		CopyMemory((buffer + offset), &destinationPort, sizeof(destinationPort));
		offset += sizeof(destinationPort);

		// copy the source ip in
		CopyMemory((buffer + offset), &sourceIP, sizeof(sourceIP));
		offset += sizeof(sourceIP);

		// copy the source port in
		CopyMemory((buffer + offset), &sourcePort, sizeof(sourcePort));
		offset += sizeof(sourcePort);

		// copy the data in
		if (packetDataLength)
			CopyMemory((buffer + offset), packetData, packetDataLength);
		offset += packetDataLength;

		// copy the original data in
		if (originalPacketDataLength > 0)
			CopyMemory((buffer + offset), originalPacketData, originalPacketDataLength);
		offset += originalPacketDataLength;

		bufferSize = offset;
	}

	//*************************************************************************
	// Method:		ConvertFromByteSequence
	// Description: Extracts the data from a byte sequence
	//
	// Parameters:
	//	buffer - the buffer that holds the byte sequence to extract from
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogEntry::ConvertFromByteSequence(unsigned char *buffer)
	{
		if (buffer == NULL)
			return;

		unsigned int offset = 0;
		
		unsigned int logSize = *((unsigned int *)(buffer + offset));
		offset += sizeof(unsigned int);

		packetDataLength = *((unsigned int *)(buffer + offset));
		offset += sizeof(packetDataLength);

		originalPacketDataLength = *((unsigned int *)(buffer + offset));
		offset += sizeof(originalPacketDataLength);

		incoming = *((bool *)(buffer + offset));
		offset += sizeof(incoming);

		dataProtocol = *((int *)(buffer + offset));
		offset += sizeof(dataProtocol);

		destinationIP = *((unsigned long *)(buffer + offset));
		offset += sizeof(destinationIP);

		destinationPort = *((unsigned short *)(buffer + offset));
		offset += sizeof(destinationPort);

		sourceIP = *((unsigned long *)(buffer + offset));
		offset += sizeof(sourceIP);

		sourcePort = *((unsigned short *)(buffer + offset));
		offset += sizeof(sourcePort);

		SetData((unsigned char *)(buffer + offset), packetDataLength);
		offset += packetDataLength;

		SetOriginalData((unsigned char *)(buffer + offset), originalPacketDataLength);
	}
}