//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReportPaneDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class ReportPaneDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 8 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "ReportPaneDataArray.h"
#include "UserNotification.h"

namespace UIData
{

	//*************************************************************************
	// Method:		ReportPaneDataArray
	// Description: Constructor for the ReportPaneDataArray class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	ReportPaneDataArray::ReportPaneDataArray(HoloScriptRegistry *HoloScriptRegistry)
	{
		holoScriptRegistry = HoloScriptRegistry;
		processTable = new Hashtable();

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
			errorCodeDB->LoadDatabase(RegistryHelper::getInstance()->GetFunctionDatabaseFolderPath());

		ModifyReportHandler = new ModifyReportDelegate(this, onModifyReport);
		dataEventRegistry->OnModifyReport += ModifyReportHandler;

	}

	//*************************************************************************
	// Method:		ReportPaneDataArray
	// Description: Destructor for the ReportPaneDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ReportPaneDataArray::~ReportPaneDataArray()
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
	void ReportPaneDataArray::CleanUp()
	{
		dataEventRegistry->OnModifyReport -= ModifyReportHandler;
		DisplayableDataArray::CleanUp();
	}

	//*************************************************************************
	// Method:		DeleteReport
	// Description: Fills in dataarray and creates report
	//
	// Parameters:
	//	none
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::DeleteReport()
	{
		String *reportFilesFolder;

		try
		{
			if (reportPath->CompareTo(defaultReportPath) != 0)
			{
				reportFilesFolder = String::Concat(reportFolder, "\\");
				reportFilesFolder = String::Concat(reportFilesFolder, reportName->Substring(0, reportName->Length-4));
				reportFilesFolder = String::Concat(reportFilesFolder, "_files");
				File::Delete(reportPath);
				Directory::Delete(reportFilesFolder, true);
			}
			OnCloseProject();
		}
		catch (...)
		{
		}
	}

	//*************************************************************************
	// Method:		CreateReport
	// Description: Fills in dataarray and creates report
	//
	// Parameters:
	//	ReportPath - where the report is saved
	//	DataArrayList - list of data to use for report creation
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	bool ReportPaneDataArray::CreateReport(String *ReportFolder,SortedList *DataArrayList)
	{
		String *xmlPath;
		String *defaultReportFolder = REPORT_FOLDER_DEFAULT_VALUE;
		
		reportFolder = ReportFolder;
		set_ReportName(REPORT_FILE_NAME);

		defaultReportPath = GuiSupport::RegistryHelper::getInstance()->GetInstallationPath();
		defaultReportPath = String::Concat(defaultReportPath, defaultReportFolder);
		SaveReportFiles(defaultReportPath, REPORT_FILE_NAME, true);
		defaultReportPath = String::Concat(defaultReportPath, "\\", REPORT_FILE_NAME);

		dataArrayList = DataArrayList;			

		//log xml takes care of return value and error code reports as well
		xmlPath = String::Concat(reportFolder, LOGS_XML_FILE_NAME);
		ReportPaneDataNode *logReportDataNode = new ReportPaneDataNode("Logs", xmlPath, UIData::Report, 0, 0);
		CreateLogReportData(logReportDataNode);
		if (!CreateReportXML(logReportDataNode))
			return false;
		logReportDataNode->ID = logReportDataNode->Name;

		xmlPath = String::Concat(reportFolder, FAULTS_XML_FILE_NAME);
		ReportPaneDataNode *faultReportDataNode = new ReportPaneDataNode("Faults", xmlPath, UIData::Report, 0, 0);
		CreateFaultReportData(faultReportDataNode);
		if (!CreateReportXML(faultReportDataNode))
			return false;
		faultReportDataNode->ID = faultReportDataNode->Name;

		xmlPath = String::Concat(reportFolder, TESTS_XML_FILE_NAME);
		ReportPaneDataNode *testReportDataNode = new ReportPaneDataNode("Tests", xmlPath, UIData::Report, 0, 0);
		CreateTestReportData(testReportDataNode);
		if (!CreateReportXML(testReportDataNode))
			return false;
		testReportDataNode->ID = testReportDataNode->Name;

		//remove param report as this can take a very long time and lots of disk space
		/*xmlPath = String::Concat(reportFolder, PARAMS_XML_FILE_NAME);
		ReportPaneDataNode *paramValueReportDataNode = new ReportPaneDataNode("Parameter Values", xmlPath, UIData::Report, 0, 0);
		CreateParamValueReportData(paramValueReportDataNode);
		CreateReportXML(paramValueReportDataNode);
		paramValueReportDataNode->ID = paramValueReportDataNode->Name;*/

		xmlPath = String::Concat(reportFolder, RESOURCES_XML_FILE_NAME);
		ReportPaneDataNode *resourceDependenciesReportDataNode = new ReportPaneDataNode("Resource Dependencies", xmlPath, UIData::Report, 0, 0);
		CreateResourceDependencyReportData(resourceDependenciesReportDataNode);
		if (!CreateReportXML(resourceDependenciesReportDataNode))
			return false;
		resourceDependenciesReportDataNode->ID = resourceDependenciesReportDataNode->Name;


		xmlPath = String::Concat(reportFolder, USAGES_XML_FILE_NAME);
		ReportPaneDataNode *usageReportDataNode = new ReportPaneDataNode("Resource Usage", xmlPath, UIData::Report, 0, 0);
		CreateUsageReportData(usageReportDataNode);
		if (!CreateReportXML(usageReportDataNode))
			return false;
		usageReportDataNode->ID = usageReportDataNode->Name;

		delete logReportDataNode;
		delete faultReportDataNode;
		delete testReportDataNode;
		//delete paramValueReportDataNode;
		delete resourceDependenciesReportDataNode;
		delete usageReportDataNode;

		System::GC::Collect();

		return true;
	}

	//*************************************************************************
	// Method:		onModifyReport
	// Description: event handler for when a report is renamed
	//
	// Parameters:
	//	dataNode - original data
	//	modifiedDataNode - modified data
	//	propertyName - property that was modified
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::onModifyReport(DisplayableDataNode *dataNode, DisplayableDataNode *modifiedDataNode, String *propertyName)
	{
		//check to ensure this is the report being modified
		if ( (propertyName->CompareTo("Name") == 0) && (dataNode->Name->CompareTo(reportName->Substring(0, reportName->Length-4)) == 0) )
		{
			//check to see if the report name already exists
			if (ReportExists(modifiedDataNode->Name) )
			{
				HolodeckGui::UserNotification::InfoNotify("There is already a report with this name, please try again.");
				return;
			}

			set_ReportName(modifiedDataNode->Name);
			if (!dataNode->ModifiedDelegate)
				return;
			else
			{
				Object *args[] = __gc new Object*[3];
				args[0] = dataNode;
				args[1] = modifiedDataNode;
				args[2] = propertyName;
				dataNode->ModifiedDelegate->DynamicInvoke(args);

				//also modify id to ensure each report node is unique.
				modifiedDataNode->ID = modifiedDataNode->Name;
				propertyName = "ID";
				args[2] = propertyName;
				dataNode->ModifiedDelegate->DynamicInvoke(args);
			}
		}
	}

	//*************************************************************************
	// Method:		SetReportFolder
	// Description: Sets the report's folder and copies/moves report
	//
	// Parameters:
	//		newFolder - newFolder to Save to
	//		copy - boolean for copying or moving
	//
	// Return Value: none
	//*************************************************************************
	void ReportPaneDataArray::SetReportFolder(String *newFolder, bool copy)
	{
		String *oldReportFolder = reportFolder;
		String *oldReportName = reportName;
		reportFolder = newFolder; 
		reportPath = String::Concat(reportFolder, "\\");
		reportPath = String::Concat(reportPath, reportName);
		SaveReportFiles(oldReportFolder, oldReportName, copy);
	}

	//*************************************************************************
	// Method:		SaveReportFiles
	// Description: Saves the report to the current report path
	//
	// Parameters:
	//	oldReportFolder - path to old report folder
	//	oldReportPath - full path to old report file
	//	copyFiles - whether to copy or move files
	//
	// Return Value: none
	//*************************************************************************
	void ReportPaneDataArray::SaveReportFiles(String *oldReportFolder, String *oldReportName, bool copyFiles)
	{
		String *oldFilesFolderPath, *filesFolderPath;
		String *oldFilesFolderName, *filesFolderName;
		String *oldReportPath;
		String *files[];
		String *oldFile, *newFile;
		IEnumerator *enumerator;
		int index;
		bool move = true;

		//never move the original files
		if (oldReportName->CompareTo(REPORT_FILE_NAME) == 0)
			move = false;
		
		if ( (oldReportFolder) && (oldReportName) )
		{
			try
			{
				oldReportPath = String::Concat(oldReportFolder, "\\");
				oldReportPath = String::Concat(oldReportPath, oldReportName);
				oldFilesFolderName = oldReportName->Substring(0, oldReportName->Length - 4);
				oldFilesFolderName = String::Concat(oldFilesFolderName, "_files");
				oldFilesFolderPath = String::Concat(oldReportFolder, "\\");
				oldFilesFolderPath = String::Concat(oldFilesFolderPath, oldFilesFolderName);
				filesFolderName = reportName->Substring(0, reportName->Length - 4);
				filesFolderName = String::Concat(filesFolderName, "_files");
				filesFolderPath = String::Concat(reportFolder, "\\");
				filesFolderPath = String::Concat(filesFolderPath, filesFolderName);

				if (!Directory::Exists(reportFolder))
					Directory::CreateDirectory(reportFolder);
	
				if (move && !copyFiles)
				{
					File::Move(oldReportPath, reportPath);
					Directory::Move(oldFilesFolderPath, filesFolderPath);
				}
				else
				{
					File::Copy(oldReportPath, reportPath, true);
					File::SetAttributes(reportPath, FileAttributes::Normal);
					Directory::CreateDirectory(filesFolderPath);
					//files = Directory::GetFileSystemEntries(oldFilesFolderPath);
					files = Directory::GetFiles (oldFilesFolderPath);
					enumerator = files->GetEnumerator();
		
					oldFilesFolderPath = String::Concat(oldFilesFolderPath, "\\");
					filesFolderPath = String::Concat(filesFolderPath, "\\");
					while (enumerator->MoveNext())
					{
						try
						{
							oldFile = dynamic_cast <String*> (enumerator->Current);
							index = oldFile->LastIndexOf("\\");
							index++;
							oldFile = oldFile->Substring(index, oldFile->Length-index);
							newFile = String::Concat(filesFolderPath, oldFile);
							oldFile = String::Concat(oldFilesFolderPath, oldFile);
							File::Copy(oldFile, newFile, true);
							File::SetAttributes(newFile, FileAttributes::Normal);
						}
						catch (...)
						{
							//resume with copying other things
						}
					}
				}

				if (oldFilesFolderName->CompareTo(filesFolderName) != 0)
					ChangeInternalValues(reportPath, oldFilesFolderName, filesFolderName);
				
			}
			catch (...)
			{
				return;
			}
		}
	}

	//*************************************************************************
	// Method:		ChangeInternalValues
	// Description: Changes paths internal to the report
	//
	// Parameters:
	//	reportPath - path to the report
	//
	// Return Value: none
	//*************************************************************************
	void ReportPaneDataArray::ChangeInternalValues(String *reportPath, String *oldValue, String *newValue)
	{
		FileStream *file = File::Open(reportPath, FileMode::Open, FileAccess::Read);
		unsigned char fileText __gc[];
		unsigned char newText __gc[];
		StringBuilder *stringBuilderFileText = new StringBuilder(file->Length);
		String *stringFileText;
		IEnumerator *enumerator;
		int i;


		fileText = new unsigned char __gc[file->Length];
		file->Read(fileText, 0, file->Length);
		file->Close();
		enumerator = fileText->GetEnumerator();

		i=0;
		while (enumerator->MoveNext())
		{
			wchar_t temp = fileText[i];
			stringBuilderFileText->Append(temp, 1);
			i++;
		}

		stringBuilderFileText = stringBuilderFileText->Replace(oldValue, newValue);
		stringFileText = stringBuilderFileText->ToString();
		newText = new unsigned char __gc[stringFileText->Length];
		enumerator = stringFileText->GetEnumerator();

		i=0;
		while (enumerator->MoveNext())
		{
            newText[i] = stringFileText->get_Chars(i);
			i++;
		}

		file = File::Open(reportPath, FileMode::Create, FileAccess::Write);
		file->Write(newText, 0, newText->Count);
		file->Flush();
		file->Close();
	}

	//*************************************************************************
	// Method:		CreateLogReportData
	// Description: Fills in the data node that represents the log report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::CreateLogReportData(ReportPaneDataNode *reportDataNode)
	{
		LogPaneDataArray *dataArray;
		HoloScriptApplication *holoScriptApp;
		LogItem *logItem;
		ArrayList *row = new ArrayList();
		String *colName, *errorType, *errorCode;
		String *pID;
		unsigned long filePos;
		int count;
		ArrayList *parameters;

		reportDataNode->addColumn("Occurrence");
		reportDataNode->addColumn("Process");
		reportDataNode->addColumn("Function");
		reportDataNode->addColumn("Type");
		for (int i=0; i<MAX_PARAMETER_COLUMNS; i++)
		{
			colName = "Param";
			colName = String::Concat(colName, Convert::ToString(i+1));
			reportDataNode->addColumn(colName);
		}
		reportDataNode->addColumn("Return Value");
		reportDataNode->addColumn("Return Value Type");
		reportDataNode->addColumn("Error Code");
		reportDataNode->addColumn("Error Code Type");

		for (int arrayCounter=0; arrayCounter<dataArrayList->Count; arrayCounter++)
		{
			dataArray = dynamic_cast <LogPaneDataArray*> (dataArrayList->GetByIndex(arrayCounter));
			if ( (dataArray) && (dataArray->ThreadID == 0) )
			{
				holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataArray->ProcessID);
				
				if (!holoScriptApp)
					holoScriptApp = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(dataArray->ProcessID);

				if (holoScriptApp)
				{
					count = holoScriptApp->LogEntryCount;
					for (int i=0; i<count; i++)
					{
						row = new ArrayList();
						logItem = holoScriptApp->GetLogEntryAt(i, filePos);
						if (logItem)
						{
							row->Add(Convert::ToString(i+1));
							pID = Convert::ToString(dataArray->ProcessID);
							if (!processTable->Contains(pID))
								processTable->Add(pID, dataArray->ExecutableFileName);
							row->Add(ConstructProcessName(dataArray->ProcessID));
							row->Add(logItem->Function);
							row->Add(logItem->Category);
							parameters = logItem->Parameters;
							for (int y=0; y<MAX_PARAMETER_COLUMNS; y++)
							{
								if (y < parameters->Count)
									row->Add(parameters->Item[y]);
								else
									row->Add(Convert::ToString(""));
							}
							row->Add(logItem->ReturnValue);
							row->Add(GetReturnValueType(logItem->Function, logItem->ReturnValue));
							errorCode = errorCodeDB->GetStringFromCode(logItem->ErrorCode);
							row->Add(errorCode);
							if (errorCode->CompareTo("ERROR_SUCCESS") == 0)
								errorType = "Success";
							else
								errorType = "Fail";
							row->Add(errorType);
					
							reportDataNode->addRow(row);
						}
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		GetReturnValueType
	// Description: Given a return value return a string representing its type
	//
	// Parameters:
	//	functionName - the function that returned this value
	//	returnValue - the returnValue to look for
	//
	// Return Value: Success, Fail, Informational, Warning, or Unknown
	//*************************************************************************
	String *ReportPaneDataArray::GetReturnValueType(String *functionName, String *returnValue)
	{	
		int numSuccess = 0, numWarning = 0, numInformational = 0, numError = 0;
		InterceptedFunction *function = interceptedFunctionDB->GetFunctionByName(functionName);
		int intReturnValue;
		bool boolReturnValue;
		unsigned int uintReturnValue;

		if (function == NULL)
		{
			// Try the .NET database
			function = dotNetInterceptedFunctionDB->GetFunctionByName(functionName);

			if (function == NULL)
				return "Unknown";
		}


		try
		{
			if (function->ReturnType->ToLower()->Equals("int"))
			{
				intReturnValue = Convert::ToInt32(returnValue);
			}
			else if (function->ReturnType->ToLower()->Equals("bool"))
			{
				boolReturnValue = Convert::ToBoolean(returnValue);
				intReturnValue = (int) boolReturnValue;
			}
			else
			{
				uintReturnValue = Convert::ToUInt32(returnValue);
				intReturnValue = (int) uintReturnValue;
			}

			for (int i=0; i < function->ReturnValueCount; i++)
			{
				InterceptedFunctionReturnValue *functionReturnValue = dynamic_cast <InterceptedFunctionReturnValue*> (function->ReturnValues->Item[i]);
				if (functionReturnValue)
				{
					switch (functionReturnValue->Type)
					{
					case Success:
						numSuccess++;
						if (CompareValues(functionReturnValue->ReturnValue, intReturnValue, functionReturnValue->OperatorType) )
							return "Success";
						break;
					case Informational:
						numInformational++;
						if (CompareValues(functionReturnValue->ReturnValue, intReturnValue, functionReturnValue->OperatorType) )
							return "Informational";
						break;
					case Warning:
						numWarning++;
						if (CompareValues(functionReturnValue->ReturnValue, intReturnValue, functionReturnValue->OperatorType) )
							return "Warning";
						break;
					case Error:
						numError++;
						if (CompareValues(functionReturnValue->ReturnValue, intReturnValue, functionReturnValue->OperatorType) )
							return "Error";
						break;
					}
				}
			}

			if ((numSuccess == 0) && (numError > 0))
				return "Success";
			else if ((numSuccess > 0) && (numError == 0))
				return "Error";
			else
				return "Unknown";
		}
		catch(...)
		{
			return "Unknown";
		}
	}

	//*************************************************************************
	// Method:		CompareValues
	// Description: Compares two values and returns true if they match
	//
	// Parameters:
	//	origValue - original value to compare
	//	valueToCompare - value to compare against
	//	operatorType - type of compare to execute
	//
	// Return Value: true if the values match, false otherwise
	//*************************************************************************
	bool ReportPaneDataArray::CompareValues(int origValue, int valueToCompare, ReturnValueOperatorType operatorType)
	{
		switch (operatorType)
		{
		case EqualTo:
			return (valueToCompare == origValue);
		case GreaterThan:
			return (valueToCompare > origValue);
		case LessThan:
			return (valueToCompare < origValue);
		case GreaterThanOrEqualTo:
			return (valueToCompare >= origValue);
		case LessThanOrEqualTo:
			return (valueToCompare <= origValue);
		case NotEqualTo:
			return (valueToCompare != origValue);
		}
	}


	//*************************************************************************
	// Method:		CreateFaultReportData
	// Description: Fills in the data node that represents the fault report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::CreateFaultReportData(ReportPaneDataNode *reportDataNode)
	{
		FaultPaneDataArray *dataArray;
		DisplayableDataNode *dataNode;
		ArrayList *row = new ArrayList();
		String *type;

		reportDataNode->addColumn("Occurrence");
		reportDataNode->addColumn("Process");
		reportDataNode->addColumn("Name");
		reportDataNode->addColumn("Type");

		for (int arrayCounter=0; arrayCounter<dataArrayList->Count; arrayCounter++)
		{
			dataArray = dynamic_cast <FaultPaneDataArray*> (dataArrayList->GetByIndex(arrayCounter));
			if (dataArray)
			{
				for (int i=0; i<dataArray->Count; i++)
				{
					row = new ArrayList();
					dataNode = dynamic_cast <DisplayableDataNode*> (dataArray->GetDataNodeByIndex(i));
					if (dataNode)
					{
						row->Add(Convert::ToString(i+1));
						row->Add(ConstructProcessName(dataNode->processID));
						row->Add(dataNode->Name);
						switch (dataNode->Type)
						{
						case UIData::MemoryFault:
							type = "Memory Fault";
							break;
						case UIData::DiskFault:
							type = "Disk Fault";
							break;
						case UIData::NetworkFault:
							type = "Network Fault";
							break;
						case UIData::RegistryFault:
							type = "Registry Fault";
							break;
						case UIData::ProcessFault:
							type = "Process Fault";
							break;
						case UIData::CustomFault:
							type = "Custom Fault";
							break;
						}
						row->Add(type);
				
						reportDataNode->addRow(row);
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		CreateTestReportData
	// Description: Fills in the data node that represents the test report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::CreateTestReportData(ReportPaneDataNode *reportDataNode)
	{
		ScheduledTestDataArray *dataArray;
		ScheduledTestDataNode *dataNode;
		ArrayList *row = new ArrayList();
		ArrayList *matchParameters;
		String *colName, *errorType;
		String *pID;

		reportDataNode->addColumn("Occurrence");
		reportDataNode->addColumn("Process");
		reportDataNode->addColumn("Function");
		reportDataNode->addColumn("Type");
		for (int i=0; i<MAX_PARAMETER_COLUMNS; i++)
		{
			colName = "Param";
			colName = String::Concat(colName, Convert::ToString(i+1));
			reportDataNode->addColumn(colName);
		}
		reportDataNode->addColumn("Return Value");
		reportDataNode->addColumn("Error Code");
		reportDataNode->addColumn("Error Code Type");

		for (int arrayCounter=0; arrayCounter<dataArrayList->Count; arrayCounter++)
		{
			dataArray = dynamic_cast <ScheduledTestDataArray*> (dataArrayList->GetByIndex(arrayCounter));
			if (dataArray)
			{
				for (int i=0; i<dataArray->Count; i++)
				{
					row = new ArrayList();
					dataNode = dynamic_cast <ScheduledTestDataNode*> (dataArray->GetDataNodeByIndex(i));
					if (dataNode)
					{
						row->Add(Convert::ToString(i+1));
						pID = Convert::ToString(dataNode->processID);
						row->Add(ConstructProcessName(dataNode->processID));
						row->Add(dataNode->Function);
						row->Add(dataNode->Category);
						matchParameters = dataNode->MatchParameters;
						for (int i=0, j=0; i<MAX_PARAMETER_COLUMNS; i++, j++)
						{
							if (matchParameters && (j < matchParameters->Count))
							{
								InterceptedFunctionParameter *param = dynamic_cast <InterceptedFunctionParameter*> (matchParameters->get_Item(j));
								if (param->ID == i)
								{
									String * rowValue = "";
									for (int ptc = 0; ptc < param->TestCasesCount; ptc++)
									{
										ParamTestCase * ptCase = dynamic_cast <ParamTestCase *> (param->TestCase(ptc));
										rowValue = String::Concat (rowValue, ptCase->TestOperator, S" ", ptCase->TestValue);
										if (ptc != param->TestCasesCount - 1)
											rowValue = String::Concat (rowValue, S" OR ");
									}
									row->Add(rowValue);
								}
								else
								{
									row->Add(Convert::ToString(""));
									j--;
								}
							}
							else
								row->Add(Convert::ToString(""));
						}
						row->Add(dataNode->ReturnValue);
						row->Add(dataNode->ErrorCode);
						if (dataNode->ErrorCode->CompareTo("ERROR_SUCCESS") == 0)
							errorType = "Success";
						else
							errorType = "Fail";
						row->Add(errorType);
				
						reportDataNode->addRow(row);
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		CreateParamValReportData
	// Description: Fills in the data node that represents the parameter report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::CreateParamValueReportData(ReportPaneDataNode *reportDataNode)
	{
		LogPaneDataArray *dataArray;
		ArrayList *row = new ArrayList();
		unsigned long filePos;
		int count;
		ArrayList *parameters;
		HoloScriptApplication *holoScriptApp;
		LogItem *logItem;

		reportDataNode->addColumn("Occurrence");
		reportDataNode->addColumn("Process");
		reportDataNode->addColumn("Value");
		reportDataNode->addColumn("Position");
		reportDataNode->addColumn("Function");

		for (int arrayCounter=0; arrayCounter<dataArrayList->Count; arrayCounter++)
		{
			dataArray = dynamic_cast <LogPaneDataArray*> (dataArrayList->GetByIndex(arrayCounter));
			if (dataArray)
			{
				holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(dataArray->ProcessID);
	
				if (!holoScriptApp)
					holoScriptApp = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(dataArray->ProcessID);

				if (holoScriptApp)
				{
					count = holoScriptApp->LogEntryCount;
					for (int i=0; i<count; i++)
					{
						logItem = holoScriptApp->GetLogEntryAt(i, filePos);
						if (logItem)
						{
							parameters = logItem->Parameters;
							for (int y=0; y < parameters->Count; y++)
							{
								row = new ArrayList();
								row->Add(Convert::ToString(i+1));
								row->Add(ConstructProcessName(dataArray->ProcessID));
								row->Add(parameters->Item[y]);
								row->Add(Convert::ToString(y+1));
								row->Add(logItem->Function);
								reportDataNode->addRow(row);
							}
						}
					}
				}
			}
		}
	}

	//*************************************************************************
	// Method:		CreateResourceDependencyReportData
	// Description: Fills in the data node that represents the Resource Report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::CreateResourceDependencyReportData(ReportPaneDataNode *reportDataNode)
	{
		ResourcePaneDataArray *dataArray;
		ResourcePaneDataNode *dataNode;
		ArrayList *row = new ArrayList();
		String *type;

		reportDataNode->addColumn("Occurrence");
		reportDataNode->addColumn("Process");
		reportDataNode->addColumn("Name");
		reportDataNode->addColumn("Type");

		for (int arrayCounter=0; arrayCounter<dataArrayList->Count; arrayCounter++)
		{
			dataArray = dynamic_cast <ResourcePaneDataArray*> (dataArrayList->GetByIndex(arrayCounter));
			if (dataArray)
			{
				for (int i=0; i<dataArray->Count; i++)
				{
					dataNode = dynamic_cast <ResourcePaneDataNode*> (dataArray->GetDataNodeByIndex(i));
					if (dataNode)
					{
						//write out dups for each time this was hit, so we get an accurate count in the report
						for (int numHitCounter = 0; numHitCounter < dataNode->NumHits; numHitCounter++)
						{
							row = new ArrayList();
							row->Add(Convert::ToString(i+1));
							row->Add(ConstructProcessName(dataNode->processID));
							row->Add(dataNode->Name);
							switch (dataNode->Type)
							{
							case UIData::File:
								type = "Files and Folders";
								break;
							case UIData::RegistryEntry:
								type = "Registry Entries";
								break;
							case UIData::Process:
								type = "Processes and Libraries";
								break;
							case UIData::ComObject:
								type = "COM Objects";
								break;
							}
							row->Add(type);
							reportDataNode->addRow(row);
						}
					}
				}
			}
		}
	}	
	
	//*************************************************************************
	// Method:		CreateUsageReportData
	// Description: Fills in the data node that represents the Usage Report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	void ReportPaneDataArray::CreateUsageReportData(ReportPaneDataNode *reportDataNode)
	{
		LimitPaneDataArray *dataArray;
		ArrayList *row = new ArrayList();
		String *type;

		reportDataNode->addColumn("Occurrence");
		reportDataNode->addColumn("Process");
		reportDataNode->addColumn("Name");
		reportDataNode->addColumn("KiloBytes Used");

		for (int arrayCounter=0; arrayCounter<dataArrayList->Count; arrayCounter++)
		{
			dataArray = dynamic_cast <LimitPaneDataArray*> (dataArrayList->GetByIndex(arrayCounter));
			if (dataArray)
			{
				// avg and max for each limit type

				//disk
				row = new ArrayList();
				row->Add(Convert::ToString(1));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Average Disk Usage"));
				row->Add(Convert::ToString(dataArray->AverageDiskUsage/1024));
				reportDataNode->addRow(row);

				row = new ArrayList();
				row->Add(Convert::ToString(2));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Max Disk Usage"));
				row->Add(Convert::ToString(dataArray->MaxDiskUsage/1024));
				reportDataNode->addRow(row);
				
				//memory
				row = new ArrayList();
				row->Add(Convert::ToString(2));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Average Memory Usage"));
				row->Add(Convert::ToString(dataArray->AverageMemoryUsage/1024));
				reportDataNode->addRow(row);

				row = new ArrayList();
				row->Add(Convert::ToString(3));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Max Memory Usage"));
				row->Add(Convert::ToString(dataArray->MaxMemoryUsage/1024));
				reportDataNode->addRow(row);
				
				//Net Up
				row = new ArrayList();
				row->Add(Convert::ToString(4));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Average Network Upload Usage per second"));
				row->Add(Convert::ToString(dataArray->AverageNetUpUsage/1024));
				reportDataNode->addRow(row);

				row = new ArrayList();
				row->Add(Convert::ToString(5));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Max Network Upload Usage per second"));
				row->Add(Convert::ToString(dataArray->MaxNetUpUsage/1024));
				reportDataNode->addRow(row);
				
				//Net Down
				row = new ArrayList();
				row->Add(Convert::ToString(6));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Average Network Download Usage per second"));
				row->Add(Convert::ToString(dataArray->AverageNetDownUsage/1024));
				reportDataNode->addRow(row);

				row = new ArrayList();
				row->Add(Convert::ToString(7));
				row->Add(ConstructProcessName(dataArray->ProcessID));
				row->Add(Convert::ToString("Max Network Download Usage per second"));
				row->Add(Convert::ToString(dataArray->MaxNetDownUsage/1024));
				reportDataNode->addRow(row);
			}
		}
	}
		
	//*************************************************************************
	// Method:		CreateReportXML
	// Description: Creates an XML file that represents a report
	//
	// Parameters:
	//	None
	//
	// Return Value: true if succesfull, otherwise false
	//*************************************************************************
	bool ReportPaneDataArray::CreateReportXML(ReportPaneDataNode *reportDataNode)
	{
		XmlTextWriter *xWriter;

		try
		{
			xWriter = new XmlTextWriter(reportDataNode->XMLPath, System::Text::Encoding::UTF8);
		}
		catch(...)
		{
			String *msg = "You don't have sufficient permission to save this file: ";
			msg = String::Concat(msg, reportDataNode->XMLPath);
			HolodeckGui::UserNotification::ErrorNotify(msg);
			return false;
		}

		xWriter->Formatting = Formatting::Indented;
		xWriter->Indentation = 4;

		//write out root and namespace info
		xWriter->WriteStartElement("xml");
		xWriter->WriteAttributeString("xmlns", "x", NULL, "urn:schemas-microsoft-com:office:excel");
		xWriter->WriteAttributeString("xmlns", "dt", NULL, "uuid:C2F41010-65B3-11d1-A29F-00AA00C14882");
		xWriter->WriteAttributeString("xmlns", "s", NULL, "uuid:BDC6E3F0-6DA3-11d1-A2A3-00AA00C14882");
		xWriter->WriteAttributeString("xmlns", "rs", NULL, "urn:schemas-microsoft-com:rowset");
		xWriter->WriteAttributeString("xmlns", "z", NULL, "#RowsetSchema");

			xWriter->WriteStartElement("x", "PivotCache", NULL);
				xWriter->WriteStartElement("x", "CacheIndex", NULL);
					xWriter->WriteString("1");
				xWriter->WriteEndElement();	 //CacheIndex
			
				xWriter->WriteStartElement("s", "Schema", NULL);
				xWriter->WriteAttributeString("id", "RowsetSchema");
					xWriter->WriteStartElement("s", "ElementType", NULL);
					xWriter->WriteAttributeString("name", "row");
					xWriter->WriteAttributeString("content", "eltOnly");
						for (int i=1; i<=reportDataNode->ColumnCount; i++)
						{
							String *col = "Col";
							col = String::Concat(col, Convert::ToString(i));
							xWriter->WriteStartElement("s", "attribute", NULL);
							xWriter->WriteAttributeString("type", col);
							xWriter->WriteEndElement();  //attribute
						}
						xWriter->WriteStartElement("s", "extends", NULL);
						xWriter->WriteAttributeString("type", "rs:rowbase");
						xWriter->WriteEndElement();  //extends
					xWriter->WriteEndElement();  //ElementType

					for (int i=0; i<reportDataNode->ColumnCount; i++)
					{							
						String *col = "Col";
						col = String::Concat(col, Convert::ToString(i+1));
						xWriter->WriteStartElement("s", "AttributeType", NULL);
						xWriter->WriteAttributeString("name", col);
						xWriter->WriteAttributeString("rs", "name", NULL, dynamic_cast <String*> (reportDataNode->Columns->get_Item(i)));
							xWriter->WriteStartElement("s", "datatype", NULL);
							xWriter->WriteAttributeString("dt", "maxlength", NULL, "255");
							xWriter->WriteEndElement();  //datatype
						xWriter->WriteEndElement();  //AttributeType
					}
				xWriter->WriteEndElement();  //Schema

				xWriter->WriteStartElement("rs", "data", NULL);
				for (int x=0; x<reportDataNode->RowCount; x++)
				{
					xWriter->WriteStartElement("z", "row", NULL);
					ArrayList *row = dynamic_cast <ArrayList*> (reportDataNode->Rows->get_Item(x));
					for (int y=0; y<row->Count; y++)
					{
						String *col = "Col";
						col = String::Concat(col, Convert::ToString(y+1));
						xWriter->WriteAttributeString(col, XMLFilter(dynamic_cast <String*> (row->get_Item(y))));
					}
					xWriter->WriteEndElement();  //row
				}
				xWriter->WriteEndElement();  //data

			xWriter->WriteEndElement();  //PivotCache

		xWriter->WriteEndElement();  //xml

		xWriter->Flush();
		xWriter->Close();

		return true;
	}

	//*************************************************************************
	// Method:		XMLFilter
	// Description: Removes dangerous chars before they make it into the XML file
	//
	// Parameters:
	//	None
	//
	// Return Value: process name
	//*************************************************************************
	String *ReportPaneDataArray::XMLFilter(String *filterString)
	{
		StringBuilder *stringBuilder = new StringBuilder(filterString);

		for (int i=0; i<stringBuilder->Length; i++)
		{
			if (Char::IsControl(stringBuilder->Chars[i]))
			{
				stringBuilder->Remove(i, 1);
			}
			else if ( (stringBuilder->Chars[i] == '#') || (stringBuilder->Chars[i] == '<') || (stringBuilder->Chars[i] == '>') || (stringBuilder->Chars[i] == '&') ||
				(stringBuilder->Chars[i] == '\'') || (stringBuilder->Chars[i] == '"') )
			{
				stringBuilder->Remove(i, 1);
			}
		}
		
		filterString = stringBuilder->ToString();

		return filterString;
	}

	//*************************************************************************
	// Method:		ReportExists
	// Description: Checks to see if a report with the given name already exists
	//
	// Parameters:
	//	reportName - report name to check
	//
	// Return Value: true if it already exists, false if it doesn't
	//*************************************************************************
	bool ReportPaneDataArray::ReportExists(String *checkReportName)
	{
		return File::Exists(String::Concat(reportFolder, "\\", checkReportName, ".htm"));
	}

	//*************************************************************************
	// Method:		ConstructProcessName
	// Description: Creates a string to represent the process in pivot table
	//
	// Parameters:
	//	None
	//
	// Return Value: process name
	//*************************************************************************
	String *ReportPaneDataArray::ConstructProcessName(int processID)
	{
		String *pID = Convert::ToString(processID);
		String *fileName = dynamic_cast <String*> (processTable->get_Item(pID));

		fileName = String::Concat(fileName, " - ");
		fileName = String::Concat(fileName, pID);

		return fileName;
	}
}