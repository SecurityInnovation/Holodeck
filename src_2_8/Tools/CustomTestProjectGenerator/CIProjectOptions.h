//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CIProjectOptions.h
//
// DESCRIPTION: Contains definitions for the class CIProjectOptions
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
#using <GuiSupport.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Crownwood::Magic::Controls;
using namespace GuiSupport;

namespace CustomTestProjectGenerator
{
	public __gc class CIProjectOptions : public WizardPage
	{
		public: 
			CIProjectOptions(System::Drawing::Image * wizImage);
			void PageLeft();
			bool IsProjectNameValid();

			__property String* get_ProjectName() {return this->tbProjectName->Text->Trim();};
			__property String* get_ProjectLocation() {return this->tbProjectLocation->Text->Trim();};

			__property bool get_IsLangCSharp() {return this->rbCSharp->Checked;};
			__property bool get_IsLangVB() {return this->rbVB->Checked;};
			__property bool get_IsLangCPP() {return this->rbmCPP->Checked;};
			__property void set_IsLangCSharp(bool value) {this->rbCSharp->Checked = value;};
			__property void set_IsLangVB(bool value) {this->rbVB->Checked = value;};
			__property void set_IsLangCPP(bool value) {this->rbmCPP->Checked = value;};

			__property void set_SetCPPOption (bool value) {this->rbmCPP->Enabled = value;};
			__property void set_SetVBOption (bool value) {this->rbVB->Enabled = value;};
			__property void set_SetCSOption (bool value) {this->rbCSharp->Enabled = value;};
	        
		protected: 
			void Dispose(Boolean disposing);

		private:
			System::Windows::Forms::PictureBox *  pbWizardImg;
			System::Windows::Forms::GroupBox *  grpMain;
			System::Windows::Forms::Label *  lnlProjectName;
			System::Windows::Forms::TextBox *  tbProjectName;

			System::Windows::Forms::Panel *  pnlLanguage;
			System::Windows::Forms::RadioButton *  rbCSharp;
			System::Windows::Forms::RadioButton *  rbVB;
			System::Windows::Forms::RadioButton *  rbmCPP;
			System::Windows::Forms::TextBox *  tbProjectLocation;
			System::Windows::Forms::Label *  lblLocation;
			System::Windows::Forms::Button *  btnBrowseLocation;
			System::Windows::Forms::Panel *  pnlvA;
			System::Windows::Forms::Label *  lblLanguage;
			System::Windows::Forms::Panel *  pnlhA;
			System::Windows::Forms::Panel *  pnlhB;
			System::Windows::Forms::Panel *  pnlhC;
			System::ComponentModel::Container* components;

			GuiSupport::MessageBalloon * msgBalloon;
			FolderBrowserDialog * folderBrowser;

			void InitializeComponent(void);
			void tb_TextChangedHandler(Object * sender, EventArgs *e);
			void btnBrowseLocation_ClickHandler(Object * sender, EventArgs * e);
	};
}