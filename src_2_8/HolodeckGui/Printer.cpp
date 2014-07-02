//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		Printer.cpp
//
// DESCRIPTION: Contains implementation for the class Printer
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 21 Mar 2003		 A. Kakrania	File created.
//*************************************************************************
#include "printer.h"
#include "ImageListManager.h"
#include "UserNotification.h"

namespace HolodeckGui
{

	//*********************************************************
	// Method:		Printer (constructor)
	// Description: Creates a new PrintDocument and sets the PageStyle
	//				Initializes other arrays and print objects
	//
	// Parameters: (none)
	//
	// Return Value: (none)
	//*********************************************************
	Printer::Printer()
	{
		fontSize = 8;

		TextLeftMargin = 7;
		TextTopMargin = 37;
		textLeftSpacing = 0;

		rightMargin = 50;
		leftMargin = 5;
		bottomMargin = 50;
		topMargin = 15;
		headerHeight = 20;

		maximumCharsAllowed = 100;

		pd = new PrintDocument ();
		
		pd->DefaultPageSettings->Landscape = true;
		pd->add_PrintPage (new Printing::PrintPageEventHandler (this, PrintPageHandler));

		PrintJobs = new Collections::ArrayList ();
		backBuffer = new Collections::ArrayList ();

		regFont = new Font ("Courier New", fontSize);
		blackPen = new Pen (Color::Black);

		this->totalRegularColumnWidth = 0;

		logItemsCompleted = 0;
		currentlySelectedNode = NULL;
		limitWarned = false;
	};

	//*********************************************************
	// Method:		ShowPreview
	// Description: Brings up the Page preview dialog box
	//
	// Parameters: (none)
	//
	// Return Value: (none)
	//*********************************************************
	void Printer::ShowPreview()
	{
		pd->PrinterSettings->PrintRange = Printing::PrintRange::AllPages;
		
		//check if a printer is installed
		if (pd->PrinterSettings->InstalledPrinters->Count == 0)
		{
			UserNotification::ErrorNotify("There are no printers installed on this computer. Cannot perform preview operation");
			return;
		}
		
		logpane = dynamic_cast <LogPane*> (PrintJobs->Item[0]);
		net_logpane = dynamic_cast <NetworkLogPane*> (PrintJobs->Item[0]);

		if (logpane)
		{
			this->logTitle = logpane->PrintTitle;
			this->logVisibleColumns = logpane->VisibleColumns;
			this->logCount = logpane->LogCount;
			this->logHeaderCollection = logpane->ColumnHeaders;
		}
		else if(net_logpane)
		{
			this->logTitle = net_logpane->Title;
			this->logVisibleColumns = net_logpane->VisibleColumns;
			this->logCount = net_logpane->LogCount;
			this->logHeaderCollection = net_logpane->ColumnHeaders;
		}
		else
			return;

		this->logVisibleColumnIndicies = new ArrayList();
		for (int cpy=0; cpy < this->logVisibleColumns->Count; cpy++)
		{
			ToggleColumnHeader * header = dynamic_cast <ToggleColumnHeader*> (this->logVisibleColumns->Item[cpy]);
			logVisibleColumnIndicies->Add (header->Index.ToString());
		}

		if (dlg == NULL)
			dlg = new PrintPreview(pd, this->logTitle, this->logVisibleColumns, ImageListManager::getInstance()->AppIcon);

		if (net_logpane)
			dlg->HideWrapOption = true;
		
		UIEventRegistry::getInstance()->OnShowDialog(dlg);
	};

	//***************************************************************************************
	// Method:		PrintPageHandler
	// Description: PrintPageEvent handler function, determines the current page and whether it
	//				is within the (if) specified print range. Calls PrintPage to perform print.
	//
	// Parameters:
	//	obj		- the sender object
	//	evargs	- PrintPageEvent arguments
	//
	// Return Value: (none)
	//***************************************************************************************
	void Printer::PrintPageHandler (System::Object * obj, Printing::PrintPageEventArgs * evargs)
	{
		try
		{
			int maxPage = 0;
			do
			{
				currentPage++;
				if (currentPage >= 301)
				{
					if (!limitWarned)
					{
						UserNotification::InfoNotify ("You can only preview and print 300 pages at a time. To preview and print the next set of logs, apply a time filter on the log and select Print");
						limitWarned = true;
					}

					//Reset counters and nodes
					logItemsCompleted = 0;
					currentlySelectedNode = NULL;
					currentPage = 0;
					
					//Clear the backbuffer
					if (backBuffer)
						backBuffer->Clear();

					break;
				}

				dlg->MaxPages = currentPage;
				//Print this page if this page is within the range
				if (evargs->PageSettings->PrinterSettings->PrintRange == Printing::PrintRange::SomePages)
				{
					maxPage = evargs->PageSettings->PrinterSettings->ToPage;

					if ((currentPage >= evargs->PageSettings->PrinterSettings->FromPage) &&
						(currentPage <= evargs->PageSettings->PrinterSettings->ToPage))
						PageInRange = true;
					else
						PageInRange = false;
				}
				else
					PageInRange = true;
			}
			while ((PrintPage (obj, evargs)) && (currentPage < maxPage));

			if ((evargs->PageSettings->PrinterSettings->PrintRange == Printing::PrintRange::SomePages) && (currentPage >= maxPage))
				evargs->HasMorePages = false;
		}
		catch (OutOfMemoryException *)
		{
			UserNotification::ErrorNotify (String::Concat ("The system ran out of memory while trying to generate print information. You can print the ", currentPage.ToString()," pages that have already been generated.\n\nTo print more of the log, please use a log filter to limit the number of pages."));
			evargs->HasMorePages = false;
		}
		catch (Exception *)
		{
			UserNotification::ErrorNotify ("An unhandled error has occurred while printing. The preview window will now be closed.");
			dlg->Close();
		}
	}


	//***************************************************************************************
	// Method:		PrintPage
	// Description: Does the actual printing. If the global variable PageInRange is true then it
	//				draws on the GDI+ object, otherwise nothing is drawn on page
	//
	// Parameters:
	//	obj		- the sender object
	//	evargs	- PrintPageEvent arguments
	//
	// Return Value: (bool) If the current page was printed.
	//***************************************************************************************
	bool Printer::PrintPage (System::Object * obj, Printing::PrintPageEventArgs * evargs)
	{
		currentPgLineCount = 0;
		maxLinesOnPage = (evargs->PageSettings->Bounds.Height - topMargin - bottomMargin - 10)/ (int) regFont->Size / 2 ;

		Graphics * graphics = evargs->Graphics;
		
		//put the title on the page
		if (PageInRange)
			graphics->DrawString (String::Concat (this->logTitle, new String("   Page:"), currentPage.ToString()) , regFont, Drawing::Brushes::Black, 5, 1);

		//draw the margins and column headers
		if (PageInRange)
			DrawMarginsAndHeaders (graphics, evargs->PageBounds , dlg->ColumnHeaders);

		//take care of any back buffer | note: only parameters are backbuffered
		int paramWidth = this->GetColumnWidth(evargs->PageBounds.Width, "Parameters");
		int regualarColWidth = this->totalRegularColumnWidth;
		for (int bItem = 0; bItem < backBuffer->Count; bItem++)
			PrintLogLine (graphics, backBuffer->Item[bItem]->ToString(), regualarColWidth + textLeftSpacing, TextTopMargin + (currentPgLineCount * (int) regFont->Size * 2), paramWidth);
		backBuffer->Clear();

		//continue printing text
		int textStartPt = 0;
		int widthAllowed = 0;
		evargs->HasMorePages = false;

		for (unsigned int cItem = logItemsCompleted; true; cItem++)
		{
			if (evargs->HasMorePages == true)
				break;

			if (cItem >= this->logCount && net_logpane)
				break;

			ListViewItem * line = this->GetLogListViewItem(cItem);

			if (!line)
			{
				evargs->HasMorePages = false;
				break;
			}

			textStartPt = TextLeftMargin;
			
			//Print the non-param column values
			for (int cSubItem=0; cSubItem < line->SubItems->Count; cSubItem++)
			{
				if (cSubItem >= logHeaderCollection->Count)
					continue;

				if (!logVisibleColumnIndicies->Contains (cSubItem.ToString()))
					continue;

				if (!logHeaderCollection->Item[cSubItem]->Text->ToString()->ToLower()->StartsWith ("param"))
				{
					widthAllowed = this->GetColumnWidth (evargs->PageBounds.Width, logHeaderCollection->Item[cSubItem]->Text);
					if (widthAllowed == 0) continue;
					
					PrintLogColumn (graphics, line->SubItems->Item[cSubItem]->Text->ToString(),
																textStartPt,
																TextTopMargin + (currentPgLineCount * (int) regFont->Size * 2),
																widthAllowed);
					
					textStartPt += widthAllowed;
				}
			}

			this->totalRegularColumnWidth = textStartPt;
			
			int paramsPrinted = 0;
			//print the param column values
			if (paramWidth != 0)
			{
				for (int cParamItem=0; cParamItem < line->SubItems->Count; cParamItem++)
				{
					if (cParamItem >= logHeaderCollection->Count)
						continue;

					if (!logVisibleColumnIndicies->Contains (cParamItem.ToString()))
						continue;

					if (logHeaderCollection->Item[cParamItem]->Text->ToString()->ToLower()->StartsWith ("param"))
					{
						paramsPrinted++;
						PrintLogLine (graphics, line->SubItems->Item[cParamItem]->Text->ToString(),
																	textStartPt,
																	TextTopMargin + (currentPgLineCount * (int) regFont->Size * 2),
																	paramWidth);
					}
				}

				if (paramsPrinted == 0)
					currentPgLineCount++;

			}
			else
				currentPgLineCount++;

			//increment lines
			logItemsCompleted++;

			if (currentPgLineCount + 1 >= maxLinesOnPage)
				evargs->HasMorePages = true;
		}

		//clean any text which may have gone outside the allocated boundary
		if (!rectangles || rectangles->Length == 0)
		{
			rectangles = new RectangleF[4];
			//top block
			//rectangles[0] = Drawing::RectangleF (0, 0, (float) evargs->PageBounds.Width + 1, (float) topMargin - 1);
			//left block
			rectangles[0] = Drawing::RectangleF (0, topMargin - 1, (float) leftMargin - 1, (float) (evargs->PageBounds.Height - bottomMargin + 2));
			//right block
			rectangles[1] = Drawing::RectangleF ((float) (evargs->PageBounds.Width - rightMargin + 6), topMargin - 1, (float) evargs->PageBounds.Width, (float) (evargs->PageBounds.Height - bottomMargin + 2));
			//bottom block
			rectangles[2] = Drawing::RectangleF (0 , (float) (evargs->PageBounds.Height - bottomMargin + 16), (float) evargs->PageBounds.Width + 1, (float) evargs->PageBounds.Height);
		}
		graphics->FillRectangles (Brushes::White, rectangles);

		//If no more pages are left, then reset counter
		if (evargs->HasMorePages == false)
		{
			logItemsCompleted = 0;
			pd->PrinterSettings->MaximumPage = currentPage;
			pd->PrinterSettings->MinimumPage = 0;
			currentPage = 0;
		}

		return !PageInRange;
	}

	
	//***************************************************************************************
	// Method:		PrintLogLine
	// Description: Prints a string on the specified coordinate and clips the string if
	//				it is longer than the width available. Increments line counter after
	//				printing every line. If text exceeds preset page boundary then text is
	//				added to a buffer which is printed on next page.
	//
	// Parameters:
	//	graphics		- the GDI+ drawing surface
	//	printText		- the text to print
	//	startX			- the x-coordinate of starting location
	//	startY			- the y-coordinate of starting location
	//	widthAvailable	- the width availabe for this string
	//
	// Return Value: (none)
	//***************************************************************************************
	void Printer::PrintLogLine(Graphics * graphics, String * printText, int startX, int startY, int widthAvailable)
	{
		//trim length to text to maximum allowed
		if (printText->Length > maximumCharsAllowed)
			printText = printText->Substring (0, maximumCharsAllowed);

		String * wrapText = "";
		if (currentPgLineCount + 1	>= maxLinesOnPage)
		{
			backBuffer->Add (printText);
		}
		else
		{
			if (PageInRange)
			{
				String * willBePrinted = StringToPrintSize (printText, (int) regFont->Size, widthAvailable);
				if (dlg->WrapParams && willBePrinted->Length < printText->Length)
					wrapText = printText->Substring (willBePrinted->Length);
				
				graphics->DrawString (willBePrinted, regFont, Drawing::Brushes::Black, (float) startX, (float) startY);
			}
		}

		currentPgLineCount++;		

		//print any lines for wraparound
		if (wrapText->Trim()->Length > 0)
			PrintLogLine (graphics, wrapText, startX, TextTopMargin + (currentPgLineCount * (int) regFont->Size * 2), widthAvailable);
	}


	
	//*************************************************************************
	// Method:		PrintLogColumn
	// Description: Prints a string on the specified coordinate and clips the string if it is longer
	//				than the width available. Does not increment line counter
	//
	// Parameters:
	//	graphics		- the GDI+ drawing surface
	//	printText		- the text to print
	//	startX			- the x-coordinate of starting location
	//	startY			- the y-coordinate of starting location
	//	widthAvailable	- the width availabe for this string
	//
	// Return Value: (none)
	//*************************************************************************
	void Printer::PrintLogColumn(Graphics * graphics, String * printText, int startX, int startY, int widthAvailable)
	{
		//trim length to text to maximum allowed
		if (printText->Length > maximumCharsAllowed)
			printText = printText->Substring (0, maximumCharsAllowed);

		if (PageInRange)
			graphics->DrawString (StringToPrintSize (printText, (int) regFont->Size, widthAvailable),
																regFont,
																Drawing::Brushes::Black,
																(float) startX,
																(float) startY);
	}

	
	//*************************************************************************
	// Method:		DrawMarginsAndHeaders
	// Description: Draws the margins and prints the header text for columns
	//
	// Parameters:
	//	graphics		- THE GDI+ drawing surface
	//	pageBounds		- the rectangle which describes the area of the printpage
	//	logColumns		- the ColumnHeaderCollection for the LogPane being printed
	//	paramColWidth	- [out] returns the width that the param column takes up, 0 if no param. cols present
	//	regualarColWidth- [out] returns the width that the non-param columns take up
	// Return Value: none
	//*************************************************************************
	void Printer::DrawMarginsAndHeaders(Graphics * graphics, Drawing::Rectangle pageBounds, ListView::ColumnHeaderCollection * logColumns)
	{
		int paramColWidth = 0, regualarColWidth = 0;

		Drawing::Font * headerFont = new Font ("Courier New", fontSize, FontStyle::Bold);

		//OUTSIDE MARGIN
		graphics->DrawRectangle (blackPen, leftMargin, topMargin,pageBounds.Width - rightMargin, pageBounds.Height - bottomMargin);
		
		//HEADER MARGIN
		graphics->DrawRectangle (blackPen, leftMargin, topMargin, pageBounds.Width - rightMargin, headerHeight);
		
		//COLUMN MARGINS
		//Get the number of parameter columns
		int paramColumns = 0;
		for (int i = 0; i< logColumns->Count; i++)
			if (logColumns->Item[i]->Text->ToString()->ToLower()->StartsWith ("param"))
				paramColumns++;

		//Draw the margins
		regualarColWidth = leftMargin;
		int itemWidth = 0;
		for (int c = 0; c < logColumns->Count; c++)
		{
			//Draw margin only for non-param columns
			if (!logColumns->Item[c]->Text->ToString()->ToLower()->StartsWith ("param"))
			{
				itemWidth = this->GetColumnWidth (pageBounds.Width, logColumns->Item[c]->Text);
				if (itemWidth == 0) continue;

				graphics->DrawLine (blackPen, regualarColWidth + itemWidth,
																topMargin,
																regualarColWidth + itemWidth,
																pageBounds.Height - bottomMargin + 15);

				Drawing::RectangleF textRect = Drawing::RectangleF ((float) regualarColWidth,
																(float) topMargin,
																(float) (regualarColWidth + itemWidth),
																(float) headerHeight);
				
				graphics->DrawString (StringToPrintSize (logColumns->Item[c]->Text->ToString(), (int) headerFont->Size, itemWidth),
																headerFont,
																Drawing::Brushes::Black,
																textRect,
																new StringFormat (StringFormatFlags::FitBlackBox));

				regualarColWidth += itemWidth;
			}

		}
		
		//Take care of Params column
		int paramWidth = this->GetColumnWidth(pageBounds.Width, "Parameters");
		if (paramWidth > 0)
		{
			//Print header text for parameters
			Drawing::RectangleF textRect = Drawing::RectangleF ((float) regualarColWidth,
																	(float) topMargin,
																	(float) paramWidth,
																	(float) headerHeight);

			graphics->DrawString (new String ("Parameters"), headerFont, Drawing::Brushes::Black, textRect);
		}

	}

	
	//*************************************************************************
	// Method:		StringToPrintSize
	// Description: Returns the part of the string which will fit in the specified width
	//
	// Parameters:
	//	printString	- the original string.
	//	fontSize	- the font size being used to print this string
	//	printWidth	- the width available for this string
	//
	// Return Value: (String) string which will fit in the specified width
	//*************************************************************************
	String * Printer::StringToPrintSize (String * printString, int fontSize, int printWidth)
	{
		printWidth += fontSize;	//add a character length
		
		if (printString->Length * fontSize > printWidth)
			return printString->Substring (0, (int) printWidth/fontSize - 1);
		else
			return printString;
	}

	//*************************************************************************
	// Method:		GetColumnWidth
	// Description: Returns the width for a column
	//
	// Parameters:
	//	totalPageWidth	- the total width of the page
	//	columnHeaderText- the header test
	//
	// Return Value: (int) column width
	//*************************************************************************
	int Printer::GetColumnWidth(int totalPageWidth, String * columnHeaderText)
	{
		totalPageWidth = totalPageWidth - leftMargin - rightMargin;
		int width = dlg->GetColumnWidthPercent (columnHeaderText);
		if (width == -1)
			return 0;

		return ((width * totalPageWidth) / 100);
	}

	//*************************************************************************
	// Method:		GetLogListViewItem
	// Description: Gets a list view item from the api or network logpane
	//
	// Parameters:
	//	index - the index of the item to get (ignored in case of api logpane)
	//
	// Return Value: ListViewItem containing log data, NULL if log end
	//*************************************************************************
	ListViewItem* Printer::GetLogListViewItem(int index)
	{
		try
		{
			ListViewItem * lvi = NULL;
			if (logpane)
			{
				TreeListView *lv = logpane->PaneTreeListView;
				
				//Start from child of root-node
				if (!currentlySelectedNode)
					currentlySelectedNode = dynamic_cast<TreeListNode *>(lv->RootNode->FirstChild());
				//traverse the tree
				else
				{
					TreeListNode *nextNode = NULL;
					
					nextNode = dynamic_cast<TreeListNode *>(currentlySelectedNode->FirstChild());
					if (!nextNode)
					{
						nextNode = dynamic_cast<TreeListNode *>(currentlySelectedNode->NextSibling());
						TreeListNode *parentNode = currentlySelectedNode;
						while (!nextNode)
						{
							parentNode = dynamic_cast<TreeListNode *>(parentNode->ParentNode());
							if (!parentNode)
								break;
							nextNode = dynamic_cast<TreeListNode *>(parentNode->NextSibling());
						}
					}

					currentlySelectedNode = nextNode;
				}

				if (currentlySelectedNode != NULL)
					lvi = currentlySelectedNode->GetListViewItem();
				else
					lvi = NULL;
			}
			else if (net_logpane)
				lvi = net_logpane->GetListViewItem[index];

			return lvi;
		}
		catch(...)
		{
		}
		return NULL;
	}
}