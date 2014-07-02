//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		DataQueue.h
//
// DESCRIPTION: DataQueue class contains the data ready to be
//				sent/received over a network connection.  The data in
//				this structure has already passed through the regular
//				expression filters and is ready to be either received
//				by the application or sent out over the real network.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Feb 2003		 R. Wagner	 File created.
//*************************************************************************

#ifndef __DATAQUEUE_H__
#define __DATAQUEUE_H__

#include "NetCorruptLibrary.h"


// DataQueueBuffer contians a single storage area for the data used in the DataQueue
// class below.  The DataQueue is represented by a linked list of these structures.
struct DataQueueBuffer
{
	BYTE* pBuffer;
	DWORD dwLength;			// Length of data in pBuffer
	DWORD dwCurOffset;		// Number of bytes already "removed" from this buffer, used
							// when dequeuing only part of a DataQueueBuffer
	struct sockaddr addr;	// Address received from or sending to for datagram protocols
	int addrLen;			// Length of address received from or sending to for datagram protocols
};


// DataQueue class contains the data ready to be sent/received over a network connection.
// The data in this structure has already passed through the regular expression filters
// and is ready to be either received by the application or sent out over the real
// network.
class DataQueue
{
	list<DataQueueBuffer> m_lQueue;
	CRITICAL_SECTION m_csQueueCriticalSection;
	bool messageMode;

public:
	DataQueue();
	~DataQueue();

	void AppendData(const BYTE* buf, int len);
	void AppendDataWithAddr(const BYTE* buf, int len, const struct sockaddr* addr, const int* addrlen);
	int RetrieveData(BYTE* buf, int len);
	int RetrieveDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen);
	int PeekData(BYTE* buf, int len);
	int PeekDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen);
	int GetRemainingDataLength();

	void SetMessageMode(bool messageModeEnable);
};


#endif
