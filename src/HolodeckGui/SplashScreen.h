//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SplashScreen.h
//
// DESCRIPTION: Contains definition for the class SplashScreen
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

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
	// defines the splash screen for the Holodeck UI
	//*********************************************************************
	public __gc class SplashScreen : public System::Windows::Forms::Form
	{
	public:
		SplashScreen();
		~SplashScreen();

	private: // UI COMPONENT MEMBERS //
		Label *webSiteLinkLabel;

		void InitializeUI();
		//void webSiteLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);
	};
}