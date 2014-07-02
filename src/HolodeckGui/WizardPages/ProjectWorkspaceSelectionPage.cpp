//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectWorkspaceSelectionPage.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectWorkspaceSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ProjectWorkspaceSelectionPage.h"
#include "defines.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		ProjectWorkspaceSelectionPage
	// Description: Constructor for the ProjectWorkspaceSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectWorkspaceSelectionPage::ProjectWorkspaceSelectionPage() : WizardPage()
	{
		this->Dock = DockStyle::Fill;
		this->SubTitle = "Select a name and location for this project";

		nameLabel = new Label();
		nameValueLabel = new Label();
		pathLabel = new Label();
		pathValueLabel = new Label();
		nameTextLabel = new Label();
		pathTextLabel = new Label();
		pathTextBox = new TextBox();
		nameTextBox = new TextBox();
		browseButton = new Button();
		saveDialog = new SaveFileDialog();
		
		//
		// nameTextLabel
		//
		this->nameTextLabel->Location = System::Drawing::Point(8, 10);
		this->nameTextLabel->Name = "nameTextLabel";
		this->nameTextLabel->Text = "Name:";
//		this->nameTextLabel->Size = System::Drawing::Size(40, 20);
		this->nameTextLabel->AutoSize = true;
		this->nameTextLabel->TabIndex = 0;

		// 
		// pathTextLabel
		// 
		this->pathTextLabel->Location = System::Drawing::Point(8, 44);
		this->pathTextLabel->Name = "pathTextLabel";
		this->pathTextLabel->Text = "Location:";
//		this->pathTextLabel->Size = System::Drawing::Size(40, 20);
		this->pathTextLabel->AutoSize = true;
		this->pathTextLabel->TabIndex = 1;

		// 
		// nameTextBox
		// 
		this->nameTextBox->Location = System::Drawing::Point(66, 10);
		this->nameTextBox->Name = "nameTextBox";
		this->nameTextBox->Size = System::Drawing::Size(122, 20);
		this->nameTextBox->TabIndex = 2;

		// 
		// pathTextBox
		// 
		this->pathTextBox->Location = System::Drawing::Point(66, 44);
		this->pathTextBox->Name = "pathTextBox";
		this->pathTextBox->Size = System::Drawing::Size(222, 20);
		this->pathTextBox->TabIndex = 3;
		
		// 
		// browseButton
		// 
		this->browseButton->Location = System::Drawing::Point(296, 44);
		this->browseButton->Name = "browseButton";
		this->browseButton->TabIndex = 4;
		this->browseButton->Text = "B&rowse...";
		// 
		// nameValueLabel
		// 
		this->nameValueLabel->Location = System::Drawing::Point(152, 100);
		this->nameValueLabel->Name = "nameValueLabel";
		this->nameValueLabel->Size = System::Drawing::Size(224, 30);
		this->nameValueLabel->TabIndex = 5;
		// 
		// pathValueLabel
		// 
		this->pathValueLabel->Location = System::Drawing::Point(152, 134);
		this->pathValueLabel->Name = "pathValueLabel";
		this->pathValueLabel->Size = System::Drawing::Size(224, 70);
		this->pathValueLabel->TabIndex = 6;
		// 
		// nameLabel
		// 
		this->nameLabel->Location = System::Drawing::Point(8, 100);
		this->nameLabel->Name = "nameLabel";
		this->nameLabel->Size = System::Drawing::Size(144, 30);
		this->nameLabel->TabIndex = 7;
		this->nameLabel->Text = "Your project\'s name is:";
		// 
		// pathLabel
		// 
		this->pathLabel->Location = System::Drawing::Point(8, 134);
		this->pathLabel->Name = "pathLabel";
		this->pathLabel->Size = System::Drawing::Size(144, 23);
		this->pathLabel->TabIndex = 8;
		this->pathLabel->Text = "Your project is located at:";
		// 
		// form1
		// 
		Controls->Add(pathLabel);
		Controls->Add(nameLabel);
		Controls->Add(pathValueLabel);
		Controls->Add(nameValueLabel);
		Controls->Add(browseButton);
		Controls->Add(pathTextLabel);
		Controls->Add(nameTextLabel);
		Controls->Add(pathTextBox);
		Controls->Add(nameTextBox);
		Controls->Add(pathLabel);

		browseButton->Click += new EventHandler(this, onBrowseClick);
		pathTextBox->TextChanged += new EventHandler(this, onPathChanged);
		nameTextBox->TextChanged += new EventHandler(this, onPathChanged);

		RegistryHelper *regHelper = RegistryHelper::getInstance();
		if (regHelper->GetLastProjectSavePath()) 
			ProjectFolder = regHelper->GetLastProjectSavePath();
	}

	//*************************************************************************
	// Method:		~ProjectWorkspaceSelectionPage
	// Description: Destructor for the ProjectWorkspaceSelectionPage class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectWorkspaceSelectionPage::~ProjectWorkspaceSelectionPage()
	{
	}	
	
	//*************************************************************************
	// Method:		onBrowseClick
	// Description: Event handler for browse button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectWorkspaceSelectionPage::onBrowseClick(Object *sender, EventArgs *e)
	{
		RegistryHelper *regHelper = RegistryHelper::getInstance();
	
		saveDialog->AddExtension = true;
		saveDialog->Filter = "Holodeck projects(*.hdp)|*.hdp";
		saveDialog->DefaultExt = "*.hdp";
		saveDialog->Title = "Pick location and file name for the project";
		saveDialog->CheckPathExists = true;
		if (regHelper->GetLastProjectSavePath()) 
			saveDialog->InitialDirectory = regHelper->GetLastProjectSavePath();

		if (saveDialog->ShowDialog (this) == DialogResult::OK)
		{
			ProjectPath = saveDialog->FileName;
		}
	}

	//*************************************************************************
	// Method:		onPathChanged
	// Description: Event handler for path text change
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ProjectWorkspaceSelectionPage::onPathChanged(Object *sender, EventArgs *e)
	{
		pathValueLabel->Text = ProjectFolder;
		nameValueLabel->Text = ProjectName;
	}
}
