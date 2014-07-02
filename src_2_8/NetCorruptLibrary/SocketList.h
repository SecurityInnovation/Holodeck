//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2003 – All Rights Reserved.
//
// FILE:		SocketList.h
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

#ifndef __SOCKETLIST_H__
#define __SOCKETLIST_H__

#include "NetCorruptLibrary.h"


class SocketInfo;

// SocketList class contains the list of open sockets and a SocketInfo instance for each of these sockets.
// Be sure to call the Lock() function before manipulating or accessing the list.  To make sure a SocketInfo
// structure that needs to be maintained across a blocking call does not get deleted, call the AddRef/Release
// members of SocketInfo.
class SocketList
{
	map<SOCKET, SocketInfo*> m_mSockets;
	CRITICAL_SECTION m_csListCriticalSection;

public:
	SocketList();
	~SocketList();

	SocketInfo* AddSocket(SOCKET s);
	SocketInfo* GetSocketInfo(SOCKET s);
	void CloseSocket(SOCKET s);
	void Lock();
	void Unlock();
};


extern SocketList g_socketList;


#endif
