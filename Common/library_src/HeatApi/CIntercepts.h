//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CIntercepts.h
//
// DESCRIPTION: Contains implementation for the class CIntercepts, part of the
//	HEAT API
//
//*************************************************************************
#ifndef __CINTERCEPTS_H__
#define __CINTERCEPTS_H__

#include "CNode.h"

class CIntercepts
{
	CNode* m_pFirst;
	CNode* findNode(char* pszFuncName, char* pszFuncDLL)
	{
		CNode* pNode = m_pFirst;
		while (pNode)
		{
			if ((!strcmp(pszFuncName, pNode->getFuncName())) && (!strcmp(pszFuncDLL, pNode->getFuncDLL())))
				return pNode;
			pNode = pNode->getNext();
		}
		return NULL;
	}

public:
	CIntercepts()
	{
		m_pFirst = NULL;
	}

	void removeAllIntercepts()
	{
		while (m_pFirst)
		{
			CNode* pNext = m_pFirst->getNext();
			delete m_pFirst;
			m_pFirst = pNext;
		}
	}

	~CIntercepts()
	{
		removeAllIntercepts();
	}

	CNode* addIntercept(char *pszFuncName, char *pszFuncDLL, char *pszHandlerFunc, char *pszHandlerDLL)
	{
		if (findNode(pszFuncName, pszFuncDLL))
			return NULL;

		CNode* pNewNode = new CNode(pszFuncName, pszFuncDLL, pszHandlerFunc, pszHandlerDLL, NULL, m_pFirst);
		if (m_pFirst)
			m_pFirst->setPrev(pNewNode);
		m_pFirst = pNewNode;

		return pNewNode;
	}

	bool removeIntercept(char *pszFuncName, char *pszFuncDLL)
	{
		CNode* pNode = findNode(pszFuncName, pszFuncDLL);
		if (!pNode)
			return false;
		if (pNode->getPrev())
		{
			pNode->getPrev()->setNext(pNode->getNext());
			if (pNode->getNext())
				pNode->getNext()->setPrev(pNode->getPrev());
		}
		else
		{
			if (pNode->getNext())
				pNode->getNext()->setPrev(pNode->getPrev());
			m_pFirst = pNode->getNext();
		}

		delete pNode;

		return true;
	}

	CNode* getIntercept(char *pszFuncName, char *pszFuncDLL)
	{
		return findNode(pszFuncName, pszFuncDLL);
	}

	CNode* getInterceptList()
	{
		return m_pFirst;
	}

	void markAllUnsent()
	{
		CNode *pNode = m_pFirst;
		while (pNode)
		{
			if (pNode->getDeleted())
			{
				// If marked as deleted, go ahead and delete it
				CNode *pNextNode = pNode->getNext();
				if (pNode->getPrev())
				{
					pNode->getPrev()->setNext(pNode->getNext());
					if (pNode->getNext())
						pNode->getNext()->setPrev(pNode->getPrev());
				}
				else
				{
					if (pNode->getNext())
						pNode->getNext()->setPrev(pNode->getPrev());
					m_pFirst = pNode->getNext();
				}
				pNode = pNextNode;
				continue;
			}
			pNode->setSent(false);
			pNode = pNode->getNext();
		}
	}
};

#endif
