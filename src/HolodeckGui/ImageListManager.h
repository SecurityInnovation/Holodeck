//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ImageListManager.h
//
// DESCRIPTION: Contains definition for the class ImageListManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 20 Mar 2003		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#define PAUSED_PROCESS_INDEX ImageListManager::getInstance()->NodeTypeImageList->Images->Count-2
#define TERMINATED_PROCESS_INDEX ImageListManager::getInstance()->NodeTypeImageList->Images->Count-1

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <GuiSupport.dll>

#include <stdio.h>
#include "defines.h"

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace GuiSupport;

namespace HolodeckGui
{
	__value enum ButtonBarTypes			//enum containing all the button bar types
	{
		NewProjectType,
		OpenProjectType,
		SaveProjectType,
		NewTestAppType,
		CreateTestType,
		CreateResourceTestType,
		NewFaultType,
		CreateReportType,
		ProjectPaneType,
		ResourcePaneType,
		LimitsPaneType,
		FileCorruptionDetailsPaneType,
		NetworkMessageDetailsPaneType,
		PropertiesPaneType,
		DynamicHelpType,
		FileCorruptionFaultType,
		NetworkCorruptionFaultType
	};

	__value enum LogListViewIconTypes
	{
		NetworkLogIcon = 1
	};

	//*************************************************************************
	// class that manages all Holodeck images
	//*************************************************************************
	public __gc class ImageListManager
	{
	private:
		static ImageListManager *instance = NULL;

		ImageList *nodeTypeImageList;
		ImageList *buttonBarImageList;
		ImageList *logTreeViewImageList;
		String *installFolder;

		System::Drawing::Icon *appIcon;
		System::Drawing::Bitmap *logoBitmap;
		System::Drawing::Bitmap *sortAscendingIcon;
		System::Drawing::Bitmap *sortDescendingIcon;

		ImageListManager();
		~ImageListManager();

		void CreateNodeTypeImageList();
		void CreateButtonBarImageList();
		void CreateLogTreeViewImageList();
	public:
		static ImageListManager *getInstance();

		__property ImageList *get_NodeTypeImageList() { return nodeTypeImageList; }
		__property ImageList *get_ButtonBarImageList() { return buttonBarImageList; }
		__property ImageList *get_LogTreeViewImageList() { return logTreeViewImageList; }
		__property Icon *get_AppIcon() { return appIcon; }
		__property Bitmap *get_LogoImage() { return logoBitmap; }
		__property Bitmap *get_SortAscendingIcon() { return sortAscendingIcon; }
		__property Bitmap *get_SortDescendingIcon() { return sortDescendingIcon; }
	};
}