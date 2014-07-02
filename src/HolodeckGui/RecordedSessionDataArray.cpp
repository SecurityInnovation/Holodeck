//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		RecordedSessionDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ResourcePaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 15 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include <windows.h>
#include "RecordedSessionDataArray.h"
#include "UserNotification.h"

#ifdef CreateDirectory
#undef CreateDirectory
#endif

namespace UIData
{

	//*************************************************************************
	// Method:		RecordedSessionDataArray
	// Description: Constructor for the RecordedSessionDataArray class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	RecordedSessionDataArray::RecordedSessionDataArray(HoloScriptRegistry *HoloScriptRegistry)
	{
		holoScriptRegistry = HoloScriptRegistry;
		injectTable = new Hashtable();
		recording = false;
		replaying = false;

		uiEventRegistry = uiEventRegistry->getInstance();

		testCreatedHandler = new TestCreatedDelegate(this, onCreated);
		resourceTestCreatedHandler = new ResourceTestCreatedDelegate(this, onCreated);
		limitCreatedHandler = new LimitCreatedDelegate(this, onCreated);
		faultCreatedHandler = new FaultCreatedDelegate(this, onCreated);
		ncCreatedHandler = new NetworkCorruptionCreatedDelegate(this, onCreated);
		fcCreatedHandler = new FileCorruptionCreatedDelegate(this, onCreated);
		testDeletedHandler = new TestDeletedDelegate(this, onDeleted);
		resourceTestDeletedHandler = new ResourceTestDeletedDelegate(this, onDeleted);
		limitDeletedHandler = new LimitDeletedDelegate(this, onDeleted);
		faultDeletedHandler = new FaultDeletedDelegate(this, onDeleted);
		ncDeletedHandler = new NetworkCorruptionDeletedDelegate(this, onDeleted);
		fcDeletedHandler = new FileCorruptionDeletedDelegate(this, onDeleted);
		limitModifiedHandler = new LimitModifiedDelegate(this, onModified);
		testModifiedHandler = new TestModifiedDelegate(this, onModified);
		resourceTestModifiedHandler = new ResourceTestModifiedDelegate(this, onModified);
		ncModifiedHandler = new NetworkCorruptionModifiedDelegate(this, onModified);
		fcModifiedHandler = new FileCorruptionModifiedDelegate(this, onModified);
		projectRestartedHandler = new ProjectRestartedDelegate(this, onProjectRestarted);
		processTerminatedHandler = new ProcessTerminatedDelegate(this, onProcessTerminated);

	}

	//*************************************************************************
	// Method:		RecordedSessionDataArray
	// Description: Destructor for the RecordedSessionDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	RecordedSessionDataArray::~RecordedSessionDataArray()
	{
		StopRecording();
		uiEventRegistry->OnProcessTerminated -= processTerminatedHandler;
	}

	//*************************************************************************
	// Method:		onCreated
	// Description: called when a new test, fault, or limit is created
	//
	// Parameters:
	//	dataNode - node that represents the created object
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::onCreated(DisplayableDataNode *dataNode)
	{
		if (recording)
		{
			unsigned long filePosition;
			LogItem *logItem = NULL;
			HoloScriptApplication *holoScript = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			unsigned int logIndex;

			if (!holoScript)
				return;

			logIndex = holoScript->LogEntryCount;

			if (logIndex > 0)
				logItem = holoScript->GetLogEntryAt(--logIndex, filePosition);
			else
				logIndex = 0;

			RecordedSessionDataNode *recordedDataNode = new RecordedSessionDataNode(dataNode, logItem, logIndex);
			recordedDataNode->AppName = holoScript->ApplicationName;
			recordedDataNode->SessionCount = sessionCount;

			recordedDataNode->NodeEnabled = enabled;		
			this->CreateDataNode(recordedDataNode);
		}
	}

	//*************************************************************************
	// Method:		onDeleted
	// Description: called when a new test, fault, or limit is deleted
	//
	// Parameters:
	//	dataNode - node that represents the deleted object
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::onDeleted(DisplayableDataNode *dataNode)
	{
		if (recording)
		{
			unsigned long filePosition;
			LogItem *logItem = NULL;
			HoloScriptApplication *holoScript = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			unsigned int logIndex;

			if (!holoScript)
				return;

			logIndex = holoScript->LogEntryCount;

			if (logIndex > 0)
				logItem = holoScript->GetLogEntryAt(--logIndex, filePosition);
			else
				logIndex = 0;

			RecordedSessionDataNode *recordedDataNode = new RecordedSessionDataNode(dataNode, logItem, logIndex);
			recordedDataNode->AppName = holoScript->ApplicationName;
			recordedDataNode->SessionCount = sessionCount;

			recordedDataNode->NodeEnabled = disabled;
			this->CreateDataNode(recordedDataNode);

		}
	}

	//*************************************************************************
	// Method:		onModified
	// Description: called when a test or limit is modified
	//
	// Parameters:
	//	dataNode - node that represents the original data
	//	modifiedDataNode - node that represents the modified data
	//	propertyName - name of the property that changed
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::onModified(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		if (recording)
		{
			unsigned long filePosition;
			LogItem *logItem = NULL;
			HoloScriptApplication *holoScript = holoScriptRegistry->getHoloScriptAppForProcess(dataNode->processID);
			unsigned int logIndex;

			if (!holoScript)
				return;

			logIndex = holoScript->LogEntryCount;

			if (logIndex > 0)
				logItem = holoScript->GetLogEntryAt(--logIndex, filePosition);
			else
				logIndex = 0;

			RecordedSessionDataNode *recordedDataNode = new RecordedSessionDataNode(dataNode, logItem, logIndex);
			recordedDataNode->ModifiedNode = modifiedDataNode;
			recordedDataNode->AppName = holoScript->ApplicationName;
			recordedDataNode->SessionCount = sessionCount;

			recordedDataNode->NodeEnabled = modified;
			recordedDataNode->PropertyName = propertyName;
			this->CreateDataNode(recordedDataNode);

		}
	}

	//*************************************************************************
	// Method:		onProcessTerminated
	// Description: called when a process is terminated.
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::onProcessTerminated(unsigned int processID)
	{
		if (recording)
		{
			unsigned long filePosition;
			LogItem *logItem = NULL;
			HoloScriptApplication *holoScript = holoScriptRegistry->getHoloScriptAppForProcess(processID);
			unsigned int logIndex;

			if (!holoScript)
			{
				holoScript = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(processID);
				if (!holoScript)
					return;
			}

			logIndex = holoScript->LogEntryCount;

			if (logIndex > 0)
				logItem = holoScript->GetLogEntryAt(--logIndex, filePosition);
			else
				logIndex = 0;

			DisplayableDataNode *dataNode = new DisplayableDataNode("Restart AUT", Process, processID, 0);
			RecordedSessionDataNode *recordedDataNode = new RecordedSessionDataNode(dataNode, logItem, logIndex);
			recordedDataNode->AppName = holoScript->ApplicationName;
			recordedDataNode->SessionCount = sessionCount;

			recordedDataNode->NodeEnabled = restartAUT;		
			this->CreateDataNode(recordedDataNode);
		}

		if (!holoScriptRegistry->AnyAppsRunning)
		{
			StopRecording();
			StopReplay();
		}
	}

	//*************************************************************************
	// Method:		StartRecording
	// Description: start recording
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::StartRecording(String *RecordedSessionFolder, String *RecordedSessionName)
	{		
		if (!recording)
		{
			recordedSessionFolder = RecordedSessionFolder;
			this->Name = RecordedSessionName;

			if (replaying)
				StopReplay();

			dataEventRegistry->OnTestCreated += testCreatedHandler;
			dataEventRegistry->OnResourceTestCreated += resourceTestCreatedHandler;
			dataEventRegistry->OnLimitCreated += limitCreatedHandler;
			dataEventRegistry->OnFaultCreated += faultCreatedHandler;
			dataEventRegistry->OnNetworkCorruptionCreated += ncCreatedHandler;
			dataEventRegistry->OnFileCorruptionCreated += fcCreatedHandler;

			dataEventRegistry->OnTestDeleted += testDeletedHandler;
			dataEventRegistry->OnResourceTestDeleted += resourceTestDeletedHandler;
			dataEventRegistry->OnLimitDeleted += limitDeletedHandler;
			dataEventRegistry->OnFaultDeleted += faultDeletedHandler;
			dataEventRegistry->OnNetworkCorruptionDeleted += ncDeletedHandler;
			dataEventRegistry->OnFileCorruptionDeleted += fcDeletedHandler;

			dataEventRegistry->OnLimitModified += limitModifiedHandler;
			dataEventRegistry->OnTestModified += testModifiedHandler;
			dataEventRegistry->OnResourceTestModified += resourceTestModifiedHandler;
			dataEventRegistry->OnNetworkCorruptionModified += ncModifiedHandler;
			dataEventRegistry->OnFileCorruptionModified += fcModifiedHandler;

			dataEventRegistry->OnProjectRestarted += projectRestartedHandler;
			uiEventRegistry->OnProcessTerminated += processTerminatedHandler;

			sessionCount = 1;
			recording = true;
			DisplayableDataNode *dataNode = new DisplayableDataNode(this->Name, UIData::RecordedSession, 0, 0);
			uiEventRegistry->OnRecordedSessionStarted->Invoke(dataNode);
		}
	}

	//*************************************************************************
	// Method:		StopRecording
	// Description: stop recording
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::StopRecording()
	{		
		if (recording)
		{
			dataEventRegistry->OnTestCreated -= testCreatedHandler;
			dataEventRegistry->OnResourceTestCreated -= resourceTestCreatedHandler;
			dataEventRegistry->OnLimitCreated -= limitCreatedHandler;
			dataEventRegistry->OnFaultCreated -= faultCreatedHandler;
			dataEventRegistry->OnNetworkCorruptionCreated -= ncCreatedHandler;
			dataEventRegistry->OnFileCorruptionCreated -= fcCreatedHandler;

			dataEventRegistry->OnTestDeleted -= testDeletedHandler;
			dataEventRegistry->OnResourceTestDeleted -= resourceTestDeletedHandler;
			dataEventRegistry->OnLimitDeleted -= limitDeletedHandler;
			dataEventRegistry->OnFaultDeleted -= faultDeletedHandler;
			dataEventRegistry->OnNetworkCorruptionDeleted -= ncDeletedHandler;
			dataEventRegistry->OnFileCorruptionDeleted -= fcDeletedHandler;

			dataEventRegistry->OnLimitModified -= limitModifiedHandler;
			dataEventRegistry->OnTestModified -= testModifiedHandler;
			dataEventRegistry->OnResourceTestModified -= resourceTestModifiedHandler;
			dataEventRegistry->OnNetworkCorruptionModified -= ncModifiedHandler;
			dataEventRegistry->OnFileCorruptionModified -= fcModifiedHandler;

			dataEventRegistry->OnProjectRestarted -= projectRestartedHandler;
			uiEventRegistry->OnProcessTerminated -= processTerminatedHandler;

			recording = false;

			DisplayableDataNode *dataNode = new DisplayableDataNode(this->Name, UIData::RecordedSession, 0, 0);
			if (RecordToFile())
			{
				uiEventRegistry->OnRecordedSessionStopped->Invoke(dataNode);
			}
			else
			{
				Object *args[] = __gc new Object*[1];
				args[0] = dataNode;
				dataNode->DeleteDelegate->DynamicInvoke(args);
			}

		}
	}

	//*************************************************************************
	// Method:		StartReplay
	// Description: start replay
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::StartReplay()
	{
		if (!replaying)
		{
			dataEventRegistry->OnProjectRestarted += projectRestartedHandler;

			if (recording)
				StopRecording();

			replaying = true;
			sessionCount = 0;
			dataEventRegistry->OnRestartProject(false);
		}
	}

	//*************************************************************************
	// Method:		StopReplay
	// Description: stop replay
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::StopReplay()
	{		
		if (replaying)
		{
			dataEventRegistry->OnProjectRestarted -= projectRestartedHandler;

			for (int i=0; i<holoScriptRegistry->Count; i++)
			{
				HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppByIndex(i);	
				HoloScript::LogsGeneratedDelegate *logHandler = new HoloScript::LogsGeneratedDelegate(this, onLogsGenerated);
				holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
			}

			replaying = false;
			ClearInjectTable();
			DisplayableDataNode *dataNode = new DisplayableDataNode(this->Name, UIData::RecordedSession, 0, 0);
			uiEventRegistry->OnRecordedSessionReplayStopped->Invoke(dataNode);
		}
	}

	//*************************************************************************
	// Method:		BuildInjectTable
	// Description: fill table in with next inject command for each process
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::BuildInjectTable()
	{		
		RecordedSessionDataNode *recordedDataNode;
		String *appName;

		for (int i=0; i<holoScriptRegistry->Count; i++)
		{
			HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppByIndex(i);	
			appName = holoScriptApp->ApplicationName;
			if (!injectTable->Contains(appName))
			{
				for (int j=0; j<this->Count; j++)
				{
					recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (this->GetDataNodeByIndex(j));
					//must match appname, session count, and not have been replayed yet.
					if ( (recordedDataNode->AppName->CompareTo(appName) == 0) && (!recordedDataNode->NodeReplayed) 
						&& (recordedDataNode->SessionCount == sessionCount) )
					{
						recordedDataNode->NodeReplayed = true;
						injectTable->Add(appName, recordedDataNode);
						break;
					}
				}
			}
		}

		if (injectTable->Count == 0)
			StopReplay();

	}

	//*************************************************************************
	// Method:		ClearInjectTable
	// Description: Clear the inject table and reset the replayed property on each node
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void RecordedSessionDataArray::ClearInjectTable()
	{	
		RecordedSessionDataNode *recordedDataNode;

		injectTable->Clear();
		for (int i=0; i<this->Count; i++)
		{
			recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (this->GetDataNodeByIndex(i));
			recordedDataNode->NodeReplayed = false;
		}
	}

	//*************************************************************************
	// Method:		onLogsGenerated
	// Description: called when new logs are generated
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[],
		unsigned int processID, unsigned int threadID)
	{
		HoloScriptApplication *holoScript = holoScriptRegistry->getHoloScriptAppForProcess(processID); 
		String *appName; 
		RecordedSessionDataNode *recordedDataNode; 
		DisplayableDataNode *dataNode;
		DisplayableDataNode *modifiedDataNode;

		//check for re-entrancy
		if (!logsGeneratedMutex->WaitOne(0, false))
			return;

		if (!holoScript) 
		{
			logsGeneratedMutex->ReleaseMutex();
			return;
		}

		appName = holoScript->ApplicationName;
		recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (injectTable->get_Item(appName));

		//return if we didn't get a node, or aren't to the right log entry yet.  Special case if logindex is 1 on the node as that means inject at the start
		if ( (!recordedDataNode) || ((holoScript->LogEntryCount < recordedDataNode->LogIndex) && (recordedDataNode->LogIndex != 0)) )
		{
			logsGeneratedMutex->ReleaseMutex();
			return;
		}


		do
		{
			dataNode = recordedDataNode->OriginalNode;
			dataNode->processID = processID;
			dataNode->threadID = 0;			//we don't know how to match threads across sessions, so always set to 0
			dataNode->UpdateDelegates();
			Object *args[] = __gc new Object*[1];
			args[0] = dataNode;
			switch (dataNode->Type)
			{
			case UIData::Process:
				if (recordedDataNode->NodeEnabled == restartAUT)
				{
					if (!isTestInNextSession(appName))
					{
						//eat this restart if not a test in next session
						sessionCount++;
					}
					else
					{
						dataEventRegistry->OnRestartProject(false);
						injectTable->Remove(appName);
						logsGeneratedMutex->ReleaseMutex();
						return;
					}
				}
				break;
			case UIData::Test:
			case UIData::NetworkCorruptionFault:
			case UIData::FileCorruptionFault:
			case UIData::ResourceFault:
				if (recordedDataNode->NodeEnabled == enabled)
				{
					//if there is a matching delete node we need to update the ID and dataArray since it will change on replay
					DisplayableDataNode *deleteNode = GetDeleteTestNode(dataNode->ID);
					dataNode->CreateDelegate->DynamicInvoke(args);
					if (deleteNode)
					{
						deleteNode->ID = dataNode->ID;
						deleteNode->DataArray = dataNode->DataArray;
					}
				}
				else if (recordedDataNode->NodeEnabled == disabled)
					dataNode->DeleteDelegate->DynamicInvoke(args);
				break;
			case UIData::MemoryFault:
			case UIData::DiskFault:
			case UIData::NetworkFault:
			case UIData::RegistryFault:
			case UIData::ProcessFault:
			case UIData::CustomFault:
			case UIData::MemoryLimit:
			case UIData::DiskLimit:
			case UIData::NetworkUpLimit:
			case UIData::NetworkDownLimit:
				if (recordedDataNode->NodeEnabled == enabled)
					dataNode->CreateDelegate->DynamicInvoke(args);
				else if (recordedDataNode->NodeEnabled == disabled)
					dataNode->DeleteDelegate->DynamicInvoke(args);
				else if (recordedDataNode->NodeEnabled == modified)
				{
					modifiedDataNode = recordedDataNode->ModifiedNode;
					modifiedDataNode->processID = processID;
					modifiedDataNode->threadID = 0;
					modifiedDataNode->UpdateDelegates();
					Object *args[] = __gc new Object*[3];
					args[0] = dataNode;
					args[1] = modifiedDataNode;
					args[2] = recordedDataNode->PropertyName;
					dataNode->ModifyDelegate->DynamicInvoke(args);
				}
				break;
			}

			injectTable->Remove(appName);
			BuildInjectTable();
			recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (injectTable->get_Item(appName));
		} while ( (recordedDataNode) && ((holoScript->LogEntryCount > recordedDataNode->LogIndex) || (recordedDataNode->LogIndex == 0)) );

		logsGeneratedMutex->ReleaseMutex();
     }

	//*************************************************************************
	// Method:		RecordToFile
	// Description: dump data array to a file
	//
	// Parameters:
	//	none
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	bool RecordedSessionDataArray::RecordToFile()
	{
		RecordedSessionDataNode *recordedDataNode;
		DisplayableDataNode *dataNode;
		XmlTextWriter *xWriter;

		try
		{
			if (!Directory::Exists(recordedSessionFolder))
				Directory::CreateDirectory(recordedSessionFolder);

			xWriter = new XmlTextWriter(recordedSessionPath, System::Text::Encoding::UTF8);
		}
		catch(...)
		{
			String *msg = "You don't have sufficient permission to save this file: ";
			msg = String::Concat(msg, recordedSessionPath);
			HolodeckGui::UserNotification::ErrorNotify(msg);
			return false;
		}

		xWriter->Formatting = Formatting::Indented;
		xWriter->Indentation = 4;

		xWriter->WriteStartElement("Recorded_Session");
		for (int i=0; i<this->Count; i++)
		{
			recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (this->GetDataNodeByIndex(i));

			xWriter->WriteStartElement("Record");

			xWriter->WriteStartElement("Log");
			xWriter->WriteAttributeString("Index", Convert::ToString(recordedDataNode->LogIndex));
			
			if (recordedDataNode->LogIndex != 0)
			{
				xWriter->WriteAttributeString("Function", recordedDataNode->LogItem->Function);
				xWriter->WriteAttributeString("TimeDelta", Convert::ToString(recordedDataNode->TimeDelta));
			}
	
			xWriter->WriteAttributeString("TimeStamp", ConvertToTimeString(recordedDataNode->TimeStamp));
			xWriter->WriteAttributeString("Session", Convert::ToString(recordedDataNode->SessionCount));
			xWriter->WriteAttributeString("AppName", Convert::ToString(recordedDataNode->AppName));
			xWriter->WriteEndElement();  //Log

			dataNode = recordedDataNode->OriginalNode;
			if (dataNode)
			{
				switch (dataNode->Type)
				{
				case UIData::Process:
					xWriter->WriteStartElement("Inject");
					xWriter->WriteAttributeString("Type", Convert::ToString(dataNode->Type));
					xWriter->WriteAttributeString("Name", dataNode->Name);
					xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
					xWriter->WriteEndElement();  //Inject
					break;
				case UIData::Test:
					{
						xWriter->WriteStartElement("Inject");
						xWriter->WriteAttributeString("Type", Convert::ToString(dataNode->Type));
						xWriter->WriteAttributeString("Name", dataNode->ID);
						xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
						ScheduledTestDataNode *testDataNode = dynamic_cast <ScheduledTestDataNode*> (dataNode);
						if (testDataNode)
						{
							xWriter->WriteAttributeString("Function", testDataNode->Function);
							xWriter->WriteAttributeString("Category", testDataNode->Category);
							xWriter->WriteAttributeString("ReturnValue", testDataNode->ReturnValue);
							xWriter->WriteAttributeString("ErrorCode", testDataNode->ErrorCode);
							for (int x=0; (testDataNode->MatchParameters) && (x < testDataNode->MatchParameters->Count); x++)
							{
								InterceptedFunctionParameter *matchParam = dynamic_cast <InterceptedFunctionParameter*> (testDataNode->MatchParameters->Item[x]);
								for (int mParams=0; mParams < matchParam->TestCasesCount; mParams++)
								{
									ParamTestCase * ptCase = dynamic_cast <ParamTestCase *> (matchParam->TestCase(mParams));
									xWriter->WriteAttributeString(String::Concat("MatchParamName", Convert::ToString(x)), matchParam->Name);
									xWriter->WriteAttributeString(String::Concat("MatchParamValue", Convert::ToString(x)), ptCase->TestValue);
									xWriter->WriteAttributeString(String::Concat("MatchParamOperator", Convert::ToString(x)), ptCase->TestOperator);
								}
							}
							for (int x=0; (testDataNode->ChangeParameters) && (x < testDataNode->ChangeParameters->Count); x++)
							{
								InterceptedFunctionParameter *changeParam = dynamic_cast <InterceptedFunctionParameter*> (testDataNode->ChangeParameters->Item[x]);
								xWriter->WriteAttributeString(String::Concat("ChangeParamName", Convert::ToString(x)), changeParam->Name);
								xWriter->WriteAttributeString(String::Concat("ChangeParamValue", Convert::ToString(x)), changeParam->ChangeValue);
							}
						}
						xWriter->WriteEndElement();  //Inject
					}
					break;
				case UIData::ResourceFault:
					{
						ResourceTestDataNode *resourceTestDataNode = dynamic_cast <ResourceTestDataNode*> (dataNode);
						if (resourceTestDataNode)
						{
								xWriter->WriteStartElement("Inject");
								xWriter->WriteAttributeString("Type", Convert::ToString(resourceTestDataNode->Type));
								xWriter->WriteAttributeString("ResourceName", resourceTestDataNode->Name);
								xWriter->WriteAttributeString("FaultName", resourceTestDataNode->FaultName);
								xWriter->WriteAttributeString("FaultType", Convert::ToString(resourceTestDataNode->FaultType));
								xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
								xWriter->WriteEndElement();  //Inject
						}
					}
					break;
				case UIData::MemoryFault:
				case UIData::DiskFault:
				case UIData::NetworkFault:
				case UIData::RegistryFault:
				case UIData::ProcessFault:
				case UIData::CustomFault:
					xWriter->WriteStartElement("Inject");
					xWriter->WriteAttributeString("Type", Convert::ToString(dataNode->Type));
					xWriter->WriteAttributeString("Name", dataNode->Name);
					xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
					xWriter->WriteEndElement();  //Inject
					break;
				case UIData::MemoryLimit:
				case UIData::DiskLimit:
				case UIData::NetworkUpLimit:
				case UIData::NetworkDownLimit:
					{
						LimitPaneDataNode *limitDataNode = dynamic_cast <LimitPaneDataNode*> (dataNode);
						if (limitDataNode)
						{
							xWriter->WriteStartElement("Inject");
							xWriter->WriteAttributeString("Type", Convert::ToString(dataNode->Type));
							xWriter->WriteAttributeString("Name", dataNode->Name);
							xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
							xWriter->WriteAttributeString("Value", Convert::ToString(limitDataNode->LimitValue));
							xWriter->WriteEndElement();  //Inject
						}
						break;
					}
				case UIData::NetworkCorruptionFault:
					{
						xWriter->WriteStartElement("Inject");
						xWriter->WriteAttributeString("Type", Convert::ToString(dataNode->Type));
						xWriter->WriteAttributeString("Name", dataNode->ID);
						xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
						NetworkCorruptionDataNode *ncDataNode = dynamic_cast<NetworkCorruptionDataNode*>(dataNode);
						if (ncDataNode)
						{
							xWriter->WriteAttributeString("SentData", Convert::ToString(ncDataNode->SentData));
							xWriter->WriteAttributeString("ReceivedData", Convert::ToString(ncDataNode->SentData));
							xWriter->WriteAttributeString("PortString", ncDataNode->PortString);
							xWriter->WriteAttributeString("CorruptType", ncDataNode->CorruptType);
							xWriter->WriteAttributeString("SearchFor", ncDataNode->SearchFor);
							xWriter->WriteAttributeString("SearchRegularExpression", ncDataNode->SearchRegularExpression);
							xWriter->WriteAttributeString("ReplaceWith", ncDataNode->ReplaceWith);
							xWriter->WriteAttributeString("ReplacementString", ncDataNode->ReplacementString);
						}
						xWriter->WriteEndElement();  //Inject
						break;
					}
				case UIData::FileCorruptionFault:
					{
						xWriter->WriteStartElement("Inject");
						xWriter->WriteAttributeString("Type", Convert::ToString(dataNode->Type));
						xWriter->WriteAttributeString("Name", dataNode->ID);
						xWriter->WriteAttributeString("Enabled", Convert::ToString(recordedDataNode->NodeEnabled));
						FileCorruptionDataNode *fcDataNode = dynamic_cast<FileCorruptionDataNode*>(dataNode);
						if (fcDataNode)
						{
							xWriter->WriteAttributeString("OriginalFile", fcDataNode->OriginalFile);
							xWriter->WriteAttributeString("Regenerate", Convert::ToString(fcDataNode->Regenerate));
							xWriter->WriteAttributeString("CorruptType", fcDataNode->CorruptType);
							xWriter->WriteAttributeString("SearchFor", fcDataNode->SearchFor);
							xWriter->WriteAttributeString("SearchRegularExpression", fcDataNode->SearchRegularExpression);
							xWriter->WriteAttributeString("ReplaceWith", fcDataNode->ReplaceWith);
							xWriter->WriteAttributeString("ReplacementString", fcDataNode->ReplacementString);
						}
						xWriter->WriteEndElement();  //Inject
						break;
					}
				}
			}
			xWriter->WriteEndElement();  //Record
		}

		xWriter->WriteEndElement();  //Recorded Session

		xWriter->Flush();
		xWriter->Close();

		return true;
	}

	//*************************************************************************
	// Method:		SetSessionFolder
	// Description: copies/moves the recorded session to specified folder
	//
	// Parameters:
	//	newFolder - folder to copy/move the session to
	//	copy - true to copy the session, false to move
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::SetSessionFolder(String *newFolder, bool copy)
	{
		String *oldPath = recordedSessionPath;
		recordedSessionFolder = newFolder; 
		recordedSessionPath = String::Concat(recordedSessionFolder, "\\");
		recordedSessionPath = String::Concat(recordedSessionPath, recordedSessionName);
		recordedSessionPath = String::Concat(recordedSessionPath, ".xml");
		SaveRecordedSession(oldPath, copy);
	}

	//*************************************************************************
	// Method:		SaveRecordedSession
	// Description: Saves the recorded session to the current path
	//
	// Parameters:
	//	oldPath - path of the old session
	//  copy - true to copy the session, false to move
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::SaveRecordedSession(String *oldPath, bool copy)
	{
		
		if (oldPath)
		{
			try
			{
				if (!copy)
				{
					if (!Directory::Exists(recordedSessionFolder))
						Directory::CreateDirectory(recordedSessionFolder);
					File::Move(oldPath, recordedSessionPath);
				}
				else
				{
					if (!Directory::Exists(recordedSessionFolder))
						Directory::CreateDirectory(recordedSessionFolder);
					File::Copy(oldPath, recordedSessionPath, true);
				}
			}
			catch (...)
			{
				return;
			}
		}
	}

	//*************************************************************************
	// Method:		LoadRecordedSession
	// Description: loads a recorded session from an xml file
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::LoadRecordedSession(String *xmlFilePath)
	{
		RecordedSessionDataNode *recordedDataNode;
		DisplayableDataNode *dataNode;
		LimitPaneDataNode *limitDataNode;
		ScheduledTestDataNode *testDataNode;
		ResourceTestDataNode *resourceTestDataNode;
		NetworkCorruptionDataNode *ncDataNode;
		FileCorruptionDataNode *fcDataNode;
		InterceptedFunctionParameter *param;
		ParamTestCase *paramTestCase;
		ArrayList *matchParamArray = new ArrayList();
		ArrayList *changeParamArray = new ArrayList();
		LogItem *logItem;
		NodeType type;
		XmlTextReader *xReader = new XmlTextReader(xmlFilePath);
		String *index, *timeDelta, *appName, *enabled;
		unsigned int session;

		this->SessionPath = xmlFilePath;

		while (xReader->Read())
		{
			if (xReader->NodeType == XmlNodeType::Element)
			{
				if (xReader->Name->CompareTo("Log") == 0)
				{
					index = xReader->GetAttribute("Index");
					if (Convert::ToInt32(index) != 0)
					{
						logItem = new LogItem();
						logItem->Function = xReader->GetAttribute("Function");
						timeDelta = xReader->GetAttribute("TimeDelta");
					}
							
					session = Convert::ToInt32(xReader->GetAttribute("Session"));
					appName = xReader->GetAttribute("AppName");

					while ( (xReader->Name->CompareTo("Inject") != 0) && (xReader->Read()) )
					{
					}

					type = (NodeType) Convert::ToInt32(xReader->GetAttribute("Type"));
					switch (type)
					{
					case UIData::Process:
						{
							dataNode = new DisplayableDataNode(xReader->GetAttribute("Name"), type, 0, 0);
							enabled = xReader->GetAttribute("Enabled");
							recordedDataNode = new RecordedSessionDataNode(dataNode, logItem, Convert::ToInt32(index));
							break;
						}						
					case UIData::Test:
						{
							testDataNode = new ScheduledTestDataNode(xReader->GetAttribute("Name"), 0, 0);
							testDataNode->ID = testDataNode->Name;
							testDataNode->Category = xReader->GetAttribute("Category");
							testDataNode->Function = xReader->GetAttribute("Function");
							testDataNode->ReturnValue = xReader->GetAttribute("ReturnValue");
							testDataNode->ErrorCode = xReader->GetAttribute("ErrorCode");
							enabled = xReader->GetAttribute("Enabled");
							while (xReader->MoveToNextAttribute())
							{
								if (xReader->Name->StartsWith("MatchParamName"))
								{
									param = new InterceptedFunctionParameter();
									paramTestCase = new ParamTestCase();
									param->Name = xReader->Value;
								}
								else if (xReader->Name->StartsWith("MatchParamValue"))
								{
									paramTestCase->TestValue = xReader->Value;
								}
								else if (xReader->Name->StartsWith("MatchParamOperator"))
								{
									paramTestCase->TestOperator = xReader->Value;
									param->AddTestCase (paramTestCase);
									matchParamArray->Add(param);
								}
								else if (xReader->Name->StartsWith("ChangeParamName"))
								{
									param = new InterceptedFunctionParameter();
									param->Name = xReader->Value;
								}
								else if (xReader->Name->StartsWith("ChangeParamValue"))
								{
									param->ChangeValue = xReader->Value;
									changeParamArray->Add(param);
								}
							}

							testDataNode->MatchParameters = matchParamArray;
							testDataNode->ChangeParameters = changeParamArray;
							recordedDataNode = new RecordedSessionDataNode(testDataNode, logItem, Convert::ToInt32(index));
							break;
						}
					case UIData::ResourceFault:
						{
							resourceTestDataNode = new ResourceTestDataNode(xReader->GetAttribute("ResourceName"), xReader->GetAttribute("FaultName"), (FaultType) Convert::ToInt32(xReader->GetAttribute("FaultType")), 0, 0);
							enabled = xReader->GetAttribute("Enabled");
							recordedDataNode = new RecordedSessionDataNode(resourceTestDataNode, logItem, Convert::ToInt32(index));
							break;
						}
					case UIData::MemoryFault:
					case UIData::DiskFault:
					case UIData::NetworkFault:
					case UIData::RegistryFault:
					case UIData::ProcessFault:
					case UIData::CustomFault:
						{
							dataNode = new DisplayableDataNode(xReader->GetAttribute("Name"), type, 0, 0);
							enabled = xReader->GetAttribute("Enabled");
							recordedDataNode = new RecordedSessionDataNode(dataNode, logItem, Convert::ToInt32(index));
							break;
						}
					case UIData::MemoryLimit:
					case UIData::DiskLimit:
					case UIData::NetworkUpLimit:
					case UIData::NetworkDownLimit:
						{
							limitDataNode = new LimitPaneDataNode(xReader->GetAttribute("Name"), type, 0, 0);
							limitDataNode->LimitValue = Convert::ToInt64(xReader->GetAttribute("Value"));
							enabled = xReader->GetAttribute("Enabled");
							recordedDataNode = new RecordedSessionDataNode(limitDataNode, logItem, Convert::ToInt32(index));
							break;
						}
					case UIData::NetworkCorruptionFault:
						{
							ncDataNode = new NetworkCorruptionDataNode(xReader->GetAttribute("Name"), 0);
							ncDataNode->ID = ncDataNode->Name;
							enabled = xReader->GetAttribute("Enabled");
							ncDataNode->SentData = Convert::ToBoolean(xReader->GetAttribute("SentData"));
							ncDataNode->ReceivedData = Convert::ToBoolean(xReader->GetAttribute("ReceivedData"));
							ncDataNode->PortString = xReader->GetAttribute("PortString");
							ncDataNode->CorruptType = xReader->GetAttribute("CorruptType");
							ncDataNode->SearchFor = xReader->GetAttribute("SearchFor");
							ncDataNode->SearchRegularExpression = xReader->GetAttribute("SearchRegularExpression");
							ncDataNode->ReplaceWith = xReader->GetAttribute("ReplaceWith");
							ncDataNode->ReplacementString = xReader->GetAttribute("ReplacementString");
							if (ncDataNode->PortString)
								ncDataNode->GetPortListFromPortString();
							recordedDataNode = new RecordedSessionDataNode(ncDataNode, logItem, Convert::ToInt32(index));
							break;
						}
					case UIData::FileCorruptionFault:
						{
							fcDataNode = new FileCorruptionDataNode(xReader->GetAttribute("Name"), 0);
							fcDataNode->ID = fcDataNode->Name;
							enabled = xReader->GetAttribute("Enabled");
							fcDataNode->OriginalFile = xReader->GetAttribute("OriginalFile");
							fcDataNode->Regenerate = Convert::ToBoolean(xReader->GetAttribute("Regenerate"));
							fcDataNode->CorruptType = xReader->GetAttribute("CorruptType");
							fcDataNode->SearchFor = xReader->GetAttribute("SearchFor");
							fcDataNode->SearchRegularExpression = xReader->GetAttribute("SearchRegularExpression");
							fcDataNode->ReplaceWith = xReader->GetAttribute("ReplaceWith");
							fcDataNode->ReplacementString = xReader->GetAttribute("ReplacementString");
							recordedDataNode = new RecordedSessionDataNode(fcDataNode, logItem, Convert::ToInt32(index));
							break;
						}
					}

					if (Convert::ToInt32(index) != 0)
						recordedDataNode->TimeDelta = Convert::ToInt64(timeDelta);

					recordedDataNode->SessionCount = session;
					recordedDataNode->AppName = appName;
					recordedDataNode->NodeEnabled = (enableTypes) Convert::ToInt32(enabled);
					this->CreateDataNode(recordedDataNode);
				}
			}
		}
	}

	//*************************************************************************
	// Method:		ConvertToTimeString
	// Description: Converts a UInt64 time representation into a friendly string
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	String *RecordedSessionDataArray::ConvertToTimeString(UInt64 timeStamp)
	{
		ULARGE_INTEGER ulargeInt;
		FILETIME fileTime;
		SYSTEMTIME systemTime;
		StringBuilder *timeString = new StringBuilder;

		ulargeInt.QuadPart = timeStamp;
		fileTime.dwHighDateTime = ulargeInt.HighPart;
		fileTime.dwLowDateTime = ulargeInt.LowPart;

		FileTimeToSystemTime(&fileTime, &systemTime);

		timeString->Append(systemTime.wMonth);
		timeString->Append("/");
		timeString->Append(systemTime.wDay);
		timeString->Append("/");
		timeString->Append(systemTime.wYear);
		timeString->Append(" ");
		timeString->Append(systemTime.wHour);
		timeString->Append(":");
		timeString->Append(systemTime.wMinute);
		timeString->Append(":");
		timeString->Append(systemTime.wSecond);
		timeString->Append(".");
		timeString->Append(systemTime.wMilliseconds);

		return timeString->ToString();
	}

	//*************************************************************************
	// Method:		onProjectRestarted
	// Description: Tracks the number of restarts
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::onProjectRestarted()
	{
		sessionCount++;
					
		if (replaying)
		{
			BuildInjectTable();
			
			//replay will end if there is nothing left to inject
			if (!replaying)
				return;

			for (int i=0; i<holoScriptRegistry->Count; i++)
			{
				unsigned int procID;
				HoloScriptApplication *holoScriptApp = HoloScriptRegistry::getInstance()->getHoloScriptAppByIndex(i);	
				HoloScript::LogsGeneratedDelegate *logHandler = new HoloScript::LogsGeneratedDelegate(this, onLogsGenerated);
				holoScriptApp->AddOnLogGeneratedHandler(logHandler);
				//make sure any pre-tests are set
				holoScriptApp->GetApplicationProcessID(procID);
				onLogsGenerated(0, NULL, procID, 0);
			}
				
			if (uiEventRegistry->OnRecordedSessionReplayStarted && replaying)
			{
				DisplayableDataNode *dataNode = new DisplayableDataNode(this->Name, UIData::RecordedSession, 0, 0);
				uiEventRegistry->OnRecordedSessionReplayStarted->Invoke(dataNode);
			}
		}
	}

	//*************************************************************************
	// Method:		DeleteRecordedSession
	// Description: Deletes the recorded session
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void RecordedSessionDataArray::DeleteRecordedSession()
	{
		try
		{
			StopRecording();
			File::Delete(recordedSessionPath);				
		}
		catch (...)
		{
			return;
		}
	}

	//*************************************************************************
	// Method:		isTestInNextSession
	// Description: Peeks ahead to see if there is another node for this app.  
	//	restart nodes don't count
	//
	// Parameters:
	//	appName - application to check for
	//
	// Return Value: true if a node is found, false otherwise
	//*************************************************************************
	bool RecordedSessionDataArray::isTestInNextSession(String *appName)
	{
		RecordedSessionDataNode *recordedDataNode;

		for (int j=0; j<this->Count; j++)
		{
			recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (this->GetDataNodeByIndex(j));
			if ( (recordedDataNode->AppName->CompareTo(appName) == 0) && (!recordedDataNode->NodeReplayed) 
				&& (recordedDataNode->NodeEnabled != restartAUT) && (recordedDataNode->SessionCount == (sessionCount + 1)) )
			{
				return true;
			}
		}
		return false;
	}

	//*************************************************************************
	// Method:		GetDeleteTestNode
	// Description: Find a delete test node that matches this ID
	//
	// Parameters:
	//	testNodeID - test ID to look for
	//
	// Return Value: Node pointer if found, NULL otherwise
	//*************************************************************************
	DisplayableDataNode *RecordedSessionDataArray::GetDeleteTestNode(String *testNodeID)
	{
		RecordedSessionDataNode *recordedDataNode;
		DisplayableDataNode *dataNode;

		for (int i=0; i<this->Count; i++)
		{
			recordedDataNode = dynamic_cast <RecordedSessionDataNode*> (this->GetDataNodeByIndex(i));
			dataNode = recordedDataNode->OriginalNode;
			if ( (recordedDataNode->NodeEnabled == disabled) && dataNode && dataNode->ID->Equals(testNodeID) )
				return dataNode;
		}

		return NULL;
	}
}