//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReportPane.h
//
// DESCRIPTION: Contains definition for the class ReportPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 8 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "Pane.h"
#include "ReportPaneDataArray.h"
#include "HTMLBrowser.h"
#include <stdio.h>

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace AxSHDocVw;
using namespace MSHTML;

namespace HolodeckGui
{
	//*************************************************************************
	// class to hold the contents of a report
	//*************************************************************************
	public __gc class ReportPane : public Pane
	{
	public:
		[Browsable(false)]
		__property ReportPaneDataArray *get_DataArray() { return dataArray; }

		ReportPane(ReportPaneDataArray *reportDataArray);
		~ReportPane();

		bool showContents(String *location);

	private:
		ReportPaneDataArray *dataArray;
		HTMLBrowser * htmlBrowser;
	};
}