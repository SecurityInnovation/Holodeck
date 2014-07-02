//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// File:		ManagedRegistrationData.cpp
//
// DESCRIPTION: Contains definition for the managed wrapper around the
//	RegistrationKeyGenerator class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 09 Sep 2003		 B Shirey	 File created.
//*************************************************************************
#pragma once

#include "ProductRegistrationLibrary.h"
#include "ManagedRegistrationData.h"

using namespace System;
using namespace RegistrationLibrary;

namespace ManagedRegistrationLibrary
{
	//*************************************************************************
	// The managed wrapper around the RegistrationKeyGenerator class
	//*************************************************************************
	public __gc class ManagedRegistrationKeyGenerator
	{
	protected:
		RegistrationKeyGenerator *keyGenerator;

		static ManagedRegistrationKeyGenerator *instance = NULL;

		ManagedRegistrationKeyGenerator();
		~ManagedRegistrationKeyGenerator();
	public:
		static ManagedRegistrationKeyGenerator *GetInstance();

		bool GenerateRegistrationKey(ManagedRegistrationData *data);
		bool DecodeRegistrationKey(ManagedRegistrationData *data);
	};
}