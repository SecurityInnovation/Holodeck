//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ParamFilterForm.h
//
// DESCRIPTION: Contains definition for the class ParamFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#pragma once

#using <System.dll>
#using <mscorlib.dll>
#using <GuiSupport.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include <stdio.h>
#include "defines.h"

using namespace System;
using namespace System::Drawing;
using namespace FunctionDatabase;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

namespace HolodeckGui
{
	//*************************************************************************
	// Class:		ParamFilterForm
	// Description: Form Class which provides an interface for the Parameters filter
	//
	//*************************************************************************
	public __gc class ParamFilterForm: public System::Windows::Forms::Form
	{
	public:
		ParamFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler);
		~ParamFilterForm();

		__property ListView::ListViewItemCollection * get_Filters() {return lvFilters->Items;}

		__property bool get_FilterEnabled() {return filterEnabled;}
		__property void set_FilterEnabled(bool value) {filterEnabled = value;}

	private:
		bool filterEnabled;
		ColumnHeader * MatchString;
		ColumnHeader * MatchType;
		ColumnHeader * ParamPos;
		ListView * lvFilters;
		ComboBox * cmbBox;
		Label * lblInfo;
		String * subItemText;
		Point firstMouseClickPoint;
		ListViewItem * lvi;
		int subItemSelected;
		Button * btnAddFilter;
		Button * btnDeleteFilter;
		Button * btnOK;
		Button * btnCancel;
		CheckBox * chbxEnableFilter;
		Button * btnMoveUp;
		Button * btnMoveDown;

		void InitializeUI();
		void btnAddFilter_Click (System::Object * sender, EventArgs * e);
		void btnDeleteFilter_Click (System::Object * sender, EventArgs * e);
		void btnMoveUp_Click (System::Object * sender, EventArgs * e);
		void btnMoveDown_Click (System::Object * sender, EventArgs * e);
		void lvFilters_DoubleClick(Object * sender, System::EventArgs * e);
		void lvFilters_MouseDown(Object * sender, System::Windows::Forms::MouseEventArgs * e);
		void lvFilters_CmbKeyPress(Object * sender , System::Windows::Forms::KeyPressEventArgs * e);
		void lvFilters_CmbSelected(Object * sender , System::EventArgs * e);
		void lvFilters_CmbFocusOver(Object * sender , System::EventArgs * e);
		void OKClick_Func (System::Object * sender, EventArgs * e);
		void CancelClick_Func(System::Object * sender, EventArgs * e);
	};
}