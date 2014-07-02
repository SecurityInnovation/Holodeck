#pragma once

#define REPLACEF  10 //Replace form
#define FINDF     11 //Find form
#define FINDNEXTF 12 //FindNext form

//#include <afxwin.h>
//#include <afxext.h>
#include <windows.h>
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <malloc.h>

#undef MessageBox

namespace BrokenApp
{
	using namespace Microsoft::Win32;
	using namespace std;
	using namespace System;
	using namespace System::Net;
	using namespace System::Text;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary> 
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
public __gc class Form1 : public System::Windows::Forms::Form
{	
public:
	Form1(void)
	{
		InitializeComponent();

		//registry functionality
		doStartupCheck();
		//set the font
		//      mainTextbox->Font=mainFont;

	}
	// manipulate the Win32 registry to have the values for 
	// number of times the application has been used and 
	// the version number.
public: void doStartupCheck() {
			RegistryKey* aboutInfo = Registry::LocalMachine->OpenSubKey(S"SOFTWARE\\BrokenApp");

			if (aboutInfo == NULL) {
				aboutInfo = Registry::LocalMachine->CreateSubKey(S"SOFTWARE\\BrokenApp");
				aboutInfo->SetValue(S"BrokenAppVersion", S"1.0");
				aboutInfo->SetValue(S"BrokenAppUsage", S"1");
			}

			else {
				aboutInfo = Registry::LocalMachine->OpenSubKey(S"SOFTWARE\\BrokenApp", true);

				timesUsed = System::Convert::ToInt32(aboutInfo->GetValue(S"BrokenAppUsage"));
				timesUsed++;
				aboutInfo->SetValue(S"BrokenAppUsage", System::Convert::ToString(timesUsed));

			}

			versionNo = System::Convert::ToString(aboutInfo->GetValue(S"BrokenAppVersion"));
			timesUsed = System::Convert::ToInt32(aboutInfo->GetValue(S"BrokenAppUsage"));
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

	//used in Find form
private: TextBox * findBox;
private: Form * findForm;
private: Button * findBut;
private: Button * findNextBut;
private: Windows::Forms::CheckBox * matchCaseCB;
private: Windows::Forms::CheckBox * wholeWordCB;
private: System::Windows::Forms::Label * findLabel;
		 //used in Calender Form
private: Form * calForm;
private: DateTimePicker * dtp1;  //for dates
private: DateTimePicker * dtp2;  //for time
private: Button * insertDateBut; //for inserting date
private: Button * insertTimeBut;  //for inserting time
		 //used in URL form
private: Form * URLForm;
private: Button * urlOK;
private: TextBox * urlBox;

		 // used in Form1
private: System::Windows::Forms::RichTextBox *  mainTextbox;
private: System::Windows::Forms::MainMenu *  mainMenu1;
private: System::Windows::Forms::MenuItem *  fileMenu;
private: System::Windows::Forms::MenuItem *  editMenu;
private: System::Windows::Forms::MenuItem *  insertMenu;
private: System::Windows::Forms::MenuItem *  FormatMenu;
private: System::Windows::Forms::MenuItem *  helpMenu;
private: System::Windows::Forms::MenuItem *  aboutMenuItem;
private: System::Windows::Forms::MenuItem *  menuItem1;
private: System::Windows::Forms::MenuItem *  menuItem2;
private: System::Windows::Forms::MenuItem *  menuItem3;
private: System::Windows::Forms::MenuItem *  menuItem4;
private: System::Windows::Forms::MenuItem *  menuItem5;


private: System::Windows::Forms::MenuItem *  menuItem8;
private: System::Windows::Forms::MenuItem *  menuItem9;
private: System::Windows::Forms::MenuItem *  menuItem10;
private: System::Windows::Forms::MenuItem *  menuItem11;
private: System::Windows::Forms::MenuItem *  menuItem12;
private: System::Windows::Forms::MenuItem *  menuItem13;
private: System::Windows::Forms::MenuItem *  menuItem14;
private: System::Windows::Forms::MenuItem *  menuItem15;
private: System::Windows::Forms::MenuItem *  menuItem16;
private: System::Windows::Forms::MenuItem *  menuItem17;
private: System::Windows::Forms::MenuItem *  menuItem18;
private: System::Windows::Forms::MenuItem *  menuItem19;
private: System::Windows::Forms::MenuItem *  menuItem20;
private: System::Windows::Forms::MenuItem *  menuItem21;

private: System::Windows::Forms::MenuItem *  menuItem23;
private: System::Windows::Forms::MenuItem *  menuItem24;
private: System::Windows::Forms::MenuItem *  menuItem25;
private: System::Windows::Forms::MenuItem *  menuItem26;





private: System::Windows::Forms::MenuItem *  menuItem32;

private: System::Windows::Forms::MenuItem *  menuItem34;
private: System::Windows::Forms::MenuItem *  menuItem35;
private: System::Windows::Forms::MenuItem *  menuItem36;
private: System::Windows::Forms::MenuItem *  menuItem37;
private: System::Windows::Forms::MenuItem *  menuItem38;
private: System::Windows::Forms::MenuItem *  menuItem39;

private: System::Windows::Forms::MenuItem *  menuItem41;
private: System::Windows::Forms::MenuItem *  menuItem42;
private: System::Windows::Forms::MenuItem *  menuItem43;

public : String * dateString;
private: String * richTBSelectedText;
private: static String * fileName = S"";
private: static String * findString;
private: static String * filterFiles = S"Text Files (*.txt)|*.txt|HTML Files (*.html,.htm)|*.htm,.*html|All Files (*.*)|*.*";
private: System::Windows::Forms::SaveFileDialog *  saveFileDialog1;
private: System::Windows::Forms::FontDialog *  fontDialog1;
private: System::Windows::Forms::ColorDialog *  colorDialog1;
private: System::Windows::Forms::MenuItem *  menuItem27;
private: System::Windows::Forms::MenuItem *  menuItem28;
private: System::Windows::Forms::MenuItem *  menuItem29;


private: int timesUsed;
private: String * versionNo;
private: WebRequest * webR;
private: WebResponse * webResp;
private: System::Windows::Forms::MenuItem *  menuItem30;
private: System::Windows::Forms::MenuItem *  menuItem31;
private: System::Windows::Forms::MenuItem *  menuItem7;
private: System::Windows::Forms::PrintDialog *  pDlg;
private: System::Drawing::Printing::PrintDocument *  printDoc1;
public : System::Diagnostics::Process * p;

private:
	/// <summary>
	/// Required designer variable.
	/// </summary>
	System::ComponentModel::Container * components;

	/// <summary>
	/// Required method for Designer support - do not modify
	/// the contents of this method with the code editor.
	/// </summary>
	void InitializeComponent(void)
	{
		this->mainTextbox = new System::Windows::Forms::RichTextBox();
		this->mainMenu1 = new System::Windows::Forms::MainMenu();
		this->fileMenu = new System::Windows::Forms::MenuItem();
		this->menuItem1 = new System::Windows::Forms::MenuItem();
		this->menuItem2 = new System::Windows::Forms::MenuItem();
		this->menuItem30 = new System::Windows::Forms::MenuItem();
		this->menuItem3 = new System::Windows::Forms::MenuItem();
		this->menuItem4 = new System::Windows::Forms::MenuItem();
		this->menuItem5 = new System::Windows::Forms::MenuItem();
		this->menuItem7 = new System::Windows::Forms::MenuItem();
		this->menuItem8 = new System::Windows::Forms::MenuItem();
		this->menuItem9 = new System::Windows::Forms::MenuItem();
		this->editMenu = new System::Windows::Forms::MenuItem();
		this->menuItem10 = new System::Windows::Forms::MenuItem();
		this->menuItem11 = new System::Windows::Forms::MenuItem();
		this->menuItem12 = new System::Windows::Forms::MenuItem();
		this->menuItem13 = new System::Windows::Forms::MenuItem();
		this->menuItem14 = new System::Windows::Forms::MenuItem();
		this->menuItem15 = new System::Windows::Forms::MenuItem();
		this->menuItem16 = new System::Windows::Forms::MenuItem();
		this->menuItem17 = new System::Windows::Forms::MenuItem();
		this->menuItem18 = new System::Windows::Forms::MenuItem();
		this->menuItem19 = new System::Windows::Forms::MenuItem();
		this->menuItem20 = new System::Windows::Forms::MenuItem();
		this->menuItem21 = new System::Windows::Forms::MenuItem();
		this->insertMenu = new System::Windows::Forms::MenuItem();
		this->menuItem23 = new System::Windows::Forms::MenuItem();
		this->menuItem24 = new System::Windows::Forms::MenuItem();
		this->menuItem25 = new System::Windows::Forms::MenuItem();
		this->FormatMenu = new System::Windows::Forms::MenuItem();
		this->menuItem29 = new System::Windows::Forms::MenuItem();
		this->menuItem28 = new System::Windows::Forms::MenuItem();
		this->menuItem31 = new System::Windows::Forms::MenuItem();
		this->menuItem27 = new System::Windows::Forms::MenuItem();
		this->menuItem26 = new System::Windows::Forms::MenuItem();
		this->menuItem32 = new System::Windows::Forms::MenuItem();
		this->menuItem34 = new System::Windows::Forms::MenuItem();
		this->menuItem35 = new System::Windows::Forms::MenuItem();
		this->menuItem36 = new System::Windows::Forms::MenuItem();
		this->menuItem37 = new System::Windows::Forms::MenuItem();
		this->menuItem38 = new System::Windows::Forms::MenuItem();
		this->menuItem39 = new System::Windows::Forms::MenuItem();
		this->menuItem41 = new System::Windows::Forms::MenuItem();
		this->menuItem42 = new System::Windows::Forms::MenuItem();
		this->menuItem43 = new System::Windows::Forms::MenuItem();
		this->helpMenu = new System::Windows::Forms::MenuItem();
		this->aboutMenuItem = new System::Windows::Forms::MenuItem();
		this->saveFileDialog1 = new System::Windows::Forms::SaveFileDialog();
		this->fontDialog1 = new System::Windows::Forms::FontDialog();
		this->colorDialog1 = new System::Windows::Forms::ColorDialog();
		this->pDlg = new System::Windows::Forms::PrintDialog();
		this->printDoc1 = new System::Drawing::Printing::PrintDocument();
		this->SuspendLayout();
		// 
		// mainTextbox
		// 
		this->mainTextbox->AcceptsTab = true;
		this->mainTextbox->Dock = System::Windows::Forms::DockStyle::Fill;
		this->mainTextbox->Location = System::Drawing::Point(0, 0);
		this->mainTextbox->Name = S"mainTextbox";
		this->mainTextbox->Size = System::Drawing::Size(792, 566);
		this->mainTextbox->TabIndex = 0;
		this->mainTextbox->Text = S"";
		this->mainTextbox->LinkClicked += new System::Windows::Forms::LinkClickedEventHandler(this, doLinkClicked);
		// 
		// mainMenu1
		// 
		System::Windows::Forms::MenuItem* __mcTemp__1[] = new System::Windows::Forms::MenuItem*[5];
		__mcTemp__1[0] = this->fileMenu;
		__mcTemp__1[1] = this->editMenu;
		__mcTemp__1[2] = this->insertMenu;
		__mcTemp__1[3] = this->FormatMenu;
		__mcTemp__1[4] = this->helpMenu;
		this->mainMenu1->MenuItems->AddRange(__mcTemp__1);
		// 
		// fileMenu
		// 
		this->fileMenu->Index = 0;
		System::Windows::Forms::MenuItem* __mcTemp__2[] = new System::Windows::Forms::MenuItem*[9];
		__mcTemp__2[0] = this->menuItem1;
		__mcTemp__2[1] = this->menuItem2;
		__mcTemp__2[2] = this->menuItem30;
		__mcTemp__2[3] = this->menuItem3;
		__mcTemp__2[4] = this->menuItem4;
		__mcTemp__2[5] = this->menuItem5;
		__mcTemp__2[6] = this->menuItem7;
		__mcTemp__2[7] = this->menuItem8;
		__mcTemp__2[8] = this->menuItem9;
		this->fileMenu->MenuItems->AddRange(__mcTemp__2);
		this->fileMenu->Text = S"&File";
		// 
		// menuItem1
		// 
		this->menuItem1->Index = 0;
		this->menuItem1->Shortcut = System::Windows::Forms::Shortcut::CtrlN;
		this->menuItem1->Text = S"&New";
		this->menuItem1->Click += new System::EventHandler(this, showNewItem);
		// 
		// menuItem2
		// 
		this->menuItem2->Index = 1;
		this->menuItem2->Shortcut = System::Windows::Forms::Shortcut::CtrlO;
		this->menuItem2->Text = S"&Open";
		this->menuItem2->Click += new System::EventHandler(this, showOpenItem);
		// 
		// menuItem30
		// 
		this->menuItem30->Index = 2;
		this->menuItem30->Text = S"Open &URL...";
		this->menuItem30->Click += new System::EventHandler(this, showOpenURL);
		// 
		// menuItem3
		// 
		this->menuItem3->Index = 3;
		this->menuItem3->Shortcut = System::Windows::Forms::Shortcut::CtrlS;
		this->menuItem3->Text = S"&Save";
		this->menuItem3->Click += new System::EventHandler(this, showSaveItem);
		// 
		// menuItem4
		// 
		this->menuItem4->Index = 4;
		this->menuItem4->Text = S"Save &As...";
		this->menuItem4->Click += new System::EventHandler(this, showSaveAsItem);
		// 
		// menuItem5
		// 
		this->menuItem5->Index = 5;
		this->menuItem5->Text = S"-";
		// 
		// menuItem7
		// 
		this->menuItem7->Index = 6;
		this->menuItem7->Shortcut = System::Windows::Forms::Shortcut::CtrlP;
		this->menuItem7->Text = S"&Print...";
		this->menuItem7->Click += new System::EventHandler(this, showPrintItem);
		// 
		// menuItem8
		// 
		this->menuItem8->Index = 7;
		this->menuItem8->Text = S"-";
		// 
		// menuItem9
		// 
		this->menuItem9->Index = 8;
		this->menuItem9->Text = S"E&xit";
		this->menuItem9->Click += new System::EventHandler(this, showFileExitItem);
		// 
		// editMenu
		// 
		this->editMenu->Index = 1;
		System::Windows::Forms::MenuItem* __mcTemp__3[] = new System::Windows::Forms::MenuItem*[12];
		__mcTemp__3[0] = this->menuItem10;
		__mcTemp__3[1] = this->menuItem11;
		__mcTemp__3[2] = this->menuItem12;
		__mcTemp__3[3] = this->menuItem13;
		__mcTemp__3[4] = this->menuItem14;
		__mcTemp__3[5] = this->menuItem15;
		__mcTemp__3[6] = this->menuItem16;
		__mcTemp__3[7] = this->menuItem17;
		__mcTemp__3[8] = this->menuItem18;
		__mcTemp__3[9] = this->menuItem19;
		__mcTemp__3[10] = this->menuItem20;
		__mcTemp__3[11] = this->menuItem21;
		this->editMenu->MenuItems->AddRange(__mcTemp__3);
		this->editMenu->Text = S"&Edit";
		// 
		// menuItem10
		// 
		this->menuItem10->Index = 0;
		this->menuItem10->Shortcut = System::Windows::Forms::Shortcut::CtrlZ;
		this->menuItem10->Text = S"&Undo";
		this->menuItem10->Click += new System::EventHandler(this, doUndoAction);
		// 
		// menuItem11
		// 
		this->menuItem11->Index = 1;
		this->menuItem11->Shortcut = System::Windows::Forms::Shortcut::CtrlY;
		this->menuItem11->Text = S"Red&o";
		this->menuItem11->Click += new System::EventHandler(this, doRedoAction);
		// 
		// menuItem12
		// 
		this->menuItem12->Index = 2;
		this->menuItem12->Text = S"-";
		// 
		// menuItem13
		// 
		this->menuItem13->Index = 3;
		this->menuItem13->Shortcut = System::Windows::Forms::Shortcut::CtrlX;
		this->menuItem13->Text = S"Cu&t";
		this->menuItem13->Click += new System::EventHandler(this, doCutAction);
		// 
		// menuItem14
		// 
		this->menuItem14->Index = 4;
		this->menuItem14->Shortcut = System::Windows::Forms::Shortcut::CtrlC;
		this->menuItem14->Text = S"&Copy";
		this->menuItem14->Click += new System::EventHandler(this, doCopyAction);
		// 
		// menuItem15
		// 
		this->menuItem15->Index = 5;
		this->menuItem15->Shortcut = System::Windows::Forms::Shortcut::CtrlV;
		this->menuItem15->Text = S"&Paste";
		this->menuItem15->Click += new System::EventHandler(this, doPasteAction);
		// 
		// menuItem16
		// 
		this->menuItem16->Index = 6;
		this->menuItem16->Shortcut = System::Windows::Forms::Shortcut::Del;
		this->menuItem16->Text = S"&Delete";
		this->menuItem16->Click += new System::EventHandler(this, doDeleteAction);
		// 
		// menuItem17
		// 
		this->menuItem17->Index = 7;
		this->menuItem17->Text = S"-";
		// 
		// menuItem18
		// 
		this->menuItem18->Index = 8;
		this->menuItem18->Shortcut = System::Windows::Forms::Shortcut::CtrlA;
		this->menuItem18->Text = S"Select &All";
		this->menuItem18->Click += new System::EventHandler(this, doSelectAllAction);
		// 
		// menuItem19
		// 
		this->menuItem19->Index = 9;
		this->menuItem19->Text = S"-";
		// 
		// menuItem20
		// 
		this->menuItem20->Index = 10;
		this->menuItem20->Text = S"&Find...";
		this->menuItem20->Click += new System::EventHandler(this, doFindFNextReplace);
		// 
		// menuItem21
		// 
		this->menuItem21->Index = 11;
		this->menuItem21->Shortcut = System::Windows::Forms::Shortcut::F3;
		this->menuItem21->Text = S"Find &Next";
		this->menuItem21->Click += new System::EventHandler(this, doFindFNextReplace);
		// 
		// insertMenu
		// 
		this->insertMenu->Index = 2;
		System::Windows::Forms::MenuItem* __mcTemp__4[] = new System::Windows::Forms::MenuItem*[3];
		__mcTemp__4[0] = this->menuItem23;
		__mcTemp__4[1] = this->menuItem24;
		__mcTemp__4[2] = this->menuItem25;
		this->insertMenu->MenuItems->AddRange(__mcTemp__4);
		this->insertMenu->Text = S"&Insert";
		// 
		// menuItem23
		// 
		this->menuItem23->Index = 0;
		this->menuItem23->Text = S"Insert &Hyperlink";
		// 
		// menuItem24
		// 
		this->menuItem24->Index = 1;
		this->menuItem24->Text = S"Insert &Picture...";
		this->menuItem24->Click += new System::EventHandler(this, doInsertPicture);
		// 
		// menuItem25
		// 
		this->menuItem25->Index = 2;
		this->menuItem25->Text = S"Insert &Date and Time...";
		this->menuItem25->Click += new System::EventHandler(this, showDateForm);
		// 
		// FormatMenu
		// 
		this->FormatMenu->Index = 3;
		System::Windows::Forms::MenuItem* __mcTemp__5[] = new System::Windows::Forms::MenuItem*[15];
		__mcTemp__5[0] = this->menuItem29;
		__mcTemp__5[1] = this->menuItem28;
		__mcTemp__5[2] = this->menuItem31;
		__mcTemp__5[3] = this->menuItem27;
		__mcTemp__5[4] = this->menuItem26;
		__mcTemp__5[5] = this->menuItem32;
		__mcTemp__5[6] = this->menuItem34;
		__mcTemp__5[7] = this->menuItem35;
		__mcTemp__5[8] = this->menuItem36;
		__mcTemp__5[9] = this->menuItem37;
		__mcTemp__5[10] = this->menuItem38;
		__mcTemp__5[11] = this->menuItem39;
		__mcTemp__5[12] = this->menuItem41;
		__mcTemp__5[13] = this->menuItem42;
		__mcTemp__5[14] = this->menuItem43;
		this->FormatMenu->MenuItems->AddRange(__mcTemp__5);
		this->FormatMenu->Text = S"F&ormat";
		// 
		// menuItem29
		// 
		this->menuItem29->Index = 0;
		this->menuItem29->RadioCheck = true;
		this->menuItem29->Shortcut = System::Windows::Forms::Shortcut::CtrlB;
		this->menuItem29->Text = S"Bold";
		this->menuItem29->Click += new System::EventHandler(this, doBold);
		// 
		// menuItem28
		// 
		this->menuItem28->Index = 1;
		this->menuItem28->RadioCheck = true;
		this->menuItem28->Shortcut = System::Windows::Forms::Shortcut::CtrlI;
		this->menuItem28->Text = S"Italic";
		this->menuItem28->Click += new System::EventHandler(this, doItalic);
		// 
		// menuItem31
		// 
		this->menuItem31->Index = 2;
		this->menuItem31->Text = S"Strikeout";
		this->menuItem31->Click += new System::EventHandler(this, doStrike);
		// 
		// menuItem27
		// 
		this->menuItem27->Index = 3;
		this->menuItem27->RadioCheck = true;
		this->menuItem27->Shortcut = System::Windows::Forms::Shortcut::CtrlU;
		this->menuItem27->Text = S"Underline";
		this->menuItem27->Click += new System::EventHandler(this, doUnderline);
		// 
		// menuItem26
		// 
		this->menuItem26->Index = 4;
		this->menuItem26->Text = S"&Font";
		this->menuItem26->Click += new System::EventHandler(this, showFontDialog1);
		// 
		// menuItem32
		// 
		this->menuItem32->Index = 5;
		this->menuItem32->Text = S"-";
		// 
		// menuItem34
		// 
		this->menuItem34->Index = 6;
		this->menuItem34->Text = S"C&olor";
		this->menuItem34->Click += new System::EventHandler(this, showColour);
		// 
		// menuItem35
		// 
		this->menuItem35->Index = 7;
		this->menuItem35->Text = S"-";
		// 
		// menuItem36
		// 
		this->menuItem36->Index = 8;
		this->menuItem36->Text = S"Align &Left";
		this->menuItem36->Click += new System::EventHandler(this, doAlignLeft);
		// 
		// menuItem37
		// 
		this->menuItem37->Index = 9;
		this->menuItem37->Text = S"Align &Center";
		this->menuItem37->Click += new System::EventHandler(this, doAlignCenter);
		// 
		// menuItem38
		// 
		this->menuItem38->Index = 10;
		this->menuItem38->Text = S"Align &Right";
		this->menuItem38->Click += new System::EventHandler(this, doAligntRight);
		// 
		// menuItem39
		// 
		this->menuItem39->Index = 11;
		this->menuItem39->Text = S"-";
		// 
		// menuItem41
		// 
		this->menuItem41->Index = 12;
		this->menuItem41->Text = S"Bullets";
		this->menuItem41->Click += new System::EventHandler(this, doBullets);
		// 
		// menuItem42
		// 
		this->menuItem42->Index = 13;
		this->menuItem42->Text = S"Unindent";
		// 
		// menuItem43
		// 
		this->menuItem43->Index = 14;
		this->menuItem43->Text = S"Indent";
		this->menuItem43->Click += new System::EventHandler(this, doIndent);
		// 
		// helpMenu
		// 
		this->helpMenu->Index = 4;
		System::Windows::Forms::MenuItem* __mcTemp__6[] = new System::Windows::Forms::MenuItem*[1];
		__mcTemp__6[0] = this->aboutMenuItem;
		this->helpMenu->MenuItems->AddRange(__mcTemp__6);
		this->helpMenu->Text = S"&Help";
		// 
		// aboutMenuItem
		// 
		this->aboutMenuItem->Index = 0;
		this->aboutMenuItem->Text = S"&About BrokenApp...";
		this->aboutMenuItem->Click += new System::EventHandler(this, showAbout);
		// 
		// saveFileDialog1
		// 
		this->saveFileDialog1->CheckPathExists = false;
		this->saveFileDialog1->Filter = S"\"Text Files (*.txt)|*.txt|HTML Files (*.html,.htm)|*.htm,.*html|All Files (*.*)|*" 
			S".*\"";
		this->saveFileDialog1->Title = S"Save File";
		this->saveFileDialog1->FileOk += new System::ComponentModel::CancelEventHandler(this, doSaveFile);
		// 
		// fontDialog1
		// 
		this->fontDialog1->ShowColor = true;
		this->fontDialog1->ShowEffects = false;
		// 
		// pDlg
		// 
		this->pDlg->AllowSelection = true;
		// 
		// Form1
		// 
		this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
		this->ClientSize = System::Drawing::Size(792, 566);
		this->Controls->Add(this->mainTextbox);
		this->Menu = this->mainMenu1;
		this->Name = S"Form1";
		this->Text = S"BrokenApp for Demo of Holodeck";
		this->ResumeLayout(false);

	}	






	//Heap Overruns code from MSDN, modified to our needs for overrun.
private: void Form1::doNonGoodThings(const char * input1, const char * input2) 
		 {

			 //do misc. memory allocation
			 malloc(65536);
			 char * buf = NULL;
			 char * buf2;
			 char * dummyBuf;

			 buf2 = (char*)malloc(16);
			 buf = (char*)malloc(16);

			 dummyBuf=buf2;

			 strcpy(buf, input1);

			 strcpy(dummyBuf,input2);

			 if(buf != NULL)
				 free(buf);

		 }
		 //heap Overruns code from MSDN.
private: void doOffsetting(){
			 char arg1[128];

			 char arg2[4] = {0x0f, 0x10, 0x40, 0};    
			 int offset = 0x40;

			 memset(arg1, 0xfd, offset);

			 arg1[offset]   = (char)0x94;
			 arg1[offset+1] = (char)0xfe;
			 arg1[offset+2] = (char)0x12;
			 arg1[offset+3] = 0;
			 arg1[offset+4] = 0;

			 doNonGoodThings(arg1,arg2);
		 }
private: System::Void showAbout(System::Object *  sender, System::EventArgs *  e)
		 {
			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }
			 String * aboutText="Broken App: Holodeck Demo Application \nVersion Info:";
			 aboutText = String::Concat(aboutText, versionNo);
			 aboutText = String::Concat(aboutText, S"\nBrokenApp has been used: ");
			 aboutText = String::Concat(aboutText, System::Convert::ToString(timesUsed));
			 aboutText = String::Concat(aboutText, S" times.\nDeveloped by the Holodeck Testing Team.");
			 String * aboutTitle="Information about BrokenApp";
			 System::Windows::Forms::MessageBox::Show(aboutText,aboutTitle);
		 }

private: System::Void checkTextModified(System::Object *  sender)
		 {
			 bool isNew      = false;
			 bool isOpen     = false;
			 bool isOpenURL  = false;
			 bool isExit     = false;

			 System::Windows::Forms::MenuItem * mI = (System::Windows::Forms::MenuItem*) sender;

			 if(mI->Equals(menuItem1))
			 {
				 isNew=true;
			 }
			 if(mI->Equals(menuItem2))
			 {
				 isOpen=true;
			 }
			 if(mI->Equals(menuItem30))
			 {
				 isOpenURL=true;
			 }
			 if(mI->Equals(menuItem9))
			 {
				 isExit=true;
			 }

			 if (mainTextbox->Modified) 
			 {
				 switch(System::Windows::Forms::MessageBox::Show(S"File has been modified!\n\nWould you like to save changes?", S"BrokenApp",MessageBoxButtons::YesNoCancel))
				 {
				 case DialogResult::Cancel: break;
				 case DialogResult::Yes: 
					 this->showSaveItem(NULL,NULL);
					 if(isNew)
					 {
						 fileName="";
					 }
					 else if(isOpen)
					 {
						 this->doOpenFile();
					 }
					 else if(isOpenURL)
					 {
						 this->doOpenURL(NULL,NULL);
					 }
					 mainTextbox->Clear(); 
					 break;
				 case DialogResult::No: 
					 mainTextbox->Clear();
					 if (isNew)
					 {
						 this->doNewFile();
					 }
					 else if (isOpen)
					 {
						 this->doOpenFile();
					 }
					 else if (isOpenURL)
					 {
						 this->doOpenURL(NULL,NULL);
					 }
					 break;
				 default : 
					 if(isOpenURL)
					 {
						 this->doOpenURL(NULL,NULL);
					 }
					 break;
				 }
			 }
			 else if (!mainTextbox->Modified)
			 {
				 if (isOpenURL)
				 {
					 this->doOpenURL(NULL,NULL);
				 }
				 else if (isOpen)
				 {
					 this->doOpenFile();
				 }
			 }
		 }
private: System::Void showNewItem(System::Object *  sender, System::EventArgs *  e)
		 {
			 checkTextModified(sender);
		 }
private: System::Void doNewFile()
		 {
			 mainTextbox->Clear();
			 fileName="";
			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }
		 }
private: System::Void showOpenItem(System::Object *  sender, System::EventArgs *  e)
		 {
			 fileName="";
			 OpenFileDialog * opf= new OpenFileDialog();
			 //setup filters for text files or all files
			 opf->Filter = filterFiles;
			 opf->DefaultExt = S"txt";
			 opf->CheckPathExists=false;
			 opf->CheckFileExists=false;
			 int responseReturn = opf->ShowDialog();
			 fileName=opf->FileName;
			 String * appName="BrokenApp";

			 if (responseReturn == DialogResult::OK)
			 {
				 this->checkTextModified(sender);
			 }
		 }
private: System::Void doOpenFile()
		 {
			 int fileNameLength=fileName->Length;

			 //convert the managed String type of file name into 
			 //flat C char to use flat C fstream to read and write to file.
			 char * fileNCharArr;
			 fileNCharArr= new char[fileNameLength];

			 int i=0;
			 for (;i<fileNameLength;i++)
			 {
				 __wchar_t temp=fileName->get_Chars(i);
				 fileNCharArr[i]=temp;
			 }
			 fileNCharArr[i]='\0';

			 HANDLE hFile = CreateFile( fileNCharArr,
				 GENERIC_READ,
				 0,
				 NULL,
				 OPEN_EXISTING,
				 FILE_ATTRIBUTE_NORMAL,
				 NULL );

			 if ( ( hFile == INVALID_HANDLE_VALUE ) || ( GetLastError( ) != ERROR_SUCCESS ) )
			 {
				 System::Windows::Forms::MessageBox::Show( S"File could not be opened.", S"Error opening File." );

				 return;
			 }

			 char * charBuffer;


			 DWORD dwBytesRead;
			 do
			 {
				 charBuffer = new char[4096];
				 if(ReadFile(hFile,(LPVOID) charBuffer,4096,&dwBytesRead,NULL))
				 {
					 String * insertStr =new String (charBuffer);
					 if (insertStr)
					 {
						 this->mainTextbox->AppendText (insertStr);
					 }
				 }
			 } while (dwBytesRead ==4096);

			 int z =12;
			 while(z<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 z--;
			 }
			 CloseHandle(hFile);

			 delete [] charBuffer;
		 }
private: System::Void showSaveItem(System::Object *  sender, System::EventArgs *  e)
		 { 
			 if (this->fileName=="")
			 {
				 this->saveFileDialog1->Title=S"Save";
				 this->saveFileDialog1->ShowDialog();
			 }
			 else {
				 this->doSaveFile(NULL,NULL);
			 }
		 }

private: System::Void showSaveAsItem(System::Object *  sender, System::EventArgs *  e)
		 {
			 this->saveFileDialog1->Title=S"Save As...";
			 this->saveFileDialog1->ShowDialog();

		 }


private: System::Void showPrintItem(System::Object *  sender, System::EventArgs *  e)
		 {
			 pDlg = new PrintDialog();
			 pDlg->Document=this->printDoc1;
			 pDlg->ShowDialog();
			 if(DialogResult::OK)
			 {
				 pDlg->Document->Print();
			 }
		 }

private: System::Void showFileExitItem(System::Object *  sender, System::EventArgs *  e)
		 {
			 //do exit with Cancel bug
			 this->checkTextModified(sender);
			 Application::Exit();
		 }

private: System::Void doUndoAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->Undo();
		 }

private: System::Void doRedoAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->Redo();
		 }

private: System::Void doCutAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->Cut();
		 }

private: System::Void doCopyAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->Copy();
		 }

private: System::Void doPasteAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->Paste();
		 }

private: System::Void doDeleteAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->Cut();
		 }

private: System::Void doSelectAllAction(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->SelectAll();
		 }
private: System::Void showFindForm(int num)
		 {
			 //labels
			 findLabel = new Label();
			 findLabel->Location=System::Drawing::Point(0,0);
			 findLabel->Text="Find:";
			 findLabel->Visible=true;

			 //input from user for string to find.
			 findBox = new TextBox();
			 findBox->ClientSize = System::Drawing::Size(320,80);
			 findBox->Location = System::Drawing::Point(25,0);
			 findBox->Multiline=false;

			 // form to simulate Find on the TextBox(mainTextbox)
			 findForm = new Form();
			 findForm->FormBorderStyle=FormBorderStyle::FixedToolWindow;
			 findForm->set_ResizeRedraw(false);
			 findForm->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			 findForm->ClientSize = System::Drawing::Size(430, 130);
			 findForm->Name = S"FindForm1";
			 switch (num)
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
			 findForm->Controls->Add(findBox);  //textbox to specify the text to find.
			 findForm->Controls->Add(matchCaseCB);   //match case option
			 findForm->Controls->Add(wholeWordCB);   //match whole word option
			 findForm->Controls->Add(findLabel);

			 findForm->ResumeLayout(false);
			 
			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }

			 findForm->Show();
		 }
private: System::Void doFind(System::Object *  sender, System::EventArgs *  e)
		 {
			 Form1::richTBSelectedText=mainTextbox->SelectedText;
			 if(richTBSelectedText!=NULL)
			 {
				 Form1::findString=richTBSelectedText;
				 findBox->Text=findString;
				 richTBSelectedText=NULL;
			 } else if(findBox->Text!=NULL) {
				 findString=findBox->get_Text();
			 }
			 mainTextbox->Find(this->findString);
			 Form1::BringToFront();
		 }
private: System::Void doFindNext(System::Object *  sender, System::EventArgs *  e)
		 {
			 Form1::findString=this->findBox->Text;
			 mainTextbox->Find(this->findString);
			 Form1::BringToFront();
		 }

private: System::Void doSaveFile(System::Object *  sender, System::ComponentModel::CancelEventArgs *  e)
		 {
			 if(fileName=="")
			 {
				 this->fileName = saveFileDialog1->FileName;
			 }
			 int fileNameLength=fileName->Length;

			 //convert the managed String type of file name into 
			 //flat C char to use flat C fstream to read and write to file.
			 char * fileNCharArr;
			 fileNCharArr= new char[fileNameLength];

			 int i=0;
			 for (;i<fileNameLength;i++)
			 {
				 __wchar_t temp=fileName->get_Chars(i);
				 fileNCharArr[i]=temp;
			 }
			 fileNCharArr[i]='\0';
			 fstream fileOut(fileNCharArr, ios::out);
			 String * lines[]= mainTextbox->Lines;
			 for(int j=0; j<lines->Length; j++)
			 {
				 int k=0;
				 char * fileOutArr = new char[(lines[j]->Length)];
				 for (; k<lines[j]->Length; k++)
				 {
					 __wchar_t temp = lines[j]->get_Chars(k);
					 fileOutArr[k]=temp;
				 }
				 fileOutArr[k]='\0';
				 fileOut << fileOutArr;

				 if(j!=lines->Length-1)
					 fileOut<<endl;
			 }
			 fileOut.close();
		 }

private: System::Void showFontDialog1(System::Object *  sender, System::EventArgs *  e)
		 {
			 this->fontDialog1->ShowDialog();
			 //if there is selected text from the textbox.
			 if(mainTextbox->SelectedText!="")
			 {
				 mainTextbox->SelectionFont= fontDialog1->Font;
				 mainTextbox->SelectionColor = fontDialog1->Color;
			 }
			 else if(mainTextbox->SelectedText=="")
			 {
				 mainTextbox->Font= fontDialog1->Font;
				 mainTextbox->SelectionColor = fontDialog1->Color;
			 }
		 }

private: System::Void doFindFNextReplace(System::Object *  sender, System::EventArgs *  e)
		 {
			 System::Windows::Forms::MenuItem * mI = (System::Windows::Forms::MenuItem*) sender;
			 //findString=
			 if ( mI->Equals(this->menuItem20))
			 {
				 //Find
				 showFindForm(11);
			 }
			 else if (mI->Equals(this->menuItem21))
			 {
				 //Find Next
				 showFindForm(12);
			 }


		 }

private: System::Void showColour(System::Object *  sender, System::EventArgs *  e)
		 {
			 this->colorDialog1->ShowDialog();
			 mainTextbox->SelectionColor = colorDialog1->Color;
		 }

private: System::Void showDateForm(System::Object *  sender, System::EventArgs *  e)
		 {
			 calForm = new Form();

			 // 
			 // dateTimePicker1
			 // 
			 dtp1 = new System::Windows::Forms::DateTimePicker();
			 dtp1->Location = System::Drawing::Point(0, 8);
			 dtp1->Name = S"dateTimePicker1";
			 dtp1->Size = System::Drawing::Size(264, 20);
			 dtp1->TabIndex = 0;
			 dtp1->ValueChanged += new System::EventHandler(this, doDateChanged);
			 //
			 //dateTimePicker2
			 //
			 dtp2 = new System::Windows::Forms::DateTimePicker();
			 dtp2->Location = System::Drawing::Point(0, 35);
			 dtp2->Name=S"dateTimePicker2";
			 dtp2->TabIndex= 3;
			 dtp2->Size = System::Drawing::Size(100,20);
			 dtp2->Format= DateTimePickerFormat::Time;
			 dtp2->Enabled=false;
			 //
			 //insertDateBut
			 //
			 insertDateBut= new Button();
			 insertDateBut->ClientSize=System::Drawing::Size(70,20);
			 insertDateBut->set_Text(S"Insert Date");
			 insertDateBut->Location= System::Drawing::Point(270,8);
			 insertDateBut->add_Click(new System::EventHandler(this, doInsertDate));
			 //
			 //insertTimeBut
			 //
			 insertTimeBut= new Button();
			 insertTimeBut->ClientSize= System::Drawing::Size(70,20);
			 insertTimeBut->set_Text(S"Insert Time");
			 insertTimeBut->Location=System::Drawing::Point(270,35);
			 insertTimeBut->add_Click(new System::EventHandler(this, doInsertTime));

			 // 
			 // calForm
			 // 
			 calForm->FormBorderStyle=FormBorderStyle::FixedToolWindow;
			 calForm->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			 calForm->ClientSize = System::Drawing::Size(340, 55);
			 calForm->Name = S"CalForm";
			 calForm->Tag = S"g";
			 calForm->Text = S"Choose Date to Insert";
			 calForm->set_ResizeRedraw(false);
			 calForm->MaximizeBox=false;
			 calForm->MinimizeBox=false;
			 calForm->MaximumSize=System::Drawing::Size(350,85);
			 calForm->MinimumSize=System::Drawing::Size(350,85);
			 calForm->ResumeLayout(false);

			 //add the controls to the form and display the form
			 calForm->Controls->Add(this->insertDateBut);
			 calForm->Controls->Add(this->insertTimeBut);
			 calForm->Controls->Add(this->dtp1);
			 calForm->Controls->Add(this->dtp2);

			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }

			 calForm->Show();
		 }
private: System::Void doInsertTime(System::Object * sender, System::EventArgs * e)
		 {
			 mainTextbox->AppendText(this->dtp2->Text);
		 }
private: System::Void doInsertDate(System::Object * sender, System::EventArgs * e)
		 {
			 mainTextbox->AppendText(this->dtp1->Text);
		 }
private: System::Void doDateChanged(System::Object * sender, System::EventArgs * e)
		 {
			 String * insertDateString = (char *) malloc(4294967296);

			 dateString = this->dtp1->Text;
		 }

private: System::Void doBold(System::Object *  sender, System::EventArgs *  e)
		 {
			 //if there is selected text from the textbox.
			 if(mainTextbox->SelectedText!="")
			 {
				 if (!mainTextbox->get_Font()->get_Bold())
				 {
					 mainTextbox->SelectionFont= new System::Drawing::Font(mainTextbox->Font, FontStyle::Bold);
				 }
				 else if (mainTextbox->get_Font()->get_Bold())
				 {
					 mainTextbox->SelectionFont = new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
			 else if(mainTextbox->SelectedText=="")
			 {
				 if(!mainTextbox->get_Font()->get_Bold())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Bold);
				 }
				 else if (mainTextbox->get_Font()->get_Bold())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
		 }

private: System::Void showOpenURL(System::Object *  sender, System::EventArgs *  e)
		 {
			 //create the form to insert URL
			 this->URLForm = new Form();
			 URLForm->FormBorderStyle=FormBorderStyle::FixedToolWindow;
			 URLForm->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			 URLForm->ClientSize = System::Drawing::Size(310, 55);
			 URLForm->Name = S"URLForm";
			 URLForm->Tag = S"u";
			 URLForm->Text = S"Enter URL to Open";
			 URLForm->ResumeLayout(false);

			 this->urlBox= new TextBox();
			 urlBox->Text=S"http://";
			 urlBox->Location = System::Drawing::Point(0,8);
			 urlBox->ClientSize= System::Drawing::Size(264, 20);

			 urlOK= new Button();
			 urlOK->ClientSize= System::Drawing::Size(40,20);
			 urlOK->Text=S"OK";
			 urlOK->Location = System::Drawing::Point(270,8);
			 urlOK->add_Click(new System::EventHandler(this, checkOpenURL));

			 URLForm->Controls->Add(this->urlBox);
			 URLForm->Controls->Add(this->urlOK);

			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }

			 URLForm->ShowDialog();
		 }
private: System::Void checkOpenURL(System::Object * sender, System::EventArgs * e)
		 {
			 this->checkTextModified(this->menuItem30);
		 }
private: System::Void doOpenURL(System::Object * sender, System::EventArgs * e)
		 {
			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }

			 String * urlStr=this->urlBox->Text;
			 URLForm->Close();
			 try 
			 {
				 // Create a 'WebRequest' object with the specified url.
				 webR = WebRequest::Create(urlStr);
				 // Send the 'WebRequest' and wait for response.
				 webResp = webR->GetResponse();
				 // Obtain a 'Stream' object associated with the response object.
				 Stream * ReceiveStream = webResp->GetResponseStream();

				 Encoding* encode = System::Text::Encoding::GetEncoding(S"utf-8");

				 // Pipe the stream to a higher level stream reader with the required encoding format.
				 StreamReader* readStream = new StreamReader(ReceiveStream, encode);
				 MessageBox::Show(S"\nResponse stream received");
				 //Char read[] = new Char[256];

				 String * line;
				 //// Read 256 charcters at a time.
				 while (line=readStream->ReadLine())
				 {
					 mainTextbox->AppendText(line);
				 }

				 //while (count > 0) {
				 //   String* str = new String(read, 0, count);
				 //   //write to richText box
				 //   mainTextbox->AppendText();
				 //   count = readStream->Read(read, 0, 256);
				 //}

				 // Release the resources of stream object.
				 readStream->Close();

				 // Release the resources of response object.
				 webResp->Close();

				 //close the previous dialog if successful webrequest.
				 this->URLForm->Close();

			 }
			 catch (System::UriFormatException * uri) 
			 {
				 MessageBox::Show(S"The URL provided is in the wrong format.\nPlease use the format 'http://' and retry.", S"BrokenApp-Error in URL Format");
			 }
			 catch (System::Net::WebException * we)
			 {
				 System::Windows::Forms::MessageBox::Show("Error due to network connection.","Network Connection Error");
			 }
		 }

private: System::Void doInsertPicture(System::Object *  sender, System::EventArgs *  e)
		 {
			 int i =12;
			 while(i<-21)
			 {
				 char * t = (char*)malloc(8290304);
				 i--;
			 }
			 MessageBox::Show("Loading default picture...","Broken App");
			 //mainTextbox->LoadFile(S"BuildLog.htm",RichTextBoxStreamType::PlainText);
		 }

private: System::Void doItalic(System::Object *  sender, System::EventArgs *  e)
		 {
			 //if there is selected text from the textbox.
			 if(mainTextbox->SelectedText!="")
			 {
				 if (!mainTextbox->get_Font()->get_Italic())
				 {
					 mainTextbox->SelectionFont= new System::Drawing::Font(mainTextbox->Font, FontStyle::Italic);
				 }
				 else if (mainTextbox->get_Font()->get_Italic())
				 {
					 mainTextbox->SelectionFont = new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
			 else if(mainTextbox->SelectedText=="")
			 {
				 if(!mainTextbox->get_Font()->get_Italic())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Italic);
				 }
				 else if (mainTextbox->get_Font()->get_Italic())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
		 }

private: System::Void doUnderline(System::Object *  sender, System::EventArgs *  e)
		 {
			 //if there is selected text from the textbox.
			 if(mainTextbox->SelectedText!="")
			 {
				 if (!mainTextbox->get_Font()->get_Underline())
				 {
					 mainTextbox->SelectionFont= new System::Drawing::Font(mainTextbox->Font, FontStyle::Underline);
				 }
				 else if (mainTextbox->get_Font()->get_Underline())
				 {
					 mainTextbox->SelectionFont = new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
			 else if(mainTextbox->SelectedText=="")
			 {
				 if(!mainTextbox->get_Font()->get_Underline())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Underline);
				 }
				 else if (mainTextbox->get_Font()->get_Underline())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
		 }

private: System::Void doStrike(System::Object *  sender, System::EventArgs *  e)
		 {
			 //if there is selected text from the textbox.
			 if(mainTextbox->SelectedText!="")
			 {
				 if (!mainTextbox->get_Font()->get_Strikeout())
				 {
					 mainTextbox->SelectionFont= new System::Drawing::Font(mainTextbox->Font, FontStyle::Strikeout);
				 }
				 else if (mainTextbox->get_Font()->get_Strikeout())
				 {
					 mainTextbox->SelectionFont = new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
			 else if(mainTextbox->SelectedText=="")
			 {
				 if(!mainTextbox->get_Font()->get_Strikeout())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Strikeout);
				 }
				 else if (mainTextbox->get_Font()->get_Strikeout())
				 {
					 mainTextbox->Font= new System::Drawing::Font(mainTextbox->Font, FontStyle::Regular);
				 }
			 }
		 }

private: System::Void doAlignLeft(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->SelectionAlignment= HorizontalAlignment::Left;
		 }

private: System::Void doIndent(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->SelectionIndent +=8;
			 mainTextbox->SelectionHangingIndent +=3;
			 mainTextbox->SelectionRightIndent +=12;
		 }

private: System::Void doAlignCenter(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->SelectionAlignment = HorizontalAlignment::Center;
		 }

private: System::Void doAligntRight(System::Object *  sender, System::EventArgs *  e)
		 {
			 mainTextbox->SelectionAlignment = HorizontalAlignment::Right;
		 }


private: System::Void doBullets(System::Object *  sender, System::EventArgs *  e)
		 {
			 if (mainTextbox->SelectionBullet)
			 {
				 mainTextbox->SelectionBullet=false;
			 }
			 else 
			 {
				 mainTextbox->SelectionBullet=true;
			 }
		 }

private: System::Void doLinkClicked(System::Object *  sender, System::Windows::Forms::LinkClickedEventArgs *  e)
		 {
			 p = new System::Diagnostics::Process();
			 p->Start("IExplore.exe", e->LinkText);
		 }
};
}


