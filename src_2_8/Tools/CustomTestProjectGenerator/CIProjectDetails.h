//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CIProjectDetails.h
//
// DESCRIPTION: Contains definitions  for the class CIProjectDetails
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
#using <FunctionDatabase.dll>

#include "FunctionTreeView.h"
#include "defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace FunctionDatabase;
using namespace Crownwood::Magic::Controls;

namespace CustomTestProjectGenerator
{
	
	public __gc class CIProjectDetails : public WizardPage
	{
		public: 
			CIProjectDetails(System::Drawing::Image * wizImage);

			__property void set_IsAUTNative(bool value);
			__property bool get_IsAUTNative() {return this->__isAUTNative;};

			__property ArrayList * get_SelectedFunctions(){return this->fTreeView->EnabledFunctionsList;};
	        
		protected: 
			void Dispose(Boolean disposing);
		
		private:		
			bool	__isAUTNative;
			FunctionTreeView * fTreeView;
			System::Windows::Forms::PictureBox *  pbWizardImg;
			System::Windows::Forms::GroupBox *  grpMain;
			System::Windows::Forms::Panel *  pnlFunctionTree;
			System::Windows::Forms::Label *  lblInfo2;
			System::Windows::Forms::Label *  lblInfo;
			System::Windows::Forms::CheckBox * chkbxFaultInjection;
			
			FunctionDatabase::FaultFunctionDB * faultDB;
			FunctionDatabase::InterceptedFunctionDB * funcDB;

			System::ComponentModel::Container* components;
			void InitializeComponent(void);
			void chkbxFaultInjection_Checked (Object * sender, EventArgs * e);
	};
}