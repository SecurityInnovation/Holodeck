//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		PrintPreview.cpp
//
// DESCRIPTION: Contains implementation for the class PrintPreview
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 22 Apr 2004		 A. Kakrania	File created.
//*************************************************************************

#include "PrintPreview.h"

namespace HolodeckGui
{
	PrintPreview::PrintPreview(Printing::PrintDocument *pd, String * docTitle, ArrayList * LogColumns, System::Drawing::Icon *icon)
	{
		this->columnWidthPercents = new Hashtable;
		this->Icon = icon;
		this->logColumns = LogColumns;
		
		InitializeComponent();			
		this->Resize += new EventHandler (this, formResizeEventFunction);
		this->cmbZoom->SelectedIndex = 0;
		
		ppvControl = new PrintPreviewControl();
		ppvControl->Dock = DockStyle::Fill;
		this->pnlPreviewControl->Controls->Add (ppvControl);

		int totalWidth = 0;
		bool paramColumnsAdded = false;
		for (int i=0; i < logColumns->Count; i++)
		{
			SIControls::ToggleColumnHeader* tch = dynamic_cast <SIControls::ToggleColumnHeader*> (this->logColumns->Item[i]);

			if (tch->Text->StartsWith ("Param"))
			{
				if (paramColumnsAdded)
					continue;
				else
					paramColumnsAdded = true;
			}

			totalWidth += tch->Width;
		}

		paramColumnsAdded = false;
		for (int i=0; i < logColumns->Count; i++)
		{
			SIControls::ToggleColumnHeader* tch = dynamic_cast <SIControls::ToggleColumnHeader*> (this->logColumns->Item[i]);
			ColumnHeader * ch = new ColumnHeader();
			ch->Text = tch->Text;
			ch->Width = ((double) tch->Width/totalWidth)  * (double)(this->lvColumnSizer->Width);

			if (tch->Text->StartsWith ("Param"))
			{
				if (paramColumnsAdded)
					continue;
				else
				{
					ch->Text = "Parameters";
					paramColumnsAdded = true;
				}
			}
			
			this->lvColumnSizer->Columns->Add (ch);
		}

		this->refreshPercentTable();

		ppvControl->Document = pd;
		ppvControl->Document->DocumentName = docTitle;
	}

	System::Void PrintPreview::pnlPreviewControl_Paint(System::Object *  sender, System::Windows::Forms::PaintEventArgs *  e)
	{
	}

	System::Void PrintPreview::btnNextPage_Click(System::Object *  sender, System::EventArgs *  e)
	{
		this->ppvControl->StartPage++;
		this->tbPageNumber->Text = (this->ppvControl->StartPage + 1).ToString();
	}

	System::Void PrintPreview::btnPrevPage_Click(System::Object *  sender, System::EventArgs *  e)
	{
		this->ppvControl->StartPage--;
		this->tbPageNumber->Text = (this->ppvControl->StartPage + 1).ToString();
	}

	System::Void PrintPreview::button2_Click(System::Object *  sender, System::EventArgs *  e)
	{
		this->ppvControl->StartPage = 0;
		this->tbPageNumber->Text = (this->ppvControl->StartPage + 1).ToString();
	}

	System::Void PrintPreview::bntLastPage_Click(System::Object *  sender, System::EventArgs *  e)
	{
		this->ppvControl->StartPage = this->maxPages - 1;
		this->tbPageNumber->Text = (this->ppvControl->StartPage + 1).ToString();
	}

	System::Void PrintPreview::formResizeEventFunction(System::Object *  sender, System::EventArgs *  e)
	{
		this->lvColumnSizer->Left = (this->pnlColumnWidths->Width /2) - (this->lvColumnSizer->Width /2);
		this->pnlBlackLt->Left = this->lvColumnSizer->Left;
		this->pnlBlack->Left = this->pnlBlackLt->Left + this->pnlBlackLt->Width;
		this->pnlBlackRt->Left = this->pnlBlack->Left + this->pnlBlack->Width - 4;
		this->lblAdjustInfo->Left = this->pnlBlack->Left + (this->pnlBlack->Width /2) - (this->lblAdjustInfo->Width /2);
	}

	int PrintPreview::GetColumnWidthPercent(String * columnHeaderText)
	{
		if (!this->columnWidthPercents->ContainsKey (columnHeaderText))
			return -1;

		return *dynamic_cast<__box int *> (this->columnWidthPercents->Item[columnHeaderText]);
	}

	void PrintPreview::refreshPercentTable()
	{
		this->columnWidthPercents->Clear();
		for (int i=0; i < this->lvColumnSizer->Columns->Count; i++)
		{
			ColumnHeader * ch = this->lvColumnSizer->Columns->Item[i];
			this->columnWidthPercents->Add (ch->Text, __box((int) (((double) (ch->Width * 100)) / this->lvColumnSizer->Width)));
		}
	}

	System::Void PrintPreview::lnkGo_LinkClicked(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e)
	{
		try
		{
			for (int i=0; i < this->tbPageNumber->Text->Length; i++)
				if (!Char::IsNumber (this->tbPageNumber->Text->Chars[i]))
					return;
				
			int reqPage = Int32::Parse(this->tbPageNumber->Text) - 1;
			if (reqPage >= 0 && reqPage <= this->maxPages)
			{
				this->ppvControl->StartPage = reqPage;
				this->tbPageNumber->Text = (this->ppvControl->StartPage + 1).ToString();
			}
		}
		catch(...)
		{
		}
	}

	System::Void PrintPreview::lblAdjustInfo_Click(System::Object *  sender, System::EventArgs *  e)
	{
		this->refreshPercentTable();
		ppvControl->InvalidatePreview();
	}

	System::Void PrintPreview::chbxWrapParams_CheckedChanged(System::Object *  sender, System::EventArgs *  e)
	{
		this->refreshPercentTable();
		ppvControl->InvalidatePreview();
	}

	System::Void PrintPreview::btnPrint_Click(System::Object *  sender, System::EventArgs *  e)
	{
		try
		{
			if (printDialog == NULL)
				printDialog = new PrintDialog();

			printDialog->AllowSomePages = true;
			printDialog->Document = this->ppvControl->Document;
			
			
			if (UIEventRegistry::getInstance()->OnShowCommonDialog(printDialog) == DialogResult::OK)
			{
				printDialog->Document->Print();
			}
		}
		catch(...)
		{
			// do nothing
		}
	}

	System::Void PrintPreview::btnClose_Click(System::Object *  sender, System::EventArgs *  e)
	{
		this->Close();
	}

	System::Void PrintPreview::cmbZoom_SelectedIndexChanged(System::Object *  sender, System::EventArgs *  e)
	{
		if (!this->ppvControl)
			return;

		if (String::Compare (this->cmbZoom->Text->Trim()->ToLower(), "auto") == 0)
		{
			this->ppvControl->AutoZoom = true;
		}
		else
		{
			double zoomPercent = Double::Parse (this->cmbZoom->Text->Replace ("%", ""));
			this->ppvControl->AutoZoom = false;
			this->ppvControl->Zoom = (double) zoomPercent / 100;
		}
	}
}