//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		DockingManagerStateSaver.h
//
// DESCRIPTION: Contains definition for the class DockingManagerStateSaver
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 1 Jul 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>

#using <mscorlib.dll>
#using <system.dll>
#using <system.drawing.dll>
#using <system.windows.forms.dll>
#using <magiclibrary.dll>

using namespace System;
using namespace System::IO;
using namespace System::Windows::Forms;
using namespace Microsoft::Win32;
using namespace Crownwood::Magic::Docking;

namespace SIControls
{
	//*********************************************************************
	// loads and stores states of a docking manager
	//*********************************************************************
	public __gc class DockingManagerStateSaver
	{
	public:
		DockingManagerStateSaver();
		~DockingManagerStateSaver();

		bool IsSomeStateSaved();
		void SaveDefaultValuesFromFile(String *fileName);

		void LoadData();
		void SaveData();

		__property void set_DockingManagerToProcess(DockingManager *value) { manager = value; }
		__property DockingManager *get_DockingManagerToProcess() { return manager; }

		__property void set_RegistrySubkey(String *value) { regPath = value; }
		__property String *get_RegistrySubkey() { return regPath; }

		__property void set_RegistryValue(String *value) { regValue = value; }
		__property String *get_RegistryValue() { return regValue; }

	private:
		void SaveData(Byte data[]);

		DockingManager *manager;
		String *regPath;
		String *regValue;
	};
}