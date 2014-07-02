#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;

\
namespace BrokenApp
{
   
	/// <summary> 
	/// Summary for DateForm
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class DateForm : public System::Windows::Forms::Form
	{
	public: 
		DateForm()//BrokenApp::Form1 & f1)
		{
			InitializeComponent();
			//MessageBox::Show(this->Parent->Name, this->Parent->Name);
			//f1Ptr=f1;
		}
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}
   public: System::Windows::Forms::DateTimePicker * dateTimePicker1;

	

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container* components;

		//Form1 * f1Ptr;
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
         this->dateTimePicker1 = new System::Windows::Forms::DateTimePicker();
         this->SuspendLayout();
         // 
         // dateTimePicker1
         // 
         this->dateTimePicker1->Format = System::Windows::Forms::DateTimePickerFormat::Time;
         this->dateTimePicker1->Location = System::Drawing::Point(0, 8);
         this->dateTimePicker1->Name = S"dateTimePicker1";
         this->dateTimePicker1->Size = System::Drawing::Size(264, 20);
         this->dateTimePicker1->TabIndex = 0;
         this->dateTimePicker1->ValueChanged += new System::EventHandler(this, doDateChanged);
         // 
         // DateForm
         // 
         this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
         this->ClientSize = System::Drawing::Size(264, 30);
         this->Controls->Add(this->dateTimePicker1);
         this->Name = S"DateForm";
         this->Tag = S"g";
         this->Text = S"Choose Date to Insert";
         this->ResumeLayout(false);

      }		
   public: System::Void doDateChanged(System::Object *  sender, System::EventArgs *  e)
           {
              String * temp = dateTimePicker1->Text;
              //change the dateString in Form1 class.
//			  f1Ptr->changeDate(temp);

           }

   };
}