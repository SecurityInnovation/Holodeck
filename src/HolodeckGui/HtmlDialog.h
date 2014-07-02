//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		HtmlDialog.h
//
// DESCRIPTION: Contains the definition for the class HtmlDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Feb 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "HTMLBrowser.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace AxSHDocVw;
using namespace MSHTML;

namespace HolodeckGui
{
	public __gc class HtmlDialog : public System::Windows::Forms::Form
	{
	public: 
		HtmlDialog(String *htmlFileName)
		{
			InitializeComponent();
			url = htmlFileName;
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

		System::Windows::Forms::Button *closeButton;
		System::Windows::Forms::Panel *buttonPanel;
		System::ComponentModel::Container* components;
		HTMLBrowser * browser;
		String *url;

		void InitializeComponent(void)
		{
			System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(HolodeckGui::HtmlDialog));
			this->closeButton = new System::Windows::Forms::Button();
			this->buttonPanel = new System::Windows::Forms::Panel();
			this->buttonPanel->SuspendLayout();
			this->SuspendLayout();
			// 
			// closeButton
			// 
			this->closeButton->Anchor = (System::Windows::Forms::AnchorStyles)(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right);
			this->closeButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->closeButton->Location = System::Drawing::Point(584, 8);
			this->closeButton->Name = S"closeButton";
			this->closeButton->TabIndex = 0;
			this->closeButton->Text = S"Close";
			// 
			// buttonPanel
			// 
			this->buttonPanel->Controls->Add(this->closeButton);
			this->buttonPanel->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->buttonPanel->Location = System::Drawing::Point(0, 438);
			this->buttonPanel->Name = S"buttonPanel";
			this->buttonPanel->Size = System::Drawing::Size(672, 40);
			this->buttonPanel->TabIndex = 0;
			//
			// browser
			//
			browser = new HTMLBrowser();
			// 
			// HtmlDialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(672, 478);
			this->Controls->Add(browser->browser);
			this->Controls->Add(this->buttonPanel);
			this->Icon = (__try_cast<System::Drawing::Icon *  >(resources->GetObject(S"$this.Icon")));
			this->MinimizeBox = false;
			this->Name = S"HtmlDialog";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = S"Holodeck";
			this->Load += new System::EventHandler(this, HtmlDialog_Load);
			this->buttonPanel->ResumeLayout(false);
			this->ResumeLayout(false);

		}		
		
		System::Void HtmlDialog_Load(System::Object *sender, System::EventArgs *e)
		{
			if (url)
				browser->Navigate(url);
		}
	};
}