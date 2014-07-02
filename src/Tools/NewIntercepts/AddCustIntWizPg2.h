//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg2.h
//
// DESCRIPTION: Contains definition for the class AddCustIntWizPg2
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  -------   ----------------------------------------
// 01 Oct 2003		 P. Singh  File created.
// 06 Oct 2003		 Ady K	   Added functionality
//*******************************************************************************
#pragma once

#include "PEFile.h"
#include "defines.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <HeaderParser.dll>
#using <FunctionDatabase.dll>

using namespace System;
using namespace System::IO;
using namespace SiUtils;
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
	public __gc struct functionInformation
	{
		String * functionName;
		String * libraryName;
		String * replacementLibraryName;
		String * moduleName;
		String * typeName;
		Type * paramTypeArray[];
		InterceptedFunction * selectedDefinition;
		ArrayList * possibleDefinitions;
		bool	isManaged;
		bool	isConstructor;
		BindingFlags bindingFlags;

		functionInformation(String * functionName, String * libraryName, bool isManaged)
		{
			this->functionName = functionName;
			this->libraryName = libraryName;
			this->isManaged = isManaged;
			this->isConstructor = false;
			this->selectedDefinition = NULL;
			this->replacementLibraryName = NULL;
			moduleName = NULL;
			typeName = NULL;
			bindingFlags = BindingFlags::Default;
			possibleDefinitions = new ArrayList();
			paramTypeArray = NULL;
		}
	};

	public __gc struct libraryInformation
	{
		String * fullLibraryPath;
		ArrayList * nativeHeaderSearchFiles;
		bool isManaged;

		libraryInformation (String * fullLibraryPath, bool isManaged)
		{
			this->fullLibraryPath = fullLibraryPath;
			this->isManaged = isManaged;
			this->nativeHeaderSearchFiles = new ArrayList();
		}
	};

	public __gc struct definitionInfo
	{
		InterceptedFunction * definition;
		String * definitionSource;

		definitionInfo (InterceptedFunction * funcDefinition, String * definitionSource)
		{
			this->definition = funcDefinition;
			this->definitionSource = definitionSource;
		}
	};

	//*************************************************************************
	// class that represents the function details and modification page of the 
	// Add Custom Intercepts wizard (page 2)
	//*************************************************************************
	public __gc class AddCustIntWizPg2 : public WizardPage
	{
		public:
			AddCustIntWizPg2();
			~AddCustIntWizPg2();

			InterceptedFunction * getManagedDefinition (functionInformation * fInfo);
			
			__property ArrayList* get_FunctionsList()
			{
				ArrayList * list = new ArrayList();
				for (int i=0; i < this->lvCustFuncSpecList->Items->Count; i++)
				{
					functionInformation * ifunc = dynamic_cast <functionInformation *> (lvCustFuncSpecList->Items->Item[i]->Tag);
					list->Add (ifunc);
				}
				return list;
			}

			__property String * get_ProjectPath() {return projectPath;}
			__property void set_ProjectPath(String * value)
			{
				projectPath = value;
				this->SubTitle = String::Concat (this->subTitleText, "\n\nYour project(s) will be located in ", projectPath, ".");
			}

			void updateFunctionSpecificationView(ArrayList * funcInformation);
			
		protected:
			GroupBox *CustFuncSpecGrpBox;
			Button *EditDetailsBtn;
			ListView *lvCustFuncSpecList;
			ColumnHeader *colEdited; 
			ColumnHeader *colFuncName;
			ColumnHeader *colTypeName; 
			ColumnHeader *colLocation; 
			ColumnHeader *colFuncMods; 
			ColumnHeader *colRetType; 
			ColumnHeader *colNParams; 
			ColumnHeader *colParamTypes;

			void onEditDetailsBtnClick (Object *sender, EventArgs *e);
			void updateListViewItemByIndex (int index, functionInformation * mInfo);
		private:
			String * getBaseName (String * fullFunctionName);
			String * projectPath;
			String * subTitleText;
			System::Windows::Forms::ImageList * imgList;
	};
}