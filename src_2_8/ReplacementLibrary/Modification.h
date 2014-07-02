//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Modification.h
//
// DESCRIPTION: Contains definition for the class Modification.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Oct 2003		 R. Wagner	 File created.
//*************************************************************************

#ifndef __MODIFICATION_H__
#define __MODIFICATION_H__

#include "SiString.h"
#include "SiArray.h"


namespace Replacement
{
	enum ModificationType {MOD_SEARCH_REPLACE, MOD_RAND_BYTES};

	// Modification structure contains the description of a single data modification filter
	class Modification
	{
		SiUtils::SiString sExpression;

		// Parsed expression data
		ModificationType type;

		SiUtils::SiString sRegExpr;
		SiUtils::SiString sReplacement;
		DWORD dwProbability;
		bool overwrite;

		bool valid;

		bool Parse();

	public:
		Modification();
		Modification(const SiUtils::SiString& expr);

		void SetExpression(const SiUtils::SiString& expr);

		bool IsValid() const;
		ModificationType GetType() const;
		SiUtils::SiString GetRegExpr() const;
		SiUtils::SiString GetReplacementStr() const;
		DWORD GetProbability() const;
		bool GetOverwrite() const;
	};

	typedef SiUtils::SiArray<Modification> ModificationList;
}

#endif
