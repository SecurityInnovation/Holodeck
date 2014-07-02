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

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Crownwood::Magic::Controls;

namespace CustomTestProjectGenerator
{
	public __gc class CIProjectSummary : public WizardPage
	{
		public: 
			CIProjectSummary(System::Drawing::Image * wizImage);
			__property void set_ProgressLevel(int value) {this->progressBar->Value = value;};
			__property void set_SummaryText(String * value) {this->lblInfo->Text = value;};
	        
		protected: 
			void Dispose(Boolean disposing);
			
		private:		
			System::Windows::Forms::ProgressBar * progressBar;
			System::Windows::Forms::PictureBox *  pbWizardImg;
			System::Windows::Forms::GroupBox *  grpMain;
			System::Windows::Forms::TextBox *  lblInfo;
			System::ComponentModel::Container* components;

			void InitializeComponent(void);
	};
}