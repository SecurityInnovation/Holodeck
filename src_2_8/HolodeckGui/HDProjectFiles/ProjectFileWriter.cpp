//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ProjectFileWriter.cpp
//
// DESCRIPTION: Contains implementation for the class ProjectFileWriter
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// Jan 07 2004		 B. Shirey	 File created.
//*************************************************************************
#include "ProjectFileWriter.h"

namespace UIData
{
	//*************************************************************************
	// Method:		ProjectFileWriter
	// Description: Constructor for the ProjectFileWriter class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectFileWriter::ProjectFileWriter()
	{
	}

	//*************************************************************************
	// Method:		~ProjectFileWriter
	// Description: Destructor for the ProjectFileWriter class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ProjectFileWriter::~ProjectFileWriter()
	{
	}

	//*************************************************************************
	// Method:		SaveProject
	// Description: Saves a project to a file
	//
	// Parameters:
	//	project - the project to save
	//	fileName - the file to save the project to
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ProjectFileWriter::SaveProject(Project *project, String *fileName)
	{
		currentProject = project;

		try
		{
			HoloScriptRegistry *hsReg;
			RegistryHelper *regHelper = RegistryHelper::getInstance();
			
			String *dtdName = regHelper->GetDTDFolder();
			if (dtdName == NULL)
				dtdName = "projectfile.dtd";
			else if (dtdName->EndsWith("\\"))
				dtdName = String::Concat(dtdName, "projectfile.dtd");
			else
				dtdName = String::Concat(dtdName, "\\projectfile.dtd");

			xmlWriter = new XmlTextWriter (currentProject->FullPath, System::Text::Encoding::UTF8);
			
			xmlWriter->Formatting = Formatting::Indented;
			xmlWriter->Indentation = 4;
			xmlWriter->WriteStartDocument(false);
			xmlWriter->WriteDocType("Project", NULL, dtdName->Trim(), NULL);

			xmlWriter->WriteStartElement("Project");
			if (currentProject->IsInPerThreadMode)
				xmlWriter->WriteAttributeString("PerThreadLogging", "True");
			else
				xmlWriter->WriteAttributeString("PerThreadLogging", "False");

			xmlWriter->WriteAttributeString("Version", currentProject->VersionString);
			
			hsReg = HoloScriptRegistry::getInstance();
			int NumProcesses = hsReg->Count;
			int NumTerminatedProcesses = hsReg->TerminatedCount;

			//Calculate the number of terminated apps which will be saved as part of the
			//project. Old instances of restarted apps should not be in the count.
			HoloScriptApplication * chk_hsApp = NULL;
			int TerminatedWillSaveCount = 0;
			for (int hsIndex = 0; hsIndex < NumTerminatedProcesses; hsIndex++)
			{
				if (hsReg->getTerminatedHoloScriptAppByIndex (hsIndex)->ShouldRestartWithProject)
					TerminatedWillSaveCount++;
			}

			xmlWriter->WriteStartElement("ApplicationsUnderTest");
			if (hsReg->ServicesExeApplication != NULL)
				xmlWriter->WriteAttributeString("Count", __box(NumProcesses + TerminatedWillSaveCount - 1)->ToString());
			else
				xmlWriter->WriteAttributeString("Count", __box(NumProcesses + TerminatedWillSaveCount)->ToString());
			
			for (int hsIndex = 0; hsIndex < NumProcesses; hsIndex++)
			{
				if (!SaveApplicationUnderTest(hsReg, hsIndex, false))
					return false;
			}
			
			for (int hsIndex = 0; hsIndex < NumTerminatedProcesses; hsIndex++)
			{
				//If stated hsApp is not marked to be restarted with project; then it
				//should not be saved either
				if (!hsReg->getTerminatedHoloScriptAppByIndex (hsIndex)->ShouldRestartWithProject)
					continue;
				
				if (!SaveApplicationUnderTest(hsReg, hsIndex, true))
					return false;
			}
			
			xmlWriter->WriteFullEndElement();		//End for ApplicationsUnderTest

			this->SaveReportsAndRecordedSessions();
			this->SaveSavedLogs();
			this->SaveSavedCorruptedFiles();
			this->SaveSavedExceptions();

			xmlWriter->WriteFullEndElement();		//End for Project
			xmlWriter->WriteEndDocument();	
			xmlWriter->Flush();
			xmlWriter->Close();
			return true;
		}
		catch(UnauthorizedAccessException*)
		{
			String *msg = "You don't have sufficient permission to save this file: ";
			msg = String::Concat(msg, currentProject->FullPath);
			UserNotification::ErrorNotify(msg);
			return false;
		}
		catch(SecurityException*)
		{
			String *msg = "You don't have sufficient permission to save this file: ";
			msg = String::Concat(msg, currentProject->FullPath);
			UserNotification::ErrorNotify(msg);
			return false;
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	//**************************************************************************
	// Method:		SaveApplicationUnderTest
	// Description: Saves the AUT
	//
	// Parameters:
	//		hsIndex - index in the array to save
	//		terminated - true if this is a terminated app
	//
	// Return Value: true if success, false otherwise
	//*************************************************************************
	bool ProjectFileWriter::SaveApplicationUnderTest(HoloScriptRegistry *hsReg, int hsIndex, bool terminated)
	{
		String *InjectMode;
		FaultPaneDataArray *faultArray;
		LimitPaneDataArray *limitArray;
		ScheduledTestDataArray * testArray;
		ResourceTestDataArray *resourceTestArray;
		LogPaneDataArray *logArray;
		NetworkCorruptionDataArray *networkCorruptionArray;
		FileCorruptionDataArray *fileCorruptionArray;
		HoloScriptApplication *hsApp;
		
		if (terminated)
			hsApp = hsReg->getTerminatedHoloScriptAppByIndex(hsIndex);
		else
			hsApp = hsReg->getHoloScriptAppByIndex(hsIndex);

		// don't save services.exe for service launch/attach
		if (hsApp == hsReg->ServicesExeApplication)
			return true;

		if (!hsApp->IsChainedProcess)
		{
			if (!hsApp->IsService)
			{
				if (hsApp->get_ApplicationAttachedTo())
					InjectMode = "ATTACH";
				else
					InjectMode = "LAUNCH";
			}
			else
			{
				if (hsApp->get_ApplicationAttachedTo())
					InjectMode = "ATTACHSERVICE";
				else
					InjectMode = "LAUNCHSERVICE";
			}

			UInt32 procId = -1;
			if (!(hsApp->GetApplicationProcessID(procId)))
			{
				UserNotification::ErrorNotify("There was an internal error, project could not be saved");
				return false;
			}

			//get the data arrays for the AUT
			DisplayableDataNode *dataNode = new DisplayableDataNode("", UIData::Faults, procId, 0);
			faultArray = dynamic_cast <FaultPaneDataArray *>(currentProject->GetDataArray(dataNode));
			dataNode = new DisplayableDataNode("", UIData::Limits, procId, 0);
			limitArray = dynamic_cast <LimitPaneDataArray *>(currentProject->GetDataArray(dataNode));
			dataNode = new DisplayableDataNode("", UIData::ActiveLogPane, procId, 0);
			logArray = dynamic_cast <LogPaneDataArray *>(currentProject->GetDataArray(dataNode));
			dataNode = new DisplayableDataNode("", UIData::Tests, procId, 0);
			testArray = dynamic_cast <ScheduledTestDataArray *>(currentProject->GetDataArray(dataNode));
			dataNode = new DisplayableDataNode("", UIData::ResourceFaults, procId, 0);
			resourceTestArray = dynamic_cast <ResourceTestDataArray *>(currentProject->GetDataArray(dataNode));
			dataNode = new DisplayableDataNode("", UIData::NetworkCorruptionFault, procId, 0);
			networkCorruptionArray = dynamic_cast <NetworkCorruptionDataArray *>(currentProject->GetDataArray(dataNode));
			dataNode = new DisplayableDataNode("", UIData::FileCorruptionFault, procId, 0);
			fileCorruptionArray = dynamic_cast <FileCorruptionDataArray *>(currentProject->GetDataArray(dataNode));

			xmlWriter->WriteStartElement("ApplicationUnderTest");
			
			this->SaveApplicationInfo(hsApp, InjectMode, procId);
			this->SaveFilteredFunctions(hsApp);
			this->SaveFaults(faultArray);
			this->SaveLimits(limitArray);
			this->SaveScheduledTests(testArray);
			this->SaveResourceTests(resourceTestArray);
			this->SaveNetworkCorruptionFaults(networkCorruptionArray);
			this->SaveFileCorruptionFaults(fileCorruptionArray);
			this->SaveActiveLogInfo(hsApp, logArray);

			xmlWriter->WriteFullEndElement();	//End for ApplicationUnderTest	
		}
		return true;
	}

	//**************************************************************************
	// Method:		SaveApplicationInfo
	// Description: Saves the info for the AUT
	//
	// Parameters:
	//		hsApp - HoloScriptApplication object for the AUT
	//		InjectMode - inject mode (launch/Attach) of the AUT
	//		procId - process id for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveApplicationInfo(HoloScriptApplication *hsApp, String *InjectMode, UInt32 procId)
	{
		xmlWriter->WriteStartElement("AppInfo");

		if (hsApp->IsService)
			xmlWriter->WriteAttributeString("ApplicationPathAndName", hsApp->ServiceName);
		else
			xmlWriter->WriteAttributeString("ApplicationPathAndName", hsApp->get_ApplicationPath());
		
		xmlWriter->WriteAttributeString("InjectMode", InjectMode);
		
		if (hsApp->get_PauseOnStart())
			xmlWriter->WriteAttributeString("StartPaused", "True");
		else
			xmlWriter->WriteAttributeString("StartPaused", "False");
					
		xmlWriter->WriteAttributeString("CommandLineParams", hsApp->get_CommandLineParams());

		if (hsApp->get_ProcessChaining())
			xmlWriter->WriteAttributeString("ProcessChaining", "True");
		else
            xmlWriter->WriteAttributeString("ProcessChaining", "False");

		if (hsApp->AttachDebugger)
			xmlWriter->WriteAttributeString("AttachDebugger", "True");
		else
			xmlWriter->WriteAttributeString("AttachDebugger", "False");

		if (hsApp->LogFirstChance)
			xmlWriter->WriteAttributeString("LogFirstChance", "True");
		else
			xmlWriter->WriteAttributeString("LogFirstChance", "False");

		if (hsApp->get_InheritSettings())
			xmlWriter->WriteAttributeString("InheritSettings", "True");
		else
            xmlWriter->WriteAttributeString("InheritSettings", "False");

		xmlWriter->WriteFullEndElement();   //End for AppInfo
	}

	//**************************************************************************
	// Method:		SaveFilteredFunctions
	// Description: Saves the filtered out functions for the AUT
	//
	// Parameters:
	//		hsApp - HoloScriptApplication object for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveFilteredFunctions(HoloScriptApplication *hsApp)
	{
		ArrayList * filteredFuncs = hsApp->get_FilteredFunctionList();

		xmlWriter->WriteStartElement("FilteredFunctions");
		
		int filtFuncIndex = 0;
		if (filteredFuncs != NULL)
		{
			IEnumerator *enumerator = filteredFuncs->GetEnumerator();
			
			while (enumerator->MoveNext())
			{					
				String *functionName = dynamic_cast<String *>(enumerator->Current);
				xmlWriter->WriteStartElement("FilteredFunction");
				xmlWriter->WriteAttributeString("Index", __box(filtFuncIndex)->ToString());
				xmlWriter->WriteString(functionName);
				xmlWriter->WriteFullEndElement(); //End for FilteredFunction
				filtFuncIndex++;
			}
		}
		xmlWriter->WriteFullEndElement();   //End for FilteredFunctions
	}

	//**************************************************************************
	// Method:		SaveFaults
	// Description: Saves the faults for the AUT
	//
	// Parameters:
	//		faultArray - FaultPaneDataArray for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveFaults(FaultPaneDataArray *faultArray)
	{
		//Faults
		ArrayList *diskFaults = new ArrayList();
		ArrayList *memoryFaults = new ArrayList();
		ArrayList *networkFaults = new ArrayList();
		ArrayList *registryFaults = new ArrayList();
		ArrayList *processFaults = new ArrayList();
		ArrayList *customFaults = new ArrayList();
		
		if(faultArray != NULL)
		{
			for (int faultCount = 0; faultCount < faultArray->get_Count(); faultCount++)
			{
				
				DisplayableDataNode * faultDataNode = faultArray->GetDataNodeByIndex(faultCount);
				if (faultDataNode != NULL)
				{
					switch (faultDataNode->get_Type())
					{
					case UIData::DiskFault:
						diskFaults->Add(faultDataNode);
						break;
					case UIData::MemoryFault:
						memoryFaults->Add(faultDataNode);
						break;
					case UIData::NetworkFault:
						networkFaults->Add(faultDataNode);
						break;
					case UIData::RegistryFault:
						registryFaults->Add(faultDataNode);
						break;
					case UIData::ProcessFault:
						processFaults->Add(faultDataNode);
						break;
					case UIData::CustomFault:
						customFaults->Add(faultDataNode);
						break;
					}

				}
			}
		}
		xmlWriter->WriteStartElement("Faults");
		//DiskFaults
		xmlWriter->WriteStartElement("DiskFaults");
		if (diskFaults != NULL)
		{
			IEnumerator *enumerator = diskFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				DisplayableDataNode * fault = dynamic_cast <DisplayableDataNode *> (enumerator->Current);
				xmlWriter->WriteStartElement("DiskFault");
				xmlWriter->WriteAttributeString("Name", fault->get_Name());
				xmlWriter->WriteFullEndElement(); //End for DiskFault
			}
		}
		xmlWriter->WriteFullEndElement(); //End for DiskFaults

		//MemoryFaults
		xmlWriter->WriteStartElement("MemoryFaults");
		if (memoryFaults != NULL)
		{
			IEnumerator *enumerator = memoryFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				DisplayableDataNode * fault = dynamic_cast <DisplayableDataNode *> (enumerator->Current);
				xmlWriter->WriteStartElement("MemoryFault");
				xmlWriter->WriteAttributeString("Name", fault->get_Name());
				xmlWriter->WriteFullEndElement(); //End for MemoryFault
			}
		}
		xmlWriter->WriteFullEndElement(); //End for MemoryFaults
		
		//NetworkFaults
		xmlWriter->WriteStartElement("NetworkFaults");
		if (networkFaults != NULL)
		{
			IEnumerator *enumerator = networkFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				DisplayableDataNode * fault = dynamic_cast <DisplayableDataNode *> (enumerator->Current);
				xmlWriter->WriteStartElement("NetworkFault");
				xmlWriter->WriteAttributeString("Name", fault->get_Name());
				xmlWriter->WriteFullEndElement(); //End for NetworkFault
			}
		}
		xmlWriter->WriteFullEndElement(); //End for NetworkFaults

		//RegistryFaults
		xmlWriter->WriteStartElement("RegistryFaults");
		if (registryFaults != NULL)
		{
			IEnumerator *enumerator = registryFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				DisplayableDataNode * fault = dynamic_cast <DisplayableDataNode *> (enumerator->Current);
				xmlWriter->WriteStartElement("RegistryFault");
				xmlWriter->WriteAttributeString("Name", fault->get_Name());
				xmlWriter->WriteFullEndElement(); //End for RegistryFault
			}
		}
		xmlWriter->WriteFullEndElement(); //End for RegistryFaults

		//ProcessFaults
		xmlWriter->WriteStartElement("ProcessFaults");
		if (processFaults != NULL)
		{
			IEnumerator *enumerator = processFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				DisplayableDataNode * fault = dynamic_cast <DisplayableDataNode *> (enumerator->Current);
				xmlWriter->WriteStartElement("ProcessFault");
				xmlWriter->WriteAttributeString("Name", fault->get_Name());
				xmlWriter->WriteFullEndElement(); //End for ProcessFault
			}
		}
		xmlWriter->WriteFullEndElement(); //End for ProcessFaults

		//CustomFaults
		xmlWriter->WriteStartElement("CustomFaults");
		if (networkFaults != NULL)
		{
			IEnumerator *enumerator = customFaults->GetEnumerator();
			while (enumerator->MoveNext())
			{
				DisplayableDataNode * fault = dynamic_cast <DisplayableDataNode *> (enumerator->Current);
				xmlWriter->WriteStartElement("CustomFault");
				xmlWriter->WriteAttributeString("Name", fault->get_Name());
				xmlWriter->WriteFullEndElement(); //End for CustomFault
			}
		}
		xmlWriter->WriteFullEndElement(); //End for CustomFaults

		xmlWriter->WriteFullEndElement();	//End for Faults
	}

	//**************************************************************************
	// Method:		SaveResourceTests
	// Description: Saves the resource tests for the AUT
	//
	// Parameters:
	//		resourceTestArray - ResourceTestDataArray for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveResourceTests(ResourceTestDataArray *resourceTestArray)
	{
		xmlWriter->WriteStartElement("ResourceTests");
		for(int testNum = 0; resourceTestArray && (testNum < resourceTestArray->Count); testNum++)
		{
			ResourceTestDataNode * resourceTestNode = dynamic_cast <ResourceTestDataNode*> (resourceTestArray->GetDataNodeByIndex(testNum));
			xmlWriter->WriteStartElement("ResourceTest");
			xmlWriter->WriteAttributeString("Enabled", resourceTestNode->IsTestOn ? "true" : "false");
			xmlWriter->WriteAttributeString("FaultName", resourceTestNode->FaultName);
			xmlWriter->WriteAttributeString("ResourceName", resourceTestNode->Name);
			xmlWriter->WriteAttributeString("FaultType", Convert::ToString(resourceTestNode->FaultType));
			xmlWriter->WriteFullEndElement();   //End for Resource Test
		}
		xmlWriter->WriteFullEndElement();   //End for Resource Tests
	}

	//**************************************************************************
	// Method:		SaveLimits
	// Description: Saves the current AUT's limit settings
	//
	// Parameters:
	//		limitArray - limit pane data array
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveLimits(LimitPaneDataArray *limitArray)
	{
		//bugbug - Limit units not available, using Bytes
		xmlWriter->WriteStartElement("Limits");

		if (limitArray != NULL)
		{
			for (int limitCount = 0; limitCount < limitArray->Count; limitCount++)
			{
				LimitPaneDataNode *limitDataNode = dynamic_cast <LimitPaneDataNode*> (limitArray->GetDataNodeByIndex(limitCount));
				if (limitDataNode != NULL)
				{
					switch (limitDataNode->Type)
					{
					case DiskLimit:
						xmlWriter->WriteStartElement("DiskSpaceLimit");
						break;
					case MemoryLimit:
						xmlWriter->WriteStartElement("MemorySpaceLimit");
						break;
					case NetworkDownLimit:
						xmlWriter->WriteStartElement("NetworkDownLimit");
						break;
					case NetworkUpLimit:
						xmlWriter->WriteStartElement("NetworkUpLimit");
						break;
					}
					xmlWriter->WriteAttributeString("LimitActivated", "True");
					xmlWriter->WriteAttributeString("Limit", Convert::ToString(limitDataNode->LimitValue));
					xmlWriter->WriteAttributeString("LimitUnit", "B");
					xmlWriter->WriteFullEndElement();   //End for Limit
				}
			}
		}
		xmlWriter->WriteFullEndElement();   //End for Limits
	}

	//**************************************************************************
	// Method:		SaveNetworkCorruptionFaults
	// Description: Saves the network corruption faults for the AUT
	//
	// Parameters:
	//		testArray - NetworkCorruptionDataArray for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveNetworkCorruptionFaults(NetworkCorruptionDataArray *testArray)
	{
		xmlWriter->WriteStartElement("NetworkCorruptionFaults");
		for(int testNum = 0; testArray && (testNum < testArray->Count); testNum++)
		{
			NetworkCorruptionDataNode *testNode = dynamic_cast <NetworkCorruptionDataNode*> (testArray->GetDataNodeByIndex(testNum));
			xmlWriter->WriteStartElement("NetworkCorruption");
			xmlWriter->WriteAttributeString("Enabled", testNode->IsTestOn ? "true" : "false");
			xmlWriter->WriteAttributeString("SentData", Convert::ToString(testNode->SentData));
			xmlWriter->WriteAttributeString("ReceivedData", Convert::ToString(testNode->ReceivedData));
			xmlWriter->WriteAttributeString("PortString", testNode->PortString);
			xmlWriter->WriteAttributeString("CorruptType", testNode->CorruptType);
			xmlWriter->WriteAttributeString("SearchFor", testNode->SearchFor);
			xmlWriter->WriteAttributeString("SearchRegularExpression", testNode->SearchRegularExpression);
			xmlWriter->WriteAttributeString("ReplaceWith", testNode->ReplaceWith);
			xmlWriter->WriteAttributeString("ReplacementString", testNode->ReplacementString);
			ArrayList * portList = testNode->PortList;
			xmlWriter->WriteStartElement("PortList");
			if (portList != NULL)
			{
				IEnumerator * portListEnumerator = portList->GetEnumerator();
				while(portListEnumerator->MoveNext())
				{
					String *port = Convert::ToString(portListEnumerator->Current);
					if (port)
					{
						xmlWriter->WriteStartElement("Port");
						xmlWriter->WriteAttributeString("Name", port);
						xmlWriter->WriteFullEndElement();   //End for Port
					}
				}
			}
			xmlWriter->WriteFullEndElement();   //End for PortList

			xmlWriter->WriteStartElement("NetworkCorruptionSettings");
			xmlWriter->WriteAttributeString("Type", testNode->UISettings.type);
			SaveCorruptionRandomSettings(testNode->UISettings.randomSettings);
			SaveCorruptionSearchSettings(testNode->UISettings.searchSettings);
			SaveCorruptionRegExprSettings(testNode->UISettings.regExprSettings);
			xmlWriter->WriteFullEndElement();	// End for network corruption settings

			xmlWriter->WriteFullEndElement();   //End for NetworkCorruption
		}
		xmlWriter->WriteFullEndElement();   //End for NetworkCorruptionFault
	}

	//**************************************************************************
	// Method:		SaveFileCorruptionFaults
	// Description: Saves the file corruption faults for the AUT
	//
	// Parameters:
	//		testArray - FileCorruptionDataArray for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveFileCorruptionFaults(FileCorruptionDataArray *testArray)
	{
		xmlWriter->WriteStartElement("FileCorruptionFaults");
		for(int testNum = 0; testArray && (testNum < testArray->Count); testNum++)
		{
			FileCorruptionDataNode *testNode = dynamic_cast <FileCorruptionDataNode*> (testArray->GetDataNodeByIndex(testNum));
			xmlWriter->WriteStartElement("FileCorruption");
			xmlWriter->WriteAttributeString("Enabled", testNode->IsTestOn ? "true" : "false");
			xmlWriter->WriteAttributeString("OriginalFile", testNode->OriginalFile);
			xmlWriter->WriteAttributeString("Regenerate", testNode->Regenerate ? "True" : "False");
			xmlWriter->WriteAttributeString("CorruptType", testNode->CorruptType);
			xmlWriter->WriteAttributeString("SearchFor", testNode->SearchFor);
			xmlWriter->WriteAttributeString("SearchRegularExpression", testNode->SearchRegularExpression);
			xmlWriter->WriteAttributeString("ReplaceWith", testNode->ReplaceWith);
			xmlWriter->WriteAttributeString("ReplacementString", testNode->ReplacementString);

			xmlWriter->WriteStartElement("FileCorruptionSettings");
			xmlWriter->WriteAttributeString("Type", testNode->UISettings.type);
			xmlWriter->WriteAttributeString("Regen", testNode->UISettings.regen.ToString());
			SaveCorruptionRandomSettings(testNode->UISettings.randomSettings);
			SaveCorruptionSearchSettings(testNode->UISettings.searchSettings);
			SaveCorruptionRegExprSettings(testNode->UISettings.regExprSettings);
			xmlWriter->WriteFullEndElement();	// End for network corruption settings
			xmlWriter->WriteFullEndElement();   //End for FileCorruption
		}
		xmlWriter->WriteFullEndElement();   //End for FileCorruptionFault
	}

	//**************************************************************************
	// Method:		SaveScheduledTests
	// Description: Saves the scheduled tests for the AUT
	//
	// Parameters:
	//		testArray - ScheduledTestDataArray for the AUT
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveScheduledTests(ScheduledTestDataArray *testArray)
	{
		xmlWriter->WriteStartElement("ScheduledTests");
		for(int testNum = 0; testArray && (testNum < testArray->Count); testNum++)
		{
			ScheduledTestDataNode * testNode = dynamic_cast <ScheduledTestDataNode*> (testArray->GetDataNodeByIndex(testNum));
			xmlWriter->WriteStartElement("ScheduledTest");
			xmlWriter->WriteAttributeString("Enabled", testNode->IsTestOn ? "true" : "false");
			xmlWriter->WriteAttributeString("TestID", testNode->get_TestID());
			xmlWriter->WriteAttributeString("FunctionName", testNode->get_Function());
			xmlWriter->WriteAttributeString("Category", testNode->get_Category());
			xmlWriter->WriteAttributeString("SpecifyErrorCode", testNode->get_SpecifyErrorCode() ? "true" : "false");
			xmlWriter->WriteAttributeString("ErrorCode", testNode->get_ErrorCode());
			xmlWriter->WriteAttributeString("ReturnValue", testNode->get_ReturnValue());
			xmlWriter->WriteAttributeString("Owner", testNode->get_TestOwner());
			ArrayList * matchParams = testNode->get_MatchParameters();
			xmlWriter->WriteStartElement("MatchParams");
			if (matchParams != NULL)
			{
				IEnumerator * matchParamEnumerator = matchParams->GetEnumerator();
				while(matchParamEnumerator->MoveNext())
				{
					InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(matchParamEnumerator->Current);
					if (param)
					{
						for (int wmp=0; wmp < param->TestCasesCount; wmp++)
						{
							ParamTestCase * ptc = param->TestCase(wmp);
							xmlWriter->WriteStartElement("MatchParam");
							xmlWriter->WriteAttributeString("Name", param->Name);
							xmlWriter->WriteAttributeString("TestOperator", ptc->TestOperator);
							xmlWriter->WriteAttributeString("TestValue", ptc->TestValue);
							xmlWriter->WriteAttributeString("CompareAsType", (param->CompareAsType).ToString());
							xmlWriter->WriteAttributeString("ID", (param->ID).ToString());
							xmlWriter->WriteFullEndElement();   //End for MatchParam
						}
					}
				}
			}
			xmlWriter->WriteFullEndElement();   //End for MatchParams
			ArrayList * changeParams = testNode->get_ChangeParameters();
			xmlWriter->WriteStartElement("ChangeParams");
			if (changeParams != NULL)
			{
				IEnumerator * changeParamEnumerator = changeParams->GetEnumerator();
				while(changeParamEnumerator->MoveNext())
				{
					InterceptedFunctionParameter *param = dynamic_cast<InterceptedFunctionParameter *>(changeParamEnumerator->Current);
					if (param)
					{
						xmlWriter->WriteStartElement("ChangeParam");
						xmlWriter->WriteAttributeString("Name", param->Name);
						xmlWriter->WriteAttributeString("TestValue", param->ChangeValue);
						xmlWriter->WriteAttributeString("CompareAsType", (param->CompareAsType).ToString());
						xmlWriter->WriteAttributeString("ID", (param->ID).ToString());
						xmlWriter->WriteFullEndElement();   //End for ChangeParam
					}
				}
			}
			xmlWriter->WriteFullEndElement();   //End for ChangeParams

			//Write Firing Specification
			xmlWriter->WriteStartElement("FiringSpecification");
			xmlWriter->WriteAttributeString("FiringOption", testNode->FiringOption.ToString());
			xmlWriter->WriteAttributeString("PauseAUTAfterTest", testNode->PauseAppOnTestFire? "True":"False");
			xmlWriter->WriteStartElement("FiringDetails");
			
			if (testNode->FiringOption == 1)
			{
				String * fData = dynamic_cast <String *> (testNode->FiringDetails);
				xmlWriter->WriteStartElement("FiringData");
				xmlWriter->WriteAttributeString("Data", fData);
				xmlWriter->WriteFullEndElement();   //End for FiringData
			}
			else
			{
				ArrayList * fDArray = dynamic_cast <ArrayList *> (testNode->FiringDetails);
				for (int fdc=0; fdc < fDArray->Count; fdc++)
				{
					String * fData = dynamic_cast <String *> (fDArray->Item[fdc]);
					xmlWriter->WriteStartElement("FiringData");
					xmlWriter->WriteAttributeString("Data", fData);
					xmlWriter->WriteFullEndElement();   //End for FiringData
				}
			}

			xmlWriter->WriteFullEndElement();   //End for FiringDetails
			xmlWriter->WriteFullEndElement();   //End for FiringSpecification

			xmlWriter->WriteFullEndElement();   //End for Scheduled Test
		}
		xmlWriter->WriteFullEndElement();   //End for Scheduled Tests
	}

	//**************************************************************************
	// Method:		SaveActiveLogInfo
	// Description: Saves the log and filters for the log
	//
	// Parameters:
	//		hsApp - HoloScriptApplication object for the AUT
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	void ProjectFileWriter::SaveActiveLogInfo(HoloScriptApplication *hsApp, LogPaneDataArray *logArray)
	{
		String *logsFolder = String::Concat(currentProject->Folder, "\\", currentProject->Title, "_Logs");
		if (!Directory::Exists(logsFolder))
			Directory::CreateDirectory(logsFolder);

		String *logFilePath = hsApp->GetLogFileName();
		int index = logFilePath->LastIndexOf("\\");
		index++;
		String *logFileName = logFilePath->Substring(index, logFilePath->Length-index);
		String *savedLogFilePath = String::Concat(logsFolder, "\\", logFileName);
		File::Copy(logFilePath, savedLogFilePath, true);

		xmlWriter->WriteStartElement("Log");
		xmlWriter->WriteAttributeString("FilePath", String::Concat (currentProject->Title, "_Logs\\", Path::GetFileName(logFileName)));
		DateTime currentTime = DateTime::Now;
		String * saveTime = String::Concat(currentTime.ToShortDateString(), " ", currentTime.ToLongTimeString());
		saveTime = String::Concat(hsApp->ApplicationName, "&", saveTime);
		xmlWriter->WriteAttributeString("ExeNameAndFileTime", saveTime);

		//not implemented for now
		xmlWriter->WriteStartElement("LogFilters");
		xmlWriter->WriteFullEndElement();	//End for LogFilters

		xmlWriter->WriteFullEndElement();	//End for Log		
	}

	//**************************************************************************
	// Method:		SaveReportsAndRecordedSessions
	// Description: Saves the reports and recorded sessions
	//
	// Parameters:
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveReportsAndRecordedSessions()
	{
		ArrayList * reportsList = new ArrayList();
		ArrayList * recSessionsList = new ArrayList();

		for (int i=0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray *dataArray = currentProject->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("RecordedSessionDataArray") == 0)
			{
				RecordedSessionDataArray * recSessionArray = dynamic_cast <RecordedSessionDataArray *> (dataArray);
				recSessionsList->Add(recSessionArray);
			}
			else if (type->Name->CompareTo("ReportPaneDataArray") == 0)
			{
				ReportPaneDataArray * reportArray = dynamic_cast <ReportPaneDataArray *> (dataArray);
				reportsList->Add(reportArray);
			}
		}
		
		xmlWriter->WriteStartElement("Reports");
		if (reportsList != NULL)
		{
			IEnumerator * enumerator = reportsList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ReportPaneDataArray * reportInfo = dynamic_cast <ReportPaneDataArray *> (enumerator->Current);
				reportInfo->SetReportFolder((String::Concat(currentProject->Folder, "\\", currentProject->Title, "_Reports")), true);

				xmlWriter->WriteStartElement("Report");
				xmlWriter->WriteAttributeString("FilePath", String::Concat (currentProject->Title, "_Reports\\", Path::GetFileName (reportInfo->get_ReportPath())));
				xmlWriter->WriteString(reportInfo->get_ReportName());
				xmlWriter->WriteFullEndElement();		//End for Report
			}
		}
		xmlWriter->WriteFullEndElement();		//End for Reports

		xmlWriter->WriteStartElement("RecordedSessions");
		if (recSessionsList != NULL)
		{
			IEnumerator * enumerator = recSessionsList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				RecordedSessionDataArray * recSessionInfo = dynamic_cast <RecordedSessionDataArray *> (enumerator->Current);
				recSessionInfo->SetSessionFolder((String::Concat(currentProject->Folder, "\\", currentProject->Title, "_Recorded Sessions")), true);

				xmlWriter->WriteStartElement("RecordedSession");
				xmlWriter->WriteAttributeString("FilePath", String::Concat (currentProject->Title, "_Recorded Sessions\\", Path::GetFileName (recSessionInfo->get_SessionPath())));
				xmlWriter->WriteString(recSessionInfo->get_Name());
				xmlWriter->WriteFullEndElement();		//End for RecordedSession
			}
		}
		xmlWriter->WriteFullEndElement();		//End for RecordedSessions
	}

	//**************************************************************************
	// Method:		TransferReportsAndRecSessions
	// Description: moves reports and recorded sessions folders to new project folder
	//
	// Parameters:
	//		project - the project to transfer them for
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::TransferReportsAndRecordedSessions(Project *project)
	{
		ArrayList * reportsList = new ArrayList();
		ArrayList * recSessionsList = new ArrayList();

		for (int i=0; i < project->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray * dataArray = project->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("RecordedSessionDataArray") == 0)
			{
				RecordedSessionDataArray * recSessionArray = dynamic_cast <RecordedSessionDataArray *> (dataArray);
				recSessionsList->Add(recSessionArray);
			}
			else if (type->Name->CompareTo("ReportPaneDataArray") == 0)
			{
				ReportPaneDataArray * reportArray = dynamic_cast <ReportPaneDataArray *> (dataArray);
				reportsList->Add(reportArray);
			}
		}

		if (reportsList->Count > 0)
		{
			String *reportsFolder = String::Concat(project->Folder, "\\", project->Title, "_Reports");
			if (Directory::Exists(reportsFolder))
				Directory::Delete(reportsFolder, true);
			Directory::CreateDirectory(reportsFolder);

			IEnumerator * enumerator = reportsList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				ReportPaneDataArray * reportInfo = dynamic_cast <ReportPaneDataArray *> (enumerator->Current);
				reportInfo->SetReportFolder(reportsFolder, true);
			}
		}

		if (recSessionsList->Count > 0)
		{			
			String *recSessionsFolder = String::Concat(project->Folder, "\\", project->Title, "_Recorded Sessions");
			if (Directory::Exists(recSessionsFolder))
				Directory::Delete(recSessionsFolder, true);
			Directory::CreateDirectory(recSessionsFolder);

			IEnumerator * enumerator = recSessionsList->GetEnumerator();
			while (enumerator->MoveNext())
			{
				RecordedSessionDataArray * recSessionInfo = dynamic_cast <RecordedSessionDataArray *> (enumerator->Current);
				recSessionInfo->SetSessionFolder(recSessionsFolder, true);
			}
		}
	}

	//**************************************************************************
	// Method:		TransferSavedLogs
	// Description: moves logs folder to new project folder
	//
	// Parameters:
	//		project - the project to transfer them for
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::TransferSavedLogs(Project *project)
	{
		ArrayList *savedLogsList = new ArrayList();
		for (int i=0; i < project->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray * dataArray = project->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("LogPaneDataArray") == 0)
			{
				LogPaneDataArray * savedLogArray = dynamic_cast <LogPaneDataArray *> (dataArray);
				if (savedLogArray->SavedLog)
					savedLogsList->Add(savedLogArray);
			}
		}

		String *logsFolder = String::Concat(project->Folder, "\\", project->Title, "_Logs");
		if (!Directory::Exists(logsFolder))
			Directory::CreateDirectory(logsFolder);

		if (savedLogsList != NULL)
		{
			IEnumerator *logsEnum = savedLogsList->GetEnumerator();
			while (logsEnum->MoveNext())
			{
				LogPaneDataArray *savedLogArray = dynamic_cast <LogPaneDataArray *> (logsEnum->Current);
				String *logFilePath = savedLogArray->get_SavedLogFilePath();
				int index = logFilePath->LastIndexOf("\\");
				index++;
				String *logFileName = logFilePath->Substring(index, logFilePath->Length-index);
				String *savedLogFilePath = String::Concat(logsFolder, "\\", logFileName);
				File::Copy(logFilePath, savedLogFilePath, true);								
			}
		}
	}

	//**************************************************************************
	// Method:		TransferSavedCorruptedFiles
	// Description: moves corrupted files folder to new project folder
	//
	// Parameters:
	//		project - the project to transfer them for
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::TransferSavedCorruptedFiles(Project *project)
	{
		ArrayList *savedFilesList = new ArrayList();
		for (int i=0; i < project->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray* dataArray = project->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("SavedFileCorruptionDataArray") == 0)
			{
				SavedFileCorruptionDataArray * savedFileArray = dynamic_cast <SavedFileCorruptionDataArray *> (dataArray);
				for (int i=0; i<savedFileArray->Count; i++)
				{
					DisplayableDataNode* savedFile = savedFileArray->GetDataNodeByIndex(i);
					if (savedFile != NULL)
						savedFilesList->Add(savedFile);
				}
			}
		}

		String *filesFolder = String::Concat(project->Folder, "\\", project->Title, "_CorruptedFiles");
		if (!Directory::Exists(filesFolder))
			Directory::CreateDirectory(filesFolder);

		if (savedFilesList != NULL)
		{
			IEnumerator *filesEnum = savedFilesList->GetEnumerator();
			while (filesEnum->MoveNext())
			{
				SavedFileCorruptionDataNode *savedFile = dynamic_cast <SavedFileCorruptionDataNode *> (filesEnum->Current);
				String *changesFilePath = savedFile->ChangesFile;
				int index = changesFilePath->LastIndexOf("\\");
				index++;
				String *changesFileName = changesFilePath->Substring(index, changesFilePath->Length-index);
				String *savedFilePath = String::Concat(filesFolder, "\\", changesFileName);
				File::Copy(changesFilePath, savedFilePath, true);								
			}
		}
	}

	//**************************************************************************
	// Method:		TransferSavedExceptions
	// Description: moves exceptions folder to new project folder
	//
	// Parameters:
	//		project - the project to transfer them for
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::TransferSavedExceptions(Project *project)
	{
		ArrayList *savedExceptionsList = new ArrayList();
		for (int i=0; i < project->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray* dataArray = project->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("ExceptionDataArray") == 0)
			{
				ExceptionDataArray * savedExceptions = dynamic_cast <ExceptionDataArray *> (dataArray);
				for (int i=0; i<savedExceptions->Count; i++)
				{
					DisplayableDataNode* exception = savedExceptions->GetDataNodeByIndex(i);
					if (exception != NULL)
						savedExceptionsList->Add(exception);
				}
			}
		}

		String *exceptionsFolder = String::Concat(project->Folder, "\\", project->Title, "_Exceptions");
		if (!Directory::Exists(exceptionsFolder))
			Directory::CreateDirectory(exceptionsFolder);

		if (savedExceptionsList != NULL)
		{
			IEnumerator *exceptionEnum = savedExceptionsList->GetEnumerator();
			while (exceptionEnum->MoveNext())
			{
				ExceptionDataNode *exception = dynamic_cast <ExceptionDataNode *> (exceptionEnum->Current);
				String *miniDumpPath = exception->MiniDumpFile;
				int index = miniDumpPath->LastIndexOf("\\");
				index++;
				String *miniDumpFileName = miniDumpPath->Substring(index, miniDumpPath->Length-index);
				String *savedExceptionPath = String::Concat(exceptionsFolder, "\\", miniDumpFileName);
				File::Copy(miniDumpPath, savedExceptionPath, true);								
			}
		}
	}

	//**************************************************************************
	// Method:		SaveSavedLogs
	// Description: Saves the saved logs
	//
	// Parameters:
	//	xmlWriter - xml writer for the save file
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveSavedLogs()
	{
		ArrayList *savedLogsList = new ArrayList();
		for (int i=0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray* dataArray = currentProject->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("LogPaneDataArray") == 0)
			{
				LogPaneDataArray * savedLogArray = dynamic_cast <LogPaneDataArray *> (dataArray);
				if (savedLogArray->SavedLog)
					savedLogsList->Add(savedLogArray);
			}
		}

		xmlWriter->WriteStartElement("SavedLogs");
		
		if (savedLogsList != NULL)
		{
			IEnumerator *logsEnum = savedLogsList->GetEnumerator();
			while (logsEnum->MoveNext())
			{
				LogPaneDataArray *savedLogArray = dynamic_cast <LogPaneDataArray *> (logsEnum->Current);
				xmlWriter->WriteStartElement("SavedLog");
				String * logFolder = this->GetLastFolderName (savedLogArray->get_SavedLogFilePath());
				xmlWriter->WriteAttributeString("FilePath", String::Concat (logFolder, "\\", Path::GetFileName(savedLogArray->get_SavedLogFilePath())));
				xmlWriter->WriteAttributeString("ExeNameAndFileTime", savedLogArray->get_ExeAndLogSavedTime());
				xmlWriter->WriteFullEndElement(); //End for SavedLog
			}
		}

		xmlWriter->WriteFullEndElement(); //End for SavedLogs
	}

	//**************************************************************************
	// Method:		SaveSavedCorruptedFiles
	// Description: Saves the saved corrupted files
	//
	// Parameters:
	//	xmlWriter - xml writer for the save file
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveSavedCorruptedFiles()
	{
		ArrayList *savedFilesList = new ArrayList();
		for (int i=0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray* dataArray = currentProject->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("SavedFileCorruptionDataArray") == 0)
			{
				SavedFileCorruptionDataArray * savedFileArray = dynamic_cast <SavedFileCorruptionDataArray *> (dataArray);
				for (int i=0; i<savedFileArray->Count; i++)
				{
					DisplayableDataNode* savedFile = savedFileArray->GetDataNodeByIndex(i);
					if (savedFile != NULL)
						savedFilesList->Add(savedFile);
				}
			}
		}

		xmlWriter->WriteStartElement("CorruptedFiles");
		
		if (savedFilesList != NULL)
		{
			IEnumerator *filesEnum = savedFilesList->GetEnumerator();
			while (filesEnum->MoveNext())
			{
				SavedFileCorruptionDataNode *savedFile = dynamic_cast <SavedFileCorruptionDataNode *> (filesEnum->Current);
				if (savedFile != NULL)
				{
					xmlWriter->WriteStartElement("CorruptedFile");
					String * changesFolder = this->GetLastFolderName (savedFile->ChangesFile);
					xmlWriter->WriteAttributeString("ChangesPath", String::Concat (changesFolder, "\\", Path::GetFileName(savedFile->ChangesFile)));
					xmlWriter->WriteFullEndElement(); //End for CorruptedFile
				}
			}
		}

		xmlWriter->WriteFullEndElement(); //End for CorruptedFiles
	}

	//**************************************************************************
	// Method:		SaveSavedExceptions
	// Description: Saves the saved exceptions
	//
	// Parameters:
	//	xmlWriter - xml writer for the save file
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveSavedExceptions()
	{
		ArrayList *savedExceptionsList = new ArrayList();
		ArrayList *keyList = new ArrayList();

		for (int i=0; i < currentProject->DisplayableDataArrayCount; i++)
		{
			DisplayableDataArray* dataArray = currentProject->GetDisplayableDataArray(i);
			Type * type = dataArray->GetType();
			
			if (type->Name->CompareTo("ExceptionDataArray") == 0)
			{
				ExceptionDataArray * exceptionArray = dynamic_cast <ExceptionDataArray *> (dataArray);
				
				//Perform check only if procID != 0
				if (dataArray->ProcessID != 0)
				{
					HoloScriptApplication * hsApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForProcess (dataArray->ProcessID);
					if (!hsApp)
						hsApp = HoloScriptRegistry::getInstance()->getHoloScriptAppForTerminatedProcess (dataArray->ProcessID);
					if (!hsApp || !hsApp->ShouldRestartWithProject)
						continue;
				}

				for (int i=0; i<exceptionArray->Count; i++)
				{
					DisplayableDataNode* savedException = exceptionArray->GetDataNodeByIndex(i);
					if (savedException != NULL)
					{
						ExceptionDataNode * eDataNode = dynamic_cast <ExceptionDataNode *> (savedException);
						if (eDataNode != NULL)
						{
							//use a key to make sure we arn't adding dupes
							String * key = String::Concat (eDataNode->ExeName, eDataNode->Exception);
							if (!keyList->Contains (key))
							{
								keyList->Add (key);
								savedExceptionsList->Add(savedException);
							}
						}
					}
				}
			}
		}

		xmlWriter->WriteStartElement("Exceptions");
		
		if (savedExceptionsList != NULL)
		{
			IEnumerator *exceptionEnum = savedExceptionsList->GetEnumerator();
			while (exceptionEnum->MoveNext())
			{
				ExceptionDataNode *savedException = dynamic_cast <ExceptionDataNode *> (exceptionEnum->Current);
				if (savedException != NULL)
				{
					xmlWriter->WriteStartElement("Exception");
					xmlWriter->WriteAttributeString("TimeStamp", savedException->TimeStamp);
					xmlWriter->WriteAttributeString("ExeName", savedException->ExeName);
					xmlWriter->WriteAttributeString("Thread", __box(savedException->threadID)->ToString());
					xmlWriter->WriteAttributeString("Description", savedException->Exception);
					xmlWriter->WriteAttributeString("MiniDump", savedException->MiniDumpFile);
					xmlWriter->WriteFullEndElement(); //End for Exception
				}
			}
		}

		xmlWriter->WriteFullEndElement(); //End for Exceptions
	}

	//**************************************************************************
	// Method:		GetLastFolderName
	// Description: Returns the name of the last folder in a given path
	//				sequence with file name.
	//				Example: C:\temp\hello\myfile.txt will return "hello"
	//
	// Parameters:
	//	fullPath - the path with filename
	//
	// Return Value: the name of the last folder in the given path
	//*************************************************************************
	String* ProjectFileWriter::GetLastFolderName(String * fullPath)
	{
		String * path = "";
		//Get the relative log path
		path = Path::GetDirectoryName (fullPath);
		int index = path->LastIndexOf (S"\\");
		if (index > 0 && path->Length >= index + 1)
			path = path->Substring (index + 1);

		return path;
	}

	//**************************************************************************
	// Method:		SaveCorruptionRandomSettings
	// Description: saves a random settings structure
	//
	// Parameters:
	//	settings - the random settings struct to save
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveCorruptionRandomSettings(CorruptionRandomSettings settings)
	{
		xmlWriter->WriteStartElement("CorruptionRandomSettings");

		xmlWriter->WriteAttributeString("CorruptAmount", settings.corruptAmount.ToString());
		switch(settings.type)
		{
		case CorruptionRandomType::RandomCorruptSingleBytes:
			xmlWriter->WriteAttributeString("CorruptionRandomType", "SingleBytes");
			break;
		case CorruptionRandomType::RandomCorruptLongStrings:
			xmlWriter->WriteAttributeString("CorruptionRandomType", "LongStrings");
			break;
		case CorruptionRandomType::RandomCorruptRepeatedSequence:
			xmlWriter->WriteAttributeString("CorruptionRandomType", "RepeatedSequence");
			break;
		default:
			xmlWriter->WriteAttributeString("CorruptionRandomType", "SingleBytes");
			break;
		}
		xmlWriter->WriteAttributeString("Length", settings.length.ToString());
		xmlWriter->WriteAttributeString("CorruptStringType", settings.corruptStringType.ToString());
		xmlWriter->WriteAttributeString("CorruptString", settings.corruptString);
		xmlWriter->WriteAttributeString("Overwrite", settings.overwrite.ToString());
		
		xmlWriter->WriteFullEndElement();
	}

	//**************************************************************************
	// Method:		SaveCorruptionSearchSettings
	// Description: saves a search settings structure
	//
	// Parameters:
	//	settings - the search settings struct to save
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveCorruptionSearchSettings(CorruptionSearchSettings settings)
	{
		xmlWriter->WriteStartElement("CorruptionSearchSettings");

		xmlWriter->WriteAttributeString("SearchStringType", settings.searchStringType.ToString());
		xmlWriter->WriteAttributeString("SearchString", settings.searchString);
		switch(settings.type)
		{
		case CorruptionSearchType::SearchCorruptReplace:
			xmlWriter->WriteAttributeString("CorruptionSearchType", "Replace");
			break;
		case CorruptionSearchType::SearchCorruptLongStrings:
			xmlWriter->WriteAttributeString("CorruptionSearchType", "LongStrings");
			break;
		case CorruptionSearchType::SearchCorruptRepeatedSequence:
			xmlWriter->WriteAttributeString("CorruptionSearchType", "RepeatedSequence");
			break;
		default:
			xmlWriter->WriteAttributeString("CorruptionSearchType", "Replace");
			break;
		}
		xmlWriter->WriteAttributeString("Length", settings.length.ToString());
		xmlWriter->WriteAttributeString("CorruptStringType", settings.corruptStringType.ToString());
		xmlWriter->WriteAttributeString("CorruptString", settings.corruptString);
		xmlWriter->WriteAttributeString("Overwrite", settings.overwrite.ToString());
		
		xmlWriter->WriteFullEndElement();
	}

	//**************************************************************************
	// Method:		SaveCorruptionRegExprSettings
	// Description: saves a regex settings structure
	//
	// Parameters:
	//	settings - the regex settings struct to save
	//
	// Return Value: None
	//*************************************************************************
	void ProjectFileWriter::SaveCorruptionRegExprSettings(CorruptionRegExprSettings settings)
	{
		xmlWriter->WriteStartElement("CorruptionRegExprSettings");

		xmlWriter->WriteAttributeString("SearchString", settings.searchString);
		xmlWriter->WriteAttributeString("ReplaceString", settings.corruptString);
		
		xmlWriter->WriteFullEndElement();
	}
}