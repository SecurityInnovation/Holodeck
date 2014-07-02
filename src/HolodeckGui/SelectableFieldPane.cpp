//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SelectableFieldPane.cpp
//
// DESCRIPTION: Contains some implementation for the abstract class SelectableFieldPane
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 B. Shirey	 File created.
//*************************************************************************
#include "SelectableFieldPane.h"

namespace HolodeckGui
{
	//*************************************************************************
	// Method:		SelectableFieldPane
	// Description: Constructor for the SelectableFieldPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SelectableFieldPane::SelectableFieldPane() : Pane()
	{
	}

	//*************************************************************************
	// Method:		~SelectableFieldPane
	// Description: Destructor for the SelectableFieldPane class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SelectableFieldPane::~SelectableFieldPane()
	{
	}

	//*************************************************************************
	// Method:		SaveSettings
	// Description: Saves the settings set by the field chooser
	//
	// Parameters:
	//	paneName - the name of the pane to save the settings for
	//
	// Return Value: None
	//*************************************************************************
	void SelectableFieldPane::SaveSettings(String *paneName)
	{
		if (!paneName)
			return;

		ArrayList *visibleHeaders = this->VisibleColumns;
		ArrayList *allHeaders = this->AllColumns;

		String *hdPanesKeyName = String::Concat(ROOT_PATH, "\\", PANES_KEY_NAME);
		RegistryKey *hdPanesKey = Registry::CurrentUser->OpenSubKey(hdPanesKeyName, true);
		if (!hdPanesKey)
			hdPanesKey = Registry::CurrentUser->CreateSubKey(hdPanesKeyName);

		IEnumerator *enumerator = allHeaders->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Object *header = NULL;
			String *headerText = NULL;
			header = dynamic_cast<ColumnHeader *>(enumerator->Current);
			if (!header)
			{
				// try ToggleColumnHeader
				header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
				if (!header)
					continue;
				headerText = (dynamic_cast<ToggleColumnHeader *>(header))->Text;
			}
			else
			{
				headerText = (dynamic_cast<ColumnHeader *>(header))->Text;
			}

			String *paneKeyName = String::Concat(paneName, "\\", headerText);
			RegistryKey *paneKey = hdPanesKey->OpenSubKey(paneKeyName, true);
			if (!paneKey)
				paneKey = hdPanesKey->CreateSubKey(paneKeyName);

			unsigned long value = 0;
			if (visibleHeaders->Contains(header))
				value = 1;

			paneKey->SetValue(PANE_VISIBLE_VALUE_NAME, __box(value));
			paneKey->Close();
		}
		hdPanesKey->Close();
	}

	//*************************************************************************
	// Method:		LoadSettings
	// Description: Loads the settings to the field chooser
	//
	// Parameters:
	//	paneName - the name of the pane to load the settings for
	//
	// Return Value: None
	//*************************************************************************
	void SelectableFieldPane::LoadSettings(String *paneName)
	{
		if (!paneName)
			return;

		ArrayList *visibleHeaders = new ArrayList();
		ArrayList *allHeaders = this->AllColumns;

		RegistryKey *hdPanesKey = Registry::CurrentUser->OpenSubKey(String::Concat(ROOT_PATH, "\\", PANES_KEY_NAME));
		if (!hdPanesKey)
			return;

		IEnumerator *enumerator = allHeaders->GetEnumerator();
		while (enumerator->MoveNext())
		{
			Object *header = NULL;
			String *headerText = NULL;
			header = dynamic_cast<ColumnHeader *>(enumerator->Current);
			if (!header)
			{
				// try ToggleColumnHeader
				header = dynamic_cast<ToggleColumnHeader *>(enumerator->Current);
				if (!header)
					continue;
				headerText = (dynamic_cast<ToggleColumnHeader *>(header))->Text;
			}
			else
			{
				headerText = (dynamic_cast<ColumnHeader *>(header))->Text;
			}

			String *paneKeyName = String::Concat(paneName, "\\", headerText);
			RegistryKey *paneKey = hdPanesKey->OpenSubKey(paneKeyName);
			if (!paneKey)
			{
				visibleHeaders->Add(header);
				continue;
			}

			Object *value = paneKey->GetValue(PANE_VISIBLE_VALUE_NAME);
			paneKey->Close();
			if (!value)
			{
				visibleHeaders->Add(header);
				continue;
			}

			String *displayValue = dynamic_cast<String *>(value);
			if ((!displayValue) || (displayValue->CompareTo("0") != 0))
				visibleHeaders->Add(header);
		}

		hdPanesKey->Close();
		this->VisibleColumns = visibleHeaders;
	}
}
