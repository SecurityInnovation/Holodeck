//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		NetworkCorruptionPortSelectionPage.h
//
// DESCRIPTION: Contains definition for the class NetworkCorruptionPortSelectionPage
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#pragma once

#using <mscorlib.dll>
#using <System.dll>
#using <System.Drawing.dll>
#using <System.Windows.Forms.dll>
#using <FunctionDatabase.dll>
#using <MagicLibrary.dll>
#using <SiControls.dll>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Drawing;
using namespace System::Windows::Forms;
using namespace Crownwood::Magic::Controls;
using namespace FunctionDatabase;
using namespace SIControls;

namespace HolodeckGui
{
	public __gc struct PortNode
	{
		int port;
		bool selected;
		bool focused;
	};

	//*************************************************************************
	// class that represents the port selection page of the network corruption wizard
	//*************************************************************************
	public __gc class NetworkCorruptionPortSelectionPage : public WizardPage
	{
	public:
		NetworkCorruptionPortSelectionPage();
		~NetworkCorruptionPortSelectionPage();

		__property ArrayList *get_PortList();
		__property String *get_PortString();
		__property void set_PortList(ArrayList *value);
		__property bool get_SentData();
		__property bool get_ReceivedData();

		void SetSentAndReceived(bool sent, bool received);

	protected:
		RadioButton *receiveOnly;
		RadioButton *sendOnly;
		RadioButton *receiveAndSend;
		ContainerListView *excluded;
		ContainerListView *included;
		Button *addButton;
		Button *removeButton;
		Button *addAllButton;
		Button *removeAllButton;
		ArrayList *excludedPorts;
		ArrayList *includedPorts;
		PortDB* portDB;

		void onAddClicked(Object *sender, EventArgs *e);
		void onRemoveClicked(Object *sender, EventArgs *e);
		void onAddAllClicked(Object *sender, EventArgs *e);
		void onRemoveAllClicked(Object *sender, EventArgs *e);

		void AddPortToList(ArrayList* list, int insertPortNum);
		void UpdateListBoxItemCounts();

		void onExcludedSetSelected(int index, bool selected);
		void onExcludedSetFocused(int index, bool focused);
		ContainerListViewItem *onExcludedQueryItem(unsigned int index);
		unsigned int onExcludedQueryItemIndex(ContainerListViewItem *item);

		void onIncludedSetSelected(int index, bool selected);
		void onIncludedSetFocused(int index, bool focused);
		ContainerListViewItem *onIncludedQueryItem(unsigned int index);
		unsigned int onIncludedQueryItemIndex(ContainerListViewItem *item);
	};
}