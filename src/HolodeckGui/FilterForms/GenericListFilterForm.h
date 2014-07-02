//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		GenericListFilterForm.h
//
// DESCRIPTION: Contains definition for the class GenericListFilterForm
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	------------------------------------------
// 18 Feb 2004		 B. Shirey		File created.
//*************************************************************************
#pragma once

#using <System.dll>
#using <mscorlib.dll>
#using <GuiSupport.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

#include <stdio.h>
#include "defines.h"

using namespace System;
using namespace System::Drawing;
using namespace FunctionDatabase;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Windows::Forms;
using namespace System::Runtime::InteropServices;

namespace HolodeckGui
{
	//*************************************************************************
	// Class:		GenericListFilterForm
	// Description: Form Class which provides an interface for the Category,
	//				DLL, Return Value and Error filters.
	//
	//*************************************************************************
	public __gc class GenericListFilterForm: public System::Windows::Forms::Form
	{
	public:
		GenericListFilterForm::GenericListFilterForm(Array * availableList, Array * selectedList, String * availableHeaderText, String * selectedHeaderText, bool AllowAddNew, String * WindowTitle, EventHandler * OK_Click_EventHandler, EventHandler * Cancel_Click_EventHandler, EventHandler * VisibleChanged_EventHandler);
		~GenericListFilterForm();

		__property Array * get_SelectedList()
		{
			ArrayList * returnList = new ArrayList();
			for (int i=0; i < lstSelected->Items->Count; i++)
				returnList->Add (lstSelected->Items->Item[i]);

			return returnList->ToArray();
		}
		__property void set_SelectedList(Array * selectItems)
		{
			this->lstSelected->Items->Clear();
			for (int i=0; i<selectItems->Length; i++)
				this->lstSelected->Items->Add (selectItems->Item[i]);
		}
		
		__property Array * get_AvailableList()
		{
			Array * returnArray;
			for (int i=0; i<lstAvailable->Items->Count; i++)
				returnArray->Add (lstAvailable->Items->Item[i]->ToString());

			return returnArray;
		}

		__property void set_AvailableList(Array * availableItems)
		{
			for (int i=0; i<availableItems->Length; i++)
			{
				if ((!this->lstAvailable->Items->Contains (availableItems->Item[i])) && (!this->lstSelected->Items->Contains (availableItems->Item[i])))
					this->lstAvailable->Items->Add (availableItems->Item[i]);
			}
		}

		__property bool get_FilterEnabled() {return filterEnabled;}
		__property void set_FilterEnabled(bool value) {filterEnabled = value;}

	private:
		bool filterEnabled;
		Label * lblSelected;
		Label * lblAvailable;
		Button * btnCancel;
		Button * btnOK;
		Button * btnRemoveAll;
		Button * btnAddAll;
		Button * btnRemove;
		Button * btnAdd;
		ListBox * lstSelected;
		ListBox * lstAvailable;
		Button * btnAddNew;
		TextBox * txtNewItem;
		CheckBox * chbxEnableFilter;

		ArrayList * internal_SelectedList;
		ArrayList * internal_AvailableList;

		void InitializeUI();
		void OKClick_Func (System::Object * sender, EventArgs * e);
		void CancelClick_Func(System::Object * sender, EventArgs * e);
		void btnCancel_Click (System::Object * sender, EventArgs * e);
		void btnOK_Click (System::Object * sender, EventArgs * e);
		void btnAdd_Click (System::Object * sender, EventArgs * e);
		void btnAddAll_Click (System::Object * sender, EventArgs * e);
		void btnRemove_Click (System::Object * sender, EventArgs * e);
		void btnRemoveAll_Click (System::Object * sender, EventArgs * e);
		void btnAddNew_Click (System::Object * sender, EventArgs * e);
	};
}