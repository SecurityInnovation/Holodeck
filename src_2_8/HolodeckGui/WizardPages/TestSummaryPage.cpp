//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestSummaryPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestSummaryPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "TestSummaryPage.h"

#undef MessageBox

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestSummaryPage
	// Description: Constructor for the TestSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestSummaryPage::TestSummaryPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Review and Modify the Test Settings.  Double-Click on an item to change it.  Click Finish when you are done making changes.";
		this->allowErrorCodePage = true;

		ColumnHeader *pageHeader = new ColumnHeader();
		pageHeader->Text = "Page Name";
		pageHeader->Width = 150;

		ColumnHeader *valueHeader = new ColumnHeader();
		valueHeader->Text = "Value(s) Set On Page:";
		valueHeader->Width = 220;

		summaryListView = new ListView();
		summaryListView->MultiSelect = false;
		summaryListView->FullRowSelect = true;
		summaryListView->GridLines = true;
		summaryListView->HideSelection = false;
		summaryListView->View = View::Details;
		summaryListView->Dock = DockStyle::Fill;

		summaryListView->DoubleClick += new EventHandler(this, onDoubleClick);

		summaryListView->Columns->Add(pageHeader);
		summaryListView->Columns->Add(valueHeader);

		GroupBox *valuesBox = new GroupBox();
		valuesBox->Text = "Current Test Values";
		valuesBox->Location = System::Drawing::Point(8, 8);
		valuesBox->Size = System::Drawing::Size(378, 220);

		valuesBox->Controls->Add(summaryListView);

		functionListViewItem = new ListViewItem();
		inParamsListViewItem = new ListViewItem();
		outParamsListViewItem = new ListViewItem();
		returnValueListViewItem = new ListViewItem();
		errorCodeListViewItem = new ListViewItem();
		firingSpecListViewItem = new ListViewItem();

		this->Controls->Add(valuesBox);
	}

	//*************************************************************************
	// Method:		~TestSummaryPage
	// Description: Destructor for the TestSummaryPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestSummaryPage::~TestSummaryPage()
	{
	}

	//*************************************************************************
	// Method:		MakeSummaryPage
	// Description: sets up the list view
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TestSummaryPage::MakeSummaryPage()
	{
		summaryListView->Items->Clear();
		functionListViewItem->SubItems->Clear();
		inParamsListViewItem->SubItems->Clear();
		outParamsListViewItem->SubItems->Clear();
		returnValueListViewItem->SubItems->Clear();
		errorCodeListViewItem->SubItems->Clear();
		firingSpecListViewItem->SubItems->Clear();

		// add the function
		functionListViewItem->Text = "Function Selection";
		functionListViewItem->SubItems->Add(function);
		summaryListView->Items->Add(functionListViewItem);

		bool dotNetFunction = false;
		if (function->IndexOf('.') != -1)
			dotNetFunction = true; // .NET functions have a '.' in the name to split the class name from the function

		// add the in params
		inParamsListViewItem->Text = "In-Parameter Specification";
		summaryListView->Items->Add(inParamsListViewItem);

		if (inParameters)
		{
			bool isFirst = true;
			IEnumerator *inParamsEnumerator = inParameters->GetEnumerator();
			while (inParamsEnumerator->MoveNext())
			{
				InterceptedFunctionParameter *inParam = dynamic_cast<InterceptedFunctionParameter *>(inParamsEnumerator->Current);
				if (!inParam)
					continue;

				for (int matches=0; matches < inParam->TestCasesCount; matches++)
				{
					String *valueToAdd;
					ParamTestCase * ptCase = inParam->TestCase(matches);
					if (matches == 0)
						valueToAdd = String::Concat(inParam->Name, S" ", ptCase->TestOperator, S" ", ptCase->TestValue);
					else
						valueToAdd = String::Concat(S"or ",inParam->Name, S" ", ptCase->TestOperator, S" ", ptCase->TestValue);

					if (isFirst)
					{
						inParamsListViewItem->SubItems->Add(valueToAdd);
						isFirst = false;
					}
					else
					{
						ListViewItem *newItem = new ListViewItem("");
						newItem->SubItems->Add(valueToAdd);
						summaryListView->Items->Add(newItem);
					}
				}
			}
		}

		// add the out params
		outParamsListViewItem->Text = "Out-Parameter Specification";
		summaryListView->Items->Add(outParamsListViewItem);

		if (outParameters)
		{
			bool isFirst = true;
			IEnumerator *outParamsEnumerator = outParameters->GetEnumerator();
			while (outParamsEnumerator->MoveNext())
			{
				InterceptedFunctionParameter *outParam = dynamic_cast<InterceptedFunctionParameter *>(outParamsEnumerator->Current);
				if (!outParam)
					continue;

				if (isFirst)
				{
					outParamsListViewItem->SubItems->Add(String::Concat(outParam->Name, S" - ", outParam->ChangeValue));
					isFirst = false;
				}
				else
				{
					ListViewItem *newItem = new ListViewItem("");
					newItem->SubItems->Add(String::Concat(outParam->Name, S" - ", outParam->ChangeValue));
					summaryListView->Items->Add(newItem);
				}
			}
		}

		// add the return value
		returnValueListViewItem->Text = "Return Value Specification";

		if (dotNetFunction && allowErrorCodePage)
		{
			// Allow "error code" page means we are throwing an exception in the .NET case.  Return values
			// don't make sense if throwing an exception
			returnValueListViewItem->SubItems->Add("Throw an exception");
		}
		else
			returnValueListViewItem->SubItems->Add(returnValue);

		summaryListView->Items->Add(returnValueListViewItem);

		if (allowErrorCodePage)
		{
			// add the error code
			errorCodeListViewItem->Text = dotNetFunction ? "Exception Specification" : "Error Code Specification";
			if (!errorCode)
				errorCodeListViewItem->SubItems->Add("");
			else
				errorCodeListViewItem->SubItems->Add(errorCode);
			summaryListView->Items->Add(errorCodeListViewItem);
		}

		//add the test firing details
		firingSpecListViewItem->Text = "Test Firing Specification";
		
		if (this->firingSpecification->PauseAppAfterTestFire)
			firingSpecListViewItem->SubItems->Add ("Application will be paused after test fires");
		else
			firingSpecListViewItem->SubItems->Add ("Application will not be paused after test fires");
		
		summaryListView->Items->Add(firingSpecListViewItem);

		ListViewItem * newLvi = new ListViewItem(S"");
		ArrayList * dataArray;
		String * firePercent;
		switch (this->firingSpecification->FiringOption)
		{
		case 1:
			firePercent = dynamic_cast <String *> (this->firingSpecification->FiringData);
			newLvi->SubItems->Add (String::Concat (S"Test will run ", firePercent, S"% of the times"));
			summaryListView->Items->Add (newLvi);
			break;
		case 2:
			newLvi->SubItems->Add (S"Test will be run when function is called by one of the following:");
			summaryListView->Items->Add (newLvi);
			dataArray = dynamic_cast <ArrayList *> (this->firingSpecification->FiringData);
			for (int i=0; i <dataArray->Count; i++)
			{
				ListViewItem * dataItem = new ListViewItem(S"");
				dataItem->SubItems->Add (String::Concat (S"- ", dynamic_cast <String *> (dataArray->Item[i])));
				summaryListView->Items->Add (dataItem);
			}
			break;
		case 3:
			newLvi->SubItems->Add (S"Test will be run when callstack matches:");
			summaryListView->Items->Add (newLvi);
			dataArray = dynamic_cast <ArrayList *> (this->firingSpecification->FiringData);
			for (int i=0; i <dataArray->Count; i++)
			{
				ListViewItem * dataItem = new ListViewItem(S"");
				dataItem->SubItems->Add (String::Concat (S"- ", dynamic_cast <String *> (dataArray->Item[i])));
				summaryListView->Items->Add (dataItem);
			}
			break;
		case 4:
			newLvi->SubItems->Add (S"Test will be run when any of the following are on the callstack:");
			summaryListView->Items->Add (newLvi);
			dataArray = dynamic_cast <ArrayList *> (this->firingSpecification->FiringData);
			for (int i=0; i <dataArray->Count; i++)
			{
				ListViewItem * dataItem = new ListViewItem(S"");
				dataItem->SubItems->Add (String::Concat (S"- ", dynamic_cast <String *> (dataArray->Item[i])));
				summaryListView->Items->Add (dataItem);
			}
			break;
		default:
			break;
		}

		functionListViewItem->Selected = true;
	}

	//*************************************************************************
	// Method:		onDoubleClick
	// Description: called when the user double clicks a list view item
	//
	// Parameters:
	//	sender - the sender of the event
	//	args - the args representing this event
	//
	// Return Value: None
	//*************************************************************************
	void TestSummaryPage::onDoubleClick(Object *sender, EventArgs *args)
	{
		if (summaryListView->SelectedIndices->get_Count() <= 0)
			return;

		int selectedIndex = summaryListView->SelectedIndices->get_Item(0);

		// find the nearest item above it with a page title
		for (int i = selectedIndex; i >= 0; i--)
		{
			ListViewItem *item = summaryListView->Items->get_Item(i);
			if (!item)
				continue;

			if (item == functionListViewItem)
			{
				MessageBox::Show ("You may not change the function at this point", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Information);
				break;
			}
			else if (item == inParamsListViewItem)
			{
				if (OnModifyInParams)
					OnModifyInParams->Invoke();
				break;
			}
			else if (item == outParamsListViewItem)
			{
				if (OnModifyOutParams)
					OnModifyOutParams->Invoke();
				break;
			}
			else if (item == returnValueListViewItem)
			{
				if (OnModifyReturnValue)
					OnModifyReturnValue->Invoke();
				break;
			}
			else if ((item == errorCodeListViewItem) && (allowErrorCodePage))
			{
				if (OnModifyErrorCode)
					OnModifyErrorCode->Invoke();
				break;
			}
			else if (item == firingSpecListViewItem)
			{
				if (OnModifyFiringSpec)
					OnModifyFiringSpec->Invoke();
				break;
			}
		}
	}
}
