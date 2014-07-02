//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogListView.h
//
// DESCRIPTION: Contains definition for the class LogListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include <stdio.h>
#include "FilterListView.h"
#include "TimeStampFilterForm.h"
#include "GenericListFilterForm.h"
#include "FunctionsFilterForm.h"
#include "ParamFilterForm.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	public __delegate void FilterUpdateDelegate();

	//*********************************************************************
	// defines the log panes' list view control
	//*********************************************************************
	public __gc class LogListView : public HolodeckGui::FilterListView
	{
	private:
		ArrayList *allColumns;
		ColumnHeader *timeStampHeader;
		ColumnHeader *threadIDHeader;
		ColumnHeader *categoryHeader;
		ColumnHeader *dllHeader;
		ColumnHeader *functionHeader;
		ColumnHeader *returnValueHeader;
		ColumnHeader *errorCodeHeader;
		ColumnHeader *exceptionHeader;
		ColumnHeader *parameterHeaders[];

		FunctionDatabase::InterceptedFunctionDB * interceptedFunctionDB;

		EventHandler *Filter_OKClick;

	public:
		LogListView();
		~LogListView();

		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }
		[Browsable(false)]
		__property ColumnHeader *get_TimeStampHeader() { return timeStampHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_ThreadIDHeader() { return threadIDHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_CategoryHeader() { return categoryHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_DllHeader() { return dllHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_FunctionHeader() { return functionHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_ReturnValueHeader() { return returnValueHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_ErrorCodeHeader() { return errorCodeHeader; }
		[Browsable(false)]
		__property ColumnHeader *get_ExceptionHeader() { return exceptionHeader; }
		
		ColumnHeader *GetParameterHeader(int paramIndex) { return parameterHeaders[paramIndex]; }

		void customfilter_TimeStamp(System::Object * sender, EventArgs * e);
		void customfilter_ThreadID(System::Object * sender, EventArgs * e);
		void customfilter_Category(System::Object * sender, EventArgs * e);
		void customfilter_DLL(System::Object * sender, EventArgs * e);
		void customfilter_Functions(System::Object * sender, EventArgs * e);
		void customfilter_ReturnValue(System::Object * sender, EventArgs * e);
		void customfilter_ErrorCode(System::Object * sender, EventArgs * e);
		void customfilter_Exception(System::Object * sender, EventArgs * e);
		void customfilter_Params(System::Object * sender, EventArgs * e);
		
		void filter_OKClick(System::Object * sender, EventArgs * e);
		void customfilter_ThreadID_RefreshValues(System::Object * sender, EventArgs * e);
		void customfilter_ReturnValue_RefreshValues(System::Object * sender, EventArgs * e);
		void customfilter_ErrorCode_RefreshValues(System::Object * sender, EventArgs *);
		void customfilter_Exception_RefreshValues(System::Object * sender, EventArgs * e);

		TimeStampFilterForm * timeStampFilterForm;
		GenericListFilterForm * threadIDFilterForm;
		GenericListFilterForm * categoryFilterForm;
		GenericListFilterForm * dllFilterForm;
		FunctionsFilterForm * functionsFilterForm;
		GenericListFilterForm * returnValueFilterForm;
		GenericListFilterForm * errorCodeFilterForm;
		GenericListFilterForm * exceptionFilterForm;
		ParamFilterForm * paramFilterForm;

		FilterUpdateDelegate * OnFilterUpdate;
	};
}