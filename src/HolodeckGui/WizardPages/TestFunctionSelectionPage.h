//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestFunctionSelectionPage.h
//
// DESCRIPTION: Contains definition for the class TestFunctionSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>
#using <CustomTestProjectGenerator.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	__delegate void FunctionSelectedDelegate(bool valid);

	//*************************************************************************
	// class that represents the function selection page of the create test
	// wizard
	//*************************************************************************
	public __gc class TestFunctionSelectionPage : public WizardPage
	{
	public:
		TestFunctionSelectionPage(unsigned int threadID, bool hideAW);
		~TestFunctionSelectionPage();

		FunctionSelectedDelegate *OnFunctionSelected;
		String* selectedValue;

		__property String *get_Function();
		__property void set_Function(String *value);

		__property bool get_IsDotNetFunction() { return dotNetFunction; }

		__property bool get_IsHiddenFunction() { return isHiddenFunction; }

	protected:
		CustomTestProjectGenerator::FunctionTreeView *functionTreeView;
		Label *functionValue;
		Label *categoryValue;
		Label *returnTypeValue;
		Label *dllValue;
		Label *numParametersValue;
		bool dotNetFunction;
		bool isHiddenFunction;

		void onFunctionClicked(String* functionName);
	};
}