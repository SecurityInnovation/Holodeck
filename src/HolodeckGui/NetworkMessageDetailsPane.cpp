//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkMessageDetailsPane.cpp
//
// DESCRIPTION: Contains implementation for the NetworkMessageDetailsPane class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Feb 2004		 B. Shirey	 File created.
//*************************************************************************
#include "NetworkMessageDetailsPane.h"


namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkMessageDetailsPane
	// Description: Constructor for the NetworkMessageDetailsPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkMessageDetailsPane::NetworkMessageDetailsPane() : currentData(NULL),
		currentDataSize(0), currentOriginalData(NULL), currentOriginalDataSize(0)
	{
		richTextBox = new RichTextBox();
		richTextBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
		richTextBox->DetectUrls = false;
		richTextBox->Multiline = true;
		richTextBox->ReadOnly = true;
		richTextBox->ScrollBars = System::Windows::Forms::RichTextBoxScrollBars::Both;
		richTextBox->Dock = DockStyle::Fill;
		richTextBox->Text = S"";
		richTextBox->WordWrap = false;
		richTextBox->Font = new System::Drawing::Font("Courier New", 8.0);

		label = new Label();
		label->Text = "Select a packet in the Network Log pane to see the details here";
		label->Location = System::Drawing::Point(25,25);
		label->AutoSize = true;

		Controls->Add(label);

		FocusChangedHandler = new FocusChangedDelegate(this, onFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;
		CloseProjectUIHandler = new CloseProjectUIDelegate(this, onCloseProjectUI);
		uiEventRegistry->OnCloseProjectUI += CloseProjectUIHandler;
	}

	//*************************************************************************
	// Method:		~NetworkMessageDetailsPane
	// Description: Destructor for the NetworkMessageDetailsPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkMessageDetailsPane::~NetworkMessageDetailsPane()
	{
		if (currentData)
		{
			delete[] currentData;
			currentData = NULL;
			currentDataSize = 0;
		}

		if (currentOriginalData)
		{
			delete[] currentOriginalData;
			currentOriginalData = NULL;
			currentOriginalDataSize = 0;
		}

		uiEventRegistry->OnCloseProjectUI -= CloseProjectUIHandler;
		uiEventRegistry->OnFocusChanged -= FocusChangedHandler;
	}

	//*************************************************************************
	// Method:		showContents
	// Description: Display the contents of the network log with focus in the pane
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkMessageDetailsPane::showContents()
	{
	}

	//*************************************************************************
	// Method:		OnFocusChanged
	// Description: Called when focus changes in any ui pane
	//
	// Parameters:
	//	dataNode - datanode representing focus change
	//
	// Return Value: none
	//*************************************************************************
	void NetworkMessageDetailsPane::onFocusChanged(DisplayableDataNode *dataNode)
	{
		int totalRows = 0;
		this->richTextBox->Text = "";

		if (!dataNode)
			return;

		bool updateData = false;
		NetworkLogItem *currentLogItem = NULL;

		if (dataNode->Type == UIData::NetworkLogs)
		{
			NetworkLogPaneDataNode *nlDataNode = dynamic_cast<NetworkLogPaneDataNode *>(dataNode);
			currentDataNode = nlDataNode;
			if (nlDataNode)
			{
				currentLogItem = nlDataNode->LogEntry;
				updateData = true;
			}
		}
		// also update from the log pane if the item has network logs
		else if (dataNode->Type == UIData::Logs)
		{
			LogPaneDataNode *lpDataNode = dynamic_cast<LogPaneDataNode *>(dataNode);
			currentDataNode = lpDataNode;
			if (lpDataNode)
			{
				LogItem *logItem = lpDataNode->LogEntry;
				if (logItem)
				{
					ArrayList *netLogItems = logItem->NetworkLogItems;
					if (netLogItems && (netLogItems->Count > 0))
					{
						currentLogItem = dynamic_cast<NetworkLogItem *>(netLogItems->get_Item(0));
					}
					updateData = true;
				}
			}
		}

		if (updateData)
		{
			if (this->Controls->Contains(label))
			{
				this->Controls->Remove(label);
				this->Controls->Add(richTextBox);
			}

			if (currentData)
			{
				delete[] currentData;
				currentData = NULL;
				currentDataSize = 0;
			}

			if (currentOriginalData)
			{
				delete[] currentOriginalData;
				currentOriginalData = NULL;
				currentOriginalDataSize = 0;
			}

			if (currentLogItem)
			{
				currentDataSize = currentLogItem->ActualDataLength;
				currentData = new unsigned char[currentDataSize];
				currentOriginalDataSize = currentLogItem->ActualOriginalDataLength;
				currentOriginalData = new unsigned char[currentOriginalDataSize];
				unsigned int sizeGot;
				unsigned int sizeGotOriginal;
				currentLogItem->GetData(currentData, sizeGot);
				currentLogItem->GetOriginalData (currentOriginalData, sizeGotOriginal);

				unsigned int numRows = sizeGot / 16;
				unsigned int numRowsOriginal = sizeGotOriginal / 16;

				// add 1 if not full row
				if ((sizeGot % 16) != 0)
					numRows++;

				// add 1 if not full row
				if ((sizeGotOriginal % 16) != 0)
					numRowsOriginal++;

				totalRows = (numRows > numRowsOriginal) ? numRows : numRowsOriginal;
			}
			else
			{
				totalRows = 0;
			}
		}

		StringBuilder* rtfText = new StringBuilder();
		//write header information
		rtfText->Append("{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang1033{\\fonttbl{\\f0\\fnil Courier New;}"
			"{\\f1\\fnil\\fcharset0 Courier New;}}{\\colortbl ;\\red0\\green0\\blue0;\\red0\\green0\\blue255;"
			"\\red255\\green0\\blue0;}\\viewkind4\\uc1\\pard\\b\\f0\\fs16 Offset         "
			"Modified Hex Data                                      Modified ASCII Data                 "
			"Original Hex Data                                      Original ASCII Data\\par\n\\cf1\\b0 ");
		for (int i=0; i < totalRows; i++)
			AddDataLine(rtfText, i);
		rtfText->Append("\\cf0\\par\n}\n");

		richTextBox->Rtf = rtfText->ToString();
	}

	//*************************************************************************
	// Method:		AddDataLine
	// Description: called to add the specified line to the richtextbox
	//
	// Parameters:
	//	rtfText - the RTF formatted text to output to
	//	index - the row number to add the data for
	//
	// Return Value: None
	//*************************************************************************
	void NetworkMessageDetailsPane::AddDataLine(StringBuilder* rtfText, unsigned int index)
	{
		if (!currentData || !currentOriginalData)
			return;

		unsigned int startingOffset = index << 4;
		char offset[12];
		sprintf(offset, "0x%08x", startingOffset);

		String *hexBytes = "";
		String *asciiBytes = "";
		String *originalHexBytes = "";
		String *originalAsciiBytes = "";

		char thisByte[3];
		char originalThisByte[3];

		for (unsigned int i = startingOffset; i < startingOffset + 16; i++)
		{
			if (i >= currentDataSize && i >= currentOriginalDataSize)
				break;

			if (i == (startingOffset + 8))
			{
				hexBytes = String::Concat(hexBytes, "  ");
				originalHexBytes = String::Concat(originalHexBytes, "  ");
			}

			
			unsigned char value;
			unsigned char originalValue;
			
			if (i >= currentDataSize)
			{
				value = ' ';
				hexBytes = String::Concat(hexBytes, "  ");
				asciiBytes = String::Concat(asciiBytes, " ");

			}
			else
			{
				value = currentData[i];
				sprintf(thisByte, "%02x", value);
				hexBytes = String::Concat(hexBytes, thisByte);
				
				if ((value > 32) && (value < 127))
					asciiBytes = String::Concat(asciiBytes, new String((wchar_t)value, 1));
				else
					asciiBytes = String::Concat(asciiBytes, ".");
			}

			if (i >= currentOriginalDataSize)
			{
				originalValue = ' ';
				originalHexBytes = String::Concat(originalHexBytes, "  ");
				originalAsciiBytes = String::Concat(originalAsciiBytes, " ");

			}
			else
			{
				originalValue = currentOriginalData[i];
				sprintf(originalThisByte, "%02x", originalValue);
				originalHexBytes = String::Concat(originalHexBytes, originalThisByte);

				if ((originalValue > 32) && (originalValue < 127))
					originalAsciiBytes = String::Concat(originalAsciiBytes, new String((wchar_t)originalValue, 1));
				else
					originalAsciiBytes = String::Concat(originalAsciiBytes, ".");
			}

			if (i < (startingOffset + 15))
			{
				hexBytes = String::Concat(hexBytes, " ");
				asciiBytes = String::Concat(asciiBytes, " ");

				originalHexBytes = String::Concat(originalHexBytes, " ");
				originalAsciiBytes = String::Concat(originalAsciiBytes, " ");
			}
		}

		AddDataSegment (rtfText, offset, offset, Color::Black, 10);
		rtfText->Append ("     ");
		AddDataSegment (rtfText, hexBytes, originalHexBytes, Color::Blue, 50);
		rtfText->Append ("     ");
		AddDataSegment (rtfText, asciiBytes, originalAsciiBytes, Color::Blue, 31);
		rtfText->Append ("     ");
		AddDataSegment (rtfText, originalHexBytes, hexBytes, Color::Red, 50);
		rtfText->Append ("     ");
		AddDataSegment (rtfText, originalAsciiBytes, asciiBytes, Color::Red, 31);
		rtfText->Append ("\\par\n");
	}

	//*************************************************************************
	// Method:		AddDataSegment
	// Description: Adds rich text data to the richtextcontrol for a specified
	//				line segment
	//
	// Parameters:
	//	rtfText       - the RTF formatted text to output to
	//	displayString - the string to display in the richtextbox
	//	compareString - the string to compare the displayString with for diff
	//	highlightColor- the color to use for highlighting differences
	//
	// Return Value: None
	//*************************************************************************
	void NetworkMessageDetailsPane::AddDataSegment(StringBuilder* rtfText, String * displayString, String * compareString, Color highlightColor, int length)
	{
		for (int i=0; i < displayString->Length && i < compareString->Length; i++)
		{
			if (displayString->Chars[i] != compareString->Chars[i])
			{
				if (highlightColor == Color::Blue)
					rtfText->Append("\\cf2 ");
				else if (highlightColor == Color::Red)
					rtfText->Append("\\cf3 ");
			}

			if (displayString->Chars[i] == '\\')
				rtfText->Append("\\\\");
			else if (displayString->Chars[i] == '{')
				rtfText->Append("\\{");
			else if (displayString->Chars[i] == '}')
				rtfText->Append("\\}");
			else
				rtfText->Append(displayString->Chars[i].ToString());

			if (displayString->Chars[i] != compareString->Chars[i])
				rtfText->Append("\\cf1 ");
		}

		if (displayString->Length < length)
		{
			for (int j=0; j < length-displayString->Length; j++)
				rtfText->Append(" ");
		}
	}
	
	//*************************************************************************
	// Method:		onCloseProjectUI
	// Description: called when project is closing
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void NetworkMessageDetailsPane::onCloseProjectUI()
	{
		currentDataNode = NULL;
		if (this->Controls->Contains(richTextBox))
		{
			this->Controls->Remove(richTextBox);
			this->Controls->Add(label);
		}
	}
}