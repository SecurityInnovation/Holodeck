//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomInterceptProject.h
//
// DESCRIPTION: Contains definition for the class CustomInterceptProject
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 12 Feb 2004		 Ady K	   File Created
//*******************************************************************************
#pragma once
#using <MagicLibrary.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Crownwood::Magic::Controls;

namespace CustomTestProjectGenerator
{
	public __gc class CustomInterceptProject : public WizardPage
	{
		public: 
			CustomInterceptProject(System::Drawing::Image * wizImage);

			__property String* get_AutPath() {return this->tbAUT->Text;};
			__property String* get_AutCmdLine() {return this->tbCmdLine->Text;};
			__property bool get_IsLaunchAUTSelected() {return this->rbExecutable->Checked;};
			__property bool get_IsAttactAUTSelected() {return this->rbProcessID->Checked;};
			__property bool get_IsAttachDotNet() {return this->rbDotNetApp->Checked;};
			__property bool get_IsAttachNative() {return this->rbNativeApp->Checked;};
			__property bool get_IsProcessChainingEnabled() {return this->chkProcessChain->Checked;};
	        
		protected: 
			void Dispose(Boolean disposing);

		private:
			OpenFileDialog * browseDialog;

			System::Windows::Forms::PictureBox *  pbWizardImg;
			System::Windows::Forms::GroupBox *  grpMain;
			System::Windows::Forms::RadioButton *  rbExecutable;
			System::Windows::Forms::RadioButton *  rbProcessID;
			System::Windows::Forms::Label *  lblInfo;
			System::Windows::Forms::RadioButton *  rbNativeApp;
			System::Windows::Forms::RadioButton *  rbDotNetApp;
			System::Windows::Forms::Panel *  pnlhA;
			System::Windows::Forms::Panel *  pnlAttach;
			System::Windows::Forms::Panel *  pnlvA;
			System::Windows::Forms::Panel *  pnlhB;
			System::Windows::Forms::Panel *  pnlLaunch;
			System::Windows::Forms::Button *  btnBrowseApp;
			System::Windows::Forms::TextBox *  tbAUT;
			System::Windows::Forms::Label *  lblCmdLine;
			System::Windows::Forms::TextBox *  tbCmdLine;
			System::Windows::Forms::CheckBox *  chkProcessChain;
			System::ComponentModel::Container* components;

			void InitializeComponent(void);
			void btnBrowseApp_ClickEventHandler(Object * sender, EventArgs * e);
			void radio_CheckedChangedHandler(Object * sender, EventArgs * e);
	};
}