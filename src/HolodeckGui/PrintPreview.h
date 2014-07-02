//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PrintPreview.h
//
// DESCRIPTION: Contains definition for the class PrintPreview
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 22 Apr 2004		 A. Kakrania	File created.
//*************************************************************************

#pragma once

#include "LogPane.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace HolodeckGui
{
	/// <summary> 
	/// Summary for PrintPreview
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class PrintPreview : public System::Windows::Forms::Form
	{
	public: 
		PrintPreview(Printing::PrintDocument *pd, String * docTitle, ArrayList * LogColumns, Icon *icon);
		__property void set_MaxPages(int value) {maxPages = value; this->lblMaxPages->Text = String::Concat ("Total Pages: ", this->maxPages.ToString());}
		__property ListView::ColumnHeaderCollection* get_ColumnHeaders() {return this->lvColumnSizer->Columns;}
		
		int GetColumnWidthPercent(String * columnHeaderText);
		__property bool get_WrapParams() {return this->chbxWrapParams->Checked;}
		__property bool get_HideWrapOption() {return !this->chbxWrapParams->Visible;}
		__property void set_HideWrapOption(bool value) {this->chbxWrapParams->Visible = !value;}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Panel *  pnlControls;

	private: System::Windows::Forms::PrintPreviewControl *  ppvControl;
	private: System::Windows::Forms::Panel *  pnlButtons;
	private: System::Windows::Forms::Button *  btnPrint;
	private: System::Windows::Forms::Panel *  pnlPageControl;
	private: System::Windows::Forms::Button *  btnPrevPage;
	private: System::Windows::Forms::Button *  btnNextPage;
	private: System::Windows::Forms::TextBox *  tbPageNumber;
	private: System::Windows::Forms::Panel *  pnlCenter;
	private: System::Windows::Forms::Button *  button2;
	private: System::Windows::Forms::Button *  bntLastPage;
	private: int maxPages;
	private: Hashtable * columnWidthPercents;

	private: ArrayList * logColumns;
	private: System::Windows::Forms::Panel *  pnlColumnWidths;
	private: System::Windows::Forms::ListView *  lvColumnSizer;
	private: System::Windows::Forms::Panel *  pnlPreviewControl;
	private: System::Windows::Forms::Panel *  pnlBlack;
	private: System::Windows::Forms::Panel *  pnlBlackLt;
	private: System::Windows::Forms::Panel *  pnlBlackRt;
	private: System::Windows::Forms::Label *  lblAdjustInfo;
	private: System::Windows::Forms::LinkLabel *  lnkGo;
	private: System::Windows::Forms::Button *  btnClose;
	private: System::Windows::Forms::CheckBox *  chbxWrapParams;
	private: System::Windows::Forms::PrintDialog *  printDialog;
	private: System::Windows::Forms::Label *  lblMaxPages;
	private: System::Windows::Forms::Label *  lblZoom;
	private: System::Windows::Forms::ComboBox *  cmbZoom;


	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pnlControls = new System::Windows::Forms::Panel();
			this->pnlCenter = new System::Windows::Forms::Panel();
			this->cmbZoom = new System::Windows::Forms::ComboBox();
			this->lblZoom = new System::Windows::Forms::Label();
			this->pnlPageControl = new System::Windows::Forms::Panel();
			this->lblMaxPages = new System::Windows::Forms::Label();
			this->btnNextPage = new System::Windows::Forms::Button();
			this->tbPageNumber = new System::Windows::Forms::TextBox();
			this->lnkGo = new System::Windows::Forms::LinkLabel();
			this->button2 = new System::Windows::Forms::Button();
			this->bntLastPage = new System::Windows::Forms::Button();
			this->btnPrevPage = new System::Windows::Forms::Button();
			this->pnlButtons = new System::Windows::Forms::Panel();
			this->chbxWrapParams = new System::Windows::Forms::CheckBox();
			this->btnClose = new System::Windows::Forms::Button();
			this->btnPrint = new System::Windows::Forms::Button();
			this->pnlColumnWidths = new System::Windows::Forms::Panel();
			this->lblAdjustInfo = new System::Windows::Forms::Label();
			this->pnlBlackRt = new System::Windows::Forms::Panel();
			this->pnlBlackLt = new System::Windows::Forms::Panel();
			this->pnlBlack = new System::Windows::Forms::Panel();
			this->lvColumnSizer = new System::Windows::Forms::ListView();
			this->pnlPreviewControl = new System::Windows::Forms::Panel();
			this->pnlControls->SuspendLayout();
			this->pnlCenter->SuspendLayout();
			this->pnlPageControl->SuspendLayout();
			this->pnlButtons->SuspendLayout();
			this->pnlColumnWidths->SuspendLayout();
			this->SuspendLayout();
			// 
			// pnlControls
			// 
			this->pnlControls->Controls->Add(this->pnlCenter);
			this->pnlControls->Controls->Add(this->pnlPageControl);
			this->pnlControls->Controls->Add(this->pnlButtons);
			this->pnlControls->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlControls->Location = System::Drawing::Point(0, 0);
			this->pnlControls->Name = S"pnlControls";
			this->pnlControls->Size = System::Drawing::Size(814, 40);
			this->pnlControls->TabIndex = 0;
			// 
			// pnlCenter
			// 
			this->pnlCenter->Controls->Add(this->cmbZoom);
			this->pnlCenter->Controls->Add(this->lblZoom);
			this->pnlCenter->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlCenter->Location = System::Drawing::Point(320, 0);
			this->pnlCenter->Name = S"pnlCenter";
			this->pnlCenter->Size = System::Drawing::Size(246, 40);
			this->pnlCenter->TabIndex = 6;
			// 
			// cmbZoom
			// 
			this->cmbZoom->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			System::Object* __mcTemp__1[] = new System::Object*[8];
			__mcTemp__1[0] = S"Auto";
			__mcTemp__1[1] = S"50%";
			__mcTemp__1[2] = S"75%";
			__mcTemp__1[3] = S"100%";
			__mcTemp__1[4] = S"125%";
			__mcTemp__1[5] = S"150%";
			__mcTemp__1[6] = S"200%";
			__mcTemp__1[7] = S"300%";
			this->cmbZoom->Items->AddRange(__mcTemp__1);
			this->cmbZoom->Location = System::Drawing::Point(48, 10);
			this->cmbZoom->Name = S"cmbZoom";
			this->cmbZoom->Size = System::Drawing::Size(88, 21);
			this->cmbZoom->TabIndex = 1;
			this->cmbZoom->SelectedIndexChanged += new System::EventHandler(this, cmbZoom_SelectedIndexChanged);
			// 
			// lblZoom
			// 
			this->lblZoom->Location = System::Drawing::Point(2, 8);
			this->lblZoom->Name = S"lblZoom";
			this->lblZoom->Size = System::Drawing::Size(40, 24);
			this->lblZoom->TabIndex = 0;
			this->lblZoom->Text = S"Zoom";
			this->lblZoom->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// pnlPageControl
			// 
			this->pnlPageControl->Controls->Add(this->lblMaxPages);
			this->pnlPageControl->Controls->Add(this->btnNextPage);
			this->pnlPageControl->Controls->Add(this->tbPageNumber);
			this->pnlPageControl->Controls->Add(this->lnkGo);
			this->pnlPageControl->Controls->Add(this->button2);
			this->pnlPageControl->Controls->Add(this->bntLastPage);
			this->pnlPageControl->Controls->Add(this->btnPrevPage);
			this->pnlPageControl->Dock = System::Windows::Forms::DockStyle::Right;
			this->pnlPageControl->Location = System::Drawing::Point(566, 0);
			this->pnlPageControl->Name = S"pnlPageControl";
			this->pnlPageControl->Size = System::Drawing::Size(248, 40);
			this->pnlPageControl->TabIndex = 5;
			// 
			// lblMaxPages
			// 
			this->lblMaxPages->Location = System::Drawing::Point(176, 8);
			this->lblMaxPages->Name = S"lblMaxPages";
			this->lblMaxPages->Size = System::Drawing::Size(72, 32);
			this->lblMaxPages->TabIndex = 10;
			this->lblMaxPages->Text = S"Total Pages:";
			// 
			// btnNextPage
			// 
			this->btnNextPage->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnNextPage->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->btnNextPage->Location = System::Drawing::Point(112, 8);
			this->btnNextPage->Name = S"btnNextPage";
			this->btnNextPage->Size = System::Drawing::Size(24, 24);
			this->btnNextPage->TabIndex = 5;
			this->btnNextPage->Text = S"►";
			this->btnNextPage->Click += new System::EventHandler(this, btnNextPage_Click);
			// 
			// tbPageNumber
			// 
			this->tbPageNumber->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->tbPageNumber->Location = System::Drawing::Point(80, 8);
			this->tbPageNumber->MaxLength = 999;
			this->tbPageNumber->Name = S"tbPageNumber";
			this->tbPageNumber->Size = System::Drawing::Size(32, 20);
			this->tbPageNumber->TabIndex = 4;
			this->tbPageNumber->Text = S"1";
			this->tbPageNumber->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// lnkGo
			// 
			this->lnkGo->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->lnkGo->Location = System::Drawing::Point(88, 24);
			this->lnkGo->Name = S"lnkGo";
			this->lnkGo->Size = System::Drawing::Size(32, 16);
			this->lnkGo->TabIndex = 9;
			this->lnkGo->TabStop = true;
			this->lnkGo->Text = S"go";
			this->lnkGo->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, lnkGo_LinkClicked);
			// 
			// button2
			// 
			this->button2->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->button2->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->button2->Location = System::Drawing::Point(16, 8);
			this->button2->Name = S"button2";
			this->button2->Size = System::Drawing::Size(32, 24);
			this->button2->TabIndex = 8;
			this->button2->Text = S"▐◄";
			this->button2->Click += new System::EventHandler(this, button2_Click);
			// 
			// bntLastPage
			// 
			this->bntLastPage->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->bntLastPage->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->bntLastPage->Location = System::Drawing::Point(136, 8);
			this->bntLastPage->Name = S"bntLastPage";
			this->bntLastPage->Size = System::Drawing::Size(32, 24);
			this->bntLastPage->TabIndex = 7;
			this->bntLastPage->Text = S"►▌";
			this->bntLastPage->Click += new System::EventHandler(this, bntLastPage_Click);
			// 
			// btnPrevPage
			// 
			this->btnPrevPage->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnPrevPage->Font = new System::Drawing::Font(S"Arial", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->btnPrevPage->Location = System::Drawing::Point(48, 8);
			this->btnPrevPage->Name = S"btnPrevPage";
			this->btnPrevPage->Size = System::Drawing::Size(32, 24);
			this->btnPrevPage->TabIndex = 6;
			this->btnPrevPage->Text = S"◄";
			this->btnPrevPage->Click += new System::EventHandler(this, btnPrevPage_Click);
			// 
			// pnlButtons
			// 
			this->pnlButtons->Controls->Add(this->chbxWrapParams);
			this->pnlButtons->Controls->Add(this->btnClose);
			this->pnlButtons->Controls->Add(this->btnPrint);
			this->pnlButtons->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlButtons->Location = System::Drawing::Point(0, 0);
			this->pnlButtons->Name = S"pnlButtons";
			this->pnlButtons->Size = System::Drawing::Size(320, 40);
			this->pnlButtons->TabIndex = 4;
			// 
			// chbxWrapParams
			// 
			this->chbxWrapParams->Checked = true;
			this->chbxWrapParams->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chbxWrapParams->Location = System::Drawing::Point(224, 5);
			this->chbxWrapParams->Name = S"chbxWrapParams";
			this->chbxWrapParams->Size = System::Drawing::Size(96, 32);
			this->chbxWrapParams->TabIndex = 3;
			this->chbxWrapParams->Text = S"Wrap text in param column";
			this->chbxWrapParams->CheckedChanged += new System::EventHandler(this, chbxWrapParams_CheckedChanged);
			// 
			// btnClose
			// 
			this->btnClose->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnClose->Location = System::Drawing::Point(120, 8);
			this->btnClose->Name = S"btnClose";
			this->btnClose->Size = System::Drawing::Size(96, 24);
			this->btnClose->TabIndex = 2;
			this->btnClose->Text = S"Close";
			this->btnClose->Click += new System::EventHandler(this, btnClose_Click);
			// 
			// btnPrint
			// 
			this->btnPrint->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnPrint->Location = System::Drawing::Point(16, 8);
			this->btnPrint->Name = S"btnPrint";
			this->btnPrint->Size = System::Drawing::Size(96, 24);
			this->btnPrint->TabIndex = 1;
			this->btnPrint->Text = S"Print";
			this->btnPrint->Click += new System::EventHandler(this, btnPrint_Click);
			// 
			// pnlColumnWidths
			// 
			this->pnlColumnWidths->BackColor = System::Drawing::SystemColors::ControlLight;
			this->pnlColumnWidths->Controls->Add(this->lblAdjustInfo);
			this->pnlColumnWidths->Controls->Add(this->pnlBlackRt);
			this->pnlColumnWidths->Controls->Add(this->pnlBlackLt);
			this->pnlColumnWidths->Controls->Add(this->pnlBlack);
			this->pnlColumnWidths->Controls->Add(this->lvColumnSizer);
			this->pnlColumnWidths->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlColumnWidths->Location = System::Drawing::Point(0, 40);
			this->pnlColumnWidths->Name = S"pnlColumnWidths";
			this->pnlColumnWidths->Size = System::Drawing::Size(814, 56);
			this->pnlColumnWidths->TabIndex = 3;
			// 
			// lblAdjustInfo
			// 
			this->lblAdjustInfo->BackColor = System::Drawing::Color::Black;
			this->lblAdjustInfo->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->lblAdjustInfo->ForeColor = System::Drawing::Color::White;
			this->lblAdjustInfo->Location = System::Drawing::Point(281, 0);
			this->lblAdjustInfo->Name = S"lblAdjustInfo";
			this->lblAdjustInfo->Size = System::Drawing::Size(250, 16);
			this->lblAdjustInfo->TabIndex = 4;
			this->lblAdjustInfo->Text = S"Adjust column widths and click here to apply";
			this->lblAdjustInfo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->lblAdjustInfo->Click += new System::EventHandler(this, lblAdjustInfo_Click);
			// 
			// pnlBlackRt
			// 
			this->pnlBlackRt->BackColor = System::Drawing::Color::Black;
			this->pnlBlackRt->Location = System::Drawing::Point(804, 0);
			this->pnlBlackRt->Name = S"pnlBlackRt";
			this->pnlBlackRt->Size = System::Drawing::Size(4, 16);
			this->pnlBlackRt->TabIndex = 3;
			// 
			// pnlBlackLt
			// 
			this->pnlBlackLt->BackColor = System::Drawing::Color::Black;
			this->pnlBlackLt->Location = System::Drawing::Point(8, 0);
			this->pnlBlackLt->Name = S"pnlBlackLt";
			this->pnlBlackLt->Size = System::Drawing::Size(4, 16);
			this->pnlBlackLt->TabIndex = 2;
			// 
			// pnlBlack
			// 
			this->pnlBlack->BackColor = System::Drawing::Color::Black;
			this->pnlBlack->Location = System::Drawing::Point(8, 8);
			this->pnlBlack->Name = S"pnlBlack";
			this->pnlBlack->Size = System::Drawing::Size(796, 2);
			this->pnlBlack->TabIndex = 1;
			// 
			// lvColumnSizer
			// 
			this->lvColumnSizer->BackColor = System::Drawing::SystemColors::ControlLight;
			this->lvColumnSizer->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->lvColumnSizer->Location = System::Drawing::Point(8, 16);
			this->lvColumnSizer->Name = S"lvColumnSizer";
			this->lvColumnSizer->Size = System::Drawing::Size(800, 34);
			this->lvColumnSizer->TabIndex = 0;
			this->lvColumnSizer->View = System::Windows::Forms::View::Details;
			// 
			// pnlPreviewControl
			// 
			this->pnlPreviewControl->BackColor = System::Drawing::Color::FromArgb((System::Byte)255, (System::Byte)224, (System::Byte)192);
			this->pnlPreviewControl->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlPreviewControl->Location = System::Drawing::Point(0, 96);
			this->pnlPreviewControl->Name = S"pnlPreviewControl";
			this->pnlPreviewControl->Size = System::Drawing::Size(814, 436);
			this->pnlPreviewControl->TabIndex = 4;
			// 
			// PrintPreview
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(814, 532);
			this->Controls->Add(this->pnlPreviewControl);
			this->Controls->Add(this->pnlColumnWidths);
			this->Controls->Add(this->pnlControls);
			this->Name = S"PrintPreview";
			this->Text = S"Holodeck Print Preview";
			this->pnlControls->ResumeLayout(false);
			this->pnlCenter->ResumeLayout(false);
			this->pnlPageControl->ResumeLayout(false);
			this->pnlButtons->ResumeLayout(false);
			this->pnlColumnWidths->ResumeLayout(false);
			this->ResumeLayout(false);

		}		

		
		System::Void pnlPreviewControl_Paint(System::Object *  sender, System::Windows::Forms::PaintEventArgs *  e);
		System::Void btnNextPage_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void btnPrevPage_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void button2_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void bntLastPage_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void formResizeEventFunction(System::Object *  sender, System::EventArgs *  e);
		System::Void lnkGo_LinkClicked(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e);
		System::Void chbxWrapParams_CheckedChanged(System::Object *  sender, System::EventArgs *  e);
		System::Void lblAdjustInfo_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void btnPrint_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void btnClose_Click(System::Object *  sender, System::EventArgs *  e);
		System::Void cmbZoom_SelectedIndexChanged(System::Object *  sender, System::EventArgs *  e);
		System::Void refreshPercentTable();
};
}
