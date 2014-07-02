//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TimeTrialNoticeDialog.h
//
// DESCRIPTION: Contains definition for the class TimeTrialNoticeDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 May 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "HTMLBrowser.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace HolodeckGui
{
	//*************************************************************************
	// class that provides a dialog display information about the time trial
	//*************************************************************************
	public __gc class TimeTrialNoticeDialog : public System::Windows::Forms::Form
	{
	public: 
		TimeTrialNoticeDialog(int daysLeft);
        
	protected: 
		void Dispose(Boolean disposing);
		void InitializeComponent();
		void OnLoad(EventArgs *args);
		void registerButton_Click(System::Object *sender, System::EventArgs *e);
		void continueButton_Click(System::Object *sender, System::EventArgs *e);
		void purchaseLinkLabel_LinkClicked(System::Object *sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *e);

		System::Windows::Forms::PictureBox *logoPictureBox;
		System::Windows::Forms::Label *infoLabel;
		System::Windows::Forms::LinkLabel *purchaseLinkLabel;
		System::Windows::Forms::Label *label1;
		System::Windows::Forms::GroupBox *groupBox1;
		System::Windows::Forms::Label *label2;
		System::Windows::Forms::Button *registerButton;
		System::Windows::Forms::Button *continueButton;
		System::ComponentModel::Container *components;
		int numDaysLeft;
	};
}