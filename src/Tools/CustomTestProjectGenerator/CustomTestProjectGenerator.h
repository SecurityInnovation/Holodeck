//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomTestProjectGenerator.h
//
// DESCRIPTION: Contains definitions for the class CustomTestProjectGenerator
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	---------------------------------------
// 12 Feb 2004		 Ady K			Fiel created
//*************************************************************************
#include "CustomTestProjectWizard.h"

#using <mscorlib.dll>
#using <system.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Threading;

namespace CustomTestProjectGenerator
{
	public __gc class CustomTestProjectGen
	{
		public:
			CustomTestProjectGen();
			~CustomTestProjectGen();
	};
}