//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CNode.h
//
// DESCRIPTION: Contains implementation for the class CNode, part of the
//	HEAT API
//
//*************************************************************************
#ifndef __CNODE_H__
#define __CNODE_H__

#include <windows.h>


class CNode
{
	char*  m_pszFuncName;
	char*  m_pszFuncDLL;
	char*  m_pszHandlerFunc;
	char*  m_pszHandlerDLL;
	CNode* m_pPrev;
	CNode* m_pNext;
	bool   m_bIsSent;
	bool   m_bIsDeleted;

public:
	CNode(char *pszFuncName, char *pszFuncDLL, char *pszHandlerFunc, char *pszHandlerDLL, CNode* pPrev, CNode* pNext):
		m_pPrev(pPrev), m_pNext(pNext)
	{
		if (pszFuncName)
		{
			m_pszFuncName = new char[strlen(pszFuncName)+1];
			strcpy(m_pszFuncName, pszFuncName);
		}
		else
			pszFuncName = NULL;

		if (pszFuncDLL)
		{
			m_pszFuncDLL = new char[strlen(pszFuncDLL)+1];
			strcpy(m_pszFuncDLL, pszFuncDLL);
		}
		else
			pszFuncDLL = NULL;

		if (pszHandlerFunc)
		{
			m_pszHandlerFunc = new char[strlen(pszHandlerFunc)+1];
			strcpy(m_pszHandlerFunc, pszHandlerFunc);
		}
		else
			m_pszHandlerFunc = NULL;

		if (pszHandlerDLL)
		{
			m_pszHandlerDLL = new char[strlen(pszHandlerDLL)+1];
			strcpy(m_pszHandlerDLL, pszHandlerDLL);
		}
		else
			pszHandlerDLL = NULL;

		m_bIsSent = false;
		m_bIsDeleted = false;
	}

	~CNode()
	{
		if (m_pszFuncName)
			delete[] m_pszFuncName;
		if (m_pszFuncDLL)
			delete[] m_pszFuncDLL;
		if (m_pszHandlerFunc)
			delete[] m_pszHandlerFunc;
		if (m_pszHandlerDLL)
			delete[] m_pszHandlerDLL;
	}

	void setNext(CNode* pNode)	{ m_pNext = pNode; }
	CNode* getNext()			{ return m_pNext; }

	void setPrev(CNode* pNode)	{ m_pPrev = pNode; }
	CNode* getPrev()			{ return m_pPrev; }

	char* getFuncName()			{ return m_pszFuncName; }
	char* getFuncDLL()			{ return m_pszFuncDLL; }
	char* getHandlerFunc()		{ return m_pszHandlerFunc; }
	char* getHandlerDLL()		{ return m_pszHandlerDLL; }

	void setSent(bool bSent)	{ m_bIsSent = bSent; }
	bool getSent()				{ return m_bIsSent; }

	void setDeleted(bool bDeleted)	{ m_bIsDeleted = bDeleted; }
	bool getDeleted()				{ return m_bIsDeleted; }
};

#endif
