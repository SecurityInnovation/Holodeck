//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TimeStampFilterForm.h
//
// DESCRIPTION: Contains definition for the class TimeStampFilterForm
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
	// Class:		TimeStampFilterForm
	// Description: Form Class which provides an interface for the Time Stamp filter
	//*************************************************************************
	public __gc class TimeStampFilterForm: public System::Windows::Forms::Form
	{
	public:
		TimeStampFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler);
		~TimeStampFilterForm();

		__property String * get_RangeTo() {return dtTo;}
		__property String * get_RangeFrom() {return dtFrom;}
		__property bool get_ShowRange() {return this->rbInclude->Checked;}

		__property bool get_FilterEnabled() {return filterEnabled;}
		__property void set_FilterEnabled(bool value) {filterEnabled = value;}

	private:
		String * dtFrom;
		String * dtTo;
		bool filterEnabled;

		GroupBox * grpRange;
		NumericUpDown * udMilliSFrom;
		NumericUpDown * udSecondFrom;
		NumericUpDown * udMinuteFrom;
		NumericUpDown * udHourFrom;
		Label * lblMilliSFrom;
		Label * lblSecondFrom;
		Label * lblMinuteFrom;
		Label * lblHourFrom;
		DateTimePicker * dtpFrom;
		Label * lblFrom;
		Label * lblTo;
		NumericUpDown * udMilliSTo;
		NumericUpDown * udSecondTo;
		NumericUpDown * udMinuteTo;
		NumericUpDown * udHourTo;
		Label * lblMilliSTo;
		Label * lblSecondTo;
		Label * lblMinuteTo;
		Label * lblHourTo;
		DateTimePicker * dtpTo;
		LinkLabel * lnkNowFrom;
		LinkLabel * lnkNowTo;
		CheckBox * chbxEnableFilter;
		RadioButton * rbInclude;
		RadioButton * rbExclude;
		Button * btnOK;
		Button * btnCancel;

		void InitializeUI();
		void OKClick_Func (System::Object * sender, EventArgs * e);
		void CancelClick_Func(System::Object * sender, EventArgs * e);

		void lnkNowFrom_LinkClicked(Object * sender, LinkLabelLinkClickedEventArgs * e);
		void lnkNowTo_LinkClicked(Object * sender, LinkLabelLinkClickedEventArgs * e);
		void fieldValueChangedEventHandlerFunc(System::Object * sender, EventArgs * e);
		void TimeStampFilterForm::fieldTextValidating(System::Object *sender, CancelEventArgs *e);

		DateTime ValidateTimeStamp(String *timeStamp);
		
		String * getDtFromString();
		String * getDtToString();
		void MakeToFieldsCurrent();
		void MakeFromFieldsCurrent();
	};
}