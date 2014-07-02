//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LogTreeListView.h
//
// DESCRIPTION: Contains definition for the class LogTreeListView
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Nov 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <SIControls.dll>

#include <stdio.h>
#include "TimeStampFilterForm.h"
#include "GenericListFilterForm.h"
#include "FunctionsFilterForm.h"
#include "ParamFilterForm.h"

using namespace SIControls;
using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	public __delegate void LogFilterUpdateDelegate();

	//*********************************************************************
	// defines the log panes' TreeListView control
	//*********************************************************************
	public __gc class LogTreeListView : public SIControls::FilterTreeListView
	{
	private:
		ArrayList *allColumns;
		ToggleColumnHeader *timeStampHeader;
		ToggleColumnHeader *threadIDHeader;
		ToggleColumnHeader *categoryHeader;
		ToggleColumnHeader *dllHeader;
		ToggleColumnHeader *functionHeader;
		ToggleColumnHeader *returnValueHeader;
		ToggleColumnHeader *errorCodeHeader;
		ToggleColumnHeader *exceptionHeader;
		ToggleColumnHeader *parameterHeaders[];

		FunctionDatabase::InterceptedFunctionDB * interceptedFunctionDB;

		EventHandler *Filter_OKClick;

	public:
		LogTreeListView();
		~LogTreeListView();
		void CleanUp();

		[Browsable(false)]
		__property ArrayList *get_AllColumns() { return allColumns; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_TimeStampHeader() { return timeStampHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_ThreadIDHeader() { return threadIDHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_CategoryHeader() { return categoryHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_DllHeader() { return dllHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_FunctionHeader() { return functionHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_ReturnValueHeader() { return returnValueHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_ErrorCodeHeader() { return errorCodeHeader; }
		[Browsable(false)]
		__property ToggleColumnHeader *get_ExceptionHeader() { return exceptionHeader; }
		
		ToggleColumnHeader *GetParameterHeader(int paramIndex) { return parameterHeaders[paramIndex]; }

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

		LogFilterUpdateDelegate * OnFilterUpdate;
	};
}