//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		SocketList.cpp
//
// DESCRIPTION: SocketList class contains the list of open sockets and a
// SocketInfo instance for each of these sockets.  Be sure to call the
// Lock() function before manipulating or accessing the list.  To make
// sure a SocketInfo structure that needs to be maintained across a
// blocking call does not get deleted, call the AddRef/Release members
// of SocketInfo.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 07 Feb 2003		 R. Wagner	 File created.
//*************************************************************************

#include "SocketList.h"
#include "SocketInfo.h"


// SocketList constructor
//
// Parameters:
//   None
//
// Return value:
//   None
SocketList::SocketList()
{
	InitializeCriticalSection(&m_csListCriticalSection);
}


// SocketList destructor
//
// Parameters:
//   None
//
// Return value:
//   None
SocketList::~SocketList()
{
	DeleteCriticalSection(&m_csListCriticalSection);
}


// AddSocket adds a newly opened socket to the socket list
//
// Parameters:
//   s - New socket handle
//
// Return value:
//   Pointer to the SocketInfo object for this socket handle
SocketInfo* SocketList::AddSocket(SOCKET s)
{
	SocketInfo* info = new SocketInfo(s);

	m_mSockets[s] = info;
	return info;
}


// GetSocketInfo retrieves the SocketInfo instance describing a given socket handle.  Returns NULL
// if the socket does not exist.
//
// Parameters:
//   s - Socket handle
//
// Return value:
//   Pointer to the SocketInfo object for this socket handle
SocketInfo* SocketList::GetSocketInfo(SOCKET s)
{
	if (m_mSockets.find(s) == m_mSockets.end())
		return NULL;

	SocketInfo* info = m_mSockets[s];
	return info;
}


// CloseSocket removes a socket from the socket list and frees the memory used by the associated SocketInfo instance
//
// Parameters:
//   s - Socket handle
//
// Return value:
//   None
void SocketList::CloseSocket(SOCKET s)
{
	map<SOCKET, SocketInfo*>::iterator socket = m_mSockets.find(s);
		
	if (socket != m_mSockets.end())
	{
		SocketInfo* info = m_mSockets[s];
		if (info)
			info->Release();

		m_mSockets.erase(socket);
	}
}


// Gives the current thread exclusive access to the socket list
//
// Parameters:
//   None
//
// Return value:
//   None
void SocketList::Lock()
{
	EnterCriticalSection(&m_csListCriticalSection);
}


// Yields exclusive access to the socket list
//
// Parameters:
//   None
//
// Return value:
//   None
void SocketList::Unlock()
{
	LeaveCriticalSection(&m_csListCriticalSection);
}
