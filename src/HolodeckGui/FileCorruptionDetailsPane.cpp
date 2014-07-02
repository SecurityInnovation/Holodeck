//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDetailsPane.cpp
//
// DESCRIPTION: Contains implementation for the FileCorruptionDetailsPane class
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#include <windows.h>
#include <vcclr.h>
#include "FileCorruptionDetailsPane.h"

using namespace System::Text;

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FileCorruptionDetailsPane
	// Description: Constructor for the FileCorruptionDetailsPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDetailsPane::FileCorruptionDetailsPane()
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
		label->Text = "Select a file in the Fuzzed Files pane to see the details here";
		label->Location = System::Drawing::Point(25,25);
		label->AutoSize = true;
		label->Visible = true;

		displayDetailsFileHandler = new DisplayDetailsFileDelegate(this, onDisplayDetailsFile);

		Controls->Add(label);
		Controls->Add(richTextBox);

		FocusChangedHandler = new FocusChangedDelegate(this, onFocusChanged);
		uiEventRegistry->OnFocusChanged += FocusChangedHandler;
		CloseProjectUIHandler = new CloseProjectUIDelegate(this, onCloseProjectUI);
		uiEventRegistry->OnCloseProjectUI += CloseProjectUIHandler;
	}

	//*************************************************************************
	// Method:		~FileCorruptionDetailsPane
	// Description: Destructor for the FileCorruptionDetailsPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDetailsPane::~FileCorruptionDetailsPane()
	{
		uiEventRegistry->OnCloseProjectUI -= CloseProjectUIHandler;
		uiEventRegistry->OnFocusChanged -= FocusChangedHandler;

		if (loadDetailsThread && loadDetailsThread->IsAlive)
			loadDetailsThread->Abort();
	}

	//*************************************************************************
	// Method:		showContents
	// Description: Display the contents at location in the pane
	//
	// Parameters:
	//	location - the location of the contents to display
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDetailsPane::showContents(String *location)
	{
		try
		{
			const __wchar_t __pin *corruptDetailsFileName = PtrToStringChars(location);

			PSECURITY_DESCRIPTOR pSD;
			SECURITY_ATTRIBUTES sa;

			pSD = (PSECURITY_DESCRIPTOR) LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
			InitializeSecurityDescriptor(pSD, SECURITY_DESCRIPTOR_REVISION);
			SetSecurityDescriptorDacl(pSD, TRUE, NULL, TRUE);
			// Initialize a security attributes structure.
			sa.nLength = sizeof (SECURITY_ATTRIBUTES);
			sa.lpSecurityDescriptor = pSD;
			sa.bInheritHandle = FALSE;
			HANDLE detailsFile = CreateFileW(corruptDetailsFileName, GENERIC_READ, FILE_SHARE_READ, &sa, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
			if (detailsFile == INVALID_HANDLE_VALUE)
				return;

			unsigned long filesize = GetFileSize(detailsFile, NULL);
			if (filesize > MAX_CORRUPT_DETAILS_FILE_SIZE_TO_DISPLAY)
			{
				char *detailsData = new char[MAX_CORRUPT_DETAILS_FILE_SIZE_TO_DISPLAY + 1];
				if (!detailsData)
					return;

				// get the original data into a buffer
				unsigned long totalNumRead = 0;
				unsigned long numRead = 0;
				unsigned long totalRemaining = MAX_CORRUPT_DETAILS_FILE_SIZE_TO_DISPLAY;
				while (totalNumRead < MAX_CORRUPT_DETAILS_FILE_SIZE_TO_DISPLAY)
				{
					ReadFile(detailsFile, detailsData + totalNumRead, totalRemaining * sizeof(char), &numRead, NULL);
					totalNumRead += numRead;
					totalRemaining -= numRead;
				}
				detailsData[MAX_CORRUPT_DETAILS_FILE_SIZE_TO_DISPLAY] = '\0';

				StringBuilder *rtfText = new StringBuilder(MAX_CORRUPT_DETAILS_FILE_SIZE_TO_DISPLAY);
				String *detailsString = new String(detailsData);

				rtfText->Append(detailsString);
				String *fileNameForDisplay = location->Replace("\\", "\\\\");
				fileNameForDisplay = fileNameForDisplay->Replace("{", "\\{");
				fileNameForDisplay = fileNameForDisplay->Replace("}", "\\}");
				String *truncString = String::Concat("\\par\n\\par\nThis file has been truncated for display here.\\par\nLoad '", fileNameForDisplay, "'\\par\nin an external viewer to view the rest of the file.\\cf0\\par\n}");
				
				rtfText->Append(truncString);
				richTextBox->Rtf = rtfText->ToString();

				CloseHandle(detailsFile);
				delete[] detailsData;
			}
			else
			{
				CloseHandle(detailsFile);
				richTextBox->LoadFile(location);
				richTextBox->Font = new System::Drawing::Font("Courier New", 8.0);
			}

			label->Visible = false;
			richTextBox->Visible = true;
		}
		catch(...)
		{
		}

		fcDataNode = NULL;
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
	void FileCorruptionDetailsPane::onFocusChanged(DisplayableDataNode *dataNode)
	{
		if (!dataNode)
		{
			richTextBox->Text = S"";
			return;
		}

		if ((dataNode->Type == UIData::CorruptedFile) || (dataNode->Type == UIData::SavedCorruptedFile))
		{
			// Show the details for the selected corrupt file
			fcDataNode = dynamic_cast<SavedFileCorruptionDataNode*>(dataNode);
			if (fcDataNode)
			{
				if ((fcDataNode->DetailsFile != NULL) ||
					(!File::Exists(fcDataNode->DetailsFile)))
				{
					String* detailsPath = Path::ChangeExtension(fcDataNode->ChangesFile, ".rtf");

					richTextBox->Visible = false;
					label->Text = String::Concat("Please wait while the file \"", detailsPath, "\" is being loaded.");
					label->Visible = true;

					if (loadDetailsThread && loadDetailsThread->IsAlive)
						loadDetailsThread->Abort();

					fileToLoadInThread = detailsPath;
					
					Threading::ThreadStart *threadStart = new Threading::ThreadStart(this, LoadDetailsFileThreadFunc);
					loadDetailsThread = new Threading::Thread(threadStart);
					loadDetailsThread->Start();
				}
			}
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
	void FileCorruptionDetailsPane::onCloseProjectUI()
	{
		fcDataNode = NULL;

		if (richTextBox->Visible)
		{
			richTextBox->Visible = false;
			label->Text = "Select a file in the Fuzzed Files pane to see the details here";
			label->Visible = true;
		}

		if (loadDetailsThread && loadDetailsThread->IsAlive)
			loadDetailsThread->Abort();
	}

	//*************************************************************************
	// Method:		LoadDetailsFileThreadFunc
	// Description: thread function to parse and load the details file
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void FileCorruptionDetailsPane::LoadDetailsFileThreadFunc()
	{
		if (fcDataNode && fileToLoadInThread)
		{
			FileCorruptionChangesParser* parser = new FileCorruptionChangesParser();

			if (parser->Load(fcDataNode->ChangesFile) &&
				parser->SaveRTFDetails(fileToLoadInThread))
			{
				fcDataNode->DetailsFile = fileToLoadInThread;

				Object *args[] = new Object *[1];
				args[0] = fcDataNode->DetailsFile;

				showContents(fcDataNode->DetailsFile);
			}
		}
		fileToLoadInThread = NULL;
	}

	//*************************************************************************
	// Method:		onDisplayDetailsFile
	// Description: called to display the details file in the richTextBox
	//
	// Parameters:
	//	file - the file to display
	//
	// Return Value: none
	//*************************************************************************
	void FileCorruptionDetailsPane::onDisplayDetailsFile(String *file)
	{
		showContents(file);
	}
}