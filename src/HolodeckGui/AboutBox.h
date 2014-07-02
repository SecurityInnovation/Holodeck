//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AboutBox.h
//
// DESCRIPTION: Contains definition for the class AboutBox
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "UIEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*********************************************************************
	// defines the about box for the Holodeck UI
	//*********************************************************************
	public __gc class AboutBox : public System::Windows::Forms::Form
	{
	public:
		AboutBox();
		~AboutBox();

	private: // UI COMPONENT MEMBERS //
		LinkLabel *webSiteLinkLabel;
		LinkLabel *magicLibraryLinkLabel;

		void InitializeUI();
		void webSiteLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);
		void magicLibraryLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);
	};
}