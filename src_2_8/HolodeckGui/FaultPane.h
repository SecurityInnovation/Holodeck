//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// Disk:		FaultPane.h
//
// DESCRIPTION: Contains definition for the class FaultPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 24 Mar 2003		 J. Taylor	 Disk created.
//*************************************************************************
#pragma once

#include "Pane.h"
#include "FaultPaneDataArray.h"
#include "Defines.h"
#include "UserNotification.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents a fault pane
	//*************************************************************************
	public __gc class FaultPane : public Pane
	{
	public:
		__property unsigned int get_ThreadID() { return threadID; }
		__property unsigned int get_ProcessID() { return dataArray->ProcessID; }
		[Browsable(false)]
		__property FaultPaneDataArray *get_DataArray() { return dataArray; }

		FaultPane(FaultPaneDataArray *data, unsigned int threadID);
		~FaultPane();
		void Dispose(bool disposing);

	private:
		bool perThread;
		unsigned int threadID;
		ItemDeletedDelegate *deletedDelegate;
		ItemAddedDelegate *addedDelegate;

		FaultPaneDataArray *dataArray;

		Panel *panel;
		GroupBox *memoryBox;
		GroupBox *diskBox;
		GroupBox *networkBox;
		GroupBox *registryBox;
		GroupBox *processBox;
		GroupBox *customBox;

		void InitializeUI();
		void onItemDeleted(DisplayableDataNode *dataNode);
		void onItemAdded(DisplayableDataNode *dataNode);
		void onCheckedChanged(Object *sender, EventArgs *e);
		void LayoutChanged(Object *sender, LayoutEventArgs *e);
		RadioButton *FindRadioButton(DisplayableDataNode *dataNode);
		void helpLinkClicked(Object *sender, LinkLabelLinkClickedEventArgs *args);
	};
}