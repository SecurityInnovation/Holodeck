//***********************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomTestProjectWizardControl.cpp
//
// DESCRIPTION: Contains implementation for the class CustomTestProjectWizardControl
//
//===================================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Feb 2004		 Ady K	  File created.
//***********************************************************************************
#include "CLRIntercept.h"
#include "CustomTestProjectWizardControl.h"

using namespace FunctionDatabase;

namespace CustomTestProjectGenerator
{
	//*************************************************************************
	// Method:		CustomTestProjectWizardControl
	// Description: Constructor for the CustomTestProjectWizardControl class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	CustomTestProjectWizardControl::CustomTestProjectWizardControl() : WizardControl()
	{
		//Get the various paths
		RegistryKey *key = Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		HolodeckInstallPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		CustomTestProjectGeneratorPath = dynamic_cast<String *> (key->GetValue(LIB_EXPORT_PATH_VALUE_NAME, new String(".")));
		CustomTestProjectGeneratorPath = String::Concat(HolodeckInstallPath, "\\", CustomTestProjectGeneratorPath);
		ReplacementDir = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));
		ReplacementDir = String::Concat(HolodeckInstallPath, "\\", ReplacementDir);
		functionDBFolder = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		functionDBFolder = String::Concat(HolodeckInstallPath, "\\", functionDBFolder);

		//Load the wizard image
		String * wizardImageFileName = String::Concat (CustomTestProjectGeneratorPath, "\\", "ctpWizardImg.bmp");
		System::Drawing::Image * wizImg = System::Drawing::Image::FromFile (wizardImageFileName);

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

		this->Title = "Create a Custom Test Project";
		cipLaunchPage = new CustomInterceptProject(wizImg);
		cipFunctionDetailsPage = new CIProjectDetails(wizImg); 
		cipProjectOptionsPage = new CIProjectOptions(wizImg);
		cipSummaryPage = new CIProjectSummary(wizImg);

		this->WizardPages->Add(cipLaunchPage);
		this->WizardPages->Add(cipFunctionDetailsPage);
		this->WizardPages->Add(cipProjectOptionsPage);
		this->WizardPages->Add(cipSummaryPage);

		currentWP = cipLaunchPage;

		this->WizardPageEnter += new WizardPageHandler(this, onPageEnter);
		this->WizardPageLeave += new WizardPageHandler(this, onPageLeave);
		this->NextClick += new CancelEventHandler(this, onNextClick);
		this->FinishClick += new EventHandler(this, onFinishClick);
		this->CancelClick += new EventHandler(this, onCancelClick);

	}

	//*************************************************************************
	// Method:		~CustomTestProjectWizardControl
	// Description: Destructor for the CustomTestProjectWizardControl class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	CustomTestProjectWizardControl::~CustomTestProjectWizardControl()
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
	void CustomTestProjectWizardControl::onPageEnter(WizardPage *wp, WizardControl *wc)
	{
		// set the title
		if (wp == cipLaunchPage)
		{
			wc->Title = "Create a Custom Test Project";
			this->EnableCancelButton = WizardControl::Status::Yes;
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::No;
			this->EnableFinishButton = WizardControl::Status::No;
		}
		else if (wp == cipFunctionDetailsPage)
		{
			wc->Title = "Create a Custom Test Project";
			this->EnableCancelButton = WizardControl::Status::Yes;
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;
		}
		else if (wp == cipProjectOptionsPage)
		{
			wc->Title = "Create a Custom Test Project";
			this->EnableCancelButton = WizardControl::Status::Yes;
			this->EnableNextButton = WizardControl::Status::Yes;
			this->EnableBackButton = WizardControl::Status::Yes;
			this->EnableFinishButton = WizardControl::Status::No;

			//Set appropriate project options
			cipProjectOptionsPage->SetCPPOption = cipFunctionDetailsPage->IsAUTNative;
			cipProjectOptionsPage->SetVBOption = false;
			cipProjectOptionsPage->SetCSOption = !cipFunctionDetailsPage->IsAUTNative;
			
			cipProjectOptionsPage->IsLangCSharp = !cipFunctionDetailsPage->IsAUTNative;
			cipProjectOptionsPage->IsLangCPP = cipFunctionDetailsPage->IsAUTNative;
		}
		else if (wp == cipSummaryPage)
		{
			wc->Title = "Create a Custom Test Project";
			this->EnableCancelButton = WizardControl::Status::No;
			this->EnableNextButton = WizardControl::Status::No;
			this->EnableBackButton = WizardControl::Status::No;
			this->EnableFinishButton = WizardControl::Status::Yes;

			//call the method which starts the project creation thread
			this->makeTestProject();
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
	void CustomTestProjectWizardControl::onPageLeave(WizardPage *wp, WizardControl *wc)
	{
		if (wp == cipProjectOptionsPage)
		{
			cipProjectOptionsPage->PageLeft();
		}
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
	void CustomTestProjectWizardControl::onNextClick(Object *sender, CancelEventArgs *e)
	{
		if (currentWP == cipLaunchPage)
		{
			
			//Error handling
			try
			{
				if (cipLaunchPage->IsLaunchAUTSelected && !File::Exists (cipLaunchPage->AutPath))
					throw new ArgumentException("Specified application does not exist, please enter a valid name and path");

				if (cipLaunchPage->IsAttactAUTSelected && !cipLaunchPage->IsAttachDotNet && !cipLaunchPage->IsAttachNative)
					throw new ArgumentException("Please select either Native or DotNet for your application type");
			}
			catch (ArgumentException *aE)
			{
				MessageBox::Show (aE->Message, "Holodeck");
				e->Cancel = true;
				return;
			}

			//Setting values for next page
			if (cipLaunchPage->IsLaunchAUTSelected)
			{
				const wchar_t __pin* wszValue = PtrToStringChars(cipLaunchPage->AutPath);
				if (CLRIsModuleManaged(wszValue))
					cipFunctionDetailsPage->IsAUTNative = false;
				else
					cipFunctionDetailsPage->IsAUTNative = true;
			}
			else
			{
				cipFunctionDetailsPage->IsAUTNative = cipLaunchPage->IsAttachNative;
			}
		}
		else if (currentWP == cipFunctionDetailsPage)
		{
			try
			{
				if (cipFunctionDetailsPage->SelectedFunctions->Count == 0)
					throw new ArgumentException("Please select at least one method for interception.");

			}
			catch (ArgumentException *aE)
			{
				MessageBox::Show (aE->Message, "Holodeck");
				e->Cancel = true;
				return;
			}
		}
		else if (currentWP == cipProjectOptionsPage)
		{
			try
			{
				if (cipProjectOptionsPage->ProjectName->Trim()->Length == 0)
					throw new ArgumentException("Please specify a name for your test project");
				
				if (!cipProjectOptionsPage->IsProjectNameValid())
					throw new ArgumentException("Invalid characters in project name field, please use only alphabets without spaces. Correct before proceeding");

				if (!Directory::Exists (cipProjectOptionsPage->ProjectLocation))
					throw new ArgumentException("Specified project location does not exist. Please specify a valid path.");
			}
			catch (ArgumentException *aE)
			{
				MessageBox::Show (aE->Message, "Holodeck");
				e->Cancel = true;
				return;
			}
			
			//Call the pageleft method for cleanup purposes
			cipProjectOptionsPage->PageLeft();
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
	void CustomTestProjectWizardControl::onFinishClick(Object *sender, EventArgs *e)
	{

	}

	//*************************************************************************
	// Method:		onCancelClick
	// Description: Event handler for cancel button click
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CustomTestProjectWizardControl::onCancelClick(Object *sender, EventArgs *e)
	{
		cipProjectOptionsPage->PageLeft();
	}

	//*************************************************************************
	// Method:		makeTestProject
	// Description: Launches the approrpiate function in a thread for project
	//				creation
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CustomTestProjectWizardControl::makeTestProject()
	{
		System::Threading::ThreadStart * threadStart;

		projectSummaryText ("Creating directories and generating code files...");

		//Load the appropriate function database
		if (this->cipFunctionDetailsPage->IsAUTNative)
		{
			functionDatabase = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
			if (!functionDatabase->IsLoaded)
				functionDatabase->LoadDatabase(functionDBFolder, FUNCTION_DB_FILE_NAME);
		}
		else
		{
			functionDatabase = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			if (!functionDatabase->IsLoaded)
				functionDatabase->LoadDatabase(functionDBFolder, DOT_NET_FUNC_DB_FILE_NAME);
		}

		//Combine the output location and project name into a project path
		ProjectPath = this->cipProjectOptionsPage->ProjectLocation;
		if (!ProjectPath->EndsWith(System::IO::Path::DirectorySeparatorChar.ToString()))
			ProjectPath = String::Concat (ProjectPath, System::IO::Path::DirectorySeparatorChar.ToString());
		ProjectPath = String::Concat (ProjectPath, this->cipProjectOptionsPage->ProjectName);


		//Create output directory
		if (!this->CreateOutputPath(ProjectPath))
		{
			projectSummaryText("There was a problem creating the output directory for the test project.");
			return;
		}

		//Add an ending slash to the ProjectPath
		if (!ProjectPath->EndsWith(System::IO::Path::DirectorySeparatorChar.ToString()))
			ProjectPath = String::Concat (ProjectPath, System::IO::Path::DirectorySeparatorChar.ToString());

		//Select the appropriate project thread
		if (this->cipProjectOptionsPage->IsLangCSharp)
			threadStart = new System::Threading::ThreadStart(this, CSharpProject_Thread);
		else if (this->cipProjectOptionsPage->IsLangVB)
			threadStart = new System::Threading::ThreadStart(this, VBProject_Thread);
		else if (this->cipProjectOptionsPage->IsLangCPP)
			threadStart = new System::Threading::ThreadStart(this, CPPProject_Thread);

		//Create and start the project thread
		System::Threading::Thread * projectThread = new System::Threading::Thread(threadStart);
		projectThread->Start();
	}

	//*************************************************************************
	// Method:		CSharpProject_Thread
	// Description: Thread function to handle creation of C# test projects
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CustomTestProjectWizardControl::CSharpProject_Thread()
	{
		this->EnableFinishButton = WizardControl::Status::No;

		this->cipSummaryPage->ProgressLevel = 0;

		//Create the control executable
		if (!this->csharp_CreateControlExe(ProjectPath, this->cipProjectOptionsPage->ProjectName, this->cipFunctionDetailsPage->SelectedFunctions))
		{
			projectSummaryText ("An error was encountered while generating code for the companion executable. Please consult with customer support");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 1;
		
		//Create thr project file for the control executable
		if (!this->csharp_CreateSolution(ProjectPath, this->cipProjectOptionsPage->ProjectName))
		{
			projectSummaryText ("An error was encountered while generating the project file for the companion executable. Please consult with customer support");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 2;

		//Create the interception library
		if (!this->csharp_CreateInterceptionLibrary(ProjectPath, this->cipProjectOptionsPage->ProjectName, this->cipFunctionDetailsPage->SelectedFunctions))
		{
			projectSummaryText ("An error was encountered while generating the interception library. Please consult with customer support");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 3;

		String * summaryText = "";
		summaryText = String::Concat (summaryText, "Your test project was created successfully and has been placed in:\r\n");
		summaryText = String::Concat (summaryText, "     ", ProjectPath, "\r\n\r\n");
		summaryText = String::Concat (summaryText, "This folder contains the following two project files:\r\n");
		summaryText = String::Concat (summaryText, "    -", this->cipProjectOptionsPage->ProjectName, ".csproj\r\n");
		summaryText = String::Concat (summaryText, "    -", this->cipProjectOptionsPage->ProjectName, "Dll.csproj\r\n\r\n");
		summaryText = String::Concat (summaryText, "The " , this->cipProjectOptionsPage->ProjectName, ".csproj project will build an executable file which you will use to test ");
		summaryText = String::Concat (summaryText, "your application. This project uses the HolodeckLib library to perform interception and testing. To customize your project ");
		summaryText = String::Concat (summaryText, "you can look up 'HolodeckLib' in the Holodeck Help and learn about the methods that you can call.\r\n\r\n");
		summaryText = String::Concat (summaryText, "The " , this->cipProjectOptionsPage->ProjectName, "Dll.csproj project will build a DLL file which will be used by the executable ");
		summaryText = String::Concat (summaryText, "mentioned above. This DLL contains interception routines for the methods you selected.");
		
		projectSummaryText (summaryText);
		this->cipSummaryPage->ProgressLevel = 4;
		
		this->EnableFinishButton = WizardControl::Status::Yes;
	}

	//*************************************************************************
	// Method:		VBProject_Thread
	// Description: Thread function to handle creation of VB test projects
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CustomTestProjectWizardControl::VBProject_Thread()
	{

	}

	//*************************************************************************
	// Method:		CPPProject_Thread
	// Description: Thread function to handle creation of mC++ test projects
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void CustomTestProjectWizardControl::CPPProject_Thread()
	{
		this->EnableFinishButton = WizardControl::Status::No;

		//Set progress counter to 0
		this->cipSummaryPage->ProgressLevel = 0;
		
		//Create the control executable
		if (!this->native_CreateControlExe(ProjectPath, this->cipProjectOptionsPage->ProjectName, this->cipFunctionDetailsPage->SelectedFunctions))
		{
			projectSummaryText ("An error was encountered while generating the control executable. Please consult with customer support");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 1;

		//Create the interception library
		if (!this->native_CreateInterceptionLibrary(ProjectPath, this->cipProjectOptionsPage->ProjectName, this->cipFunctionDetailsPage->SelectedFunctions))
		{
			projectSummaryText ("An error was encountered while generating the interception library. Please consult with customer support");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 2;

		//Create the project file
		if (!this->native_CreateSolution(ProjectPath, this->cipProjectOptionsPage->ProjectName))
		{
			projectSummaryText ("An error was encountered while generating the .vcproj file. Please consult with customer support");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 3;
		
		//Copy the required libraries
		if (!this->native_CopyLibraries(ProjectPath))
		{
			projectSummaryText ("An error was encountered while copying required library files to the output folder.");
			this->EnableFinishButton = WizardControl::Status::Yes;
			return;
		}
		this->cipSummaryPage->ProgressLevel = 4;

		String * summaryText = "";
		summaryText = String::Concat (summaryText, "Your test project was created successfully and has been placed in:\r\n");
		summaryText = String::Concat (summaryText, "     ", ProjectPath, "\r\n\r\n");
		summaryText = String::Concat (summaryText, "This folder contains the following two project files:\r\n");
		summaryText = String::Concat (summaryText, "    -", this->cipProjectOptionsPage->ProjectName, ".vcproj\r\n");
		summaryText = String::Concat (summaryText, "    -", this->cipProjectOptionsPage->ProjectName, "Dll.vcproj\r\n\r\n");
		summaryText = String::Concat (summaryText, "The " , this->cipProjectOptionsPage->ProjectName, ".vcproj project will build an executable file which you will use to test ");
		summaryText = String::Concat (summaryText, "your application. This project uses the HolodeckLib library to perform interception and testing. To customize your project ");
		summaryText = String::Concat (summaryText, "you can look up 'HolodeckLib' in the Holodeck Help and learn about the methods that you can call.\r\n\r\n");
		summaryText = String::Concat (summaryText, "The " , this->cipProjectOptionsPage->ProjectName, "Dll.vcproj project will build a DLL file which will be used by the executable ");
		summaryText = String::Concat (summaryText, "mentioned above. This DLL contains interception routines for the methods you selected.");
		
		projectSummaryText (summaryText);
		
		this->EnableFinishButton = WizardControl::Status::Yes;
	}

	//*************************************************************************
	// Method:		CreateOutputPath
	// Description: Creates the specified path. If path already exisits, asks the
	//				user permission to overwrite
	//
	// Parameters:
	//	ProjectPath - the path to create
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool CustomTestProjectWizardControl::CreateOutputPath(String * ProjectPath)
	{
		//Check if output directory exists, if not ask to create it
		if (!System::IO::Directory::Exists (ProjectPath))
		{
			try
			{
				System::IO::Directory::CreateDirectory (ProjectPath);
				return true;
			}
			catch (Exception *e)
			{
				MessageBox::Show (e->Message, "Holodeck");
				return false;
			}
		}
		else
		{
			System::Windows::Forms::DialogResult dirResult = System::Windows::Forms::MessageBox::Show ("The output directory already exists, some files maybe overwritten. Do you wish to proceed?", "Holodeck", MessageBoxButtons::YesNo);

			if (dirResult == DialogResult::Yes)
			{
				try
				{
					System::IO::Directory::Delete (ProjectPath, true);
					System::IO::Directory::CreateDirectory (ProjectPath);
					return true;
				}
				catch (Exception *e)
				{
					MessageBox::Show (String::Concat ("There was an error recreating the output directory. Please delete the folder manually and try again or specify a different location.\nError details:\n", e->Message), "Holodeck");
					return false;
				}
			}
			else
				return false;
		}
	}

	
	
	//*************************************************************************
	// Method:		projectSummaryText
	// Description: Sets the text in the summary page of the wizard.
	//
	// Parameters:
	//	text - the text to set
	//
	// Return Value: None
	//*************************************************************************	
	void CustomTestProjectWizardControl::projectSummaryText(String * text)
	{
		this->cipSummaryPage->SummaryText = text;
	}

	//*************************************************************************
	// Method:		native_CreateInterceptionLibrary
	// Description: Generates code for a native replacement library
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::native_CreateInterceptionLibrary(String * projectPath, String * projectName, ArrayList * interceptedFunctionList)
	{
		//Create a list of replacement libraries
		ArrayList * replLibs = new ArrayList();
		for (int i = 0; i < interceptedFunctionList->Count; i++)
		{
			InterceptedFunction * function = functionDatabase->GetFunctionByName (interceptedFunctionList->Item[i]->ToString());
			if (!replLibs->Contains (function->InterceptedDll))
				replLibs->Add (function->InterceptedDll);
		}

		//Create the interception header
		String * headerFileName = String::Concat (projectPath, projectName, "Dll.h");
		try
		{
			//Open the template file for reading
			IO::StreamReader * headerTemplate = System::IO::File::OpenText (String::Concat (CustomTestProjectGeneratorPath, "\\replacementLibH.dat"));
			headerTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);
      
			//Open the outout header file for writing
			IO::StreamWriter * headerFile = System::IO::File::CreateText (headerFileName);
			
			//Parse and replace strings and write to output file
			while (headerTemplate->Peek() > -1)
			{
				String * templateLine = headerTemplate->ReadLine ();
				String * replaceLine = ParseAndReplaceString (templateLine, NULL);
				if (replaceLine)
				{
					//If INSERT_MODULE_DECLARATIONS_FOR_LIBS command then
					if (String::Compare (replaceLine->Trim(), "@INSERT_MODULE_DECLARATIONS_FOR_LIBS", true) == 0)
					{
						for (int i = 0; i < replLibs->Count; i++)
						{
							String * replLibName = dynamic_cast <String *> (replLibs->Item[i]);
							String * moduleDeclString = String::Concat ("HMODULE ", this->GetHandleNameForReplLib(replLibName), " = NULL;");
							headerFile->WriteLine (moduleDeclString);
						}
					}
					else
						headerFile->WriteLine (replaceLine);
				}
			}

			//Close the template file
			headerTemplate->Close();

			//Define the intercepted functions
			headerFile->WriteLine ("");
			for (int i=0; i< interceptedFunctionList->Count; i++)
			{
				String * typedefStr = "typedef ";
				InterceptedFunction * function = functionDatabase->GetFunctionByName (interceptedFunctionList->Item[i]->ToString());
				if (function)
				{
					Array * modifiers = function->Modifiers;
					Array * params = function->Parameters;
					
					typedefStr = String::Concat (typedefStr, modifiers->Item[0]->ToString(), " ");			//Add Modifier
					typedefStr = String::Concat (typedefStr, function->ReturnType, " (");					// "  ReturnType
					typedefStr = String::Concat (typedefStr, modifiers->Item[1]->ToString(), " *");			// "  Second Modifier
					typedefStr = String::Concat (typedefStr, function->Name, "FuncType) (");				// "  Function Name
					for (int p = 0; p < params->Length; p++)												// "  Param Types
					{
						InterceptedFunctionParameter * param = dynamic_cast <InterceptedFunctionParameter *> (params->Item[p]);
						typedefStr = String::Concat (typedefStr, param->Type);
						if (p +1 != params->Length)
							typedefStr = String::Concat (typedefStr, ", ");
					}
					typedefStr = String::Concat (typedefStr, ");");
					
					headerFile->WriteLine (typedefStr);
				}

			}
			headerFile->Close();
			
		}
		catch (Exception * e)
		{
			MessageBox::Show (e->Message, "Holodeck");
			return false;
		}

		//Create the interception def file
		String * defFileName = String::Concat (projectPath, projectName, "Dll.def");
		try
		{
			//Open the outout def file for writing
			IO::StreamWriter * defFile = System::IO::File::CreateText (defFileName);
			
			
			//Write DEF file
			defFile->WriteLine (String::Concat ("LIBRARY ", projectName, "Dll"));
			defFile->WriteLine ("EXPORTS");
			for (int i=0; i< interceptedFunctionList->Count; i++)
				defFile->WriteLine (String::Concat(interceptedFunctionList->Item[i]->ToString(), "Replacement"));

			defFile->WriteLine ("");
			defFile->WriteLine ("ReplacementLibraryAttach");
			defFile->WriteLine ("ReplacementLibraryDetach");
			defFile->WriteLine ("");
			
			//Close the DEF file
			defFile->Close();
		}
		catch (Exception * e)
		{
			MessageBox::Show (e->Message, "Holodeck");
			return false;
		}

		//Create the interception cpp file
		String * cppFileName = String::Concat (projectPath, projectName, "Dll.cpp");
		try
		{
			//Open the template file for reading
			IO::StreamReader * cppHeaderTemplate = System::IO::File::OpenText (String::Concat (CustomTestProjectGeneratorPath, "\\replacementLibCpp.dat"));
			cppHeaderTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);
      
			//Open the outout header file for writing
			IO::StreamWriter * cppFile = System::IO::File::CreateText (cppFileName);

			//Parse and replace strings from the CPP Header file and write to output file
			while (cppHeaderTemplate->Peek() > -1)
			{
				String * templateLine = cppHeaderTemplate->ReadLine ();
				String * replaceLine = ParseAndReplaceString (templateLine, NULL);
				if (replaceLine)
				{
					//If CALL_ATTACH_IN_LIBS command then
					if (String::Compare (replaceLine->Trim(), "@CALL_ATTACH_IN_LIBS", true) == 0)
					{
						for (int libz=0; libz < replLibs->Count; libz++)
						{
							String * replLibName = dynamic_cast <String *> (replLibs->Item[libz]);
							String * tmpStr = String::Concat (S"\t", GetHandleNameForReplLib(replLibName), S" = LoadLibrary (\"", GetFullPathToReplLib(replLibName), S"\");");
							cppFile->WriteLine (tmpStr);

							tmpStr = String::Concat (S"\tvoid (*p", GetHandleNameForReplLib(replLibName), S")() = (void(*)()) GetProcAddress (", GetHandleNameForReplLib(replLibName), S", \"ReplacementLibraryAttach\");");
							cppFile->WriteLine (tmpStr);

							tmpStr = String::Concat (S"\tp", GetHandleNameForReplLib(replLibName), S"();");
							cppFile->WriteLine (tmpStr);

							cppFile->WriteLine (S"");
						}
					}
					//If CALL_DETACH_IN_LIBS command then
					else if (String::Compare (replaceLine->Trim(), "@CALL_DETACH_IN_LIBS", true) == 0)
					{
						for (int libz=0; libz < replLibs->Count; libz++)
						{
							String * replLibName = dynamic_cast <String *> (replLibs->Item[libz]);

							String *tmpStr = String::Concat (S"\tif (", GetHandleNameForReplLib(replLibName), S")");
							cppFile->WriteLine (String::Concat (tmpStr, "\n\t{"));

							tmpStr = String::Concat (S"\t\tvoid (*p", GetHandleNameForReplLib(replLibName), S")() = (void(*)()) GetProcAddress (", GetHandleNameForReplLib(replLibName), S", \"ReplacementLibraryDetach\");");
							cppFile->WriteLine (tmpStr);

							tmpStr = String::Concat (S"\t\tp", GetHandleNameForReplLib(replLibName), S"();");
							cppFile->WriteLine (String::Concat (tmpStr, S"\n\t}"));

							cppFile->WriteLine (S"");
						}
					}
					else
						cppFile->WriteLine (replaceLine);
				}
			}
			cppFile->WriteLine ("");
			cppFile->WriteLine ("");

			//Close the cpp header template file
			cppHeaderTemplate->Close();

			//Open the function template file
			IO::StreamReader * funcTemplate = System::IO::File::OpenText (String::Concat (CustomTestProjectGeneratorPath, "\\replacementLibFunc.dat"));
			funcTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);

			//Read data into buffer
			ArrayList * funcBuffer = new ArrayList();
			while (funcTemplate->Peek() > -1)
				funcBuffer->Add (funcTemplate->ReadLine());

			//Close the function template file
			funcTemplate->Close();

			//Write function code for each interception function
			for (int i=0; i< interceptedFunctionList->Count; i++)
			{
				InterceptedFunction * function = functionDatabase->GetFunctionByName (interceptedFunctionList->Item[i]->ToString());
				if (function)
				{
					for (int j=0; j<funcBuffer->Count; j++)
					{
						String * replaceLine = ParseAndReplaceString (funcBuffer->Item[j]->ToString(), function);
						if (replaceLine)
							cppFile->WriteLine (replaceLine);
					}

				}
			}

			//Close the cpp file
			cppFile->Close();
		}
		catch (Exception * e)
		{
			MessageBox::Show (e->Message, "Holodeck");
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		ParseAndReplaceString
	// Description: Performs replacement of tokens inside strings and replaces
	//				them with project specific strings
	//
	// Parameters:
	//	templateString	- the string read from a template source
	//	function - function info to use while parsing this string, can be NULL
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * CustomTestProjectWizardControl::ParseAndReplaceString(String * templateString, InterceptedFunction * function)
	{
		Array * modifiers;
		Array * params;

		if (function)
		{
			modifiers = function->Modifiers;
			params = function->Parameters;
		}

		//Check for @ COMMANDS
		//Command are handled in main loop
		if (templateString->Trim()->StartsWith("@INTERCEPT_FUNCTIONS"))
			return "@INTERCEPT_FUNCTIONS";
		else if (templateString->Trim()->StartsWith("@ATTACH_OR_LAUNCH"))
			return "@ATTACH_OR_LAUNCH";
		else if (templateString->Trim()->StartsWith("@CSHARP_ATTACH_OR_LAUNCH"))
			return "@CSHARP_ATTACH_OR_LAUNCH";

		//Check for IFs
		if (templateString->Trim()->StartsWith("#IF_RETURN_NONVOID#"))
		{
			if (String::Compare (function->ReturnType, "VOID", true) == 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_RETURN_NONVOID#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_RETURN_VOID#"))
		{
			if (String::Compare (function->ReturnType, "VOID", true) != 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_RETURN_VOID#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_PROCESS_CHAINING#"))
		{
			if (this->cipLaunchPage->IsProcessChainingEnabled == true)
			{
				templateString = templateString->Replace ("#IF_PROCESS_CHAINING#", "");
			}
			else
				return NULL;
		}

		templateString = templateString->Replace ("$USER_PROJECT_NAME$", this->cipProjectOptionsPage->ProjectName);
		templateString = templateString->Replace ("$HOLODECK_REPLACEMENT_DIR$", ReplacementDir->Replace ("\\", "\\\\"));
		templateString = templateString->Replace ("$LIB_EXPORT_PATH$", CustomTestProjectGeneratorPath->Replace("\\", "\\\\"));
		templateString = templateString->Replace ("$INSTALL_PATH$", HolodeckInstallPath->Replace("\\", "\\\\"));
		templateString = templateString->Replace ("$TEST_APPLICATION_PATH$", this->cipLaunchPage->AutPath->Replace("\\", "\\\\"));
		templateString = templateString->Replace ("$PROJECT_FOLDER$", this->cipProjectOptionsPage->ProjectLocation->Replace("\\", "\\\\"));
		templateString = templateString->Replace ("$CMD_LINE$", this->cipLaunchPage->AutCmdLine->Replace ("\\", "\\\\"));
		templateString = templateString->Replace ("$LOG_FILE$", String::Concat (this->cipProjectOptionsPage->ProjectLocation->Replace("\\", "\\\\"), "Logs"));

		if (function)
		{
			templateString = templateString->Replace ("$RETURNTYPE$", function->ReturnType);
			templateString = templateString->Replace ("$MODIFIER1$", modifiers->Item[0]->ToString());
			templateString = templateString->Replace ("$MODIFIER2$", modifiers->Item[1]->ToString());
			templateString = templateString->Replace ("$FUNCTIONNAME$", function->Name);
			templateString = templateString->Replace ("$PARAM_TYPES_AND_NAMES$", GetParamTypesAndNames (params));
			templateString = templateString->Replace ("$HANDLE_REPL_LIB$", GetHandleNameForReplLib (function->InterceptedDll));
			templateString = templateString->Replace ("$PARAM_NAMES$", GetParamNames(params));
		}
		
		return templateString;
	}

	//*************************************************************************
	// Method:		GetParamTypesAndNames
	// Description: Returns a string of the form:
	//				param1Type param1Name, param2Type param2Name, param3Type param3Name
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * CustomTestProjectWizardControl::GetParamTypesAndNames(Array * params)
	{
		String * returnStr = "";
		for (int i=0; i<params->Length; i++)
		{
				InterceptedFunctionParameter * param = dynamic_cast <InterceptedFunctionParameter *> (params->Item[i]);
				returnStr = String::Concat (returnStr, param->Type);
				//Remove "[]" from end of paramType and add it to end of paramName
				if (param->Type->EndsWith ("[]"))
					returnStr = returnStr->Remove (returnStr->Length - 2, 2);
				returnStr = String::Concat (returnStr, " ", param->Name);
				if (param->Type->EndsWith ("[]"))
					returnStr = String::Concat (returnStr, "[]");

				if (i+1 != params->Length)
					returnStr = String::Concat (returnStr, ", ");
		}
		
		return returnStr;
	}

	//*************************************************************************
	// Method:		GetHandleNameForReplLib
	// Description: Returns the name of dll without the extension and in lower case
	//
	// Parameters:
	//	replDllName	- the dll name
	//
	// Return Value: a lower-case dll name without extensions
	//*************************************************************************
	String * CustomTestProjectWizardControl::GetHandleNameForReplLib(String * replDllName)
	{
		return String::Concat (S"h_", replDllName->Replace (".dll", "")->ToLower());
	}

	//*************************************************************************
	// Method:		GetUniqueName
	// Description: Makes sure a faultName is unique, if a fault with the same
	//				name exists already, the function appends a number to the end
	//				of the name
	//
	// Parameters:
	//	faultName	- a fault name
	//
	// Return Value: A unique fault name
	//*************************************************************************
	String * CustomTestProjectWizardControl::GetUniqueName (String * faultName)
	{
		String * tmpStr;
		
		if (!uinqueNameList)
			uinqueNameList = new ArrayList();

		//If name already exists, then find a unique name
		if (uinqueNameList->Contains (faultName))
		{
			for (int i=1; i <= 10000; i++)
			{
				tmpStr = String::Concat (faultName, i.ToString());
				if (!uinqueNameList->Contains (tmpStr))
				{
					faultName = tmpStr;
					break;
				}
			}
		}
	
		//Add this name to the list
		uinqueNameList->Add (faultName);

		return faultName;
	}

	//*************************************************************************
	// Method:		GetValidCodeName
	// Description: Removes all non-alpha chars and replaces spaces with '_'
	//
	// Parameters:
	//	faultName	- a fault name
	//
	// Return Value: String with no non-alpha chars and no spaces
	//*************************************************************************
	String * CustomTestProjectWizardControl::GetValidCodeName (String * faultName)
	{
		String * retString = "";

		//Replace the spaces with '_'
		faultName = faultName->Replace (S" ", S"_");
		
		//Cycle through characters and save only alpha chars
		for (int i=0; i < faultName->Length; i++)
		{
			if (Char::IsLetter (faultName->Chars[i]) || faultName->Chars[i] == '_')
				retString = String::Concat (retString, faultName->Chars[i].ToString());
		}

		return retString;
	}

	//*************************************************************************
	// Method:		GetFullPathToReplLib
	// Description: Returns the full path to the specified library
	//
	// Parameters:
	//	replDllName	- the dll name
	//
	// Return Value: full path to the library
	//*************************************************************************
	String * CustomTestProjectWizardControl::GetFullPathToReplLib(String * replDllName)
	{
		return String::Concat (ReplacementDir, S"\\", replDllName)->Replace ("\\", "\\\\");
	}

	//*************************************************************************
	// Method:		GetParamNames
	// Description: Returns a string of the form:
	//				param1Name, param2Name, param3Name ....
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * CustomTestProjectWizardControl::GetParamNames(Array * params)
	{
		String * returnStr = "";
		for (int i=0; i< params->Length; i++)
		{
				InterceptedFunctionParameter * param = dynamic_cast <InterceptedFunctionParameter *> (params->Item[i]);
				returnStr = String::Concat (returnStr, param->Name);
				if (i+1 != params->Length)
					returnStr = String::Concat (returnStr, ", ");
		}
		
		return returnStr;
	}

	//*************************************************************************
	// Method:		native_CreateControlExe
	// Description: Generates code for a control exe.
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::native_CreateControlExe(String * ProjectPath, String * projectName, ArrayList * interceptedFunctionList)
	{
		//Create the control exe code and header files
		String * exeCodeFileName = String::Concat (ProjectPath, projectName, ".cpp");
		String * exeHeaderFileName = String::Concat (ProjectPath, projectName, ".h");
		try
		{
			//Open the template file for reading
			IO::StreamReader * exeCodeTemplate = System::IO::File::OpenText (String::Concat (this->CustomTestProjectGeneratorPath, "\\replacementExeCpp.dat"));
			exeCodeTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);
      
			//Open the output cpp file for writing
			IO::StreamWriter * exeCodeFile = System::IO::File::CreateText (exeCodeFileName);
			
			//Parse and replace strings and write to output file
			while (exeCodeTemplate->Peek() > -1)
			{
				String * templateLine = exeCodeTemplate->ReadLine ();
				String * replaceLine = ParseAndReplaceString (templateLine, NULL);
				if (replaceLine)
				{
					//If intercept command then
					if (String::Compare (replaceLine->Trim(), "@INTERCEPT_FUNCTIONS", true) == 0)
					{
						for (int i = 0; i < interceptedFunctionList->Count; i++)
						{
							InterceptedFunction * function = functionDatabase->GetFunctionByName (interceptedFunctionList->Item[i]->ToString());
							String * interceptLine = ParseAndReplaceString ("\ttestApp->InterceptFunction (\"$FUNCTIONNAME$\" , \"$FUNCTIONNAME$Replacement\",  replacementLibrary);", function);
							if (interceptLine)
								exeCodeFile->WriteLine (interceptLine);
						}

					}
					else if (String::Compare (replaceLine->Trim(), "@ATTACH_OR_LAUNCH", true) == 0)
					{
						String * appLine = "";
						if (this->cipLaunchPage->IsLaunchAUTSelected)
							appLine = "\ttestApp->LaunchApplication (\"$TEST_APPLICATION_PATH$\", \"$CMD_LINE$\", false);";
						else
						{
							exeCodeFile->WriteLine ("\t//TODO: Enter the processID of the test application in place of 0.");
							appLine = "\ttestApp->AttachApplication (0, false);";
						}

						String * replaceLine = ParseAndReplaceString (appLine, NULL);
						if (replaceLine)
							exeCodeFile->WriteLine (replaceLine);
					}

					else
						exeCodeFile->WriteLine (replaceLine);
				}
			}
			//Close the template file
			exeCodeTemplate->Close();
			//Close the exe code file
			exeCodeFile->Flush();
			exeCodeFile->Close();

			//Open the output header file for writing
			IO::StreamWriter * exeHFile = System::IO::File::CreateText (exeHeaderFileName);
			exeHFile->WriteLine (S"//Contains enumerations for fault types\n\n");
			FunctionDatabase::FaultFunctionDB * faultDB = FunctionDatabase::FaultFunctionDB::GetInstance();

			//WRITE THE FAULTS
			Fault * fault;
			String * faultName = "";

			//Write the disk faults
			exeHFile->WriteLine (S"class DiskFaults\n{\npublic:");
			for (int i=0; i < faultDB->DiskFaults->Count; i++)
			{
				fault = dynamic_cast <Fault *> (faultDB->DiskFaults->Item[i]);
				faultName = GetUniqueName(GetValidCodeName (fault->Name));
				exeHFile->WriteLine (String::Concat (S"\tstatic const int ", faultName, S" = ", fault->FaultID.ToString(), S";"));
			}
			exeHFile->WriteLine (S"};\n");

			//Write the memory faults
			exeHFile->WriteLine (S"class MemoryFaults\n{\npublic:");
			for (int i=0; i < faultDB->MemoryFaults->Count; i++)
			{
				fault = dynamic_cast <Fault *> (faultDB->MemoryFaults->Item[i]);
				faultName = GetUniqueName(GetValidCodeName (fault->Name));
				exeHFile->WriteLine (String::Concat (S"\tstatic const int ", faultName, S" = ", fault->FaultID.ToString(), S";"));
			}
			exeHFile->WriteLine (S"};\n");

			//Write the network faults
			exeHFile->WriteLine (S"class NetworkFaults\n{\npublic:");
			for (int i=0; i < faultDB->NetworkFaults->Count; i++)
			{
				fault = dynamic_cast <Fault *> (faultDB->NetworkFaults->Item[i]);
				faultName = GetUniqueName(GetValidCodeName (fault->Name));
				exeHFile->WriteLine (String::Concat (S"\tstatic const int ", faultName, S" = ", fault->FaultID.ToString(), S";"));
			}
			exeHFile->WriteLine (S"};\n");

			//Write the registry faults
			exeHFile->WriteLine (S"class RegistryFaults\n{\npublic:");
			for (int i=0; i < faultDB->RegistryFaults->Count; i++)
			{
				fault = dynamic_cast <Fault *> (faultDB->RegistryFaults->Item[i]);
				faultName = GetUniqueName(GetValidCodeName (fault->Name));
				exeHFile->WriteLine (String::Concat (S"\tstatic const int ", faultName, S" = ", fault->FaultID.ToString(), S";"));
			}
			exeHFile->WriteLine (S"};\n");

			//Write the process faults
			exeHFile->WriteLine (S"class ProcessFaults\n{\npublic:");
			for (int i=0; i < faultDB->ProcessFaults->Count; i++)
			{
				fault = dynamic_cast <Fault *> (faultDB->ProcessFaults->Item[i]);
				faultName = GetUniqueName(GetValidCodeName (fault->Name));
				exeHFile->WriteLine (String::Concat (S"\tstatic const int ", faultName, S" = ", fault->FaultID.ToString(), S";"));
			}
			exeHFile->WriteLine (S"};\n");

			//Write the custom faults
			exeHFile->WriteLine (S"class CustomFaults\n{\npublic:");
			for (int i=0; i < faultDB->CustomFaults->Count; i++)
			{
				fault = dynamic_cast <Fault *> (faultDB->CustomFaults->Item[i]);
				faultName = GetUniqueName(GetValidCodeName (fault->Name));
				exeHFile->WriteLine (String::Concat (S"\tstatic const int ", faultName, S" = ", fault->FaultID.ToString(), S";"));
			}
			exeHFile->WriteLine (S"};\n");

			//Close the exe header file
			exeHFile->Flush();
			exeHFile->Close();
		}
		catch (Exception * e)
		{
			MessageBox::Show (e->Message, "Holodeck");
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		native_CreateSolution
	// Description: Creates project files for the EXE and DLL projects
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::native_CreateSolution(String * ProjectPath, String * projectName)
	{	
		//Create EXE Project File
		String * sourceFile = this->CustomTestProjectGeneratorPath;
		sourceFile = String::Concat (sourceFile, "\\exeProject.vcproj");
		String * destinationFile = ProjectPath;
		destinationFile = String::Concat (destinationFile, "\\");
		destinationFile = String::Concat (destinationFile, projectName);
		destinationFile = String::Concat (destinationFile, ".vcproj");
		
		//Open the source file for reading
		IO::StreamReader * exeProjectTemplate = System::IO::File::OpenText (sourceFile);
		exeProjectTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);
    
		//Open the exe project file for writing
		IO::StreamWriter * exeProject = System::IO::File::CreateText (destinationFile);

		while (exeProjectTemplate->Peek() > -1)
			exeProject->WriteLine (ParseAndReplaceString (exeProjectTemplate->ReadLine(), NULL));

		exeProject->Close();
		exeProjectTemplate->Close();


		//Create DLL Project File
		sourceFile = CustomTestProjectGeneratorPath;
		sourceFile = String::Concat (sourceFile, "\\dllProject.vcproj");
		destinationFile = ProjectPath;
		destinationFile = String::Concat (destinationFile, "\\");
		destinationFile = String::Concat (destinationFile, projectName);
		destinationFile = String::Concat (destinationFile, "Dll.vcproj");
		
		//Open the source file for reading
		IO::StreamReader * dllProjectTemplate = System::IO::File::OpenText (sourceFile);
		dllProjectTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);
    
		//Open the dll project file for writing
		IO::StreamWriter * dllProject = System::IO::File::CreateText (destinationFile);

		while (dllProjectTemplate->Peek() > -1)
			dllProject->WriteLine (ParseAndReplaceString (dllProjectTemplate->ReadLine(), NULL));
		dllProject->Close();
		dllProjectTemplate->Close();

		return true;
	}

	//*************************************************************************
	// Method:		native_CopyLibraries
	// Description: Copies the required library files to the user project location
	//
	// Parameters:
	//	None
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::native_CopyLibraries(String * ProjectPath)
	{
		try
		{
			IO::Directory::CreateDirectory (String::Concat (ProjectPath, "Output"));
			IO::File::Copy (String::Concat (this->HolodeckInstallPath, "\\HolodeckLib.dll"), String::Concat (ProjectPath, "Output\\", "HolodeckLib.dll"), true);
			IO::File::Copy (String::Concat (this->HolodeckInstallPath, "\\FunctionDatabase.dll"), String::Concat (ProjectPath, "Output\\", "FunctionDatabase.dll"), true);
			IO::File::Copy (String::Concat (this->HolodeckInstallPath, "\\HoloScript.dll"), String::Concat (ProjectPath, "Output\\", "HoloScript.dll"), true);
		}
		catch (Exception * e)
		{
			MessageBox::Show (e->Message, "Holodeck");
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		csharp_CreateInterceptionLibrary
	// Description: Generates code for a csharp interception library
	//
	// Parameters:
	//	projectPath - the path of the project
	//	projectName - the name of the project
	//	interceptedFunctionList - an array of the functions to intercept
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::csharp_CreateInterceptionLibrary(String * projectPath, String * projectName, ArrayList * interceptedFunctionList)
	{
		//Load the DotNet Database
		functionDatabase = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		if (!functionDatabase->IsLoaded)
			functionDatabase->LoadDatabase(functionDBFolder, DOT_NET_FUNC_DB_FILE_NAME);

		ArrayList * libsList = new ArrayList();
		ArrayList * funcList = new ArrayList();
		for (int i=0; i < interceptedFunctionList->Count; i++)
		{
			String * funcName = dynamic_cast <String *> (interceptedFunctionList->Item[i]);
			InterceptedFunction * ifunc = functionDatabase->GetFunctionByName(funcName);
			
			if (!ifunc)
				continue;

			funcList->Add (ifunc);

			if (!libsList->Contains (ifunc->OriginalDll))
				libsList->Add (ifunc->OriginalDll);
		}

		AddNewIntercepts::DotNetCodeGenerator * dotnetCodeGen = new AddNewIntercepts::DotNetCodeGenerator(false);
		String * projectPathAndName = String::Concat (projectPath, Path::DirectorySeparatorChar.ToString(), projectName, "Dll.csproj");
		dotnetCodeGen->CreateProject (projectPathAndName, libsList, funcList);
		return true;
	}

	//*************************************************************************
	// Method:		csharp_CreateControlExe
	// Description: Generates code for a csharp control exe
	//
	// Parameters:
	//	projectPath - the path of the project
	//	projectName - the name of the project
	//	interceptedFunctionList - an array of the functions to intercept
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::csharp_CreateControlExe(String * ProjectPath, String * projectName, ArrayList * interceptedFunctionList)
	{
		//Create the control exe code and header files
		String * exeCodeFileName = String::Concat (ProjectPath, projectName, ".cs");
		try
		{
			//Open the template file for reading
			IO::StreamReader * exeCodeTemplate = System::IO::File::OpenText (String::Concat (this->CustomTestProjectGeneratorPath, "\\replacementExeC#.dat"));
			exeCodeTemplate->BaseStream->Seek(0, IO::SeekOrigin::Begin);
      
			//Open the output cpp file for writing
			IO::StreamWriter * exeCodeFile = System::IO::File::CreateText (exeCodeFileName);
			
			//Parse and replace strings and write to output file
			while (exeCodeTemplate->Peek() > -1)
			{
				String * templateLine = exeCodeTemplate->ReadLine ();
				String * replaceLine = ParseAndReplaceString (templateLine, NULL);
				if (replaceLine)
				{
					if (String::Compare (replaceLine->Trim(), "@CSHARP_ATTACH_OR_LAUNCH", true) == 0)
					{
						String * appLine = "";
						if (this->cipLaunchPage->IsLaunchAUTSelected)
						{
							appLine = "\t\t\ttestApp.LaunchApplication (@\"$TEST_APPLICATION_PATH$\", \"$CMD_LINE$\", false);";
						}
						else
						{
							exeCodeFile->WriteLine ("\t\t\t//TODO: Enter the processID of the test application in the line below.");
							appLine = "\t\t\ttestApp.AttachApplication (Enter_PID_Here, false);";
						}

						String * replaceLine = ParseAndReplaceString (appLine, NULL);
						if (replaceLine)
							exeCodeFile->WriteLine (replaceLine);
					}
					else
						exeCodeFile->WriteLine (replaceLine);
				}
			}
			//Close the template file
			exeCodeTemplate->Close();
			//Close the exe code file
			exeCodeFile->Flush();
			exeCodeFile->Close();
		}
		catch (Exception *)
		{
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		csharp_CreateSolution
	// Description: Generates the .csproj
	//
	// Parameters:
	//	projectPath - the path of the project
	//	projectName - the name of the project
	//
	// Return Value: true if success, otherwise false
	//*************************************************************************
	bool CustomTestProjectWizardControl::csharp_CreateSolution(String * ProjectPath, String * projectName)
	{
		try
		{
			String * projFileName = String::Concat (ProjectPath, projectName, ".csproj");

			//Open the .csproj template file for reading
			StreamReader * projTemplate = File::OpenText (String::Concat (CustomTestProjectGeneratorPath, "\\DotNetProjectsCtp.dat"));
			//Open the .csproj output file for writing
			StreamWriter * projWriter = File::CreateText (projFileName);

			String * tplString = "";
			String * parsedString = "";
			while ((tplString = projTemplate->ReadLine()) != NULL) 
			{
				parsedString = tplString;
				parsedString = parsedString->Replace ("$PROJECT_NAME$", projectName);
				parsedString = parsedString->Replace ("$INSTALL_PATH$", this->HolodeckInstallPath);
				projWriter->WriteLine (parsedString);
			}
			
			//Flush and close the output stream
			projWriter->Flush();
			projWriter->Close();
			//Close the input template stream
			projTemplate->Close();
		}
		catch (Exception *)
		{
			return false;
		}

		return true;
	}

}