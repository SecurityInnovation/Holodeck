#pragma once


#include "Form1.h"

namespace BrokenApp
{
   using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
   using namespace BrokenApp;

public __gc class MultiFindForm : public System::Windows::Forms::Form
{
public:
   MultiFindForm(int determineFormType, String * findIt)
   {
      findStr=findIt;
      initFindForm(determineFormType);
   }

private: TextBox * findBox;
private: TextBox * replaceBox;
private: Form * findForm;
private: Button * findBut;
private: Button * findNextBut;
private: Button * replaceBut;
private: Windows::Forms::CheckBox * matchCaseCB;
private: Windows::Forms::CheckBox * wholeWordCB;
private: System::Windows::Forms::Label * findLabel;
private: System::Windows::Forms::Label * replaceLabel;
private: static String * findStr="";
private:
   bool isReplace;

		 bool isFindNext;
private: void initFindForm(int determineFormType)
   {
          //labels
          findLabel = new Label();
          findLabel->Location=System::Drawing::Point(0,0);
          findLabel->Text="Find:";
          findLabel->Visible=true;

          replaceLabel = new Label();
          replaceLabel->Location=System::Drawing::Point(0,75);
          replaceLabel->Text="Replace:";
          replaceLabel->Visible=true;

          //input from user for string to find.
			 findBox = new TextBox();
			 findBox->ClientSize = System::Drawing::Size(320,80);
          findBox->Location = System::Drawing::Point(25,0);
			 findBox->Multiline=false;

          //input from user for replace string
          replaceBox = new TextBox();
          replaceBox->ClientSize=System::Drawing::Size(320,80);
          replaceBox->Multiline=false;
          replaceBox->Location=System::Drawing::Point(25,75);
			 
          // form to simulate Find on the TextBox(mainTextbox)
          findForm = new Form();
			 findForm->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			 findForm->ClientSize = System::Drawing::Size(430, 130);
			 findForm->Name = S"FindForm1";
          switch (determineFormType)
          {
            case 10: findForm->Text = S"Find and Replace..."; break;
            case 11: findForm->Text = S"Find"; break;
            case 12: findForm->Text = S"Find Next:"; break;
          }
          findForm->MaximizeBox=false;
          findForm->MinimizeBox=false;
          findForm->TopMost=true;
          findForm->MaximumSize=findForm->ClientSize;
          findForm->MinimumSize=findForm->ClientSize;
          
          //find button
          findBut = new System::Windows::Forms::Button();
          findBut->Location = System::Drawing::Point(355,0);
          findBut->ClientSize = System::Drawing::Size(65,20);
          findBut->Visible=true;
          findBut->Enabled=true;
          findBut->Text="Find!";
          findBut->add_Click(new System::EventHandler (this, doFind));
          
          //find next button
          findNextBut = new System::Windows::Forms::Button();
          findNextBut->Location = System::Drawing::Point(355, 30);
          findNextBut->ClientSize = System::Drawing::Size(65,20);
          findNextBut->Visible=true;
          findNextBut->Enabled=true;
          findNextBut->Text="Find Next!";
          findNextBut->Click+= new System::EventHandler (this, doFindNext);

          //replace button
          replaceBut = new System::Windows::Forms::Button();
          replaceBut->Location = System::Drawing::Point(355, 75);
          replaceBut->ClientSize = System::Drawing::Size(65,20);
          replaceBut->Visible=true;
          replaceBut->Enabled=true;
          replaceBut->Text="Replace!";
          replaceBut->Click+= new System::EventHandler (this, doReplace);

          //option to match case for the finder.
          matchCaseCB = new CheckBox();
          matchCaseCB->Location= System::Drawing::Point(25,25);
          matchCaseCB->Text=S"Match Case";
          
          //option to match whole word for the finder.
          wholeWordCB = new CheckBox();
          wholeWordCB->Location = System::Drawing::Point(25, 50);
          wholeWordCB->Text=S"Match Whole Word";

          //add the control objects to the Find Form
          findForm->Controls->Add(findBut);   //find button
          findForm->Controls->Add(findNextBut);  //find next button
          findForm->Controls->Add(replaceBut); // replace button
			 findForm->Controls->Add(findBox);  //textbox to specify the text to find.
          findForm->Controls->Add(replaceBox); //textbox for string replacement.
          findForm->Controls->Add(matchCaseCB);   //match case option
          findForm->Controls->Add(wholeWordCB);   //match whole word option
          findForm->Controls->Add(findLabel);
          findForm->Controls->Add(replaceLabel);
          
          findForm->ResumeLayout(false);
          findForm->Show();
   }  
private: System::Void doFind(System::Object *  sender, System::EventArgs *  e)
       {
          findStr=BrokenApp::Form1::findString;
          
          
       }
private: System::Void doFindNext(System::Object *  sender, System::EventArgs *  e)
       {
       }
private: System::Void doReplace(System::Object *  sender, System::EventArgs *  e)
       {
       }
private: System::Void InitializeComponent()
		 {
		 }

};

}