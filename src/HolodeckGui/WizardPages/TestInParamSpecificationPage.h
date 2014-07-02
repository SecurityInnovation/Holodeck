#pragma once

#include "defines.h"
#include "TestPatternBuilderDialog.h"

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
using namespace System::Drawing;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;

namespace HolodeckGui
{
	/// <summary> 
	/// Summary for TestInParamSpecificationPage
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
public __gc class TestInParamSpecificationPage : public WizardPage
	{
	public: 
		TestInParamSpecificationPage();

		__property String *get_Function() { return function->Name; }
		__property void set_Function(String *value);

		__property ArrayList *get_Parameters();
		__property void set_Parameters(ArrayList *value);

		__property void set_IsDotNetFunction(bool value) { isDotNetFunction = value; }
		
		
		void UpdateListView();
		void SetDoubleClickHandlerState(bool stateValue);
	
	private: EventHandler *  lvParams_DoubleClickHandler;
	private: ArrayList *  gParameters;
	private: System::Windows::Forms::ColumnHeader *  chOR;
	private: System::Windows::Forms::Button *  btnDelete;
	private: System::Windows::Forms::Button *  btnAddMatchCondition;

	protected: 
		
		InterceptedFunction * function;
		bool isDotNetFunction;

		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private: System::Windows::Forms::ColumnHeader *  chIndex;
	private: System::Windows::Forms::ListView *  lvParams;
	private: System::Windows::Forms::ColumnHeader *  chParamName;
	private: System::Windows::Forms::ColumnHeader *  chMatchCondition;
	private: System::Windows::Forms::Button *  btnModifyParamConditions;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->lvParams = new System::Windows::Forms::ListView();
			this->chIndex = new System::Windows::Forms::ColumnHeader();
			this->chParamName = new System::Windows::Forms::ColumnHeader();
			this->chOR = new System::Windows::Forms::ColumnHeader();
			this->chMatchCondition = new System::Windows::Forms::ColumnHeader();
			this->btnModifyParamConditions = new System::Windows::Forms::Button();
			this->btnDelete = new System::Windows::Forms::Button();
			this->btnAddMatchCondition = new System::Windows::Forms::Button();
			this->SuspendLayout();
			// 
			// lvParams
			// 
			System::Windows::Forms::ColumnHeader* __mcTemp__1[] = new System::Windows::Forms::ColumnHeader*[4];
			__mcTemp__1[0] = this->chIndex;
			__mcTemp__1[1] = this->chParamName;
			__mcTemp__1[2] = this->chOR;
			__mcTemp__1[3] = this->chMatchCondition;
			this->lvParams->Columns->AddRange(__mcTemp__1);
			this->lvParams->FullRowSelect = true;
			this->lvParams->GridLines = true;
			this->lvParams->Location = System::Drawing::Point(8, 8);
			this->lvParams->Name = S"lvParams";
			this->lvParams->Size = System::Drawing::Size(376, 192);
			this->lvParams->TabIndex = 0;
			this->lvParams->View = System::Windows::Forms::View::Details;
			this->lvParams->HideSelection = false;
			// 
			// chIndex
			// 
			this->chIndex->Text = S"Index";
			this->chIndex->Width = 38;
			// 
			// chParamName
			// 
			this->chParamName->Text = S"Param name";
			this->chParamName->Width = 90;
			// 
			// chOR
			// 
			this->chOR->Text = S" Match Type";
			this->chOR->Width = 74;
			// 
			// chMatchCondition
			// 
			this->chMatchCondition->Text = S"Match Condition";
			this->chMatchCondition->Width = 170;
			// 
			// btnModifyParamConditions
			// 
			this->btnModifyParamConditions->Location = System::Drawing::Point(104, 208);
			this->btnModifyParamConditions->Name = S"btnModifyParamConditions";
			this->btnModifyParamConditions->Size = System::Drawing::Size(88, 24);
			this->btnModifyParamConditions->TabIndex = 1;
			this->btnModifyParamConditions->Text = S"Edit";
			this->btnModifyParamConditions->Click += new System::EventHandler(this, btnModifyParamConditions_Click);
			// 
			// btnDelete
			// 
			this->btnDelete->Location = System::Drawing::Point(200, 208);
			this->btnDelete->Name = S"btnDelete";
			this->btnDelete->Size = System::Drawing::Size(88, 24);
			this->btnDelete->TabIndex = 2;
			this->btnDelete->Text = S"Delete";
			this->btnDelete->Click += new System::EventHandler(this, btnDelete_Click);
			// 
			// btnAddMatchCondition
			// 
			this->btnAddMatchCondition->Location = System::Drawing::Point(8, 208);
			this->btnAddMatchCondition->Name = S"btnAddMatchCondition";
			this->btnAddMatchCondition->Size = System::Drawing::Size(88, 24);
			this->btnAddMatchCondition->TabIndex = 3;
			this->btnAddMatchCondition->Text = S"Add";
			this->btnAddMatchCondition->Click += new System::EventHandler(this, btnAddMatchCondition_Click);
			// 
			// TestInParamSpecificationPage
			// 
			this->ClientSize = System::Drawing::Size(392, 241);
			this->Controls->Add(this->btnAddMatchCondition);
			this->Controls->Add(this->btnDelete);
			this->Controls->Add(this->btnModifyParamConditions);
			this->Controls->Add(this->lvParams);
			this->Name = S"TestInParamSpecificationPage";
			this->Text = S"TestInParamSpecificationPage";
			this->ResumeLayout(false);

		}		
	
		void btnModifyParamConditions_Click(System::Object *  sender, System::EventArgs *  e);
		void btnAddMatchCondition_Click(System::Object *  sender, System::EventArgs *  e);
		void btnDelete_Click(System::Object *  sender, System::EventArgs *  e);
		void lvParams_DoubleClickHandlerMethod(System::Object *  sender, System::EventArgs *  e);
	};
}