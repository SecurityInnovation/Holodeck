//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LaunchProgressDialog.h
//
// DESCRIPTION: Contains definition for the class LaunchProgressDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 JAN 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

#undef GetObject

namespace HolodeckGui
{
	/// <summary> 
	/// Summary for LaunchProgressDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class LaunchProgressDialog : public System::Windows::Forms::Form
	{
	public: 
		LaunchProgressDialog(void)
		{
			InitializeComponent();

			closeHandler = new CancelEventHandler(this, closeHandlerFunc);
		}

		void ShowDialogCustom()
		{
			//Do not allow Alt+F4 or any other form of close
			this->Closing += closeHandler;
			this->ShowDialog();
		}

		void CloseDialog()
		{
			this->Closing -= closeHandler;
			this->Close();
			
		}

		__property void set_UserText(String *value) { waitLabel->Text = value; }
		__property String *get_UserText() { return waitLabel->Text; }
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
	private: System::Windows::Forms::Label *  waitLabel;

	private:
		CancelEventHandler * closeHandler;
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
			System::Resources::ResourceManager *  resources = new System::Resources::ResourceManager(__typeof(HolodeckGui::LaunchProgressDialog));
			this->waitLabel = new System::Windows::Forms::Label();
			this->SuspendLayout();
			// 
			// waitLabel
			// 
			this->waitLabel->Dock = System::Windows::Forms::DockStyle::Fill;
			this->waitLabel->Font = new System::Drawing::Font(S"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)0);
			this->waitLabel->Location = System::Drawing::Point(0, 0);
			this->waitLabel->Name = S"waitLabel";
			this->waitLabel->Size = System::Drawing::Size(470, 44);
			this->waitLabel->TabIndex = 0;
			this->waitLabel->Text = S"Holodeck is launching the specified application.  Please wait...";
			this->waitLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
			// 
			// LaunchProgressDialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(470, 44);
			this->ControlBox = false;
			this->Controls->Add(this->waitLabel);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedToolWindow;
			this->Icon = (__try_cast<System::Drawing::Icon *  >(resources->GetObject(S"$this.Icon")));
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = S"LaunchProgressDialog";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = S"Please Wait...";
			this->ResumeLayout(false);

		}		

		void closeHandlerFunc (Object * sender, CancelEventArgs * e)
		{
			e->Cancel = true;
		}
	};
}