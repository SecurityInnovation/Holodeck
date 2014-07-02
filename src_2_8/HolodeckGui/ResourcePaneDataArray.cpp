//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ResourcePaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ResourcePaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 27 Mar 2003		 J. Taylor	 File created.
//*************************************************************************
#include <Windows.h>
#include <vcclr.h>
#include "ResourcePaneDataArray.h"
#include "ResourceTestDataNode.h"
#include "defines.h"
#include "UserNotification.h"

namespace UIData
{

	//*************************************************************************
	// Method:		ResourcePaneDataArray
	// Description: Constructor for the ResourcePaneDataArray class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	ResourcePaneDataArray::ResourcePaneDataArray(int pID)
	{
		processID = pID;
		totalEntries = 0;
		registryResourceMappings = new Hashtable();
		registryHandleMappings = new Hashtable();
		fileResourceMappings = new Hashtable();
		fileHandleMappings = new Hashtable();
		processResourceMappings = new Hashtable();
		processHandleMappings = new Hashtable();
		unmatchedResourceTests = new SortedList();
		String *name;
		ResourcePaneDataNode *resourceDataNode;

		fileResources = 0;
		registryResources = 0;
		processResources = 0;
		comResources = 0;

		//add the predefined roots.
		name = "HKEY_CLASSES_ROOT";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_CLASSES_ROOT, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_CURRENT_USER";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_CURRENT_USER, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_LOCAL_MACHINE";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_LOCAL_MACHINE, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_USERS";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_USERS, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_PERFORMANCE_DATA";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_PERFORMANCE_DATA, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_PERFORMANCE_TEXT";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_PERFORMANCE_TEXT, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_PERFORMANCE_NLSTEXT";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_PERFORMANCE_NLSTEXT, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_CURRENT_CONFIG";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_CURRENT_CONFIG, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		name = "HKEY_DYN_DATA";
		AddToResourceMappings(UIData::RegistryEntry, Convert::ToString((int) HKEY_DYN_DATA, 16), name);
		resourceDataNode = new ResourcePaneDataNode(name, UIData::RegistryEntry, pID, 0);
		resourceDataNode->ID = name->ToLower();
		resourceDataNode->Displayable = false;
		CreateDataNode(resourceDataNode);

		holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess(processID);	
		logHandler = new HoloScript::LogsGeneratedDelegate(this, onLogsGenerated);
		holoScriptApp->AddOnLogGeneratedHandler(logHandler);

		resourceTestCreatedHandler = new ResourceTestCreatedDelegate(this, onResourceTestCreated);
		dataEventRegistry->OnResourceTestCreated += resourceTestCreatedHandler;
		resourceTestDeletedHandler = new ResourceTestDeletedDelegate(this, onResourceTestDeleted);
		dataEventRegistry->OnResourceTestDeleted += resourceTestDeletedHandler;

		// initialize the function database
		String *functionFolder = GuiSupport::RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath();
		interceptedFunctionDB = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		if (!interceptedFunctionDB->IsLoaded)
			interceptedFunctionDB->LoadDatabase(functionFolder, FUNCTION_DB_FILE_NAME);
		dotNetInterceptedFunctionDB = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
		if (!dotNetInterceptedFunctionDB->IsLoaded)
			dotNetInterceptedFunctionDB->LoadDatabase(functionFolder, DOT_NET_FUNC_DB_FILE_NAME);
		
		errorCodeDB = ErrorCodeDB::GetInstance();
		if (!errorCodeDB->IsLoaded)
			errorCodeDB->LoadDatabase(functionFolder);

	}

	//*************************************************************************
	// Method:		ResourcePaneDataArray
	// Description: Destructor for the ResourcePaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ResourcePaneDataArray::~ResourcePaneDataArray()
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
	void ResourcePaneDataArray::CleanUp()
	{
		dataEventRegistry->OnResourceTestCreated -= resourceTestCreatedHandler;
		dataEventRegistry->OnResourceTestDeleted -= resourceTestDeletedHandler;
		holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		onLogsGenerated
	// Description: Called when logs are generated for the process
	//
	// Parameters:
	//	numNewLogs - the number of new logs generated
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePaneDataArray::onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[],
		unsigned int processID, unsigned int threadID)
	{
		UIData::NodeType type;
		ArrayList *names = new ArrayList;

		if (numNewLogs > 0)
		{
			for (int i = 0; i < numNewLogs; i++)
			{
				LogItem *item = newLogItems[i];
				totalEntries++;
				if (item)
				{
					names->Clear();

					if (item->Category->CompareTo("REGISTRY") == 0)
					{
						type = UIData::RegistryEntry;
						names = GetResourcePathStrings(type, item);
					}
					else if (item->Category->CompareTo("FILE") == 0) 
					{
						type = UIData::File;
						names = GetResourcePathStrings(type, item);
					}
					else if ( (item->Category->CompareTo("PROCESS") == 0) || (item->Category->CompareTo("LIBRARY") == 0) )
					{
						type = UIData::Process;
						names = GetResourcePathStrings(type, item);
					}
					else if (item->Category->CompareTo("COM") == 0) 
					{
						type = UIData::ComObject;
						names = GetResourcePathStrings(type, item);
					}

					if (names->Count > 0)
					{
						for (int i=0; i < names->Count; i++)
						{
							String *name = dynamic_cast <String*> (names->Item[i]);

							if (name->Equals("NULL"))
								continue;

							threadID = Convert::ToUInt32(item->ThreadID);
							ResourcePaneDataNode *resourceDataNode = dynamic_cast <ResourcePaneDataNode*> (GetDataNodeByID(name->ToLower()));
							
							if (!resourceDataNode)
							{
								resourceDataNode = new ResourcePaneDataNode(name, type, processID, threadID);
								//unique cross-thread ID
								resourceDataNode->ID = name->ToLower();

								//update resource count
								switch (type)
								{
								case RegistryEntry:
									registryResources++;
									break;
								case File:
									fileResources++;
									break;
								case Process:
									processResources++;
									break;
								case ComObject:
									comResources++;
									break;
								}
							}
							resourceDataNode->TimeStamp = item->TimeStamp;
							resourceDataNode->LastFunction = item->Function;
							resourceDataNode->ErrorCode = errorCodeDB->GetStringFromCode(item->ErrorCode);
							resourceDataNode->ReturnValue = item->ReturnValue;
							resourceDataNode->FilePosition = item->FilePosition;
							resourceDataNode->NumHits++;
							resourceDataNode->DataArray = resourceDataNode;

							if (dataEventRegistry->OnResourceCreated)
								dataEventRegistry->OnResourceCreated->Invoke(resourceDataNode);

							//check for unmatched resource tests
							ResourceTestDataNode *resourceTestDataNode = dynamic_cast <ResourceTestDataNode*> (unmatchedResourceTests->Item[resourceDataNode->Name]);
							if (resourceTestDataNode)
							{
								resourceDataNode->ResourceTestNode = resourceTestDataNode;
								unmatchedResourceTests->Remove(resourceDataNode->Name);
							}

							CreateDataNode(resourceDataNode);
						}
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		onResourceTestCreated
	// Description: called when a new resource test is created
	//
	// Parameters:
	//	dataNode - node representing created test
	//
	// Return Value: none
	//*************************************************************************
	void ResourcePaneDataArray::onResourceTestCreated(DisplayableDataNode *dataNode)
	{
		if (dataNode->processID != this->ProcessID)
			return;

		ResourcePaneDataNode *resourcePaneDataNode = dynamic_cast <ResourcePaneDataNode*> (GetDataNodeByID(dataNode->Name->ToLower()));

		if (resourcePaneDataNode)
			resourcePaneDataNode->ResourceTestNode = dataNode;
		else
		{
			//send modified event anyway as not all tree nodes have a datanode associated
			ResourcePaneDataNode *resourceDataNode = new ResourcePaneDataNode(dataNode->Name, dataNode->Type, dataNode->processID, dataNode->threadID);
			resourceDataNode->ResourceTestNode = dataNode;

			if (this->onItemModified)
				this->onItemModified->Invoke(resourceDataNode, "ResourceTest");

			if (!unmatchedResourceTests->Contains(dataNode->Name))
				unmatchedResourceTests->Add(dataNode->Name, dataNode);
		}
	}

	//*************************************************************************
	// Method:		onResourceTestDeleted
	// Description: called when a new resource test is deleted
	//
	// Parameters:
	//	dataNode - node representing created test
	//
	// Return Value: none
	//*************************************************************************
	void ResourcePaneDataArray::onResourceTestDeleted(DisplayableDataNode *dataNode)
	{
		if (!dataNode)
			return;

		ResourcePaneDataNode *resourcePaneDataNode = dynamic_cast <ResourcePaneDataNode*> (GetDataNodeByID(dataNode->Name->ToLower()));

		if (resourcePaneDataNode)
			resourcePaneDataNode->ResourceTestNode = NULL;
		else
		{
			//send modified event anyway as not all tree nodes have a datanode associated
			ResourcePaneDataNode *resourceDataNode = new ResourcePaneDataNode(dataNode->Name, dataNode->Type, dataNode->processID, dataNode->threadID);
			if (this->onItemModified)
				this->onItemModified->Invoke(resourceDataNode, "ResourceTest");
		}
	}

	//*************************************************************************
	// Method:		get_ResourcePaths
	// Description: gets a list containing all the resource paths in this array
	//
	// Parameters:
	//	None
	//
	// Return Value: list of resources
	//*************************************************************************
	SortedList *ResourcePaneDataArray::get_ResourcePaths()
	{
		SortedList *resourcePaths = new SortedList();
		SortedList *registryPaths = new SortedList();
		SortedList *filePaths = new SortedList();
		SortedList *processPaths = new SortedList();
		SortedList *comPaths = new SortedList();

		resourcePaths->Add(__box(UIData::RegistryEntry), registryPaths);
		resourcePaths->Add(__box(UIData::File), filePaths);
		resourcePaths->Add(__box(UIData::Process), processPaths);
		resourcePaths->Add(__box(UIData::ComObject), comPaths);

		for (int i=0; i<itemList->Count; i++)
		{
			ResourcePaneDataNode *dataNode = dynamic_cast <ResourcePaneDataNode*> (itemList->GetByIndex(i));
			
			if ( (dataNode->Type == UIData::RegistryEntry) && (!registryPaths->Contains(dataNode->Name)) )
				registryPaths->Add(dataNode->Name, dataNode->Name);
			else if ( (dataNode->Type == UIData::File) && (!filePaths->Contains(dataNode->Name)) )
				filePaths->Add(dataNode->Name, dataNode->Name);
			else if ( (dataNode->Type == UIData::Process) && (!processPaths->Contains(dataNode->Name)) )
				processPaths->Add(dataNode->Name, dataNode->Name);
			else if ( (dataNode->Type == UIData::ComObject) && (!comPaths->Contains(dataNode->Name)) )
				comPaths->Add(dataNode->Name, dataNode->Name);
		}

		return resourcePaths;
	}

	//*************************************************************************
	// Method:		GetResourcePathStrings
	// Description: returns path strings if the log item corresponds to an access to
	//	a resource
	//
	// Parameters:
	//	type - type of node
	//	item - log item from which to grab path information
	//
	// Return Value: resource paths if found or empty array otherwise
	//*************************************************************************
	ArrayList *ResourcePaneDataArray::GetResourcePathStrings(NodeType type, LogItem *item)
	{
		ArrayList *resourcePaths = new ArrayList();
		StringBuilder *resourcePath, *temp;
		String *currentParamValue;
		InterceptedFunctionDB *functionDB = functionDB->GetInstance(FUNCTION_DB_FILE_NAME);
		InterceptedFunction *function = functionDB->GetFunctionByName(item->Function);

		if (function == NULL)
		{
			// Try the .NET database
			functionDB = functionDB->GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			function = functionDB->GetFunctionByName(item->Function);

			if (function == NULL)
			{
				// Failed to find function, return an empty array
				return resourcePaths;
			}
		}

		InterceptedFunctionParameter *parameter, *concatParameter;
		Array *parameterArray = function->Parameters;

		for (int i=0; i<parameterArray->Count; i++)
		{
			parameter = dynamic_cast <InterceptedFunctionParameter*> (parameterArray->Item[i]);
			currentParamValue = GetMappedValue(type, (ParameterType) parameter->CompareAsType, dynamic_cast <String*> (item->Parameters->Item[i]));
			if (currentParamValue == NULL)
				continue;
			
			resourcePath = NULL;
			if (parameter->PointsToResource)
			{
				resourcePath = new StringBuilder(currentParamValue);
				concatParameter = parameter;
				while (concatParameter->ConcatWith != -1)
				{
					InterceptedFunctionParameter *nextConcatParameter = dynamic_cast <InterceptedFunctionParameter*> (parameterArray->Item[concatParameter->ConcatWith]);
					ParameterType concatParameterType = (ParameterType) nextConcatParameter->CompareAsType;
					temp = new StringBuilder(resourcePath->ToString());
					resourcePath = new StringBuilder(GetMappedValue(type, concatParameterType, dynamic_cast <String*> (item->Parameters->Item[concatParameter->ConcatWith])));
					if (resourcePath->Length > 0)
						resourcePath->Append("\\");

					resourcePath->Append(temp);
					concatParameter = nextConcatParameter;
				}
				resourcePaths->Add(resourcePath->ToString());
			}

			if (parameter->MapTo != NULL)
			{
				//could be Return, HKCU, HKCR, or another param index
				if (parameter->MapTo->CompareTo("Return") == 0)
				{
					if (parameter->CompareAsType != PointerType)
						AddToResourceMappings(type, item->ReturnValue, resourcePath->ToString());
					else
						AddToHandleMappings(type, item->ReturnValue, currentParamValue);
				}
				else if (parameter->MapTo->CompareTo("HKCU") == 0)
				{
					AddToHandleMappings(type, currentParamValue, Convert::ToString((int) HKEY_CURRENT_USER, 16));
				}
				else if (parameter->MapTo->CompareTo("HKCR") == 0)
				{
					AddToHandleMappings(type, currentParamValue, Convert::ToString((int) HKEY_CLASSES_ROOT, 16));
				}
				else
				{
					//map from one param to another
					try
					{
						int index = Convert::ToInt32(parameter->MapTo);
						//add to handle mappings only if the handle didn't point to a resource
						if ( (parameter->CompareAsType != PointerType) || (resourcePath && (!currentParamValue->Equals(resourcePath->ToString()))) )
							AddToResourceMappings(type, dynamic_cast <String*> (item->Parameters->Item[index]), resourcePath->ToString());
						else
							AddToHandleMappings(type, dynamic_cast <String*> (item->Parameters->Item[index]), currentParamValue);

					}
					catch(...)
					{
						//catch and move on
					}
				}
			}
		}

		return resourcePaths;
	}

	//*************************************************************************
	// Method:		GetMappedValue
	// Description: Given a parameter value returns a mapped string
	//
	// Parameters:
	//	type - type of node
	//	value - value to look up in the mapping tables
	//
	// Return Value: returns mapped value, or if nothing found returns value passed into
	//	this function
	//*************************************************************************
	String *ResourcePaneDataArray::GetMappedValue(NodeType nodeType, ParameterType parameterType, String *value)
	{
		String *handleValue;
		String *resourceValue;
		Hashtable *resourceTable, *handleTable;

		if (!value)
			return value;

		switch (nodeType)
		{
		case RegistryEntry:
			resourceTable = registryResourceMappings;
			handleTable = registryHandleMappings;
			break;
		case File:
			resourceTable = fileResourceMappings;
			handleTable = fileHandleMappings;
			//check for pipenames
			if ( (value->IndexOf("\\\\.\\") != -1) || (value->IndexOf("\\\\?\\") != -1) || (value->Length < 3) )
				return NULL;
			break;
		case Process:
			resourceTable = processResourceMappings;
			handleTable = processHandleMappings;
			break;
		case ComObject:
			return value;
		default:
			return NULL;
		}

		handleValue = value;

		while (handleTable->Contains(handleValue))
			handleValue = dynamic_cast <String*> (handleTable->Item[handleValue]);

		resourceValue = dynamic_cast <String*> (resourceTable->Item[handleValue]);

		if (resourceValue == NULL)
			return value;
		else
			return resourceValue;
	}

	//*************************************************************************
	// Method:		AddToResourceMappings
	// Description: adds a mapping to the resource table
	//
	// Parameters:
	//	handle - handle that the resource will be mapped to
	//	resourcePath - path to map to the handle
	//
	// Return Value: none
	//*************************************************************************
	void ResourcePaneDataArray::AddToResourceMappings(NodeType type, String *handle, String *resourcePath)
	{
		Hashtable *resourceTable;

		//ignore null handles
		if (handle->CompareTo("0") == 0)
			return;

		switch (type)
		{
		case RegistryEntry:
			resourceTable = registryResourceMappings;
			break;
		case File:
			resourceTable = fileResourceMappings;
			break;
		case Process:
			resourceTable = processResourceMappings;
		default:
			return;
		}

		if (resourceTable->Contains(handle))
			resourceTable->Remove(handle);

		resourceTable->Add(handle, resourcePath);
	}

	//*************************************************************************
	// Method:		AddToHandleMappings
	// Description: adds a mapping to the handle table
	//
	// Parameters:
	//	mapFromHandle - handle to map from
	//	mapToHandle - handle to map to
	//
	// Return Value: none
	//*************************************************************************
	void ResourcePaneDataArray::AddToHandleMappings(NodeType type, String *mapFromHandle, String *mapToHandle)
	{
		Hashtable *handleTable;

		//ignore null handles
		if ( (mapFromHandle->CompareTo("0") == 0) || (mapToHandle->CompareTo("0") == 0) )
			return;

		switch (type)
		{
		case RegistryEntry:
			handleTable = registryHandleMappings;
			break;
		case File:
			handleTable = fileHandleMappings;
			break;
		case Process:
			handleTable = processHandleMappings;
		default:
			return;
		}

		if (handleTable->Contains(mapFromHandle))
			handleTable->Remove(mapFromHandle);

		handleTable->Add(mapFromHandle, mapToHandle);
	}

	//*************************************************************************
	// Method:		ExportToFile
	// Description: Exports the resources to the specified filename
	//
	// Parameters:
	//	exportFileName - The filename to export the resources to
	//
	// Return Value: None
	//*************************************************************************
	void ResourcePaneDataArray::ExportToFile(String * exportFileName)
	{
		StreamWriter *sw = NULL;

		try
		{
			sw = new StreamWriter(exportFileName, false);
			if (!sw)
				return;

			//write column headers
			sw->Write("TimeStamp");
			sw->Write(",");
			sw->Write("Type");
			sw->Write(",");
			sw->Write("Resource");
			sw->Write(",");
			sw->Write("LastFunction");
			sw->Write(",");
			sw->Write("ErrorCode");
			sw->Write(",");
			sw->Write("ReturnValue");
			sw->Write(",");
			sw->Write("NumHits");
			sw->Write("\r\n");


			for (int i = 0; i < Count; i++)
			{
				ResourcePaneDataNode *node = dynamic_cast<ResourcePaneDataNode *>(GetDataNodeByIndex(i));
				if (!node || !node->TimeStamp)
					continue;

				String *typeString;
				switch(node->Type)
				{
				case UIData::File:
					typeString = "FILE";
					break;
				case UIData::RegistryEntry:
					typeString = "REGISTRY";
					break;
				case UIData::Process:
					typeString = "PROCESS";
					break;
				case UIData::ComObject:
					typeString = "COM";
					break;
				default:
					// don't know how this would happen, better skip it
					continue;
				}

				sw->Write("\"");
				sw->Write(node->TimeStamp);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(typeString);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(node->Name);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(node->LastFunction);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(node->ErrorCode);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(node->ReturnValue);
				sw->Write("\"");
				sw->Write(",");
				sw->Write("\"");
				sw->Write(node->NumHits.ToString());
				sw->Write("\"");
				sw->Write("\r\n");
			}
			sw->Close();
		}
		catch (Exception *)
		{
			if (sw)
				sw->Close();

			HolodeckGui::UserNotification::ErrorNotify("There was a problem exporting the resources to the specified location. Please verify that you have adequate security permissions to write to the selected location.");
			return;
		}
	}
}