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

#ifdef  HEATAPI_EXPORTS
#define HEATAPI_API __declspec(dllexport)
#else
#define HEATAPI_API __declspec(dllimport)
#endif

#include <windows.h>

#pragma warning(disable: 4996)

class HEATAPI_API CNode
{
	char*       m_pszFuncName;
	char*       m_pszFuncDLL;
	wchar_t*    m_pwszFuncDLL;

	char*       m_pszHandlerFunc;
	char*       m_pszHandlerDLL;
	wchar_t*    m_pwszHandlerDLL;

	CNode*      m_pPrev;
	CNode*      m_pNext;

	bool        m_bIsSent;
	bool        m_bIsDeleted;

public:
	CNode(char *pszFuncName, char *pszFuncDLL, char *pszHandlerFunc, char *pszHandlerDLL, CNode* pPrev, CNode* pNext);
	~CNode();
	void setNext(CNode* pNode);
	CNode* getNext();

	void setPrev(CNode* pNode);
	CNode* getPrev();

	char*    getFuncName();
	char*    getFuncDLL();
	wchar_t* getFuncDLLW();

	char*    getHandlerFunc();
	char*    getHandlerDLL();
	wchar_t* getHandlerDLLW();

	void setSent(bool bSent);
	bool getSent();

	void setDeleted(bool bDeleted);
	bool getDeleted();
};

#endif
