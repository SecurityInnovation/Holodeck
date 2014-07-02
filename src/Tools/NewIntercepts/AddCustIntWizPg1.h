//*******************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		AddCustIntWizPg1.h
//
// DESCRIPTION: Contains definition for the class AddCustIntWizPg1
//
//===============================================================================
// Modification History
//
// Date         SCR  Name      Purpose
// -----------  ---  --------  ----------------------------------------
// 01 Oct 2003		 P. Singh  File created.
// 06 Oct 2003		 Ady K	   Added functionality
//*******************************************************************************

#pragma once
#include "PEFile.h"
#include "defines.h"
#include <vcclr.h>
#include "addcustintwizpg2.h"

#undef MessageBox

#using <GuiSupport.dll>
#using <System.dll>
#using <mscorlib.dll>
#using <MagicLibrary.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace SiUtils;
using namespace System::IO;
using namespace System::Drawing;
using namespace System::Threading;
using namespace System::Reflection;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::ComponentModel;
using namespace Crownwood::Magic::Controls;

namespace AddNewIntercepts
{
	//*************************************************************************
	// class that represents the project and function lib selection page of the 
	// Add Custom Intercepts wizard (page 1)
	//*************************************************************************
	public __gc class AddCustIntWizPg1 : public WizardPage
	{
		public:
			AddCustIntWizPg1();
			~AddCustIntWizPg1();

			__property String* get_ProjectPath() {return ProjLocTxtBox->Text;}
			__property ArrayList*  get_IntFuncInformation() {return selectedFunctions;}
			__property unsigned int get_FunctionsCount() {return selectedFunctions->Count;}

			InterceptedFunctionDB * dotNetInterceptedFunctionDB;
			InterceptedFunctionDB * nativeInterceptedFunctionDB;

			void loadDotNetFunctionDatabase();
			void loadNativeFunctionDatabase();

			ArrayList * getFunctionsInformation (ArrayList * funcList, libraryInformation * libInfo, InterceptedFunctionDB * db);
			ArrayList * getFunctionsDefinitionsFromDatabase (InterceptedFunctionDB * db, ArrayList * funcList);
			ArrayList * getFunctionsDefinitionsFromHeaderFiles (ArrayList * functionsList, libraryInformation * libInfo);

		protected:
			GuiSupport::WaitDialog * waitDialog;

			GroupBox *ProjLocGrpBox;
			Button *btnBrowseOutput;
			TextBox *ProjLocTxtBox;
			Panel * pnlExtendedInfo;
			Label *OutputLocLabel;
			OpenFileDialog *selectFileDialog;
			LinkLabel * lnkHeaderFile;
			LinkLabel * lnkCleanNoDefs;

			GroupBox *SelFuncLibGrpBox;
			TreeView *FuncLibTreeView;
			Button *RmFuncLibBtn;
			Button *AddFuncLibBtn;

			String *ProjName;
			String *OutputLoc;

			System::Windows::Forms::FolderBrowserDialog * folderBrowse;
			TreeNode * lastDisplayedNode;
			InterceptedFunction * lastSelectedFuncDefinition;
			ComboBox * cmbHeaders;

			void btnBrowseOutput_Click(Object *sender, EventArgs *e);
			void AddFuncLibBtn_Click(Object *sender, EventArgs *e);
			void RmFuncLibBtn_Click(Object *sender, EventArgs *e);
			void FuncLibTreeView_AfterCheck(Object *sender, TreeViewEventArgs *e);
			void FuncLibTreeView_AfterSelect(Object *sender, TreeViewEventArgs *e);
			void btnHeaderAdd_Click (Object * sender, EventArgs *e);
			void btnHeaderDel_Click (Object * sender, EventArgs *e);
			void funcDef_CheckChanged (Object * sender, EventArgs *e);

			TextBox * getTextBoxFromText (String * text, System::Drawing::Color color, bool IsMultiLine);
			Panel * getBlankPanel (int height, System::Drawing::Color color);
			void addControlToInformationPanel (System::Windows::Forms::Control * control, String * headerText);
			void displayNativeFunctionInformation (TreeNode * funcNode);
			void displayManagedFunctionInformation (TreeNode * funcNode);
			void displayNativeLibraryInformation (TreeNode * libNode);
			void displayManagedLibraryInformation (TreeNode * libNode);
			void UpdateNativeFunctionDefinitions (TreeNode * libNode);
			bool ShouldIncludeType(Type * typeToTest);
			void waitDialogCancelEventHandlerFunc(Object * sender, EventArgs *e);
			void GetLibraryInformationThreadFunc();
			void UpdateWaitDialog(int CurrentModule, int totalModules, int progressValue);
			Type * GetParamTypeArray(MethodInfo * mi)[];
			Type * GetParamTypeArray(ConstructorInfo * ci)[];
			void AddInformationToNode(TreeNode *node, String * moduleName, String * memberName, String * libraryName, String * scopeName, bool isConstructor, bool isPublic, bool isStatic, Type * paramArray[], libraryInformation * libInfo);

		private:
			ArrayList * selectedFunctions;
			Threading::Thread * queryThread;
			TreeNode * nodeToAdd;
			TreeViewEventHandler * FuncLibTreeView_AfterCheckHandler;

			BindingFlags bfPublicStatic;
			BindingFlags bfPublicInstance;
			BindingFlags bfNonPublicStatic;
			BindingFlags bfNonPublicInstance;

			void AddToList (SortedList * list, TreeNode * node);
			void lnkCleanNoDefs_LinkClicked(Object * sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs * e);

	};
}