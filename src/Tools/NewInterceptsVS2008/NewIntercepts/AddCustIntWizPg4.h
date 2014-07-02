//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg4.h
//
// DESCRIPTION: Contains definition for the class AddCustIntWizPg4
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

#undef MessageBox

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <HeaderParser.dll>
#using <FunctionDatabase.dll>

using namespace System;
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
	public __gc class AddCustIntWizPg4 : public WizardPage
	{
		public:
			AddCustIntWizPg4();
			~AddCustIntWizPg4();

			__property void set_ProjectsTable(Hashtable * value) {projectsTable = value;}
			__property void set_ProjectsLocations(Hashtable * value) {projectsLocations = value;}

			__property void set_ProjectsPath(String * value)
			{
				if (! value->EndsWith (Path::DirectorySeparatorChar.ToString()))
					value = String::Concat (value, Path::DirectorySeparatorChar.ToString());

				this->projectsPath = value;
			}

			void displayCompletedDetails();
			void displayFailureDetails();

		protected:

		private:
			Hashtable * projectsTable;
			Hashtable * projectsLocations;
			String * projectsPath;

			GroupBox * grpMain;
			RichTextBox * rtbDetails;

			void setRtbTextStyle (float newFontSize, System::Drawing::FontStyle newFontStyle, System::Drawing::Color newFontColor);
	};
}