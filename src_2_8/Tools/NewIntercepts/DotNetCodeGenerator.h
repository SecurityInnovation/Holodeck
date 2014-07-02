//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DotNetCodeGenerator.h
//
// DESCRIPTION: Contains definition for the class DotNetCodeGenerator
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  --------  ----------------------------------------
// 14 Nov 2003		 Ady K		 File created.
//*******************************************************************************
#include <windows.h>
#include "defines.h"

#using <System.dll>
#using <mscorlib.dll>
#using <FunctionDatabase.dll>
#using <GuiSupport.dll>

#undef MessageBox
#undef GetTempFileName
#undef CreateDirectory

using namespace System;
using namespace System::IO;
using namespace FunctionDatabase;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Windows::Forms;


namespace AddNewIntercepts
{
	public __gc class DotNetCodeGenerator
	{
		public:
			DotNetCodeGenerator(bool isAddNewIntercepts);
			~DotNetCodeGenerator();
			
			void CreateProject (String * projectFileName, ArrayList * libraryPathList, ArrayList * interceptArray);
		
		private:
			OpenFileDialog * fileDialog;
			String  * frameworkDirectory;
			StreamWriter * xmlFile;
			Hashtable * methodsTable;

			bool isAddNewIntercepts;
			String * templatePath;
			String * holodeckPath;
			String * replacementFolderPath;
			String * holodeckDatabaseFolderPath;
			String * gacUtilPath;

			String * ShouldInclude(String * methodName, ParameterInfo * paramList[]);
			static String * GetTypeName(Type * t);
			static String * GetHelpLink(System::Type * t, System::Reflection::ConstructorInfo *c);
			static String * GetHelpLink(System::Type * t, System::Reflection::MethodInfo *m);
			String * GetMethodNameWithParams (String * methodName, ParameterInfo * paramList[]);
			
			void GenerateLibrary(ArrayList * assemblyFileNameList, ArrayList * interceptMethods, String * outFileName);
			void GenerateProjectFile (String * projFileName, ArrayList * libraryPathList);

			String* getLibReferencesText(ArrayList * listOfLibraries);

			void GetFrameworkDir();
			System::Reflection::Assembly* LoadAssembly (String * assemblyString, bool askUserIfNotFound);
	};
}