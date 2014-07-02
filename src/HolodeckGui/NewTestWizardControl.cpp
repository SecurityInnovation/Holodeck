//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NewTestWizardControl.cpp
//
// DESCRIPTION: Contains implementation for the class NewTestWizardControl
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include <windows.h>
#include "NewTestWizardControl.h"
#include "UserNotification.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		NewTestWizardControl
	// Description: Constructor for the NewTestWizardControl class
	//
	// Parameters:
	//	dataNode - node to base the test on
	//	modifyTest - true if modifying a test
	//
	// Return Value: None
	//*************************************************************************
	NewTestWizardControl::NewTestWizardControl(ScheduledTestDataNode *dataNode, bool modifyTest)
	{
		this->Profile = Crownwood::Magic::Controls::WizardControl::Profiles::Configure;
		this->Visible = true;
		this->AssignDefaultButton = true;
		this->Title = "Function Selection";
		this->Dock = DockStyle::Fill;
		this->EnableNextButton = WizardControl::Status::No;
		this->EnableBackButton = WizardControl::Status::No;
		this->EnableFinishButton = WizardControl::Status::No;
		this->ButtonNextText = "&Next";
		this->ButtonBackText = "&Back";
		this->ButtonFinishText = "&Finish";
		this->ButtonCancelText = "&Cancel";
		this->inSummaryMode = false;
		this->skipPageLeaveActions = false;

		HoloScriptRegistry * holoScriptRegistry = HoloScript::HoloScriptRegistry::getInstance();
		HoloScriptApplication * holoApp = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
		Array * processSymbols;
		if (!holoApp)
			processSymbols = NULL;
		else
			processSymbols = holoApp->ProcessSymbols;

		this->parametersToMatch = new ArrayList();
		this->parametersToChange = new ArrayList();

		functionSelectionPage = new TestFunctionSelectionPage(dataNode->threadID, !modifyTest);
		functionSelectionPage->OnFunctionSelected += new FunctionSelectedDelegate(this, onFunctionSelected);

		inParamSpecificationPage = new TestInParamSpecificationPage();
		testFiringSpecificationPage = new TestFiringSpecificationPage(processSymbols);
		outParamSelectionPage = new TestOutParamSelectionPage();
		returnValueSelectionPage = new TestReturnValueSelectionPage();
		errorCodeSelectionPage = new TestErrorCodeSelectionPage();

		summaryPage = new TestSummaryPage();
		summaryPage->OnModifyInParams += new ModifyInParamsDelegate(this, onModifyInParams);
		summaryPage->OnModifyOutParams += new ModifyOutParamsDelegate(this, onModifyOutParams);
		summaryPage->OnModifyReturnValue += new ModifyReturnValueDelegate(this, onModifyReturnValue);
		summaryPage->OnModifyErrorCode += new ModifyErrorCodeDelegate(this, onModifyErrorCode);
		summaryPage->OnModifyFiringSpec += new ModifyFiringSpecDelegate(this, onModifyFiringSpec);

		this->WizardPages->Add(functionSelectionPage);
		this->WizardPages->Add(inParamSpecificationPage);
		this->WizardPages->Add(outParamSelectionPage);
		this->WizardPages->Add(returnValueSelectionPage);
		this->WizardPages->Add(errorCodeSelectionPage);
		this->WizardPages->Add(testFiringSpecificationPage);
		this->WizardPages->Add(summaryPage);

		this->WizardPageEnter += new WizardPageHandler(this, onPageEnter);
		this->WizardPageLeave += new WizardPageHandler(this, onPageLeave);

		// avoid an unhandled exception here because onPageLeave gets called when I
		// set the selectedindex below
		if (dataNode->Function)
		{
			functionSelectionPage->Function = dataNode->Function;
			onFunctionSelected(true);
			this->function = dataNode->Function;
		}

		if (dataNode->MatchParameters)
		{
			inParamSpecificationPage->Parameters = dataNode->MatchParameters;
			summaryPage->InParameters = dataNode->MatchParameters;
	
			this->parametersToMatch->Clear();
			IEnumerator *enumerator = dataNode->MatchParameters->GetEnumerator();
			while (enumerator->MoveNext())
				this->parametersToMatch->Add(enumerator->Current);
		}

		if (dataNode->ChangeParameters)
		{
			outParamSelectionPage->Parameters = dataNode->ChangeParameters;
			summaryPage->OutParameters = dataNode->ChangeParameters;

			this->parametersToChange->Clear();
			IEnumerator *enumerator = dataNode->ChangeParameters->GetEnumerator();
			while (enumerator->MoveNext())
				this->parametersToChange->Add(enumerator->Current);
		}

		if (dataNode->ReturnValue)
		{
			returnValueSelectionPage->ReturnValue = dataNode->ReturnValue;
			summaryPage->ReturnValue = dataNode->ReturnValue;
			this->returnValue = dataNode->ReturnValue;
		}

		if (dataNode->SpecifyErrorCode && dataNode->ErrorCode)
		{
			errorCodeSelectionPage->ErrorCodeValue = dataNode->ErrorCode;
			summaryPage->ErrorCode = dataNode->ErrorCode;
			this->errorCode = dataNode->ErrorCode;
			this->specifyErrorCode = dataNode->SpecifyErrorCode;
		}
		
		if (!dataNode->SpecifyErrorCode)
		{
			returnValueSelectionPage->SpecifyErrorCode = false;
			summaryPage->AllowErrorCodePage = false;
			this->errorCode = "0";
			this->specifyErrorCode = false;
		}

		testFiringSpecificationPage->FiringSpecification = new FiringDetails (dataNode->FiringOption, dataNode->FiringDetails, dataNode->PauseAppOnTestFire);
		summaryPage->FiringSpecification = testFiringSpecificationPage->FiringSpecification;
		this->firingOption = dataNode->FiringOption;
		this->pauseAppOnTestFire = dataNode->PauseAppOnTestFire;
		this->firingData = dataNode->FiringDetails;
		
	
		if (modifyTest)
			this->SelectedIndex = 6;
		else
			this->SelectedIndex = 0;
	}

	//*************************************************************************
	// Method:		~NewTestWizardControl
	// Description: Destructor for the NewTestWizardControl class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	NewTestWizardControl::~NewTestWizardControl()
	{
	}

	//*************************************************************************
	// Method:		onPageEnter
	// Description: Called when a page is being entered
	//
	// Parameters:
	//	wp - The wizard page being entered
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onPageEnter(WizardPage *wp, WizardControl *wc)
	{
		// set the title
		if (wp == functionSelectionPage)
			wc->Title = "Function Selection";
		else if (wp == inParamSpecificationPage)
		{
			wc->Title = "In-Parameter Specification";
			inParamSpecificationPage->SetDoubleClickHandlerState(true);
			if (this->InSummaryMode)
				inParamSpecificationPage->Parameters = this->parametersToMatch;
		}
		else if (wp == outParamSelectionPage)
		{
			wc->Title = "Out-Parameter Specification";
			if (this->InSummaryMode)
				outParamSelectionPage->Parameters = this->parametersToChange;
		}
		else if (wp == returnValueSelectionPage)
		{
			wc->Title = "Return Value Specification";
			if (this->InSummaryMode)
			{
				returnValueSelectionPage->SpecifyErrorCode = this->specifyErrorCode;
				returnValueSelectionPage->ReturnValue = this->returnValue;
			}
		}
		else if (wp == errorCodeSelectionPage)
		{
			wc->Title = "Error Code Specification";
			if (this->InSummaryMode)
				errorCodeSelectionPage->ErrorCodeValue = this->errorCode;
		}
		else if (wp == testFiringSpecificationPage)
		{
			wc->Title = "Test Firing Specification";
			if (this->InSummaryMode)
				testFiringSpecificationPage->FiringSpecification = new FiringDetails (this->firingOption, this->firingData, this->pauseAppOnTestFire);
		}
		else if (wp == summaryPage)
			wc->Title = "Summary Page";

		// set the button status
		if (wp == functionSelectionPage)
		{
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::No;
		}
		else
		{
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
		}

		// set up the summary page values
		if (wp == summaryPage)
		{
			inSummaryMode = true;
			ButtonNextText = "&Edit";
			this->EnableFinishButton = WizardControl::Status::Yes;
			this->ShowBackButton = WizardControl::Status::No;
			this->ShowNextButton = WizardControl::Status::Yes;

			summaryPage->MakeSummaryPage();
		}
		else
		{
			if (!inSummaryMode)
				this->ShowBackButton = WizardControl::Status::Yes;
			else
				ButtonNextText = "&Done";

			this->ShowNextButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;
		}
	}

	//*************************************************************************
	// Method:		onPageLeave
	// Description: Called when a page is being left
	//
	// Parameters:
	//	wp - The wizard page being left
	//  wc - The wizard control changing the pages
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onPageLeave(WizardPage *wp, WizardControl *wc)
	{
		//If cancel is clicked while in edit\summary mode then page leave actions should be skipped
		if (this->skipPageLeaveActions)
		{
			this->skipPageLeaveActions = false;
			return;
		}

		// set the values of the summary screen
		if (wp == functionSelectionPage)
		{
			summaryPage->Function = functionSelectionPage->Function;
			this->function = functionSelectionPage->Function;
			this->isHiddenFunction = functionSelectionPage->IsHiddenFunction;
		}
		else if (wp == inParamSpecificationPage)
		{
			inParamSpecificationPage->SetDoubleClickHandlerState(false);
			summaryPage->InParameters = inParamSpecificationPage->Parameters;
			this->parametersToMatch = inParamSpecificationPage->Parameters;
		}
		else if (wp == outParamSelectionPage)
		{
			summaryPage->OutParameters = outParamSelectionPage->Parameters;
			this->parametersToChange = outParamSelectionPage->Parameters;
		}
		else if (wp == returnValueSelectionPage)
		{
			summaryPage->ReturnValue = returnValueSelectionPage->ReturnValue;
			summaryPage->AllowErrorCodePage = returnValueSelectionPage->SpecifyErrorCode;
			this->specifyErrorCode = returnValueSelectionPage->SpecifyErrorCode;
			this->returnValue = returnValueSelectionPage->ReturnValue;
			if ((wc->SelectedIndex == 4) && (!returnValueSelectionPage->SpecifyErrorCode))
			{
				this->errorCode = "0";
				wc->SelectedIndex = 5;
			}
		}
		else if (wp == errorCodeSelectionPage)
		{
			summaryPage->ErrorCode = errorCodeSelectionPage->ErrorCodeValue;
			this->errorCode = errorCodeSelectionPage->ErrorCodeValue;
		}
		else if (wp == testFiringSpecificationPage)
		{
			summaryPage->FiringSpecification = 	testFiringSpecificationPage->FiringSpecification;		
			this->firingOption = testFiringSpecificationPage->FiringSpecification->FiringOption;
			this->firingData = testFiringSpecificationPage->FiringSpecification->FiringData;
			this->pauseAppOnTestFire = testFiringSpecificationPage->FiringSpecification->PauseAppAfterTestFire;

			if ((wc->SelectedIndex == 4) && (!returnValueSelectionPage->SpecifyErrorCode))
			{
				this->errorCode = "0";
				wc->SelectedIndex = 3;
			}
		}

		if (wp == summaryPage)
		{
			this->EnableFinishButton = WizardControl::Status::No;
		}

		if (inSummaryMode && (wp != summaryPage))
		{
			// return to the summary screen
			summaryPage->MakeSummaryPage();
			wc->SelectedIndex = 6;
		}
	}

	//*************************************************************************
	// Method:		onFunctionSelected
	// Description: Called when a function has been selected
	//
	// Parameters:
	//	valid - true if the function selected was valid, false otherwise
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onFunctionSelected(bool valid)
	{
		SetNextButtonEnabled(valid);

		inParamSpecificationPage->Function = functionSelectionPage->Function;
		inParamSpecificationPage->IsDotNetFunction = functionSelectionPage->IsDotNetFunction;
		outParamSelectionPage->Function = functionSelectionPage->Function;
		returnValueSelectionPage->Function = functionSelectionPage->Function;
		errorCodeSelectionPage->Function = functionSelectionPage->Function;
	}

	//*************************************************************************
	// Method:		onModifyInParams
	// Description: Called when the user wants to modify the in parameters
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onModifyInParams()
	{
		// set the page to the inParamsSelectionPage
		this->SelectedIndex = 1;
	}

	//*************************************************************************
	// Method:		onModifyOutParams
	// Description: Called when the user wants to modify the out parameters
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onModifyOutParams()
	{
		// set the page to the outParamsSelectionPage
		this->SelectedIndex = 2;
	}

	//*************************************************************************
	// Method:		onModifyReturnValue
	// Description: Called when the user wants to modify the return value
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onModifyReturnValue()
	{
		// set the page to the returnValueSelectionPage
		this->SelectedIndex = 3;
	}

	//*************************************************************************
	// Method:		onModifyErrorCode
	// Description: Called when the user wants to modify the error code
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onModifyErrorCode()
	{
		// set the page to the errorCodeSelectionPage
		if (returnValueSelectionPage->SpecifyErrorCode)
			this->SelectedIndex = 4;
	}

	//*************************************************************************
	// Method:		onModifyFiringSpec
	// Description: Called when the user wants to modify the firing specification
	//
	// Parameters:
	//  None
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::onModifyFiringSpec()
	{
		// set the page to the firing details page
		this->SelectedIndex = 5;
	}

	//*************************************************************************
	// Method:		OnNextClick
	// Description: Called when the user clicks the next button
	//
	// Parameters:
	//  args - the args representing the event
	//
	// Return Value: None
	//*************************************************************************
	void NewTestWizardControl::OnNextClick(CancelEventArgs *args)
	{
		if (this->SelectedIndex == 2)
		{
			outParamSelectionPage->SaveLastEditedValue();
		}
		// if this is the return value page
		else if (this->SelectedIndex == 3)
		{
			// do some input validation
			if (!functionSelectionPage->IsDotNetFunction)
			{
				if (!isNumericValue(returnValueSelectionPage->ReturnValue, true))
				{
					UserNotification::InfoNotify("You must enter an integer value for this field");
					args->Cancel = true;
				}
			}
		}
		// if this is the error code page
		else if (this->SelectedIndex == 4)
		{
			// do some input validation
			if (!functionSelectionPage->IsDotNetFunction)
			{
				if (errorCodeSelectionPage->WriteInValue && !isNumericValue(errorCodeSelectionPage->ErrorCodeValue, false))
				{
					UserNotification::InfoNotify("You must enter a non-negative integer value for this field");
					args->Cancel = true;
				}
			}
		}
		//if this is the firing details page
		else if (this->SelectedIndex == 5)
		{
			//If a list based match is specified; make sure something is in the list
			if (this->testFiringSpecificationPage->FiringSpecification->FiringOption > 1)
			{
				ArrayList * list = dynamic_cast <ArrayList *> (this->testFiringSpecificationPage->FiringSpecification->FiringData);
				if (!list || list->Count == 0)
				{
					UserNotification::InfoNotify("You must specify at least one method for the match");
					args->Cancel = true;
				}
			}
		}
		// if this is the summary page
		else if (this->SelectedIndex == 6)
		{
			summaryPage->onDoubleClick(this, NULL);
			args->Cancel = true;
		}
	}

	//*************************************************************************
	// Method:		isNumericValue
	// Description: determines if a string contains only numeric characters
	//
	// Parameters:
	//	value - string to examine
	//	allowNegative - true to allow negative numbers, false otherwise
	//
	// Return Value: true if there are only digits, false otherwise
	//*************************************************************************
	bool NewTestWizardControl::isNumericValue(String *value, bool allowNegative)
	{
		if (!value || value->Length == 0)
			return false;

		//True, False, and NULL count as numeric
		if (value->ToLower()->Equals("true") || value->ToLower()->Equals("false")
			|| value->ToLower()->Equals("null") )
			return true;

		IEnumerator *enumerator = value->GetEnumerator();
		wchar_t character;

		int index = 0;
		while (enumerator->MoveNext())
		{
			character = Convert::ToChar(enumerator->Current);
			if (allowNegative && (index == 0))
			{
				if ((!iswdigit(character)) && (character != L'-'))
					return false;
			}
			else if (!iswdigit(character))
				return false;
			index++;
		}

		return true;
	}
}
