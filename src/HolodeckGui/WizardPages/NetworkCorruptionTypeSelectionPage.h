//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionTypeSelectionPage.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionTypeSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the type selection page of the network corruption wizard
	//*************************************************************************
	public __gc class NetworkCorruptionTypeSelectionPage : public WizardPage
	{
	public:
		NetworkCorruptionTypeSelectionPage();
		~NetworkCorruptionTypeSelectionPage();

		__property String* get_CorruptType();
		__property void set_CorruptType(String* value);

	protected:
		RadioButton *random;
		RadioButton *findAndReplace;
		RadioButton *regExpr;
	};
}