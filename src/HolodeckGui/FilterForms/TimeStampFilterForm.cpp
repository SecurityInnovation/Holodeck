//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TimeStampFilterForm.cpp
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
#include "defines.h"
#include "TimeStampFilterForm.h"
#include "../ImageListManager.h"
#include "../UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TimeStampFilterForm
	// Description: Constructor for the TimeStampFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TimeStampFilterForm::TimeStampFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler)
	{
		InitializeUI();
		
		filterEnabled = false;
		dtFrom = NULL;
		dtTo = NULL;

		this->rbInclude->Checked = true;

		// add the internal button event handlers
		this->btnOK->Click += new EventHandler (this, OKClick_Func);
		this->btnCancel->Click += new EventHandler (this, CancelClick_Func);

		// add additional button event handlers if requested
		if (OK_Click_EventHandler)
			this->btnOK->Click += OK_Click_EventHandler;
		if (Cancel_Click_EventHandler)
			this->btnCancel->Click += Cancel_Click_EventHandler;

		MakeFromFieldsCurrent();
		MakeToFieldsCurrent();

		this->dtTo = getDtToString();
		this->dtFrom = getDtFromString();

		this->Icon = ImageListManager::getInstance()->AppIcon;
		
		// add Field Value changed handlers
		EventHandler * fieldValueChangedEventHandler = new EventHandler (this, fieldValueChangedEventHandlerFunc);
		this->udHourFrom->add_ValueChanged (fieldValueChangedEventHandler);
		this->udMinuteFrom->add_ValueChanged (fieldValueChangedEventHandler);
		this->udSecondFrom->add_ValueChanged (fieldValueChangedEventHandler);
		this->udMilliSFrom->add_ValueChanged (fieldValueChangedEventHandler);
		this->udHourTo->add_ValueChanged (fieldValueChangedEventHandler);
		this->udMinuteTo->add_ValueChanged (fieldValueChangedEventHandler);
		this->udSecondTo->add_ValueChanged (fieldValueChangedEventHandler);
		this->udMilliSTo->add_ValueChanged (fieldValueChangedEventHandler);
		this->dtpFrom->add_ValueChanged (fieldValueChangedEventHandler);
		this->dtpTo->add_ValueChanged (fieldValueChangedEventHandler);

		CancelEventHandler *textValidatingHandler = new CancelEventHandler(this, fieldTextValidating);
		this->udHourFrom->Validating += textValidatingHandler;
		this->udHourTo->Validating += textValidatingHandler;
		this->udMilliSFrom->Validating += textValidatingHandler;
		this->udMilliSTo->Validating += textValidatingHandler;
		this->udMinuteFrom->Validating += textValidatingHandler;
		this->udMinuteTo->Validating += textValidatingHandler;
		this->udSecondFrom->Validating += textValidatingHandler;
		this->udSecondTo->Validating += textValidatingHandler;
	}

	//*************************************************************************
	// Method:		~TimeStampFilterForm
	// Description: Destructor for the TimeStampFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TimeStampFilterForm::~TimeStampFilterForm()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: UI Elements creation and initialization
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::InitializeUI()
	{
		this->grpRange = new System::Windows::Forms::GroupBox();
		this->udMilliSFrom = new System::Windows::Forms::NumericUpDown();
		this->udSecondFrom = new System::Windows::Forms::NumericUpDown();
		this->udMinuteFrom = new System::Windows::Forms::NumericUpDown();
		this->udHourFrom = new System::Windows::Forms::NumericUpDown();
		this->lblMilliSFrom = new System::Windows::Forms::Label();
		this->lblSecondFrom = new System::Windows::Forms::Label();
		this->lblMinuteFrom = new System::Windows::Forms::Label();
		this->lblHourFrom = new System::Windows::Forms::Label();
		this->dtpFrom = new System::Windows::Forms::DateTimePicker();
		this->lblFrom = new System::Windows::Forms::Label();
		this->lblTo = new System::Windows::Forms::Label();
		this->udMilliSTo = new System::Windows::Forms::NumericUpDown();
		this->udSecondTo = new System::Windows::Forms::NumericUpDown();
		this->udMinuteTo = new System::Windows::Forms::NumericUpDown();
		this->udHourTo = new System::Windows::Forms::NumericUpDown();
		this->lblMilliSTo = new System::Windows::Forms::Label();
		this->lblSecondTo = new System::Windows::Forms::Label();
		this->lblMinuteTo = new System::Windows::Forms::Label();
		this->lblHourTo = new System::Windows::Forms::Label();
		this->dtpTo = new System::Windows::Forms::DateTimePicker();
		this->lnkNowFrom = new System::Windows::Forms::LinkLabel();
		this->lnkNowTo = new System::Windows::Forms::LinkLabel();
		this->chbxEnableFilter = new System::Windows::Forms::CheckBox();
		this->rbInclude = new System::Windows::Forms::RadioButton();
		this->rbExclude = new System::Windows::Forms::RadioButton();
		this->btnOK = new System::Windows::Forms::Button();
		this->btnCancel = new System::Windows::Forms::Button();
		this->grpRange->SuspendLayout();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMilliSFrom)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udSecondFrom)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMinuteFrom)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udHourFrom)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMilliSTo)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udSecondTo)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMinuteTo)->BeginInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udHourTo)->BeginInit();
		this->SuspendLayout();
		// 
		// grpRange
		// 
		this->grpRange->Controls->Add(lnkNowTo);
		this->grpRange->Controls->Add(lnkNowFrom);
		this->grpRange->Controls->Add(lblTo);
		this->grpRange->Controls->Add(udMilliSTo);
		this->grpRange->Controls->Add(udSecondTo);
		this->grpRange->Controls->Add(udMinuteTo);
		this->grpRange->Controls->Add(udHourTo);
		this->grpRange->Controls->Add(lblMilliSTo);
		this->grpRange->Controls->Add(lblSecondTo);
		this->grpRange->Controls->Add(lblMinuteTo);
		this->grpRange->Controls->Add(lblHourTo);
		this->grpRange->Controls->Add(dtpTo);
		this->grpRange->Controls->Add(lblFrom);
		this->grpRange->Controls->Add(udMilliSFrom);
		this->grpRange->Controls->Add(udSecondFrom);
		this->grpRange->Controls->Add(udMinuteFrom);
		this->grpRange->Controls->Add(udHourFrom);
		this->grpRange->Controls->Add(lblMilliSFrom);
		this->grpRange->Controls->Add(lblSecondFrom);
		this->grpRange->Controls->Add(lblMinuteFrom);
		this->grpRange->Controls->Add(lblHourFrom);
		this->grpRange->Controls->Add(dtpFrom);
		this->grpRange->Location = System::Drawing::Point(8, 0);
		this->grpRange->Name = "grpRange";
		this->grpRange->Size = System::Drawing::Size(472, 128);
		this->grpRange->TabIndex = 13;
		this->grpRange->TabStop = false;
		this->grpRange->Text = "Specify range";
		// 
		// udMilliSFrom
		// 
		this->udMilliSFrom->Location = System::Drawing::Point(344, 32);
		this->udMilliSFrom->Minimum = 0;
		this->udMilliSFrom->Maximum = System::Decimal(999);
		this->udMilliSFrom->Name = "udMilliSFrom";
		this->udMilliSFrom->Size = System::Drawing::Size(40, 20);
		this->udMilliSFrom->TabIndex = 21;
		this->udMilliSFrom->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// udSecondFrom
		// 
		this->udSecondFrom->Location = System::Drawing::Point(296, 32);
		this->udSecondFrom->Minimum = 0;
		this->udSecondFrom->Maximum = System::Decimal(59);
		this->udSecondFrom->Name = "udSecondFrom";
		this->udSecondFrom->Size = System::Drawing::Size(40, 20);
		this->udSecondFrom->TabIndex = 20;
		this->udSecondFrom->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// udMinuteFrom
		// 
		this->udMinuteFrom->Location = System::Drawing::Point(240, 32);
		this->udMinuteFrom->Minimum = 0;
		this->udMinuteFrom->Maximum = System::Decimal(59);
		this->udMinuteFrom->Name = "udMinuteFrom";
		this->udMinuteFrom->Size = System::Drawing::Size(40, 20);
		this->udMinuteFrom->TabIndex = 19;
		this->udMinuteFrom->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// udHourFrom
		// 
		this->udHourFrom->Location = System::Drawing::Point(184, 32);
		this->udHourFrom->Minimum = 0;
		this->udHourFrom->Maximum = System::Decimal(23);
		this->udHourFrom->Name = "udHourFrom";
		this->udHourFrom->Size = System::Drawing::Size(40, 20);
		this->udHourFrom->TabIndex = 18;
		this->udHourFrom->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// lblMilliSFrom
		// 
		this->lblMilliSFrom->Location = System::Drawing::Point(384, 40);
		this->lblMilliSFrom->Name = "lblMilliSFrom";
		this->lblMilliSFrom->Size = System::Drawing::Size(24, 16);
		this->lblMilliSFrom->TabIndex = 17;
		this->lblMilliSFrom->Text = "ms";
		this->lblMilliSFrom->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// lblSecondFrom
		// 
		this->lblSecondFrom->Location = System::Drawing::Point(336, 40);
		this->lblSecondFrom->Name = "lblSecondFrom";
		this->lblSecondFrom->Size = System::Drawing::Size(16, 16);
		this->lblSecondFrom->TabIndex = 16;
		this->lblSecondFrom->Text = "s";
		this->lblSecondFrom->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// lblMinuteFrom
		// 
		this->lblMinuteFrom->Location = System::Drawing::Point(280, 40);
		this->lblMinuteFrom->Name = "lblMinuteFrom";
		this->lblMinuteFrom->Size = System::Drawing::Size(16, 16);
		this->lblMinuteFrom->TabIndex = 15;
		this->lblMinuteFrom->Text = "m";
		this->lblMinuteFrom->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// lblHourFrom
		// 
		this->lblHourFrom->Location = System::Drawing::Point(224, 40);
		this->lblHourFrom->Name = "lblHourFrom";
		this->lblHourFrom->Size = System::Drawing::Size(16, 16);
		this->lblHourFrom->TabIndex = 14;
		this->lblHourFrom->Text = "hr";
		this->lblHourFrom->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// dtpFrom
		// 
		this->dtpFrom->Format = System::Windows::Forms::DateTimePickerFormat::Short;
		this->dtpFrom->Location = System::Drawing::Point(64, 32);
		this->dtpFrom->Name = "dtpFrom";
		this->dtpFrom->Size = System::Drawing::Size(112, 20);
		this->dtpFrom->TabIndex = 13;
		// 
		// lblFrom
		// 
		this->lblFrom->Font = new System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, ((System::Byte)(0)));
		this->lblFrom->Location = System::Drawing::Point(24, 29);
		this->lblFrom->Name = "lblFrom";
		this->lblFrom->Size = System::Drawing::Size(32, 24);
		this->lblFrom->TabIndex = 22;
		this->lblFrom->Text = "From";
		this->lblFrom->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// lblTo
		// 
		this->lblTo->Font = new System::Drawing::Font("Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, ((System::Byte)(0)));
		this->lblTo->Location = System::Drawing::Point(25, 77);
		this->lblTo->Name = "lblTo";
		this->lblTo->Size = System::Drawing::Size(32, 24);
		this->lblTo->TabIndex = 32;
		this->lblTo->Text = "To";
		this->lblTo->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
		// 
		// udMilliSTo
		// 
		this->udMilliSTo->Location = System::Drawing::Point(344, 80);
		this->udMilliSTo->Minimum = 0;
		this->udMilliSTo->Maximum = System::Decimal(999);
		this->udMilliSTo->Name = "udMilliSTo";
		this->udMilliSTo->Size = System::Drawing::Size(40, 20);
		this->udMilliSTo->TabIndex = 31;
		this->udMilliSTo->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// udSecondTo
		// 
		this->udSecondTo->Location = System::Drawing::Point(296, 80);
		this->udSecondTo->Minimum = 0;
		this->udSecondTo->Maximum = System::Decimal(59);
		this->udSecondTo->Name = "udSecondTo";
		this->udSecondTo->Size = System::Drawing::Size(40, 20);
		this->udSecondTo->TabIndex = 30;
		this->udSecondTo->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// udMinuteTo
		// 
		this->udMinuteTo->Location = System::Drawing::Point(240, 80);
		this->udMinuteTo->Minimum = 0;
		this->udMinuteTo->Maximum = System::Decimal(59);
		this->udMinuteTo->Name = "udMinuteTo";
		this->udMinuteTo->Size = System::Drawing::Size(40, 20);
		this->udMinuteTo->TabIndex = 29;
		this->udMinuteTo->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// udHourTo
		// 
		this->udHourTo->Location = System::Drawing::Point(184, 80);
		this->udHourTo->Minimum = 0;
		this->udHourTo->Maximum = System::Decimal(23);
		this->udHourTo->Name = "udHourTo";
		this->udHourTo->Size = System::Drawing::Size(40, 20);
		this->udHourTo->TabIndex = 28;
		this->udHourTo->TextAlign = System::Windows::Forms::HorizontalAlignment::Right;
		// 
		// lblMilliSTo
		// 
		this->lblMilliSTo->Location = System::Drawing::Point(384, 88);
		this->lblMilliSTo->Name = "lblMilliSTo";
		this->lblMilliSTo->Size = System::Drawing::Size(24, 16);
		this->lblMilliSTo->TabIndex = 27;
		this->lblMilliSTo->Text = "ms";
		this->lblMilliSTo->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// lblSecondTo
		// 
		this->lblSecondTo->Location = System::Drawing::Point(336, 88);
		this->lblSecondTo->Name = "lblSecondTo";
		this->lblSecondTo->Size = System::Drawing::Size(16, 16);
		this->lblSecondTo->TabIndex = 26;
		this->lblSecondTo->Text = "s";
		this->lblSecondTo->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// lblMinuteTo
		// 
		this->lblMinuteTo->Location = System::Drawing::Point(280, 88);
		this->lblMinuteTo->Name = "lblMinuteTo";
		this->lblMinuteTo->Size = System::Drawing::Size(16, 16);
		this->lblMinuteTo->TabIndex = 25;
		this->lblMinuteTo->Text = "m";
		this->lblMinuteTo->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// lblHourTo
		// 
		this->lblHourTo->Location = System::Drawing::Point(224, 88);
		this->lblHourTo->Name = "lblHourTo";
		this->lblHourTo->Size = System::Drawing::Size(16, 16);
		this->lblHourTo->TabIndex = 24;
		this->lblHourTo->Text = "hr";
		this->lblHourTo->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
		// 
		// dtpTo
		// 
		this->dtpTo->Format = System::Windows::Forms::DateTimePickerFormat::Short;
		this->dtpTo->Location = System::Drawing::Point(64, 80);
		this->dtpTo->Name = "dtpTo";
		this->dtpTo->Size = System::Drawing::Size(112, 20);
		this->dtpTo->TabIndex = 23;
		// 
		// lnkNowFrom
		// 
		this->lnkNowFrom->Location = System::Drawing::Point(408, 37);
		this->lnkNowFrom->Name = "lnkNowFrom";
		this->lnkNowFrom->Size = System::Drawing::Size(32, 16);
		this->lnkNowFrom->TabIndex = 33;
		this->lnkNowFrom->TabStop = true;
		this->lnkNowFrom->Text = "Now";
		this->lnkNowFrom->LinkClicked += new LinkLabelLinkClickedEventHandler(this, lnkNowFrom_LinkClicked);
		// 
		// lnkNowTo
		// 
		this->lnkNowTo->Location = System::Drawing::Point(408, 82);
		this->lnkNowTo->Name = "lnkNowTo";
		this->lnkNowTo->Size = System::Drawing::Size(32, 16);
		this->lnkNowTo->TabIndex = 34;
		this->lnkNowTo->TabStop = true;
		this->lnkNowTo->Text = "Now";
		this->lnkNowTo->LinkClicked += new LinkLabelLinkClickedEventHandler(this, lnkNowTo_LinkClicked);
		// 
		// chbxEnableFilter
		// 
		this->chbxEnableFilter->Location = System::Drawing::Point(8, 176);
		this->chbxEnableFilter->Name = "chbxEnableFilter";
		this->chbxEnableFilter->Size = System::Drawing::Size(88, 16);
		this->chbxEnableFilter->TabIndex = 14;
		this->chbxEnableFilter->Text = "Enable Filter";
		// 
		// rbInclude
		// 
		this->rbInclude->Location = System::Drawing::Point(8, 128);
		this->rbInclude->Name = "rbInclude";
		this->rbInclude->Size = System::Drawing::Size(144, 16);
		this->rbInclude->TabIndex = 15;
		this->rbInclude->Text = "Include all within range";
		// 
		// rbExclude
		// 
		this->rbExclude->Location = System::Drawing::Point(8, 144);
		this->rbExclude->Name = "rbExclude";
		this->rbExclude->Size = System::Drawing::Size(144, 16);
		this->rbExclude->TabIndex = 16;
		this->rbExclude->Text = "Exclude all within range";
		// 
		// btnOK
		// 
		this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
		this->btnOK->Location = System::Drawing::Point(288, 168);
		this->btnOK->Name = "btnOK";
		this->btnOK->Size = System::Drawing::Size(88, 24);
		this->btnOK->TabIndex = 17;
		this->btnOK->Text = "OK";
		// 
		// btnCancel
		// 
		this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
		this->btnCancel->Location = System::Drawing::Point(392, 168);
		this->btnCancel->Name = "btnCancel";
		this->btnCancel->Size = System::Drawing::Size(88, 24);
		this->btnCancel->TabIndex = 18;
		this->btnCancel->Text = "Cancel";
		// 
		// frmTimeStampFilter
		// 
		this->AcceptButton = this->btnOK;
		this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->CancelButton = this->btnCancel;
		this->ClientSize = System::Drawing::Size(490, 200);
		this->Controls->Add(btnCancel);
		this->Controls->Add(btnOK);
		this->Controls->Add(rbExclude);
		this->Controls->Add(rbInclude);
		this->Controls->Add(chbxEnableFilter);
		this->Controls->Add(grpRange);

		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ControlBox = false;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;

		this->Name = "frmTimeStampFilter";
		this->Text = "TimeStamp filter";
		this->grpRange->ResumeLayout(false);
		this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMilliSFrom)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udSecondFrom)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMinuteFrom)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udHourFrom)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMilliSTo)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udSecondTo)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udMinuteTo)->EndInit();
		dynamic_cast <System::ComponentModel::ISupportInitialize *>(this->udHourTo)->EndInit();
		this->ResumeLayout(false);
	}

	//*************************************************************************
	// Method:		ValidateTimeStamps
	// Description: validate the timestamp
	//
	// Parameters:
	//	timeStamp - the timestamp string to validate
	//
	// Return Value: None
	//*************************************************************************
	DateTime TimeStampFilterForm::ValidateTimeStamp(String *timeStamp)
	{
		// make sure the dates parse correctly
		int lastColon = 0;
		for (int i=0; i<3; i++)
			lastColon = timeStamp->IndexOf (":", lastColon + 1);
		
		//store the sub-second value in a string
		String *milliSecond = timeStamp->Substring (lastColon + 1);
		//separate the sub-second part from the string, to make it DateTime compatible
		String * dtString = timeStamp->Substring (0, lastColon);
		//parse rest of string into DateTime type
		DateTime dt = DateTime::Parse (dtString, NULL, Globalization::DateTimeStyles::None);
		//create a DateTime type with date time information including sub-second info.
		DateTime dtLogEntry = DateTime (dt.Year, dt.Month, dt.Day, dt.Hour, dt.Minute, dt.Second, Int32::Parse (milliSecond));
		
		return dtLogEntry;
	}

	//*************************************************************************
	// Method:		OKClick_Func
	// Description: Internal handler for TimeStampFilterForm OK button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::OKClick_Func (System::Object * sender, EventArgs * e)
	{
		try
		{
			String *testFromString = getDtFromString();
			String *testToString = getDtToString();
			
			if (DateTime::Compare (ValidateTimeStamp(testFromString), ValidateTimeStamp(testToString)) > 0)
			{
				UserNotification::InfoNotify("The time specified in the 'From' field may not occur after the time specified in the 'To' field. Please correct this and continue.");
				this->DialogResult = DialogResult::None;
				return;
			}

			//update properties
			this->dtTo = testToString;
			this->dtFrom = testFromString;

			//update filter enable status
			this->FilterEnabled = this->chbxEnableFilter->Checked;
		}
		catch (Exception *)
		{
			UserNotification::InfoNotify("You have not entered valid dates/times.  Please try again.");
			this->DialogResult = DialogResult::None;
		}
	}

	//*************************************************************************
	// Method:		fieldTextValidating
	// Description: Called when the text is being validated in a field
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::fieldTextValidating(System::Object *sender, CancelEventArgs *e)
	{
		if ((Convert::ToDecimal(this->udHourFrom->Text) < this->udHourFrom->Minimum) ||
			(Convert::ToDecimal(this->udHourFrom->Text) > this->udHourFrom->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Hour in the 'From' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udHourTo->Text) < this->udHourTo->Minimum) ||
			(Convert::ToDecimal(this->udHourTo->Text) > this->udHourTo->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Hour in the 'To' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udMilliSFrom->Text) < this->udMilliSFrom->Minimum) ||
			(Convert::ToDecimal(this->udMilliSFrom->Text) > this->udMilliSFrom->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Millisecond in the 'From' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udMilliSTo->Text) < this->udMilliSTo->Minimum) ||
			(Convert::ToDecimal(this->udMilliSTo->Text) > this->udMilliSTo->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Millisecond in the 'To' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udMinuteFrom->Text) < this->udMinuteFrom->Minimum) ||
			(Convert::ToDecimal(this->udMinuteFrom->Text) > this->udMinuteFrom->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Minute in the 'From' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udMinuteTo->Text) < this->udMinuteTo->Minimum) ||
			(Convert::ToDecimal(this->udMinuteTo->Text) > this->udMinuteTo->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Minute in the 'To' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udSecondFrom->Text) < this->udSecondFrom->Minimum) ||
			(Convert::ToDecimal(this->udSecondFrom->Text) > this->udSecondFrom->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Second in the 'From' field");
			e->Cancel = true;
		}
		else if ((Convert::ToDecimal(this->udSecondTo->Text) < this->udSecondTo->Minimum) ||
			(Convert::ToDecimal(this->udSecondTo->Text) > this->udSecondTo->Maximum))
		{
			UserNotification::InfoNotify("Please enter a valid Second in the 'To' field");
			e->Cancel = true;
		}
	}

	//*************************************************************************
	// Method:		fieldValueChangedEventHandlerFunc
	// Description: Called when a significant control's value has been changed
	//				Automatically checks the "Filter Enabled" checkbox
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::fieldValueChangedEventHandlerFunc(System::Object * sender, EventArgs * e)
	{
		this->chbxEnableFilter->Checked = true;
	}

	//*************************************************************************
	// Method:		MakeFromFieldsCurrent
	// Description: Updates the FROM UI elements to reflect the current date and time
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::MakeFromFieldsCurrent()
	{
		//update UI to reflect current time
		this->dtpFrom->Value = DateTime::Now;
		this->udHourFrom->Text = DateTime::Now.Hour.ToString();
		this->udMinuteFrom->Text = DateTime::Now.Minute.ToString();
		this->udSecondFrom->Text = DateTime::Now.Second.ToString();
		this->udMilliSFrom->Text = DateTime::Now.Millisecond.ToString();
	}

	//*************************************************************************
	// Method:		MakeToFieldsCurrent
	// Description: Updates the TO UI elements to reflect the current date and time
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::MakeToFieldsCurrent()
	{
		this->dtpTo->Value = DateTime::Now;
		this->udHourTo->Text = DateTime::Now.Hour.ToString();
		this->udMinuteTo->Text = DateTime::Now.Minute.ToString();
		this->udSecondTo->Text = DateTime::Now.Second.ToString();
		this->udMilliSTo->Text = DateTime::Now.Millisecond.ToString();
	}

	//*************************************************************************
	// Method:		CancelClick_Func
	// Description: Internal handler for TimeStampFilterForm Cancel button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::CancelClick_Func(System::Object * sender, EventArgs * e)
	{
		//restore to old enable setting
		this->chbxEnableFilter->Checked = this->FilterEnabled;
	}


	//*************************************************************************
	// Method:		lnkNowFrom_LinkClicked
	// Description: Click handler for the link Now for the "From" time field
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void TimeStampFilterForm::lnkNowFrom_LinkClicked(Object * sender, LinkLabelLinkClickedEventArgs * e)
	{
		this->MakeFromFieldsCurrent();

	}
	//*************************************************************************
	// Method:		lnkNowTo_LinkClicked
	// Description: Click handler for the link Now for the "To" time field
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************	
	void TimeStampFilterForm::lnkNowTo_LinkClicked(Object * sender, LinkLabelLinkClickedEventArgs * e)
	{
		this->MakeToFieldsCurrent();
	}

	//*************************************************************************
	// Method:		getDtFromString
	// Description: Combines the FROM date and time in a string and returns it
	//
	// Parameters:
	//	None
	//
	// Return Value: String, with date and time details
	//*************************************************************************	
	String * TimeStampFilterForm::getDtFromString()
	{
		String * fromValue = this->dtpFrom->Value.Month.ToString();
		fromValue = String::Concat (fromValue, "/");
		fromValue = String::Concat (fromValue, this->dtpFrom->Value.Day.ToString());
		fromValue = String::Concat (fromValue, "/");
		fromValue = String::Concat (fromValue, this->dtpFrom->Value.Year.ToString());
		fromValue = String::Concat (fromValue, "  ");
		fromValue = String::Concat (fromValue, this->udHourFrom->Text);
		fromValue = String::Concat (fromValue, ":");
		fromValue = String::Concat (fromValue, this->udMinuteFrom->Text);
		fromValue = String::Concat (fromValue, ":");
		fromValue = String::Concat (fromValue, this->udSecondFrom->Text);
		fromValue = String::Concat (fromValue, ":");
		fromValue = String::Concat (fromValue, this->udMilliSFrom->Text);
		return fromValue;
	}
	//*************************************************************************
	// Method:		getDtToString
	// Description: Combines the TO date and time in a string and returns it
	//
	// Parameters:
	//	None
	//
	// Return Value: String, with date and time details
	//*************************************************************************	
	String * TimeStampFilterForm::getDtToString()
	{
		String * toValue = this->dtpTo->Value.Month.ToString();
		toValue = String::Concat (toValue, "/");
		toValue = String::Concat (toValue, this->dtpTo->Value.Day.ToString());
		toValue = String::Concat (toValue, "/");
		toValue = String::Concat (toValue, this->dtpTo->Value.Year.ToString());
		toValue = String::Concat (toValue, "  ");
		toValue = String::Concat (toValue, this->udHourTo->Text);
		toValue = String::Concat (toValue, ":");
		toValue = String::Concat (toValue, this->udMinuteTo->Text);
		toValue = String::Concat (toValue, ":");
		toValue = String::Concat (toValue, this->udSecondTo->Text);
		toValue = String::Concat (toValue, ":");
		toValue = String::Concat (toValue, this->udMilliSTo->Text);
		return toValue;
	}
}