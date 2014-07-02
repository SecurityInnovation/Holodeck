#pragma once

#include <windows.h>
#include "defines.h"
#include "../UIEventRegistry.h"

#using <CustomTestProjectGenerator.dll>
#using <FunctionDatabase.dll>
#using <GuiSupport.dll>

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
	/// Summary for TestGetFunctionsListDialog
	///
	/// WARNING: If you change the name of this class, you will need to change the 
	///          'Resource File Name' property for the managed resource compiler tool 
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public __gc class TestGetFunctionsListDialog : public System::Windows::Forms::Form
	{
	public: 
		TestGetFunctionsListDialog(String* title, bool EnableMoveButtons, Array * processSymbols);

		ArrayList * DisplayDialog(System::Windows::Forms::IWin32Window * parentWindow, ArrayList * functionList);
        
	protected: 
		void Dispose(Boolean disposing)
		{
			if (disposing && components)
			{
				components->Dispose();
			}
			__super::Dispose(disposing);
		}

	private: CustomTestProjectGenerator::FunctionTreeView *  tvFunctions;
	private: System::Windows::Forms::Panel *  pnlBottomButtons;

	private: System::Windows::Forms::Panel *  pnlTreeViewText;
	private: System::Windows::Forms::Label *  lblSelectFunctions;
	private: System::Windows::Forms::Panel *  pnlSelectFunctions;
	private: System::Windows::Forms::Panel *  pnlSelectedMethods;

	private: System::Windows::Forms::Panel *  pnlMoveButtons;
	private: System::Windows::Forms::Panel *  pnlUpDownButtons;
	private: System::Windows::Forms::Button *  btnMoveDn;
	private: System::Windows::Forms::Button *  btnMoveUp;
	private: System::Windows::Forms::ListView *  lvItems;
	private: System::Windows::Forms::ColumnHeader *  chFuncName;
	private: System::Windows::Forms::ColumnHeader *  chLibName;
	private: System::Windows::Forms::Label *  lblSelectedFunctions;
	private: System::Windows::Forms::Panel *  pnlTreeView;
	private: System::Windows::Forms::Panel *  pnlSelectedText;
	private: System::Windows::Forms::Panel *  pnlBottomTpFiller;
	private: System::Windows::Forms::Panel *  pnlBottomBtFiller;
	private: System::Windows::Forms::Panel *  pnlBottomRtFiller;
	private: System::Windows::Forms::Button *  btnAccept;
	private: Array *  processSymbols;
	private: System::Windows::Forms::LinkLabel *  lnkSymbolsLearnMore;

	private: TreeNode* symbolsNode;

	private:
			 void functionCheckChanged_Handler (String * functionName, bool IsChecked);

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
			this->pnlBottomButtons = new System::Windows::Forms::Panel();
			this->lnkSymbolsLearnMore = new System::Windows::Forms::LinkLabel();
			this->btnAccept = new System::Windows::Forms::Button();
			this->pnlBottomRtFiller = new System::Windows::Forms::Panel();
			this->pnlBottomBtFiller = new System::Windows::Forms::Panel();
			this->pnlBottomTpFiller = new System::Windows::Forms::Panel();
			this->pnlSelectFunctions = new System::Windows::Forms::Panel();
			this->pnlTreeView = new System::Windows::Forms::Panel();
			this->pnlTreeViewText = new System::Windows::Forms::Panel();
			this->lblSelectFunctions = new System::Windows::Forms::Label();
			this->pnlSelectedMethods = new System::Windows::Forms::Panel();
			this->lvItems = new System::Windows::Forms::ListView();
			this->chFuncName = new System::Windows::Forms::ColumnHeader();
			this->chLibName = new System::Windows::Forms::ColumnHeader();
			this->pnlSelectedText = new System::Windows::Forms::Panel();
			this->lblSelectedFunctions = new System::Windows::Forms::Label();
			this->pnlMoveButtons = new System::Windows::Forms::Panel();
			this->pnlUpDownButtons = new System::Windows::Forms::Panel();
			this->btnMoveDn = new System::Windows::Forms::Button();
			this->btnMoveUp = new System::Windows::Forms::Button();
			this->pnlBottomButtons->SuspendLayout();
			this->pnlSelectFunctions->SuspendLayout();
			this->pnlTreeViewText->SuspendLayout();
			this->pnlSelectedMethods->SuspendLayout();
			this->pnlSelectedText->SuspendLayout();
			this->pnlMoveButtons->SuspendLayout();
			this->pnlUpDownButtons->SuspendLayout();
			this->SuspendLayout();
			// 
			// pnlBottomButtons
			// 
			this->pnlBottomButtons->Controls->Add(this->lnkSymbolsLearnMore);
			this->pnlBottomButtons->Controls->Add(this->btnAccept);
			this->pnlBottomButtons->Controls->Add(this->pnlBottomRtFiller);
			this->pnlBottomButtons->Controls->Add(this->pnlBottomBtFiller);
			this->pnlBottomButtons->Controls->Add(this->pnlBottomTpFiller);
			this->pnlBottomButtons->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->pnlBottomButtons->Location = System::Drawing::Point(0, 278);
			this->pnlBottomButtons->Name = S"pnlBottomButtons";
			this->pnlBottomButtons->Size = System::Drawing::Size(446, 40);
			this->pnlBottomButtons->TabIndex = 7;
			// 
			// lnkSymbolsLearnMore
			// 
			this->lnkSymbolsLearnMore->LinkColor = System::Drawing::Color::Blue;
			this->lnkSymbolsLearnMore->Location = System::Drawing::Point(0, 5);
			this->lnkSymbolsLearnMore->Name = S"lnkSymbolsLearnMore";
			this->lnkSymbolsLearnMore->Size = System::Drawing::Size(344, 31);
			this->lnkSymbolsLearnMore->TabIndex = 7;
			this->lnkSymbolsLearnMore->TabStop = true;
			this->lnkSymbolsLearnMore->Text = S"Symbols were not found in the executable you are testing. Click here to learn mor" 
				S"e about how you can use symbols for testing.";
			this->lnkSymbolsLearnMore->Visible = false;
			this->lnkSymbolsLearnMore->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, lnkSymbolsLearnMore_LinkClicked);
			// 
			// btnAccept
			// 
			this->btnAccept->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->btnAccept->Dock = System::Windows::Forms::DockStyle::Right;
			this->btnAccept->Location = System::Drawing::Point(350, 8);
			this->btnAccept->Name = S"btnAccept";
			this->btnAccept->Size = System::Drawing::Size(72, 24);
			this->btnAccept->TabIndex = 6;
			this->btnAccept->Text = S"Accept";
			// 
			// pnlBottomRtFiller
			// 
			this->pnlBottomRtFiller->Dock = System::Windows::Forms::DockStyle::Right;
			this->pnlBottomRtFiller->Location = System::Drawing::Point(422, 8);
			this->pnlBottomRtFiller->Name = S"pnlBottomRtFiller";
			this->pnlBottomRtFiller->Size = System::Drawing::Size(24, 24);
			this->pnlBottomRtFiller->TabIndex = 5;
			// 
			// pnlBottomBtFiller
			// 
			this->pnlBottomBtFiller->Dock = System::Windows::Forms::DockStyle::Bottom;
			this->pnlBottomBtFiller->Location = System::Drawing::Point(0, 32);
			this->pnlBottomBtFiller->Name = S"pnlBottomBtFiller";
			this->pnlBottomBtFiller->Size = System::Drawing::Size(446, 8);
			this->pnlBottomBtFiller->TabIndex = 3;
			// 
			// pnlBottomTpFiller
			// 
			this->pnlBottomTpFiller->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlBottomTpFiller->Location = System::Drawing::Point(0, 0);
			this->pnlBottomTpFiller->Name = S"pnlBottomTpFiller";
			this->pnlBottomTpFiller->Size = System::Drawing::Size(446, 8);
			this->pnlBottomTpFiller->TabIndex = 2;
			// 
			// pnlSelectFunctions
			// 
			this->pnlSelectFunctions->Controls->Add(this->pnlTreeView);
			this->pnlSelectFunctions->Controls->Add(this->pnlTreeViewText);
			this->pnlSelectFunctions->Dock = System::Windows::Forms::DockStyle::Left;
			this->pnlSelectFunctions->Location = System::Drawing::Point(0, 0);
			this->pnlSelectFunctions->Name = S"pnlSelectFunctions";
			this->pnlSelectFunctions->Size = System::Drawing::Size(216, 278);
			this->pnlSelectFunctions->TabIndex = 9;
			this->pnlSelectFunctions->Paint += new System::Windows::Forms::PaintEventHandler(this, pnlTreeView_Paint);
			// 
			// pnlTreeView
			// 
			this->pnlTreeView->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlTreeView->Location = System::Drawing::Point(0, 24);
			this->pnlTreeView->Name = S"pnlTreeView";
			this->pnlTreeView->Size = System::Drawing::Size(216, 254);
			this->pnlTreeView->TabIndex = 1;
			// 
			// pnlTreeViewText
			// 
			this->pnlTreeViewText->Controls->Add(this->lblSelectFunctions);
			this->pnlTreeViewText->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlTreeViewText->Location = System::Drawing::Point(0, 0);
			this->pnlTreeViewText->Name = S"pnlTreeViewText";
			this->pnlTreeViewText->Size = System::Drawing::Size(216, 24);
			this->pnlTreeViewText->TabIndex = 0;
			// 
			// lblSelectFunctions
			// 
			this->lblSelectFunctions->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblSelectFunctions->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lblSelectFunctions->Location = System::Drawing::Point(0, 0);
			this->lblSelectFunctions->Name = S"lblSelectFunctions";
			this->lblSelectFunctions->Size = System::Drawing::Size(216, 24);
			this->lblSelectFunctions->TabIndex = 4;
			this->lblSelectFunctions->Text = S"Select functions:";
			this->lblSelectFunctions->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
			// 
			// pnlSelectedMethods
			// 
			this->pnlSelectedMethods->Controls->Add(this->lvItems);
			this->pnlSelectedMethods->Controls->Add(this->pnlSelectedText);
			this->pnlSelectedMethods->Controls->Add(this->pnlMoveButtons);
			this->pnlSelectedMethods->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pnlSelectedMethods->Location = System::Drawing::Point(216, 0);
			this->pnlSelectedMethods->Name = S"pnlSelectedMethods";
			this->pnlSelectedMethods->Size = System::Drawing::Size(230, 278);
			this->pnlSelectedMethods->TabIndex = 10;
			// 
			// lvItems
			// 
			System::Windows::Forms::ColumnHeader* __mcTemp__1[] = new System::Windows::Forms::ColumnHeader*[2];
			__mcTemp__1[0] = this->chFuncName;
			__mcTemp__1[1] = this->chLibName;
			this->lvItems->Columns->AddRange(__mcTemp__1);
			this->lvItems->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lvItems->FullRowSelect = true;
			this->lvItems->HeaderStyle = System::Windows::Forms::ColumnHeaderStyle::Nonclickable;
			this->lvItems->HideSelection = false;
			this->lvItems->Location = System::Drawing::Point(0, 24);
			this->lvItems->Name = S"lvItems";
			this->lvItems->Size = System::Drawing::Size(206, 254);
			this->lvItems->TabIndex = 6;
			this->lvItems->View = System::Windows::Forms::View::Details;
			// 
			// chFuncName
			// 
			this->chFuncName->Text = S"Function";
			this->chFuncName->Width = 100;
			// 
			// chLibName
			// 
			this->chLibName->Text = S"Library";
			this->chLibName->Width = 80;
			// 
			// pnlSelectedText
			// 
			this->pnlSelectedText->Controls->Add(this->lblSelectedFunctions);
			this->pnlSelectedText->Dock = System::Windows::Forms::DockStyle::Top;
			this->pnlSelectedText->Location = System::Drawing::Point(0, 0);
			this->pnlSelectedText->Name = S"pnlSelectedText";
			this->pnlSelectedText->Size = System::Drawing::Size(206, 24);
			this->pnlSelectedText->TabIndex = 5;
			// 
			// lblSelectedFunctions
			// 
			this->lblSelectedFunctions->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->lblSelectedFunctions->Dock = System::Windows::Forms::DockStyle::Fill;
			this->lblSelectedFunctions->Location = System::Drawing::Point(0, 0);
			this->lblSelectedFunctions->Name = S"lblSelectedFunctions";
			this->lblSelectedFunctions->Size = System::Drawing::Size(206, 24);
			this->lblSelectedFunctions->TabIndex = 5;
			this->lblSelectedFunctions->Text = S"Selected functions:";
			this->lblSelectedFunctions->TextAlign = System::Drawing::ContentAlignment::BottomLeft;
			// 
			// pnlMoveButtons
			// 
			this->pnlMoveButtons->Controls->Add(this->pnlUpDownButtons);
			this->pnlMoveButtons->Dock = System::Windows::Forms::DockStyle::Right;
			this->pnlMoveButtons->Location = System::Drawing::Point(206, 0);
			this->pnlMoveButtons->Name = S"pnlMoveButtons";
			this->pnlMoveButtons->Size = System::Drawing::Size(24, 278);
			this->pnlMoveButtons->TabIndex = 0;
			// 
			// pnlUpDownButtons
			// 
			this->pnlUpDownButtons->Controls->Add(this->btnMoveDn);
			this->pnlUpDownButtons->Controls->Add(this->btnMoveUp);
			this->pnlUpDownButtons->Location = System::Drawing::Point(0, 96);
			this->pnlUpDownButtons->Name = S"pnlUpDownButtons";
			this->pnlUpDownButtons->Size = System::Drawing::Size(24, 72);
			this->pnlUpDownButtons->TabIndex = 10;
			// 
			// btnMoveDn
			// 
			this->btnMoveDn->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnMoveDn->Font = new System::Drawing::Font(S"Wingdings", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)2);
			this->btnMoveDn->Location = System::Drawing::Point(0, 44);
			this->btnMoveDn->Name = S"btnMoveDn";
			this->btnMoveDn->Size = System::Drawing::Size(24, 24);
			this->btnMoveDn->TabIndex = 8;
			this->btnMoveDn->Text = S"Ú";
			// 
			// btnMoveUp
			// 
			this->btnMoveUp->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
			this->btnMoveUp->Font = new System::Drawing::Font(S"Wingdings", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point, (System::Byte)2);
			this->btnMoveUp->Location = System::Drawing::Point(0, 0);
			this->btnMoveUp->Name = S"btnMoveUp";
			this->btnMoveUp->Size = System::Drawing::Size(24, 24);
			this->btnMoveUp->TabIndex = 7;
			this->btnMoveUp->Text = S"Ù";
			// 
			// TestGetFunctionsListDialog
			// 
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ClientSize = System::Drawing::Size(446, 318);
			this->ControlBox = false;
			this->Controls->Add(this->pnlSelectedMethods);
			this->Controls->Add(this->pnlSelectFunctions);
			this->Controls->Add(this->pnlBottomButtons);
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->MinimumSize = System::Drawing::Size(472, 344);
			this->Name = S"TestGetFunctionsListDialog";
			this->ShowInTaskbar = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = S"Specify Immediate Caller Functions";
			this->pnlBottomButtons->ResumeLayout(false);
			this->pnlSelectFunctions->ResumeLayout(false);
			this->pnlTreeViewText->ResumeLayout(false);
			this->pnlSelectedMethods->ResumeLayout(false);
			this->pnlSelectedText->ResumeLayout(false);
			this->pnlMoveButtons->ResumeLayout(false);
			this->pnlUpDownButtons->ResumeLayout(false);
			this->ResumeLayout(false);

		}		
	
		void btnMoveUp_Click(System::Object *  sender, System::EventArgs *  e);
		void btnMoveDn_Click(System::Object *  sender, System::EventArgs *  e);
		ArrayList * GetArrayListFromListView();
		void lvItems_SelectChanged (Object * sender, EventArgs * e);
		void onFormResize (Object * sender, EventArgs * e);
		void lnkSymbolsLearnMore_LinkClicked(System::Object *  sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs *  e);
	private: System::Void pnlTreeView_Paint(System::Object *  sender, System::Windows::Forms::PaintEventArgs *  e)
			 {
			 }
};
}