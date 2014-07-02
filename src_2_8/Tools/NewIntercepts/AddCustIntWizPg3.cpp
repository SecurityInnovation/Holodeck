//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg3.cpp
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
#include "AddCustIntWizPg3.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		AddCustIntWizPg3
	// Description: Constructor for the AddCustIntWizPg3 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg3::AddCustIntWizPg3() : WizardPage()
	{
		this->loadNativeFunctionDatabase();
		this->loadDotNetFunctionDatabase();

		this->Dock = DockStyle::Fill;
		this->SubTitle = "Please review the information below and click 'Next' to generate the replacement libraries. Click 'Back' to make any changes.";

		grpMain = new GroupBox();
		rtbDetails = new RichTextBox();
		lblProgress = new Label();
		
		//
		// lblProgress
		//
		this->lblProgress->Name = "lblProgress";
		this->lblProgress->Size = System::Drawing::Size(312, 96);
		this->lblProgress->BackColor = System::Drawing::SystemColors::ActiveCaption;
		this->lblProgress->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
		this->lblProgress->FlatStyle = System::Windows::Forms::FlatStyle::Popup;
		this->lblProgress->ForeColor = System::Drawing::SystemColors::ActiveCaptionText;
		this->lblProgress->Text = "Please Wait";
		this->lblProgress->TextAlign = System::Drawing::ContentAlignment::MiddleCenter;
		this->lblProgress->Visible = false;
		
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
		this->grpMain->Controls->Add (lblProgress);

		// 
		// AddCustIntWizPg3
		// 
		this->Controls->Add(this->grpMain);
		this->ClientSize = System::Drawing::Size(896, 486);
		this->Name = "AddCustIntWizPg3";
		this->ResumeLayout(false);

		this->lblProgress->Top = ((this->lblProgress->Parent->Height /2 ) - (this->lblProgress->Height /2));
		this->lblProgress->Left = ((this->lblProgress->Parent->Width /2 ) - (this->lblProgress->Width /2));
	}

	//*************************************************************************
	// Method:		~AddCustIntWizPg3
	// Description: Destructor for the AddCustIntWizPg3 class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	AddCustIntWizPg3::~AddCustIntWizPg3()
	{

	}

	//*************************************************************************
	// Method:		updateDetails
	// Description: Displays code generation information on a RichtextBox
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg3::updateDetails()
	{
		
		//Assign directory locations for the projects
		this->assignDirectoryLocations();
		
		//Clear the old details
		this->rtbDetails->Clear();
		//Add the new details
		this->setRtbTextStyle (14, FontStyle::Bold, SystemColors::WindowText);
		this->rtbDetails->AppendText ("The following projects will be generated:\r\n\r\n");
		
		IEnumerator * enumerator = projectsTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList * functions = dynamic_cast <ArrayList *> (projectsTable->Item[enumerator->Current]);
			functionInformation * fInfo = dynamic_cast <functionInformation *> (functions->Item[0]);
			InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (fInfo->selectedDefinition);

			String * replacementDllName = this->getReplacementLibraryName (dynamic_cast <String *> (projectsLocations->Item[enumerator->Current->ToString()]));

			this->setRtbTextStyle (10, FontStyle::Bold, SystemColors::WindowText);
			this->rtbDetails->AppendText ("Project location: ");
			this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
			this->rtbDetails->AppendText (String::Concat (dynamic_cast <String *> (projectsLocations->Item[enumerator->Current->ToString()]), "\r\n"));

			this->setRtbTextStyle (10, FontStyle::Bold, SystemColors::WindowText);
			this->rtbDetails->AppendText ("Original library: ");
			this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
			this->rtbDetails->AppendText (String::Concat (ifunc->OriginalDll, "\r\n"));
			
			this->setRtbTextStyle (10, FontStyle::Bold, SystemColors::WindowText);
			this->rtbDetails->AppendText ("Replacement library: ");
			this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
			this->rtbDetails->AppendText (String::Concat (replacementDllName, "\r\n"));
			
			this->setRtbTextStyle (10, FontStyle::Bold, SystemColors::WindowText);
			this->rtbDetails->AppendText ("Functions to intercept:\r\n");
			StringBuilder * strBld = new StringBuilder();
			for (int i=0; i < functions->Count; i++)
			{
				functionInformation * fInfo = dynamic_cast <functionInformation *> (functions->Item[i]);
				InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (fInfo->selectedDefinition);
				fInfo->replacementLibraryName = replacementDllName;
				ifunc->InterceptedDll = replacementDllName;
				this->setRtbTextStyle (10, FontStyle::Regular, SystemColors::WindowText);
				strBld->Append ("\t\t\t");
				strBld->Append (ifunc->Name);
				strBld->Append ("\r\n");
			}
			this->rtbDetails->AppendText (strBld->ToString());
			this->rtbDetails->AppendText ("\r\n\r\n");
		}
	}

	//*************************************************************************
	// Method:		getDefaultReplacementLibraryName
	// Description: Constructs a replacement library name from a library name
	//				Example:
	//					Library Name = kernel32.dll
	//					Replacement Name = kernel32ReplacementLibrary.dll
	//
	// Parameters:
	//	originalLibraryName - Original library name
	//
	// Return Value: (String) Replacement library name
	//*************************************************************************
	String * AddCustIntWizPg3::getDefaultReplacementLibraryName (String * originalLibraryName)
	{
		String * replacementDll = Path::GetFileNameWithoutExtension (originalLibraryName);
		String * libExt = originalLibraryName->Replace (replacementDll, "");
		replacementDll = String::Concat (replacementDll, "ReplacementLibrary", libExt);
		return replacementDll;
	}

	//*************************************************************************
	// Method:		assignDirectoryLocations
	// Description: Assigns directories for the projects which will be generated.
	//				Checks if directories can be created with project name, if not
	//				appends dir names with an unused number
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg3::assignDirectoryLocations()
	{
		if (!reservedNames)
		{
			reservedNames = new ArrayList(dotNetInterceptedFunctionDB->GetReplacementDLLArray()->Count + nativeInterceptedFunctionDB->GetReplacementDLLArray()->Count);

			for (int i=0; i < dotNetInterceptedFunctionDB->GetReplacementDLLArray()->Count; i++)
				reservedNames->Add (dynamic_cast <String *> (dotNetInterceptedFunctionDB->GetReplacementDLLArray()->Item[i])->ToLower());
			
			for (int i=0; i < nativeInterceptedFunctionDB->GetReplacementDLLArray()->Count; i++)
				reservedNames->Add (dynamic_cast <String *> (nativeInterceptedFunctionDB->GetReplacementDLLArray()->Item[i])->ToLower());
		}

		//Create a new locations table
		projectsLocations = new Hashtable();
		IEnumerator * enumerator = projectsTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			String * keyName = enumerator->Current->ToString()->Trim();
			String * replacementPathName = Path::GetFileNameWithoutExtension(this->getDefaultReplacementLibraryName (keyName));
			String * startingName = replacementPathName;
			String * replacementPath = String::Concat (this->projectPath, startingName);
			//Get an unused replacementPath by adding numbers to the end (if required)
			int i=0;
			while (Directory::Exists (replacementPath) || reservedNames->Contains (String::Concat (startingName->ToLower(), S".dll")))
			{
				i++;
				startingName = String::Concat (replacementPathName, i.ToString());
				replacementPath = String::Concat (this->projectPath, startingName);
			}
			reservedNames->Add (startingName->ToLower());

			//Save the location in the table
			projectsLocations->Add (keyName, String::Concat (replacementPath, Path::DirectorySeparatorChar.ToString()));
		}
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
	void AddCustIntWizPg3::setRtbTextStyle (float newFontSize, System::Drawing::FontStyle newFontStyle, System::Drawing::Color newFontColor)
	{
		//If every param passed is NULL, then reset font to rtb default
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
	// Method:		setProgressText
	// Description: Sets the supplied text in the progress panel (which ishardly seen
	//				since computers are so fast these days :p)
	//
	// Parameters:
	//	newFontSize - font size
	//	newFontStyle - font style
	//	newFontColor - font color
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg3::setProgressText(String * progressText)
	{
		this->lblProgress->Text = progressText;
	}

	//*************************************************************************
	// Method:		generateCode
	// Description: Calls the appropriate code generator for each project and
	//				generates projects containing interception code.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg3::generateCode()
	{
		this->lblProgress->Visible = true;

		//Cycle through the projects
		IEnumerator * enumerator = projectsTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			//Set the progress text in the popup
			setProgressText (String::Concat ("Generating project for ", enumerator->Current->ToString(), "..."));

			//Get information from the first function to check if this library is Managed or Native
			ArrayList * funcsToIntercept = dynamic_cast <ArrayList *> (this->projectsTable->Item[enumerator->Current]);
			ArrayList * interceptArray = this->getInterceptedFunctionArray (funcsToIntercept);
			functionInformation * fInfo = dynamic_cast <functionInformation *> (funcsToIntercept->Item[0]);

			//Construct path strings
			String * pPath = dynamic_cast <String *> (this->projectsLocations->Item[enumerator->Current->ToString()]);
			String * projectName = Path::GetFileNameWithoutExtension(fInfo->replacementLibraryName);

			//Create appropriate project
			if (fInfo->isManaged)
			{
				pPath = String::Concat (pPath, projectName, ".csproj");
				//Initialize DotNet Code Generator
				DotNetCodeGenerator * dncGen = new DotNetCodeGenerator(true);
				//Create DotNet project
				ArrayList * libsList = new ArrayList();
				libsList->Add (fInfo->libraryName);
				dncGen->CreateProject (pPath, libsList, interceptArray);
			}
			else
			{
				pPath = String::Concat (pPath, projectName, ".vcproj");
				//Initialize Native Code Generator
				InterceptCodeGenerator * icGen = new InterceptCodeGenerator();
				//Create Native Project
				icGen->CreateProject (pPath, interceptArray);
			}
		}

		this->lblProgress->Visible = false;
	}

	//*************************************************************************
	// Method:		updateHolodeckDatabase
	// Description: Updates the appropriate Holodeck XML database with new
	//				intercepted function information
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg3::updateHolodeckDatabase()
	{
		String * fullPathToUserDirXMLFile = "";
		String *holodeckDBFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();

		IEnumerator * enumerator = projectsTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList * funcsToIntercept = dynamic_cast <ArrayList *> (this->projectsTable->Item[enumerator->Current]);
			ArrayList * interceptArray = this->getInterceptedFunctionArray (funcsToIntercept);
			functionInformation * fInfo = dynamic_cast <functionInformation *> (funcsToIntercept->Item[0]);
			
			String * projectDirectory = dynamic_cast <String *> (projectsLocations->Item[enumerator->Current->ToString()]);

			if (!fInfo->isManaged)
			{
				//Place the file in the Holodeck DB directory; and in the users output directory
				String * xmlFileName = String::Concat (NATIVE_USER_FILENAME_DB_PREFIX, fInfo->replacementLibraryName, ".xml");
				
				fullPathToUserDirXMLFile = String::Concat (projectDirectory, xmlFileName);
				HeaderParser::XmlGenerator * xmlGen = new HeaderParser::XmlGenerator (interceptArray, fullPathToUserDirXMLFile, false);
				xmlGen->writeFunctionsXml();
			}
			else
			{
				//locate the geenrated file and rename it
				String * projectName = Path::GetFileNameWithoutExtension (fInfo->replacementLibraryName);
				String * pPath = String::Concat (projectDirectory, projectName, ".xml");
				fullPathToUserDirXMLFile = String::Concat (projectDirectory, DOTNET_USER_FILENAME_DB_PREFIX, fInfo->replacementLibraryName, ".xml");
				if (File::Exists (pPath))
				{
					File::Copy (pPath, fullPathToUserDirXMLFile, true);
					File::Delete (pPath);
				}
				else
				{
					MessageBox::Show (String::Concat (S"Holodeck Database not updated. Update file ", pPath, S" not found"), S"Holodeck");
				}
			}

			//copy to the holodeck db folder
			File::Copy (fullPathToUserDirXMLFile, String::Concat (holodeckDBFolder, "\\", Path::GetFileName(fullPathToUserDirXMLFile)), true);
		}
	}

	//*************************************************************************
	// Method:		updateReplacementReference
	// Description: Adds .NET replacement libraries in the registry for reference
	//				use by DotNetCacheGen
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void AddCustIntWizPg3::updateReplacementReference()
	{
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		InterceptedFunctionDB *dotNetDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		if (!dotNetDB->IsLoaded)
			dotNetDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);
		
		IEnumerator * enumerator = projectsTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			ArrayList * funcsToIntercept = dynamic_cast <ArrayList *> (this->projectsTable->Item[enumerator->Current]);
			ArrayList * interceptArray = this->getInterceptedFunctionArray (funcsToIntercept);
			functionInformation * fInfo = dynamic_cast <functionInformation *> (funcsToIntercept->Item[0]);

			//Create .reg files in .NET project folders to be called by the post-build event
			//for that project
			if (fInfo->isManaged)
			{
				String * projDir = dynamic_cast <String *> (projectsLocations->Item[enumerator->Current->ToString()]);
				String * replacementDllName = this->getReplacementLibraryName (projDir);
				String * replDllKey = String::Concat (S"HKEY_LOCAL_MACHINE", S"\\", new String(ROOT_PATH), S"\\", new String(DOTNET_REPLACEMENT_DLLS_KEY));

				StreamWriter * regFile = File::CreateText (String::Concat (projDir, replacementDllName, S".reg"));
				regFile->WriteLine ("Windows Registry Editor Version 5.00");
				regFile->WriteLine (String::Concat (S"[", replDllKey, S"]"));
				regFile->WriteLine (String::Concat (S"\"", replacementDllName, S"\"=\"", fInfo->libraryName->Replace("\\", "\\\\"), S"\""));
				regFile->Flush();
				regFile->Close();
			}
		}

	}

	//*************************************************************************
	// Method:		getInterceptedFunctionArray
	// Description: Gets the selectedDefinition members from an array of 
	//				type functionInformation and returns an array of type
	//				InterceptedFunction
	//
	// Parameters:
	//	fInfoArray - an arraylist containing members of type functionInformation
	//
	// Return Value: (ArrayList) list of InterceptedFunction types
	//*************************************************************************
	ArrayList * AddCustIntWizPg3::getInterceptedFunctionArray(ArrayList * fInfoArray)
	{
		//Convert to an arraylist of InterceptFuntion types
		ArrayList * interceptArray = new ArrayList (fInfoArray->Count);
		for (int i=0; i < fInfoArray->Count; i++)
		{
			functionInformation * fInfo = dynamic_cast <functionInformation *> (fInfoArray->Item[i]);
			interceptArray->Add (fInfo->selectedDefinition);
		}
		return interceptArray;
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
	void AddCustIntWizPg3::loadDotNetFunctionDatabase()
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
	void AddCustIntWizPg3::loadNativeFunctionDatabase()
	{
		//Load the DotNet function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		nativeInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!nativeInterceptedFunctionDB->IsLoaded)
			nativeInterceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);
	}

	//*************************************************************************
	// Method:		getReplacementLibraryName
	// Description: Returns the replacement library name based on the project location
	//
	// Parameters:
	//	projectPath - project path
	//
	// Return Value: project name
	//*************************************************************************	
	String * AddCustIntWizPg3::getReplacementLibraryName (String * projectPath)
	{
		projectPath = projectPath->Trim();
		if (projectPath->EndsWith ("\\"))
			projectPath = projectPath->Substring (0, projectPath->Length -1 );

		int lastSlashIndex = projectPath->LastIndexOf ("\\");

		if (lastSlashIndex == -1)
			return String::Concat (projectPath, ".dll");
		else
			return String::Concat (projectPath->Substring (lastSlashIndex + 1), ".dll");
	}
}