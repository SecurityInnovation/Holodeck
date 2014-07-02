//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CodeGenerator.h
//
// DESCRIPTION: Contains definition for the class InterceptCodeGenerator
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  --------  ----------------------------------------
// 10 Nov 2003		 Ady K		 File created.
//*******************************************************************************

#include <windows.h>
#include "ParameterType.h"
#include "defines.h"

#undef CreateDirectory
#undef MessageBox

#using <mscorlib.dll>
#using <System.dll>

#using <FunctionDatabase.dll>

using namespace System;
using namespace FunctionDatabase;
using namespace System::Collections;
using namespace System::IO;
using namespace System::Windows::Forms;

namespace AddNewIntercepts
{
	public __gc class ProjectInformation
	{
		public:
			String * projectName;
			String * projectPath;
			String * originalDll;

			ProjectInformation(String * projectName, String * projectPath, String * originalDll)
			{
				this->projectName = projectName;
				this->projectPath = projectPath;
				this->originalDll = originalDll;
			}
	};
	
	public __gc class InterceptCodeGenerator
	{
		public:
			InterceptCodeGenerator();
			~InterceptCodeGenerator();

			int CreateProject (String * fileName, ArrayList * interceptedFunctions);
			//String * BuildProject (String * devenvPath);

		private:
			ArrayList * beforeReturnTypeKeywords;
			String * templatePath;
			String * holodeckPath;
			String * replacementFolderPath;
			String * holodeckDatabaseFolderPath;

			ArrayList * callerTemplate;
			ArrayList * replTemplate;

			int createProjectFile (String * projFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			int createCppFile (String * cppFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			int createFuncCppFiles (ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			int createHeaderFile (String * hFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			int createDefFile (String * defFileName, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			void insertCallerFunction (StreamWriter * cppWriter, InterceptedFunction * iFunction, ProjectInformation * projectInfo);
			void insertReplacementFunction (StreamWriter * cppWriter, InterceptedFunction * iFunction, ProjectInformation * projectInfo);
			void insertHeaderDefinitions (StreamWriter * cppWriter, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			void insertExportedList (StreamWriter * defWriter, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			void insertCppFileRefsInVCProj(StreamWriter * fWriter, ArrayList * interceptedFunctions, ProjectInformation * projectInfo);
			String * getModifiers (Array * modifiers, int modTypeRequested);

			String * ParseAndReplaceString(String * templateString, InterceptedFunction * function, ProjectInformation * projInfo);
			String * GetCastedParamsList(Array *params);
			String * GetParamTypesAndNames(Array *params);
			String * GetParamTypesList(Array *params);
			int funcNumSiStringParams(Array *params);
			String * GetSiStringLine(Array *params);
			String * GetParamRefsList(Array *params);
			String * GetBasicParamTypeList(Array *params);
	};
}