//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestSummaryPage.h
//
// DESCRIPTION: Contains definition for the class TestSummaryPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include "TestFiringSpecificationPage.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <FunctionDatabase.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	__delegate void ModifyFunctionValueDelegate();
	__delegate void ModifyInParamsDelegate();
	__delegate void ModifyOutParamsDelegate();
	__delegate void ModifyReturnValueDelegate();
	__delegate void ModifyErrorCodeDelegate();
	__delegate void ModifyFiringSpecDelegate();

	//*************************************************************************
	// class that represents the summary/modification page of the create test
	// wizard
	//*************************************************************************
	public __gc class TestSummaryPage : public WizardPage
	{
	public:
		TestSummaryPage();
		~TestSummaryPage();

		ModifyInParamsDelegate *OnModifyInParams;
		ModifyOutParamsDelegate *OnModifyOutParams;
		ModifyReturnValueDelegate *OnModifyReturnValue;
		ModifyErrorCodeDelegate *OnModifyErrorCode;
		ModifyFiringSpecDelegate *OnModifyFiringSpec;

		__property String *get_Function() { return function; }
		__property void set_Function(String *value) { function = value; }

		__property ArrayList *get_InParameters() { return inParameters; }
		__property void set_InParameters(ArrayList *value) { inParameters = value; }

		__property FiringDetails *get_FiringSpecification() { return firingSpecification; }
		__property void set_FiringSpecification(FiringDetails *value) { firingSpecification = value; }

		__property ArrayList *get_OutParameters() { return outParameters; }
		__property void set_OutParameters(ArrayList *value) { outParameters = value; }

		__property String *get_ReturnValue() { return returnValue; }
		__property void set_ReturnValue(String *value) { returnValue = value; }

		__property bool get_AllowErrorCodePage() { return allowErrorCodePage; }
		__property void set_AllowErrorCodePage(bool value) { allowErrorCodePage = value; }

		__property String *get_ErrorCode() { return errorCode; }
		__property void set_ErrorCode(String *value) { errorCode = value; }

		void MakeSummaryPage();
		void onDoubleClick(Object *sender, EventArgs *args);

	protected:
		ListView *summaryListView;
		ListViewItem *functionListViewItem;
		ListViewItem *inParamsListViewItem;
		ListViewItem *outParamsListViewItem;
		ListViewItem *returnValueListViewItem;
		ListViewItem *errorCodeListViewItem;
		ListViewItem *firingSpecListViewItem;

		String *function;
		ArrayList *inParameters;
		ArrayList *outParameters;
		String *returnValue;
		bool allowErrorCodePage;
		String *errorCode;
		FiringDetails * firingSpecification;
	};
}