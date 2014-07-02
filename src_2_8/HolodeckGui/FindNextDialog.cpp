//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FindNextDialog.cpp
//
// DESCRIPTION: Contains implementation for the class FindNextDialog
//
//=========================================================================
// Modification History
//
// Date         SCR  Name			Purpose
// -----------  ---  -----------	---------------------------------------
// 05 May 2003		 A. Kakrania	File created.
//*************************************************************************
#include "FindNextDialog.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		FindNextDialog
	// Description: Constructor for the FindNextDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FindNextDialog::FindNextDialog()
	{
		InitializeUI();

		this->logPane = NULL;

		//add the event handlers
		EventHandler * btnCancelEventHandler = new EventHandler (this, btnCancelEventHandlerFunc);
		this->btnCancel->add_Click (btnCancelEventHandler);

		EventHandler * btnFindEventHandler = new EventHandler (this, btnFindEventHandlerFunc);
		this->btnFindnext->add_Click (btnFindEventHandler);

	}

	//*************************************************************************
	// Method:		~FindNextDialog
	// Description: Destructor for the FindNextDialog class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FindNextDialog::~FindNextDialog()
	{
	}

	//*************************************************************************
	// Method:		InitializeUI
	// Description: Initializes the UI components of the dialog
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::InitializeUI()
	{
			this->lblLookFor = new System::Windows::Forms::Label();
			this->txtFindString = new System::Windows::Forms::TextBox();
			this->grpFindInCategories = new System::Windows::Forms::GroupBox();
			this->chkFieldThreadID = new System::Windows::Forms::CheckBox();
			this->chkFieldDllName = new System::Windows::Forms::CheckBox();
			this->chkFieldFuncName = new System::Windows::Forms::CheckBox();
			this->chkFieldReturnValue = new System::Windows::Forms::CheckBox();
			this->chkFieldErrorCode = new System::Windows::Forms::CheckBox();
			this->chkFieldException = new System::Windows::Forms::CheckBox();
			this->chkFieldParameters = new System::Windows::Forms::CheckBox();
			this->chkMatchWhole = new System::Windows::Forms::CheckBox();
			this->chkCaseSensitive = new System::Windows::Forms::CheckBox();
			this->btnFindnext = new System::Windows::Forms::Button();
			this->btnCancel = new System::Windows::Forms::Button();
			this->chkFieldCategory = new System::Windows::Forms::CheckBox();
			this->pnlSearchDirection = new System::Windows::Forms::Panel();
			this->rbSrchUp = new System::Windows::Forms::RadioButton();
			this->rbSrchDn = new System::Windows::Forms::RadioButton();
			this->lblSrchDirection = new System::Windows::Forms::Label();
			this->pnlSearchDirection->SuspendLayout();
			this->grpFindInCategories->SuspendLayout();
			this->SuspendLayout();
			// 
			// pnlSearchDirection
			// 
			this->pnlSearchDirection->Controls->Add(this->lblSrchDirection);
			this->pnlSearchDirection->Controls->Add(this->rbSrchDn);
			this->pnlSearchDirection->Controls->Add(this->rbSrchUp);
			this->pnlSearchDirection->Location = System::Drawing::Point(288, 37);
			this->pnlSearchDirection->Name = "pnlSearchDirection";
			this->pnlSearchDirection->Size = System::Drawing::Size(176, 24);
			// 
			// rbSrchUp
			// 
			this->rbSrchUp->Location = System::Drawing::Point(128, 4);
			this->rbSrchUp->Name = "rbSrchUp";
			this->rbSrchUp->Size = System::Drawing::Size(40, 16);
			this->rbSrchUp->TabIndex = 1;
			this->rbSrchUp->Text = "Up";
			// 
			// rbSrchDn
			// 
			this->rbSrchDn->Checked = true;
			this->rbSrchDn->Location = System::Drawing::Point(56, 4);
			this->rbSrchDn->Name = "rbSrchDn";
			this->rbSrchDn->Size = System::Drawing::Size(56, 16);
			this->rbSrchDn->TabIndex = 2;
			this->rbSrchDn->TabStop = true;
			this->rbSrchDn->Text = "Down";
			// 
			// lblSrchDirection
			// 
			this->lblSrchDirection->Location = System::Drawing::Point(-8, 4);
			this->lblSrchDirection->Name = "lblSrchDirection";
			this->lblSrchDirection->Size = System::Drawing::Size(56, 16);
			this->lblSrchDirection->TabStop = false;
			this->lblSrchDirection->Text = "Search:";
			this->lblSrchDirection->TextAlign = System::Drawing::ContentAlignment::MiddleRight;
			// 
			// lblLookFor
			// 
			this->lblLookFor->ImageAlign = System::Drawing::ContentAlignment::MiddleRight;
			this->lblLookFor->Location = System::Drawing::Point(8, 19);
			this->lblLookFor->Name = "lblLookFor";
			this->lblLookFor->Size = System::Drawing::Size(48, 16);
			this->lblLookFor->TabIndex = 1;
			this->lblLookFor->Text = "Look for";
			// 
			// txtFindString
			// 
			this->txtFindString->Location = System::Drawing::Point(56, 16);
			this->txtFindString->Name = "txtFindString";
			this->txtFindString->Size = System::Drawing::Size(480, 20);
			this->txtFindString->TabIndex = 0;
			this->txtFindString->Text = "";
			// 
			// grpFindInCategories
			// 
			this->grpFindInCategories->Controls->Add(chkFieldCategory);
			this->grpFindInCategories->Controls->Add(chkFieldParameters);
			this->grpFindInCategories->Controls->Add(chkFieldException);
			this->grpFindInCategories->Controls->Add(chkFieldErrorCode);
			this->grpFindInCategories->Controls->Add(chkFieldReturnValue);
			this->grpFindInCategories->Controls->Add(chkFieldFuncName);
			this->grpFindInCategories->Controls->Add(chkFieldDllName);
			this->grpFindInCategories->Controls->Add(chkFieldThreadID);
			this->grpFindInCategories->Location = System::Drawing::Point(56, 64);
			this->grpFindInCategories->Name = "grpFindInCategories";
			this->grpFindInCategories->Size = System::Drawing::Size(400, 80);
			this->grpFindInCategories->TabIndex = 2;
			this->grpFindInCategories->TabStop = false;
			this->grpFindInCategories->Text = "Search fields";
			// 
			// chkFieldThreadID
			// 
			this->chkFieldThreadID->Checked = true;
			this->chkFieldThreadID->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldThreadID->Location = System::Drawing::Point(8, 24);
			this->chkFieldThreadID->Name = "chkFieldThreadID";
			this->chkFieldThreadID->Size = System::Drawing::Size(80, 16);
			this->chkFieldThreadID->TabIndex = 0;
			this->chkFieldThreadID->Text = "Thread ID";
			// 
			// chkFieldDllName
			// 
			this->chkFieldDllName->Checked = true;
			this->chkFieldDllName->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldDllName->Location = System::Drawing::Point(216, 24);
			this->chkFieldDllName->Name = "chkFieldDllName";
			this->chkFieldDllName->Size = System::Drawing::Size(80, 16);
			this->chkFieldDllName->TabIndex = 1;
			this->chkFieldDllName->Text = "Dll name";
			// 
			// chkFieldFuncName
			// 
			this->chkFieldFuncName->Checked = true;
			this->chkFieldFuncName->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldFuncName->Location = System::Drawing::Point(312, 24);
			this->chkFieldFuncName->Name = "chkFieldFuncName";
			this->chkFieldFuncName->Size = System::Drawing::Size(80, 16);
			this->chkFieldFuncName->TabIndex = 2;
			this->chkFieldFuncName->Text = "Function";
			// 
			// chkFieldReturnValue
			// 
			this->chkFieldReturnValue->Checked = true;
			this->chkFieldReturnValue->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldReturnValue->Location = System::Drawing::Point(216, 48);
			this->chkFieldReturnValue->Name = "chkFieldReturnValue";
			this->chkFieldReturnValue->Size = System::Drawing::Size(96, 16);
			this->chkFieldReturnValue->TabIndex = 3;
			this->chkFieldReturnValue->Text = "Return Value";
			// 
			// chkFieldErrorCode
			// 
			this->chkFieldErrorCode->Checked = true;
			this->chkFieldErrorCode->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldErrorCode->Location = System::Drawing::Point(112, 24);
			this->chkFieldErrorCode->Name = "chkFieldErrorCode";
			this->chkFieldErrorCode->Size = System::Drawing::Size(80, 16);
			this->chkFieldErrorCode->TabIndex = 4;
			this->chkFieldErrorCode->Text = "Error Code";
			// 
			// chkFieldException
			// 
			this->chkFieldException->Checked = true;
			this->chkFieldException->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldException->Location = System::Drawing::Point(8, 48);
			this->chkFieldException->Name = "chkFieldException";
			this->chkFieldException->Size = System::Drawing::Size(96, 16);
			this->chkFieldException->TabIndex = 5;
			this->chkFieldException->Text = "Exception";
			// 
			// chkFieldParameters
			// 
			this->chkFieldParameters->Checked = true;
			this->chkFieldParameters->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldParameters->Location = System::Drawing::Point(112, 48);
			this->chkFieldParameters->Name = "chkFieldParameters";
			this->chkFieldParameters->Size = System::Drawing::Size(88, 16);
			this->chkFieldParameters->TabIndex = 6;
			this->chkFieldParameters->Text = "Parameters";
			// 
			// chkMatchWhole
			// 
			this->chkMatchWhole->Location = System::Drawing::Point(160, 40);
			this->chkMatchWhole->Name = "chkMatchWhole";
			this->chkMatchWhole->Size = System::Drawing::Size(120, 16);
			this->chkMatchWhole->TabIndex = 5;
			this->chkMatchWhole->Text = "Match whole word";
			// 
			// chkCaseSensitive
			// 
			this->chkCaseSensitive->Location = System::Drawing::Point(56, 40);
			this->chkCaseSensitive->Name = "chkCaseSensitive";
			this->chkCaseSensitive->Size = System::Drawing::Size(128, 16);
			this->chkCaseSensitive->TabIndex = 4;
			this->chkCaseSensitive->Text = "Case sensitive";
			// 
			// btnFindnext
			// 
			this->btnFindnext->Location = System::Drawing::Point(464, 88);
			this->btnFindnext->Name = "btnFindnext";
			this->btnFindnext->Size = System::Drawing::Size(72, 24);
			this->btnFindnext->TabIndex = 6;
			this->btnFindnext->Text = "Find Next";
			// 
			// btnCancel
			// 
			this->btnCancel->Location = System::Drawing::Point(464, 120);
			this->btnCancel->Name = "btnCancel";
			this->btnCancel->Size = System::Drawing::Size(72, 24);
			this->btnCancel->TabIndex = 7;
			this->btnCancel->Text = "Cancel";
			// 
			// chkFieldCategory
			// 
			this->chkFieldCategory->Checked = true;
			this->chkFieldCategory->CheckState = System::Windows::Forms::CheckState::Checked;
			this->chkFieldCategory->Location = System::Drawing::Point(312, 48);
			this->chkFieldCategory->Name = "chkFieldCategory";
			this->chkFieldCategory->Size = System::Drawing::Size(80, 16);
			this->chkFieldCategory->TabIndex = 7;
			this->chkFieldCategory->Text = "Category";
			// 
			// frmFindUI
			// 
			this->AcceptButton = this->btnFindnext;
			this->CancelButton = this->btnCancel;
			this->AutoScaleBaseSize = System::Drawing::Size(5, 13);
			this->ControlBox = false;
			this->ShowInTaskbar = false;
			this->ClientSize = System::Drawing::Size(546, 160);
			this->Controls->Add(btnCancel);
			this->Controls->Add(btnFindnext);
			this->Controls->Add(chkMatchWhole);
			this->Controls->Add(chkCaseSensitive);
			this->Controls->Add(pnlSearchDirection);
			this->Controls->Add(grpFindInCategories);
			this->Controls->Add(txtFindString);
			this->Controls->Add(lblLookFor);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->TopMost = true;
			this->Name = "frmFindUI";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = "Find in API Logs";
			this->pnlSearchDirection->ResumeLayout(false);
			this->grpFindInCategories->ResumeLayout(false);
			this->ResumeLayout(false);

	}

	//*************************************************************************
	// Method:		set_CurrentLogPane
	// Description: Sets the pane to search
	//
	// Parameters:
	//	value - the pane to search
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::set_CurrentLogPane(LogPane *value)
	{
		if (SearchThread && SearchThread->IsAlive)
			SearchThread->Abort();

		logPane = value;
	}

	//*************************************************************************
	// Method:		btnCancelEventHandlerFunc
	// Description: Handles the cancel button click
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - click event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::btnCancelEventHandlerFunc (Object * sender, EventArgs * e)
	{
		this->btnFindnext->Enabled = true;
		
		if (String::Compare (this->btnCancel->Text, "Stop", true) == 0)
		{
			if (SearchThread != NULL && SearchThread->IsAlive)
				SearchThread->Abort();
			
			this->btnCancel->Text = "Cancel";
			this->btnFindnext->Enabled = true;
		}
		else
			this->Hide();
	}

	//*************************************************************************
	// Method:		btnFindEventHandlerFunc
	// Description: Handles the Find button click
	//
	// Parameters:
	//	sender - the sender of the event
	//	e - click event arguments
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::btnFindEventHandlerFunc (Object * sender, EventArgs * e)
	{
		SearchNext();
	}

	//*************************************************************************
	// Method:		SearchThreadFunc
	// Description: Function which runs in the search thread. Performs
	//				the search and calls method to select item.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::SearchThreadFunc ()
	{
		try
		{
			bool IsSearchDown = this->rbSrchDn->Checked;
			
			this->entryFound = false;

			TreeListView *lv = logPane->PaneTreeListView;

			TreeListNode *currentlySelectedNode;
			lv->GetCurrentNode(&currentlySelectedNode);
			if (!currentlySelectedNode)
				currentlySelectedNode = dynamic_cast<TreeListNode *>(lv->RootNode);
			
			TreeListNode *findingNode = currentlySelectedNode;
			TreeListNode *nextNode = NULL;
			
			//Start the search from the item after or before the currently selected item
			if (IsSearchDown)
			{
				nextNode = dynamic_cast<TreeListNode *>(findingNode->FirstChild());
				if (!nextNode)
				{
					nextNode = dynamic_cast<TreeListNode *>(findingNode->NextSibling());
					TreeListNode *parentNode = findingNode;
					while (!nextNode)
					{
						parentNode = dynamic_cast<TreeListNode *>(parentNode->ParentNode());
						if (!parentNode)
							break;
						nextNode = dynamic_cast<TreeListNode *>(parentNode->NextSibling());
					}
				}
			}
			else
			{
				//get to the previous node
				nextNode = dynamic_cast<TreeListNode *>(findingNode->PreviousSibling());
				
				if (nextNode)
				{
					//get to the last child now
					while (nextNode && nextNode->LastChild())
						nextNode = dynamic_cast<TreeListNode *> (nextNode->LastChild());
				}
				else
				{
					nextNode = dynamic_cast<TreeListNode *> (findingNode->ParentNode());
				}
			}

			findingNode = nextNode;

			if (!findingNode)
				findingNode = dynamic_cast<TreeListNode *>(lv->RootNode->FirstChild());

			String* searchString = this->txtFindString->Text;

			bool threadIdField = this->chkFieldThreadID->Checked;
			bool dllField = this->chkFieldDllName->Checked;
			bool errorCodeField = this->chkFieldErrorCode->Checked;
			bool exceptionField = this->chkFieldException->Checked;
			bool funcNameField = this->chkFieldFuncName->Checked;
			bool returnValueField = this->chkFieldReturnValue->Checked;
			bool categoryField = this->chkFieldCategory->Checked;
			bool parametersField = this->chkFieldParameters->Checked;

			bool caseSensitive = this->chkCaseSensitive->Checked;
			bool matchWhole = this->chkMatchWhole->Checked;

			// search till we cycle back to the selected entry or find the required value
			while (!this->entryFound)
			{
				//if we've reached the end of the list then we cycle around and continue the search
				if (!findingNode)
				{
					Object* currentlySelectedTag = currentlySelectedNode->Tag;
					Object* rootTag = dynamic_cast<TreeListNode *>(lv->RootNode)->Tag;
					if (currentlySelectedTag && rootTag && (dynamic_cast<UIntPtr*>(currentlySelectedTag)->ToPointer() ==
						dynamic_cast<UIntPtr*>(rootTag)->ToPointer()))
					{
						break;
					}

					if (IsSearchDown)
					{
						//Cycle to the first parent node in the list
						findingNode = dynamic_cast<TreeListNode *>(lv->RootNode->FirstChild());
					}
					else
					{
						//Cycle to the last child of the last parent node in the list
						findingNode = dynamic_cast<TreeListNode *>(lv->RootNode->LastChild());
						while (findingNode && findingNode->LastChild())
							findingNode = dynamic_cast<TreeListNode *>(findingNode->LastChild());
					}

					if (!findingNode)
						break;
				}

				Object* currentlySelectedTag = currentlySelectedNode->Tag;
				Object* findingTag = findingNode->Tag;
				if (currentlySelectedTag && findingTag && (dynamic_cast<UIntPtr*>(currentlySelectedTag)->ToPointer() ==
					dynamic_cast<UIntPtr*>(findingTag)->ToPointer()))
				{
					break;
				}

				try
				{
					IndexerNode *indexerNode = (IndexerNode*)(dynamic_cast<UIntPtr*>(findingNode->Tag)->ToPointer());
					LogItem *logItem = logPane->DataArray->GetLogItemForIndexerNode(indexerNode);

					if (threadIdField && logItem->ThreadID && MatchString (logItem->ThreadID, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (dllField && logItem->DllName && MatchString (logItem->DllName, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (errorCodeField && logItem->ErrorCode && MatchString (logItem->ErrorCode, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (errorCodeField && logItem->ErrorCode && ErrorCodeDB::GetInstance()->GetStringFromCode(logItem->ErrorCode) && MatchString (ErrorCodeDB::GetInstance()->GetStringFromCode(logItem->ErrorCode), searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (exceptionField && logItem->Exception && MatchString (logItem->Exception, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (funcNameField && logItem->Function && MatchString (logItem->Function, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (returnValueField && logItem->ReturnValue && MatchString (logItem->ReturnValue, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (categoryField && logItem->Category && MatchString (logItem->Category, searchString, matchWhole, caseSensitive))
						SelectFoundEntry (findingNode);
					else if (parametersField)
					{
						for (int j = 0; j < logItem->Parameters->Count; j++)
						{
							if (logItem->Parameters->Item[j] && MatchString (logItem->Parameters->Item[j]->ToString(), searchString, matchWhole, caseSensitive))
								SelectFoundEntry (findingNode);
						}
					}
				}
				catch(...)
				{

				}

				TreeListNode *nextNode = NULL;
				
				if (IsSearchDown)
				{
					nextNode = dynamic_cast<TreeListNode *>(findingNode->FirstChild());
					if (!nextNode)
					{
						nextNode = dynamic_cast<TreeListNode *>(findingNode->NextSibling());
						TreeListNode *parentNode = findingNode;
						while (!nextNode)
						{
							parentNode = dynamic_cast<TreeListNode *>(parentNode->ParentNode());
							if (!parentNode)
								break;
							nextNode = dynamic_cast<TreeListNode *>(parentNode->NextSibling());
						}
					}
				}
				else
				{
					//get to the previous node
					nextNode = dynamic_cast<TreeListNode *>(findingNode->PreviousSibling());
					
					if (nextNode)
					{
						//get to the last child now
						while (nextNode && nextNode->LastChild())
							nextNode = dynamic_cast<TreeListNode *> (nextNode->LastChild());
					}
					else
					{
						nextNode = dynamic_cast<TreeListNode *> (findingNode->ParentNode());
					}
				}

				findingNode = nextNode;
			}

			if (!this->entryFound)
			{
				UserNotification::InfoNotify("Could not find search string");
				this->txtFindString->SelectAll();		
				this->txtFindString->Focus();
			}
				
			this->btnCancel->Text = "Cancel";
			this->btnFindnext->Enabled = true;
		}
		catch(...)
		{
			UserNotification::InfoNotify("There was a problem searching for the search string.  Please try again");
			this->txtFindString->SelectAll();		
			this->txtFindString->Focus();
			this->btnCancel->Text = "Cancel";
			this->btnFindnext->Enabled = true;
		}
	}

	//*************************************************************************
	// Method:		MatchString
	// Description: Matches two strings based on the Match Whole word and Case-
	//				-sensitivitive options selected in the dialog
	//
	// Parameters:
	//	baseString - the string to be compared against
	//	matchString - the string to compare
	//  matchWhole - true to match whole string only
	//  caseSensitive - true for case sensitive match
	//
	// Return Value: true if match, false otherwise
	//*************************************************************************
	bool FindNextDialog::MatchString (String * baseString, String * matchString, bool matchWhole, bool caseSensitive)
	{
		if (matchWhole)
		{
			if (String::Compare(baseString, matchString, !caseSensitive) == 0)
				return true;
			else
				return false;
		}
		else
		{
			if (caseSensitive)
			{
				if (baseString->IndexOf (matchString) != -1)
					return true;
				else
					return false;
			}
			else
			{
				if (baseString->ToLower()->IndexOf (matchString->ToLower()) != -1)
					return true;
				else
					return false;
			}

		}

	}

	//*************************************************************************
	// Method:		SelectFoundEntry
	// Description: Selects the given entry in the listview
	//
	// Parameters:
	//	node - the node to select
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::SelectFoundEntry (TreeListNode *node)
	{
		logPane->PaneTreeListView->SelectedNode = node;
		this->entryFound = true;
	}

	//*************************************************************************
	// Method:		SearchNext
	// Description: Starts the search thread
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::SearchNext()
	{
		if (this->logPane != NULL)
		{
			this->btnCancel->Text = "Stop";
			this->btnFindnext->Enabled = false;
			
			if (SearchThread && SearchThread->IsAlive)
				SearchThread->Abort();

			SearchThread = new Threading::Thread(new Threading::ThreadStart(this, SearchThreadFunc));
			SearchThread->IsBackground = true;
			SearchThread->Start();
		}
	}

	//*************************************************************************
	// Method:		Show
	// Description: Shows the form
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::Show()
	{
		if (logPane && logPane->PaneTreeListView)
		{
			logPane->PaneTreeListView->FullHighlightOnHideSelection = true;
			logPane->PaneTreeListView->Invalidate();
		}

		__super::Show();
	}

	//*************************************************************************
	// Method:		Hide
	// Description: Hides the form
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::Hide()
	{
		if (logPane && logPane->PaneTreeListView)
		{
			logPane->PaneTreeListView->FullHighlightOnHideSelection = false;
			logPane->PaneTreeListView->Invalidate();
		}

		__super::Hide();
	}

	//*************************************************************************
	// Method:		Dispose
	// Description: Disposes the form
	//
	// Parameters:
	//	disposing - true if the form is being disposed, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void FindNextDialog::Dispose(bool disposing)
	{
		if (logPane && logPane->PaneTreeListView)
		{
			logPane->PaneTreeListView->FullHighlightOnHideSelection = false;
			logPane->PaneTreeListView->Invalidate();
		}

		__super::Dispose(disposing);
	}
}