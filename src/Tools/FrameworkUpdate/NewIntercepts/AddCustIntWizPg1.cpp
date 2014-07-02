//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg1.cpp
//
// DESCRIPTION: Contains implementation for the class AddCustIntWizPg1
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  --------  ----------------------------------------
// 01 Oct 2003		 P. Singh  File created.
// 06 Oct 2003		 Ady K	   Added functionality
//*******************************************************************************
#include "addcustintwizpg1.h"

#undef MessageBox
#undef GetCurrentDirectory

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		AddCustIntWizPg1
	// Description: Constructor for the AddCustIntWizPg1 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg1::AddCustIntWizPg1() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "This wizard will assist you in adding additional functions for Holodeck to intercept. Please add the libraries which contain the functions you wish to add.";

		ProjLocGrpBox = new GroupBox();
		btnBrowseOutput = new Button();
		ProjLocTxtBox = new TextBox();
		OutputLocLabel = new Label();

		SelFuncLibGrpBox = new GroupBox();
		FuncLibTreeView = new TreeView();
		pnlExtendedInfo = new Panel();
		RmFuncLibBtn = new Button();
		AddFuncLibBtn = new Button();
		lnkCleanNoDefs = new LinkLabel();

		selectedFunctions = new ArrayList();
		selectFileDialog = new OpenFileDialog();

		// 
		// ProjLocGrpBox
		//
		this->ProjLocGrpBox->Controls->Add(this->btnBrowseOutput);
		this->ProjLocGrpBox->Controls->Add(this->OutputLocLabel);
		this->ProjLocGrpBox->Controls->Add(this->ProjLocTxtBox);
		this->ProjLocGrpBox->Location = System::Drawing::Point(16, 324);
		this->ProjLocGrpBox->Name = "ProjLocGrpBox";
		this->ProjLocGrpBox->Size = System::Drawing::Size(858, 55);
		this->ProjLocGrpBox->TabStop = false;
		this->ProjLocGrpBox->Text = "";

		// 
		// ProjLocTxtBox 
		// 
		this->ProjLocTxtBox->Location = System::Drawing::Point(120, 20);
		this->ProjLocTxtBox->Name = "ProjLocTxtBox";
		this->ProjLocTxtBox->Size = System::Drawing::Size(700, 20);
		this->ProjLocTxtBox->TabIndex = 1;
		this->ProjLocTxtBox->Text = "";
			
		// 
		// btnBrowseOutput
		// 
		this->btnBrowseOutput->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
		this->btnBrowseOutput->Location = System::Drawing::Point(820, 20);
		this->btnBrowseOutput->Name = "btnBrowseOutput";
		this->btnBrowseOutput->Size = System::Drawing::Size(24, 20);
		this->btnBrowseOutput->TabIndex = 2;
		this->btnBrowseOutput->Text = "...";

		// 
		// OutputLocLabel
		// 
		this->OutputLocLabel->Location = System::Drawing::Point(24, 18);
		this->OutputLocLabel->Name = "OutputLocLabel";
		this->OutputLocLabel->Size = System::Drawing::Size(96, 24);
		this->OutputLocLabel->Text = "Output Location";
		this->OutputLocLabel->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;

		// 
		// SelFuncLibGrpBox
		// 
		this->SelFuncLibGrpBox->Controls->Add(this->FuncLibTreeView);
		this->SelFuncLibGrpBox->Controls->Add(this->AddFuncLibBtn);
		this->SelFuncLibGrpBox->Controls->Add(this->lnkCleanNoDefs);
		this->SelFuncLibGrpBox->Controls->Add(this->RmFuncLibBtn);
		this->SelFuncLibGrpBox->Controls->Add(this->pnlExtendedInfo);
		this->SelFuncLibGrpBox->Location = System::Drawing::Point(16, 12);
		this->SelFuncLibGrpBox->Name = "SelFuncLibGrpBox";
		this->SelFuncLibGrpBox->Size = System::Drawing::Size(858, 318);
		this->SelFuncLibGrpBox->TabStop = false;
		this->SelFuncLibGrpBox->Text = "   Select functions to intercept from custom libraries";

		// 
		// FuncLibTreeView
		// 
		this->FuncLibTreeView->CheckBoxes = true;
		this->FuncLibTreeView->ImageIndex = -1;
		this->FuncLibTreeView->Location = System::Drawing::Point(24, 20);
		this->FuncLibTreeView->Name = "FuncLibTreeView";
		this->FuncLibTreeView->SelectedImageIndex = -1;
		this->FuncLibTreeView->Size = System::Drawing::Size(350, 255);
		this->FuncLibTreeView->TabIndex = 3;
		this->FuncLibTreeView->HideSelection = false;
		
		//
		// pnlExtendedInfo
		//
		this->pnlExtendedInfo->Location = System::Drawing::Point(380, 20);
		this->pnlExtendedInfo->Name = "pnlExtendedInfo";
		this->pnlExtendedInfo->Size = System::Drawing::Size(455, 255);
		this->pnlExtendedInfo->DockPadding->All = 3;
		this->pnlExtendedInfo->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->pnlExtendedInfo->BackColor = System::Drawing::SystemColors::ControlLightLight;
		this->pnlExtendedInfo->AutoScroll = true;
		
		// 
		// AddFuncLibBtn
		// 
		this->AddFuncLibBtn->Location = System::Drawing::Point(274, 285);
		this->AddFuncLibBtn->Name = "AddFuncLibBtn";
		this->AddFuncLibBtn->Size = System::Drawing::Size(100, 25);
		this->AddFuncLibBtn->TabIndex = 1;
		this->AddFuncLibBtn->Text = "Add Library";

		//
		// lnkCleanNoDefs
		//
		this->lnkCleanNoDefs->Location = System::Drawing::Point(25, 283);
		this->lnkCleanNoDefs->Name = "lnkCleanNoDefs";
		this->lnkCleanNoDefs->Size = System::Drawing::Size (140,30);
		this->lnkCleanNoDefs->TabIndex = 3;
		this->lnkCleanNoDefs->Text = "Uncheck methods which have no definition";
		
		// 
		// RmFuncLibBtn
		// 
		this->RmFuncLibBtn->Location = System::Drawing::Point(170, 285);
		this->RmFuncLibBtn->Name = "RmFuncLibBtn";
		this->RmFuncLibBtn->Size = System::Drawing::Size(100, 25);
		this->RmFuncLibBtn->TabIndex = 2;
		this->RmFuncLibBtn->Text = "Remove Library";

		// 
		// AddCustIntWizPg1
		// 
		this->ClientSize = System::Drawing::Size(456, 550);
		this->Controls->Add(this->SelFuncLibGrpBox);
		this->Controls->Add(this->ProjLocGrpBox);
		this->Name = "AddCustIntWizPg1";
		this->ProjLocGrpBox->ResumeLayout(false);
		this->SelFuncLibGrpBox->ResumeLayout(false);
		this->ResumeLayout(false);
		
		FuncLibTreeView_AfterCheckHandler = new TreeViewEventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::FuncLibTreeView_AfterCheck);
		FuncLibTreeView->AfterCheck += FuncLibTreeView_AfterCheckHandler;
		btnBrowseOutput->Click += new EventHandler(this, &AddNewIntercepts::AddCustIntWizPg1::btnBrowseOutput_Click);
		AddFuncLibBtn->Click += new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::AddFuncLibBtn_Click);
		RmFuncLibBtn->Click += new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::RmFuncLibBtn_Click);
		FuncLibTreeView->AfterSelect += new TreeViewEventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::FuncLibTreeView_AfterSelect);
		RmFuncLibBtn->Enabled = false;

		//Load function databases
		loadDotNetFunctionDatabase();
		loadNativeFunctionDatabase();

		//Display help message in pnlExtendedInfo on startup
		TextBox * tbInitial = new TextBox();
		tbInitial->Text = "Welcome to the New Intercepts Wizard.\r\n\r\nAdd your library to the list, by clicking the \"Add Library\" button and selecting the appropriate file.\r\n\r\nIf you add a native library, you may specify header files which Holodeck can use to retrieve information from. You will also be able to specify\\edit function definitions manually on the next page.\r\n\r\nCheckmark the methods which you want to test. Clicking on a method node will bring up a list of method definitions, from which you may select your preference.";
		tbInitial->Multiline = true;
		tbInitial->BorderStyle = System::Windows::Forms::BorderStyle::None;
		tbInitial->Dock = DockStyle::Fill;
		pnlExtendedInfo->Controls->Add (tbInitial);

		//Add handlers
		this->lnkCleanNoDefs->LinkClicked += new System::Windows::Forms::LinkLabelLinkClickedEventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::lnkCleanNoDefs_LinkClicked);
	}
	//*************************************************************************
	// Method:		~AddCustIntWizPg1
	// Description: Destructor for the AddCustIntWizPg1 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg1::~AddCustIntWizPg1()
	{
		
	}

	//*************************************************************************
	// Method:		btnBrowseOutput_Click
	// Description: Event handler for output browse button click
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::btnBrowseOutput_Click(Object *sender, EventArgs *e)
	{
		folderBrowse = new FolderBrowserDialog ();
		folderBrowse->ShowNewFolderButton = true;
		folderBrowse->Description = "Select the location where you would like your project folder(s) to be created:";

		DialogResult result = folderBrowse->ShowDialog();
		if (result == DialogResult::OK)
		{
			this->ProjLocTxtBox->Text = folderBrowse->SelectedPath;
		}
	}

	//*************************************************************************
	// Method:		AddFuncLibBtn_Click
	// Description: Event handler for Add New Function Library button click
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::AddFuncLibBtn_Click(Object *sender, EventArgs *e)
	{
		//Create a dialog if none exists and set its attributes
		if (!selectFileDialog) selectFileDialog = new OpenFileDialog();
		selectFileDialog->Title = "Select a library";
		selectFileDialog->CheckFileExists = true;
		selectFileDialog->Multiselect = false;
		selectFileDialog->Filter = "Native or .NET Library(*.dll)|*.dll|All Files(*.*)|*.*";
		selectFileDialog->FilterIndex = 1;
		
		//Display the dialog and get the result
		DialogResult result = selectFileDialog->ShowDialog(this);

		if (result == DialogResult::OK)
		{
			this->nodeToAdd = NULL;

			waitDialog = NULL;
//			waitDialog = new GuiSupport::WaitDialog();
//			waitDialog->Title = "Please Wait..";
//			waitDialog->Message = "Getting information from library";
//			waitDialog->Progress = 0;
//			waitDialog->DisplayProgress = true;
//			waitDialog->Cancel_ClickHandler = new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::waitDialogCancelEventHandlerFunc);

			AddNewIntercepts::AddCustIntWizPg1::GetLibraryInformationThreadFunc();
//			ThreadStart *queryThreadStart = new ThreadStart(this, &AddNewIntercepts::AddCustIntWizPg1::GetLibraryInformationThreadFunc);
//			queryThread = new Threading::Thread(queryThreadStart);
//			queryThread->IsBackground = true;
//			queryThread->Start();
//			waitDialog->ShowDialog();

			if (this->nodeToAdd)
			{
				this->FuncLibTreeView->Nodes->Add (this->nodeToAdd);

				if (this->FuncLibTreeView->Nodes->Count > 0)
					this->FuncLibTreeView->SelectedNode = this->nodeToAdd;
			}
		}
	}

	//*************************************************************************
	// Method:		RmFuncLibBtn_Click
	// Description: Event handler for Remove Function Library button click
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::RmFuncLibBtn_Click(Object *sender, EventArgs *e)
	{
		if (FuncLibTreeView->SelectedNode == NULL)
			return;

		//Get the selcted node which is to be removed from the treeview
		TreeNode * nodeToRemove = FuncLibTreeView->SelectedNode;
		
		//Clear the information area
		this->pnlExtendedInfo->Controls->Clear();

		//Remove from selectedFunctions array
		if (selectedFunctions)
		{
			ArrayList * itemsToRemove = new ArrayList();
			for (int i=0; i < selectedFunctions->Count; i++)
			{
				TreeNode * lvi = dynamic_cast <TreeNode *> (selectedFunctions->Item[i]);
				if (lvi && lvi->Tag)
				{
					functionInformation * ifunc = dynamic_cast <functionInformation *> (lvi->Tag);
					if (ifunc && String::Compare (Path::GetFileName(ifunc->libraryName), Path::GetFileName(nodeToRemove->Text)) == 0)
						itemsToRemove->Add (lvi);
				}
			}
			for (int i=0; i < itemsToRemove->Count; i++)
			{
				selectedFunctions->Remove (itemsToRemove->Item[i]);
			}
		}

		//Remove the library node from the treeview
		this->FuncLibTreeView->Nodes->Remove (nodeToRemove);

		if (FuncLibTreeView->SelectedNode == NULL)
			RmFuncLibBtn->Enabled = false;

	}

	//*************************************************************************
	// Method:		FuncLibTreeView_AfterCheck
	// Description: Event handler for AfterCheck event for the treeview
	//				(un)Checks all child nodes (if any)
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::FuncLibTreeView_AfterCheck(Object *sender, TreeViewEventArgs *e)
	{
		//If a parent node was clicked, then (un)\check all it's children
		for (int i=0; i < e->Node->Nodes->Count; i++)
			e->Node->Nodes->Item[i]->Checked = e->Node->Checked;

		//If a function node has been checked then add it to the selectedFunctions array
		//Check if this is a function node by verifying that it has no children and that
		//its Tag property is not NULL
		if (e->Node->Nodes->Count == 0 && e->Node->Tag != NULL)
		{
			if (e->Node->Checked)
			{
				if (!selectedFunctions->Contains (e->Node))
					selectedFunctions->Add (e->Node);
			}
			else
			{
				if (selectedFunctions->Contains (e->Node))
					selectedFunctions->Remove (e->Node);
			}
		}
	}

	//*************************************************************************
	// Method:		FuncLibTreeView_AfterSelect
	// Description: Event handler for AfterSelect event for the treeview.
	//				Enables\Disables the Remove Library button depending on selection
	//				in treeview
	//				If function node, then displays extended function information in
	//				information text box
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::FuncLibTreeView_AfterSelect(Object *sender, TreeViewEventArgs *e)
	{
		//Clear the information area
		this->pnlExtendedInfo->Controls->Clear();

		//If this is a root node then enable the Remove Library button, else disable it
		if (e->Node->Parent == NULL)
			this->RmFuncLibBtn->Enabled = true;
		else
			this->RmFuncLibBtn->Enabled = false;

		//Check if node has any information that can be displayed
		if (e->Node->Tag)
		{
			//If function node, then display function information
			if (String::Compare (e->Node->Tag->GetType()->Name, "functionInformation") == 0)
			{
				functionInformation * fInfo = dynamic_cast <functionInformation *> (e->Node->Tag);
				if (fInfo->isManaged)
					displayManagedFunctionInformation (e->Node);
				else
					displayNativeFunctionInformation (e->Node);
			}
			//If library node, then display library information
			else if (String::Compare (e->Node->Tag->GetType()->Name, "libraryInformation") == 0)
			{
				libraryInformation * libInfo = dynamic_cast <libraryInformation *> (e->Node->Tag);

				if (libInfo->isManaged)
					displayManagedLibraryInformation (e->Node);
				else
					displayNativeLibraryInformation (e->Node);
			}
		}
	}
	
	//*************************************************************************
	// Method:		displayNativeLibraryInformation
	// Description: Displays the information for a native library in the info
	//				area on the page
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::displayNativeLibraryInformation (TreeNode * libNode)
	{
		lastDisplayedNode = libNode;
		
		libraryInformation * libInfo = dynamic_cast <libraryInformation *> (libNode->Tag);

		//Clear controls from the information area
		this->pnlExtendedInfo->Controls->Clear();

		//Create a textbox for displaying library information
		//Function information
		String * infoText = String::Concat ("Library: ", libInfo->fullLibraryPath);
		System::Windows::Forms::TextBox * txtLibraryName = this->getTextBoxFromText (infoText, System::Drawing::SystemColors::ControlLightLight, false);

		//Create a panel (pnlHeaderFiles) for displaying header file options
		//pnlHeaderFiles
		Panel * pnlHeaderFiles = new Panel();
		pnlHeaderFiles->DockPadding->All = 3;
		pnlHeaderFiles->BackColor = System::Drawing::SystemColors::ControlLightLight;
		pnlHeaderFiles->Dock = DockStyle::Top;
		pnlHeaderFiles->Height = 64;

		//lblHeaderInfo
		Label * lblHeaderInfo = new Label();
		lblHeaderInfo->Location = System::Drawing::Point(8, 4);
		lblHeaderInfo->Name = "lblHeaderInfo";
		lblHeaderInfo->Size = System::Drawing::Size(368, 32);
		lblHeaderInfo->Text = "Header files are used to obtain definitions for the functions in the library. You may add or remove header files for this library here.";

		//lblHeaders
		Label * lblHeaders = new Label();
		lblHeaders->Location = System::Drawing::Point(9, 37);
		lblHeaders->Name = "lblHeaders";
		lblHeaders->Size = System::Drawing::Size(56, 16);
		lblHeaders->Text = "Headers:";
		lblHeaders->TextAlign = System::Drawing::ContentAlignment::MiddleLeft;

		//cmbHeaders
		cmbHeaders = new ComboBox();
		cmbHeaders->Location = System::Drawing::Point(59, 35);
		cmbHeaders->Name = "cmbHeaders";
		cmbHeaders->Size = System::Drawing::Size(216, 21);
		cmbHeaders->Text = "";

		//btnHeaderAdd
		Button * btnHeaderAdd= new Button();
		btnHeaderAdd->Location = System::Drawing::Point(288, 36);
		btnHeaderAdd->Name = "btnHeaderAdd";
		btnHeaderAdd->Size = System::Drawing::Size(40, 21);
		btnHeaderAdd->TabIndex = 2;
		btnHeaderAdd->Text = "Add";
		btnHeaderAdd->Click += new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::btnHeaderAdd_Click);
		
		//btnHeaderDel
		Button * btnHeaderDel = new Button();
		btnHeaderDel->Location = System::Drawing::Point(336, 36);
		btnHeaderDel->Name = "btnHeaderDel";
		btnHeaderDel->Size = System::Drawing::Size(40, 21);
		btnHeaderDel->TabIndex = 3;
		btnHeaderDel->Text = "Del";
		btnHeaderDel->Click += new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::btnHeaderDel_Click);

		//Add all controls to the panel
		pnlHeaderFiles->Controls->Add (lblHeaderInfo);
		pnlHeaderFiles->Controls->Add (btnHeaderDel);
		pnlHeaderFiles->Controls->Add (btnHeaderAdd);
		pnlHeaderFiles->Controls->Add (cmbHeaders);
		pnlHeaderFiles->Controls->Add (lblHeaders);

		//Add combo-box values
		if (libInfo->nativeHeaderSearchFiles)
		{
			for (int hfiles=0; hfiles < libInfo->nativeHeaderSearchFiles->Count; hfiles++)
				cmbHeaders->Items->Add (Path::GetFileName (dynamic_cast <String *> (libInfo->nativeHeaderSearchFiles->Item[hfiles])));
		}
		addControlToInformationPanel (pnlHeaderFiles, "Header files to use for definitions:");
		addControlToInformationPanel (txtLibraryName, "Library details:");
	}

	//*************************************************************************
	// Method:		displayManagedLibraryInformation
	// Description: Displays the information for a managed library in the info
	//				area on the page
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::displayManagedLibraryInformation (TreeNode * libNode)
	{
		lastDisplayedNode = libNode;
		
		libraryInformation * libInfo = dynamic_cast <libraryInformation *> (libNode->Tag);

		//Clear controls from the information area
		this->pnlExtendedInfo->Controls->Clear();

		//Create a textbox for displaying library information
		//Function information
		String * infoText = String::Concat ("Library: ", libInfo->fullLibraryPath);
		System::Windows::Forms::TextBox * txtLibraryName = this->getTextBoxFromText (infoText, System::Drawing::SystemColors::ControlLightLight, false);

		addControlToInformationPanel (txtLibraryName, "Library details:");
	}


	//*************************************************************************
	// Method:		displayNativeFunctionInformation
	// Description: Displays the information for a native function in the info
	//				area on the page
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::displayNativeFunctionInformation (TreeNode * funcNode)
	{
		//Save this node for future reference
		lastDisplayedNode = funcNode;

		functionInformation * fInfo = dynamic_cast <functionInformation *> (funcNode->Tag);
		this->pnlExtendedInfo->Controls->Clear();

		//Function information
		String * infoText = String::Concat ("Library: ", fInfo->libraryName, "\r\nFunction: ", fInfo->functionName);
		System::Windows::Forms::TextBox * txtLibraryName = this->getTextBoxFromText (infoText, System::Drawing::SystemColors::ControlLightLight, true);

		//Display definition information
		Panel * pnlFuncDefs = new Panel();
		pnlFuncDefs->DockPadding->All = 3;
		pnlFuncDefs->BackColor = System::Drawing::SystemColors::ControlLightLight;
		pnlFuncDefs->AutoScroll = true;
		pnlFuncDefs->Dock = DockStyle::Fill;

		//Create a radio button for "Edit function details manually"
		RadioButton * rbManualEdit = new RadioButton();
		rbManualEdit->Text = "Enter definition manually on next page";
		rbManualEdit->Tag = new InterceptedFunction();
		rbManualEdit->Checked = true;
		rbManualEdit->Dock = DockStyle::Top;
		rbManualEdit->CheckedChanged += new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::funcDef_CheckChanged);
		
		//Create a text label to tell the user where this definition came from
		System::Windows::Forms::Label * lblDefinitionSource = new System::Windows::Forms::Label();
		lblDefinitionSource->Text = "[Other]";
		lblDefinitionSource->Dock = DockStyle::Top;
		lblDefinitionSource->Height = lblDefinitionSource->PreferredHeight;
		pnlFuncDefs->Controls->Add (rbManualEdit);
		pnlFuncDefs->Controls->Add (lblDefinitionSource);

		//Create and display radio buttons for the function definition(s)
		ArrayList * funcDefinitions;
		
		//Get the possible function definitionse
		funcDefinitions = (dynamic_cast <functionInformation *> (funcNode->Tag))->possibleDefinitions;
		
		if (funcDefinitions && funcDefinitions->Count > 0)
		{
			//If "edit manually was selected" then keep check mark on manual edit button
			//else remove it
			if (fInfo->selectedDefinition->Name != NULL)
				rbManualEdit->Checked = false;

			for (int i = funcDefinitions->Count - 1; i >= 0 ; i--)
			{
				definitionInfo * defInfo = dynamic_cast <definitionInfo *> (funcDefinitions->Item[i]);
				InterceptedFunction * interceptFunc = defInfo->definition;
				RadioButton * rb = new RadioButton();
				rb->Tag = interceptFunc;
				rb->CheckedChanged += new EventHandler (this, &AddNewIntercepts::AddCustIntWizPg1::funcDef_CheckChanged);
				String * text = String::Concat (interceptFunc->ReturnType, new String(" "), interceptFunc->Name, " (");
				for (int params=0; params < interceptFunc->Parameters->Count; params++)
				{
					InterceptedFunctionParameter * ifParam = dynamic_cast <InterceptedFunctionParameter *> (interceptFunc->Parameters->Item[params]);
					text = String::Concat (text, ifParam->Type, ", ");
				}
				//Remove the ending ", " from the text (if any)
				if (String::Compare (text->Substring (text->Length - 2), ", ") == 0)
					text = text->Substring (0, text->Length - 2);
				text = String::Concat (text, ")");
				
				//Set text in RadioButton
				rb->Text = text;
				rb->Dock = DockStyle::Top;
				
				//If no definition is selected select the definition on top of the list
				if (fInfo->selectedDefinition == NULL && i == 0)
					rb->Checked = true;

				//if this definition is selected; then mark it as selected
				if ((fInfo->selectedDefinition != NULL) && (interceptFunc == fInfo->selectedDefinition))
					rb->Checked = true;
				
				//Create a text label to tell the user where this definition came from
				lblDefinitionSource = new System::Windows::Forms::Label();
				lblDefinitionSource->Text = defInfo->definitionSource;
				lblDefinitionSource->Dock = DockStyle::Top;
				lblDefinitionSource->Height = lblDefinitionSource->PreferredHeight;

				pnlFuncDefs->Controls->Add (rb);
				pnlFuncDefs->Controls->Add (lblDefinitionSource);
			}
		}

		//Add controls to the main panel 
		addControlToInformationPanel (pnlFuncDefs, "Function definition(s):");
		addControlToInformationPanel (txtLibraryName, "Function details:");
		
	}

	//*************************************************************************
	// Method:		displayManagedFunctionInformation
	// Description: Displays the information for a managed method in the info
	//				area on the page
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::displayManagedFunctionInformation (TreeNode * funcNode)
	{
		//Save this node for future reference
		lastDisplayedNode = funcNode;

		functionInformation * fInfo = dynamic_cast <functionInformation *> (funcNode->Tag);
		this->pnlExtendedInfo->Controls->Clear();

		//Function information
		String * infoText = String::Concat ("Library: ", fInfo->libraryName, "\r\nFunction: ", fInfo->functionName);
		System::Windows::Forms::TextBox * txtLibraryName = this->getTextBoxFromText (infoText, System::Drawing::SystemColors::ControlLightLight, true);

		//Add controls to the main panel
		addControlToInformationPanel (txtLibraryName, "Function details:");
	}

	//*************************************************************************
	// Method:		funcDef_CheckChanged
	// Description: Handles the event when a user selects an alternate definition
	//				for a function
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::funcDef_CheckChanged (Object * sender, EventArgs *e)
	{
		RadioButton * rb = dynamic_cast <RadioButton *> (sender);
		if (rb->Checked)
		{
			functionInformation * fInfo = dynamic_cast <functionInformation *> (lastDisplayedNode->Tag);
			fInfo->selectedDefinition = dynamic_cast <InterceptedFunction *> (rb->Tag);
			lastDisplayedNode->Tag = fInfo;
		}
	}

	//*************************************************************************
	// Method:		addControlToInformationPanel
	// Description: function to add the supplied control to the information panel
	//
	// Parameters:
	//	control - the control to add
	//	headerText - the text to put in the header for the control being added
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::addControlToInformationPanel (System::Windows::Forms::Control * control, String * headerText)
	{
		pnlExtendedInfo->Controls->Add (getBlankPanel(6, control->BackColor ));
		pnlExtendedInfo->Controls->Add (control);
		pnlExtendedInfo->Controls->Add (getBlankPanel(1, control->BackColor ));
		pnlExtendedInfo->Controls->Add (getTextBoxFromText (headerText, System::Drawing::SystemColors::ControlLight, false));
	}

	//*************************************************************************
	// Method:		getBlankPanel
	// Description: returns a blank Panel with the specified height and backcolor
	//
	// Parameters:
	//	height - height of the panel
	//	color - the backcolor of the panel
	//
	// Return Value: None
	//*************************************************************************	
	Panel * AddCustIntWizPg1::getBlankPanel (int height, System::Drawing::Color color)
	{
		Panel * panel = new Panel();
		panel->Height = height;
		panel->BackColor = color;
		panel->Dock = DockStyle::Top;
		return panel;
	}

	//*************************************************************************
	// Method:		getTextBoxFromText
	// Description: Adds the given text in a textbox with the specified background color
	//				and places it inside the extended information panel
	//
	// Return Value: None
	//*************************************************************************
	TextBox * AddCustIntWizPg1::getTextBoxFromText (String * text, System::Drawing::Color color, bool IsMultiLine)
	{
		System::Windows::Forms::TextBox * txtbox = new System::Windows::Forms::TextBox();
		txtbox->Multiline = IsMultiLine;
		txtbox->Text = text;
		txtbox->ReadOnly = true;
		txtbox->BorderStyle = BorderStyle::None;
		txtbox->Height = txtbox->PreferredHeight * (txtbox->Lines->Length);
		txtbox->BackColor = color;
		txtbox->Dock = DockStyle::Top;
		return txtbox;
	}

	//*************************************************************************
	// Method:		loadDotNetFunctionDatabase
	// Description: Loads the dot net function database
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::loadDotNetFunctionDatabase()
	{
		//Load the DotNet function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		dotNetInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		if (!dotNetInterceptedFunctionDB->IsLoaded)
			dotNetInterceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);
	}

	//*************************************************************************
	// Method:		loadNativeFunctionDatabase
	// Description: Loads the native function database
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::loadNativeFunctionDatabase()
	{
		//Load the DotNet function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		nativeInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!nativeInterceptedFunctionDB->IsLoaded)
			nativeInterceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);
	}

	//*************************************************************************
	// Method:		getFunctionsDefinitionsFromDatabase
	// Description: returns an arraylist of functions which match the list of
	//				functions supplied
	//
	// Parameters:
	//	db	-	the database to search in (native or dotnet)
	//	funcList - an array of functions for which to locate definitions	
	//
	// Return Value: (ArrayList) an array of function definitions
	//*************************************************************************	
	ArrayList * AddCustIntWizPg1::getFunctionsDefinitionsFromDatabase (InterceptedFunctionDB * db, ArrayList * funcList)
	{
		ArrayList * possbileMatches = new ArrayList();
		Array * functions = db->Functions;
		IEnumerator *funcEnum = functions->GetEnumerator();
		
		while (funcEnum->MoveNext())
		{
			InterceptedFunction *func = dynamic_cast<InterceptedFunction *>(funcEnum->Current);
			if (func != NULL)
			{
				//If a match is found then add it to the list of possible matches
				for (int i=0; i < funcList->Count; i++)
				{
					functionInformation * fInfo = dynamic_cast <functionInformation *> (funcList->Item[i]);
					if ((!String::Compare (fInfo->functionName, func->Name, true)) && (!String::Compare (Path::GetFileName(fInfo->libraryName), func->OriginalDll, true)))
					{
						possbileMatches->Add (new definitionInfo (func, "[Holodeck Database]"));
					}
				}
			}
		}
		
		if (possbileMatches->Count > 0)
			return possbileMatches;
		else
			return NULL;
	}

	//*************************************************************************
	// Method:		getFunctionsInformation
	// Description: searches the holodeck function databases and any header files
	//				for function definitions
	//
	// Parameters:
	//	funcList -	an array of functions for which to locate definitions	
	//	libInfo - the library within which the functions are contained
	//  db - the database in which this function is to be searched (native or dotnet)
	//
	// Return Value: (ArrayList) an array of function definitions
	//*************************************************************************	
	ArrayList * AddCustIntWizPg1::getFunctionsInformation (ArrayList * funcList, libraryInformation * libInfo, InterceptedFunctionDB * db)
	{
		ArrayList * allDefinitions = new ArrayList();
		ArrayList * dbDefinitions = getFunctionsDefinitionsFromDatabase (db, funcList);
		ArrayList * hDefinitions = getFunctionsDefinitionsFromHeaderFiles (funcList, libInfo);
		
		if (dbDefinitions)
		{
			for (int dbitems = 0; dbitems < dbDefinitions->Count; dbitems++)
				allDefinitions->Add (dbDefinitions->Item[dbitems]);
		}

		if (hDefinitions)
		{
			for (int hitems = 0; hitems < hDefinitions->Count; hitems++)
				allDefinitions->Add (hDefinitions->Item[hitems]);
		}

		return allDefinitions;
	}

	//*************************************************************************
	// Method:		getFunctionsDefinitionsFromHeaderFiles
	// Description: searches the any specifed header files for function definitions
	//
	// Parameters:
	//	functionsList -	an array of functions for which to locate definitions	
	//	libInfo - the library within which the functions are contained
	//
	// Return Value: (ArrayList) an array of function definitions
	//*************************************************************************	
	ArrayList * AddCustIntWizPg1::getFunctionsDefinitionsFromHeaderFiles (ArrayList * functionsList, libraryInformation * libInfo)
	{
		ArrayList * masterList = new ArrayList();
		ArrayList * parsedReturns;
		for (int i=0; i < libInfo->nativeHeaderSearchFiles->Count; i++)
		{
			HeaderParser::HFileParser * hParser = new HeaderParser::HFileParser (libInfo->nativeHeaderSearchFiles->Item[i]->ToString(), functionsList);
			parsedReturns = hParser->parseFile();

			if (parsedReturns != NULL)
			{
				for (int preturns=0; preturns < parsedReturns->Count; preturns++)
				{
					definitionInfo * defInfo = new definitionInfo (dynamic_cast <InterceptedFunction *> (parsedReturns->Item[preturns]), String::Concat ("[", Path::GetFileName(libInfo->nativeHeaderSearchFiles->Item[i]->ToString()), "]"));
					masterList->Add (defInfo);
				}
			}
		}

		return masterList;
	}

	//*************************************************************************
	// Method:		btnHeaderAdd_Click
	// Description: Handles the event when the user clicks the "Add Header" button
	//
	//*************************************************************************	
	void AddCustIntWizPg1::btnHeaderAdd_Click (Object * sender, EventArgs *e)
	{
		if (!lastDisplayedNode)
			return;

		libraryInformation * libInfo = dynamic_cast <libraryInformation *> (lastDisplayedNode->Tag);

		if (!libInfo)
			return;

		if (!selectFileDialog)
			selectFileDialog = new OpenFileDialog();

		selectFileDialog->Title = "Select header file(s)";
		selectFileDialog->CheckFileExists = true;
		selectFileDialog->Multiselect = true;
		selectFileDialog->Filter = "Header Files(*.h)|*.h";
		selectFileDialog->FilterIndex = 1;

		if (selectFileDialog->ShowDialog() == DialogResult::OK)
		{
			for (int i=0; i < selectFileDialog->FileNames->Count; i++)
			{
				if (! libInfo->nativeHeaderSearchFiles->Contains (selectFileDialog->FileNames->Item[i]))
					libInfo->nativeHeaderSearchFiles->Add (selectFileDialog->FileNames->Item[i]);
			}

			lastDisplayedNode->Tag = libInfo;
		}

		//Refresh the combo view
		cmbHeaders->Items->Clear();
		for (int i=0; i < libInfo->nativeHeaderSearchFiles->Count; i++)
			cmbHeaders->Items->Add (Path::GetFileName (libInfo->nativeHeaderSearchFiles->Item[i]->ToString()));
		
		if (cmbHeaders->Items->Count > 0)
		{
			cmbHeaders->SelectedIndex = 0;
			cmbHeaders->Text = dynamic_cast <String *> (cmbHeaders->Items->Item[0]);
		}
		else
			cmbHeaders->Text = "";

		//Update function definitions
		UpdateNativeFunctionDefinitions (this->FuncLibTreeView->SelectedNode);
	}

	//*************************************************************************
	// Method:		UpdateNativeFunctionDefinitions
	// Description: Called when a new header is added to the list. This method
	//				scans for new definitions and updates the functions with the
	//				new information
	//
	// Parameters:
	//	libNode -	the library node which we are updating
	//
	// Return Value: None
	//*************************************************************************	
	void AddCustIntWizPg1::UpdateNativeFunctionDefinitions (TreeNode * libNode)
	{
		libraryInformation * libInfo = dynamic_cast <libraryInformation *> (libNode->Tag);

		//Verify that library is native :p
		if (! libInfo->isManaged)
		{
			//Create a list of functionInformation types to send to header parser
			ArrayList * functions = new ArrayList(libNode->Nodes->Count);
			for (int i=0; i < libNode->Nodes->Count; i++)
			{
				//Try and get pre-existing information
				functionInformation * funcInformation = NULL;
				if (libNode->Nodes->Item[i]->Tag != NULL)
					funcInformation = dynamic_cast <functionInformation *> (libNode->Nodes->Item[i]->Tag);
	
				//If pre-existing information does not exist, then create new infromation package
				if (funcInformation == NULL)
					funcInformation = new functionInformation (libNode->Nodes->Item[i]->Text, libNode->Text, libInfo->isManaged);

				//Clear any old definitions that may exist
				funcInformation->possibleDefinitions = new ArrayList();
				functions->Add (funcInformation);
			}

			//Get definitions for each function
			ArrayList * masterListOfDefinitions = getFunctionsInformation (functions, libInfo, nativeInterceptedFunctionDB);

			//Browse through the retrieved definitions and add to appropriate function nodes
			for (int mdefs=0; mdefs < masterListOfDefinitions->Count; mdefs++)
			{
				definitionInfo * defInfo = dynamic_cast <definitionInfo *> (masterListOfDefinitions->Item[mdefs]);
				InterceptedFunction * iFunction = defInfo->definition;
				for (int fnodes=0; fnodes < libNode->Nodes->Count; fnodes++)
				{
					functionInformation * tagInformation = dynamic_cast <functionInformation *> (libNode->Nodes->Item[fnodes]->Tag);

					//If  no tag information exists; then create tag information
					if (tagInformation == NULL)
						tagInformation = new functionInformation (libNode->Nodes->Item[fnodes]->Text, libInfo->fullLibraryPath, libInfo->isManaged);

					if (String::Compare (tagInformation->functionName, iFunction->Name) == 0)
					{
						//Update definition information
						tagInformation->possibleDefinitions->Add (defInfo);
						//If this is the first possible definition for the function, then make it the default selection
						if (tagInformation->possibleDefinitions->Count == 1)
							tagInformation->selectedDefinition = iFunction;
						//Assign tag formation back to function node
						libNode->Nodes->Item[fnodes]->Tag = tagInformation;
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		btnHeaderDel_Click
	// Description: Handles the event when the user wants to remove a header file
	//
	//*************************************************************************	
	void AddCustIntWizPg1::btnHeaderDel_Click (Object * sender, EventArgs *e)
	{
		if (!lastDisplayedNode)
			return;

		libraryInformation * libInfo = dynamic_cast <libraryInformation *> (lastDisplayedNode->Tag);

		if (!libInfo)
			return;

		for (int i=0; i < libInfo->nativeHeaderSearchFiles->Count; i++)
		{
			if (String::Compare(Path::GetFileName (libInfo->nativeHeaderSearchFiles->Item[i]->ToString()), cmbHeaders->SelectedItem->ToString()) == 0)
			{
				libInfo->nativeHeaderSearchFiles->RemoveAt(i);
				continue;
			}
		}

		//Refresh the combo view
		cmbHeaders->Items->Clear();
		for (int i=0; i < libInfo->nativeHeaderSearchFiles->Count; i++)
			cmbHeaders->Items->Add (Path::GetFileName (libInfo->nativeHeaderSearchFiles->Item[i]->ToString()));
		
		if (cmbHeaders->Items->Count > 0)
		{
			cmbHeaders->SelectedIndex = 0;
			cmbHeaders->Text = dynamic_cast <String *> (cmbHeaders->Items->Item[0]);
		}
		else
			cmbHeaders->Text = "";
		
		//Update function definitions
		UpdateNativeFunctionDefinitions (this->FuncLibTreeView->SelectedNode);
	}


	//*************************************************************************
	// Method:		isGenericType
	// Description: Checks if the given type uses generics
	//
	// Parameters:
	//	ti			- Reflection::Type object for the type to be checked
	//
	// Return Value: (bool) handled or not
	//*************************************************************************	
	bool AddCustIntWizPg1::isTypeGeneric (Type * ti)
	{
		if (ti->IsGenericParameter || ti->IsGenericType || ti->IsGenericTypeDefinition)
			return false;
		return true;
	}

	//*************************************************************************
	// Method:		ShouldIncludeType
	// Description: Selects if the supplied type should be included in the list
	//
	// Parameters:
	//	typeToTest - the supplied type
	//
	// Return Value: true if it should be included, false otherwise
	//*************************************************************************	
	bool AddCustIntWizPg1::ShouldIncludeType(Type * typeToTest)
	{
		if (!isTypeGeneric(typeToTest))
			return false;
		for (int i=0; i < typeToTest->Name->Length; i++)
		{
			if (!Char::IsLetterOrDigit (typeToTest->Name->Chars[i]) &&
				(!(typeToTest->Name->Chars[i] == '_')) &&
				(!(typeToTest->Name->Chars[i] == '.')) &&
				(!(typeToTest->Name->Chars[i] == ':')))
				return false;
		}
		
		if (typeToTest->IsClass && !typeToTest->IsImport)
			return true;
		else
			return false;
	}

	//*************************************************************************
	// Method:		isTypeHandled
	// Description: Check's to see if we handle this type
	//
	// Parameters:
	//	ti			- Reflection::Type object for the type to be checked
	//
	// Return Value: (bool) handled or not
	//*************************************************************************
	bool AddCustIntWizPg1::isTypeHandled (Type * ti)
	{
		// Sanity check
		if (!ti) return false;

		// Default checks
		if (ti->ToString()->Contains("'") || ti->ToString()->Contains("<") || ti->ToString()->Contains("!") || ti->ToString()->Contains("~"))
			return false;

		if (!isTypeGeneric(ti))
			return false;

		return true;
	}

	//*************************************************************************
	// Method:		isMethodHandled
	// Description: Check's various parameters to see if Holodeck currently
	//				handles this kind of method
	//
	// Parameters:
	//	mi			- Reflection MethodInfo object for the method to be checked
	//
	// Return Value: (bool) handled or not
	//*************************************************************************
	bool AddCustIntWizPg1::isMethodHandled (MethodInfo * mi, Type *mt)
	{
		// Sanity check
		if (!mi || !mt) return false;

		// Default checks
		if (mi->ToString()->Contains("'") || mi->ToString()->Contains("<") || mi->ToString()->Contains("!") || mi->ToString()->Contains("~"))
			return false;

		// Check return type
		if (!isTypeHandled(mi->ReturnType))
			return false;

		// Try a GetMethod on it before we proceed
		BindingFlags bindingFlags;
		if (mi->IsPublic)
			mi->IsStatic ? bindingFlags = bfPublicStatic : bindingFlags = bfPublicInstance;
		else
			mi->IsStatic ? bindingFlags = bfNonPublicStatic : bindingFlags = bfNonPublicInstance;

		try
		{
			mt->GetMethod (mi->Name, bindingFlags, NULL, GetParamTypeArray(mi), NULL);
		}
		catch (...)
		{
			return false;
		}

		// Get Parameter Info
		ParameterInfo * paramInfo[] = mi->GetParameters();

		// Check Parameter Info
		for (int parCount = 0; parCount < paramInfo->Count; parCount++)
			if (!isTypeHandled(paramInfo[parCount]->ParameterType))
				return false;

		return true;
	}

	//*************************************************************************
	// Method:		isConstructorHandled
	// Description: Check's various parameters to see if Holodeck currently
	//				handles this kind of constructor
	//
	// Parameters:
	//	ci			- Reflection ConstructorInfo object for the construnctor 
	//				to be checked
	//
	// Return Value: (bool) handled or not
	//*************************************************************************
	bool AddCustIntWizPg1::isConstructorHandled (ConstructorInfo * ci, Type *mt)
	{
		// Sanity check
		if (!ci) return false;

		// Default checks
		if (ci->ToString()->Contains("'") || ci->ToString()->Contains("<") || ci->ToString()->Contains("!") || ci->ToString()->Contains("~"))
			return false;

		// Try a GetConstructor on it before we proceed
		BindingFlags bindingFlags;
		if (ci->IsPublic)
			bindingFlags = bfPublicInstance;
		else
			bindingFlags = bfNonPublicInstance;

		ci->IsStatic;

		try
		{
			mt->GetConstructor(bindingFlags, NULL, GetParamTypeArray(ci), NULL);
			
		}
		catch (...)
		{
			return false;
		}

		// Get Parameter Info
		ParameterInfo * paramInfo[] = ci->GetParameters();

		// Check Parameter Info
		for (int parCount = 0; parCount < paramInfo->Count; parCount++)
			if (!isTypeHandled(paramInfo[parCount]->ParameterType))
				return false;
		return true;
	}

	//*************************************************************************
	// Method:		GetParamTypeArray (overloaded)
	// Description: Returns a parameter type array for the given method
	//
	// Parameters:
	//	mi - the method for which the type array is required
	//
	// Return Value: A type array for the method's parameters
	//*************************************************************************	
	Type * AddCustIntWizPg1::GetParamTypeArray(MethodInfo * mi)[]
	{
		ParameterInfo * params[] = mi->GetParameters();
		if (!params || params->Length == 0)
			return new Type*[0];

		Type * typeArray[] = new Type*[params->Length];
		for (int i=0; i < params->Length; i++)
			typeArray->SetValue (params[i]->ParameterType, i);

		return typeArray;
	}

	//*************************************************************************
	// Method:		GetParamTypeArray (overloaded)
	// Description: Returns a parameter type array for the given Constructor
	//
	// Parameters:
	//	ci - the constructor for which the type array is required
	//
	// Return Value: A type array for the constructors's parameters
	//*************************************************************************	
	Type * AddCustIntWizPg1::GetParamTypeArray(ConstructorInfo * ci)[]
	{
		ParameterInfo * params[] = ci->GetParameters();
		if (!params || params->Length == 0)
			return new Type*[0];

		Type * typeArray[] = new Type*[params->Length];
		for (int i=0; i < params->Length; i++)
			typeArray->SetValue (params[i]->ParameterType, i);

		return typeArray;
	}

	//*************************************************************************
	// Method:		waitDialogCancelEventHandlerFunc
	// Description: Handler function Cancel button click event in wait dialog
	//*************************************************************************	
	void AddCustIntWizPg1::waitDialogCancelEventHandlerFunc(Object * sender, EventArgs *e)
	{
		this->queryThread->Abort();
	}

	//*************************************************************************
	// Method:		GetLibraryInformationThreadFunc
	// Description: Thread function which gets information from the library
	//*************************************************************************	
	void AddCustIntWizPg1::GetLibraryInformationThreadFunc()
	{
		try
		{
			libraryInformation * libInfo;
			SortedList * functionNodes;
			SortedList * publicNodes = new SortedList();
			SortedList * privateNodes = new SortedList();
			SortedList * typeNodes = NULL;
			SortedList * moduleNodes = NULL;

			//Load the specified library in the PE loader
			const __wchar_t __pin *fileName = PtrToStringChars(selectFileDialog->FileName);
			PEFile * peFile = new PEFile();
			if (!peFile->load (fileName, true))
			{
//				if (waitDialog)
//					waitDialog->Close();
				
				MessageBox::Show ("Could not load the library in the PE Loader", "Holodeck");
				return;
			}

			//Create a DLL name node
			TreeNode * asmNode = new TreeNode (IO::Path::GetFileName(selectFileDialog->FileName));

			//Save the full path to the library in the Node Tag
			libInfo = new libraryInformation (selectFileDialog->FileName, peFile->isManagedExecutable());
			asmNode->Tag = libInfo;

			//Enumerate function names and add to the DLL name node
			if (!peFile->isManagedExecutable())
			{
				SiArray<SiString> funcs = peFile->enumerateNativeFunctions();
				
				if (funcs.GetLength() > 0)
				{
					//Add function names to the sorted list
					functionNodes = new SortedList();
					for (unsigned int i=0; i < funcs.GetLength(); i++)
					{
						TreeNode * addedNode = new TreeNode((char *)funcs[i]);
						functionInformation * funcInfo = new functionInformation ((char *)funcs[i], selectFileDialog->FileName, false);
						addedNode->Tag = funcInfo;
						AddToList (functionNodes, addedNode);
					}
					//Add sorted function nodes to the dll node
					for (int addfuncs=0; addfuncs < functionNodes->Count; addfuncs++)
						asmNode->Nodes->Add (dynamic_cast <TreeNode*> (functionNodes->GetByIndex(addfuncs)));

					//Update native function definitions
					UpdateNativeFunctionDefinitions (asmNode);
				}
				else
				{
					if (waitDialog)
						waitDialog->Close();

					MessageBox::Show (S"The library that you selected is either of an unrecognized type or does not contain any methods that Holodeck can intercept. This library will not be added to the list.", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Information);
					return;
				}
			}
			else
			{
				//Declare Binding Flags
				bfPublicStatic = static_cast <BindingFlags> (BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::Static);
				bfPublicInstance = static_cast <BindingFlags> (BindingFlags::DeclaredOnly|BindingFlags::Public|BindingFlags::Instance);
				bfNonPublicStatic = static_cast <BindingFlags> (BindingFlags::DeclaredOnly|BindingFlags::NonPublic|BindingFlags::Static);
				bfNonPublicInstance = static_cast <BindingFlags> (BindingFlags::DeclaredOnly|BindingFlags::NonPublic|BindingFlags::Instance);

				//Load the assembly from the file and get the types within
				Assembly * asmLibrary = Assembly::LoadFrom (selectFileDialog->FileName);
				
				//Create the module node
				Module * asmModules[] = asmLibrary->GetModules (false);
				
				if (asmModules->Count == 0)
				{
					if (waitDialog)
						waitDialog->Close();

					MessageBox::Show (S"This library does not contain any modules that Holodeck can enumerate.", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Information);
					return;
				}

				moduleNodes = new SortedList();
				for (int countModule=0; countModule < asmModules->Count; countModule++)
				{
					Module * module = asmModules[countModule];
					TreeNode * moduleNode = new TreeNode (module->Name);
					
					Type * moduleTypes[] = NULL;
					 moduleTypes = module->GetTypes();

					typeNodes = new SortedList();
					for (int i=0; i < moduleTypes->Count; i++)
					{
//						this->UpdateWaitDialog (countModule + 1, asmModules->Count, (i * 100) / moduleTypes->Count);

						if (!ShouldIncludeType(moduleTypes[i]))
							continue;

						TreeNode * typeNode = new TreeNode (moduleTypes[i]->Name);
						//Create "Public" Node
						TreeNode * publicNode = new TreeNode ("Public");
						
						//Declare variables used
						functionInformation * funcInfo = NULL;
						TreeNode * addedNode = NULL;

						//Enumerate all constructors and methods
						ConstructorInfo * publicStaticConstructors[] = moduleTypes[i]->GetConstructors (bfPublicStatic);
						ConstructorInfo * publicInstanceConstructors[] = moduleTypes[i]->GetConstructors (bfPublicInstance);
						ConstructorInfo * nonpublicStaticConstructors[] = moduleTypes[i]->GetConstructors (bfNonPublicStatic);
						ConstructorInfo * nonpublicInstanceConstructors[] = moduleTypes[i]->GetConstructors (bfNonPublicInstance);
						MethodInfo * publicMembersInstance[] = moduleTypes[i]->GetMethods (bfPublicInstance);
						MethodInfo * publicMembersStatic[] = moduleTypes[i]->GetMethods (bfPublicStatic);
						MethodInfo * nonpublicMembersInstance[] = moduleTypes[i]->GetMethods (bfNonPublicInstance);
						MethodInfo * nonpublicMembersStatic[] = moduleTypes[i]->GetMethods (bfNonPublicStatic);
						
						publicNodes->Clear();
						privateNodes->Clear();
						
						for (int pc=0; pc < publicStaticConstructors->Count; pc++)
						{
							ConstructorInfo * cInfo = publicStaticConstructors[pc];
							if (!isConstructorHandled(cInfo, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (cInfo->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, cInfo->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, true,  true, cInfo->IsStatic, GetParamTypeArray(cInfo), libInfo);
							AddToList (publicNodes, addedNode);
						}
						for (int pc=0; pc < publicInstanceConstructors->Count; pc++)
						{
							ConstructorInfo * cInfo = publicInstanceConstructors[pc];
							if (!isConstructorHandled(cInfo, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (cInfo->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, cInfo->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, true,  true, cInfo->IsStatic, GetParamTypeArray(cInfo), libInfo);
							AddToList (publicNodes, addedNode);
						}

						for (int pmcounter=0; pmcounter < publicMembersInstance->Count; pmcounter++)
						{
							MethodInfo * publicMember = publicMembersInstance[pmcounter];
							if (!isMethodHandled(publicMember, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (publicMember->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, publicMember->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, false,  true, publicMember->IsStatic, GetParamTypeArray(publicMember), libInfo);
							AddToList (publicNodes, addedNode);
						}
						for (int pmcounter=0; pmcounter < publicMembersStatic->Count; pmcounter++)
						{
							MethodInfo * publicMember = publicMembersStatic[pmcounter];
							if (!isMethodHandled(publicMember, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (publicMember->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, publicMember->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, false,  true, publicMember->IsStatic, GetParamTypeArray(publicMember), libInfo);
							AddToList (publicNodes, addedNode);
						}
						//Add public nodes to the public tree node
						for (int addPublicNodes=0; addPublicNodes < publicNodes->Count; addPublicNodes++)
							publicNode->Nodes->Add (dynamic_cast <TreeNode *> (publicNodes->GetByIndex(addPublicNodes)));
						
						//Add public tree node to the type node
						typeNode->Nodes->Add (publicNode);

						//Create "Non-public" Node
						TreeNode * nonpublicNode = new TreeNode ("Non-Public");

						for (int npc = 0; npc< nonpublicStaticConstructors->Count; npc++)
						{
							ConstructorInfo * npcInfo = nonpublicStaticConstructors[npc];
							if (!isConstructorHandled(npcInfo, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (npcInfo->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, npcInfo->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, true,  false, npcInfo->IsStatic, GetParamTypeArray(npcInfo), libInfo);
							AddToList (privateNodes, addedNode);
						}
						for (int npc = 0; npc< nonpublicInstanceConstructors->Count; npc++)
						{
							ConstructorInfo * npcInfo = nonpublicInstanceConstructors[npc];
							if (!isConstructorHandled(npcInfo, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (npcInfo->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, npcInfo->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, true,  false, npcInfo->IsStatic, GetParamTypeArray(npcInfo), libInfo);
							AddToList (privateNodes, addedNode);
						}
						for (int npmcounter = 0; npmcounter < nonpublicMembersInstance->Count; npmcounter++)
						{
							MethodInfo * nonpublicMember = nonpublicMembersInstance[npmcounter];
							if (!isMethodHandled(nonpublicMember, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (nonpublicMember->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, nonpublicMember->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, false,  false, nonpublicMember->IsStatic, GetParamTypeArray(nonpublicMember), libInfo);
							AddToList (privateNodes, addedNode);
						}
						for (int npmcounter = 0; npmcounter < nonpublicMembersStatic->Count; npmcounter++)
						{
							MethodInfo * nonpublicMember = nonpublicMembersStatic[npmcounter];
							if (!isMethodHandled(nonpublicMember, moduleTypes[i]))
								continue;
							addedNode = new TreeNode (nonpublicMember->Name);
							AddInformationToNode (addedNode, moduleTypes[i]->FullName, nonpublicMember->Name, selectFileDialog->FileName, asmModules[countModule]->ScopeName, false,  false, nonpublicMember->IsStatic, GetParamTypeArray(nonpublicMember), libInfo);
							AddToList (privateNodes, addedNode);
						}
						//Add non-public nodes to the non-public tree node
						for (int addNonPublicNodes=0; addNonPublicNodes < privateNodes->Count; addNonPublicNodes++)
							nonpublicNode->Nodes->Add (dynamic_cast <TreeNode *> (privateNodes->GetByIndex(addNonPublicNodes)));

						//Add nonpublic tree node to the type node
						typeNode->Nodes->Add (nonpublicNode);
						
						//Add type node to the type node list
						AddToList (typeNodes, typeNode);
					}

					for (int addTypes=0; addTypes < typeNodes->Count; addTypes++)
						moduleNode->Nodes->Add (dynamic_cast <TreeNode*> (typeNodes->GetByIndex (addTypes)));
					
					//Add the module node to the module nodes sorted list
					AddToList (moduleNodes, moduleNode);
				}

				//Add the module nodes to the asm node
				for (int addModules=0; addModules < moduleNodes->Count; addModules++)
					asmNode->Nodes->Add (dynamic_cast <TreeNode*> (moduleNodes->GetByIndex (addModules)));
			}
			//Add asm node to the main treeview
			this->nodeToAdd = asmNode;

//			if (waitDialog)
//				waitDialog->Close();
		}
		catch (System::Reflection::ReflectionTypeLoadException *e)
		{
			System::Exception __gc *earr __gc [] = e->LoaderExceptions;
			for (int i = 0; i < earr->Count; i++)
			{
				if (i > 0)
					break;

				System::Windows::Forms::MessageBox::Show(earr[i]->Message, \
														 earr[i]->Source, \
														 MessageBoxButtons::OK,\
														 MessageBoxIcon::Exclamation);
			}

			MessageBox::Show (S"There was a problem accessing information from the selected file. Please make sure that this file is not on a network drive and that you have the required security privileges to access this file.", S"Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
		}
		catch (ThreadAbortException *)
		{
			this->nodeToAdd = NULL;

			if (waitDialog)
				waitDialog->Close();			
		}
		catch (Exception *e)
		{
			this->nodeToAdd = NULL;

			System::Windows::Forms::MessageBox::Show(e->Message, \
													 e->Source, \
													 MessageBoxButtons::OK,\
													 MessageBoxIcon::Exclamation);
			MessageBox::Show (S"There was a problem accessing information from the selected file. Please make sure that this file is not on a network drive and that you have the required security privileges to access this file.", S"Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			
			if (waitDialog)
				waitDialog->Close();

			return;
		}
	}

	//*************************************************************************
	// Method:		UpdateWaitDialog
	// Description: Updates the information in the wait dialog
	//
	// Parameters:
	//	CurrentModule - count of the current module
	//	totalModules - count of the total modules in the library
	//	progressValue - the progress made in the current module
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::UpdateWaitDialog(int CurrentModule, int totalModules, int progressValue)
	{
		if (waitDialog)
		{
			String * msg = "Getting information from library";
			msg = String::Concat (msg, S"\nReading module ", CurrentModule.ToString(), S" of ", totalModules.ToString());
			waitDialog->Message = msg;
			waitDialog->Progress = progressValue;
		}
	}

	//*************************************************************************
	// Method:		AddInformationToNode
	// Description: Adds the information provided to the treenode specified
	//
	// Parameters:
	//	node	- the treenode to which the information should be added
	//	moduleName - name of the module within the assembly
	//	memberName - name of the module member
	//	scopeName - name of the module scope
	//	isConstructor - whether member is a constructor
	//	isPublic - whether member is public
	//	isStatic - whether member is static
	//	paramArray - array of parameters for the member
	//	libInfo - library information for the module
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::AddInformationToNode(TreeNode *node, String * moduleName, String * memberName, String * libraryName, String * scopeName, bool isConstructor, bool isPublic, bool isStatic, Type * paramArray[], libraryInformation * libInfo)
	{
		String * fullFunctionName = String::Concat (moduleName, ".", memberName->Replace(S".", S""));
		functionInformation * funcInfo = new functionInformation (fullFunctionName, libraryName, true);
		funcInfo->isConstructor = isConstructor;
		funcInfo->moduleName = scopeName;
		funcInfo->typeName = moduleName;
		
		if (isPublic)
		{
			if (isStatic)
				funcInfo->bindingFlags = bfPublicStatic;
			else
				funcInfo->bindingFlags = bfPublicInstance;
		}
		else
		{
			if (isStatic)
				funcInfo->bindingFlags = bfNonPublicStatic;
			else
				funcInfo->bindingFlags = bfNonPublicInstance;
		}
		
		funcInfo->paramTypeArray = paramArray;

		//Check to see if function definition(s) exists in the dotnet HD database
		ArrayList * alFunc = new ArrayList();
		alFunc->Add (funcInfo);
		ArrayList * definitions = getFunctionsInformation (alFunc, libInfo, dotNetInterceptedFunctionDB);
		if (definitions->Count > 0)
		{
			definitionInfo * defInfo = dynamic_cast <definitionInfo *> (definitions->Item[0]);
			funcInfo->selectedDefinition = defInfo->definition;
			funcInfo->possibleDefinitions = definitions;
		}
		else
			funcInfo->selectedDefinition = NULL;

		//Save managed information in its treeview node
		node->Tag = funcInfo;
	}

	//*************************************************************************
	// Method:		AddToList
	// Description: Adds the given node to the given list, making sure to add 
	//				a unique key
	//
	// Parameters:
	//	list	- the list to add the node to
	//  node	- the node to add to the list
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::AddToList (SortedList * list, TreeNode * node)
	{
		if (!list || !node)
			return;

		String * key = node->Text;
		while (list->ContainsKey (key))
			key = String::Concat (key, "q");

		list->Add (key, node);
	}

	//*************************************************************************
	// Method:		lnkCleanNoDefs_LinkClicked
	// Description: Handler method for the click event of the link
	//
	// Parameters:
	//	sender	- the event sender
	//  e	- the event arguments
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg1::lnkCleanNoDefs_LinkClicked(Object * sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs * e)
	{
		if (!selectedFunctions)
			return;

		//Remove event handler
		FuncLibTreeView->AfterCheck -= FuncLibTreeView_AfterCheckHandler;

		ArrayList * removeArray = new ArrayList();

		for (int i=0; i < selectedFunctions->Count; i++)
		{
			TreeNode * node = dynamic_cast <TreeNode *> (selectedFunctions->Item[i]);
			if (!node)
				continue;

			if (node->Tag != NULL)
			{
				functionInformation * fInfo = dynamic_cast <functionInformation *> (node->Tag);
				if (!fInfo)
					break;

				if (fInfo->selectedDefinition == NULL)
					removeArray->Add (node);
			}
		}

		for (int i=0 ; i < removeArray->Count; i++)
		{
			TreeNode * node = dynamic_cast <TreeNode *> (removeArray->Item[i]);
			node->Checked = false;
			selectedFunctions->Remove (node);
		}

		//Add event handler
		FuncLibTreeView->AfterCheck += FuncLibTreeView_AfterCheckHandler;
	}
}