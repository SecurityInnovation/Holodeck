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
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

namespace HolodeckGui
{
	//*************************************************************************
	// Class:		NumericRangeFilterForm
	// Description: Form Class which provides an interface for a filter for
	//	numeric values, including ranges of values
	//*************************************************************************
	public __gc class NumericRangeFilterForm : public System::Windows::Forms::Form
	{
	public: 
		NumericRangeFilterForm(EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler);
		~NumericRangeFilterForm();

		__property bool get_FilterEnabled() {return filterEnabled;}
		__property void set_FilterEnabled(bool value) {filterEnabled = value;}

		__property bool get_IncludeValues() { return includeAllRadioButton->Checked; }

		__property bool get_UseSingleValue() { return singleValueRadioButton->Checked; }
		__property unsigned int get_SingleValue() { return Convert::ToUInt32(singleValueNumericUpDown->Value); }

		__property bool get_UseRange() { return betweenValuesRadioButton->Checked; }
		__property unsigned int get_LowerRangeValue() { return Convert::ToUInt32(betweenLowerNumericUpDown->Value); }
		__property unsigned int get_UpperRangeValue() { return Convert::ToUInt32(betweenUpperNumericUpDown->Value); }

		__property bool get_UseLessThan() { return lessThanValueRadioButton->Checked; }
		__property unsigned int get_LessThanValue() { return Convert::ToUInt32(lessThanNumericUpDown->Value); }

		__property bool get_UseGreaterThan() { return greaterThanValueRadioButton->Checked; }
		__property unsigned int get_GreaterThanValue() { return Convert::ToUInt32(greaterThanNumericUpDown->Value); }

		__property unsigned int get_MinNumericValue() { return Convert::ToUInt32(singleValueNumericUpDown->Minimum); }
		__property void set_MinNumericValue(unsigned int value);

		__property unsigned int get_MaxNumericValue() { return Convert::ToUInt32(singleValueNumericUpDown->Maximum); }
		__property void set_MaxNumericValue(unsigned int value);
        
	protected:
		bool filterEnabled;
		CancelEventHandler *validationHandler;

		void Dispose(Boolean disposing);

		System::Windows::Forms::GroupBox *valueTypeGroupBox;
		System::Windows::Forms::RadioButton *singleValueRadioButton;
		System::Windows::Forms::RadioButton *betweenValuesRadioButton;
		System::Windows::Forms::RadioButton *lessThanValueRadioButton;
		System::Windows::Forms::RadioButton *greaterThanValueRadioButton;
		System::Windows::Forms::NumericUpDown *singleValueNumericUpDown;
		System::Windows::Forms::NumericUpDown *betweenLowerNumericUpDown;
		System::Windows::Forms::NumericUpDown *betweenUpperNumericUpDown;
		System::Windows::Forms::Label *label1;
		System::Windows::Forms::NumericUpDown *lessThanNumericUpDown;
		System::Windows::Forms::NumericUpDown *greaterThanNumericUpDown;
		System::Windows::Forms::RadioButton *includeAllRadioButton;
		System::Windows::Forms::RadioButton *excludeAllRadioButton;
		System::Windows::Forms::CheckBox *enableFilterCheckBox;
		System::Windows::Forms::Button *okButton;
		System::Windows::Forms::Button *cancelButton;
		System::ComponentModel::Container *components;

		void OKClick_Func(System::Object *sender, EventArgs *e);
		void CancelClick_Func(System::Object *sender, EventArgs *e);
		void OnValidatingNumericUpDown(Object *sender, CancelEventArgs *e);

		void InitializeComponent(void)
		{
			this->valueTypeGroupBox = new System::Windows::Forms::GroupBox();
			this->greaterThanNumericUpDown = new System::Windows::Forms::NumericUpDown();
			this->lessThanNumericUpDown = new System::Windows::Forms::NumericUpDown();
			this->label1 = new System::Windows::Forms::Label();
			this->betweenUpperNumericUpDown = new System::Windows::Forms::NumericUpDown();
			this->betweenLowerNumericUpDown = new System::Windows::Forms::NumericUpDown();
			this->singleValueNumericUpDown = new System::Windows::Forms::NumericUpDown();
			this->greaterThanValueRadioButton = new System::Windows::Forms::RadioButton();
			this->lessThanValueRadioButton = new System::Windows::Forms::RadioButton();
			this->betweenValuesRadioButton = new System::Windows::Forms::RadioButton();
			this->singleValueRadioButton = new System::Windows::Forms::RadioButton();
			this->includeAllRadioButton = new System::Windows::Forms::RadioButton();
			this->excludeAllRadioButton = new System::Windows::Forms::RadioButton();
			this->enableFilterCheckBox = new System::Windows::Forms::CheckBox();
			this->okButton = new System::Windows::Forms::Button();
			this->cancelButton = new System::Windows::Forms::Button();
			this->valueTypeGroupBox->SuspendLayout();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->greaterThanNumericUpDown))->BeginInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->lessThanNumericUpDown))->BeginInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->betweenUpperNumericUpDown))->BeginInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->betweenLowerNumericUpDown))->BeginInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->singleValueNumericUpDown))->BeginInit();
			this->SuspendLayout();
			// 
			// valueTypeGroupBox
			// 
			this->valueTypeGroupBox->Controls->Add(this->greaterThanNumericUpDown);
			this->valueTypeGroupBox->Controls->Add(this->lessThanNumericUpDown);
			this->valueTypeGroupBox->Controls->Add(this->label1);
			this->valueTypeGroupBox->Controls->Add(this->betweenUpperNumericUpDown);
			this->valueTypeGroupBox->Controls->Add(this->betweenLowerNumericUpDown);
			this->valueTypeGroupBox->Controls->Add(this->singleValueNumericUpDown);
			this->valueTypeGroupBox->Controls->Add(this->greaterThanValueRadioButton);
			this->valueTypeGroupBox->Controls->Add(this->lessThanValueRadioButton);
			this->valueTypeGroupBox->Controls->Add(this->betweenValuesRadioButton);
			this->valueTypeGroupBox->Controls->Add(this->singleValueRadioButton);
			this->valueTypeGroupBox->Location = System::Drawing::Point(8, 8);
			this->valueTypeGroupBox->Name = S"valueTypeGroupBox";
			this->valueTypeGroupBox->Size = System::Drawing::Size(304, 152);
			this->valueTypeGroupBox->TabIndex = 0;
			this->valueTypeGroupBox->TabStop = false;
			this->valueTypeGroupBox->Text = S"Select Value Type";
			// 
			// greaterThanNumericUpDown
			// 
			this->greaterThanNumericUpDown->Location = System::Drawing::Point(128, 120);
			System::Int32 __mcTemp__1[] = new System::Int32[4];
			__mcTemp__1[0] = 9999999;
			__mcTemp__1[1] = 0;
			__mcTemp__1[2] = 0;
			__mcTemp__1[3] = 0;
			this->greaterThanNumericUpDown->Maximum = System::Decimal(__mcTemp__1);
			this->greaterThanNumericUpDown->Name = S"greaterThanNumericUpDown";
			this->greaterThanNumericUpDown->Size = System::Drawing::Size(64, 20);
			this->greaterThanNumericUpDown->TabIndex = 9;
			// 
			// lessThanNumericUpDown
			// 
			this->lessThanNumericUpDown->Location = System::Drawing::Point(128, 88);
			System::Int32 __mcTemp__2[] = new System::Int32[4];
			__mcTemp__2[0] = 9999999;
			__mcTemp__2[1] = 0;
			__mcTemp__2[2] = 0;
			__mcTemp__2[3] = 0;
			this->lessThanNumericUpDown->Maximum = System::Decimal(__mcTemp__2);
			this->lessThanNumericUpDown->Name = S"lessThanNumericUpDown";
			this->lessThanNumericUpDown->Size = System::Drawing::Size(64, 20);
			this->lessThanNumericUpDown->TabIndex = 8;
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(200, 48);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(24, 24);
			this->label1->TabIndex = 7;
			this->label1->Text = S"and";
			this->label1->TextAlign = System::Drawing::ContentAlignment::BottomCenter;
			// 
			// betweenUpperNumericUpDown
			// 
			this->betweenUpperNumericUpDown->Location = System::Drawing::Point(232, 56);
			System::Int32 __mcTemp__3[] = new System::Int32[4];
			__mcTemp__3[0] = 9999999;
			__mcTemp__3[1] = 0;
			__mcTemp__3[2] = 0;
			__mcTemp__3[3] = 0;
			this->betweenUpperNumericUpDown->Maximum = System::Decimal(__mcTemp__3);
			this->betweenUpperNumericUpDown->Name = S"betweenUpperNumericUpDown";
			this->betweenUpperNumericUpDown->Size = System::Drawing::Size(64, 20);
			this->betweenUpperNumericUpDown->TabIndex = 6;
			// 
			// betweenLowerNumericUpDown
			// 
			this->betweenLowerNumericUpDown->Location = System::Drawing::Point(128, 56);
			System::Int32 __mcTemp__4[] = new System::Int32[4];
			__mcTemp__4[0] = 9999999;
			__mcTemp__4[1] = 0;
			__mcTemp__4[2] = 0;
			__mcTemp__4[3] = 0;
			this->betweenLowerNumericUpDown->Maximum = System::Decimal(__mcTemp__4);
			this->betweenLowerNumericUpDown->Name = S"betweenLowerNumericUpDown";
			this->betweenLowerNumericUpDown->Size = System::Drawing::Size(64, 20);
			this->betweenLowerNumericUpDown->TabIndex = 5;
			// 
			// singleValueNumericUpDown
			// 
			this->singleValueNumericUpDown->Location = System::Drawing::Point(128, 24);
			System::Int32 __mcTemp__5[] = new System::Int32[4];
			__mcTemp__5[0] = 9999999;
			__mcTemp__5[1] = 0;
			__mcTemp__5[2] = 0;
			__mcTemp__5[3] = 0;
			this->singleValueNumericUpDown->Maximum = System::Decimal(__mcTemp__5);
			this->singleValueNumericUpDown->Name = S"singleValueNumericUpDown";
			this->singleValueNumericUpDown->Size = System::Drawing::Size(64, 20);
			this->singleValueNumericUpDown->TabIndex = 4;
			// 
			// greaterThanValueRadioButton
			// 
			this->greaterThanValueRadioButton->Location = System::Drawing::Point(16, 120);
			this->greaterThanValueRadioButton->Name = S"greaterThanValueRadioButton";
			this->greaterThanValueRadioButton->TabIndex = 3;
			this->greaterThanValueRadioButton->Text = S"Greater Than";
			// 
			// lessThanValueRadioButton
			// 
			this->lessThanValueRadioButton->Location = System::Drawing::Point(16, 88);
			this->lessThanValueRadioButton->Name = S"lessThanValueRadioButton";
			this->lessThanValueRadioButton->TabIndex = 2;
			this->lessThanValueRadioButton->Text = S"Less Than";
			// 
			// betweenValuesRadioButton
			// 
			this->betweenValuesRadioButton->Location = System::Drawing::Point(16, 56);
			this->betweenValuesRadioButton->Name = S"betweenValuesRadioButton";
			this->betweenValuesRadioButton->TabIndex = 1;
			this->betweenValuesRadioButton->Text = S"Between Values";
			// 
			// singleValueRadioButton
			// 
			this->singleValueRadioButton->Checked = true;
			this->singleValueRadioButton->Location = System::Drawing::Point(16, 24);
			this->singleValueRadioButton->Name = S"singleValueRadioButton";
			this->singleValueRadioButton->Size = System::Drawing::Size(88, 24);
			this->singleValueRadioButton->TabIndex = 0;
			this->singleValueRadioButton->TabStop = true;
			this->singleValueRadioButton->Text = S"Single Value";
			// 
			// includeAllRadioButton
			// 
			this->includeAllRadioButton->Checked = true;
			this->includeAllRadioButton->Location = System::Drawing::Point(16, 176);
			this->includeAllRadioButton->Name = S"includeAllRadioButton";
			this->includeAllRadioButton->Size = System::Drawing::Size(144, 24);
			this->includeAllRadioButton->TabIndex = 1;
			this->includeAllRadioButton->TabStop = true;
			this->includeAllRadioButton->Text = S"Include all specified";
			// 
			// excludeAllRadioButton
			// 
			this->excludeAllRadioButton->Location = System::Drawing::Point(16, 200);
			this->excludeAllRadioButton->Name = S"excludeAllRadioButton";
			this->excludeAllRadioButton->Size = System::Drawing::Size(144, 24);
			this->excludeAllRadioButton->TabIndex = 2;
			this->excludeAllRadioButton->Text = S"Exclude all specified";
			// 
			// enableFilterCheckBox
			// 
			this->enableFilterCheckBox->Checked = true;
			this->enableFilterCheckBox->CheckState = System::Windows::Forms::CheckState::Checked;
			this->enableFilterCheckBox->Location = System::Drawing::Point(192, 176);
			this->enableFilterCheckBox->Name = S"enableFilterCheckBox";
			this->enableFilterCheckBox->Size = System::Drawing::Size(88, 24);
			this->enableFilterCheckBox->TabIndex = 3;
			this->enableFilterCheckBox->Text = S"Enable Filter";
			// 
			// okButton
			// 
			this->okButton->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->okButton->Location = System::Drawing::Point(144, 232);
			this->okButton->Name = S"okButton";
			this->okButton->TabIndex = 4;
			this->okButton->Text = S"OK";
			// 
			// cancelButton
			// 
			this->cancelButton->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->cancelButton->Location = System::Drawing::Point(232, 232);
			this->cancelButton->Name = S"cancelButton";
			this->cancelButton->TabIndex = 5;
			this->cancelButton->Text = S"Cancel";
			// 
			// NumericRangeFilterForm
			// 
			this->AcceptButton = this->okButton;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->CancelButton = this->cancelButton;
			this->ClientSize = System::Drawing::Size(320, 262);
			this->Controls->Add(this->cancelButton);
			this->Controls->Add(this->okButton);
			this->Controls->Add(this->enableFilterCheckBox);
			this->Controls->Add(this->excludeAllRadioButton);
			this->Controls->Add(this->includeAllRadioButton);
			this->Controls->Add(this->valueTypeGroupBox);
			this->Name = S"NumericRangeFilterForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = S"NumericRangeFilterForm";
			this->valueTypeGroupBox->ResumeLayout(false);
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->greaterThanNumericUpDown))->EndInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->lessThanNumericUpDown))->EndInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->betweenUpperNumericUpDown))->EndInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->betweenLowerNumericUpDown))->EndInit();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->singleValueNumericUpDown))->EndInit();
			this->ResumeLayout(false);

		}		
	};
}