//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectWorkspaceSelectionPage.h
//
// DESCRIPTION: Contains definition for the class ProjectWorkspaceSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 28 Apr 2003		 J. Taylor   File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>
#using <GuiSupport.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace GuiSupport;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the workspace selection page of the create project
	// wizard
	//*************************************************************************
	public __gc class ProjectWorkspaceSelectionPage : public WizardPage
	{
	public:
		ProjectWorkspaceSelectionPage();
		~ProjectWorkspaceSelectionPage();

		__property String *get_ProjectFolder() 
		{ 		
			String *value = pathTextBox->Text;

			int index = value->LastIndexOf(".hdp");
			if (index != -1)
				value = value->Substring(0, index);

			index = value->LastIndexOf("\\");
			while ( (index != -1) && (index == value->Length - 1) )
			{
				value = value->Remove(index, 1);
				index = value->LastIndexOf("\\");
			}
			return value; 		
		}
		__property void set_ProjectFolder(String *value) 
		{ 
			int index = value->LastIndexOf("\\");
			while ( (index != -1) && (index == value->Length - 1) )
			{
				value = value->Remove(index, 1);
				index = value->LastIndexOf("\\");
			}
			pathTextBox->Text = value; 
		}

		__property String *get_ProjectName() 
		{ 
			String *value = nameTextBox->Text;
			int index = value->IndexOf("\\");
			while (index == 0)
			{
				value = value->Remove(index, 1);
				index = value->IndexOf("\\");
			}

			index = value->LastIndexOf(".hdp");
			if (index != -1)
				return value->Substring(0, index);
			else
				return value; 
		}
		__property void set_ProjectName(String *value)
		{		
			int index = value->IndexOf("\\");
			while (index == 0)
			{
				value = value->Remove(index, 1);
				index = value->IndexOf("\\");
			}
			nameTextBox->Text = value; 
		}

		__property String *get_ProjectPath() 
		{ 
			String *fileName;
			if (ProjectName->Length > 0)
				fileName = String::Concat(ProjectName, ".hdp");;

			return String::Concat(ProjectFolder, "\\", fileName); 
		}
		__property void set_ProjectPath(String *value) 
		{ 
			int index = value->LastIndexOf('\\');
			if (index == -1)
			{
				ProjectFolder = "c:";
				ProjectName = value;
			}
			else
			{
				ProjectFolder = value->Substring(0, index);
				ProjectName = value->Substring(index+1);
			}
		}

	protected:
		Label *pathLabel;
		Label *pathValueLabel;
		Label *nameLabel;
		Label *nameValueLabel;
		Label *pathTextLabel;
		Label *nameTextLabel;
		TextBox *pathTextBox;
		TextBox *nameTextBox;
		Button *browseButton;
		SaveFileDialog *saveDialog;

		void onBrowseClick(Object *sender, EventArgs *e);
		void onPathChanged(Object *sender, EventArgs *e);
	};
}