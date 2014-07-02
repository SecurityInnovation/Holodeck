//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2004 – All Rights Reserved.
//
// FILE:		FormStateSaver.cpp
//
// DESCRIPTION: Contains definition for the FormStateSaver class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 23 Jun 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <MsCorLib.dll>
#using <System.dll>
#using <System.Drawing.Dll>
#using <System.Windows.Forms.Dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Microsoft::Win32;

namespace SIControls
{
	public __delegate void WindowStateDelegate(Object *sender, RegistryKey *key);

	public __gc class FormStateSaver
	{
	public:
		FormStateSaver();
		~FormStateSaver();

		WindowStateDelegate *LoadState;
		WindowStateDelegate *SaveState;

		__property void set_Parent(Form *value);
		__property Form *get_Parent() { return parent; }

		__property void set_RegistryPath(String *value) { regPath = value; }
		__property String *get_RegistryPath() { return regPath; }

		__property void set_AllowSaveMinimized(bool value) { allowSaveMinimized = value; }
		__property bool get_AllowSaveMinimized() { return allowSaveMinimized; }

	private:
		void OnResize(Object *sender, EventArgs *args);
		void OnMove(Object *sender, EventArgs *args);
		void OnClosing(Object *sender, CancelEventArgs *args);
		void OnLoad(Object *sender, EventArgs *args);

		Form *parent;
		String *regPath;
		int normalLeft;
		int normalTop;
		int normalWidth;
		int normalHeight;
		FormWindowState windowState;
		bool allowSaveMinimized;
	};
}