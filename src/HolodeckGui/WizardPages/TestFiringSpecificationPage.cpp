//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestFiringSpecificationPage.cpp
//
// DESCRIPTION: Contains implementation for the class TestFiringSpecificationPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 02 Dec 2003		 Ady K		 File created.
//*************************************************************************

#include "TestFiringSpecificationPage.h"


namespace HolodeckGui
{
	//*************************************************************************
	// Method:		TestFiringSpecificationPage
	// Description: Constructor for the TestFiringSpecificationPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestFiringSpecificationPage::TestFiringSpecificationPage(Array* processSymbols) : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Define when this test should be run. Also specify whether the application should be paused after this test is run.";
		this->processSymbols = processSymbols;

		this->immediateCallerList = new ArrayList();
		this->callstackPattern = new ArrayList();
		this->functionsList = new ArrayList();

		InitializeComponent();

		lnkImmediateCaller->LinkClicked += new LinkLabelLinkClickedEventHandler (this, lnkimmediateCaller_Clicked);
		lnkCallstackPattern->LinkClicked += new LinkLabelLinkClickedEventHandler (this, lnkCallstackPattern_Clicked);
		lnkFunctionsList->LinkClicked += new LinkLabelLinkClickedEventHandler (this, lnkFunctionsList_Clicked);
	}

	//*************************************************************************
	// Method:		UpdateUserInterface
	// Description: Updates the user interface according to the internal firing
	//				details storage
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void TestFiringSpecificationPage::UpdateUserInterface()
	{
		this->immediateCallerList = NULL;
		this->callstackPattern = NULL;
		this->functionsList = NULL;

		switch (this->firingDetails->FiringOption)
		{
		case 1:
			this->rbRunPercentage->Checked = true;
			this->nudPercentage->Value = Int32::Parse (dynamic_cast <String *> (this->firingDetails->FiringData));
			break;
		case 2:
			this->rbImmediateCaller->Checked = true;
			this->immediateCallerList = dynamic_cast <ArrayList *> (this->firingDetails->FiringData);
			break;
		case 3:
			this->rbCallStackPattern->Checked = true;
			this->callstackPattern = dynamic_cast <ArrayList *> (this->firingDetails->FiringData);
			break;
		case 4:
			this->rbFunctionsList->Checked = true;
			this->functionsList = dynamic_cast <ArrayList *> (this->firingDetails->FiringData);
			break;
		}

		this->cbPauseAppAfterTestFire->Checked = this->firingDetails->PauseAppAfterTestFire;
	}

	//*************************************************************************
	// Method:		GetFiringDetails
	// Description: Gathers firing details from the UI and returns it
	//
	// Parameters:
	//	None
	//
	// Return Value: (FiringDetails)
	//*************************************************************************
	FiringDetails * TestFiringSpecificationPage::GetFiringDetails()
	{
		//Create a new object
		FiringDetails * fd = new FiringDetails();

		//Set the pause app value
		fd->PauseAppAfterTestFire = this->cbPauseAppAfterTestFire->Checked;

		//Set the firing option and data
		if (this->rbRunPercentage->Checked)
		{
			fd->FiringOption = 1;
			fd->FiringData = dynamic_cast <Object *> (this->nudPercentage->Value.ToString());

		}
		else if (this->rbImmediateCaller->Checked)
		{
			fd->FiringOption = 2;
			fd->FiringData = dynamic_cast <Object *> (this->immediateCallerList);

		}
		else if (this->rbCallStackPattern->Checked)
		{
			fd->FiringOption = 3;
			fd->FiringData = dynamic_cast <Object *> (this->callstackPattern);
		}
		else if (this->rbFunctionsList->Checked)
		{
			fd->FiringOption = 4;
			fd->FiringData = dynamic_cast <Object *> (this->functionsList);
		}

		return fd;
	}

	//*************************************************************************
	// Method:		lnkimmediateCaller_Clicked
	// Description: Handler for the Immediate caller link click event
	//*************************************************************************
	void TestFiringSpecificationPage::lnkimmediateCaller_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e)
	{
		if (!immediateCallerDialog)
			immediateCallerDialog = new TestGetFunctionsListDialog(S"Specify direct callers list", false, this->processSymbols);

		this->rbImmediateCaller->Checked = true;
		
		immediateCallerList = immediateCallerDialog->DisplayDialog (this, immediateCallerList);

	}

	//*************************************************************************
	// Method:		lnkCallstackPattern_Clicked
	// Description: Handler for the CallStack Pattern link click event
	//*************************************************************************
	void TestFiringSpecificationPage::lnkCallstackPattern_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e)
	{
		if (!callStackPatternDialog)
			callStackPatternDialog = new TestGetFunctionsListDialog(S"Define callstack patern", true, this->processSymbols);

		this->rbCallStackPattern->Checked = true;
		
		callstackPattern = callStackPatternDialog->DisplayDialog (this, callstackPattern);

	}

	//*************************************************************************
	// Method:		lnkFunctionsList_Clicked
	// Description: Handler for the Functions List link click event
	//*************************************************************************
	void TestFiringSpecificationPage::lnkFunctionsList_Clicked (Object * sender, LinkLabelLinkClickedEventArgs * e)
	{
		if (!functionsListDialog)
			functionsListDialog = new TestGetFunctionsListDialog(S"Select methods to look for in the callstack", false, this->processSymbols);

		this->rbFunctionsList->Checked = true;

		functionsList = functionsListDialog->DisplayDialog (this, functionsList);
	}

	//*************************************************************************
	// Method:		GetFuncNameWithoutClass
	// Description: Removes the class name from the given full function name
	//				
	// Parameters:
	//	fullFunctionName - function name including class name
	//
	// Return Value: (String*) Function name without the class name
	//*************************************************************************
	String * TestFiringSpecificationPage::GetFuncNameWithoutClass(String * fullFunctionName)
	{
		String * funcNameWithoutClass = fullFunctionName;
		
		int dotIndex = fullFunctionName->LastIndexOf('.');
		if (dotIndex != -1)
			funcNameWithoutClass = fullFunctionName->Substring(dotIndex + 1);

		return funcNameWithoutClass;
	}
}
