//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestFiringSpecificationPage.h
//
// DESCRIPTION: Contains definitions for the class TestFiringSpecificationPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 Dec 2003		 Ady K		 File created.
//*************************************************************************
#pragma once

#include <windows.h>
#include <defines.h>
#include "TestGetFunctionsListDialog.h"

#using <mscorlib.dll>
#using <System.dll>
#using <MagicLibrary.dll>
#using <FunctionDatabase.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	public __gc class FiringDetails
	{
		private:
		//1: Fire xx% of time
		//2: When called by a specified function
		//3: When call stack matches defined pattern
		//4: When any one from defined list is in the callstack
		int chosenFiringOption;
		bool pauseAppOnTestFire;
		Object * firingData;

		public:
		//Default constructor
		//Selects firing option 1 and runs the test 100% of the times
		//and does not pause app when test is fired
		FiringDetails()
		{
			chosenFiringOption = 1;
			pauseAppOnTestFire = false;
			firingData = dynamic_cast <Object *> (S"100");
		}

		FiringDetails(int firingOption, Object * firingData, bool pauseAppWhenTestFires)
		{
			pauseAppOnTestFire = pauseAppWhenTestFires;
			this->firingData = firingData;
			chosenFiringOption = firingOption;
		}

		__property int get_FiringOption() {return chosenFiringOption;}
		__property void set_FiringOption(int value) {chosenFiringOption = value;}
		__property bool get_PauseAppAfterTestFire() {return pauseAppOnTestFire;}
		__property void set_PauseAppAfterTestFire(bool value) {pauseAppOnTestFire = value;}
		__property Object* get_FiringData() {return firingData;}
		__property void set_FiringData(Object * value) {firingData = value;}
	};

	/// <summary> 
	/// Summary for TestFiringSpecificationPage
	///
	/// </summary>
	//public __gc class TestFiringSpecificationPage : public System::Windows::Forms::Form
	public __gc class TestFiringSpecificationPage : public WizardPage
	{
	public: 
		TestFiringSpecificationPage(Array* processSymbols);

		__property FiringDetails *get_FiringSpecification() { return GetFiringDetails(); }
		__property void set_FiringSpecification(FiringDetails *value) { firingDetails = value; this->UpdateUserInterface();}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private:
		FunctionDatabase::InterceptedFunctionDB *db;
		Array * processSymbols;
		ArrayList * immediateCallerList;
		ArrayList * callstackPattern;
		ArrayList * functionsList;
		FiringDetails * firingDetails;
		TestGetFunctionsListDialog * immediateCallerDialog;
		TestGetFunctionsListDialog * callStackPatternDialog;
		TestGetFunctionsListDialog * functionsListDialog;
		
		System::Windows::Forms::GroupBox *  grpMain;
		System::Windows::Forms::RadioButton *  rbFunctionsList;
		System::Windows::Forms::RadioButton *  rbCallStackPattern;
		System::Windows::Forms::RadioButton *  rbImmediateCaller;
		System::Windows::Forms::NumericUpDown *  nudPercentage;
		System::Windows::Forms::RadioButton *  rbRunPercentage;
		System::Windows::Forms::CheckBox *  cbPauseAppAfterTestFire;
		System::ComponentModel::IContainer *  components;
		System::Windows::Forms::LinkLabel *  lnkFunctionsList;
		System::Windows::Forms::LinkLabel *  lnkCallstackPattern;
	private: System::Windows::Forms::Label *  label1;
			 System::Windows::Forms::LinkLabel *  lnkImmediateCaller;
		

		/// <summary>
		/// Required designer variable.
		/// </summary>


		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->grpMain = new System::Windows::Forms::GroupBox();
			this->lnkFunctionsList = new System::Windows::Forms::LinkLabel();
			this->rbFunctionsList = new System::Windows::Forms::RadioButton();
			this->lnkCallstackPattern = new System::Windows::Forms::LinkLabel();
			this->rbCallStackPattern = new System::Windows::Forms::RadioButton();
			this->lnkImmediateCaller = new System::Windows::Forms::LinkLabel();
			this->rbImmediateCaller = new System::Windows::Forms::RadioButton();
			this->nudPercentage = new System::Windows::Forms::NumericUpDown();
			this->rbRunPercentage = new System::Windows::Forms::RadioButton();
			this->cbPauseAppAfterTestFire = new System::Windows::Forms::CheckBox();
			this->label1 = new System::Windows::Forms::Label();
			this->grpMain->SuspendLayout();
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->nudPercentage))->BeginInit();
			this->SuspendLayout();
			// 
			// grpMain
			// 
			this->grpMain->Controls->Add(this->label1);
			this->grpMain->Controls->Add(this->lnkFunctionsList);
			this->grpMain->Controls->Add(this->rbFunctionsList);
			this->grpMain->Controls->Add(this->lnkCallstackPattern);
			this->grpMain->Controls->Add(this->rbCallStackPattern);
			this->grpMain->Controls->Add(this->lnkImmediateCaller);
			this->grpMain->Controls->Add(this->rbImmediateCaller);
			this->grpMain->Controls->Add(this->nudPercentage);
			this->grpMain->Controls->Add(this->rbRunPercentage);
			this->grpMain->Location = System::Drawing::Point(8, 8);
			this->grpMain->Name = S"grpMain";
			this->grpMain->Size = System::Drawing::Size(376, 200);
			this->grpMain->TabIndex = 0;
			this->grpMain->TabStop = false;
			this->grpMain->Text = S"The test should fire:";
			// 
			// label1
			// 
			this->label1->Location = System::Drawing::Point(16, 168);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(336, 16);
			this->label1->TabIndex = 2;
			this->label1->Text = S"* Click on link to define a list";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lnkFunctionsList
			// 
			this->lnkFunctionsList->Location = System::Drawing::Point(24, 128);
			this->lnkFunctionsList->Name = S"lnkFunctionsList";
			this->lnkFunctionsList->Size = System::Drawing::Size(336, 32);
			this->lnkFunctionsList->TabIndex = 17;
			this->lnkFunctionsList->TabStop = true;
			this->lnkFunctionsList->Text = S"when the callstack contains any of the functions in this list.";
			this->lnkFunctionsList->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// rbFunctionsList
			// 
			this->rbFunctionsList->Location = System::Drawing::Point(8, 128);
			this->rbFunctionsList->Name = S"rbFunctionsList";
			this->rbFunctionsList->Size = System::Drawing::Size(344, 32);
			this->rbFunctionsList->TabIndex = 16;
			this->rbFunctionsList->Text = S"Have the test fire when any of these                     are on the callstack";
			// 
			// lnkCallstackPattern
			// 
			this->lnkCallstackPattern->Location = System::Drawing::Point(24, 96);
			this->lnkCallstackPattern->Name = S"lnkCallstackPattern";
			this->lnkCallstackPattern->Size = System::Drawing::Size(344, 24);
			this->lnkCallstackPattern->TabIndex = 15;
			this->lnkCallstackPattern->TabStop = true;
			this->lnkCallstackPattern->Text = S"when the callstack matches the pattern in this list.";
			this->lnkCallstackPattern->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// rbCallStackPattern
			// 
			this->rbCallStackPattern->Location = System::Drawing::Point(8, 96);
			this->rbCallStackPattern->Name = S"rbCallStackPattern";
			this->rbCallStackPattern->Size = System::Drawing::Size(352, 24);
			this->rbCallStackPattern->TabIndex = 14;
			this->rbCallStackPattern->Text = S"Have the test fire when the callstack matches this";
			// 
			// lnkImmediateCaller
			// 
			this->lnkImmediateCaller->Location = System::Drawing::Point(24, 56);
			this->lnkImmediateCaller->Name = S"lnkImmediateCaller";
			this->lnkImmediateCaller->Size = System::Drawing::Size(344, 32);
			this->lnkImmediateCaller->TabIndex = 13;
			this->lnkImmediateCaller->TabStop = true;
			this->lnkImmediateCaller->Text = S"when this function is called directly by any of the functions in this list.";
			this->lnkImmediateCaller->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// rbImmediateCaller
			// 
			this->rbImmediateCaller->Location = System::Drawing::Point(8, 56);
			this->rbImmediateCaller->Name = S"rbImmediateCaller";
			this->rbImmediateCaller->Size = System::Drawing::Size(352, 32);
			this->rbImmediateCaller->TabIndex = 12;
			this->rbImmediateCaller->Text = S"Have the test fire when the test function is called by any one of the functions d" 
				S"efined in this";
			// 
			// nudPercentage
			// 
			this->nudPercentage->Location = System::Drawing::Point(28, 26);
			this->nudPercentage->Name = S"nudPercentage";
			this->nudPercentage->Size = System::Drawing::Size(40, 20);
			this->nudPercentage->TabIndex = 11;
			System::Int32 __mcTemp__1[] = new System::Int32[4];
			__mcTemp__1[0] = 100;
			__mcTemp__1[1] = 0;
			__mcTemp__1[2] = 0;
			__mcTemp__1[3] = 0;
			this->nudPercentage->Value = System::Decimal(__mcTemp__1);
			// 
			// rbRunPercentage
			// 
			this->rbRunPercentage->Checked = true;
			this->rbRunPercentage->Location = System::Drawing::Point(8, 24);
			this->rbRunPercentage->Name = S"rbRunPercentage";
			this->rbRunPercentage->Size = System::Drawing::Size(352, 24);
			this->rbRunPercentage->TabIndex = 10;
			this->rbRunPercentage->TabStop = true;
			this->rbRunPercentage->Text = S"               % of the time";
			// 
			// cbPauseAppAfterTestFire
			// 
			this->cbPauseAppAfterTestFire->Location = System::Drawing::Point(16, 216);
			this->cbPauseAppAfterTestFire->Name = S"cbPauseAppAfterTestFire";
			this->cbPauseAppAfterTestFire->Size = System::Drawing::Size(224, 16);
			this->cbPauseAppAfterTestFire->TabIndex = 1;
			this->cbPauseAppAfterTestFire->Text = S"Pause application when test is fired";
			// 
			// TestFiringSpecificationPage
			// 
			this->ClientSize = System::Drawing::Size(392, 241);
			this->Controls->Add(this->cbPauseAppAfterTestFire);
			this->Controls->Add(this->grpMain);
			this->Name = S"TestFiringSpecificationPage";
			this->Text = S"Title";
			this->grpMain->ResumeLayout(false);
			(__try_cast<System::ComponentModel::ISupportInitialize *  >(this->nudPercentage))->EndInit();
			this->ResumeLayout(false);

		}		

		void UpdateUserInterface();
		FiringDetails * GetFiringDetails();

		void lnkimmediateCaller_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e);
		void lnkCallstackPattern_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e);
		void lnkFunctionsList_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e);
		String * GetFuncNameWithoutClass(String * fullFunctionName);
	};
}