//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogFindNextDialog.h
//
// DESCRIPTION: Contains definition for the class NetworkLogFindNextDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Feb 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "NetworkLogPane.h"
#include "NetworkMessageDetailsPane.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents the find/find next dialog for network logs
	//*************************************************************************
	public __gc class NetworkLogFindNextDialog : public System::Windows::Forms::Form
	{
	public: 
		NetworkLogFindNextDialog();

		__property void set_CurrentLogPane(NetworkLogPane *value);
		__property NetworkLogPane *get_CurrentLogPane() { return logPane; }

		__property void set_CurrentLogDetailsPane(NetworkMessageDetailsPane *value) { detailsPane = value; }
		__property NetworkMessageDetailsPane *get_CurrentLogDetailsPane() { return detailsPane; }

		void SearchNext();
		void Show();
		void Hide();
        
	protected: 
		void Dispose(Boolean disposing);

		System::Windows::Forms::Label *  label1;
		System::Windows::Forms::CheckBox *  caseSensitiveCheckBox;
		System::Windows::Forms::CheckBox *  wholeWordCheckBox;
		System::Windows::Forms::GroupBox *  groupBox1;
		System::Windows::Forms::TextBox *  searchForTextBox;
		System::Windows::Forms::CheckBox *  threadIDCheckBox;

		System::Windows::Forms::CheckBox *  protocolCheckBox;
		System::Windows::Forms::CheckBox *  sourceIPCheckBox;
		System::Windows::Forms::CheckBox *  directionCheckBox;
		System::Windows::Forms::CheckBox *  dataCheckBox;
		System::Windows::Forms::CheckBox *  destinationIPCheckBox;
		System::Windows::Forms::CheckBox *  sourcePortCheckBox;
		System::Windows::Forms::Button *  findNextButton;
		System::Windows::Forms::Button *  cancelButton;
		System::ComponentModel::Container* components;
		System::Windows::Forms::CheckBox *  destinationPortCheckBox;
		System::Windows::Forms::GroupBox *  groupBox2;
		System::Windows::Forms::RadioButton *  searchAsciiRadioButton;
		System::Windows::Forms::RadioButton *  searchHexRadioButton;

		NetworkLogPane *logPane;
		NetworkMessageDetailsPane *detailsPane;
		bool entryFound;
	private: System::Windows::Forms::Panel *  pnlSearchDirection;
	private: System::Windows::Forms::Label *  lblSrchDirection;
	private: System::Windows::Forms::RadioButton *  rbSrchDn;
	private: System::Windows::Forms::RadioButton *  rbSrchUp;
			 System::Threading::Thread *SearchThread;

		void SearchThreadFunc();
		bool MatchString(String *baseString, String *matchString, bool matchWhole, bool caseSensitive);
		void SelectFoundEntry(unsigned int entry);

		void InitializeComponent(void)
		{
			this->label1 = new System::Windows::Forms::Label();
			this->searchForTextBox = new System::Windows::Forms::TextBox();
			this->caseSensitiveCheckBox = new System::Windows::Forms::CheckBox();
			this->wholeWordCheckBox = new System::Windows::Forms::CheckBox();
			this->groupBox1 = new System::Windows::Forms::GroupBox();
			this->sourcePortCheckBox = new System::Windows::Forms::CheckBox();
			this->destinationIPCheckBox = new System::Windows::Forms::CheckBox();
			this->dataCheckBox = new System::Windows::Forms::CheckBox();
			this->directionCheckBox = new System::Windows::Forms::CheckBox();
			this->sourceIPCheckBox = new System::Windows::Forms::CheckBox();
			this->protocolCheckBox = new System::Windows::Forms::CheckBox();
			this->destinationPortCheckBox = new System::Windows::Forms::CheckBox();
			this->threadIDCheckBox = new System::Windows::Forms::CheckBox();
			this->findNextButton = new System::Windows::Forms::Button();
			this->cancelButton = new System::Windows::Forms::Button();
			this->groupBox2 = new System::Windows::Forms::GroupBox();
			this->searchHexRadioButton = new System::Windows::Forms::RadioButton();
			this->searchAsciiRadioButton = new System::Windows::Forms::RadioButton();
			this->pnlSearchDirection = new System::Windows::Forms::Panel();
			this->lblSrchDirection = new System::Windows::Forms::Label();
			this->rbSrchDn = new System::Windows::Forms::RadioButton();
			this->rbSrchUp = new System::Windows::Forms::RadioButton();
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->pnlSearchDirection->SuspendLayout();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(8, 16);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(48, 24);
			this->label1->TabIndex = 0;
			this->label1->Text = S"Look for";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// searchForTextBox
			// 
			this->searchForTextBox->Location = System::Drawing::Point(56, 16);
			this->searchForTextBox->Name = S"searchForTextBox";
			this->searchForTextBox->Size = System::Drawing::Size(472, 20);
			this->searchForTextBox->TabIndex = 1;
			this->searchForTextBox->Text = S"";
			// 
			// caseSensitiveCheckBox
			// 
			this->caseSensitiveCheckBox->Location = System::Drawing::Point(56, 40);
			this->caseSensitiveCheckBox->Name = S"caseSensitiveCheckBox";
			this->caseSensitiveCheckBox->TabIndex = 2;
			this->caseSensitiveCheckBox->Text = S"Case sensitive";
			// 
			// wholeWordCheckBox
			// 
			this->wholeWordCheckBox->Location = System::Drawing::Point(160, 40);
			this->wholeWordCheckBox->Name = S"wholeWordCheckBox";
			this->wholeWordCheckBox->Size = System::Drawing::Size(120, 24);
			this->wholeWordCheckBox->TabIndex = 3;
			this->wholeWordCheckBox->Text = S"Match whole word";
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->sourcePortCheckBox);
			this->groupBox1->Controls->Add(this->destinationIPCheckBox);
			this->groupBox1->Controls->Add(this->dataCheckBox);
			this->groupBox1->Controls->Add(this->directionCheckBox);
			this->groupBox1->Controls->Add(this->sourceIPCheckBox);
			this->groupBox1->Controls->Add(this->protocolCheckBox);
			this->groupBox1->Controls->Add(this->destinationPortCheckBox);
			this->groupBox1->Controls->Add(this->threadIDCheckBox);
			this->groupBox1->Location = System::Drawing::Point(56, 64);
			this->groupBox1->Name = S"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(392, 80);
			this->groupBox1->TabIndex = 4;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = S"Search fields";
			// 
			// sourcePortCheckBox
			// 
			this->sourcePortCheckBox->Checked = true;
			this->sourcePortCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->sourcePortCheckBox->Location = System::Drawing::Point(288, 48);
			this->sourcePortCheckBox->Name = S"sourcePortCheckBox";
			this->sourcePortCheckBox->Size = System::Drawing::Size(88, 24);
			this->sourcePortCheckBox->TabIndex = 7;
			this->sourcePortCheckBox->Text = S"Source Port";
			// 
			// destinationIPCheckBox
			// 
			this->destinationIPCheckBox->Checked = true;
			this->destinationIPCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->destinationIPCheckBox->Location = System::Drawing::Point(176, 16);
			this->destinationIPCheckBox->Name = S"destinationIPCheckBox";
			this->destinationIPCheckBox->Size = System::Drawing::Size(96, 24);
			this->destinationIPCheckBox->TabIndex = 6;
			this->destinationIPCheckBox->Text = S"Destination IP";
			// 
			// dataCheckBox
			// 
			this->dataCheckBox->Location = System::Drawing::Point(96, 48);
			this->dataCheckBox->Name = S"dataCheckBox";
			this->dataCheckBox->Size = System::Drawing::Size(80, 24);
			this->dataCheckBox->TabIndex = 5;
			this->dataCheckBox->Text = S"Data";
			this->dataCheckBox->CheckedChanged += new System::EventHandler(this, dataCheckBox_CheckedChanged);
			// 
			// directionCheckBox
			// 
			this->directionCheckBox->Checked = true;
			this->directionCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->directionCheckBox->Location = System::Drawing::Point(16, 48);
			this->directionCheckBox->Name = S"directionCheckBox";
			this->directionCheckBox->Size = System::Drawing::Size(80, 24);
			this->directionCheckBox->TabIndex = 4;
			this->directionCheckBox->Text = S"Direction";
			// 
			// sourceIPCheckBox
			// 
			this->sourceIPCheckBox->Checked = true;
			this->sourceIPCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->sourceIPCheckBox->Location = System::Drawing::Point(288, 16);
			this->sourceIPCheckBox->Name = S"sourceIPCheckBox";
			this->sourceIPCheckBox->Size = System::Drawing::Size(80, 24);
			this->sourceIPCheckBox->TabIndex = 3;
			this->sourceIPCheckBox->Text = S"Source IP";
			// 
			// protocolCheckBox
			// 
			this->protocolCheckBox->Checked = true;
			this->protocolCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->protocolCheckBox->Location = System::Drawing::Point(96, 16);
			this->protocolCheckBox->Name = S"protocolCheckBox";
			this->protocolCheckBox->Size = System::Drawing::Size(80, 24);
			this->protocolCheckBox->TabIndex = 2;
			this->protocolCheckBox->Text = S"Protocol";
			// 
			// destinationPortCheckBox
			// 
			this->destinationPortCheckBox->Checked = true;
			this->destinationPortCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->destinationPortCheckBox->Location = System::Drawing::Point(176, 48);
			this->destinationPortCheckBox->Name = S"destinationPortCheckBox";
			this->destinationPortCheckBox->TabIndex = 1;
			this->destinationPortCheckBox->Text = S"Destination Port";
			// 
			// threadIDCheckBox
			// 
			this->threadIDCheckBox->Checked = true;
			this->threadIDCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->threadIDCheckBox->Location = System::Drawing::Point(16, 16);
			this->threadIDCheckBox->Name = S"threadIDCheckBox";
			this->threadIDCheckBox->Size = System::Drawing::Size(80, 24);
			this->threadIDCheckBox->TabIndex = 0;
			this->threadIDCheckBox->Text = S"Thread ID";
			// 
			// findNextButton
			// 
			this->findNextButton->Location = System::Drawing::Point(456, 144);
			this->findNextButton->Name = S"findNextButton";
			this->findNextButton->Size = System::Drawing::Size(72, 23);
			this->findNextButton->TabIndex = 5;
			this->findNextButton->Text = S"Find Next";
			this->findNextButton->Click += new System::EventHandler(this, findNextButton_Click);
			// 
			// cancelButton
			// 
			this->cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->cancelButton->Location = System::Drawing::Point(456, 176);
			this->cancelButton->Name = S"cancelButton";
			this->cancelButton->Size = System::Drawing::Size(72, 23);
			this->cancelButton->TabIndex = 6;
			this->cancelButton->Text = S"Cancel";
			this->cancelButton->Click += new System::EventHandler(this, cancelButton_Click);
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->searchHexRadioButton);
			this->groupBox2->Controls->Add(this->searchAsciiRadioButton);
			this->groupBox2->Location = System::Drawing::Point(56, 144);
			this->groupBox2->Name = S"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(392, 56);
			this->groupBox2->TabIndex = 7;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = S"Payload Data Search Options";
			// 
			// searchHexRadioButton
			// 
			this->searchHexRadioButton->Enabled = false;
			this->searchHexRadioButton->Location = System::Drawing::Point(224, 24);
			this->searchHexRadioButton->Name = S"searchHexRadioButton";
			this->searchHexRadioButton->Size = System::Drawing::Size(128, 24);
			this->searchHexRadioButton->TabIndex = 1;
			this->searchHexRadioButton->Text = S"Search as Hex";
			// 
			// searchAsciiRadioButton
			// 
			this->searchAsciiRadioButton->Checked = true;
			this->searchAsciiRadioButton->Enabled = false;
			this->searchAsciiRadioButton->Location = System::Drawing::Point(16, 24);
			this->searchAsciiRadioButton->Name = S"searchAsciiRadioButton";
			this->searchAsciiRadioButton->Size = System::Drawing::Size(144, 24);
			this->searchAsciiRadioButton->TabIndex = 0;
			this->searchAsciiRadioButton->TabStop = true;
			this->searchAsciiRadioButton->Text = S"Search as ASCII";
			// 
			// pnlSearchDirection
			// 
			this->pnlSearchDirection->Controls->Add(this->lblSrchDirection);
			this->pnlSearchDirection->Controls->Add(this->rbSrchDn);
			this->pnlSearchDirection->Controls->Add(this->rbSrchUp);
			this->pnlSearchDirection->Location = System::Drawing::Point(288, 40);
			this->pnlSearchDirection->Name = S"pnlSearchDirection";
			this->pnlSearchDirection->Size = System::Drawing::Size(176, 24);
			this->pnlSearchDirection->TabIndex = 9;
			// 
			// lblSrchDirection
			// 
			this->lblSrchDirection->Location = System::Drawing::Point(-8, 4);
			this->lblSrchDirection->Name = S"lblSrchDirection";
			this->lblSrchDirection->Size = System::Drawing::Size(56, 16);
			this->lblSrchDirection->TabIndex = 5;
			this->lblSrchDirection->Text = S"Search:";
			this->lblSrchDirection->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// rbSrchDn
			// 
			this->rbSrchDn->Checked = true;
			this->rbSrchDn->Location = System::Drawing::Point(56, 4);
			this->rbSrchDn->Name = S"rbSrchDn";
			this->rbSrchDn->Size = System::Drawing::Size(56, 16);
			this->rbSrchDn->TabIndex = 2;
			this->rbSrchDn->TabStop = true;
			this->rbSrchDn->Text = S"Down";
			// 
			// rbSrchUp
			// 
			this->rbSrchUp->Location = System::Drawing::Point(128, 4);
			this->rbSrchUp->Name = S"rbSrchUp";
			this->rbSrchUp->Size = System::Drawing::Size(40, 16);
			this->rbSrchUp->TabIndex = 1;
			this->rbSrchUp->Text = S"Up";
			// 
			// NetworkLogFindNextDialog
			// 
			this->AcceptButton = this->findNextButton;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->CancelButton = this->cancelButton;
			this->ClientSize = System::Drawing::Size(536, 204);
			this->ControlBox = false;
			this->Controls->Add(this->pnlSearchDirection);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->cancelButton);
			this->Controls->Add(this->findNextButton);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->wholeWordCheckBox);
			this->Controls->Add(this->caseSensitiveCheckBox);
			this->Controls->Add(this->searchForTextBox);
			this->Controls->Add(this->label1);
			this->Name = S"NetworkLogFindNextDialog";
			this->ShowInTaskbar = false;
			this->Text = S"Find in Network Logs";
			this->TopMost = true;
			this->groupBox1->ResumeLayout(false);
			this->groupBox2->ResumeLayout(false);
			this->pnlSearchDirection->ResumeLayout(false);
			this->ResumeLayout(false);

		}		
		
		System::Void cancelButton_Click(System::Object *  sender, System::EventArgs *  e)
		{
			this->findNextButton->Enabled = true;
		
			if (String::Compare (this->cancelButton->Text, "Stop", true) == 0)
			{
				if (SearchThread != NULL && SearchThread->IsAlive)
					SearchThread->Abort();
				
				this->cancelButton->Text = "Cancel";
				this->findNextButton->Enabled = true;
			}
			else
			this->Hide();
		}

		System::Void findNextButton_Click(System::Object *  sender, System::EventArgs *  e)
		{
			SearchNext();
		}

		System::Void dataCheckBox_CheckedChanged(System::Object *  sender, System::EventArgs *  e)
		{
			searchAsciiRadioButton->Enabled = dataCheckBox->Checked;
			searchHexRadioButton->Enabled = dataCheckBox->Checked;
		}

};
}