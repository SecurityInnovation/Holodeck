#pragma once

#include "defines.h"
#include "ParameterType.h"
#include <windows.h>
#using <FunctionDatabase.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace FunctionDatabase;


namespace HolodeckGui
{
	/// <summary> 
	/// Summary for TestPatternBuilderDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class TestPatternBuilderDialog : public System::Windows::Forms::Form
	{
	public: 
		TestPatternBuilderDialog(bool dotNetFunction);
		ListViewItem * getUserInput (ListViewItem * lvi, int compareType);
		static String * getOperatorText (String * operatorSymbol);
		static String * getOperatorSymbol (String * operatorText);
	private: System::Windows::Forms::Label *  lblInfo;
	private: System::Windows::Forms::Label *  lblParamName;
	private: System::Windows::Forms::Label *  lblMatchType;
	private: System::Windows::Forms::Label *  lblValueToMatch;
			 EventHandler * textChangedEventHandler;
			 bool isDotNetFunction;
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private: System::Windows::Forms::ComboBox *  cbMatchType;

	private: System::Windows::Forms::Button *  btnAccept;
	private: System::Windows::Forms::Button *  btnCancel;
	private: System::Windows::Forms::TextBox *  tbParamName;
	private: System::Windows::Forms::ComboBox *  tbMatchValue;



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
			this->tbParamName = new System::Windows::Forms::TextBox();
			this->cbMatchType = new System::Windows::Forms::ComboBox();
			this->btnAccept = new System::Windows::Forms::Button();
			this->btnCancel = new System::Windows::Forms::Button();
			this->tbMatchValue = new System::Windows::Forms::ComboBox();
			this->lblInfo = new System::Windows::Forms::Label();
			this->lblParamName = new System::Windows::Forms::Label();
			this->lblMatchType = new System::Windows::Forms::Label();
			this->lblValueToMatch = new System::Windows::Forms::Label();
			this->SuspendLayout();
			// 
			// tbParamName
			// 
			this->tbParamName->Location = System::Drawing::Point(8, 16);
			this->tbParamName->Name = S"tbParamName";
			this->tbParamName->ReadOnly = true;
			this->tbParamName->Size = System::Drawing::Size(176, 20);
			this->tbParamName->TabIndex = 0;
			this->tbParamName->Text = S"";
			// 
			// cbMatchType
			// 
			this->cbMatchType->Location = System::Drawing::Point(192, 16);
			this->cbMatchType->Name = S"cbMatchType";
			this->cbMatchType->Size = System::Drawing::Size(168, 21);
			this->cbMatchType->TabIndex = 1;
			this->cbMatchType->SelectedIndexChanged += new System::EventHandler(this, cbMatchType_SelectedIndexChanged);
			// 
			// btnAccept
			// 
			this->btnAccept->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnAccept->Location = System::Drawing::Point(488, 48);
			this->btnAccept->Name = S"btnAccept";
			this->btnAccept->Size = System::Drawing::Size(96, 24);
			this->btnAccept->TabIndex = 3;
			this->btnAccept->Text = S"Accept";
			// 
			// btnCancel
			// 
			this->btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->btnCancel->Location = System::Drawing::Point(384, 48);
			this->btnCancel->Name = S"btnCancel";
			this->btnCancel->Size = System::Drawing::Size(96, 24);
			this->btnCancel->TabIndex = 4;
			this->btnCancel->Text = S"Cancel";
			// 
			// tbMatchValue
			// 
			System::Object* __mcTemp__1[] = new System::Object*[1];
			__mcTemp__1[0] = S"{Any}";
			this->tbMatchValue->Items->AddRange(__mcTemp__1);
			this->tbMatchValue->Location = System::Drawing::Point(368, 16);
			this->tbMatchValue->Name = S"tbMatchValue";
			this->tbMatchValue->Size = System::Drawing::Size(216, 21);
			this->tbMatchValue->TabIndex = 5;
			// 
			// lblInfo
			// 
			this->lblInfo->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 8.25F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->lblInfo->ForeColor = System::Drawing::Color::DarkRed;
			this->lblInfo->Location = System::Drawing::Point(6, 48);
			this->lblInfo->Name = S"lblInfo";
			this->lblInfo->Size = System::Drawing::Size(312, 24);
			this->lblInfo->TabIndex = 6;
			this->lblInfo->Text = S"The only match allowed with the value \'{Any}\' is \'Equal To\'";
			this->lblInfo->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			this->lblInfo->Visible = false;
			// 
			// lblParamName
			// 
			this->lblParamName->Location = System::Drawing::Point(8, 0);
			this->lblParamName->Name = S"lblParamName";
			this->lblParamName->Size = System::Drawing::Size(176, 16);
			this->lblParamName->TabIndex = 7;
			this->lblParamName->Text = S"Param Identifier:";
			this->lblParamName->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
			// 
			// lblMatchType
			// 
			this->lblMatchType->Location = System::Drawing::Point(192, 0);
			this->lblMatchType->Name = S"lblMatchType";
			this->lblMatchType->Size = System::Drawing::Size(120, 16);
			this->lblMatchType->TabIndex = 8;
			this->lblMatchType->Text = S"Match Type:";
			this->lblMatchType->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
			// 
			// lblValueToMatch
			// 
			this->lblValueToMatch->Location = System::Drawing::Point(368, 0);
			this->lblValueToMatch->Name = S"lblValueToMatch";
			this->lblValueToMatch->Size = System::Drawing::Size(192, 16);
			this->lblValueToMatch->TabIndex = 9;
			this->lblValueToMatch->Text = S"Value to Match:";
			this->lblValueToMatch->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
			// 
			// TestPatternBuilderDialog
			// 
			this->AcceptButton = this->btnAccept;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->CancelButton = this->btnCancel;
			this->ClientSize = System::Drawing::Size(594, 80);
			this->Controls->Add(this->lblValueToMatch);
			this->Controls->Add(this->lblMatchType);
			this->Controls->Add(this->lblParamName);
			this->Controls->Add(this->lblInfo);
			this->Controls->Add(this->tbMatchValue);
			this->Controls->Add(this->btnCancel);
			this->Controls->Add(this->btnAccept);
			this->Controls->Add(this->cbMatchType);
			this->Controls->Add(this->tbParamName);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"TestPatternBuilderDialog";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"TestPatternBuilderDialog";
			this->TopMost = true;
			this->ResumeLayout(false);

		}		

		private: System::Void cbMatchType_SelectedIndexChanged(System::Object *  sender, System::EventArgs *  e);
		private: System::Void tbMatchValue_TextChanged(System::Object *  sender, System::EventArgs *  e);

};
}