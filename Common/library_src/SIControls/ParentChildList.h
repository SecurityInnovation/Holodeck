//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ParentChildList.h
//
// DESCRIPTION: Contains definition for the interface ParentChildList
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 17 Nov 2003		 B. Shirey	  File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>

using namespace System;

namespace SIControls
{
	//*********************************************************************
	// Provides an interface for describing a child parent relationship
	//*********************************************************************
	public __gc __interface ParentChildList
	{
		ParentChildList *ParentNode();
		
		ParentChildList *PreviousSibling();
		ParentChildList *NextSibling();

		ParentChildList *FirstChild();
		ParentChildList *NextChild();
		ParentChildList *PreviousChild();
		ParentChildList *LastChild();
	};
}