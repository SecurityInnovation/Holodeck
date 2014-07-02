#define LogDll_File

#include "logqueue.h"

namespace Log
{
	HANDLE LogQueue::heap = NULL;

	//*************************************************************************
	// Method:		LogQueue
	// Description: Constructor for the LogQueue class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogQueue::LogQueue() : headPosition(0), tailPosition(0), numQueueEntries(0)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		InitializeCriticalSection(&queueGuard);

		FillMemory(byteSequences, sizeof(byteSequences), 0);
		FillMemory(byteSequenceSizes, sizeof(byteSequenceSizes), 0);
	}

	//*************************************************************************
	// Method:		LogQueue
	// Description: Destructor for the LogQueue class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	LogQueue::~LogQueue()
	{
		for (int i = 0; i < MAX_LOG_QUEUE_SIZE; i++)
		{
			if (byteSequences[i] != NULL)
			{
				HeapFree(heap, HEAP_ZERO_MEMORY, byteSequences[i]);
				byteSequences[i] = NULL;
			}
		}

		DeleteCriticalSection(&queueGuard);
	}

	//*************************************************************************
	// Method:		GetLogCount
	// Description: Gets the number of logs in the queue
	//
	// Parameters:
	//	None
	//
	// Return Value: the number of logs in the queue
	//*************************************************************************
	int LogQueue::GetLogCount() 
	{ 
		EnterCriticalSection(&queueGuard);
		int retVal = numQueueEntries;
		LeaveCriticalSection(&queueGuard);
		return retVal;
	}
	
	//*************************************************************************
	// Method:		IsFull
	// Description: Gets whether or not the queue can hold more log entries
	//
	// Parameters:
	//	None
	//
	// Return Value: true if the queue cannot hold any more log entries, false
	//	otherwise
	//*************************************************************************
	bool LogQueue::IsFull() 
	{
		EnterCriticalSection(&queueGuard);
		bool retVal = (numQueueEntries == MAX_LOG_QUEUE_SIZE);
		LeaveCriticalSection(&queueGuard);
		return retVal;
	}
	
	//*************************************************************************
	// Method:		IsEmpty
	// Description: Gets whether or not the queue has any log entries
	//
	// Parameters:
	//	None
	//
	// Return Value: true if there are no log entries in the queue, false 
	//	otherwise
	//*************************************************************************
	bool LogQueue::IsEmpty() 
	{
		EnterCriticalSection(&queueGuard);
		bool retVal = (numQueueEntries == 0);
		LeaveCriticalSection(&queueGuard);
		return retVal;
	}

	//*************************************************************************
	// Method:		GetNextLogSize
	// Description: Return the size of the next log entry to get
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	int LogQueue::GetNextLogSize()
	{
		EnterCriticalSection(&queueGuard);
		int retVal = byteSequenceSizes[headPosition];
		LeaveCriticalSection(&queueGuard);
		return retVal;
	}

	//*************************************************************************
	// Method:		GetNextLogSize
	// Description: Return the combined size of all the log entries in the queue
	//
	// Parameters:
	//	None
	//
	// Return Value: the combined size of the entries
	//*************************************************************************
	int LogQueue::GetTotalLogSizes()
	{
		if (IsEmpty())
			return 0;

		EnterCriticalSection(&queueGuard);
		int retVal = 0;
		int count = 0;
		int i = headPosition;
		bool done = false;
		
		while (!done)
		{
			count++;
			if (count == numQueueEntries)
				done = true;

			retVal += byteSequenceSizes[i];

			i++;
			i %= MAX_LOG_QUEUE_SIZE;
		}
		LeaveCriticalSection(&queueGuard);

		return retVal;
	}

	//*************************************************************************
	// Method:		PushLog
	// Description: Adds a log entry data to tail of the queue
	//
	// Parameters:
	//	size - the number of bytes in logData
	//	logData - the data for the log entry
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LogQueue::PushLog(unsigned int size, unsigned char *logData)
	{
		EnterCriticalSection(&queueGuard);
		if ((!IsFull()) && (logData != NULL))
		{
			byteSequences[tailPosition] = (unsigned char *)HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
			if (byteSequences[tailPosition] != NULL)
			{
				CopyMemory(byteSequences[tailPosition], logData, size);
				byteSequenceSizes[tailPosition] = size;
				numQueueEntries++;

				tailPosition++;
				tailPosition %= MAX_LOG_QUEUE_SIZE;

				LeaveCriticalSection(&queueGuard);
				return true;
			}

			LeaveCriticalSection(&queueGuard);
			return false;
		}

		LeaveCriticalSection(&queueGuard);
		return false;
	}

	//*************************************************************************
	// Method:		PopLog
	// Description: Retrieves and removes the next log data from the head of the 
	//	queue
	//
	// Parameters:
	//	size - the max number of bytes logData will hold
	//	logData - the buffer to store the next log data in
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool LogQueue::PopLog(unsigned int size, unsigned char *logData)
	{
		EnterCriticalSection(&queueGuard);
		if ((!IsEmpty()) && (logData != NULL) && (size >= byteSequenceSizes[headPosition]))
		{
			CopyMemory(logData, byteSequences[headPosition], byteSequenceSizes[headPosition]);
			HeapFree(heap, HEAP_ZERO_MEMORY, byteSequences[headPosition]);
			byteSequences[headPosition] = NULL;
			byteSequenceSizes[headPosition] = 0;
			numQueueEntries--;

			headPosition++;
			headPosition %= MAX_LOG_QUEUE_SIZE;

			LeaveCriticalSection(&queueGuard);
			return true;
		}

		LeaveCriticalSection(&queueGuard);
		return false;
	}
}