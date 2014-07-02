//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkLogFindNextDialog.cpp
//
// DESCRIPTION: Contains implementation for the class NetworkLogFindNextDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 19 Feb 2004		 B. Shirey	 File created.
//*************************************************************************
#include "SiString.h"
#include "NetworkLogFindNextDialog.h"
#include "ImageListManager.h"
#include "UserNotification.h"

using namespace SiUtils;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NetworkLogFindNextDialog
	// Description: Constructor for the NetworkLogFindNextDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NetworkLogFindNextDialog::NetworkLogFindNextDialog()
	{
		InitializeComponent();

		this->Icon = ImageListManager::getInstance()->AppIcon;
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Cleans up the object
	//
	// Parameters:
	//	disposing - true if the object is being disposed, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::Dispose(Boolean disposing)
	{
		if (logPane && logPane->PaneContainerListView)
		{
			logPane->PaneContainerListView->FullHighlightOnHideSelection = false;
			logPane->PaneContainerListView->Invalidate();
		}

		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}

	//*************************************************************************
	// Method:		set_CurrentLogPane
	// Description: Sets the pane to search
	//
	// Parameters:
	//	value - the pane to search
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::set_CurrentLogPane(NetworkLogPane *value)
	{
		if (SearchThread && SearchThread->IsAlive)
			SearchThread->Abort();

		logPane = value;
	}

	//*************************************************************************
	// Method:		SearchNext
	// Description: Starts the search thread
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::SearchNext()
	{
		if (this->logPane != NULL)
		{
			this->cancelButton->Text = "Stop";
			this->findNextButton->Enabled = false;
			
			if (SearchThread && SearchThread->IsAlive)
				SearchThread->Abort();

			SearchThread = new Threading::Thread(new Threading::ThreadStart(this, SearchThreadFunc));
			SearchThread->IsBackground = true;
			SearchThread->Start();
		}
	}

	//*************************************************************************
	// Method:		SearchThreadFunc
	// Description: Function which runs in the search thread. Performs
	//				the search and calls method to select item.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::SearchThreadFunc ()
	{
		bool IsSearchDown = this->rbSrchDn->Checked;

		//Start the search from the item after or before the currently selected item
		this->entryFound = false;
		unsigned int selectedIndex = logPane->SelectedListViewIndex;
		unsigned int startIndex;
		
		//if no entry was selected and selectedIndex was -1, then reset it to 0;
		//this way the search will stop when it comes back to index 0
		if (selectedIndex == -1)
			selectedIndex = 0;

		if (IsSearchDown)
		{
			startIndex = selectedIndex + 1;
			if (startIndex >= (unsigned int) logPane->LogCount)
				startIndex =0;
		}
		else
		{
			if (selectedIndex == 0)
				startIndex = (unsigned int) logPane->LogCount - 1;
			else
				startIndex = selectedIndex - 1;
		}

		//Cache values for the search loop
		bool threadIDCheckBoxChecked = this->threadIDCheckBox->Checked;
		bool destinationPortCheckBoxChecked = this->destinationPortCheckBox->Checked;
		bool protocolCheckBoxChecked = this->protocolCheckBox->Checked;
		bool sourceIPCheckBoxChecked = this->sourceIPCheckBox->Checked;
		bool directionCheckBoxChecked = this->directionCheckBox->Checked;
		bool dataCheckBoxChecked = this->dataCheckBox->Checked;
		bool destinationIPCheckBoxChecked = this->destinationIPCheckBox->Checked;
		bool sourcePortCheckBoxChecked = this->sourcePortCheckBox->Checked;
		bool searchAsciiRadioButtonChecked = this->searchAsciiRadioButton->Checked;
		bool searchHexRadioButtonChecked = this->searchHexRadioButton->Checked;
		bool wholeWordCheckBoxChecked = this->wholeWordCheckBox->Checked;
		bool caseSensitiveCheckBoxChecked = this->caseSensitiveCheckBox->Checked;
		String * searchForTextBoxText = this->searchForTextBox->Text;

		// search till we cycle back to the selected entry or find the required value
		for (unsigned int i = startIndex; (i != selectedIndex) && (!this->entryFound) && logPane->LogCount > 0;)
		{
			if (i == selectedIndex)
				continue;

			NetworkLogItem * logItem = logPane->get_LogItem(i);
			
			if (logItem)
			{
				if (threadIDCheckBoxChecked && logItem->ThreadID && MatchString(logItem->ThreadID, searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
				{
					SelectFoundEntry (i);
					continue;
				}
				if (directionCheckBoxChecked)
				{
					if (logItem->IsIncomingMessage && MatchString(INCOMING_MESSAGE_TEXT, searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
					{
						SelectFoundEntry(i);
						continue;
					}
					else if (!logItem->IsIncomingMessage && MatchString(OUTGOING_MESSAGE_TEXT, searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
					{
						SelectFoundEntry(i);
						continue;
					}
				}
				if (protocolCheckBoxChecked && logItem->ProtocolString && MatchString(logItem->ProtocolString, searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
				{
					SelectFoundEntry(i);
					continue;
				}
				if (destinationIPCheckBoxChecked && logItem->DestinationIPAddressString && MatchString(logItem->DestinationIPAddressString, searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
				{
					SelectFoundEntry(i);
					continue;
				}
				if (destinationPortCheckBoxChecked && MatchString(logItem->DestinationPort.ToString(), searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
				{
					SelectFoundEntry(i);
					continue;
				}
				if (sourceIPCheckBoxChecked && logItem->SourceIPAddressString && MatchString(logItem->SourceIPAddressString, searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
				{
					SelectFoundEntry(i);
					continue;
				}
				if (sourcePortCheckBoxChecked && MatchString(logItem->SourcePort.ToString(), searchForTextBoxText, wholeWordCheckBoxChecked, caseSensitiveCheckBoxChecked))
				{
					SelectFoundEntry(i);
					continue;
				}
				if (dataCheckBoxChecked)
				{
					// add room for the null terminator.  we need it so we can use strstr below safely
					unsigned char *data = new unsigned char[logItem->ActualDataLength + 1];
					if (data)
					{
						unsigned int sizeGot;
						logItem->GetData(data, sizeGot);

						String *textBoxStr = this->searchForTextBox->Text;
						unsigned char *searchStr = NULL;

						if (searchAsciiRadioButtonChecked)
						{
							searchStr = new unsigned char[textBoxStr->Length + 1];
							for (int pos = 0; pos < (int)textBoxStr->Length; pos++)
							{
								searchStr[pos] = Convert::ToByte(textBoxStr->get_Chars(pos));
							}
							searchStr[pos] = '\0';
						}
						else if (searchHexRadioButtonChecked)
						{
							// make sure the string has an even # of chars
							if (textBoxStr->Length % 2 != 0)
								textBoxStr = String::Concat("0", textBoxStr);

							searchStr = new unsigned char[(textBoxStr->Length / 2) + 1];

							for (int pos = 0; pos < (int)textBoxStr->Length; pos += 2)
							{
								unsigned char thisHexByte[3];
								thisHexByte[0] = Convert::ToByte(textBoxStr->get_Chars(pos));
								thisHexByte[1] = Convert::ToByte(textBoxStr->get_Chars(pos+1));
								thisHexByte[2] = '\0';

								searchStr[pos / 2] = (unsigned char)strtol((const char *)thisHexByte, NULL, 16);
							}
							searchStr[pos / 2] = '\0';
						}

						char *pos = strstr((const char *)data, (const char *)searchStr);
						if (pos != NULL)
						{
							int byteOffsetInDataPacket = (unsigned char *)pos - data;

							if (data)
							{
								delete[] data;
								data = NULL;
							}

							if (searchStr)
							{
								delete[] searchStr;
								searchStr = NULL;
							}

							SelectFoundEntry(i);
							
							continue;
						}
					}
				}
			}

			//This must remain outside the 'for statement' otherwise it will be executed when 'continue' is called

			//increment or decrement the counter
			if (IsSearchDown)
			{
				i++;
				if (i >= (unsigned int) logPane->LogCount)
					i = 0;
			}
			else
			{
				if (i == 0)
					i = (unsigned int) logPane->LogCount - 1;
				else
					i--;
			}
		}

		if (!this->entryFound)
		{
			UserNotification::InfoNotify("Could not find search string");
			this->searchForTextBox->SelectAll();
			this->searchForTextBox->Focus();
		}
		
		this->cancelButton->Text = "Cancel";
		this->findNextButton->Enabled = true;
	}

	//*************************************************************************
	// Method:		MatchString
	// Description: Matches two strings based on the Match Whole word and Case-
	//				-sensitivitive options selected in the dialog
	//
	// Parameters:
	//	baseString - the string to be compared against
	//	matchString - the string to compare
	//
	// Return Value: true if match, false otherwise
	//*************************************************************************
	bool NetworkLogFindNextDialog::MatchString (String *baseString, String *matchString, bool matchWhole, bool caseSensitive)
	{
		if (matchWhole)
		{
			if (String::Compare(baseString, matchString, !caseSensitive) == 0)
				return true;
			else
				return false;
		}
		else
		{
			if (caseSensitive)
			{
				if (baseString->IndexOf (matchString) != -1)
					return true;
				else
					return false;
			}
			else
			{
				if (baseString->ToLower()->IndexOf (matchString->ToLower()) != -1)
					return true;
				else
					return false;
			}

		}
	}

	//*************************************************************************
	// Method:		SelectFoundEntry
	// Description: Selects the given entry in the listview
	//
	// Parameters:
	//	entry	- the entry to select
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::SelectFoundEntry (unsigned int entry)
	{
		logPane->SelectedListViewIndex = entry;
		this->entryFound = true;		
	}

	//*************************************************************************
	// Method:		Show
	// Description: Shows the form
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::Show()
	{
		if (logPane && logPane->PaneContainerListView)
		{
			logPane->PaneContainerListView->FullHighlightOnHideSelection = true;
			logPane->PaneContainerListView->Invalidate();
		}

		__super::Show();
	}

	//*************************************************************************
	// Method:		Hide
	// Description: Hides the form
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void NetworkLogFindNextDialog::Hide()
	{
		if (logPane && logPane->PaneContainerListView)
		{
			logPane->PaneContainerListView->FullHighlightOnHideSelection = false;
			logPane->PaneContainerListView->Invalidate();
		}

		__super::Hide();
	}
}