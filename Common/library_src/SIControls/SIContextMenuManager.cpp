//*************************************************************************
// Copyright(C) Security Innovation, LLC, 2002-2004 – All Rights Reserved.
//
// FILE:		SIContextMenuManager.cpp
//
// DESCRIPTION: Contains implementation for the class SIContextMenuManager
//
//=========================================================================
// Modification History
//
// Date         SCR  Name		  Purpose
// -----------  ---  -----------  -----------------------
// 14 Jul 2004		 B. Shirey	  File created.
//*************************************************************************
#include <stdio.h>
#include "SIContextMenuManager.h"

namespace SIControls
{
	//*************************************************************************
	// Method:		SIContextMenuManager
	// Description: Constructor for the SIContextMenuManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIContextMenuManager::SIContextMenuManager()
	{
		contextMenuTable = new Hashtable();
	}
	
	//*************************************************************************
	// Method:		~SIContextMenuManager
	// Description: Destructor for the SIContextMenuManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SIContextMenuManager::~SIContextMenuManager()
	{
	}

	//*************************************************************************
	// Method:		get_Instance
	// Description: Gets the single instance of the class
	//
	// Parameters:
	//	None
	//
	// Return Value: the single instance of this class
	//*************************************************************************
	SIContextMenuManager *SIContextMenuManager::get_Instance()
	{
		if (!instance)
			instance = new SIContextMenuManager();
		return instance;
	}

	//*************************************************************************
	// Method:		IsContextMenuRegistered
	// Description: Gets whether or not a menu with the given name has been
	//	registered or not
	//
	// Parameters:
	//	menuName - the name of the menu to check
	//
	// Return Value: whether or not a menu with the given name has been
	//	registered or not
	//*************************************************************************
	bool SIContextMenuManager::IsContextMenuRegistered(String *menuName)
	{
		if (!menuName)
			return false;

		IEnumerator *enumerator = contextMenuTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			String *currName = dynamic_cast<String *>(enumerator->Current);
			if (menuName->Equals(currName))
				return true;
		}
		return false;
	}

	//*************************************************************************
	// Method:		RegisterContextMenu
	// Description: registers a context menu to be managed
	//
	// Parameters:
	//	menuName - the name of the menu
	//	menu - the context menu to be managed
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SIContextMenuManager::RegisterContextMenu(String *menuName, SIContextMenu *menu)
	{
		try
		{
			contextMenuTable->Add(menuName, menu);
			return true;
		}
		catch(...)
		{
		}

		return false;
	}

	//*************************************************************************
	// Method:		GetContextMenu
	// Description: Gets the context menu associated with the specified name
	//
	// Parameters:
	//	menuName - the name of the menu to get
	//
	// Return Value: the context menu associated with the specified name, or
	//	null if not found
	//*************************************************************************
	SIContextMenu *SIContextMenuManager::GetContextMenu(String *menuName)
	{
		return dynamic_cast<SIContextMenu *>(contextMenuTable->get_Item(menuName));
	}

	//*************************************************************************
	// Method:		GetRegisteredMenuNames
	// Description: Gets the list of menu names registered
	//
	// Parameters:
	//	None
	//
	// Return Value: the list of menu names registered
	//*************************************************************************
	String *SIContextMenuManager::GetRegisteredMenuNames()[]
	{
		String *retVal[] = new String *[contextMenuTable->Keys->Count];
		int i = 0;
		IEnumerator *enumerator = contextMenuTable->Keys->GetEnumerator();
		while (enumerator->MoveNext())
		{
			String *currName = dynamic_cast<String *>(enumerator->Current);
			retVal[i] = currName;
			i++;
		}
		return retVal;
	}
}