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

#ifdef  HEATAPI_EXPORTS
#define HEATAPI_API __declspec(dllexport)
#else
#define HEATAPI_API __declspec(dllimport)
#endif

class HEATAPI_API CIntercepts
{
	CNode* m_pFirst;
	CNode* findNode(char* pszFuncName, char* pszFuncDLL);

public:
	CIntercepts();
	void removeAllIntercepts();

	~CIntercepts();

	CNode* addIntercept(char *pszFuncName, char *pszFuncDLL, char *pszHandlerFunc, char *pszHandlerDLL);
	bool removeIntercept(char *pszFuncName, char *pszFuncDLL);

	CNode* getIntercept(char *pszFuncName, char *pszFuncDLL);
    CNode* getInterceptList();

    void markAllUnsent();
};

#endif
