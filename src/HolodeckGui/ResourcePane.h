//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourcePane.h
//
// DESCRIPTION: Contains definition for the class ResourcePane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#pragma once

#include "DockablePane.h"
#include "ResourcePaneDataArray.h"
#include "LogPaneDataArray.h"
#include "ImageListManager.h"
#include "ResourceTestDataNode.h"
#include "VirtualResourceTreeView.h"
#include "FileCorruptionDataNode.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Microsoft::Win32;

namespace HolodeckGui
{
	//*************************************************************************
	// class that all represents the dockable resource pane
	//*************************************************************************
	public __gc class ResourcePane : public DockablePane
	{
	public:
		ResourcePane();
		~ResourcePane();
		void Dispose(bool disposing);
		void AddDataArray(ResourcePaneDataArray *data);
		void ClearAll ();

		__property ResourcePaneDataArray *get_CurrentDataArray() { return dataArray; }

		__property unsigned int get_ProcessID() 
		{ 
			if (dataArray)
				return dataArray->ProcessID; 
			else
				return 0;
		}

		ResourcePaneDataNode * GetSelectedNode();
		String * GetSelectedNodeText();

	private:
		DataEventRegistry *dataEventRegistry;
		unsigned int threadID;
		SortedList *dataArrayList;
		ResourcePaneDataArray *dataArray;
		ItemAddedDelegate *itemAddedHandler;
		ItemModifiedDelegate *itemModifiedHandler;
		FocusChangedDelegate *focusChangedHandler;
		CloseProjectUIDelegate *closeProjectUIHandler;
		ProcessRemovedDelegate *processRemovedHandler;
		bool ignoreFocusChange;

		VirtualResourceTreeView *resourceTreeView;
		TreeNode *selectedNode;

		MenuItem *createFileCorruptionMenu;
		MenuItem *createFaultMenu;
		MenuItem *modifyFaultMenu;
		MenuItem *clearFaultMenu;
		MenuItem *fileFaultMenu;
		MenuItem *registryFaultMenu;
		MenuItem *processFaultMenu;
		MenuItem *customFaultMenu;
		MenuItem *selectLogItemMenu;

		void InitializeUI();
		void onProcessRemoved(unsigned int processID);
		void onFocusChanged(DisplayableDataNode *dataNode);
		void onCloseProjectUI();
		void onItemAdded(DisplayableDataNode *dataNode);
		void onItemModified(DisplayableDataNode *dataNode, String *propertyName);
		void onAfterSelect(Object *sender, EventArgs *e);
		void onRightClick(Object *sender, MouseEventArgs *e);
		void onCopyToClipboard(Object *sender, EventArgs *e);
		void InjectResourceFault(Object *sender, EventArgs *e);
		void DeleteResourceFault(Object *sender, EventArgs *e);
		void StartCreateFileCorruptionWizard(Object *sender, EventArgs *e);
		void StartCreateResourceTestWizard(Object *sender, EventArgs *e);
		void StartResourceTestWizard(TreeNode *treeNode, bool modify);
		void SelectLogEntry(Object *sender, EventArgs *e);
		void onExportToFile(Object *sender, EventArgs *e);
	};
}