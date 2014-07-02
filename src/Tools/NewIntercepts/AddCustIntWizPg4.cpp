//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg4.cpp
//
// DESCRIPTION: Contains implementation for the class AddCustIntWizPg2
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  --------  ----------------------------------------
// 06 Oct 2003		 Ady K	   File Created
//*******************************************************************************
#include "AddCustIntWizPg4.h"
#include "AddCustIntWizPg2.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		AddCustIntWizPg4
	// Description: Constructor for the AddCustIntWizPg4 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg4::AddCustIntWizPg4() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Information about the projects generated for you and how to use them.";

		grpMain = new GroupBox();
		rtbDetails = new RichTextBox();
		
		//
		// rtbDetails
		//
		this->rtbDetails->Name = "rtbDetails";
		this->rtbDetails->Dock = DockStyle::Fill;
		this->rtbDetails->ReadOnly = true;

		//
		// grpMain 
		//
		this->grpMain->TabStop = false;
		this->grpMain->Name = "grpMain";
		this->grpMain->Location = System::Drawing::Point(8, 16);
		this->grpMain->Size = System::Drawing::Size(880, 360);
		this->grpMain->Controls->Add (rtbDetails);

		// 
		// AddCustIntWizPg4
		// 
		this->Controls->Add(this->grpMain);
		this->ClientSize = System::Drawing::Size(896, 486);
		this->Name = "AddCustIntWizPg4";
		this->ResumeLayout(false);
	}

	//*************************************************************************
	// Method:		~AddCustIntWizPg4
	// Description: Destructor for the AddCustIntWizPg4 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg4::~AddCustIntWizPg4()
	{

	}

	//*************************************************************************
	// Method:		setRtbTextStyle
	// Description: Sets fontsize, fontstyle and color for the richtextbox
	//
	// Parameters:
	//	newFontSize - font size
	//	newFontStyle - font style
	//	newFontColor - font color
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg4::setRtbTextStyle (float newFontSize, System::Drawing::FontStyle newFontStyle, System::Drawing::Color newFontColor)
	{
		if (newFontSize == NULL)
		{
			this->rtbDetails->SelectionFont = this->rtbDetails->Font;
			this->rtbDetails->SelectionColor = this->rtbDetails->ForeColor;
			return;
		}

		System::Drawing::Font * newFont = new System::Drawing::Font (this->rtbDetails->Font->FontFamily, newFontSize, newFontStyle);

		if (newFont)
			this->rtbDetails->SelectionFont = newFont;

		this->rtbDetails->SelectionColor = newFontColor;
	}

	//*************************************************************************
	// Method:		displayCompletedDetails
	// Description: Displays a result summary in the RichTextBox
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg4::displayCompletedDetails()
	{
		//Clear the old details
		this->rtbDetails->Clear();

		//Add the new details
		this->setRtbTextStyle (14, FontStyle::Bold, SystemColors::WindowText);
		this->rtbDetails->AppendText ("The projects have been generated and placed in the directories listed below:\r\n\r\n");
		
		IEnumerator * enumerator = projectsTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList * functions = dynamic_cast <ArrayList *> (projectsTable->Item[enumerator->Current]);
			functionInformation * fInfo = dynamic_cast <functionInformation *> (functions->Item[0]);

			this->setRtbTextStyle (10, FontStyle::Bold, SystemColors::WindowText);
			this->rtbDetails->AppendText ("Project Name: ");

			this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
			this->rtbDetails->AppendText (String::Concat (fInfo->replacementLibraryName, "\r\n"));

			this->setRtbTextStyle (10, FontStyle::Bold, SystemColors::WindowText);
			this->rtbDetails->AppendText ("Project location: ");

			this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
			this->rtbDetails->AppendText (String::Concat (dynamic_cast <String *> (projectsLocations->Item[enumerator->Current->ToString()]), "\r\n\r\n"));
		}
		
		this->rtbDetails->AppendText ("\r\n\r\n");
		this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
		this->rtbDetails->AppendText ("To use your new interception libraries you must exit Holodeck and build each of the generated projects. On building the projects, the embedded post-build events will automatically copy the generated libraries to the Holodeck folder. The next time you start Holodeck, your new interception libraries will be available for use.\r\n\r\nIf you wish to uninstall a library, delete the library DLL file from the Holodeck folder and start Holodeck, you will then be prompted to remove that library from the Holodeck database.");
	}

	//*************************************************************************
	// Method:		displayFailureDetails
	// Description: Displays failure notice in the richtext box
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg4::displayFailureDetails()
	{
		//Clear the old details
		this->rtbDetails->Clear();

		//Add the new details
		this->setRtbTextStyle (14, FontStyle::Bold, SystemColors::WindowText);
		this->rtbDetails->AppendText ("The projects have not been generated.\r\n\r\n");
		
		this->setRtbTextStyle (12, FontStyle::Regular, SystemColors::WindowText);
		this->rtbDetails->AppendText ("Either you aborted the operation or there was an error which the code generator could not handle. Please check the output locations for any garbage files and delete them.");
	}
}