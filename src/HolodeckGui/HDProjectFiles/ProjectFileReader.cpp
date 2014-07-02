//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectFileReader.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectFileReader
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Jan 08 2004		 B. Shirey	 File created.
//*************************************************************************
#include "ProjectFileReader.h"
#include "../FileCorruptionChangesParser.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ProjectFileReader
	// Description: Constructor for the ProjectFileReader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectFileReader::ProjectFileReader()
	{
		dataEventRegistry = DataEventRegistry::getInstance();
	}

	//*************************************************************************
	// Method:		~ProjectFileReader
	// Description: Destructor for the ProjectFileReader class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectFileReader::~ProjectFileReader()
	{
	}

	//*************************************************************************
	// Method:		OpenProject
	// Description: Opens project file
	//
	// Parameters:
	//		fileName - Full path with filename to open
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ProjectFileReader::OpenProject(String *fileName, Project *project)
	{
		currentProject = project;
		bool perThreadLogging = false;

		XmlTextReader *xmlReader = NULL;
		XmlValidatingReader *validator = NULL;
		XPathDocument * doc;
		String *AutPath;
		String *AutInjectMode;
		String *AutCmdLine;
		bool AutProcessChaining;
		bool AutInheritSettings;
		bool AutStartPaused;
		bool AutAttachDebugger;
		bool AutLogFirstChance;
		ArrayList *FilteredFuncs = new ArrayList();
		ArrayList *DiskFaults = new ArrayList();
		ArrayList *MemoryFaults = new ArrayList();
		ArrayList *NetworkFaults = new ArrayList();
		ArrayList *RegistryFaults = new ArrayList();
		ArrayList *ProcessFaults = new ArrayList();
		ArrayList *CustomFaults = new ArrayList();
		ArrayList *SchTests = new ArrayList();
		ArrayList *ResourceTests = new ArrayList();
		ArrayList *NetworkCorruptionFaults = new ArrayList();
		ArrayList *FileCorruptionFaults = new ArrayList();
		SortedList *SavedLogs = new SortedList();
		ArrayList *SavedCorruptedFiles = new ArrayList();
		ArrayList *SavedExceptions = new ArrayList();
		Object *args[] = __gc new Object*[1];
		

		bool diskLimitActive;
		Int64 diskLimitValue;
		String * diskLimitUnit;

		bool memoryLimitActive;
		Int64 memoryLimitValue;
		String * memoryLimitUnit;

		bool downLimitEnabled, upLimitEnabled;
		UInt64 downloadBandwidth, uploadBandwidth;

	
		try
		{
			xmlReader = new XmlTextReader(fileName);
			validator = new XmlValidatingReader(xmlReader);
			validator->ValidationType = ValidationType::DTD;
			// any errors will cause an XmlException to be thrown
			while (validator->Read())
			{
			}
			
			doc = new XPathDocument(fileName);
			navigator = doc->CreateNavigator();
		}

		catch (Exception *e)
		{
			UserNotification::ErrorNotify(String::Concat("There was an error reading the project file.", e->Message));

			if (xmlReader != NULL)
				xmlReader->Close();
			if (validator != NULL)
				validator->Close();
			return false;
		}

	
		//parse info 
		
		// move to the project tag
		navigator->MoveToFirstChild();
		//check for per thread logging
		bool hasMoreAttributes = navigator->MoveToFirstAttribute();
		bool foundVersionString = false;
		while (hasMoreAttributes)
		{
			if (navigator->Name->CompareTo("PerThreadLogging") == 0)
			{
				if (String::Compare(navigator->Value, "True") == 0)
					perThreadLogging = true;
				else
					perThreadLogging = false;
			}
			else if (navigator->Name->CompareTo("Version") == 0)
			{
				currentProject->VersionString = navigator->Value;
				foundVersionString = true;
			}
			
			hasMoreAttributes = navigator->MoveToNextAttribute();
			if (!hasMoreAttributes)
				navigator->MoveToParent();
		}

		if (!foundVersionString)
		{
			UserNotification::ErrorNotify("The project file's version could not be determined.  The project file can not be loaded.");
			return false;
		}

		bool hasMoreAUTs = false;
		if (navigator->HasChildren)
		{
			// move to the ApplicationsUnderTest tag
			navigator->MoveToFirstChild();
			if (navigator->HasChildren)
			{
				//move to the first ApplicationUnderTest tag
				navigator->MoveToFirstChild();
				hasMoreAUTs = true;				
			} 
		}

		
		while (hasMoreAUTs)
		{
			//set variables to defaults
            AutPath = "";
			AutInjectMode = "";
			AutCmdLine = "";
			AutProcessChaining = false;
			AutInheritSettings = false;
			AutStartPaused = false;
			AutAttachDebugger = false;
			AutLogFirstChance = false;
			FilteredFuncs->Clear();
			DiskFaults->Clear();
			MemoryFaults->Clear();
			NetworkFaults->Clear();
			RegistryFaults->Clear();
			ProcessFaults->Clear();
			CustomFaults->Clear();

			diskLimitActive = false;
			diskLimitValue = -1;
			diskLimitUnit = "B";

			memoryLimitActive = false;
			memoryLimitValue = -1;
			memoryLimitUnit = "B";

			downLimitEnabled = false;
			upLimitEnabled = false;
			downloadBandwidth = 0;
			uploadBandwidth = 0;			

			bool hasMoreAutElements = false;
			bool hasMoreAttributes = false;

			if (navigator->HasChildren)
			{
				//move to AppInfo
				navigator->MoveToFirstChild();
				hasMoreAutElements = true;
			}
			
			while (hasMoreAutElements)
			{
				if (String::Compare(navigator->Name, "AppInfo") == 0)
				{
					if (navigator->HasAttributes)
					{
						//move to ApplicationPathAndName
						navigator->MoveToFirstAttribute();
						hasMoreAttributes = true;
					}
					while (hasMoreAttributes)
					{
						if (String::Compare(navigator->Name, "ApplicationPathAndName") == 0)
							AutPath = navigator->Value;
						else if (String::Compare(navigator->Name, "InjectMode") == 0)
							AutInjectMode = navigator->Value;
						else if (String::Compare(navigator->Name, "StartPaused") == 0)
						{
							if (String::Compare(navigator->Value, "True") == 0)
								AutStartPaused = true;
						}
						else if (String::Compare(navigator->Name, "CommandLineParams") == 0)
							AutCmdLine = navigator->Value;
						else if (String::Compare(navigator->Name, "ProcessChaining") == 0)
						{
							if (String::Compare(navigator->Value, "True") == 0)
								AutProcessChaining = true;
						}
						else if (navigator->Name->CompareTo("AttachDebugger") == 0)
						{
							AutAttachDebugger = (String::Compare(navigator->Value, "True") == 0);
						}
						else if (navigator->Name->CompareTo("LogFirstChance") == 0)
						{
							AutLogFirstChance = (String::Compare(navigator->Value, "True") == 0);
						}
						else if (String::Compare(navigator->Name, "InheritSettings") == 0)
						{
							if (String::Compare(navigator->Value, "True") == 0)
								AutInheritSettings = true;
						}

						hasMoreAttributes = navigator->MoveToNextAttribute();
					}
					//move back up to just inside AUT tag
					navigator->MoveToParent();					
				}

					
				else if (String::Compare(navigator->Name, "FilteredFunctions") == 0)
				{
					FilteredFuncs = this->ParseFilteredFunctions();
				}

				else if (String::Compare(navigator->Name, "Faults") == 0)
				{
					bool hasMoreFaults = false;
					hasMoreAttributes = false;
					if (navigator->HasChildren)
					{
						hasMoreFaults = true;
						//move to DiskFaults
						navigator->MoveToFirstChild();
					}
					while (hasMoreFaults)
					{
						bool moreFaults = false;
						if (String::Compare(navigator->Name, "DiskFaults") == 0)
						{							
							DiskFaults = ParseFaults();						
						}
						else if (String::Compare(navigator->Name, "MemoryFaults") == 0)
						{
							MemoryFaults = ParseFaults();
						}
						else if (String::Compare(navigator->Name, "NetworkFaults") == 0)
						{
							NetworkFaults = ParseFaults();
						}
						else if (String::Compare(navigator->Name, "RegistryFaults") == 0)
						{
							RegistryFaults = ParseFaults();
						}
						else if (String::Compare(navigator->Name, "ProcessFaults") == 0)
						{
							ProcessFaults = ParseFaults();
						}
						else if (String::Compare(navigator->Name, "CustomFaults") == 0)
						{
							CustomFaults = ParseFaults();
						}
						hasMoreFaults = navigator->MoveToNext();
						//move back up to Faults
						if (!hasMoreFaults)
							navigator->MoveToParent();
					}
				}

				else if (String::Compare(navigator->Name, "Limits") == 0)
				{
					bool moreLimits = false;
					hasMoreAttributes = false;
					if (navigator->HasChildren)
					{
						moreLimits = true;
						//move to Disk/Memory/Network Limit tag
						navigator->MoveToFirstChild();
					}

					while (moreLimits)
					{
						hasMoreAttributes = false;

						if (String::Compare(navigator->Name, "DiskSpaceLimit") == 0)
						{
							if (navigator->HasAttributes)
							{
								navigator->MoveToFirstAttribute();
								hasMoreAttributes = true;
							}
						
							while (hasMoreAttributes)
							{
								if(String::Compare(navigator->Name, "LimitActivated") == 0)
								{
									if(String::Compare(navigator->Value, "True") == 0)
										diskLimitActive = true;
								}
								else if(String::Compare(navigator->Name, "Limit") == 0)
									diskLimitValue = Int64::Parse(navigator->Value);
								else if(String::Compare(navigator->Name, "LimitUnit") == 0)
									diskLimitUnit = navigator->Value;

								hasMoreAttributes = navigator->MoveToNextAttribute();
								//move back up to DiskSpaceLimit
								if (!hasMoreAttributes)
									navigator->MoveToParent();
							}
						}

						if (String::Compare(navigator->Name, "MemorySpaceLimit") == 0)
						{
							if (navigator->HasAttributes)
							{
								navigator->MoveToFirstAttribute();
								hasMoreAttributes = true;
							}
						
							while (hasMoreAttributes)
							{
								if(String::Compare(navigator->Name, "LimitActivated") == 0)
								{
									if(String::Compare(navigator->Value, "True") == 0)
										memoryLimitActive = true;
								}
								else if(String::Compare(navigator->Name, "Limit") == 0)
									memoryLimitValue = Int64::Parse(navigator->Value);
								else if(String::Compare(navigator->Name, "LimitUnit") == 0)
									memoryLimitUnit = navigator->Value;

								hasMoreAttributes = navigator->MoveToNextAttribute();
								//move back up to MemorySpaceLimit
								if (!hasMoreAttributes)
									navigator->MoveToParent();
							}
						}

						if (String::Compare(navigator->Name, "NetworkDownLimit") == 0)
						{
							if (navigator->HasAttributes)
							{
								navigator->MoveToFirstAttribute();
								hasMoreAttributes = true;
							}
						
							while (hasMoreAttributes)
							{
								if(String::Compare(navigator->Name, "LimitActivated") == 0)
								{
									if(String::Compare(navigator->Value, "True") == 0)
										downLimitEnabled = true;
								}
								else if(String::Compare(navigator->Name, "Limit") == 0)
									downloadBandwidth = Int64::Parse(navigator->Value);

								hasMoreAttributes = navigator->MoveToNextAttribute();
								//move back up to NetworkDownLimit
								if (!hasMoreAttributes)
									navigator->MoveToParent();
							}
						}						
						
						if (String::Compare(navigator->Name, "NetworkUpLimit") == 0)
						{
							if (navigator->HasAttributes)
							{
								navigator->MoveToFirstAttribute();
								hasMoreAttributes = true;
							}
						
							while (hasMoreAttributes)
							{
								if(String::Compare(navigator->Name, "LimitActivated") == 0)
								{
									if(String::Compare(navigator->Value, "True") == 0)
										upLimitEnabled = true;
								}
								else if(String::Compare(navigator->Name, "Limit") == 0)
									uploadBandwidth = Int64::Parse(navigator->Value);

								hasMoreAttributes = navigator->MoveToNextAttribute();
								//move back up to NetworkUpLimit
								if (!hasMoreAttributes)
									navigator->MoveToParent();
							}
						}

						moreLimits = navigator->MoveToNext();
						//move back up to Limits
						if (!moreLimits)
							navigator->MoveToParent();
					}
				}

				else if (String::Compare(navigator->Name, "ScheduledTests") == 0)
				{
					SchTests = ParseScheduledTests();
				}

				else if (String::Compare(navigator->Name, "ResourceTests") == 0)
				{
					ResourceTests = ParseResourceTests();
				}

				else if (String::Compare(navigator->Name, "NetworkCorruptionFaults") == 0)
				{
					NetworkCorruptionFaults = ParseNetworkCorruptionFaults();
				}

				else if (String::Compare(navigator->Name, "FileCorruptionFaults") == 0)
				{
					FileCorruptionFaults = ParseFileCorruptionFaults();
				}

				if (String::Compare(navigator->Name, "Log") == 0)
				{
					if (navigator->HasAttributes)
					{
						navigator->MoveToFirstAttribute();
						String *logFilePath = String::Concat (currentProject->Folder, S"\\", navigator->Value);
						navigator->MoveToNextAttribute();
						String *ExeAndLogFileTime = navigator->Value;
						String *logKey = CreateSavedLogKey(logFilePath, ExeAndLogFileTime);
						SavedLogs->Add(logKey, logFilePath);
					}
					//move back up to log level
					navigator->MoveToParent();
				}
				//find sibling
				hasMoreAutElements = navigator->MoveToNext();
			}
			
			//Make sure the AUT executable exists if it is a launch operation
			if (AutInjectMode->CompareTo("LAUNCH") == 0)
			{
				if (!File::Exists (AutPath))
				{
					String * errmsg = S"Could not locate the application executable specified below:\r\n\r\n";
					errmsg = String::Concat (errmsg, AutPath, S"\r\n\r\n", S"Would you like to locate the executable yourself?");

					if (UserNotification::AskNotify (errmsg) == IDYES)
					{
						OpenFileDialog * browseDialog = new OpenFileDialog();
						browseDialog->Filter = "Applications (*.exe)|*.exe";
						browseDialog->CheckFileExists = true;
						browseDialog->CheckPathExists = true;
						browseDialog->Multiselect = false;
						browseDialog->Title = "Select the application to test";

						if (browseDialog->ShowDialog() == DialogResult::OK)
						{
							AutPath = browseDialog->FileName;
						}
						else
							return false;
					}
					else
						return false;
				}
			}

			//Process information for the AUT
			String *keyString;
			SortedList * processInfo = new SortedList();
			keyString = "AutPath";
			processInfo->Add(keyString, AutPath);
			keyString = "AutCmdLine";
			processInfo->Add(keyString, AutCmdLine);
			keyString = "AutInjectMode";
			processInfo->Add(keyString, AutInjectMode);
			keyString = "AutStartPaused";
			processInfo->Add(keyString, __box(AutStartPaused));
			keyString = "AutProcessChaining";
			processInfo->Add(keyString, __box(AutProcessChaining));
			keyString = "AutAttachDebugger";
			processInfo->Add(keyString, __box(AutAttachDebugger));
			keyString = "AutLogFirstChance";
			processInfo->Add(keyString, __box(AutLogFirstChance));
			keyString = "AutInheritSettings";
			processInfo->Add(keyString, __box(AutInheritSettings));
			keyString = "FilteredFuncs";
			processInfo->Add(keyString, FilteredFuncs);
			keyString = "SchTests";
			processInfo->Add(keyString, SchTests);
			keyString = "ResourceTests";
			processInfo->Add(keyString, ResourceTests);
			keyString = "NetworkCorruptionFaults";
			processInfo->Add(keyString, NetworkCorruptionFaults);
			keyString = "FileCorruptionFaults";
			processInfo->Add(keyString, FileCorruptionFaults);

			SortedList * processFaults = new SortedList();
			keyString = "DiskFaults";
			processFaults->Add(keyString, DiskFaults);
			keyString = "MemoryFaults";
			processFaults->Add(keyString, MemoryFaults);
			keyString = "NetworkFaults";
			processFaults->Add(keyString, NetworkFaults);
			keyString = "RegistryFaults";
			processFaults->Add(keyString, RegistryFaults);
			keyString = "ProcessFaults";
			processFaults->Add(keyString, ProcessFaults);
			keyString = "CustomFaults";
			processFaults->Add(keyString, CustomFaults);

			SortedList * processLimits = new SortedList();
			keyString = "diskLimitActive";
			processLimits->Add(keyString, __box(diskLimitActive));
			keyString = "diskLimitValue";
			processLimits->Add(keyString, __box(diskLimitValue));
			keyString = "diskLimitUnit";
			processLimits->Add(keyString, diskLimitUnit);
			keyString = "memoryLimitActive";
			processLimits->Add(keyString, __box(memoryLimitActive));
			keyString = "memoryLimitValue";
			processLimits->Add(keyString, __box(memoryLimitValue));
			keyString = "memoryLimitUnit";
			processLimits->Add(keyString, memoryLimitUnit);
			keyString = "downLimitEnabled";
			processLimits->Add(keyString, __box(downLimitEnabled));
			keyString = "downloadBandwidth";
			processLimits->Add(keyString, __box(downloadBandwidth));
			keyString = "upLimitEnabled";
			processLimits->Add(keyString, __box(upLimitEnabled));
			keyString = "uploadBandwidth";
			processLimits->Add(keyString, __box(uploadBandwidth));

			if (OnLaunchSavedApp)
				OnLaunchSavedApp->Invoke(processInfo, processFaults, processLimits);

			
			//move to back up to ApplicationUnderTest
			navigator->MoveToParent();
			hasMoreAUTs = navigator->MoveToNext();
			//move back up to AppilcationsUnderTest after done with all AUTs
			if(!hasMoreAUTs)
				navigator->MoveToParent();
		}

		
		ArrayList * reportPaths = new ArrayList();
		ArrayList * recSessionPaths = new ArrayList();

		//move to reports
		navigator->MoveToNext();
		bool moreReports = false;
		if (navigator->HasChildren)
		{
			//move to report
			navigator->MoveToFirstChild();
			moreReports = true;
		}
		
		while (moreReports)
		{
			navigator->MoveToFirstAttribute();
			String * rPath = currentProject->Folder;
			if (!rPath->EndsWith (S"\\"))
				rPath = String::Concat (rPath, S"\\");
			rPath = String::Concat (rPath, navigator->Value);
			reportPaths->Add(rPath);
			//move back up to report
			navigator->MoveToParent();
			moreReports = navigator->MoveToNext();
			//move back up to Reports
			if (!moreReports)
				navigator->MoveToParent();
		}

		//move to recorded sessions
		navigator->MoveToNext();
		bool moreSessions = false;

		if (navigator->HasChildren)
		{
			//move to recorded session tag
			navigator->MoveToFirstChild();
			moreSessions = true;
		}
		while (moreSessions)
		{
            navigator->MoveToFirstAttribute();
			recSessionPaths->Add(navigator->Value);
			//move back up to recorded session;
			navigator->MoveToParent();
			moreSessions = navigator->MoveToNext();
			//move back up to RecordedSessions
			if (!moreSessions)
				navigator->MoveToParent();
		}

		//move to Saved Logs
		navigator->MoveToNext();
		bool moreSavedLogs = false;
		
		if (navigator->HasChildren)
		{
			//move to SavedLog
			navigator->MoveToFirstChild();
			moreSavedLogs = true;
		}
		while (moreSavedLogs)
		{
			navigator->MoveToFirstAttribute();
			String *logFilePath = String::Concat (currentProject->Folder, S"\\", navigator->Value);
			navigator->MoveToNextAttribute();
			String *ExeAndLogFileTime = navigator->Value;
			String *logKey = CreateSavedLogKey(logFilePath, ExeAndLogFileTime);
			SavedLogs->Add(logKey, logFilePath);
			//move back up to Saved Log level and look for siblings
			navigator->MoveToParent();
			moreSavedLogs = navigator->MoveToNext();
			if (!moreSavedLogs)
				navigator->MoveToParent();
		}

		//move to Saved Corrupted Files
		if (navigator->MoveToNext())
		{
			bool moreSavedFiles = false;
			
			if (navigator->HasChildren)
			{
				//move to CorruptedFiles
				navigator->MoveToFirstChild();
				moreSavedFiles = true;
			}
			while (moreSavedFiles)
			{
				navigator->MoveToFirstAttribute();
				String *changesFilePath = String::Concat (currentProject->Folder, S"\\", navigator->Value);
				SavedCorruptedFiles->Add(changesFilePath);
				//move back up to Saved Log level and look for siblings
				navigator->MoveToParent();
				moreSavedFiles = navigator->MoveToNext();
				if (!moreSavedFiles)
					navigator->MoveToParent();
			}
		}

		//move to Saved Exceptions
		if (navigator->MoveToNext())
		{
			bool moreExceptions = false;
			
			if (navigator->HasChildren)
			{
				//move to CorruptedFiles
				navigator->MoveToFirstChild();
				moreExceptions = true;
			}
			while (moreExceptions)
			{
				bool hasMoreAttributes = false;
				if (navigator->HasAttributes)
				{
					//move to ApplicationPathAndName
					navigator->MoveToFirstAttribute();
					hasMoreAttributes = true;
				}
				ExceptionDataNode* exception = new ExceptionDataNode("Exception", 0, 0);
				while (hasMoreAttributes)
				{
					if (String::Compare(navigator->Name, "TimeStamp") == 0)
						exception->TimeStamp = navigator->Value;
					else if (String::Compare(navigator->Name, "ExeName") == 0)
						exception->ExeName = navigator->Value;
					else if (String::Compare(navigator->Name, "Thread") == 0)
						exception->threadID = Int32::Parse(navigator->Value);
					else if (String::Compare(navigator->Name, "Description") == 0)
						exception->Exception = navigator->Value;
					else if (String::Compare(navigator->Name, "MiniDump") == 0)
						exception->MiniDumpFile = navigator->Value;

					hasMoreAttributes = navigator->MoveToNextAttribute();
				}
				SavedExceptions->Add(exception);

				//move back up to just inside AUT tag
				navigator->MoveToParent();					
				moreExceptions = navigator->MoveToNext();
				if (!moreExceptions)
					navigator->MoveToParent();
			}
		}

		if (reportPaths != NULL)
		{
			IEnumerator * enumerator = reportPaths->GetEnumerator();
			int reportCount = 0;
			while (enumerator->MoveNext())
			{
				if (File::Exists(enumerator->Current->ToString()))
				{
					ReportPaneDataArray * repDataArray = new ReportPaneDataArray(HoloScriptRegistry::getInstance());
					repDataArray->set_ReportPath(enumerator->Current->ToString());

					DisplayableDataNode *dataNode = new DisplayableDataNode(repDataArray->get_ReportName(), UIData::Report, 0, 0);
					dataNode->ID = repDataArray->get_ReportName();
					String *keyString = String::Concat(repDataArray->GetType()->Name, Convert::ToString(0));
					keyString = String::Concat(keyString, Convert::ToString(reportCount));
					if (!currentProject->ContainsDataArrayKey(keyString))
						currentProject->AddDataArray(keyString, repDataArray);
					dataNode->DataArray = repDataArray;
					dataEventRegistry->OnReportCreated->Invoke(dataNode);
					reportCount++;
				}
			}
		}
	
		if (recSessionPaths != NULL)
		{
			IEnumerator *enumerator = recSessionPaths->GetEnumerator();
			while (enumerator->MoveNext())
			{
				if (File::Exists(enumerator->Current->ToString()))
				{
					RecordedSessionDataArray * recordedSessionData = new RecordedSessionDataArray(HoloScriptRegistry::getInstance());
					recordedSessionData->LoadRecordedSession(enumerator->Current->ToString());
					String *keyString = recordedSessionData->Name;
					if (!currentProject->ContainsDataArrayKey(keyString))
						currentProject->AddDataArray(keyString, recordedSessionData);

					DisplayableDataNode *dataNode = new DisplayableDataNode(recordedSessionData->Name, UIData::RecordedSession, 0, 0);
					dataNode->DataArray = recordedSessionData;
					Object *args[] = __gc new Object*[1];
					args[0] = dataNode;
					dataNode->CreatedDelegate->DynamicInvoke(args);
					currentProject->CurrentRecordingSession = recordedSessionData;
				}
			}
		}

		//Add saved logs
		if (SavedLogs != NULL)
		{
			for (int savedLogFileCount = 0; savedLogFileCount < SavedLogs->Count; savedLogFileCount++)
			{
				String *logFilePath = SavedLogs->GetByIndex(savedLogFileCount)->ToString();
				String *logKey = SavedLogs->GetKey(savedLogFileCount)->ToString()->ToLower();
 				if (File::Exists(logFilePath))
				{
					//parse PID from key
					int index = logKey->LastIndexOf("&");
					unsigned int oldPID = UInt32::Parse(logKey->Substring(index+1, logKey->Length-index-1));
					//remove pid from key string
					String *ExeAndLogFileTime = logKey->Substring(0, index);
					//parse exe name and file time from key
					index = ExeAndLogFileTime->LastIndexOf(".exe&");
					index = index + 4;
					String *ExeName = ExeAndLogFileTime->Substring(0, index);
					String *logFileTime = ExeAndLogFileTime->Substring(++index, ExeAndLogFileTime->Length-index);
					
					LogPaneDataArray * logDataArray = new LogPaneDataArray(ExeName, logFilePath, oldPID, ExeAndLogFileTime);
					//make old log node name from app name and  save time
					String *nodeName = String::Concat(ExeName, " (", logFileTime, ")"); 
					nodeName = String::Concat(" #", (savedLogFileCount+1).ToString(), ". ", nodeName);
					DisplayableDataNode *dataNode = new DisplayableDataNode(nodeName, UIData::SavedLogPane, oldPID, 0);
					dataNode->ID = String::Concat(oldPID.ToString(), dataNode->threadID.ToString());
					String *keyString = String::Concat(logDataArray->GetType()->Name, Convert::ToString(oldPID));
					keyString = String::Concat(keyString, Convert::ToString(savedLogFileCount));
					if (!currentProject->ContainsDataArrayKey(keyString))
						currentProject->AddDataArray(keyString, logDataArray);
					dataNode->DataArray = logDataArray;

					NetworkLogPaneDataArray *netLogDataArray = new NetworkLogPaneDataArray(oldPID, 0, logFilePath, logDataArray->HoloScriptApplicationInstance);
					nodeName = String::Concat(nodeName, " Network Log");
					DisplayableDataNode *netDataNode = new DisplayableDataNode(nodeName, UIData::SavedNetworkLogPane, oldPID, 0);
					netDataNode->ID = String::Concat("nl", oldPID.ToString(), netDataNode->threadID.ToString());
					keyString = String::Concat(netLogDataArray->GetType()->Name, Convert::ToString(oldPID));
					keyString = String::Concat(keyString, Convert::ToString(savedLogFileCount));
					if (!currentProject->ContainsDataArrayKey(keyString))
						currentProject->AddDataArray(keyString, netLogDataArray);
					netDataNode->DataArray = netLogDataArray;

					dataEventRegistry->OnLogPaneCreated->Invoke(dataNode);
					dataEventRegistry->OnNetworkLogPaneCreated->Invoke(netDataNode);
				}
			}
		}

		// Add saved corrupted files
		SavedFileCorruptionDataArray* savedFilesArray = new SavedFileCorruptionDataArray(0);
		currentProject->AddDataArray(S"Saved Fuzzed Files", savedFilesArray);
		if (SavedCorruptedFiles != NULL)
		{
			for (int savedFileCount = 0; savedFileCount < SavedCorruptedFiles->Count; savedFileCount++)
			{
				String *changesFilePath = dynamic_cast<String*>(SavedCorruptedFiles->Item[savedFileCount]);
 				if (File::Exists(changesFilePath))
				{
					SavedFileCorruptionDataNode* node = new SavedFileCorruptionDataNode(changesFilePath, NodeType::SavedCorruptedFile, 0);
					node->ChangesFile = changesFilePath;

					FileCorruptionChangesParser* parser = new FileCorruptionChangesParser();
					if (parser->Load(changesFilePath))
					{
						node->OriginalFile = parser->OriginalFile;
						node->CorruptedFile = parser->CorruptedFile;
						node->ID = changesFilePath;

						Object *args[] = __gc new Object*[1];
						args[0] = node;
						node->CreateDelegate->DynamicInvoke(args);
					}
				}
			}
		}

		// Add saved exceptions
		ExceptionDataArray* exceptionsArray = new ExceptionDataArray(0);
		currentProject->AddDataArray(S"Saved Exceptions", exceptionsArray);
		if (SavedExceptions != NULL)
		{
			for (int savedExceptionCount = 0; savedExceptionCount < SavedExceptions->Count; savedExceptionCount++)
			{
				ExceptionDataNode* exceptionInfo = dynamic_cast<ExceptionDataNode*>(SavedExceptions->Item[savedExceptionCount]);

				// Create a new data node that will be placed in the data array
				ExceptionDataNode* node = new ExceptionDataNode(exceptionInfo->MiniDumpFile, 0, exceptionInfo->threadID);
				node->TimeStamp = exceptionInfo->TimeStamp;
				node->ExeName = exceptionInfo->ExeName;
				node->Exception = exceptionInfo->Exception;
				node->MiniDumpFile = exceptionInfo->MiniDumpFile;
				node->ID = exceptionInfo->MiniDumpFile;

				Object *args[] = __gc new Object*[1];
				args[0] = node;
				node->CreateDelegate->DynamicInvoke(args);
			}
		}

		if (perThreadLogging)
		{
			currentProject->IsInPerThreadMode = true;
		}
		
		if (xmlReader != NULL)
			xmlReader->Close();
		if (validator != NULL)
			validator->Close();

		return true;
	}

	//*************************************************************************
	// Method:		ParseFilteredFunctions
	// Description: parses list of filtered functions from project file
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing list of functions
	//*************************************************************************
	ArrayList *ProjectFileReader::ParseFilteredFunctions()
	{
		XPathNavigator * funcNavigator = navigator->Clone();
		ArrayList * FilteredFuncs = new ArrayList();
		bool moreFuncs = false;
		
		if (funcNavigator->HasChildren)
		{
			moreFuncs = true;
			//move to FilteredFunction
			funcNavigator->MoveToFirstChild();
		}
		while (moreFuncs)
		{
			FilteredFuncs->Add(funcNavigator->Value);
			moreFuncs = funcNavigator->MoveToNext();
		}
		return FilteredFuncs;
	}

	//*************************************************************************
	// Method:		ParseFaults
	// Description: parses list of disk/memory/network faults from project file
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing list of disk/memory/network faults
	//*************************************************************************
	ArrayList *ProjectFileReader::ParseFaults()
	{
		XPathNavigator * faultNavigator = navigator->Clone();
		bool moreFaults = false;
		ArrayList * Faults = new ArrayList();
		if (faultNavigator->HasChildren)
		{
			moreFaults = true;
			//move to Fault
			faultNavigator->MoveToFirstChild();
		}
		while (moreFaults)
		{
			faultNavigator->MoveToFirstAttribute();
			Faults->Add(faultNavigator->Value);
			//move back up to Fault
			faultNavigator->MoveToParent();
			moreFaults = faultNavigator->MoveToNext();
		}
		return Faults;
	}

	//*************************************************************************
	// Method:		ParseNetworkCorruptionFaults
	// Description: parses list of network corruption faults from project file
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing list of scheduled tests
	//*************************************************************************
	ArrayList *ProjectFileReader::ParseNetworkCorruptionFaults()
	{
		ArrayList *networkCorruptionFaults = new ArrayList();
		NetworkCorruptionDataNode *networkCorruption;
		
		XPathNavigator * testNavigator = navigator->Clone();
		
		bool hasMoreFaults = false;
		bool hasMoreAttributes = false;

		if (testNavigator->HasChildren)
		{
			hasMoreFaults = true;
			//move to NetworkCorruption
			testNavigator->MoveToFirstChild();
		}
		while (hasMoreFaults)
		{
			hasMoreAttributes = false;
			ArrayList *portList = new ArrayList();

			if (testNavigator->HasAttributes)
			{
				networkCorruption = new NetworkCorruptionDataNode("", 0);
				hasMoreAttributes = true;
				testNavigator->MoveToFirstAttribute();
			}
			
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "Enabled") == 0)
					networkCorruption->IsTestOn = (testNavigator->Value->CompareTo("true") == 0) ? true : false;
				else if (String::Compare(testNavigator->Name, "SentData") == 0)
					networkCorruption->SentData = Convert::ToBoolean(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "ReceivedData") == 0)
					networkCorruption->ReceivedData = Convert::ToBoolean(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "PortString") == 0)
					networkCorruption->PortString = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "CorruptType") == 0)
					networkCorruption->CorruptType = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "SearchFor") == 0)
					networkCorruption->SearchFor = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "SearchRegularExpression") == 0)
					networkCorruption->SearchRegularExpression = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "ReplaceWith") == 0)
					networkCorruption->ReplaceWith = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "ReplacementString") == 0)
					networkCorruption->ReplacementString = testNavigator->Value->ToString();
				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}

			bool hasMoreParams = false;

			//move to portlist
			testNavigator->MoveToFirstChild();
			
			if (testNavigator->HasChildren)
			{
				hasMoreParams = true;
				//move to match param
				testNavigator->MoveToFirstChild();
			}

			//get all the ports
			while (hasMoreParams)
			{
				Int32 port;
				hasMoreAttributes = false;
				if (testNavigator->HasAttributes)
				{
					hasMoreAttributes = true;
					//move to match port attrtibutes
					testNavigator->MoveToFirstAttribute();
				}

				while (hasMoreAttributes)
				{
					if (String::Compare(testNavigator->Name, "Name") == 0)
						port = Convert::ToInt32(testNavigator->Value);
					
					hasMoreAttributes = testNavigator->MoveToNextAttribute();
					if (!hasMoreAttributes)
					{
						testNavigator->MoveToParent();
						portList->Add(__box(port));
					}
				}
				hasMoreParams = testNavigator->MoveToNext();
				//move back up to portlist
				if (!hasMoreParams)
					testNavigator->MoveToParent();
			}
			networkCorruption->PortList = portList;

			//move to the settings node
			testNavigator->MoveToNext();
			
			NetworkCorruptionUISettings settings;
			ParseNetworkCorruptionSettings(testNavigator, &settings);
			networkCorruption->UISettings = settings;

			//add to list of faults
			networkCorruptionFaults->Add(networkCorruption);

			//move to NetworkCorruption
			testNavigator->MoveToParent();
			
			hasMoreFaults = testNavigator->MoveToNext();			
		}
		return networkCorruptionFaults;
	}

	//*************************************************************************
	// Method:		ParseFileCorruptionFaults
	// Description: parses list of file corruption faults from project file
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing list of file corruption faults
	//*************************************************************************
	ArrayList *ProjectFileReader::ParseFileCorruptionFaults()
	{
		ArrayList *fileCorruptionFaults = new ArrayList();
		FileCorruptionDataNode *fileCorruption;
		
		XPathNavigator *testNavigator = navigator->Clone();
		
		bool hasMoreFaults = false;
		bool hasMoreAttributes = false;

		if (testNavigator->HasChildren)
		{
			hasMoreFaults = true;
			//move to FileCorruption
			testNavigator->MoveToFirstChild();
		}
		while (hasMoreFaults)
		{
			hasMoreAttributes = false;

			if (testNavigator->HasAttributes)
			{
				fileCorruption = new FileCorruptionDataNode("", 0);
				hasMoreAttributes = true;
				testNavigator->MoveToFirstAttribute();
			}
			
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "Enabled") == 0)
					fileCorruption->IsTestOn = (testNavigator->Value->CompareTo("true") == 0) ? true : false;
				else if (String::Compare(testNavigator->Name, "OriginalFile") == 0)
					fileCorruption->OriginalFile = testNavigator->Value;
				else if (String::Compare(testNavigator->Name, "Regenerate") == 0)
					fileCorruption->Regenerate = Convert::ToBoolean(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "CorruptType") == 0)
					fileCorruption->CorruptType = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "SearchFor") == 0)
					fileCorruption->SearchFor = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "SearchRegularExpression") == 0)
					fileCorruption->SearchRegularExpression = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "ReplaceWith") == 0)
					fileCorruption->ReplaceWith = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "ReplacementString") == 0)
					fileCorruption->ReplacementString = testNavigator->Value->ToString();
				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}

			if (testNavigator->HasChildren)
			{
				testNavigator->MoveToFirstChild();

				FileCorruptionUISettings settings;
				ParseFileCorruptionSettings(testNavigator, &settings);
				fileCorruption->UISettings = settings;

				testNavigator->MoveToParent();
			}
			
			//add to list of faults
			fileCorruptionFaults->Add(fileCorruption);
			
			hasMoreFaults = testNavigator->MoveToNext();			
		}
		return fileCorruptionFaults;
	}

	//*************************************************************************
	// Method:		ParseScheduledTests
	// Description: parses list of scheduled tests from project file
	//
	// Parameters:
	//		None
	//
	// Return Value: ArrayList containing list of scheduled tests
	//*************************************************************************
	ArrayList *ProjectFileReader::ParseScheduledTests()
	{
		ArrayList * SchTests = new ArrayList();
		ScheduledTestDataNode *schTest;
		
		XPathNavigator *testNavigator = navigator->Clone();
		
		bool hasMoreTests = false;
		bool hasMoreAttributes = false;

		if (testNavigator->HasChildren)
		{
			hasMoreTests = true;
			//move to ScheduledTest
			testNavigator->MoveToFirstChild();
		}
		while (hasMoreTests)
		{
			hasMoreAttributes = false;
			ArrayList * matchParams = new ArrayList();
			ArrayList * changeParams = new ArrayList();

			if (testNavigator->HasAttributes)
			{
				schTest = new ScheduledTestDataNode("", 0, 0);
				hasMoreAttributes = true;
				testNavigator->MoveToFirstAttribute();
			}
			
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "Enabled") == 0)
					schTest->IsTestOn = (testNavigator->Value->CompareTo("true") == 0) ? true : false;
				else if (String::Compare(testNavigator->Name, "TestID") == 0)
					schTest->TestID = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "FunctionName") == 0)
					schTest->Function = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "Category") == 0)
					schTest->Category = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "SpecifyErrorCode") == 0)
					schTest->SpecifyErrorCode = (testNavigator->Value->CompareTo("true") == 0) ? true : false;
				else if (String::Compare(testNavigator->Name, "ErrorCode") == 0)
					schTest->ErrorCode = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "ReturnValue") == 0)
					schTest->ReturnValue = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "Owner") == 0)
					schTest->TestOwner = testNavigator->Value->ToString();

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}

			bool hasMoreParams = false;

			//move to MatchParams
			testNavigator->MoveToFirstChild();
			
			if (testNavigator->HasChildren)
			{
				hasMoreParams = true;
				//move to match param
				testNavigator->MoveToFirstChild();
			}

			//get all the match parameters
			while (hasMoreParams)
			{
				InterceptedFunctionParameter *matchParam = new InterceptedFunctionParameter();
				ParamTestCase * ptCase = NULL;
				bool newParamCase = false;
				hasMoreAttributes = false;
				if (testNavigator->HasAttributes)
				{
					hasMoreAttributes = true;
					//move to match param attrtibutes
					testNavigator->MoveToFirstAttribute();
				}

				while (hasMoreAttributes)
				{
					if (String::Compare(testNavigator->Name, "Name") == 0)
					{
						//Check if the param is already in the list
						//If so, then add the match values to it
						//If not, then add the param to the list and the match values also
						newParamCase = false;
						matchParam = NULL;
						for (int sp=0; sp < matchParams->Count; sp++)
						{
							InterceptedFunctionParameter * ifp = dynamic_cast <InterceptedFunctionParameter *> (matchParams->Item[sp]);
							if (String::Compare (ifp->Name, testNavigator->Value) == 0)
							{
								matchParam = ifp;
								newParamCase = false;
								continue;
							}
						}
						if (matchParam == NULL)
						{
							matchParam = new InterceptedFunctionParameter();
							matchParam->Name = testNavigator->Value;
							newParamCase = true;
						}

						ptCase = new ParamTestCase();
					}
					else if (String::Compare(testNavigator->Name, "TestOperator") == 0)
						ptCase->TestOperator = testNavigator->Value;
					else if (String::Compare(testNavigator->Name, "TestValue") == 0)
						ptCase->TestValue = testNavigator->Value;
					else if (String::Compare(testNavigator->Name, "CompareAsType") == 0)
						matchParam->CompareAsType = Convert::ToInt32(testNavigator->Value);
					else if (String::Compare(testNavigator->Name, "ID") == 0)
						matchParam->ID = Convert::ToInt32(testNavigator->Value);
					
					hasMoreAttributes = testNavigator->MoveToNextAttribute();
					if (!hasMoreAttributes)
					{
						testNavigator->MoveToParent();
						matchParam->AddTestCase (ptCase);
						
						if (newParamCase)
							matchParams->Add(matchParam);
					}
				}
				hasMoreParams = testNavigator->MoveToNext();
				//move back up to MatchParams
				if (!hasMoreParams)
					testNavigator->MoveToParent();
			}
			schTest->set_MatchParameters(matchParams);
			
			//move to Change Params
			testNavigator->MoveToNext();
			hasMoreParams = false;

			if (testNavigator->HasChildren)
			{
				hasMoreParams = true;
				//move to change param
				testNavigator->MoveToFirstChild();
			}

			//get all the change parameters
			while (hasMoreParams)
			{
				InterceptedFunctionParameter *changeParam = new InterceptedFunctionParameter();
				hasMoreAttributes = false;
				if (testNavigator->HasAttributes)
				{
					hasMoreAttributes = true;
					//move to change param attrtibutes
					testNavigator->MoveToFirstAttribute();
				}

				while (hasMoreAttributes)
				{
					if (String::Compare(testNavigator->Name, "Name") == 0)
						changeParam->Name = testNavigator->Value;
					else if (String::Compare(testNavigator->Name, "TestValue") == 0)
						changeParam->ChangeValue = testNavigator->Value;
					else if (String::Compare(testNavigator->Name, "CompareAsType") == 0)
						changeParam->CompareAsType = Convert::ToInt32(testNavigator->Value);
					else if (String::Compare(testNavigator->Name, "ID") == 0)
						changeParam->ID = Convert::ToInt32(testNavigator->Value);
					hasMoreAttributes = testNavigator->MoveToNextAttribute();
					if (!hasMoreAttributes)
					{
						//move to ChangeParam
						testNavigator->MoveToParent();
						changeParams->Add(changeParam);
					}
				}
				hasMoreParams = testNavigator->MoveToNext();
				if (!hasMoreParams)
				{
					//move to ChangeParams
					testNavigator->MoveToParent();
				}
			}
			schTest->set_ChangeParameters(changeParams);

			//move to Firing Specification
			ArrayList * details = new ArrayList();
			hasMoreAttributes = false;
			if (testNavigator->MoveToNext())
			{
				//If a firing specification node is present then read firing attribs
				if (testNavigator->HasAttributes)
				{
					hasMoreAttributes = true;
					testNavigator->MoveToFirstAttribute();
				}
				while (hasMoreAttributes)
				{
					if (String::Compare(testNavigator->Name, "FiringOption") == 0)
						schTest->FiringOption = Int32::Parse (testNavigator->Value);

					if (String::Compare(testNavigator->Name, "PauseAUTAfterTest") == 0)
						schTest->PauseAppOnTestFire = String::Compare (testNavigator->Value, "true", true) == 0? true:false;

					hasMoreAttributes = testNavigator->MoveToNextAttribute();
					if (!hasMoreAttributes)
						testNavigator->MoveToParent();
				}

				//get to the firingdetails node
				if (testNavigator->HasChildren)
				{
					//move to the firingdetails node
					testNavigator->MoveToFirstChild();

					//look for and navigate FiringData nodes
					if (testNavigator->HasChildren)
					{
						//Get to the first node
						bool hasMoreChildren = true;
						testNavigator->MoveToFirstChild();

						while (hasMoreChildren)
						{
							//Read the data attribute
							if (testNavigator->HasAttributes)
							{
								//there is only one attribute per FiringData node
								testNavigator->MoveToFirstAttribute();
								details->Add (testNavigator->Value);
								testNavigator->MoveToParent();
							}

							//move to next child (firingdata) or else move
							//back to firingdetails
							hasMoreChildren = testNavigator->MoveToNext();
							if (!hasMoreChildren)
								testNavigator->MoveToParent();
						}
					}

					//set whatever data was received
					if (schTest->FiringOption == 1)
					{
						schTest->FiringDetails = dynamic_cast <Object *> (details->Item[0]);
					}
					else
					{
						schTest->FiringDetails = dynamic_cast <Object *> (details);
					}
					
					//return back to FiringSpecification
					testNavigator->MoveToParent();
				}
			}
			else
			{
				//If there is no firing specification node
				//then create default option which is to fire
				//the test 100% of the times without pausing the aut
				schTest->FiringOption = 1;
				schTest->FiringDetails = dynamic_cast <Object *> (new String ("100"));
				schTest->PauseAppOnTestFire = false;
			}

			//add to list of tests
			SchTests->Add(schTest);

			//move to ScheduledTest
			testNavigator->MoveToParent();
			
			hasMoreTests = testNavigator->MoveToNext();			
		}
		return SchTests;
	}

	//*************************************************************************
	// Method:		ParseResourceTests
	// Description: parses list of resource tests from project file
	//
	// Parameters:
	//	None
	//
	// Return Value: ArrayList containing list of resource tests
	//*************************************************************************
	ArrayList *ProjectFileReader::ParseResourceTests()
	{
		ArrayList *resourceTests = new ArrayList();
		ResourceTestDataNode *resourceTest;
		
		XPathNavigator *testNavigator = navigator->Clone();
		
		bool hasMoreTests = false;
		bool hasMoreAttributes = false;

		if (testNavigator->HasChildren)
		{
			hasMoreTests = true;
			//move to ScheduledTest
			testNavigator->MoveToFirstChild();
		}
		while (hasMoreTests)
		{
			hasMoreAttributes = false;

			if (testNavigator->HasAttributes)
			{
				hasMoreAttributes = true;
				testNavigator->MoveToFirstAttribute();
			}
			
			String *faultName, *resourceName;
			FaultType faultType;
			bool isResourceTestOn = false;
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "Enabled") == 0)
					isResourceTestOn = (testNavigator->Value->CompareTo("true") == 0) ? true : false;
				else if (String::Compare(testNavigator->Name, "FaultName") == 0)
					faultName = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "ResourceName") == 0)
					resourceName = testNavigator->Value->ToString();
				else if (String::Compare(testNavigator->Name, "FaultType") == 0)
					faultType = (FaultType) Convert::ToInt32(testNavigator->Value->ToString());

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}

			resourceTest = new ResourceTestDataNode(resourceName, faultName, faultType, 0, 0);
			resourceTest->IsTestOn = isResourceTestOn;

			//add to list of tests
			resourceTests->Add(resourceTest);
			
			hasMoreTests = testNavigator->MoveToNext();			
		}
		return resourceTests;
	}

	//*************************************************************************
	// Method:		CreateSavedLogKey
	// Description: creates a key for storing saved log in sorted list
	//
	// Parameters:
	//	logFilePath - full path to log file
	//	ExeNameAndSaveTime - exe name and save time for the log
	//
	// Return Value: log key
	//*************************************************************************
	String *ProjectFileReader::CreateSavedLogKey(String *logFilePath, String *ExeAndLogFileTime)
	{
		int index = logFilePath->LastIndexOf("\\");
		String *logPID = logFilePath->Substring(++index, logFilePath->Length-index);
		logPID =logPID->Substring(0, logPID->Length-4);
		String *logKey = String::Concat(ExeAndLogFileTime, "&", logPID);
		return logKey;
	}

	//*************************************************************************
	// Method:		ParseNetworkCorruptionSettings
	// Description: reads a network corruption settings node and makes an object
	//	out of it
	//
	// Parameters:
	//	navigator - the xml navigator object at the start of the element in the xml
	//	settings (out) - the object to get created
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileReader::ParseNetworkCorruptionSettings(XPathNavigator *navigator, NetworkCorruptionUISettings *settings)
	{
		XPathNavigator *testNavigator = navigator->Clone();

		bool hasMoreAttributes = false;

		if (testNavigator->HasAttributes)
		{
			hasMoreAttributes = true;
			testNavigator->MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "Type") == 0)
					settings->type = testNavigator->Value;

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}
		}

		if (testNavigator->HasChildren)
		{
			testNavigator->MoveToFirstChild();
			ParseCorruptionRandomSettings(testNavigator, &(settings->randomSettings));
			
			testNavigator->MoveToNext();
			ParseCorruptionSearchSettings(testNavigator, &(settings->searchSettings));

			testNavigator->MoveToNext();
			ParseCorruptionRegExprSettings(testNavigator, &(settings->regExprSettings));

			testNavigator->MoveToParent();
		}
	}

	//*************************************************************************
	// Method:		ParseFileCorruptionSettings
	// Description: reads a file corruption settings node and makes an object
	//	out of it
	//
	// Parameters:
	//	navigator - the xml navigator object at the start of the element in the xml
	//	settings (out) - the object to get created
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileReader::ParseFileCorruptionSettings(XPathNavigator *navigator, FileCorruptionUISettings *settings)
	{
		XPathNavigator *testNavigator = navigator->Clone();

		bool hasMoreAttributes = false;

		if (testNavigator->HasAttributes)
		{
			hasMoreAttributes = true;
			testNavigator->MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "Type") == 0)
					settings->type = testNavigator->Value;
				else if (String::Compare(testNavigator->Name, "Regen") == 0)
					settings->regen = Convert::ToBoolean(testNavigator->Value);

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}
		}

		if (testNavigator->HasChildren)
		{
			testNavigator->MoveToFirstChild();
			ParseCorruptionRandomSettings(testNavigator, &(settings->randomSettings));
			
			testNavigator->MoveToNext();
			ParseCorruptionSearchSettings(testNavigator, &(settings->searchSettings));

			testNavigator->MoveToNext();
			ParseCorruptionRegExprSettings(testNavigator, &(settings->regExprSettings));

			testNavigator->MoveToParent();
		}
	}

	//*************************************************************************
	// Method:		ParseCorruptionRandomSettings
	// Description: reads a random corruption settings node and makes an object
	//	out of it
	//
	// Parameters:
	//	navigator - the xml navigator object at the start of the element in the xml
	//	settings (out) - the object to get created
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileReader::ParseCorruptionRandomSettings(XPathNavigator *navigator, CorruptionRandomSettings *settings)
	{
		XPathNavigator *testNavigator = navigator->Clone();

		bool hasMoreAttributes = false;

		if (testNavigator->HasAttributes)
		{
			hasMoreAttributes = true;
			testNavigator->MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "CorruptAmount") == 0)
					settings->corruptAmount = Convert::ToInt32(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "CorruptionRandomType") == 0)
				{
					if (String::Compare(testNavigator->Value, "SingleBytes") == 0)
						settings->type = CorruptionRandomType::RandomCorruptSingleBytes;
					else if (String::Compare(testNavigator->Value, "LongStrings") == 0)
						settings->type = CorruptionRandomType::RandomCorruptLongStrings;
					else if (String::Compare(testNavigator->Value, "RepeatedSequence") == 0)
						settings->type = CorruptionRandomType::RandomCorruptRepeatedSequence;
				}
				else if (String::Compare(testNavigator->Name, "Length") == 0)
					settings->length = Convert::ToInt32(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "CorruptStringType") == 0)
					settings->corruptStringType = Convert::ToInt32(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "CorruptString") == 0)
					settings->corruptString = testNavigator->Value;
				else if (String::Compare(testNavigator->Name, "Overwrite") == 0)
					settings->overwrite = Convert::ToBoolean(testNavigator->Value);

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}
		}
	}
	
	//*************************************************************************
	// Method:		ParseCorruptionSearchSettings
	// Description: reads a search corruption settings node and makes an object
	//	out of it
	//
	// Parameters:
	//	navigator - the xml navigator object at the start of the element in the xml
	//	settings (out) - the object to get created
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileReader::ParseCorruptionSearchSettings(XPathNavigator *navigator, CorruptionSearchSettings *settings)
	{
		XPathNavigator *testNavigator = navigator->Clone();

		bool hasMoreAttributes = false;

		if (testNavigator->HasAttributes)
		{
			hasMoreAttributes = true;
			testNavigator->MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "SearchStringType") == 0)
					settings->searchStringType = Convert::ToInt32(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "SearchString") == 0)
					settings->searchString = testNavigator->Value;
				else if (String::Compare(testNavigator->Name, "CorruptionSearchType") == 0)
				{
					if (String::Compare(testNavigator->Value, "Replace") == 0)
						settings->type = CorruptionSearchType::SearchCorruptReplace;
					else if (String::Compare(testNavigator->Value, "LongStrings") == 0)
						settings->type = CorruptionSearchType::SearchCorruptLongStrings;
					else if (String::Compare(testNavigator->Value, "RepeatedSequence") == 0)
						settings->type = CorruptionSearchType::SearchCorruptRepeatedSequence;
				}
				else if (String::Compare(testNavigator->Name, "Length") == 0)
					settings->length = Convert::ToInt32(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "CorruptStringType") == 0)
					settings->corruptStringType = Convert::ToInt32(testNavigator->Value);
				else if (String::Compare(testNavigator->Name, "CorruptString") == 0)
					settings->corruptString = testNavigator->Value;
				else if (String::Compare(testNavigator->Name, "Overwrite") == 0)
					settings->overwrite = Convert::ToBoolean(testNavigator->Value);

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}
		}
	}
	
	//*************************************************************************
	// Method:		ParseCorruptionRegExprSettings
	// Description: reads a reg ex corruption settings node and makes an object
	//	out of it
	//
	// Parameters:
	//	navigator - the xml navigator object at the start of the element in the xml
	//	settings (out) - the object to get created
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileReader::ParseCorruptionRegExprSettings(XPathNavigator *navigator, CorruptionRegExprSettings *settings)
	{
		XPathNavigator *testNavigator = navigator->Clone();

		bool hasMoreAttributes = false;

		if (testNavigator->HasAttributes)
		{
			hasMoreAttributes = true;
			testNavigator->MoveToFirstAttribute();
			while (hasMoreAttributes)
			{
				if (String::Compare(testNavigator->Name, "SearchString") == 0)
					settings->searchString = testNavigator->Value;
				else if (String::Compare(testNavigator->Name, "ReplaceString") == 0)
					settings->corruptString = testNavigator->Value;

				hasMoreAttributes = testNavigator->MoveToNextAttribute();
				if (!hasMoreAttributes)
					testNavigator->MoveToParent();
			}
		}
	}
}