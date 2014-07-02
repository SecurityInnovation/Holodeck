//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		CustomTestProjectWizardControl.h
//
// DESCRIPTION: Contains definition for the class CustomTestProjectWizardControl
//
//===============================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 12 Feb 2004		 Ady K	  File created.
//*******************************************************************************
#pragma once

#include <defines.h>
#include <vcclr.h>
#include "..\..\CLRInterceptLib\CLRIntercept.h"
#include "CustomInterceptProject.h"
#include "CIProjectDetails.h"
#include "CIProjectOptions.h"
#include "CIProjectSummary.h"

#undef MessageBox
#undef CreateDirectory

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <NewIntercepts.dll>

using namespace System;
using namespace Microsoft::Win32;
using namespace System::IO;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;

namespace CustomTestProjectGenerator
{
	public __gc class CustomTestProjectWizardControl : public WizardControl
	{
		protected:
			ArrayList * uinqueNameList;
			
			String * ReplacementDir;
			String * functionDBFolder;
			String * HolodeckInstallPath;
			String * CustomTestProjectGeneratorPath;
			
			CIProjectSummary * cipSummaryPage;
			CustomInterceptProject * cipLaunchPage;
			CIProjectOptions * cipProjectOptionsPage;
			CIProjectDetails * cipFunctionDetailsPage;

			InterceptedFunctionDB * functionDatabase;

			void onPageEnter(WizardPage *wp, WizardControl *wc);
			void onPageLeave(WizardPage *wp, WizardControl *wc);
			void onNextClick(Object *sender, CancelEventArgs *e);
			void onFinishClick(Object *sender, EventArgs *e);
			void onCancelClick(Object *sender, EventArgs *e);

			WizardPage *currentWP;

			void makeTestProject();
			
			//Thread functions for creating test projects
			void CSharpProject_Thread();
			void VBProject_Thread();
			void CPPProject_Thread();

			String * ProjectPath;

			void projectSummaryText(String * text);
			bool CreateOutputPath(String * ProjectPath);
			
			bool native_CopyLibraries(String * ProjectPath);
			bool native_CreateInterceptionLibrary(String * projectPath, String * projectName, ArrayList * interceptedFunctionList);
			bool native_CreateControlExe(String * ProjectPath, String * projectName, ArrayList * interceptedFunctionList);
			bool native_CreateSolution(String * ProjectPath, String * projectName);
			
			bool csharp_CreateInterceptionLibrary(String * projectPath, String * projectName, ArrayList * interceptedFunctionList);
			bool csharp_CreateControlExe(String * ProjectPath, String * projectName, ArrayList * interceptedFunctionList);
			bool csharp_CreateSolution(String * ProjectPath, String * projectName);

			//Parsing functions
			String * ParseAndReplaceString(String * templateString, InterceptedFunction * function);
			String * GetParamTypesAndNames(Array * params);
			String * GetHandleNameForReplLib(String * replDllName);
			String * GetFullPathToReplLib(String * replDllName);
			String * GetParamNames(Array * params);

			String * GetValidCodeName (String * faultName);
			String * GetUniqueName (String * faultName);

		public:
			CustomTestProjectWizardControl();
			~CustomTestProjectWizardControl();

			void SetNextButtonEnabled(bool enabled) { this->EnableNextButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }
			void SetBackButtonEnabled(bool enabled) { this->EnableBackButton = (enabled ? WizardControl::Status::Yes : WizardControl::Status::No); }	
	};
}