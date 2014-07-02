//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Mutex.cpp
//
// DESCRIPTION: Contains implementation for the utility class Mutex
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 JAN 2003		 B. Shirey	 File created.
//*************************************************************************
#include "mutex.h"

namespace SiUtils
{
	//*************************************************************************
	// Method:		Mutex
	// Description: Constructor for the Mutex class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Mutex::Mutex()
	{
	}

	//*************************************************************************
	// Method:		~Mutex
	// Description: Destructor for the Mutex class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Mutex::~Mutex()
	{
	}

	//*************************************************************************
	// Method:		Create
	// Description: Creates a named mutex object
	//
	// Parameters:
	//	name - The name of the mutex, or NULL if creating an unnamed mutex
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Mutex::Create(char *name)
	{
		handle = CreateMutex(NULL, FALSE, name);
		return (handle != INVALID_HANDLE_VALUE);
	}

	//*************************************************************************
	// Method:		Create
	// Description: Creates a named mutex object
	//
	// Parameters:
	//	name - The name of the mutex
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Mutex::Create(const SiString &name)
	{
		return this->Create((char *)name);
	}

	//*************************************************************************
	// Method:		Open
	// Description: Opens a mutex object if it exists or creates a new one if
	//	it does not already exist
	//
	// Parameters:
	//	name - The name of the mutex, or NULL if opening an unnamed mutex
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Mutex::Open(char *name)
	{
		return this->Create(name);
	}

	//*************************************************************************
	// Method:		Open
	// Description: Opens a named mutex object if it exists or creates a new
	//	one if it does not already exist
	//
	// Parameters:
	//	name - The name of the mutex
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Mutex::Open(const SiString &name)
	{
		return this->Open((char *)name);
	}

	//*************************************************************************
	// Method:		Close
	// Description: Closes the mutex object
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Mutex::Close()
	{
		if (handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
			handle = INVALID_HANDLE_VALUE;
		}

		return true;
	}

	//*************************************************************************
	// Method:		Acquire
	// Description: Waits for the mutex object to become signaled
	//
	// Parameters:
	//	waitCount - The number of milliseconds to wait for the object to become 
	//		signaled
	//
	// Return Value: MUTEX_SIGNALED if the object is signaled, or
	//	MUTEX_TIMED_OUT if the object was not signaled before waitCount
	//	milliseconds expired, or MUTEX_ABANDONED if the thread holding the
	//	mutex object terminates before releasing the mutex
	//*************************************************************************
	unsigned int Mutex::Acquire(long waitCount)
	{
		return WaitForSingleObject(handle, waitCount);
	}

	//*************************************************************************
	// Method:		Release
	// Description: Resets the state of the mutex object
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void Mutex::Release()
	{
		ReleaseMutex(handle);
	}
}
