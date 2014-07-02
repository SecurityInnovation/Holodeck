//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDetailsPane.h
//
// DESCRIPTION: Contains definition for the class FileCorruptionDetailsPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 11 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#include "DockablePane.h"
#include <stdio.h>
#include "SavedFileCorruptionDataNode.h"
#include "FileCorruptionChangesParser.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace GuiSupport;

namespace HolodeckGui
{
	//*************************************************************************
	// class to hold the contents of the file corruption details
	//*************************************************************************
	public __gc class FileCorruptionDetailsPane : public DockablePane
	{
	public:
		FileCorruptionDetailsPane();
		~FileCorruptionDetailsPane();

		void showContents(String *location);

		__property SavedFileCorruptionDataNode *get_CurrentDataNode() { return fcDataNode; }

	private:
		__delegate void DisplayDetailsFileDelegate(String *file);
		DisplayDetailsFileDelegate *displayDetailsFileHandler;

		RichTextBox *richTextBox;
		FocusChangedDelegate *FocusChangedHandler;
		CloseProjectUIDelegate *CloseProjectUIHandler;
		Label *label;
		SavedFileCorruptionDataNode *fcDataNode;
		String *fileToLoadInThread;
		Threading::Thread *loadDetailsThread;

		void LoadDetailsFileThreadFunc();
		void onFocusChanged(DisplayableDataNode *dataNode);
		void onCloseProjectUI();
		void onDisplayDetailsFile(String *file);
	};
}