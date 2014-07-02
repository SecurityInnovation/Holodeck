//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		FileCorruptionDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class FileCorruptionDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 30 Apr 2003		 R. Wagner	 File created.
//*************************************************************************
#include "RegistrationMgr.h"
#include "FileCorruptionDataArray.h"
#include "SavedFileCorruptionDataArray.h"
#include "FileCorruptionChangesParser.h"

using namespace HolodeckRegistration;

namespace UIData
{
	//*************************************************************************
	// Method:		FileCorruptionDataArray
	// Description: Constructor for the FileCorruptionDataArray class
	//
	// Parameters:
	//	procID - the process id for the array
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDataArray::FileCorruptionDataArray(int procID)
	{
		processID = procID;
		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(procID);

		ToggleFileCorruptionHandler = new ToggleFileCorruptionDelegate(this, onToggleFileCorruption);
		dataEventRegistry->OnToggleFileCorruption += ToggleFileCorruptionHandler;
		DeleteFileCorruptionHandler = new DeleteFileCorruptionDelegate(this, onDeleteFileCorruption);
		dataEventRegistry->OnDeleteFileCorruption += DeleteFileCorruptionHandler;
		CreateFileCorruptionHandler = new CreateFileCorruptionDelegate(this, onCreateFileCorruption);
		dataEventRegistry->OnCreateFileCorruption += CreateFileCorruptionHandler;
		ModifyFileCorruptionHandler = new ModifyFileCorruptionDelegate(this, onModifyFileCorruption);
		dataEventRegistry->OnModifyFileCorruption += ModifyFileCorruptionHandler;

		logHandler = new HoloScript::LogsGeneratedDelegate(this, onLogsGenerated);
		holoScriptApp->AddOnLogGeneratedHandler(logHandler);
	}

	//*************************************************************************
	// Method:		~FileCorruptionDataArray
	// Description: Destructor for the FileCorruptionDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	FileCorruptionDataArray::~FileCorruptionDataArray()
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
	void FileCorruptionDataArray::CleanUp()
	{
		dataEventRegistry->OnToggleFileCorruption -= ToggleFileCorruptionHandler;
		dataEventRegistry->OnDeleteFileCorruption -= DeleteFileCorruptionHandler;
		dataEventRegistry->OnCreateFileCorruption -= CreateFileCorruptionHandler;
		dataEventRegistry->OnModifyFileCorruption -= ModifyFileCorruptionHandler;
		holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onCreateFileCorruption
	// Description: Called when a ui pane creates a fault
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void FileCorruptionDataArray::onCreateFileCorruption(DisplayableDataNode *dataNode)
	{
		Object *args[] = __gc new Object*[1];
		FileCorruptionDataNode *ncDataNode;
		String *testID;

		if (!dataNode->CreatedDelegate)
			return;
		else if (dataNode->processID == processID)
		{
			ncDataNode = dynamic_cast<FileCorruptionDataNode *>(dataNode);
			if (!ncDataNode)
				return;

			//Write details to registry only if test is ON
			if (ncDataNode->IsTestOn)
			{
				holoScriptApp->CreateFileCorruptionFault(ncDataNode->OriginalFile,
					ncDataNode->Regenerate, ncDataNode->SearchRegularExpression,
					ncDataNode->ReplacementString, &testID);
				
				ncDataNode->Name = String::Concat(ncDataNode->OriginalFile, " - ", ncDataNode->CorruptType);
				ncDataNode->ID = testID;
				ncDataNode->DataArray = ncDataNode;
			}
			else
			{
				ncDataNode->Name = String::Concat(ncDataNode->OriginalFile, " - ", ncDataNode->CorruptType);
				ncDataNode->ID = holoScriptApp->GetUUID();
			}

			CreateDataNode(ncDataNode);
			args[0] = ncDataNode;
			ncDataNode->CreatedDelegate->DynamicInvoke(args);

			hasCreatedOneNCFault = true;
		}
	}

	//*************************************************************************
	// Method:		onDeleteFileCorruption
	// Description: Called when a ui pane deletes a fault
	//
	// Parameters:
	//	dataNode - the node to delete
	//
	// Return Value: none
	//*************************************************************************
	void FileCorruptionDataArray::onDeleteFileCorruption(DisplayableDataNode *dataNode)
	{
		if (!dataNode->DeletedDelegate)
			return;
		else
		{
			String *testID = dataNode->ID;

			holoScriptApp->DeleteFileCorruptionFault(testID);
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			dataNode->DeletedDelegate->DynamicInvoke(args);
			DeleteDataNode(dataNode);
		}
	}

	//*************************************************************************
	// Method:		onToggleFileCorruption
	// Description: called when a test state is toggled
	//
	// Parameters:
	//	dataNode - node being modified
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDataArray::onToggleFileCorruption(DisplayableDataNode *dataNode)
	{
		FileCorruptionDataNode *fcDataNode = dynamic_cast<FileCorruptionDataNode *>(dataNode);
		if (!fcDataNode)
			return;

		String *testID = dataNode->ID;

		if (!fcDataNode->IsTestOn)
		{
			holoScriptApp->DeleteFileCorruptionFault(testID);
		}
		else
		{
			holoScriptApp->CreateFileCorruptionFault(fcDataNode->OriginalFile,
				fcDataNode->Regenerate, fcDataNode->SearchRegularExpression,
				fcDataNode->ReplacementString, &testID);
			
			fcDataNode->Name = String::Concat(fcDataNode->OriginalFile, " - ", fcDataNode->CorruptType);
			fcDataNode->ID = testID;
		}
	}

	//*************************************************************************
	// Method:		onModifyFileCorruption
	// Description: Called when a ui pane modifies a fault
	//
	// Parameters:
	//	dataNode - the node to create
	//
	// Return Value: none
	//*************************************************************************
	void FileCorruptionDataArray::onModifyFileCorruption(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		Object *args[] = __gc new Object*[1];
		FileCorruptionDataNode *ncDataNode;
		String *testID;

		FileCorruptionDataNode *originalNode = dynamic_cast<FileCorruptionDataNode *>
			(GetDataNodeByID(dataNode->ID));

		if ((!dataNode->ModifiedDelegate) || (!originalNode))
			return;
		else if (dataNode->processID == processID)
		{
			ncDataNode = dynamic_cast<FileCorruptionDataNode *>(modifiedDataNode);
			if (!ncDataNode)
				return;

			holoScriptApp->DeleteFileCorruptionFault(dataNode->ID);
			this->DeleteDataNode(dataNode);
			if (dataNode->DeletedDelegate)
			{
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeletedDelegate->DynamicInvoke(args);
			}

			holoScriptApp->CreateFileCorruptionFault(ncDataNode->OriginalFile,
				ncDataNode->Regenerate, ncDataNode->SearchRegularExpression,
				ncDataNode->ReplacementString, &testID);
			
			ncDataNode->Name = String::Concat(ncDataNode->OriginalFile, " - ", ncDataNode->CorruptType);
			ncDataNode->ID = testID;

			CreateDataNode(ncDataNode);
			args[0] = ncDataNode;
			ncDataNode->CreatedDelegate->DynamicInvoke(args);
		}
	}

	//*************************************************************************
	// Method:		RegenerateCorruption
	// Description: Forces regeneration of a file corruption at the next
	//		access to the file
	//
	// Parameters:
	//	dataNode - the node to regenerate
	//
	// Return Value: none
	//*************************************************************************
	void FileCorruptionDataArray::RegenerateCorruption(DisplayableDataNode *dataNode)
	{
		FileCorruptionDataNode *fcDataNode = dynamic_cast<FileCorruptionDataNode *>
			(GetDataNodeByID(dataNode->ID));
		if (fcDataNode != NULL)
			holoScriptApp->RegenerateFileCorruptionFault(fcDataNode->OriginalFile);
	}

	//*************************************************************************
	// Method:		OnLogsGenerated
	// Description: Called when logs are generated for the process
	//
	// Parameters:
	//	numNewLogs - the number of new logs generated
	//
	// Return Value: None
	//*************************************************************************
	void FileCorruptionDataArray::onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[], unsigned int procID,
		unsigned int threadID)
	{
		for (int count = 0; count < numNewLogs; count++)
		{
			// Get the log entry from Holoscript
			LogItem *item = newLogItems[count];
			if (!item)
				continue;

			// Check for a log events
			for (int j = 0; j < item->Events->Count; j++)
			{
				LogEvent* event = dynamic_cast<LogEvent*>(item->Events->Item[j]);
				if (!event)
					continue;

				if (event->Type == Holodeck::LogEventType::FileCorruptionEvent)
				{
					// Log entry has an attached file corruption event.  Add the corrupted file
					// to this pane if it hasn't already been added
					String* xmlChangesFile = event->StringValue;
					SavedFileCorruptionDataNode* node = dynamic_cast<SavedFileCorruptionDataNode*>(GetDataNodeByID(xmlChangesFile));
					if (node == NULL)
					{
						SavedFileCorruptionDataNode* node = new SavedFileCorruptionDataNode(xmlChangesFile, NodeType::CorruptedFile, processID);
						node->ChangesFile = xmlChangesFile;

						FileCorruptionChangesParser* parser = new FileCorruptionChangesParser();
						if (parser->Load(xmlChangesFile))
						{
							node->OriginalFile = parser->OriginalFile;
							node->CorruptedFile = parser->CorruptedFile;
							node->Element = xmlChangesFile;
							node->ID = String::Concat(node->ChangesFile, this->ProcessID.ToString(), this->ThreadID.ToString());

							Object *args[] = __gc new Object*[1];
							args[0] = node;
							node->CreateDelegate->DynamicInvoke(args);
						}
					}
				}
			}
		}
	}
}
