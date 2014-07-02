//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		LimitPane.h
//
// DESCRIPTION: Contains definition for the class LimitPane
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
#include "LogPaneDataArray.h"
#include "FaultPaneDataArray.h"
#include "LimitPaneDataArray.h"
#include "DataEventRegistry.h"

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Drawing;
using namespace System::Reflection;
using namespace System::Windows::Forms;

namespace HolodeckGui
{
	//*************************************************************************
	// class that represents the dockable resource pane
	//*************************************************************************
	public __gc class LimitPane : public DockablePane
	{
	public:
		LimitPane();
		~LimitPane();
		void AddDataArray(LimitPaneDataArray *data);
		void ClearAll ();
		void Dispose(bool disposing);

		__property unsigned int get_ProcessID() { return processID; }
		__property LimitPaneDataArray *get_DataArray() { return dataArray; }

	private:
		ItemDeletedDelegate *itemDeletedHandler;
		ItemAddedDelegate *itemAddedHandler;
		ItemModifiedDelegate *itemModifiedHandler;

		SortedList *dataArrayList;
		LimitPaneDataArray *dataArray;

		Windows::Forms::Timer *timer;
		DateTime oldTime;
		long oldCurrentUsageSample;
		unsigned int processID;
		DataEventRegistry *dataEventRegistry;
		DeleteLimitDelegate *DeleteLimitHandler;
		CreateLimitDelegate *CreateLimitHandler;
		ModifyLimitDelegate *ModifyLimitHandler;
		FocusChangedDelegate *FocusChangedHandler;
		CloseProjectUIDelegate *CloseProjectUIHandler;
		HoloScriptApplication *holoScriptApp;	
		UInt64 realMaxDisk, realMaxMemory, realMaxNetworkUp, realMaxNetworkDown, diskUnits, memoryUnits, networkUpUnits, networkDownUnits;
		UInt64 maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage, maximumNetworkUpUsage, currentNetworkUpUsage, maximumNetworkDownUsage, currentNetworkDownUsage;
		bool memoryLimitTextBoxFocus, memoryTrackBarFocus, diskLimitTextBoxFocus, diskTrackBarFocus, networkUpLimitTextBoxFocus, networkUpTrackBarFocus, networkDownLimitTextBoxFocus, networkDownTrackBarFocus;
		bool diskLimitEnabled, memoryLimitEnabled, networkUpLimitEnabled, networkDownLimitEnabled;

		Panel *diskPanel;
		Panel *memoryPanel;
		Panel *networkUpPanel;
		Panel *networkDownPanel;

		Label *diskLabel;
		Label *diskLimitLabel;
		Label *diskUsedLabel;
		Label *diskUnitsLabel;
		Label *memoryLabel;
		Label *memoryLimitLabel;
		Label *memoryUsedLabel;
		Label *memoryUnitsLabel;
		Label *networkUpLabel;
		Label *networkUpLimitLabel;
		Label *networkUpUsedLabel;
		Label *networkUpUnitsLabel;
		Label *networkDownLabel;
		Label *networkDownLimitLabel;
		Label *networkDownUsedLabel;
		Label *networkDownUnitsLabel;

		TrackBar *diskTrackBar;
		TrackBar *memoryTrackBar;
		TrackBar *networkUpTrackBar;
		TrackBar *networkDownTrackBar;

		ProgressBar *diskProgressBar;
		ProgressBar *memoryProgressBar;
		ProgressBar *networkUpProgressBar;
		ProgressBar *networkDownProgressBar;

		TextBox *diskUsedTextBox;
		TextBox *memoryUsedTextBox;
		TextBox *networkUpUsedTextBox;
		TextBox *networkDownUsedTextBox;

		TextBox *diskLimitTextBox;
		TextBox *memoryLimitTextBox;
		TextBox *networkUpLimitTextBox;
		TextBox *networkDownLimitTextBox;

		ComboBox *diskUnitsComboBox;
		ComboBox *memoryUnitsComboBox;
		ComboBox *networkUpUnitsComboBox;
		ComboBox *networkDownUnitsComboBox;

		void InitializeUI();
		void LayoutChanged(Object *sender, LayoutEventArgs *e);
		void onFocusChanged(DisplayableDataNode *dataNode);
		void onCloseProjectUI();
		void onItemDeleted(DisplayableDataNode *dataNode);
		void onItemAdded(DisplayableDataNode *dataNode);
		void onItemModified(DisplayableDataNode *dataNode, String *propertyName);
		void diskTrackBarMouseDown(Object *sender, MouseEventArgs *e);
		void diskTrackBarValueChanged(Object *sender, MouseEventArgs *e);
		void diskLimitTextBoxGotFocus(Object *sender, EventArgs *e);
		void diskLimitTextBoxTextChanged(Object *sender, EventArgs *e);
		void diskLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e);
		void diskUnitsComboBoxIndexChanged(Object *sender, EventArgs *e);
		void memoryTrackBarMouseDown(Object *sender, MouseEventArgs *e);
		void memoryTrackBarValueChanged(Object *sender, MouseEventArgs *e);
		void memoryLimitTextBoxGotFocus(Object *sender, EventArgs *e);
		void memoryLimitTextBoxTextChanged(Object *sender, EventArgs *e);
		void memoryLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e);
		void memoryUnitsComboBoxIndexChanged(Object *sender, EventArgs *e);
		void networkUpTrackBarMouseDown(Object *sender, MouseEventArgs *e);
		void networkUpTrackBarValueChanged(Object *sender, MouseEventArgs *e);
		void networkUpLimitTextBoxGotFocus(Object *sender, EventArgs *e);
		void networkUpLimitTextBoxTextChanged(Object *sender, EventArgs *e);
		void networkUpLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e);
		void networkUpUnitsComboBoxIndexChanged(Object *sender, EventArgs *e);
		void networkDownTrackBarMouseDown(Object *sender, MouseEventArgs *e);
		void networkDownTrackBarValueChanged(Object *sender, MouseEventArgs *e);
		void networkDownLimitTextBoxGotFocus(Object *sender, EventArgs *e);
		void networkDownLimitTextBoxTextChanged(Object *sender, EventArgs *e);
		void networkDownLimitTextBoxKeyDown(Object *sender, KeyEventArgs *e);
		void networkDownUnitsComboBoxIndexChanged(Object *sender, EventArgs *e);
		void UpdateResourceMonitors();
		void onTimerTick(Object *sender, EventArgs *e);
	};
}