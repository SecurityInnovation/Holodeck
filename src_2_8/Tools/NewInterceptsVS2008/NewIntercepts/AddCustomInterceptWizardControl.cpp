//***********************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustomInterceptWizardControl.cpp
//
// DESCRIPTION: Contains implementation for the class AddCustomInterceptWizardControl
//
//===================================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 01 Oct 2003		 P. Singh	 File created.
//***********************************************************************************
#include "addcustominterceptwizardcontrol.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		AddCustomInterceptWizardControl
	// Description: Constructor for the AddCustomInterceptWizardControl class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	AddCustomInterceptWizardControl::AddCustomInterceptWizardControl() : WizardControl()
	{
		this->Profile = Crownwood::Magic::Controls::WizardControl::Profiles::Configure;
		this->Visible = true;
		this->AssignDefaultButton = true;
		this->SelectedIndex = 0;
		this->Dock = DockStyle::Fill;
		this->EnableNextButton = WizardControl::Status::Yes;
		this->EnableBackButton = WizardControl::Status::No;
		this->EnableFinishButton = WizardControl::Status::No;
		this->ButtonNextText = "&Next";
		this->ButtonBackText = "&Back";
		this->ButtonFinishText = "&Finish";
		this->ButtonCancelText = "&Cancel";

		this->Title = "Add Holodeck Intercepts";
		addCustIntPg1 = new AddCustIntWizPg1();
		addCustIntPg2 = new AddCustIntWizPg2(); 
		addCustIntPg3 = new AddCustIntWizPg3();
		addCustIntPg4 = new AddCustIntWizPg4();

		this->WizardPages->Add(addCustIntPg1);
		this->WizardPages->Add(addCustIntPg2);
		this->WizardPages->Add(addCustIntPg3);
		this->WizardPages->Add(addCustIntPg4);

		currentWP = addCustIntPg1;

		this->WizardPageEnter += new WizardPageHandler(this, &AddNewIntercepts::AddCustomInterceptWizardControl::onPageEnter);
		this->WizardPageLeave += new WizardPageHandler(this, &AddNewIntercepts::AddCustomInterceptWizardControl::onPageLeave);
		this->NextClick += new CancelEventHandler(this, &AddNewIntercepts::AddCustomInterceptWizardControl::onNextClick);
		this->FinishClick += new EventHandler(this, &AddNewIntercepts::AddCustomInterceptWizardControl::onFinishClick);
	}

	//*************************************************************************
	// Method:		~AddCustomInterceptWizardControl
	// Description: Destructor for the AddCustomInterceptWizardControl class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	AddCustomInterceptWizardControl::~AddCustomInterceptWizardControl()
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
	void AddCustomInterceptWizardControl::onPageEnter(WizardPage *wp, WizardControl *wc)
	{
		// set the title
		if (wp == addCustIntPg1)
		{
			wc->Title = "Select the functions to add to the Holodeck database";
			this->EnableCancelButton = WizardControl::Status::Yes;
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::No;
			this->EnableFinishButton = WizardControl::Status::No;
		}
		else if (wp == addCustIntPg2)
		{
			wc->Title = "Review  the definitions of the functions";
			this->EnableCancelButton = WizardControl::Status::Yes;
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;

			//Update the project path
			addCustIntPg2->ProjectPath = addCustIntPg1->ProjectPath;

			//Update the listview in the second page with data from the first
			addCustIntPg2->updateFunctionSpecificationView (addCustIntPg1->IntFuncInformation);
		}
		else if (wp == addCustIntPg3)
		{
			wc->Title = "Review your choices";
			this->EnableCancelButton = WizardControl::Status::Yes;
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;

			//Update the details textbox
			addCustIntPg3->updateDetails();

		}
		else if (wp == addCustIntPg4)
		{
			wc->Title = "Results";
			this->EnableCancelButton = WizardControl::Status::No;
			this->EnableNextButton = WizardControl::Status::No;
			this->EnableBackButton = WizardControl::Status::No;
			this->EnableFinishButton = WizardControl::Status::Yes;

			//Update the details textbox
			if (this->createSuccess)
				addCustIntPg4->displayCompletedDetails();
			else
				addCustIntPg4->displayFailureDetails();
		}
		
		currentWP = wp;
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
	void AddCustomInterceptWizardControl::onPageLeave(WizardPage *wp, WizardControl *wc)
	{
	}

	//*************************************************************************
	// Method:		onNextClick
	// Description: Event handler for next button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustomInterceptWizardControl::onNextClick(Object *sender, CancelEventArgs *e)
	{
		if (currentWP == addCustIntPg1)
		{
			try
			{
				if (!Directory::Exists (addCustIntPg1->ProjectPath))
				{
					try
					{
						Directory::CreateDirectory (addCustIntPg1->ProjectPath);
					}
					catch (...)
					{
						throw new ArgumentException("Could not create the folder for the location specified, please enter a valid location and try again");
					}
				}

				if (addCustIntPg1->FunctionsCount < 1)
					throw new ArgumentException("Please select at least one function/method from the treeview by adding a new function library");
			}
			catch (ArgumentException *aE)
			{
				MessageBox::Show (aE->Message, "Holodeck");
				e->Cancel = true;
			}
		}
		else if (currentWP == addCustIntPg2)
		{
			ArrayList * faultyFunctionsList = new ArrayList();
			//Separate out the functions and arrange according to project
			Hashtable * projectsTable = new Hashtable ();
			for (int funcs = 0; funcs < addCustIntPg2->FunctionsList->Count; funcs++)
			{
				functionInformation * fInfo = dynamic_cast <functionInformation *> (addCustIntPg2->FunctionsList->Item[funcs]);
				if (!fInfo)
					continue;
				
				InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (fInfo->selectedDefinition);
				if (ifunc == NULL)
					continue;

				if (!AddNewIntercepts::VerificationUtils::verifyFunctionDetails (fInfo))
					faultyFunctionsList->Add (fInfo);

				if ((!ifunc->ReturnType) || (ifunc->ReturnType->Trim()->Length == 0))
				{
					MessageBox::Show ("The functions marked with a red dot do not have sufficient information for code generation. You need to define at least a return type for each function in order to proceed.", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Information); 
					e->Cancel = true;
					return;
				}
				
				//If a key does not exist for this project, then create one
				if (!projectsTable->ContainsKey (ifunc->OriginalDll))
					projectsTable->Add (ifunc->OriginalDll, new ArrayList());

				//Add this function to its project key
				ArrayList * pList = dynamic_cast <ArrayList *> (projectsTable->Item[ifunc->OriginalDll]);
				pList->Add (fInfo);
			}

			if (faultyFunctionsList->Count > 0)
			{
				String * faultyList = "";
				for (int i=0; i < faultyFunctionsList->Count && i < 30; i++)
				{
					functionInformation * fi = dynamic_cast <functionInformation *> (faultyFunctionsList->Item[i]);
					faultyList = String::Concat (faultyList, fi->typeName ,S"::", fi->functionName, S"\r\n");
				}

				String * errMsg = "";
				if (faultyFunctionsList->Count < 30)
					errMsg = S"The following function definitions contain characters or other invalid data which may lead to a fault during code generation:\r\n\r\n";
				else
					errMsg = S"Some function definitions contain characters or other invalid data which may lead to a fault during code generation. The first 30 of these faulty functions are:\r\n\r\n";

				errMsg = String::Concat (errMsg, faultyList, S"\r\nPlease fix these errors before proceeding.");

				MessageBox::Show (errMsg, "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error); 
				e->Cancel = true;
				return;
			}
			
			addCustIntPg3->ProjectsTable = projectsTable;
			addCustIntPg3->ProjectPath = addCustIntPg2->ProjectPath;
		}
		else if (currentWP == addCustIntPg3)
		{
			waitDialog = new GuiSupport::WaitDialog();
			waitDialog->Title = "Please Wait..";
			waitDialog->Message = "Generating your projects..";
			waitDialog->Cancel_ClickHandler = new EventHandler (this, &AddNewIntercepts::AddCustomInterceptWizardControl::waitDialogCancelEventHandlerFunc);
			
//			ThreadStart *builderThreadStart = new ThreadStart(this, &AddNewIntercepts::AddCustomInterceptWizardControl::BuildProjectsThread);
//			builderThread= new Threading::Thread(builderThreadStart );
//			builderThread->IsBackground = true;
//			builderThread->Start();
			AddNewIntercepts::AddCustomInterceptWizardControl::BuildProjectsThread();
//			waitDialog->ShowDialog();
		}
	}

	//**************************************************************************
	// Method:		BuildProjectsThread
	// Description: Thread function which builds the projects and updates the
	//				holodeck database
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustomInterceptWizardControl::BuildProjectsThread()
	{
		this->createSuccess = true;
		
		try
		{
			addCustIntPg3->generateCode();
			addCustIntPg3->updateHolodeckDatabase();
			addCustIntPg3->updateReplacementReference();

			addCustIntPg4->ProjectsTable = addCustIntPg3->ProjectsTable;
			addCustIntPg4->ProjectsLocations = addCustIntPg3->ProjectsLocations;
			waitDialog->Close();
		}
		catch(Exception * e)
		{
			this->createSuccess = false;
			if (waitDialog)
				waitDialog->Close();
		}
	}

	//*************************************************************************
	// Method:		onFinishClick
	// Description: Event handler for next button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustomInterceptWizardControl::onFinishClick(Object *sender, EventArgs *e)
	{

	}

	//*************************************************************************
	// Method:		waitDialogCancelEventHandler
	// Description: Handler function for the event the cancel button is clicked
	//				on the wait dialog box while generating projects
	//*************************************************************************
	void AddCustomInterceptWizardControl::waitDialogCancelEventHandlerFunc (Object * sender, EventArgs * e)
	{
		this->createSuccess = false;

		if (builderThread && builderThread->IsAlive)
			builderThread->Abort();

		waitDialog->Close();
	}

	//*************************************************************************
	// Method:		verifyFunctionDetails
	// Description: Does a light scan on a the provided function details for
	//				invalid names and other problems which may cause a problem
	//				during code generation
	//
	// Parameters:
	//	finfo	- the function details to be verified
	//
	// Return Value: true if verification successful, false otherwise
	//*************************************************************************
	bool VerificationUtils::verifyFunctionDetails(functionInformation * finfo)
	{
		if (!isValidIdentifier (finfo->functionName) ||
			!isValidIdentifier (finfo->moduleName) ||
			!isValidIdentifier (finfo->typeName))
			return false;
		else
			return true;
	}

	//*************************************************************************
	// Method:		isValidIdentifier
	// Description: Scans the supplied identifier name for invalud characters
	//
	// Parameters:
	//	idname - the identifiername to scan
	//
	// Return Value: true if valid idname or if idname is NULL, false otherwise
	//*************************************************************************
	bool VerificationUtils::isValidIdentifier(String * idname)
	{
		if (!idname)
			return true;

		for (int i=0; i < idname->Length; i++)
		{
			Char x = idname->Chars[i];
			if (!Char::IsLetterOrDigit (x) && x != '_' && x != '.' && x != ':' && x != '&' && x !='*' && x !='$' && x!='(' && x != ')' && x!='+')
				return false;
		}

		return true;
	}
}

