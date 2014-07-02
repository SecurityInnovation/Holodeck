//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Event.cpp
//
// DESCRIPTION: Contains implementation for the utility class Event
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 JAN 2003		 B. Shirey	 File created.
//*************************************************************************
#include "event.h"

namespace SiUtils
{
	//*************************************************************************
	// Method:		Event
	// Description: Constructor for the Event class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Event::Event() : handle(INVALID_HANDLE_VALUE)
	{
	}

	//*************************************************************************
	// Method:		~Event
	// Description: Destructor for the Event class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	Event::~Event()
	{
		Close();
	}

	//*************************************************************************
	// Method:		Create
	// Description: Creates a named event object
	//
	// Parameters:
	//	name - The name of the event, or NULL if creating an unnamed event
	//	initialState - true if the event object should initially be signaled,
	//		false otherwise
	//	manualReset - true if the event will be reset manually, false if it
	//		will be release automatically
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::Create(char *name, bool initialState, bool manualReset)
	{
		PSECURITY_DESCRIPTOR pSD;
		SECURITY_ATTRIBUTES sa;

		pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
		InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
		SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
		// Initialize a security attributes structure.
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.lpSecurityDescriptor = pSD;
		sa.bInheritHandle = FALSE;
		
		handle = CreateEvent(&sa, manualReset, initialState, name);
		return (handle != INVALID_HANDLE_VALUE);
	}

	//*************************************************************************
	// Method:		Create
	// Description: Creates a named event object
	//
	// Parameters:
	//	name - The name of the event
	//	initialState - true if the event object should initially be signaled,
	//		false otherwise
	//	manualReset - true if the event will be reset manually, false if it
	//		will be release automatically
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::Create(const SiString &name, bool initialState, bool manualReset)
	{
		return this->Create((char *)name, initialState, manualReset);
	}

	//*************************************************************************
	// Method:		Open
	// Description: Opens an event object if it exists or creates a new one if
	//	it does not already exist
	//
	// Parameters:
	//	name - The name of the event, or NULL if opening an unnamed event
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::Open(char *name)
	{
		return this->Create(name);
	}

	//*************************************************************************
	// Method:		Open
	// Description: Opens a named event object if it exists or creates a new
	//	one if it does not already exist
	//
	// Parameters:
	//	name - The name of the event
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::Open(const SiString &name)
	{
		return this->Open((char *)name);
	}

	//*************************************************************************
	// Method:		OpenExisting
	// Description: Opens an existing event object, and fails if it does not
	//	exist
	//
	// Parameters:
	//	name - The name of the event, or NULL if opening an unnamed event
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::OpenExisting(char *name)
	{
		handle = OpenEvent(EVENT_ALL_ACCESS, FALSE, name);
		return (handle != INVALID_HANDLE_VALUE);
	}

	//*************************************************************************
	// Method:		OpenExisting
	// Description: Opens an existing named event object, and fails if it does
	//	not exist
	//
	// Parameters:
	//	name - The name of the event
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::OpenExisting(const SiString &name)
	{
		return this->OpenExisting((char *)name);
	}

	//*************************************************************************
	// Method:		Close
	// Description: Closes the event object
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool Event::Close()
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
	// Description: Waits for the event object to become signaled
	//
	// Parameters:
	//	waitCount - The number of milliseconds to wait for the object to become 
	//		signaled
	//
	// Return Value: EVENT_SIGNALED if the object is signaled, or
	//	EVENT_TIMED_OUT if the object was not signaled before waitCount
	//	milliseconds expired
	//*************************************************************************
	unsigned int Event::Acquire(long waitCount)
	{
		return WaitForSingleObject(handle, waitCount);
	}

	//*************************************************************************
	// Method:		Release
	// Description: Resets the state of the event object
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void Event::Release()
	{
		ResetEvent(handle);
	}

	//*************************************************************************
	// Method:		Signal
	// Description: Sets the state of the event object to signaled
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void Event::Signal()
	{
		SetEvent(handle);
	}

	//*************************************************************************
	// Method:		AcquireMultiple
	// Description: Waits for multiple event objects to become signaled
	//
	// Parameters:
	//  eventArray - Array of pointers to Event objects representing the events
	//				 to wait for
	//  eventCount - Number of events in eventArray
	//  waitAll - If true, wait for all events to be signaled, otherwise wait
	//			  for any one of the events to be signaled
	//	waitCount - The number of milliseconds to wait for the objects to become 
	//		signaled
	//
	// Return Value: (WAIT_OBJECT_0 + n) if an object is signaled, where n is
	//  the index into the event array, WAIT_TIMEOUT if the objects were not
	//	signaled before waitCount milliseconds expired, or WAIT_FAILED on
	//  failure
	//*************************************************************************
	unsigned int Event::AcquireMultiple(Event** eventArray, int eventCount, bool waitAll, long waitCount)
	{
		if (eventCount > MAXIMUM_WAIT_OBJECTS)
			return WAIT_FAILED;

		HANDLE hEventArray[MAXIMUM_WAIT_OBJECTS];
		for (int i=0; i<eventCount; i++)
			hEventArray[i] = eventArray[i]->handle;

		return WaitForMultipleObjects(eventCount, hEventArray, waitAll ? TRUE : FALSE, waitCount);
	}
}
