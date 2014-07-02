//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		DataQueue.cpp
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

#include "DataQueue.h"


// DataQueue constructor
//
// Parameters:
//   None
//
// Return value:
//   None
DataQueue::DataQueue()
{
	InitializeCriticalSection(&m_csQueueCriticalSection);
	messageMode = false;
}


// DataQueue destructor
//
// Parameters:
//   None
//
// Return value:
//   None
DataQueue::~DataQueue()
{
	DeleteCriticalSection(&m_csQueueCriticalSection);

	list<DataQueueBuffer>::iterator i;
	for (i=m_lQueue.begin(); i!=m_lQueue.end(); i++)
	{
		if (i->pBuffer)
			delete[] i->pBuffer;
	}
	m_lQueue.clear();
}


// AppendData will add data to the end the data queue
//
// Parameters:
//   buf - Data buffer containing data to add to the queue
//   len - Length of data buffer
//
// Return value:
//   None
void DataQueue::AppendData(const BYTE* buf, int len)
{
	DataQueueBuffer dqb;

	// Add the data to the end of the queue
	dqb.pBuffer = new BYTE[len];
	memcpy(dqb.pBuffer, buf, len);
	dqb.dwLength = (DWORD)len;
	dqb.dwCurOffset = 0;

	EnterCriticalSection(&m_csQueueCriticalSection);
	m_lQueue.push_back(dqb);
	LeaveCriticalSection(&m_csQueueCriticalSection);
}


// AppendData will add data to the end the data queue
//
// Parameters:
//   buf - Data buffer containing data to add to the queue
//   len - Length of data buffer
//
// Return value:
//   None
void DataQueue::AppendDataWithAddr(const BYTE* buf, int len, const struct sockaddr* addr, const int* addrlen)
{
	DataQueueBuffer dqb;

	// Add the data to the end of the queue
	dqb.pBuffer = new BYTE[len];
	memcpy(dqb.pBuffer, buf, len);
	dqb.dwLength = (DWORD)len;
	dqb.dwCurOffset = 0;
	memcpy(&dqb.addr, addr, sizeof(sockaddr));
	dqb.addrLen = *addrlen;

	EnterCriticalSection(&m_csQueueCriticalSection);
	m_lQueue.push_back(dqb);
	LeaveCriticalSection(&m_csQueueCriticalSection);
}


// RetrieveData will dequeue at most len bytes from the data queue.  Returns
// the number of bytes actually dequeued.
//
// Parameters:
//   buf - Data buffer that will receieve the data from the queue
//   len - Length of data buffer
//
// Return value:
//   Number of bytes retrieved from the data queue
int DataQueue::RetrieveData(BYTE* buf, int len)
{
	list<DataQueueBuffer>::iterator i;
	int totalBytes = 0;	// number of bytes dequeued

	EnterCriticalSection(&m_csQueueCriticalSection);

	i = m_lQueue.begin();

	if (messageMode)
	{
		// In message mode, only return data from the first message, and consume the entire
		// message even if some of it wasn't read.
		if (i != m_lQueue.end())
		{
			if ((i->dwLength - i->dwCurOffset) > (DWORD)len)
			{
				// There is more data left in this DataQueueBuffer than
				// is left in the retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, len);
				totalBytes += len;
			}
			else
			{
				// There is not enough data left in the queue to fill the
				// retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, i->dwLength - i->dwCurOffset);
				totalBytes += i->dwLength - i->dwCurOffset;
			}
			// We are done with this DataQueueBuffer
			delete[] i->pBuffer;
			m_lQueue.erase(i);
		}
	}
	else
	{
		while ((i!=m_lQueue.end()) && (len>0))
		{
			if ((i->dwLength - i->dwCurOffset) > (DWORD)len)
			{
				// There is more data left in this DataQueueBuffer than
				// is left in the retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, len);
				i->dwCurOffset += len;
				totalBytes += len;
				break;
			}
			else
			{
				// There is not enough data left in the queue to fill the
				// retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, i->dwLength - i->dwCurOffset);
				totalBytes += i->dwLength - i->dwCurOffset;
				len -= i->dwLength - i->dwCurOffset;
				// We are done with this DataQueueBuffer
				delete[] i->pBuffer;
				m_lQueue.erase(i);
				// Get the next DataQueueBuffer entry
				i = m_lQueue.begin();
			}
		}
	}

	LeaveCriticalSection(&m_csQueueCriticalSection);
	return totalBytes;
}


// RetrieveDataFrom will dequeue at most len bytes from the data queue and return
// the address it was received from.  Assumes message mode.  Returns the number
// of bytes actually dequeued.
//
// Parameters:
//   buf - Data buffer that will receieve the data from the queue
//   len - Length of data buffer
//   from - Receives from adddress
//   fromlen - Receives from length
//
// Return value:
//   Number of bytes retrieved from the data queue
int DataQueue::RetrieveDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen)
{
	list<DataQueueBuffer>::iterator i;
	int totalBytes = 0;	// number of bytes dequeued

	EnterCriticalSection(&m_csQueueCriticalSection);

	// In message mode, only return data from the first message, and consume the entire
	// message even if some of it wasn't read.
	i = m_lQueue.begin();

	if (i != m_lQueue.end())
	{
		if ((i->dwLength - i->dwCurOffset) > (DWORD)len)
		{
			// There is more data left in this DataQueueBuffer than
			// is left in the retrieval buffer
			memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, len);
			totalBytes += len;
		}
		else
		{
			// There is not enough data left in the queue to fill the
			// retrieval buffer
			memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, i->dwLength - i->dwCurOffset);
			totalBytes += i->dwLength - i->dwCurOffset;
		}

		memcpy(from, &i->addr, sizeof(sockaddr));
		*fromlen = i->addrLen;

		// We are done with this DataQueueBuffer
		delete[] i->pBuffer;
		m_lQueue.erase(i);
	}

	LeaveCriticalSection(&m_csQueueCriticalSection);
	return totalBytes;
}


// PeekData will return at most len bytes from the data queue, without removing
// the data from the queue.  Returns the number of bytes actually dequeued.
//
// Parameters:
//   buf - Data buffer that will receive the data at the front of the queue.
//   len - Length of the data buffer.
//
// Return value:
//   Number of bytes placed in the data buffer
int DataQueue::PeekData(BYTE* buf, int len)
{
	list<DataQueueBuffer>::iterator i;
	int totalBytes = 0;	// number of bytes dequeued

	EnterCriticalSection(&m_csQueueCriticalSection);

	i = m_lQueue.begin();

	if (messageMode)
	{
		// In message mode, only return data from the first message
		if (i != m_lQueue.end())
		{
			if ((i->dwLength - i->dwCurOffset) > (DWORD)len)
			{
				// There is more data left in this DataQueueBuffer than
				// is left in the retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, len);
				totalBytes += len;
			}
			else
			{
				// There is not enough data left in the queue to fill the
				// retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, i->dwLength - i->dwCurOffset);
				totalBytes += i->dwLength - i->dwCurOffset;
			}
		}
	}
	else
	{
		while ((i!=m_lQueue.end()) && (len>0))
		{
			if ((i->dwLength - i->dwCurOffset) > (DWORD)len)
			{
				// There is more data left in this DataQueueBuffer than
				// is left in the retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, len);
				totalBytes += len;
				break;
			}
			else
			{
				// There is not enough data left in the queue to fill the
				// retrieval buffer
				memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, i->dwLength - i->dwCurOffset);
				totalBytes += i->dwLength - i->dwCurOffset;
				len -= i->dwLength - i->dwCurOffset;
				// Get the next DataQueueBuffer entry
				i++;
			}
		}
	}

	LeaveCriticalSection(&m_csQueueCriticalSection);
	return totalBytes;
}


// PeekDataFrom will return at most len bytes from the data queue, without removing
// the data from the queue.  Returns the address the data was received from and
// assumes message mode.  Returns the number of bytes actually dequeued.
//
// Parameters:
//   buf - Data buffer that will receive the data at the front of the queue.
//   len - Length of the data buffer.
//   from - Receives from adddress
//   fromlen - Receives from length
//
// Return value:
//   Number of bytes placed in the data buffer
int DataQueue::PeekDataFrom(BYTE* buf, int len, struct sockaddr* from, int* fromlen)
{
	list<DataQueueBuffer>::iterator i;
	int totalBytes = 0;	// number of bytes dequeued

	EnterCriticalSection(&m_csQueueCriticalSection);

	i = m_lQueue.begin();

	// In message mode, only return data from the first message
	if (i != m_lQueue.end())
	{
		if ((i->dwLength - i->dwCurOffset) > (DWORD)len)
		{
			// There is more data left in this DataQueueBuffer than
			// is left in the retrieval buffer
			memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, len);
			totalBytes += len;
		}
		else
		{
			// There is not enough data left in the queue to fill the
			// retrieval buffer
			memcpy(buf + totalBytes, i->pBuffer + i->dwCurOffset, i->dwLength - i->dwCurOffset);
			totalBytes += i->dwLength - i->dwCurOffset;
		}

		memcpy(from, &i->addr, sizeof(sockaddr));
		*fromlen = i->addrLen;
	}

	LeaveCriticalSection(&m_csQueueCriticalSection);
	return totalBytes;
}


// GetRemainingDataLength returns the number of bytes currently in the data queue.
//
// Parameters:
//   None
//
// Return value:
//   Number of bytes left in the queue
int DataQueue::GetRemainingDataLength()
{
	list<DataQueueBuffer>::const_iterator i;
	int len = 0;

	EnterCriticalSection(&m_csQueueCriticalSection);

	if (messageMode)
	{
		// Only count the first message in message mode
		i = m_lQueue.begin();
		if (i != m_lQueue.end())
			len += i->dwLength - i->dwCurOffset;
	}
	else
	{
		for (i=m_lQueue.begin(); i!=m_lQueue.end(); i++)
			len += i->dwLength - i->dwCurOffset;
	}

	LeaveCriticalSection(&m_csQueueCriticalSection);
	return len;
}


//*************************************************************************
// SetMessageMode will enable or disable message mode.  When message
// mode is enabled, GetRemainingDataLength, RetrieveData, and PeekData
// will only retrieve one message at a time, even if more data is
// available in the queue.
//
// Parameters:
//   buf - Data buffer used for receving data
//   len - Length of data buffer
//
// Return value:
//   Number of bytes added to the receive queue, or -1 on error.
//*************************************************************************
void DataQueue::SetMessageMode(bool messageModeEnable)
{
	messageMode = messageModeEnable;
}
