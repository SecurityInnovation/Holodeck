//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		DockablePaneManager.h
//
// DESCRIPTION: Contains definition for the class DockablePaneManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 1 Jul 2004		 B. Shirey	 File created.
//*************************************************************************
#pragma once

#include <stdio.h>
#include "DockablePane.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <MagicLibrary.dll>

using namespace System;
using namespace System::Collections;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Common;
using namespace Crownwood::Magic::Docking;
using namespace Crownwood::Magic::Collections;

namespace SIControls
{
	public __delegate void DockablePaneManagerPaneHiddenDelegate(DockablePane *pane);
	public __delegate void DockablePaneManagerPaneShownDelegate(DockablePane *pane);

	public __gc class DockablePaneManager
	{
	public:
		DockablePaneManager(System::Windows::Forms::ContainerControl *control, VisualStyle style);
		~DockablePaneManager();

		DockablePaneManagerPaneHiddenDelegate *OnPaneHidden;
		DockablePaneManagerPaneShownDelegate *OnPaneShown;

		int AddImageToList(String *fileName);
		int AddImageToList(Bitmap *image);

		void AddPane(DockablePane *pane);
		DockablePane *GetPane(String *group, String *name);
		void RemovePane(DockablePane *pane);
		void RemovePane(String *group, String *name);

		void SetGroupPosition(String *group, DockablePanePosition position);
		DockablePanePosition GetGroupPosition(String *group);
		
		void SetPaneIsVisible(String *group, String *name, bool isVisible);
		void SetPaneIsVisible(DockablePane *pane, bool isVisible);
		bool GetIsPaneVisible(String *group, String *name);
		bool GetIsPaneVisible(DockablePane *pane);

		void RefreshLayout();
		void RefreshLayoutOfGroup(String *group);

		__property void set_FirstControlThatCannotBeMovedDueToDocking(Control *value) { dockManager->OuterControl = value; }
		__property Control *get_FirstControlThatCannotBeMovedDueToDocking() { return dockManager->OuterControl; }

		__property void set_LastControlThatCannotBeMovedDueToDocking(Control *value) { dockManager->InnerControl = value; }
		__property Control *get_LastControlThatCannotBeMovedDueToDocking() { return dockManager->InnerControl; }

		__property DockingManager *get_DockManager() { return dockManager; }

		__property System::Windows::Forms::ImageList *get_ImageList() { return imageList; }

		__property int get_EmptyImageIndex() { return 0; }

	private:
		void OnPaneNameChanged(Pane *pane, String *oldName, String *newName);
		void OnPaneTitleChanged(Pane *pane, String *paneName, String *oldTitle, String *newTitle);
		void OnPaneImageIndexChanged(Pane *pane, String *paneName, int oldIndex, int newIndex);
		Content *GetContentForPane(DockablePane *pane);
		int GetInsertPositionForContentList(ArrayList *list, DockablePane *pane);
		int GetInsertPositionForWindowContentList(WindowCollection *list, DockablePanePosition positionToInsert);
		void OnPaneVisibilityChangedToHidden(Content *c, EventArgs *args);
		void OnPaneVisibilityChangedToShown(Content *c, EventArgs *args);

		VisualStyle visualStyle;
		System::Windows::Forms::ImageList *imageList;
		Hashtable *groupToContentListTable;
		Hashtable *groupPositionsTable;
		Content *lastVerticalLeftContentAdded;
		Content *lastVerticalRightContentAdded;
		Content *lastHorizontalTopContentAdded;
		Content *lastHorizontalBottomContentAdded;
		DockingManager *dockManager;
		ContainerControl *parentControl;
		PaneNameChangedDelegate *PaneNameChangedHandler;
		PaneTitleChangedDelegate *PaneTitleChangedHandler;
		PaneImageIndexChangedDelegate *PaneImageIndexChangedHandler;
	};
}