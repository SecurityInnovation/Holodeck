//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg3.h
//
// DESCRIPTION: Contains definition for the class AddCustIntWizPg3
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 10 Nov 2003		 Ady K.		File created.
//*******************************************************************************
#pragma once
#include <windows.h>
#include "CodeGenerator.h"
#include "DotNetCodeGenerator.h"
#include "AddCustIntWizPg2.h"

#undef MessageBox

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <HeaderParser.dll>
#using <FunctionDatabase.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::Text;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace System::Collections;
using namespace System::Reflection;
using namespace HeaderParser;
using namespace FunctionDatabase;

namespace AddNewIntercepts
{
	//*************************************************************************
	// class that represents the function details and modification page of the 
	// Add Custom Intercepts wizard (page 2)
	//*************************************************************************
	public __gc class AddCustIntWizPg3 : public WizardPage
	{
		public:
			AddCustIntWizPg3();
			~AddCustIntWizPg3();

			__property Hashtable *get_ProjectsTable() { return projectsTable;}
			__property void set_ProjectsTable(Hashtable * value) {projectsTable = value;}

			__property Hashtable *get_ProjectsLocations() { return projectsLocations;}

			__property void set_ProjectPath(String * value)
			{
				if (! value->EndsWith (Path::DirectorySeparatorChar.ToString()))
					value = String::Concat (value, Path::DirectorySeparatorChar.ToString());

				this->projectPath = value;
			}
			__property String * get_ProjectPath() {return this->projectPath;}

			void updateDetails();
			void generateCode();
			void updateHolodeckDatabase();
			void updateReplacementReference();

		protected:

		private:
			ArrayList * reservedNames;
			Hashtable * projectsTable;
			Hashtable * projectsLocations;
			String * projectPath;

			GroupBox * grpMain;
			RichTextBox * rtbDetails;
			Label * lblProgress;

			InterceptedFunctionDB * dotNetInterceptedFunctionDB;
			InterceptedFunctionDB * nativeInterceptedFunctionDB;

			void loadDotNetFunctionDatabase();
			void loadNativeFunctionDatabase();
	
			void setRtbTextStyle (float newFontSize, System::Drawing::FontStyle newFontStyle, System::Drawing::Color newFontColor);
			void assignDirectoryLocations();
			void setProgressText(String * progressText);
			String * getDefaultReplacementLibraryName (String * originalLibraryName);
			String * getReplacementLibraryName (String * projectPath);
			ArrayList * getInterceptedFunctionArray(ArrayList * fInfoArray);
	};
}