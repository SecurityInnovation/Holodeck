//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Printer.h
//
// DESCRIPTION: Contains definition for the class Printer
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 21 Mar 2003		 A. Kakrania	File created.
//*************************************************************************

#include <stdio.h>
#include "Logpane.h"
#include "NetworkLogpane.h"
#include "PrintPreview.h"

#using <System.dll>
#using <mscorlib.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Drawing::Printing;

namespace HolodeckGui
{

	public __gc class Printer
	{
	public:
		Printer();		
		void ShowPreview();

		Collections::ArrayList * PrintJobs;

	private:
		RectangleF rectangles[];

		LogPane * logpane;
		TreeListNode *currentlySelectedNode;
		NetworkLogPane * net_logpane;
		ColumnHeaderCollection * logHeaderCollection;
		String * logTitle;
		ArrayList * logVisibleColumns;
		ArrayList * logVisibleColumnIndicies;
		unsigned int logCount;

		int maximumCharsAllowed;

		int totalRegularColumnWidth;

		long logItemsCompleted;
		int currentPage;
		int currentPgLineCount;
		int maxLinesOnPage;
		
		int TextLeftMargin;
		int TextTopMargin;
		int textLeftSpacing;

		int rightMargin;
		int leftMargin;
		int bottomMargin;
		int topMargin;
		int headerHeight;

		float fontSize;

		bool PageInRange;
		bool limitWarned;

		ArrayList * backBuffer;
		Drawing::Font * regFont;
		Pen * blackPen;
		
		PrintDocument * pd;
		PrintPreview * dlg;
		PageSetupDialog * pgSetupDlg;
		PrintDialog	* printDialog;
		
		void DrawMarginsAndHeaders(Graphics * graphics, Drawing::Rectangle pageBounds, ListView::ColumnHeaderCollection * logColumns);
		void PrintLogLine(Graphics * graphics, String * printText, int startX, int startY, int widthAvailable);
		void PrintLogColumn(Graphics * graphics, String * printText, int startX, int startY, int widthAvailable);
		void PrintPageHandler (System::Object * obj, Printing::PrintPageEventArgs * evargs);
		bool PrintPage (System::Object * obj, Printing::PrintPageEventArgs * evargs);
		String * StringToPrintSize (String * printString, int fontSize, int printWidth);
		int GetColumnWidth(int totalPageWidth, String * columnHeaderText);
		ListViewItem* GetLogListViewItem(int index);
	};
}