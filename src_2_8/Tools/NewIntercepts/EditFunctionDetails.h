//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		EditFunctionDetails.h
//
// DESCRIPTION: Contains definition for the class EditFunctionDetails
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 10 Oct 2003		 Ady K	   Created
//*******************************************************************************
#pragma once

#include "AddCustomInterceptWizardControl.h"
#include "addcustintwizpg2.h"
#include "defines.h"

#using <FunctionDatabase.dll>
#using <GuiSupport.dll>

#undef MessageBox

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace FunctionDatabase;


namespace AddNewIntercepts
{
	/// <summary> 
	/// Summary for EditFunctionDetails
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class EditFunctionDetails : public System::Windows::Forms::Form
	{
	public: 
		EditFunctionDetails();
		functionInformation * displayFunctionInformation (functionInformation * fInfo, bool isEditable);
		ArrayList * getStringTokens (String * tString);
		void updateFunctionDefinition();
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private: int subItemSelected;
	private: ListViewItem * lviParam;
	private: Point firstMouseClickPoint;


	private: System::Windows::Forms::Label *  lblStar1;
	private: System::Windows::Forms::Label *  label1;
	private: System::Windows::Forms::Label *  lblStarInfo;
	private: System::Windows::Forms::Label *  lblTypeModifiers;
	private: System::Windows::Forms::TextBox *  txtTypeMods;


	private: System::Windows::Forms::TextBox *  txtDefinition;
	private: System::Windows::Forms::Label *  lblDefinition;
	private: System::Windows::Forms::TextBox *  txtCCMods;
	private: System::Windows::Forms::Label *  lblCCMods;



	private: TextBox * txtEditor;
	private: ArrayList * getUnManagedTypes();
	private: ArrayList * getManagedTypes();
	private: InterceptedFunctionDB * dotNetInterceptedFunctionDB;
	private: functionInformation * gatherEditedInformation();
	private: functionInformation * internalCopyofFuncInfo;
	private: void setComboBoxValuesFromArray (ComboBox * cbox, ArrayList * values, bool shouldClear);
	private: void definitionChanged_Handler (Object * sender, EventArgs * e);

	private: System::Windows::Forms::GroupBox *  grpMain;
	private: System::Windows::Forms::Button *  btnCancel;
	private: System::Windows::Forms::Button *  btnOK;
	private: System::Windows::Forms::ToolTip *  toolTip;
	private: System::Windows::Forms::GroupBox *  grpFunctionName;
	private: System::Windows::Forms::TextBox *  txtLocation;
	private: System::Windows::Forms::Label *  lblLocation;
	private: System::Windows::Forms::TextBox *  txtName;
	private: System::Windows::Forms::Label *  lblName;
	private: System::Windows::Forms::GroupBox *  grpTypeSepcification;






	private: System::Windows::Forms::ComboBox *  cmbReturnType;
	private: System::Windows::Forms::Label *  lblCategory;
	private: System::Windows::Forms::ComboBox *  cmbCategory;
	private: System::Windows::Forms::GroupBox *  grpParamSpecification;
	private: System::Windows::Forms::ListView *  lvParams;
	private: System::Windows::Forms::ColumnHeader *  chParamIndex;
	private: System::Windows::Forms::ColumnHeader *  chParamIdentifier;
	private: System::Windows::Forms::ColumnHeader *  chParamType;
	private: System::Windows::Forms::Label *  lblReturnType;
	private: System::Windows::Forms::Panel *  panelParamButtons;
	private: System::Windows::Forms::Button *  btnParamDel;
	private: System::Windows::Forms::Button *  btnParamAdd;


	private: System::ComponentModel::IContainer *  components;
	private: System::Void btnParamAdd_Click(System::Object *  sender, System::EventArgs *  e);
	private: System::Void btnParamDel_Click(System::Object *  sender, System::EventArgs *  e);
	private: void txtEditor_KeyPress (Object * sender , KeyPressEventArgs * e);	
	private: void txtEditor_LostFocus (Object * sender, EventArgs * e);
	private: void lvParams_MouseDown(Object * sender, MouseEventArgs * e);
	private: void lvParams_MouseDoubleClick(Object * sender, EventArgs * e);
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = new System::ComponentModel::Container();
			System::String* __mcTemp__1[] = new System::String*[4];
			__mcTemp__1[0] = S"0";
			__mcTemp__1[1] = S"TestVariableName";
			__mcTemp__1[2] = S"System.Boolean";
			__mcTemp__1[3] = S"no";
			System::Windows::Forms::ListViewItem *  listViewItem1 = new System::Windows::Forms::ListViewItem(__mcTemp__1, 1);
			System::String* __mcTemp__2[] = new System::String*[4];
			__mcTemp__2[0] = S"1";
			__mcTemp__2[1] = S"VeryLongVariableNameThisIsATest";
			__mcTemp__2[2] = S"System.WeirdSystemTypeToTestMyListView";
			__mcTemp__2[3] = S"yes";
			System::Windows::Forms::ListViewItem *  listViewItem2 = new System::Windows::Forms::ListViewItem(__mcTemp__2, 1);
			this->grpMain = new System::Windows::Forms::GroupBox();
			this->grpParamSpecification = new System::Windows::Forms::GroupBox();
			this->panelParamButtons = new System::Windows::Forms::Panel();
			this->btnParamDel = new System::Windows::Forms::Button();
			this->btnParamAdd = new System::Windows::Forms::Button();
			this->lvParams = new System::Windows::Forms::ListView();
			this->chParamIndex = new System::Windows::Forms::ColumnHeader();
			this->chParamIdentifier = new System::Windows::Forms::ColumnHeader();
			this->chParamType = new System::Windows::Forms::ColumnHeader();
			this->grpTypeSepcification = new System::Windows::Forms::GroupBox();
			this->lblStar1 = new System::Windows::Forms::Label();
			this->txtCCMods = new System::Windows::Forms::TextBox();
			this->lblCCMods = new System::Windows::Forms::Label();
			this->txtTypeMods = new System::Windows::Forms::TextBox();
			this->lblTypeModifiers = new System::Windows::Forms::Label();
			this->lblReturnType = new System::Windows::Forms::Label();
			this->cmbReturnType = new System::Windows::Forms::ComboBox();
			this->lblCategory = new System::Windows::Forms::Label();
			this->cmbCategory = new System::Windows::Forms::ComboBox();
			this->grpFunctionName = new System::Windows::Forms::GroupBox();
			this->txtDefinition = new System::Windows::Forms::TextBox();
			this->lblDefinition = new System::Windows::Forms::Label();
			this->txtLocation = new System::Windows::Forms::TextBox();
			this->lblLocation = new System::Windows::Forms::Label();
			this->txtName = new System::Windows::Forms::TextBox();
			this->lblName = new System::Windows::Forms::Label();
			this->btnCancel = new System::Windows::Forms::Button();
			this->btnOK = new System::Windows::Forms::Button();
			this->toolTip = new System::Windows::Forms::ToolTip(this->components);
			this->label1 = new System::Windows::Forms::Label();
			this->lblStarInfo = new System::Windows::Forms::Label();
			this->grpMain->SuspendLayout();
			this->grpParamSpecification->SuspendLayout();
			this->panelParamButtons->SuspendLayout();
			this->grpTypeSepcification->SuspendLayout();
			this->grpFunctionName->SuspendLayout();
			this->SuspendLayout();
			// 
			// grpMain
			// 
			this->grpMain->Controls->Add(this->grpParamSpecification);
			this->grpMain->Controls->Add(this->grpTypeSepcification);
			this->grpMain->Controls->Add(this->grpFunctionName);
			this->grpMain->Location = System::Drawing::Point(0, 0);
			this->grpMain->Name = S"grpMain";
			this->grpMain->Size = System::Drawing::Size(432, 528);
			this->grpMain->TabIndex = 0;
			this->grpMain->TabStop = false;
			// 
			// grpParamSpecification
			// 
			this->grpParamSpecification->Controls->Add(this->panelParamButtons);
			this->grpParamSpecification->Controls->Add(this->lvParams);
			this->grpParamSpecification->Location = System::Drawing::Point(8, 336);
			this->grpParamSpecification->Name = S"grpParamSpecification";
			this->grpParamSpecification->Size = System::Drawing::Size(416, 184);
			this->grpParamSpecification->TabIndex = 12;
			this->grpParamSpecification->TabStop = false;
			this->grpParamSpecification->Text = S"Parameter Sepcification";
			// 
			// panelParamButtons
			// 
			this->panelParamButtons->Controls->Add(this->btnParamDel);
			this->panelParamButtons->Controls->Add(this->btnParamAdd);
			this->panelParamButtons->Location = System::Drawing::Point(374, 24);
			this->panelParamButtons->Name = S"panelParamButtons";
			this->panelParamButtons->Size = System::Drawing::Size(30, 144);
			this->panelParamButtons->TabIndex = 1;
			// 
			// btnParamDel
			// 
			this->btnParamDel->Location = System::Drawing::Point(4, 72);
			this->btnParamDel->Name = S"btnParamDel";
			this->btnParamDel->Size = System::Drawing::Size(24, 16);
			this->btnParamDel->TabIndex = 8;
			this->btnParamDel->Text = S"-";
			this->btnParamDel->Click += new System::EventHandler(this, btnParamDel_Click);
			// 
			// btnParamAdd
			// 
			this->btnParamAdd->Location = System::Drawing::Point(4, 56);
			this->btnParamAdd->Name = S"btnParamAdd";
			this->btnParamAdd->Size = System::Drawing::Size(24, 16);
			this->btnParamAdd->TabIndex = 7;
			this->btnParamAdd->Text = S"+";
			this->btnParamAdd->Click += new System::EventHandler(this, btnParamAdd_Click);
			// 
			// lvParams
			// 
			System::Windows::Forms::ColumnHeader* __mcTemp__3[] = new System::Windows::Forms::ColumnHeader*[3];
			__mcTemp__3[0] = this->chParamIndex;
			__mcTemp__3[1] = this->chParamIdentifier;
			__mcTemp__3[2] = this->chParamType;
			this->lvParams->Columns->AddRange(__mcTemp__3);
			this->lvParams->FullRowSelect = true;
			this->lvParams->GridLines = true;
			this->lvParams->HideSelection = false;
			System::Windows::Forms::ListViewItem* __mcTemp__4[] = new System::Windows::Forms::ListViewItem*[2];
			__mcTemp__4[0] = listViewItem1;
			__mcTemp__4[1] = listViewItem2;
			this->lvParams->Items->AddRange(__mcTemp__4);
			this->lvParams->Location = System::Drawing::Point(20, 24);
			this->lvParams->MultiSelect = false;
			this->lvParams->Name = S"lvParams";
			this->lvParams->Size = System::Drawing::Size(352, 144);
			this->lvParams->TabIndex = 0;
			this->lvParams->View = System::Windows::Forms::View::Details;
			// 
			// chParamIndex
			// 
			this->chParamIndex->Text = S"Index";
			this->chParamIndex->Width = 38;
			// 
			// chParamIdentifier
			// 
			this->chParamIdentifier->Text = S"Identifier";
			this->chParamIdentifier->Width = 170;
			// 
			// chParamType
			// 
			this->chParamType->Text = S"Type";
			this->chParamType->Width = 140;
			// 
			// grpTypeSepcification
			// 
			this->grpTypeSepcification->Controls->Add(this->lblStar1);
			this->grpTypeSepcification->Controls->Add(this->txtCCMods);
			this->grpTypeSepcification->Controls->Add(this->lblCCMods);
			this->grpTypeSepcification->Controls->Add(this->txtTypeMods);
			this->grpTypeSepcification->Controls->Add(this->lblTypeModifiers);
			this->grpTypeSepcification->Controls->Add(this->lblReturnType);
			this->grpTypeSepcification->Controls->Add(this->cmbReturnType);
			this->grpTypeSepcification->Controls->Add(this->lblCategory);
			this->grpTypeSepcification->Controls->Add(this->cmbCategory);
			this->grpTypeSepcification->Location = System::Drawing::Point(8, 160);
			this->grpTypeSepcification->Name = S"grpTypeSepcification";
			this->grpTypeSepcification->Size = System::Drawing::Size(416, 168);
			this->grpTypeSepcification->TabIndex = 11;
			this->grpTypeSepcification->TabStop = false;
			this->grpTypeSepcification->Text = S"Type Specification";
			// 
			// lblStar1
			// 
			this->lblStar1->BackColor = System::Drawing::SystemColors::Control;
			this->lblStar1->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->lblStar1->ForeColor = System::Drawing::Color::Red;
			this->lblStar1->Location = System::Drawing::Point(398, 56);
			this->lblStar1->Name = S"lblStar1";
			this->lblStar1->Size = System::Drawing::Size(16, 16);
			this->lblStar1->TabIndex = 19;
			this->lblStar1->Text = S"*";
			// 
			// txtCCMods
			// 
			this->txtCCMods->Location = System::Drawing::Point(85, 136);
			this->txtCCMods->Name = S"txtCCMods";
			this->txtCCMods->Size = System::Drawing::Size(315, 20);
			this->txtCCMods->TabIndex = 22;
			this->txtCCMods->Text = S"";
			// 
			// lblCCMods
			// 
			this->lblCCMods->Location = System::Drawing::Point(8, 112);
			this->lblCCMods->Name = S"lblCCMods";
			this->lblCCMods->Size = System::Drawing::Size(152, 24);
			this->lblCCMods->TabIndex = 21;
			this->lblCCMods->Text = S"Calling Convention Modifiers:";
			this->lblCCMods->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;
			// 
			// txtTypeMods
			// 
			this->txtTypeMods->Location = System::Drawing::Point(86, 88);
			this->txtTypeMods->Name = S"txtTypeMods";
			this->txtTypeMods->Size = System::Drawing::Size(314, 20);
			this->txtTypeMods->TabIndex = 20;
			this->txtTypeMods->Text = S"";
			// 
			// lblTypeModifiers
			// 
			this->lblTypeModifiers->Location = System::Drawing::Point(6, 88);
			this->lblTypeModifiers->Name = S"lblTypeModifiers";
			this->lblTypeModifiers->Size = System::Drawing::Size(80, 16);
			this->lblTypeModifiers->TabIndex = 14;
			this->lblTypeModifiers->Text = S"Type Modifiers";
			this->lblTypeModifiers->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lblReturnType
			// 
			this->lblReturnType->Location = System::Drawing::Point(14, 56);
			this->lblReturnType->Name = S"lblReturnType";
			this->lblReturnType->Size = System::Drawing::Size(72, 16);
			this->lblReturnType->TabIndex = 13;
			this->lblReturnType->Text = S"Return Type";
			this->lblReturnType->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// cmbReturnType
			// 
			this->cmbReturnType->Location = System::Drawing::Point(86, 56);
			this->cmbReturnType->Name = S"cmbReturnType";
			this->cmbReturnType->Size = System::Drawing::Size(314, 21);
			this->cmbReturnType->TabIndex = 12;
			// 
			// lblCategory
			// 
			this->lblCategory->Location = System::Drawing::Point(16, 24);
			this->lblCategory->Name = S"lblCategory";
			this->lblCategory->Size = System::Drawing::Size(64, 16);
			this->lblCategory->TabIndex = 11;
			this->lblCategory->Text = S"Category";
			this->lblCategory->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// cmbCategory
			// 
			this->cmbCategory->Location = System::Drawing::Point(84, 24);
			this->cmbCategory->Name = S"cmbCategory";
			this->cmbCategory->Size = System::Drawing::Size(316, 21);
			this->cmbCategory->TabIndex = 10;
			// 
			// grpFunctionName
			// 
			this->grpFunctionName->Controls->Add(this->txtDefinition);
			this->grpFunctionName->Controls->Add(this->lblDefinition);
			this->grpFunctionName->Controls->Add(this->txtLocation);
			this->grpFunctionName->Controls->Add(this->lblLocation);
			this->grpFunctionName->Controls->Add(this->txtName);
			this->grpFunctionName->Controls->Add(this->lblName);
			this->grpFunctionName->Location = System::Drawing::Point(8, 16);
			this->grpFunctionName->Name = S"grpFunctionName";
			this->grpFunctionName->Size = System::Drawing::Size(416, 136);
			this->grpFunctionName->TabIndex = 10;
			this->grpFunctionName->TabStop = false;
			this->grpFunctionName->Text = S"Function";
			// 
			// txtDefinition
			// 
			this->txtDefinition->Location = System::Drawing::Point(84, 88);
			this->txtDefinition->Multiline = true;
			this->txtDefinition->Name = S"txtDefinition";
			this->txtDefinition->ReadOnly = true;
			this->txtDefinition->Size = System::Drawing::Size(320, 40);
			this->txtDefinition->TabIndex = 9;
			this->txtDefinition->Text = S"";
			// 
			// lblDefinition
			// 
			this->lblDefinition->Location = System::Drawing::Point(20, 88);
			this->lblDefinition->Name = S"lblDefinition";
			this->lblDefinition->Size = System::Drawing::Size(64, 16);
			this->lblDefinition->TabIndex = 8;
			this->lblDefinition->Text = S"Definition";
			this->lblDefinition->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// txtLocation
			// 
			this->txtLocation->Location = System::Drawing::Point(84, 56);
			this->txtLocation->Name = S"txtLocation";
			this->txtLocation->ReadOnly = true;
			this->txtLocation->Size = System::Drawing::Size(320, 20);
			this->txtLocation->TabIndex = 7;
			this->txtLocation->Text = S"";
			// 
			// lblLocation
			// 
			this->lblLocation->Location = System::Drawing::Point(20, 56);
			this->lblLocation->Name = S"lblLocation";
			this->lblLocation->Size = System::Drawing::Size(64, 16);
			this->lblLocation->TabIndex = 6;
			this->lblLocation->Text = S"Location";
			this->lblLocation->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// txtName
			// 
			this->txtName->Location = System::Drawing::Point(84, 24);
			this->txtName->Name = S"txtName";
			this->txtName->ReadOnly = true;
			this->txtName->Size = System::Drawing::Size(320, 20);
			this->txtName->TabIndex = 5;
			this->txtName->Text = S"";
			// 
			// lblName
			// 
			this->lblName->Location = System::Drawing::Point(20, 24);
			this->lblName->Name = S"lblName";
			this->lblName->Size = System::Drawing::Size(64, 16);
			this->lblName->TabIndex = 4;
			this->lblName->Text = S"Name";
			this->lblName->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(256, 536);
			this->btnCancel->Name = S"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(80, 24);
			this->btnCancel->TabIndex = 1;
			this->btnCancel->Text = S"Cancel";
			// 
			// btnOK
			// 
			this->btnOK->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnOK->Location = System::Drawing::Point(344, 536);
			this->btnOK->Name = S"btnOK";
			this->btnOK->Size = System::Drawing::Size(80, 24);
			this->btnOK->TabIndex = 2;
			this->btnOK->Text = S"OK";
			// 
			// toolTip
			// 
			this->toolTip->AutomaticDelay = 200;
			// 
			// label1
			// 
			this->label1->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 14, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->label1->ForeColor = System::Drawing::Color::Red;
			this->label1->Location = System::Drawing::Point(12, 544);
			this->label1->Name = S"label1";
			this->label1->Size = System::Drawing::Size(8, 16);
			this->label1->TabIndex = 20;
			this->label1->Text = S"*";
			this->label1->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// lblStarInfo
			// 
			this->lblStarInfo->Location = System::Drawing::Point(20, 544);
			this->lblStarInfo->Name = S"lblStarInfo";
			this->lblStarInfo->Size = System::Drawing::Size(128, 16);
			this->lblStarInfo->TabIndex = 21;
			this->lblStarInfo->Text = S"indicates a required field";
			// 
			// EditFunctionDetails
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(434, 568);
			this->Controls->Add(this->lblStarInfo);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->btnOK);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->grpMain);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"EditFunctionDetails";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"Edit Function Details";
			this->grpMain->ResumeLayout(false);
			this->grpParamSpecification->ResumeLayout(false);
			this->panelParamButtons->ResumeLayout(false);
			this->grpTypeSepcification->ResumeLayout(false);
			this->grpFunctionName->ResumeLayout(false);
			this->ResumeLayout(false);

		}		
};
}