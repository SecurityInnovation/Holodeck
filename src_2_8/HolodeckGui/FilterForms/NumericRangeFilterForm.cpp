//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NumericRangeFilterForm.h
//
// DESCRIPTION: Contains definition for the class NumericRangeFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#include "NumericRangeFilterForm.h"
#include "../ImageListManager.h"
#include "../UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NumericRangeFilterForm
	// Description: Constructor for the NumericRangeFilterForm class
	//
	// Parameters:
	//	OK_Click_EventHandler - the event handler for ok button clicks
	//	Cancel_Click_EventHandler - the event handler for cancel button clicks
	//
	// Return Value: None
	//*************************************************************************
	NumericRangeFilterForm::NumericRangeFilterForm(EventHandler *OK_Click_EventHandler, EventHandler *Cancel_Click_EventHandler)
	{
		filterEnabled = false;
		this->Icon = ImageListManager::getInstance()->AppIcon;
		this->MaximizeBox = false;
		this->MinimizeBox = false;
		this->ControlBox = false;
		this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::Fixed3D;

		InitializeComponent();

		// add the internal button event handlers
		this->okButton->Click += new EventHandler (this, OKClick_Func);
		this->cancelButton->Click += new EventHandler (this, CancelClick_Func);
	
		// if specified, add external button event handlers
		if (Cancel_Click_EventHandler)
			this->cancelButton->Click += Cancel_Click_EventHandler;
		if (OK_Click_EventHandler)
			this->okButton->Click += OK_Click_EventHandler;

		validationHandler = new CancelEventHandler(this, OnValidatingNumericUpDown);
		singleValueNumericUpDown->Validating += validationHandler;
		betweenLowerNumericUpDown->Validating += validationHandler;
		betweenUpperNumericUpDown->Validating += validationHandler;
		lessThanNumericUpDown->Validating += validationHandler;
		greaterThanNumericUpDown->Validating += validationHandler;
	}

	//*************************************************************************
	// Method:		~NumericRangeFilterForm
	// Description: Destructor for the NumericRangeFilterForm class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NumericRangeFilterForm::~NumericRangeFilterForm()
	{
		singleValueNumericUpDown->Validating -= validationHandler;
		betweenLowerNumericUpDown->Validating -= validationHandler;
		betweenUpperNumericUpDown->Validating -= validationHandler;
		lessThanNumericUpDown->Validating -= validationHandler;
		greaterThanNumericUpDown->Validating -= validationHandler;
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Cleans up the object
	//
	// Parameters:
	//	disposing - true if the object is being disposed, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NumericRangeFilterForm::Dispose(Boolean disposing)
	{
		if (disposing && components)
		{
			components->Dispose();
		}
		__super::Dispose(disposing);
	}

	//*************************************************************************
	// Method:		OKClick_Func
	// Description: Internal handler for NumericRangeFilterForm OK button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NumericRangeFilterForm::OKClick_Func (System::Object * sender, EventArgs * e)
	{
		//update filter enable status
		this->FilterEnabled = this->enableFilterCheckBox->Checked;
	}

	//*************************************************************************
	// Method:		CancelClick_Func
	// Description: Internal handler for NumericRangeFilterForm Cancel button Click
	//
	// Parameters:
	//	sender		- sending object
	//	e			- event arguments
	//
	// Return Value: None
	//*************************************************************************
	void NumericRangeFilterForm::CancelClick_Func(System::Object * sender, EventArgs * e)
	{
		//restore to old enable setting
		this->enableFilterCheckBox->Checked = this->FilterEnabled;
	}

	//*************************************************************************
	// Method:		set_MinNumericValue
	// Description: sets the minimum value for numericUpDown controls in the form
	//
	// Parameters:
	//	value - the value to set
	//
	// Return Value: None
	//*************************************************************************
	void NumericRangeFilterForm::set_MinNumericValue(unsigned int value)
	{
		singleValueNumericUpDown->Minimum = Convert::ToDecimal(value);
		betweenLowerNumericUpDown->Minimum = Convert::ToDecimal(value);
		betweenUpperNumericUpDown->Minimum = Convert::ToDecimal(value);
		lessThanNumericUpDown->Minimum = Convert::ToDecimal(value);
		greaterThanNumericUpDown->Minimum = Convert::ToDecimal(value);
	}

	//*************************************************************************
	// Method:		set_MaxNumericValue
	// Description: sets the maximum value for numericUpDown controls in the form
	//
	// Parameters:
	//	value - the value to set
	//
	// Return Value: None
	//*************************************************************************
	void NumericRangeFilterForm::set_MaxNumericValue(unsigned int value)
	{
		singleValueNumericUpDown->Maximum = Convert::ToDecimal(value);
		betweenLowerNumericUpDown->Maximum = Convert::ToDecimal(value);
		betweenUpperNumericUpDown->Maximum = Convert::ToDecimal(value);
		lessThanNumericUpDown->Maximum = Convert::ToDecimal(value);
		greaterThanNumericUpDown->Maximum = Convert::ToDecimal(value);
	}

	//*************************************************************************
	// Method:		OnValidatingNumericUpDown
	// Description: called when a validation event occurs on a numeric up down
	//	control
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - the args of the event
	//
	// Return Value: None
	//*************************************************************************
	void NumericRangeFilterForm::OnValidatingNumericUpDown(Object *sender, CancelEventArgs *e)
	{
		NumericUpDown *control = dynamic_cast<NumericUpDown *>(sender);
		if (!control)
			return;

		Decimal valueToValidate = Convert::ToDecimal(control->Text);
		if ((valueToValidate < control->Minimum) || (valueToValidate > control->Maximum))
		{
			UserNotification::ErrorNotify(String::Format("Invalid value.  The value must be between {0} and {1}", control->Minimum.ToString(), control->Maximum.ToString()));
			e->Cancel = true;
		}
	}
}