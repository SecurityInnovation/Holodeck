//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogCompareDialog.h
//
// DESCRIPTION: Contains definition for the class LogCompareDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "LogPaneDataArray.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace UIData;

namespace HolodeckGui
{
	//*************************************************************************
	// represents the dialog to setup log comparison
	//*************************************************************************
	public __gc class LogCompareDialog : public Form
	{
	public:
		LogCompareDialog(ArrayList *dataArrayList);
		~LogCompareDialog();

		__property LogPaneDataArray *get_FirstDataArrayToCompare() { return dynamic_cast<LogPaneDataArray *>(dataArrayList->get_Item(pane1ComboBox->SelectedIndex)); }
		__property LogPaneDataArray *get_SecondDataArrayToCompare() { return dynamic_cast<LogPaneDataArray *>(dataArrayList->get_Item(pane2ComboBox->SelectedIndex)); }

	protected:
		ComboBox *pane1ComboBox;
		ComboBox *pane2ComboBox;

		ArrayList *dataArrayList;
	};
}