//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkMessageDetailsPane.h
//
// DESCRIPTION: Contains definition for the class NetworkMessageDetailsPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 Feb 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "DockablePane.h"
#include <stdio.h>
#include "NetworkLogPaneDataNode.h"
#include "LogPaneDataNode.h"
#include "defines.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <SIControls.dll>

using namespace System;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace System::Text;
using namespace SIControls;

namespace HolodeckGui
{
	//*************************************************************************
	// class to hold the contents of the network message log details
	//*************************************************************************
	public __gc class NetworkMessageDetailsPane : public DockablePane
	{
	public:
		NetworkMessageDetailsPane();
		~NetworkMessageDetailsPane();

		void showContents();

		__property DisplayableDataNode *get_CurrentDataNode() { return currentDataNode; }

	private:
		RichTextBox *richTextBox;
		Label *label;
		DisplayableDataNode *currentDataNode;

		FocusChangedDelegate *FocusChangedHandler;
		CloseProjectUIDelegate *CloseProjectUIHandler;
		unsigned char *currentData;
		unsigned int currentDataSize;
		unsigned char *currentOriginalData;
		unsigned int currentOriginalDataSize;

		void onFocusChanged(DisplayableDataNode *dataNode);
		void onCloseProjectUI();
		void AddDataLine(StringBuilder* rtfText, unsigned int index);
		void AddDataSegment(StringBuilder* rtfText, String * displayString, String * compareString, Color highlightColor, int length);
	};
}