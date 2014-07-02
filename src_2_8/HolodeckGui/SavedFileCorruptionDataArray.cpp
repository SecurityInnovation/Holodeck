//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SavedFileCorruptionDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class SavedFileCorruptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 04 Nov 2003		 R. Wagner	 File created.
//*************************************************************************
#include "RegistrationMgr.h"
#include "SavedFileCorruptionDataArray.h"

namespace UIData
{
	//*************************************************************************
	// Method:		SavedFileCorruptionDataArray
	// Description: Constructor for the SavedFileCorruptionDataArray class
	//
	// Parameters:
	//	procID - the process id for the array
	//
	// Return Value: None
	//*************************************************************************
	SavedFileCorruptionDataArray::SavedFileCorruptionDataArray(int procID) :
		DisplayableDataArray()
	{
		processID = procID;
		DeleteSavedFileCorruptionHandler = new DeleteSavedFileCorruptionDelegate(this, onDeleteSavedFileCorruption);
		dataEventRegistry->OnDeleteSavedFileCorruption += DeleteSavedFileCorruptionHandler;
		CreateSavedFileCorruptionHandler = new CreateSavedFileCorruptionDelegate(this, onCreateSavedFileCorruption);
		dataEventRegistry->OnCreateSavedFileCorruption += CreateSavedFileCorruptionHandler;
	}

	//*************************************************************************
	// Method:		~SavedFileCorruptionDataArray
	// Description: Destructor for the SavedFileCorruptionDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SavedFileCorruptionDataArray::~SavedFileCorruptionDataArray()
	{
		CleanUp();
	}

	//*************************************************************************
	// Method:		CleanUp
	// Description: Cleans up all dataarray delegates
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void SavedFileCorruptionDataArray::CleanUp()
	{
		dataEventRegistry->OnDeleteSavedFileCorruption -= DeleteSavedFileCorruptionHandler;
		dataEventRegistry->OnCreateSavedFileCorruption -= CreateSavedFileCorruptionHandler;
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateSavedFileCorruption
	// Description: Called when a new saved file corruption is created
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void SavedFileCorruptionDataArray::onCreateSavedFileCorruption(DisplayableDataNode *dataNode)
	{
		Object *args[] = __gc new Object*[1];

		if (!dataNode->CreatedDelegate)
			return;
		if ( (processID == 0) && (dataNode->processID == 0) )
		{
			dataNode->DataArray = dataNode;
			CreateDataNode(dataNode);
			args[0] = dataNode;
			dataNode->CreatedDelegate->DynamicInvoke(args);
		}
		else if ( (processID != 0) && (dataNode->processID != 0) )
		{
			//Get the holoscript object for this process
			HoloScriptApplication * holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(processID);
			if (holoScriptApp)
			{		
				dataNode->DataArray = dataNode;
				CreateDataNode(dataNode);
				args[0] = dataNode;
				dataNode->CreatedDelegate->DynamicInvoke(args);

				//Set the last file corruption to the data from the project file
				SavedFileCorruptionDataNode* node = dynamic_cast <SavedFileCorruptionDataNode*> (dataNode);
				holoScriptApp->SetLastFileCorruption (node->OriginalFile, node->CorruptedFile, node->ChangesFile);
			}
		}
	}

	//*************************************************************************
	// Method:		onDeleteSavedFileCorruption
	// Description: Called when a saved file corruption is deleted
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void SavedFileCorruptionDataArray::onDeleteSavedFileCorruption(DisplayableDataNode *dataNode)
	{
		if (!dataNode->DeletedDelegate)
			return;
		else
		{
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);
			DeleteDataNode(dataNode);
		}
	}
}
