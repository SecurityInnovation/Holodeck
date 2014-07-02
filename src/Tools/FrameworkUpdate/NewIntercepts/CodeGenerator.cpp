//***********************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CodeGenerator.cpp
//
// DESCRIPTION: Contains implementation for the class InterceptCodeGenerator
//
//===================================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 10 Nov 2003		 Ady K		 File created.
//***********************************************************************************
#include "CodeGenerator.h"

namespace AddNewIntercepts
{
	//*************************************************************************
	// Method:		InterceptCodeGenerator
	// Description: Constructor for the InterceptCodeGenerator class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************
	InterceptCodeGenerator::InterceptCodeGenerator()
	{
		//Define the attributes used for specifying storage-class information
		beforeReturnTypeKeywords = new ArrayList();
		beforeReturnTypeKeywords->Add (new String ("extern"));
		beforeReturnTypeKeywords->Add (new String ("extern \"C\""));
		beforeReturnTypeKeywords->Add (new String ("inline"));
		beforeReturnTypeKeywords->Add (new String ("__inline"));
		beforeReturnTypeKeywords->Add (new String ("static"));
		beforeReturnTypeKeywords->Add (new String ("DECLSPEC_IMPORT"));
		beforeReturnTypeKeywords->Add (new String ("WINADVAPI"));
		beforeReturnTypeKeywords->Add (new String ("WINBASEAPI"));
		beforeReturnTypeKeywords->Add (new String ("ZAWPROXYAPI"));
		beforeReturnTypeKeywords->Add (new String ("__cplusplus"));
		beforeReturnTypeKeywords->Add (new String ("WINUSERAPI"));

		//Get the template path from the registry
		Microsoft::Win32::RegistryKey *key = Microsoft::Win32::Registry::LocalMachine->OpenSubKey(ROOT_PATH);
		this->holodeckPath = dynamic_cast<String *> (key->GetValue(INSTALL_PATH_VALUE_NAME, new String(".")));
		this->replacementFolderPath = dynamic_cast<String *> (key->GetValue(PLUGIN_DLL_PATH_VALUE_NAME, new String(".")));
		this->replacementFolderPath = String::Concat (holodeckPath, Path::DirectorySeparatorChar.ToString(), this->replacementFolderPath);
		this->templatePath = dynamic_cast<String *> (key->GetValue(NEWINTERCEPTS_TPL_PATH_NAME, new String(".")));
		this->templatePath = String::Concat(holodeckPath, Path::DirectorySeparatorChar.ToString(), this->templatePath);
		this->holodeckDatabaseFolderPath = dynamic_cast<String *> (key->GetValue(FUNCTION_DB_PATH_VALUE_NAME, new String(".")));
		this->holodeckDatabaseFolderPath = String::Concat (holodeckPath, Path::DirectorySeparatorChar.ToString(), this->holodeckDatabaseFolderPath);
		
		if (! templatePath->EndsWith (Path::DirectorySeparatorChar.ToString()))
			templatePath = String::Concat (templatePath, Path::DirectorySeparatorChar.ToString());
	}

	//*************************************************************************
	// Method:		~InterceptCodeGenerator
	// Description: Desstructor for the InterceptCodeGenerator class
	//
	// Parameters:
	//		None
	//
	// Return Value: None
	//*************************************************************************	
	InterceptCodeGenerator::~InterceptCodeGenerator()
	{
	}

	//*************************************************************************
	// Method:		CreateProject
	// Description: Generates a cpp file, header file, def file and a vcproj file
	//				for the users replacement code
	//
	// Parameters:
	//		fileName - name of the project file to create (Example: c:\temp\myLibrary.vcproj)
	//		interceptedFunctions - an array of functions to intercepts
	//
	// Return Value: None
	//*************************************************************************	
	int InterceptCodeGenerator::CreateProject (String * fileName, ArrayList * interceptedFunctions)
	{
		if (interceptedFunctions->Count < 1)
			return 0;

		//Get first function from list to get some information
		InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (interceptedFunctions->Item[0]);
		
		//Define a ProjectInformation object
		String * originalDll = Path::GetFileName (ifunc->OriginalDll);
		String * projectName = Path::GetFileNameWithoutExtension (fileName);
		String * projectPath = String::Concat (Path::GetDirectoryName (fileName), Path::DirectorySeparatorChar.ToString());
		ProjectInformation * projectInfo = new ProjectInformation (projectName, projectPath, originalDll);

		//Create the directory
		try
		{
			Directory::CreateDirectory (projectPath);
		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("Error:\n", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		//Check to see if template files exist
		if ((templatePath == NULL) || (! Directory::Exists (templatePath)))
		{
			MessageBox::Show ("Error: The registry entry required for locating template files is either not present or is incorrect.\n\nThis procedure is being aborted.", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		if ((! File::Exists (String::Concat (this->templatePath, "callerfunc.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "cppfile_func.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "cppfile.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "hfile.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "deffile.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "hfile_func.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "dllProject.vcproj.dat"))) ||
			(! File::Exists (String::Concat (this->templatePath, "replfunc.dat"))))
		{
			MessageBox::Show ("Error: Template files required for code generation could not be located.\nPlease reinstall Holodeck", "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		//Create the main cpp file
		if (!this->createCppFile (String::Concat (projectPath, projectName, ".cpp"), interceptedFunctions, projectInfo))
			return 0;
		
		//create the header file
		if (!this->createHeaderFile (String::Concat (projectPath, projectName, ".h"), interceptedFunctions, projectInfo))
			return 0;
		
		//create the individual cpp files
		if (!this->createFuncCppFiles (interceptedFunctions, projectInfo))
			return 0;

		//create the def file
		if (!this->createDefFile (String::Concat (projectPath, projectName, ".def"), interceptedFunctions, projectInfo))
			return 0;
		
		//create project file
		if (!this->createProjectFile (String::Concat (projectPath, projectName, ".vcproj"), interceptedFunctions, projectInfo))
			return 0;

		return 1;
	}

	//*************************************************************************
	// Method:		createFuncCppFiles
	// Description: Generates the cpp code files for each replacement function
	//
	// Parameters:
	//		interceptedFunctions - an array of functions to intercepts
	//		projectInfo - information about the project
	//
	// Return Value: (int) 1 if success, 0 if failed
	//*************************************************************************	
	int InterceptCodeGenerator::createFuncCppFiles (ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		try
		{
			//Open the cpp template file
			StreamReader * cppfReader = File::OpenText (String::Concat (this->templatePath, "cppfile_func.dat"));
			
			//Read the information into an array
			ArrayList * templateArray = new ArrayList();
			String * tplString = "";
			while ((tplString = cppfReader->ReadLine()) != NULL) 
				templateArray->Add (tplString);
			//Close the file
			cppfReader->Close();

			//Create a separate cpp file for each function
			for (int i=0; i < interceptedFunctions->Count; i++)
			{
				InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (interceptedFunctions->Item[i]);

				//Create the output cpp file
				StreamWriter * cppWriter =  File::CreateText (String::Concat (projectInfo->projectPath, ifunc->Name , S"Replacement.cpp"));

				for (int j=0; j < templateArray->Count; j++)
				{
					String * tString = dynamic_cast <String *> (templateArray->Item[j]);

					String * parsedString = ParseAndReplaceString (tString, ifunc, projectInfo);
					//Execute @command if any, otherwise write parsed string to output cpp file
					if (parsedString)
					{
						if (String::Compare (parsedString->Trim(), "@INSERT_CALLER_FUNCTION") == 0)
							this->insertCallerFunction (cppWriter, ifunc, projectInfo);
						else if (String::Compare (parsedString->Trim(), "@INSERT_REPLACEMENT_FUNCTION") == 0)
							this->insertReplacementFunction (cppWriter, ifunc, projectInfo);
						else
							cppWriter->WriteLine (parsedString);
					}
				}

				cppWriter->Flush();
				cppWriter->Close();
			}
			
		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("An error occurred while generating a code file.\n\nThis procedure is being aborted.\n\nError:", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		return 1;
	}

	//*************************************************************************
	// Method:		createProjectFile
	// Description: Generates the vcproj file and places it in the correct location
	//
	// Parameters:
	//		projFileName - name of the project file to create (Example: c:\temp\myLibrary.vcproj)
	//		interceptedFunctions - an array of functions to intercepts
	//
	// Return Value: (int) 1 if success, 0 if failed
	//*************************************************************************	
	int InterceptCodeGenerator::createProjectFile (String * projFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		try
		{
			//Open the cpp template file
			StreamReader * projReader = File::OpenText (String::Concat (this->templatePath, "dllProject.vcproj.dat"));
			//Create the output project file
			StreamWriter * projWriter =  File::CreateText (projFileName);

			String * tplString = "";
			while ((tplString = projReader->ReadLine()) != NULL) 
			{
				String * parsedString = ParseAndReplaceString (tplString, NULL, projectInfo);
				if (parsedString)
				{
					if (String::Compare (parsedString->Trim(), "@INSERT_CPP_FILES_REFS") == 0)
						this->insertCppFileRefsInVCProj(projWriter, interceptedFunctions, projectInfo);
					else
						projWriter->WriteLine (parsedString);
				}
			}
			
			//flush and close streams
			projWriter->Flush();
			projWriter->Close();
			projReader->Close();

		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("An error occurred while generating the project file.\n\nThis procedure is being aborted.\n\nError:", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		return 1;
	}

	//*************************************************************************
	// Method:		createCppFile
	// Description: Generates the cpp code file
	//
	// Parameters:
	//		cppFileName - name of the cpp file to create (Example: c:\temp\myLibrary.cpp)
	//		interceptedFunctions - an array of functions to intercepts
	//
	// Return Value: (int) 1 if success, 0 if failed
	//*************************************************************************	
	int InterceptCodeGenerator::createCppFile (String * cppFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		try
		{
			//Open the cpp template file
			StreamReader * cppReader = File::OpenText (String::Concat (this->templatePath, "cppfile.dat"));
			//Create the output cpp file
			StreamWriter * cppWriter =  File::CreateText (cppFileName);
			
			String * tplString = "";
			while ((tplString = cppReader->ReadLine()) != NULL) 
			{
				String * parsedString = ParseAndReplaceString (tplString, NULL, projectInfo);
				//Execute @command if any, otherwise write parsed string to output cpp file
				if (parsedString)
					cppWriter->WriteLine (parsedString);
			}
			
			//flush and close streams
			cppWriter->Flush();
			cppWriter->Close();
			cppReader->Close();
		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("An error occurred while generating a code file.\n\nThis procedure is being aborted.\n\nError:", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		return 1;
	}
	
	//*************************************************************************
	// Method:		createHeaderFile
	// Description: Generates the header code file
	//
	// Parameters:
	//		hFileName - name of the header file to create (Example: c:\temp\myLibrary.h)
	//		interceptedFunctions - an array of functions to intercepts
	//
	// Return Value: (int) 1 if success, 0 if failed
	//*************************************************************************	
	int InterceptCodeGenerator::createHeaderFile (String * hFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		try
		{
			//Open the cpp template file
			StreamReader * hReader = File::OpenText (String::Concat (this->templatePath, "hfile.dat"));
			//Create the output header file
			StreamWriter * hWriter =  File::CreateText (hFileName);
			
			String * tplString = "";
			while ((tplString = hReader->ReadLine()) != NULL) 
			{
				String * parsedString = ParseAndReplaceString (tplString, NULL, projectInfo);
				//Execute @command if any, otherwise write parsed string to output h file`
				if (parsedString)
				{
					if (String::Compare (parsedString->Trim(), "@INSERT_FUNCTION_DEFINITIONS") == 0)
					{
						this->insertHeaderDefinitions (hWriter, interceptedFunctions, projectInfo);
					}
					else
						hWriter->WriteLine (parsedString);
				}
			}
			
			//flush and close streams
			hWriter->Flush();
			hWriter->Close();
			hReader->Close();
		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("An error occurred while generating a code file.\n\nThis procedure is being aborted.\n\nError:", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		return 1;
	}
	
	//*************************************************************************
	// Method:		createDefFile
	// Description: Generates the def code file
	//
	// Parameters:
	//		defFileName - name of the def file to create (Example: c:\temp\myLibrary.def)
	//		interceptedFunctions - an array of functions to intercepts
	//
	// Return Value: (int) 1 if success, 0 if failed
	//*************************************************************************	
	int InterceptCodeGenerator::createDefFile (String * defFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		try
		{
			//Open the def template file
			StreamReader * defReader = File::OpenText (String::Concat (this->templatePath, "deffile.dat"));
			//Create the output def file
			StreamWriter * defWriter =  File::CreateText (defFileName);
			
			String * tplString = "";
			while ((tplString = defReader->ReadLine()) != NULL) 
			{
				String * parsedString = ParseAndReplaceString (tplString, NULL, projectInfo);
				//Execute @command if any, otherwise write parsed string to output h file`
				if (parsedString)
				{
					if (String::Compare (parsedString->Trim(), "@INSERT_EXPORTED_LIST") == 0)
					{
						this->insertExportedList (defWriter, interceptedFunctions, projectInfo);
					}
					else
						defWriter->WriteLine (parsedString);
				}
			}
			
			//flush and close streams
			defWriter->Flush();
			defWriter->Close();
			defReader->Close();
		}
		catch (Exception * e)
		{
			MessageBox::Show (String::Concat ("An error occurred while generating a code file.\n\nThis procedure is being aborted.\n\nError:", e->Message), "Holodeck", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return 0;
		}

		return 1;
	}

	//*************************************************************************
	// Method:		insertExportedList
	// Description: writes the exports list information in the provided stream
	//
	// Parameters:
	//		defWriter - the stream to write to
	//		interceptedFunctions - an array of functions to intercepts
	//		projectInfo - information about the project
	//
	// Return Value: None
	//*************************************************************************	
	void InterceptCodeGenerator::insertExportedList (StreamWriter * defWriter, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		//Write exported list to file
		for (int i=0; i < interceptedFunctions->Count; i++)
		{
			InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (interceptedFunctions->Item[i]);
			defWriter->WriteLine (String::Concat (ifunc->Name, "Replacement"));
		}
	}

	//*************************************************************************
	// Method:		insertHeaderDefinitions
	// Description: writes the header definitions in the provided stream
	//
	// Parameters:
	//		hWriter - the stream to write to
	//		interceptedFunctions - an array of functions to intercepts
	//		projectInfo - information about the project
	//
	// Return Value: None
	//*************************************************************************	
	void InterceptCodeGenerator::insertHeaderDefinitions (StreamWriter * hWriter, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		//open the header function definition template file and read into an array
		ArrayList * hfuncTemplate = new ArrayList();
		StreamReader * htReader = File::OpenText (String::Concat (this->templatePath, "hfile_func.dat"));
		String * tplString = "";
		while ((tplString = htReader->ReadLine()) != NULL) 
			hfuncTemplate->Add (tplString);

		//Write header definitions to the file
		for (int i=0; i < interceptedFunctions->Count; i++)
		{
			InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (interceptedFunctions->Item[i]);
			for (int t=0; t < hfuncTemplate->Count; t++)
			{
				String * templateString = dynamic_cast <String *> (hfuncTemplate->Item[t]);
				String * parsedString = this->ParseAndReplaceString (templateString, ifunc, projectInfo);
				if (parsedString)
					hWriter->WriteLine (parsedString);
			}
		}
	}

	//*************************************************************************
	// Method:		insertCallerFunction
	// Description: writes the replacement caller functions in the provided stream
	//
	// Parameters:
	//		cppWriter - the stream to write to
	//		iFunction - the function for which to generate the caller function
	//		projectInfo - information about the project
	//
	// Return Value: None
	//*************************************************************************	
	void InterceptCodeGenerator::insertCallerFunction (StreamWriter * cppWriter, InterceptedFunction * iFunction, ProjectInformation * projectInfo)
	{
		//Read the callerFunc template into an array (if not already done so)
		if (!callerTemplate)
		{
			callerTemplate = new ArrayList();
			//Open the callerFunction template file
			StreamReader * ctReader = File::OpenText (String::Concat (this->templatePath, "callerfunc.dat"));
			String * tplString = "";
			while ((tplString = ctReader->ReadLine()) != NULL) 
				callerTemplate->Add (tplString);
			
			//Close the template file
			ctReader->Close();
		}

		//Write callerFunc code to the file
		for (int t=0; t < callerTemplate->Count; t++)
		{
			String * templateString = dynamic_cast <String *> (callerTemplate->Item[t]);
			String * parsedString = this->ParseAndReplaceString (templateString, iFunction, projectInfo);
			if (parsedString)
				cppWriter->WriteLine (parsedString);
		}
	}

	
	//*************************************************************************
	// Method:		insertReplacementFunction
	// Description: writes replacement code for the specified function
	//
	// Parameters:
	//		cppWriter - the stream to write to
	//		iFunction - an array of functions to intercepts
	//		projectInfo - information about the project
	//
	// Return Value: None
	//*************************************************************************	
	void InterceptCodeGenerator::insertReplacementFunction (StreamWriter * cppWriter, InterceptedFunction * iFunction, ProjectInformation * projectInfo)
	{
		if (!replTemplate)
		{
			//Read the callerFunc template into an array
			replTemplate = new ArrayList();
			//Open the callerFunction template file
			StreamReader * ctReader = File::OpenText (String::Concat (this->templatePath, "replfunc.dat"));
			String * tplString = "";
			while ((tplString = ctReader->ReadLine()) != NULL) 
				replTemplate->Add (tplString);

			//Close the file
			ctReader->Close();
		}

		//Write callerFunc code to the file
		for (int t=0; t < replTemplate->Count; t++)
		{
			String * templateString = dynamic_cast <String *> (replTemplate->Item[t]);
			String * parsedString = this->ParseAndReplaceString (templateString, iFunction, projectInfo);
			if (parsedString)
				cppWriter->WriteLine (parsedString);
		}
	}

	//*************************************************************************
	// Method:		insertCppFileRefsInVCProj
	// Description: Insert references to the individual CPP files in the vcproj
	//				file
	//
	// Parameters:
	//	fWriter	- the stream to write to
	//	interceptedFunctions - an array of intercepted functions
	//	projectInfo - project information
	//
	// Return Value: the parsed string
	//*************************************************************************
	void InterceptCodeGenerator::insertCppFileRefsInVCProj(StreamWriter * fWriter, ArrayList * interceptedFunctions, ProjectInformation * projectInfo)
	{
		String * baseString = "\t\t\t<File RelativePath=\".\\$FUNCTIONNAME$Replacement.cpp\"></File>";

		for (int i=0; i < interceptedFunctions->Count; i++)
		{
			InterceptedFunction * ifunc = dynamic_cast <InterceptedFunction *> (interceptedFunctions->Item[i]);
			String * rString = ParseAndReplaceString (baseString, ifunc, projectInfo);
			if (rString)
				fWriter->WriteLine (rString);
		}
	}

	//*************************************************************************
	// Method:		ParseAndReplaceString
	// Description: Performs replacement of tokens inside strings and replaces
	//				them with project specific strings
	//
	// Parameters:
	//	templateString	- the string read from a template source
	//	function - function info to use while parsing this string, can be NULL
	//	projInfo - project information
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * InterceptCodeGenerator::ParseAndReplaceString(String * templateString, InterceptedFunction * function, ProjectInformation * projInfo)
	{
		//Check for IFs
		if (templateString->Trim()->StartsWith("#IF_RETURNTYPE_VOID#"))
		{
			if (String::Compare (function->ReturnType, "VOID", true) != 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_RETURNTYPE_VOID#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_RETURN_NONVOID#"))
		{
			if (String::Compare (function->ReturnType, "VOID", true) == 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_RETURN_NONVOID#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_RETURNTYPE_DWORD#"))
		{
			if (String::Compare (function->ReturnType, "DWORD", true) != 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_RETURNTYPE_DWORD#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_RETURNTYPE_NONDWORD#"))
		{
			if (String::Compare (function->ReturnType, "DWORD", true) == 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_RETURNTYPE_NONDWORD#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_HAS_SISTRINGS#"))
		{
			if (funcNumSiStringParams(function->Parameters) <= 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_HAS_SISTRINGS#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_NUMPARAMS_NONZERO#"))
		{
			if (function->ParameterCount == 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_NUMPARAMS_NONZERO#", "");
		}

		if (templateString->Trim()->StartsWith("#IF_NUMPARAMS_ZERO#"))
		{
			if (function->ParameterCount != 0)
				return NULL;
			else
				templateString = templateString->Replace ("#IF_NUMPARAMS_ZERO#", "");
		}

		templateString = templateString->Replace ("$ORIGINAL_DLL_NAME$", projInfo->originalDll);
		templateString = templateString->Replace ("$PROJECT_NAME$", projInfo->projectName);
		templateString = templateString->Replace ("$HOLODECK_INSTALL_PATH$", this->holodeckPath);
		templateString = templateString->Replace ("$REPLACEMENT_FOLDER_PATH$", this->replacementFolderPath);
		templateString = templateString->Replace ("$HDDB_FOLDER_PATH$", this->holodeckDatabaseFolderPath);

		if (function)
		{
			templateString = templateString->Replace ("$FUNCTIONNAME$", function->Name);
			templateString = templateString->Replace ("$NUMPARAMS$", function->ParameterCount.ToString());
			templateString = templateString->Replace ("$RETURNTYPE$", function->ReturnType);
			templateString = templateString->Replace ("$CASTEDPARAMSLIST$", GetCastedParamsList(function->Parameters));
			templateString = templateString->Replace ("$PARAM_TYPES_AND_NAMES$", GetParamTypesAndNames(function->Parameters));
			templateString = templateString->Replace ("$MODIFIERS$", function->ModifiersString);
			templateString = templateString->Replace ("$FUNCTIONCATEGORY$", function->Category);
			templateString = templateString->Replace ("$PARAM_TYPES_LIST$", GetParamTypesList(function->Parameters));
			templateString = templateString->Replace ("$SISTRING_LIST$", GetSiStringLine(function->Parameters));
			templateString = templateString->Replace ("$PARAM_REFS_LIST$", GetParamRefsList(function->Parameters));
			templateString = templateString->Replace ("$MODIFIERS_A$", getModifiers(function->Modifiers, 0));
			templateString = templateString->Replace ("$MODIFIERS_B$", getModifiers(function->Modifiers, 1));
			templateString = templateString->Replace ("$BASIC_PARAM_TYPE_LIST$", GetBasicParamTypeList(function->Parameters));
		}
		
		return templateString;
	}

	//*************************************************************************
	// Method:		getModifiers
	// Description: Returns a string of modifiers based on the information provided
	//
	// Parameters:
	//	modifiers	- an array of modifiers
	//	modTypeRequested - 0, will return modifiers which occur before the return value
	//					   1, will return modifiers which occur after the return value
	//
	// Return Value: the modifiers string
	//*************************************************************************
	String * InterceptCodeGenerator::getModifiers (Array * modifiers, int modTypeRequested)
	{
		String * mods = "";
		for (int i=0; i < modifiers->Count; i++)
		{
			if (modTypeRequested == 0)
			{
				if (beforeReturnTypeKeywords->Contains (modifiers->Item[i]))
					mods = String::Concat (mods, dynamic_cast <String *> (modifiers->Item[i]), " ");
			}
			else
			{
				if (! beforeReturnTypeKeywords->Contains (modifiers->Item[i]))
					mods = String::Concat (mods, dynamic_cast <String *> (modifiers->Item[i]), " ");
			}
		}

		return mods->Trim();
	}

	////*************************************************************************
	//// Method:		GetCastedParamsList
	//// Description: generates list of casted params for caller functions
	////			eg.: (*((UINT *)params[0]), *((SIZE_T *)params[1]))
	////
	//// Parameters:
	////	params	- the array to read the info. from
	////
	//// Return Value: the parsed string
	////*************************************************************************
	String * InterceptCodeGenerator::GetCastedParamsList(Array *params)
	{
		InterceptedFunctionParameter *prm; 
		String * paramString = "";
		String * temp = "";
		int nParams = params->GetLength(0);
		if (nParams > 0)
		{
			for (int i = 0; i < nParams; i++)
			{			
				prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				if((prm->CompareAsType == StringType)||(prm->CompareAsType == WideStringType))
					temp = String::Concat ("*((SiString *)params[", i.ToString(), new String("])"));
				else
					temp = String::Concat (new String("*(("), prm->Type->ToString(), new String(" *)params[") , i.ToString(), new String ("])"));

				paramString = String::Concat(paramString, temp);

				if (i < nParams - 1)
					paramString = String::Concat(paramString, ", ");
			}
		}		
		return paramString;
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
	String * InterceptCodeGenerator::GetParamTypesAndNames(Array *params)
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
	// Method:		GetParamTypesList
	// Description: Returns a string of the form:
	//				param1Type, param2Type, param3Type
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * InterceptCodeGenerator::GetParamTypesList(Array *params)
	{
		InterceptedFunctionParameter *prm; 
		String *paramString = "";
		int nParams = params->GetLength(0);
		if (nParams > 0)
		{
			for (int i = 0; i < nParams; i++)
			{			
				prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				paramString = String::Concat(paramString, prm->Type);
				if (i < nParams-1)
					paramString = String::Concat(paramString, ", ");
			}
		}		
		return paramString;
	}

	//*************************************************************************
	// Method:		funcNumSiStringParams
	// Description: returns # of params needed to be cast to SiString
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: # of params needed to be cast to SiString
	//*************************************************************************
	int InterceptCodeGenerator::funcNumSiStringParams(Array *params)
	{
		int numSiStrings = 0;
		for (int i = 0; i < params->GetLength(0); i++)
		{
			InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
			if ((prm->CompareAsType == StringType)||(prm->CompareAsType == WideStringType))
				numSiStrings++;
		}
		return numSiStrings;
	}

	//*************************************************************************
	// Method:		GetSiStringLine
	// Description: generates lineof code for SiString in replacement function
	//			eg.: SiString str[] = { (wchar_t *)lpExistingFileName, (wchar_t *)lpNewFileName };
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * InterceptCodeGenerator::GetSiStringLine(Array *params)
	{
		String *SiStringLine = "";
		int numSiStrings = funcNumSiStringParams(params);
		if (numSiStrings <= 0)
			return NULL;
		else if (numSiStrings == 1)
		{
			SiStringLine = "SiString str = ";
			for (int i = 0; i < params->GetLength(0); i++)
			{
				InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				if (prm->CompareAsType == StringType)
				{
					SiStringLine = String::Concat(SiStringLine, "(char *) ", prm->Name, ";");
					return SiStringLine;
				}
				else if(prm->CompareAsType == WideStringType)
				{
					SiStringLine = String::Concat(SiStringLine, "(wchar_t *) ", prm->Name, ";");
					return SiStringLine;
				}
			}
		}
		else
		{
			bool insertComma = false;
			SiStringLine = "SiString str[] = { ";
			for (int i = 0; i < params->GetLength(0); i++)
			{
				InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				if (prm->CompareAsType == StringType)
				{
					if (insertComma && i < params->GetLength(0) - 1)
						SiStringLine = String::Concat(SiStringLine, ", ");
					else
						insertComma = true;

					SiStringLine = String::Concat(SiStringLine, "(char *) ", prm->Name);
				}
				else if(prm->CompareAsType == WideStringType)
				{
					if (insertComma && i < params->GetLength(0) - 1)
						SiStringLine = String::Concat(SiStringLine, ", ");
					else
						insertComma = true;

					SiStringLine = String::Concat(SiStringLine, "(wchar_t *) ", prm->Name);
				}
			}
			SiStringLine = String::Concat(SiStringLine, " };");
			return SiStringLine;
		}

		return "";
	}

	//*************************************************************************
	// Method:		GetParamRefsList
	// Description: generates line of code for 'void *params[] ='  in replacement function
	//			eg.: &str[0], &str[1], &lpSecurityAttributes
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * InterceptCodeGenerator::GetParamRefsList(Array *params)
	{
		String *prmRefsList = "";
		if (params->GetLength(0) <= 0)
			return NULL;

		if (funcNumSiStringParams(params) <= 0)
		{
			for (int i = 0; i < params->GetLength(0); i++)
			{
				InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				prmRefsList = String::Concat(prmRefsList, "&", prm->Name);
				if (i < params->GetLength(0) - 1)
					prmRefsList = String::Concat(prmRefsList, ", ");
			}
			return prmRefsList;
		}
		else if (funcNumSiStringParams(params) == 1)
		{
			for (int i = 0; i < params->GetLength(0); i++)
			{
				InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				if (prm->CompareAsType == StringType || prm->CompareAsType == WideStringType)
					prmRefsList = String::Concat(prmRefsList, "&str");
				else
					prmRefsList = String::Concat(prmRefsList, "&", prm->Name);
				if (i < params->GetLength(0) - 1)
					prmRefsList = String::Concat(prmRefsList, ", ");
			}
			return prmRefsList;
		}
		else
		{
			for (int i = 0; i < params->GetLength(0); i++)
			{
				int strCount = 0;
				InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
				if (prm->CompareAsType == StringType || prm->CompareAsType == WideStringType)
				{
					prmRefsList = String::Concat(prmRefsList, "&str[", strCount.ToString(), "]");
					strCount++;
				}
				else
					prmRefsList = String::Concat(prmRefsList, "&", prm->Name);
				if (i < params->GetLength(0) - 1)
					prmRefsList = String::Concat(prmRefsList, ", ");
			}
			return prmRefsList;
		}
	}

	//*************************************************************************
	// Method:		GetBasicParamTypeList
	// Description: generates line of code for 'ParameterType paramTypes[numParams] ='  in replacement function
	//			eg.: StringType, PointerType, PointerType, PointerType, PointerType
	//
	// Parameters:
	//	params	- the array to read the info. from
	//
	// Return Value: the parsed string
	//*************************************************************************
	String * InterceptCodeGenerator::GetBasicParamTypeList(Array *params)
	{
		if (params->GetLength(0) <= 0)
			return NULL;

		String *prmType = "";
		for (int i = 0; i < params->GetLength(0); i++)
		{
			InterceptedFunctionParameter *prm = dynamic_cast <InterceptedFunctionParameter *> (params->GetValue(i));
			switch (prm->CompareAsType)
			{
				case NullType:
					prmType = String::Concat(prmType, "NullType");
					break;
				case StringType:
					prmType = String::Concat(prmType, "StringType");
					break;
				case WideStringType:
					prmType = String::Concat(prmType, "WideStringType");
					break;
				case BooleanType:
					prmType = String::Concat(prmType, "BooleanType");
					break;
				case IntegerType:
					prmType = String::Concat(prmType, "IntegerType");
					break;
				case UnsignedLongType:
					prmType = String::Concat(prmType, "UnsignedLongType");
					break;
				case RealType:
					prmType = String::Concat(prmType, "RealType");
					break;
				case PointerType:
					prmType = String::Concat(prmType, "PointerType");
					break;
				case PointerPointerType:
					prmType = String::Concat(prmType, "PointerPointerType");
					break;
				case Integer64Type:
					prmType = String::Concat(prmType, "Integer64Type");
					break;
				case IgnoreType:
					prmType = String::Concat(prmType, "IgnoreType");
					break;
				case OutStringType:
					prmType = String::Concat(prmType, "OutStringType");
					break;
				case OutWideStringType:
					prmType = String::Concat(prmType, "OutWideStringType");
					break;
				case IIDRefType:
					prmType = String::Concat(prmType, "IIDRefType");
					break;
				default:
					prmType = String::Concat(prmType, "PointerType");
					break;
			}
			if (i < params->GetLength(0) - 1)
					prmType = String::Concat(prmType, ", ");
		}
		return prmType;

	}
}