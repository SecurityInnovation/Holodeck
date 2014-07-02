//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		TestGenerationDataArray.cpp
//
// DESCRIPTION: Contains implementation for the class TestGenerationDataArray
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 18 Apr 2003		 J. Taylor	 File created.
//*************************************************************************
#include "TestGenerationDataArray.h"

namespace UIData
{

	//*************************************************************************
	// Method:		TestGenerationDataArray
	// Description: Constructor for the TestGenerationDataArray class
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	TestGenerationDataArray::TestGenerationDataArray(HoloScriptRegistry *HoloScriptRegistry, SortedList *DataArrayList)
	{
		settingsFile = GuiSupport::RegistryHelper::getInstance()->GetInstallationPath();
		settingsFile = String::Concat(settingsFile, TESTGEN_SETTINGS_FILE);

		//if null, just using this class for loadsettings.
		if (HoloScriptRegistry && DataArrayList)
		{
			memoryLimitMax = 0;
			diskLimitMax = 0;
			maxDiskUsed = 0;
			maxMemoryUsed = 0;
			avgDiskUsed = 0;
			avgMemoryUsed = 0;
			dataPoints = 0;
			projectRestartStarted = false;
			holoScriptRegistry = HoloScriptRegistry;
			dataArrayList = DataArrayList;
			random = new Random();
			stressInjectList = new ArrayList();
			apiList = new SortedList();

			FaultFunctionDB *faultDB = FaultFunctionDB::GetInstance();
			diskFaults = faultDB->DiskFaults;
			memoryFaults = faultDB->MemoryFaults;
			networkFaults = faultDB->NetworkFaults;
			registryFaults = faultDB->RegistryFaults;
			processFaults = faultDB->ProcessFaults;
			customFaults = faultDB->CustomFaults;

			TimerCallback *timerDelegate = new TimerCallback(this, onStartRandomStressTimer);
			startRandomStressTimer = new System::Threading::Timer(timerDelegate, NULL, Timeout::Infinite, Timeout::Infinite);
			timerDelegate = new TimerCallback(this, onStartIntelligentStressTimer);
			startIntelligentStressTimer = new System::Threading::Timer(timerDelegate, NULL, Timeout::Infinite, Timeout::Infinite);
			timerDelegate = new TimerCallback(this, onStopTimer);
			stopTimer = new System::Threading::Timer(timerDelegate, NULL, Timeout::Infinite, Timeout::Infinite);
			timerDelegate = new TimerCallback(this, onRestartTimer);
			restartTimer = new System::Threading::Timer(timerDelegate, NULL, Timeout::Infinite, Timeout::Infinite);

			logHandler = new LogsGeneratedDelegate(this, onLogsGenerated);
			projectRestartedHandler = new ProjectRestartedDelegate(this, ProjectRestarted);
			
			uiEventRegistry = UIEventRegistry::getInstance();
			processTerminatedHandler = new ProcessTerminatedDelegate(this, ProcessTerminated);
			uiEventRegistry->OnProcessTerminated += processTerminatedHandler;

			dataEventRegistry->OnStopCodeCoverageTestGeneration += new StopCodeCoverageTestGenerationDelegate(this, CodeCoverageTestGenerationStop);
			dataEventRegistry->OnStopStressTestGeneration += new StopStressTestGenerationDelegate(this, StressTestGenerationStop);
		}
	}

	//*************************************************************************
	// Method:		TestGenerationDataArray
	// Description: Destructor for the TestGenerationDataArray class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	TestGenerationDataArray::~TestGenerationDataArray()
	{
		if (uiEventRegistry && processTerminatedHandler)
			uiEventRegistry->OnProcessTerminated -= processTerminatedHandler;
	}

	//*************************************************************************
	// Method:		BuildArray
	// Description: adds a datanode in the array for each possible limit and fault
	//
	// Parameters:
	//	generationType - specifies stress or code coverage testing
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::BuildArray(GenerationType generationType)
	{
		LimitPaneDataNode *limitDataNode;
		TestGenerationDataNode *testGenNode;
		FaultPaneDataNode *faultNode;

		this->Clear();

		//if randomstress than we add all and check at time of test set.  If code coverage we only add if allowed
		for (int i=0; (i<diskFaults->Count) && (allowDiskFaults || (generationType != CodeCoverageTest)); i++)
		{
			Fault *diskFault = dynamic_cast <Fault*> (diskFaults->Item[i]);
			faultNode = new FaultPaneDataNode(diskFault->Name, UIData::DiskFault, 0, 0);
			faultNode->FaultID = diskFault->FaultID;
			testGenNode = new TestGenerationDataNode(faultNode);
			CreateDataNode(testGenNode);
		}

   		for (int i=0; (i<memoryFaults->Count) && (allowMemoryFaults || (generationType != CodeCoverageTest)); i++)
		{
			Fault *memoryFault = dynamic_cast <Fault*> (memoryFaults->Item[i]);
			faultNode = new FaultPaneDataNode(memoryFault->Name, UIData::MemoryFault, 0, 0);
			faultNode->FaultID = memoryFault->FaultID;
			testGenNode = new TestGenerationDataNode(faultNode);
			CreateDataNode(testGenNode);
		}

		for (int i=0; (i<networkFaults->Count) && (allowNetworkFaults || (generationType != CodeCoverageTest)); i++)
		{
			Fault *networkFault = dynamic_cast <Fault*> (networkFaults->Item[i]);
			faultNode = new FaultPaneDataNode(networkFault->Name, UIData::NetworkFault, 0, 0);
			faultNode->FaultID = networkFault->FaultID;
			testGenNode = new TestGenerationDataNode(faultNode);
			CreateDataNode(testGenNode);
		}

		for (int i=0; (i<registryFaults->Count) && (allowRegistryFaults || (generationType != CodeCoverageTest)); i++)
		{
			Fault *registryFault = dynamic_cast <Fault*> (registryFaults->Item[i]);
			faultNode = new FaultPaneDataNode(registryFault->Name, UIData::RegistryFault, 0, 0);
			faultNode->FaultID = registryFault->FaultID;
			testGenNode = new TestGenerationDataNode(faultNode);
			CreateDataNode(testGenNode);
		}

		for (int i=0; (i<processFaults->Count) && (allowProcessFaults || (generationType != CodeCoverageTest)); i++)
		{
			Fault *processFault = dynamic_cast <Fault*> (processFaults->Item[i]);
			faultNode = new FaultPaneDataNode(processFault->Name, UIData::ProcessFault, 0, 0);
			faultNode->FaultID = processFault->FaultID;
			testGenNode = new TestGenerationDataNode(faultNode);
			CreateDataNode(testGenNode);
		}

		for (int i=0; (i<customFaults->Count) && (allowCustomFaults || (generationType != CodeCoverageTest)); i++)
		{
			Fault *customFault = dynamic_cast <Fault*> (customFaults->Item[i]);
			faultNode = new FaultPaneDataNode(customFault->Name, UIData::CustomFault, 0, 0);
			faultNode->FaultID = customFault->FaultID;
			testGenNode = new TestGenerationDataNode(faultNode);
			CreateDataNode(testGenNode);
		}
	
		if (generationType != CodeCoverageTest)
		{
			limitDataNode = new LimitPaneDataNode("Disk Limit", UIData::DiskLimit, 0, 0);
			testGenNode = new TestGenerationDataNode(limitDataNode);
			CreateDataNode(testGenNode);

			limitDataNode = new LimitPaneDataNode("Memory Limit", UIData::MemoryLimit, 0, 0);
			testGenNode = new TestGenerationDataNode(limitDataNode);
			CreateDataNode(testGenNode);

			limitDataNode = new LimitPaneDataNode("Network Upload Limit", UIData::NetworkUpLimit, 0, 0);
			testGenNode = new TestGenerationDataNode(limitDataNode);
			CreateDataNode(testGenNode);

			limitDataNode = new LimitPaneDataNode("Network Download Limit", UIData::NetworkDownLimit, 0, 0);
			testGenNode = new TestGenerationDataNode(limitDataNode);
			CreateDataNode(testGenNode);
		}
	}

	//*************************************************************************
	// Method:		StressTestGenerationStart
	// Description: start stress test generation
	//
	// Parameters:
	//	GenerationType - random or intelligent testing
	//	intensity - low, medium, or high intensity
	//	procID - process to apply tests to, 0 means pick randomly
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::StressTestGenerationStart(GenerationType generationType, Intensity intensity)
	{
		testGenStatusBox = new HolodeckGui::TestGenStatusBox();
		stressTestRunning = true;
		if (generationType == RandomStressTest)
		{
			LoadSettings(generationType, intensity);
			BuildArray(generationType);
			testInterval = testInterval - testDuration;
			startRandomStressTimer->Change(testInterval, Timeout::Infinite);
			testGenStatusBox->DescriptionText = "Waiting to turn on first set of random tests";
		}
		else if (generationType == IntelligentStressTest)
		{
			LoadSettings(generationType, intensity);
			startIntelligentStressTimer->Change(testInterval, Timeout::Infinite);
			testGenStatusBox->DescriptionText = "Observing application behavior";
		}

		testGenStatusBox->TopMost = true;
		testGenStatusBox->TestsRun = 0;
		testGenStatusBox->TestsLeft = -1;
		testGenStatusBox->Show(generationType, intensity);
		testGenStatusBox->DesktopLocation = Point(0,0);
	}

	//*************************************************************************
	// Method:		StressTestGenerationStop
	// Description: stop stress test generation
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::StressTestGenerationStop()
	{
		Object *args[] = __gc new Object*[1];

		stressTestRunning = false;
		startRandomStressTimer->Change(Timeout::Infinite, Timeout::Infinite);
		startIntelligentStressTimer->Change(Timeout::Infinite, Timeout::Infinite);
		stopTimer->Change(Timeout::Infinite, Timeout::Infinite);

		//clear old tests
		for (int i=0; i<stressInjectList->Count; i++)
		{
			DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (stressInjectList->get_Item(i));
			args[0] = dataNode;
			dataNode->DeleteDelegate->DynamicInvoke(args);
		}
		stressInjectList->Clear();

		if (dataEventRegistry->OnStressTestGenerationStopped)
			dataEventRegistry->OnStressTestGenerationStopped->Invoke();

		testGenStatusBox->Close();
	}

	//*************************************************************************
	// Method:		CodeCoverageTestGenerationStart
	// Description: start code coverage test generation
	//
	// Parameters:
	//	intensity - low, medium, or high intensity
	//	restartProject - if true restart the project immediately
	//  timeout - timeout in milliseconds before application is auto-restarted
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::CodeCoverageTestGenerationStart(Intensity intensity, bool restartProject, int timeout)
	{
		LoadSettings(CodeCoverageTest, intensity);
		BuildArray(CodeCoverageTest);
		recording = true;
		codeCoverTestTimeout = timeout;

		testGenStatusBox = new HolodeckGui::TestGenStatusBox();
		testGenStatusBox->TopMost = true;
		testGenStatusBox->DescriptionText = "Restarting application";
		testGenStatusBox->TestsRun = 0;
		testGenStatusBox->TestsLeft = -1;
		testGenStatusBox->Show(CodeCoverageTest, intensity);
		testGenStatusBox->DesktopLocation = Point(0,0);

		dataEventRegistry->OnProjectRestarted += projectRestartedHandler;

		if (restartProject)
		{
			restartMutex->WaitOne();
			dataEventRegistry->OnRestartProject->Invoke(true);
			restartMutex->ReleaseMutex();
		}
		else
			ProjectRestarted();
	}

	//*************************************************************************
	// Method:		CodeCoverageTestGenerationStop
	// Description: stop code coverage test generation
	//
	// Parameters:
	//	none
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::CodeCoverageTestGenerationStop()
	{
		codeCoverageTestRunning = false;
		HoloScriptApplication *holoScriptApp = NULL;

		if (dataEventRegistry->OnProjectRestarted)
			dataEventRegistry->OnProjectRestarted -= projectRestartedHandler;

		if (holoScriptRegistry->Count != 0)
			holoScriptApp = holoScriptRegistry->getHoloScriptAppByIndex(0);

		if ( (holoScriptApp) && (holoScriptApp->OnLogGenerated) )
			holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);

		if (dataEventRegistry->OnCodeCoverageTestGenerationStopped)
			dataEventRegistry->OnCodeCoverageTestGenerationStopped->Invoke();

		restartTimer->Change(Timeout::Infinite, Timeout::Infinite);

		testGenStatusBox->Close();
	}

	//*************************************************************************
	// Method:		onStartRandomStressTimer
	// Description: called to start the next test, fault, or limit for random stress testing
	//
	// Parameters:
	//	state - timer information
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::onStartRandomStressTimer(Object *state)
	{
		FaultPaneDataNode *faultNode = NULL;
		TestGenerationDataNode *testGenNode = NULL;
		LimitPaneDataNode *limitDataNode = NULL;
		NetworkCorruptionDataNode *ncDataNode = NULL;
		HoloScriptApplication *holoScriptApp;
		Object *args[] = __gc new Object*[1];
		int faultIndex, processIndex, testType;
		unsigned int pID;
		UInt64 maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage, limitValue;
		bool setDiskFault = false, setMemoryFault = false, setNetworkFault = false, setRegistryFault = false, setProcessFault = false, setCustomFault = false;
		bool setDiskLimit = false, setMemoryLimit = false, setNetworkUpLimit = false, setNetworkDownLimit = false;
		
		//turn off timer
		startRandomStressTimer->Change(Timeout::Infinite, Timeout::Infinite);

		if (!stressTestRunning)
			return;

		testGenStatusBox->DescriptionText = "Running random tests";
		
		//pick a process to inject
		processIndex = random->Next(0, holoScriptRegistry->Count);
		holoScriptApp = holoScriptRegistry->getHoloScriptAppByIndex(processIndex);
		holoScriptApp->GetApplicationProcessID(pID);

		//pick a fault or limit to inject up to numOverlapping
		for (int i=0; i<numOverlappingTests; i++)
		{
			while ( (!faultNode) && (!limitDataNode) && (!ncDataNode) )
			{
				testType = random->Next(diskFault, networkCorruption + 1);
				switch (testType)
				{
				case diskFault:
					if (allowDiskFaults && !setDiskFault)
					{
						setDiskFault = true;
						faultIndex = random->Next(0, diskFaults->Count);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(faultIndex));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						faultNode = dynamic_cast <FaultPaneDataNode*> (testGenNode->DataNode);
						faultNode->processID = pID;
						if (!stressInjectList->Contains(faultNode))
						{
							args[0] = faultNode;
							faultNode->CreateDelegate->DynamicInvoke(args); 
							stressInjectList->Add(faultNode);
						}
						else 
							faultNode = NULL;
					}
					break;
				case memoryFault:
					if (allowMemoryFaults && !setMemoryFault)
					{
						setMemoryFault = true;
						faultIndex = random->Next(0, memoryFaults->Count);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(faultIndex + diskFaults->Count));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						faultNode = dynamic_cast <FaultPaneDataNode*> (testGenNode->DataNode);
						faultNode->processID = pID;
						if (!stressInjectList->Contains(faultNode))
						{
							args[0] = faultNode;
							faultNode->CreateDelegate->DynamicInvoke(args);                     
							stressInjectList->Add(faultNode);
						}
						else 
							faultNode = NULL;
					}
					break;
				case networkFault:
					if (allowNetworkFaults && !setNetworkFault)
					{
						setNetworkFault = true;
						faultIndex = random->Next(0, networkFaults->Count);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(faultIndex + diskFaults->Count + memoryFaults->Count));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						faultNode = dynamic_cast <FaultPaneDataNode*> (testGenNode->DataNode);
						faultNode->processID = pID;
						if (!stressInjectList->Contains(faultNode))
						{
							args[0] = faultNode;
							faultNode->CreateDelegate->DynamicInvoke(args);                     
							stressInjectList->Add(faultNode);
						}
						else 
							faultNode = NULL;
					}
					break;
				case registryFault:
					if (allowRegistryFaults && !setRegistryFault)
					{
						setRegistryFault = true;
						faultIndex = random->Next(0, registryFaults->Count);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(faultIndex + diskFaults->Count + memoryFaults->Count + networkFaults->Count));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						faultNode = dynamic_cast <FaultPaneDataNode*> (testGenNode->DataNode);
						faultNode->processID = pID;
						if (!stressInjectList->Contains(faultNode))
						{
							args[0] = faultNode;
							faultNode->CreateDelegate->DynamicInvoke(args);                     
							stressInjectList->Add(faultNode);
						}
						else 
							faultNode = NULL;
					}
					break;
				case processFault:
					if (allowProcessFaults && !setProcessFault)
					{
						setProcessFault = true;
						faultIndex = random->Next(0, processFaults->Count);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(faultIndex + diskFaults->Count + memoryFaults->Count + networkFaults->Count + registryFaults->Count));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						faultNode = dynamic_cast <FaultPaneDataNode*> (testGenNode->DataNode);
						faultNode->processID = pID;
						if (!stressInjectList->Contains(faultNode))
						{
							args[0] = faultNode;
							faultNode->CreateDelegate->DynamicInvoke(args);                     
							stressInjectList->Add(faultNode);
						}
						else 
							faultNode = NULL;
					}
					break;
				case customFault:
					if (allowCustomFaults && !setCustomFault && (customFaults->Count > 0))
					{
						setCustomFault = true;
						faultIndex = random->Next(0, customFaults->Count);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(faultIndex + diskFaults->Count + memoryFaults->Count + networkFaults->Count + registryFaults->Count + processFaults->Count));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						faultNode = dynamic_cast <FaultPaneDataNode*> (testGenNode->DataNode);
						faultNode->processID = pID;
						if (!stressInjectList->Contains(faultNode))
						{
							args[0] = faultNode;
							faultNode->CreateDelegate->DynamicInvoke(args);                     
							stressInjectList->Add(faultNode);
						}
						else 
							faultNode = NULL;
					}
					break;
				case diskLimit:
					if (allowDiskLimit && holoScriptApp && !setDiskLimit)
					{
						setDiskLimit = true;
						holoScriptApp->GetMonitorInformation(maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage);
						if (diskLimitSet == OverCurrentUsage)
						{
							limitValue = random->Next(1, diskLimitMax);
							currentDiskUsage += limitValue;
						}

						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(diskFaults->Count + memoryFaults->Count + networkFaults->Count + registryFaults->Count + processFaults->Count + customFaults->Count));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						limitDataNode = dynamic_cast <LimitPaneDataNode*> (testGenNode->DataNode);
						limitDataNode->processID = pID;
						limitDataNode->LimitValue = currentDiskUsage;
						args[0] = limitDataNode;
						limitDataNode->CreateDelegate->DynamicInvoke(args);
						stressInjectList->Add(limitDataNode);
					}
					break;
				case memoryLimit:
					if (allowMemoryLimit && holoScriptApp && !setMemoryLimit)
					{
						setMemoryLimit = true;
						holoScriptApp->GetMonitorInformation(maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage);
						if (memoryLimitSet == OverCurrentUsage)
						{
							limitValue = random->Next(1, memoryLimitMax);
							currentMemoryUsage += limitValue;
						}

						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(diskFaults->Count + memoryFaults->Count + networkFaults->Count + registryFaults->Count + processFaults->Count + customFaults->Count + 1));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						limitDataNode = dynamic_cast <LimitPaneDataNode*> (testGenNode->DataNode);
						limitDataNode->processID = pID;
						limitDataNode->LimitValue = currentMemoryUsage;
						args[0] = limitDataNode;
						limitDataNode->CreateDelegate->DynamicInvoke(args);
						stressInjectList->Add(limitDataNode);
					}
					break;
				case networkUploadLimit:
					if (allowNetworkLimit && !setNetworkUpLimit)
					{
						setNetworkUpLimit = true;
						limitValue = random->Next(networkLowRange, networkHighRange+1);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(diskFaults->Count + memoryFaults->Count + networkFaults->Count + registryFaults->Count + processFaults->Count + customFaults->Count + 2));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						limitDataNode = dynamic_cast <LimitPaneDataNode*> (testGenNode->DataNode);
						limitDataNode->processID = pID;
						limitDataNode->LimitValue = limitValue;
						args[0] = limitDataNode;
						limitDataNode->CreateDelegate->DynamicInvoke(args);
						stressInjectList->Add(limitDataNode);
					}
					break;
				case networkDownloadLimit:
					if (allowNetworkLimit && !setNetworkDownLimit)
					{
						setNetworkDownLimit = true;
						limitValue = random->Next(networkLowRange, networkHighRange+1);
						testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(diskFaults->Count + memoryFaults->Count + networkFaults->Count + registryFaults->Count + processFaults->Count + customFaults->Count + 3));
						if (!testGenNode || !testGenNode->DataNode)
							continue;

						limitDataNode = dynamic_cast <LimitPaneDataNode*> (testGenNode->DataNode);
						limitDataNode->processID = pID;
						limitDataNode->LimitValue = limitValue;
						args[0] = limitDataNode;
						limitDataNode->CreateDelegate->DynamicInvoke(args);
						stressInjectList->Add(limitDataNode);
					}
					break;
				case networkCorruption:
					if (allowNetworkCorruption)
					{
						ncDataNode = new NetworkCorruptionDataNode("Network Fuzzing", pID);

						ArrayList *portList = new ArrayList();
						ncDataNode->ReceivedData = true;
						ncDataNode->SentData = false;
						ncDataNode->PortString = "Recv on All";
						ncDataNode->GetPortListFromPortString();
						ncDataNode->CorruptType = "Random";

						int corruptAmount = random->Next(maxCorruptAmount);
						ncDataNode->SearchFor = String::Concat(__box(corruptAmount)->ToString(), " of 10000 bytes");
						bool insert;
						if (random->Next(2) == 0)
						{
							// Insert
							insert = true;
							ncDataNode->SearchRegularExpression = String::Concat("r/", __box(corruptAmount)->ToString(), "/");
						}
						else
						{
							// Overwrite
							insert = false;
							ncDataNode->SearchRegularExpression = String::Concat("or/", __box(corruptAmount)->ToString(), "/");
						}

						if (random->Next(2) == 0)
						{
							// Random single bytes
							if (insert)
								ncDataNode->ReplaceWith = "Insert random single byte";
							else
								ncDataNode->ReplaceWith = "Overwrite random single byte";
							ncDataNode->ReplacementString = S"$R1,1$";
						}
						else
						{
							// Random long strings
							int strLength = random->Next(maxLongStringLen);
							if (insert)
							{
								ncDataNode->ReplaceWith = String::Concat("Insert long string, up to ",
									__box(strLength)->ToString(), " bytes");
							}
							else
							{
								ncDataNode->ReplaceWith = String::Concat("Overwrite long string, up to ",
									__box(strLength)->ToString(), " bytes");
							}
							ncDataNode->ReplacementString = String::Concat(S"$R1,", __box(strLength)->ToString(), S"$");
						}

						args[0] = ncDataNode;
						ncDataNode->CreateDelegate->DynamicInvoke(args);
						stressInjectList->Add(ncDataNode);
					}
					break;
				}
			}
			limitDataNode = NULL;
			faultNode = NULL;
			testGenNode = NULL;
		}

		testGenStatusBox->TestsRun += stressInjectList->Count;
		//turn on timer to remove tests
		stopTimer->Change(testDuration, Timeout::Infinite);
	}

	//*************************************************************************
	// Method:		onStartIntelligentStressTimer
	// Description: called to start the next test for intelligent stress testing
	//
	// Parameters:
	//	state - timer information
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::onStartIntelligentStressTimer(Object *state)
	{
		HoloScriptApplication *holoScriptApp;
		Object *args[] = __gc new Object*[1];
		LogItem *logItem;
		unsigned long filePos;
		unsigned int pID;

		//turn off timer
		startIntelligentStressTimer->Change(Timeout::Infinite, Timeout::Infinite);
		if (!stressTestRunning)
			return;

		testGenStatusBox->DescriptionText = "Removing old tests";

		//clear old tests
		for (int i=0; i<stressInjectList->Count; i++)
		{
			DisplayableDataNode *dataNode = dynamic_cast <DisplayableDataNode*> (stressInjectList->get_Item(i));
			args[0] = dataNode;
			dataNode->DeleteDelegate->DynamicInvoke(args);
		}
		
		stressInjectList->Clear();

		testGenStatusBox->DescriptionText = "Running tests based on application observation";
		//create tests for each process 
		for (int i=0; i < holoScriptRegistry->Count; i++)
		{
			holoScriptApp = holoScriptRegistry->getHoloScriptAppByIndex(i);
			holoScriptApp->GetApplicationProcessID(pID);

			for (int i=1; i<=numOverlappingTests; i++)
			{
				//50% chance of scheduled test or resource test
				if ((!allowResourceTests) || (random->Next(2)))
				{
					// Scheduled test
					logItem = holoScriptApp->GetLogEntryAt(random->Next(holoScriptApp->LogEntryCount), filePos);
					ScheduledTestDataNode *testDataNode = new ScheduledTestDataNode(logItem->Function, pID, 0);
					testDataNode->Function = logItem->Function;
					testDataNode->Category = logItem->Category;
					testDataNode->MatchParameters = logItem->Parameters;
					testDataNode->ReturnValue = GetReturnValue(logItem->Function);
					testDataNode->ErrorCode = GetErrorCode(logItem->Function);
					testDataNode->TestOwner = "Intelligent Stress Test Generator";
					if (testDataNode->ErrorCode == NULL)
					{
						i--;
						continue;
					}

					args[0] = testDataNode;
					testDataNode->CreateDelegate->DynamicInvoke(args);
					stressInjectList->Add(testDataNode);
				}
				else
				{
					// Resource test (includes file corruption)
					Fault *fault;
					String *keyString = String::Concat(__typeof(ResourcePaneDataArray)->Name, Convert::ToString(pID));
					keyString = String::Concat(keyString, "0");
					ResourcePaneDataArray *dataArray = dynamic_cast <ResourcePaneDataArray*> (dataArrayList->Item[keyString]);
					if (!dataArray)
					{
						i--;
						continue;
					}

					ResourcePaneDataNode *resourceDataNode = dynamic_cast <ResourcePaneDataNode*> (dataArray->GetDataNodeByIndex(random->Next(dataArray->Count)));
					if (!resourceDataNode)
					{
						i--;
						continue;
					}

					DisplayableDataNode *testDataNode = NULL;

					//pick a fault name
					switch (resourceDataNode->Type)
					{
					case File:
						{
							if ((File::GetAttributes(resourceDataNode->Name) & FileAttributes::Directory) !=
								FileAttributes::Directory)
							{
								// Path is not a directory
								if (random->Next(2) == 0)
								{
									// File corruption 50% of the time we want to test a file
									FileCorruptionDataNode *fcDataNode = new FileCorruptionDataNode(resourceDataNode->Name, pID);
									fcDataNode->OriginalFile = resourceDataNode->Name;
									fcDataNode->CorruptType = "Random";

									int corruptAmount = random->Next(maxCorruptAmount);
									fcDataNode->SearchFor = String::Concat(__box(corruptAmount)->ToString(), " of 10000 bytes");
									bool insert;
									if (random->Next(2) == 0)
									{
										// Insert
										insert = true;
										fcDataNode->SearchRegularExpression = String::Concat("r/", __box(corruptAmount)->ToString(), "/");
									}
									else
									{
										// Overwrite
										insert = false;
										fcDataNode->SearchRegularExpression = String::Concat("or/", __box(corruptAmount)->ToString(), "/");
									}

									if (random->Next(2) == 0)
									{
										// Random single bytes
										if (insert)
											fcDataNode->ReplaceWith = "Insert random single byte";
										else
											fcDataNode->ReplaceWith = "Overwrite random single byte";
										fcDataNode->ReplacementString = S"$R1,1$";
									}
									else
									{
										// Random long strings
										int strLength = random->Next(maxLongStringLen);
										if (insert)
										{
											fcDataNode->ReplaceWith = String::Concat("Insert long string, up to ",
												__box(strLength)->ToString(), " bytes");
										}
										else
										{
											fcDataNode->ReplaceWith = String::Concat("Overwrite long string, up to ",
												__box(strLength)->ToString(), " bytes");
										}
										fcDataNode->ReplacementString = String::Concat(S"$R1,", __box(strLength)->ToString(), S"$");
									}

									// Set the test node to the file corruption node
									testDataNode = fcDataNode;
								}
							}
							if (testDataNode == NULL)
							{
								// No test selected yet, use a resource test
								fault = dynamic_cast <Fault*> (diskFaults->Item[random->Next(diskFaults->Count)]);
							}
							break;
						}
					case RegistryEntry:
						fault = dynamic_cast <Fault*> (registryFaults->Item[random->Next(registryFaults->Count)]);
						break;
					case Process:
						fault = dynamic_cast <Fault*> (processFaults->Item[random->Next(processFaults->Count)]);
						break;
					case ComObject:
						if (customFaults->Count > 0)
							fault = dynamic_cast <Fault*> (customFaults->Item[0]);
						break;
					}

					if (testDataNode == NULL)
					{
						// Create a test node if one hasn't already been created
						testDataNode = new ResourceTestDataNode(resourceDataNode->Name, fault->Name, fault->Type, pID, 0);
					}

					args[0] = testDataNode;
					testDataNode->CreateDelegate->DynamicInvoke(args);
					stressInjectList->Add(testDataNode);
				}
			}
		}
		
		testGenStatusBox->TestsRun += stressInjectList->Count;
		//turn timer back on
		startIntelligentStressTimer->Change(testInterval, Timeout::Infinite);
	}

	//*************************************************************************
	// Method:		onStopTimer
	// Description: called to stop the tests that were previously started
	//
	// Parameters:
	//	state - timer information
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::onStopTimer(Object *state)
	{
		Object *args[] = __gc new Object*[1];
		DisplayableDataNode *dataNode;
		testGenStatusBox->DescriptionText = "Removing old tests";
		
		//turn off the timer
		stopTimer->Change(Timeout::Infinite, Timeout::Infinite);
		if (!stressTestRunning)
			return;

		//turn off the injected faults/limits
		for (int i=0; i<stressInjectList->Count; i++)
		{
			dataNode = dynamic_cast <DisplayableDataNode*> (stressInjectList->get_Item(i));
			args[0] = dataNode;
			dataNode->DeleteDelegate->DynamicInvoke(args);
		}
		
		stressInjectList->Clear();

		testGenStatusBox->DescriptionText = "Waiting to turn on next set of random tests";
		//turn on timer for next test to be set
		startRandomStressTimer->Change(testInterval, Timeout::Infinite);
	}

	//*************************************************************************
	// Method:		onRestartTimer
	// Description: called to restart a project in cc tests after timeout
	//
	// Parameters:
	//	state - timer information
	//
	// Return Value: None
	//*************************************************************************
	void TestGenerationDataArray::onRestartTimer(Object *state)
	{
		if (!codeCoverageTestRunning)
		{
			//testing has stopped, turn off timer
			restartTimer->Change(Timeout::Infinite, Timeout::Infinite);
			return;
		}

		HoloScriptApplication *holoScriptApp = NULL;

		if (holoScriptRegistry->Count != 0)
			holoScriptApp = holoScriptRegistry->getHoloScriptAppByIndex(0);

		if (!holoScriptApp)
		{
			CodeCoverageTestGenerationStop();
			return;
		}

		if (this->Count > 0)
		{
			testGenStatusBox->DescriptionText = "Restarting application";
			holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
			restartTimer->Change(Timeout::Infinite, Timeout::Infinite);
			restartMutex->WaitOne();
			projectRestartStarted = true;
			dataEventRegistry->OnRestartProject->Invoke(true);
			restartMutex->ReleaseMutex();
		}
		else
		{
			this->CodeCoverageTestGenerationStop();
		}
	}

	//*************************************************************************
	// Method:		GetErrorCode
	// Description: gets a new error code or exception that can be used to generate a test
	//
	// Parameters:
	//	function - name of the function to get an error code for
	//
	// Return Value: the new error code in numeric form or an exception if the function is .net
	//*************************************************************************
	String *TestGenerationDataArray::GetErrorCode(String *function)
	{
		bool dotNet = false;
		int index;
		String *errorCode;
		ICollection *collection;
		ErrorCodeDB *errorCodeDB = ErrorCodeDB::GetInstance();
		ExceptionDB *exceptionDB = ExceptionDB::GetInstance();
		InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		InterceptedFunction *interceptedFunction = db->GetFunctionByName(function);

		if (!interceptedFunction)
		{
			// Try the .NET database
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			interceptedFunction = db->GetFunctionByName(function);
			
			if (interceptedFunction)
				dotNet = true;
			else
				return NULL;
		}
		
		if (dotNet)
		{
			collection = interceptedFunction->Exceptions;
			if (collection->Count != 0)
			{
				String *errorArray[] = __gc new String*[collection->Count];
				collection->CopyTo(errorArray, 0);
				index = random->Next(0, errorArray->Length);
				errorCode = dynamic_cast <String*> (errorArray->GetValue(index));
			}
			else
				errorCode = "0";
		}
		else
		{
			collection = interceptedFunction->ErrorCodes;
			if (collection->Count == 0)
			{
				collection = errorCodeDB->GetAllValues();
				String *errorArray[] = __gc new String*[collection->Count];
				collection->CopyTo(errorArray, 0);
				index = random->Next(0, errorArray->Length);
				errorCode = errorCodeDB->GetCodeFromString(dynamic_cast <String*> (errorArray->GetValue(index)));
			}
			else
			{
				Array *errorArray = interceptedFunction->ErrorCodes;
				index = random->Next(0, errorArray->Length);
				InterceptedFunctionErrorCode *ec = dynamic_cast<InterceptedFunctionErrorCode *>(errorArray->GetValue(index));
				errorCode = Convert::ToString(ec->ECValue);
			}
		}
		
		return errorCode;
	}

	//*************************************************************************
	// Method:		GetReturnValue
	// Description: gets a new return value that can be used to generate a test
	//
	// Parameters:
	//	function - name of the function to get an error code for
	//
	// Return Value: the new return value in numeric form
	//*************************************************************************
	String *TestGenerationDataArray::GetReturnValue(String *function)
	{
		bool dotNet = false;
		int index;
		String *retValue;
		InterceptedFunctionDB *db = InterceptedFunctionDB::GetInstance(FUNCTION_DB_FILE_NAME);
		InterceptedFunction *interceptedFunction = db->GetFunctionByName(function);

		if (!interceptedFunction)
		{
			// Try the .NET database
			db = InterceptedFunctionDB::GetInstance(DOT_NET_FUNC_DB_FILE_NAME);
			interceptedFunction = db->GetFunctionByName(function);
			
			if (interceptedFunction)
				dotNet = true;
			else
				return NULL;
		}
		
		if (dotNet)
		{
			// doesn't matter what we return here because the exception will be thrown and we won't return
			retValue = "0";
		}
		else
		{
			// get only error return values
			ArrayList *errorValues = new ArrayList();
			IEnumerator *enumerator = interceptedFunction->ReturnValues->GetEnumerator();
			while (enumerator->MoveNext())
			{
				InterceptedFunctionReturnValue *rv = dynamic_cast<InterceptedFunctionReturnValue *>(enumerator->Current);
				if (!rv)
					continue;

				if (rv->Type == ReturnValueType::Error)
					errorValues->Add(rv);
			}

			if (errorValues->Count == 0)
			{
				// this must be a void function or at least one with no specified error return values
				// go ahead and return 0 since we have no idea
				retValue = "0";
			}
			else
			{
				index = random->Next(0, errorValues->Count);
				InterceptedFunctionReturnValue *rv = dynamic_cast<InterceptedFunctionReturnValue *>(errorValues->get_Item(index));

				// return something based on the operator type and the value
				switch(rv->OperatorType)
				{
				case ReturnValueOperatorType::EqualTo:
					retValue = Convert::ToString(rv->ReturnValue);
					break;
				case ReturnValueOperatorType::GreaterThan:  // fall through intentional
				case ReturnValueOperatorType::GreaterThanOrEqualTo:
				case ReturnValueOperatorType::NotEqualTo:
					retValue = Convert::ToString(rv->ReturnValue + 1);
					break;
				case ReturnValueOperatorType::LessThan:  // fall through intentional
				case ReturnValueOperatorType::LessThanOrEqualTo:
					retValue = Convert::ToString(rv->ReturnValue - 1);
					break;
				default:
					retValue = Convert::ToString(rv->ReturnValue);
					break;
				}
			}
		}
		
		return retValue;
	}

	//*************************************************************************
	// Method:		LoadSettings
	// Description: Loads the test generation settings from an xml file
	//
	// Parameters:
	//	GenerationType - random or intelligent
	//	Intensity - high, medium, or low
	//
	// Return Value: true if success, false if fail
	//*************************************************************************
	bool TestGenerationDataArray::LoadSettings(GenerationType generationType, Intensity intensity)
	{
		try
		{
			XmlTextReader *xReader = new XmlTextReader(settingsFile);

			while (xReader->Read())
			{
				if (xReader->NodeType == XmlNodeType::Element)
				{
					if ( (xReader->Name->CompareTo("IntelligentStressTest") == 0) && (generationType != IntelligentStressTest) )
						xReader->Skip();
					else if ( (xReader->Name->CompareTo("RandomStressTest") == 0) && (generationType != RandomStressTest) )
						xReader->Skip();
					else if ( (xReader->Name->CompareTo("CodeCoverageTest") == 0) && (generationType != CodeCoverageTest) )
						xReader->Skip();
					else if	(xReader->Name->CompareTo("Intensity") == 0)
					{
						String *intensityValue = xReader->GetAttribute("value");
						if ( (intensityValue->CompareTo("Low") == 0) && (intensity != Low) )
							xReader->Skip();
						else if ( (intensityValue->CompareTo("Medium") == 0) && (intensity != Medium) )
							xReader->Skip();
						else if ( (intensityValue->CompareTo("High") == 0) && (intensity != High) )
							xReader->Skip();
					}
					else if (xReader->Name->CompareTo("TestInterval") == 0)
						testInterval = Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("TestDuration") == 0)
						testDuration = Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowNetworkLimit") == 0)
						allowNetworkLimit = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowDiskLimit") == 0)
						allowDiskLimit = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowMemoryLimit") == 0)
						allowMemoryLimit = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowNetworkFaults") == 0)
						allowNetworkFaults = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowRegistryFaults") == 0)
						allowRegistryFaults = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowProcessFaults") == 0)
						allowProcessFaults = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowCustomFaults") == 0)
						allowCustomFaults = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowDiskFaults") == 0)
						allowDiskFaults = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowMemoryFaults") == 0)
						allowMemoryFaults = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowResourceTests") == 0)
						allowResourceTests = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowTests") == 0)
						allowTests = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("AllowNetworkCorruption") == 0)
						allowNetworkCorruption = Convert::ToBoolean(xReader->ReadString());
					else if (xReader->Name->CompareTo("NumOverlappingTests") == 0)
						numOverlappingTests = Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("NetworkLowRange") == 0)
						networkLowRange = Convert::ToInt64(xReader->ReadString());
					else if (xReader->Name->CompareTo("NetworkHighRange") == 0)
						networkHighRange = Convert::ToInt64(xReader->ReadString());
					else if (xReader->Name->CompareTo("DiskLimitSet") == 0)
						diskLimitSet = (LimitSet) Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("DiskLimitMax") == 0)
						diskLimitMax = Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("MemoryLimitSet") == 0)
						memoryLimitSet = (LimitSet) Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("MemoryLimitMax") == 0)
						memoryLimitMax = Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("CorruptAmountMax") == 0)
						maxCorruptAmount = Convert::ToInt32(xReader->ReadString());
					else if (xReader->Name->CompareTo("LongStringLenMax") == 0)
						maxLongStringLen = Convert::ToInt32(xReader->ReadString());

				}
			}
		}
		catch (...)
		{
			//can't get from file, so load defaults
			switch (generationType)
			{
			case GenerationType::CodeCoverageTest:
				switch (intensity)
				{
				case Intensity::Low:
					allowNetworkLimit = true;
					allowDiskLimit = true;
					allowMemoryLimit = true;
					allowNetworkFaults = true;
					allowRegistryFaults = true;
					allowProcessFaults = true;
					allowCustomFaults = true;
					allowDiskFaults = true;
					allowMemoryFaults = true;
					allowResourceTests = false;		
					allowTests = false;
					break;
				case Intensity::Medium:
					allowNetworkLimit = true;
					allowDiskLimit = true;
					allowMemoryLimit = true;
					allowNetworkFaults = true;
					allowRegistryFaults = true;
					allowProcessFaults = true;
					allowCustomFaults = true;
					allowDiskFaults = true;
					allowMemoryFaults = true;
					allowResourceTests = true;
					allowTests = false;
					break;
				case Intensity::High:
					allowNetworkLimit = true;
					allowDiskLimit = true;
					allowMemoryLimit = true;
					allowNetworkFaults = true;
					allowRegistryFaults = true;
					allowProcessFaults = true;
					allowCustomFaults = true;
					allowDiskFaults = true;
					allowMemoryFaults = true;
					allowResourceTests = true;
					allowTests = true;
					break;
				}				
				break;
			case GenerationType::IntelligentStressTest:
				switch (intensity)
				{
				case Intensity::Low:
					testInterval = 60000;
					numOverlappingTests = 1;
					allowResourceTests = false;		
					break;
				case Intensity::Medium:
					testInterval = 30000;
					numOverlappingTests = 2;
					allowResourceTests = false;		
					break;
				case Intensity::High:
					testInterval = 15000;
					numOverlappingTests = 5;
					allowResourceTests = true;
					maxCorruptAmount = 200;
					maxLongStringLen = 1000;
					break;
				}				
				break;
			case GenerationType::RandomStressTest:
				switch (intensity)
				{
				case Intensity::Low:
					testInterval = 30000;
					testDuration = 1000;
					allowNetworkLimit = true;
					allowDiskLimit = false;
					allowMemoryLimit = false;
					allowNetworkFaults = true;
					allowRegistryFaults = true;
					allowProcessFaults = true;
					allowCustomFaults = true;
					allowDiskFaults = true;
					allowMemoryFaults = true;
					allowNetworkCorruption = false;
					numOverlappingTests = 1;
					networkLowRange = 500 * 1000; //500kbs
					networkHighRange = 10 * 1000 * 1000;  //10 mbs
					break;
				case Intensity::Medium:
					testInterval = 15000;
					testDuration = 2000;
					allowNetworkLimit = true;
					allowDiskLimit = true;
					allowMemoryLimit = true;
					allowNetworkFaults = true;
					allowRegistryFaults = true;
					allowProcessFaults = true;
					allowCustomFaults = true;
					allowDiskFaults = true;
					allowMemoryFaults = true;
					allowNetworkCorruption = true;
					numOverlappingTests = 1;
					networkLowRange = 56 * 1000;  //56kbs
					networkHighRange = 500 * 1000;  //500kbs
					diskLimitSet = OverCurrentUsage;
					diskLimitMax = 1024;
					memoryLimitSet = OverCurrentUsage;
					memoryLimitMax = 1024;
					maxCorruptAmount = 200;
					maxLongStringLen = 1000;
					break;
				case Intensity::High:
					testInterval = 15000;
					testDuration = 2000;
					allowNetworkLimit = true;
					allowDiskLimit = true;
					allowMemoryLimit = true;
					allowNetworkFaults = true;
					allowRegistryFaults = true;
					allowProcessFaults = true;
					allowCustomFaults = true;
					allowDiskFaults = true;
					allowMemoryFaults = true;
					allowNetworkCorruption = true;
					numOverlappingTests = 5;
					networkLowRange = 0;
					networkHighRange = 56 * 1000; //56kbs
					diskLimitSet = AtCurrentUsage;
					memoryLimitSet = AtCurrentUsage;
					maxCorruptAmount = 200;
					maxLongStringLen = 1000;
					break;
				}				
				break;
			}
			return false;
		}

		return true;
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
	void TestGenerationDataArray::onLogsGenerated(int numNewLogs, LogItem* newLogItems __gc[],
		unsigned int processID, unsigned int threadID)
	{
		try
		{
			HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(processID);

			if (recording)
			{
				UInt64 maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage;
				LogItem *logItem;
					
				if ( (allowDiskLimit) || (allowMemoryLimit) )
				{
					holoScriptApp->GetMonitorInformation(maximumDiskUsage, currentDiskUsage, maximumMemoryUsage, currentMemoryUsage);

					if (currentDiskUsage > maxDiskUsed)
						maxDiskUsed = currentDiskUsage;
					if (currentMemoryUsage > maxMemoryUsed)
						maxMemoryUsed = currentMemoryUsage;
					dataPoints++;
				}

				//add one entry per unique api
				for (int i = 0; i < numNewLogs; i++)
				{
					logItem = newLogItems[i];
					if ( (logItem) && (!apiList->Contains(logItem->Function)) )
						apiList->Add(logItem->Function, logItem);
				}
			}
			else if (currentCodeCoverageTest)
			{
				//check to see if we should run a test
				if (currentCodeCoverageTest->LogIndex <= holoScriptApp->LogEntryCount)
				{
					Object *args[] = __gc new Object*[1];
					DisplayableDataNode *dataNode = currentCodeCoverageTest->DataNode;
					testGenStatusBox->TestsLeft = this->Count;
					testGenStatusBox->DescriptionText = GetCodeCoverageDescriptionText(dataNode);
					this->DeleteDataNode(currentCodeCoverageTest);
					dataNode->processID = processID;
					dataNode->UpdateDelegates();
					args[0] = dataNode;
					restartMutex->WaitOne();
					dataNode->CreateDelegate->DynamicInvoke(args);

					//used the test so set to NULL
					currentCodeCoverageTest = NULL;

					testGenStatusBox->TestsLeft = this->Count;
					testGenStatusBox->TestsRun++;
					restartMutex->ReleaseMutex();
				}
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		UpdateArrayWithLogIndexes
	// Description: sets log indexes on each item in the dataarray
	//
	// Parameters:
	//	logEntryCount - number of logs generated during the recording code coverage run
	//
	// Return Value: none
	//*************************************************************************
	void TestGenerationDataArray::UpdateArrayWithLogIndexes(long logEntryCount)
	{
		Random *rand = new Random();

		for (int i=0; i<this->Count; i++)
		{
			TestGenerationDataNode *testGenNode = dynamic_cast <TestGenerationDataNode*> (this->GetDataNodeByIndex(i));
			if (!testGenNode)
				continue;

			testGenNode->LogIndex = rand->Next(logEntryCount);
		}

	}
	
	//*************************************************************************
	// Method:		ProjectRestarted
	// Description: called when the project has been restarted
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void TestGenerationDataArray::ProjectRestarted()
	{
		DisplayableDataNode *dataNode;
		TestGenerationDataNode *testGenNode;
		Object *args[] = __gc new Object*[1];
		HoloScriptApplication *holoScriptApp = NULL;
		unsigned int pID;

		try
		{
			codeCoverageTestRunning = true;

			if (holoScriptRegistry->Count != 0)
				holoScriptApp = holoScriptRegistry->getHoloScriptAppByIndex(0);

			if (!holoScriptApp)
			{
				CodeCoverageTestGenerationStop();
				projectRestartStarted = false;
				return;
			}

			holoScriptApp->GetApplicationProcessID(pID);
			
			if (!recording)
			{
				testGenStatusBox->DescriptionText = "Waiting to set a test";
				testGenNode = dynamic_cast <TestGenerationDataNode*> (GetDataNodeByIndex(0));

				if (testGenNode && testGenNode->DataNode)
				{
					currentCodeCoverageTest = testGenNode;
					
					//only fire the test at restart if log index is 0
					if (testGenNode->LogIndex == 0) 
					{
						dataNode = testGenNode->DataNode;
						testGenStatusBox->TestsLeft = this->Count;
						testGenStatusBox->DescriptionText = GetCodeCoverageDescriptionText(dataNode);
						this->DeleteDataNode(testGenNode);
						dataNode->processID = pID;
						dataNode->UpdateDelegates();
						args[0] = dataNode;
						restartMutex->WaitOne();
						dataNode->CreateDelegate->DynamicInvoke(args);

						//used the test so set to NULL
						currentCodeCoverageTest = NULL;

						testGenStatusBox->TestsLeft = this->Count;
						testGenStatusBox->TestsRun++;
						restartMutex->ReleaseMutex();
					}
				}
				else
					currentCodeCoverageTest = NULL;

				restartTimer->Change(codeCoverTestTimeout, Timeout::Infinite);
			}
			else
				testGenStatusBox->DescriptionText = "Observing application behavior until test process terminates";


			holoScriptApp->AddOnLogGeneratedHandler(logHandler);
			holoScriptApp->ResumeApplication();
			projectRestartStarted = false;
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		ProcessTerminated
	// Description: called when an AUT has been terminated
	//
	// Parameters:
	//	processID - id of the terminated process
	//
	// Return Value: none
	//*************************************************************************
	void TestGenerationDataArray::ProcessTerminated(unsigned int processID)
	{
		try
		{
			//deal with stress test gen first
			if (!holoScriptRegistry->AnyAppsRunning && stressTestRunning)
				StressTestGenerationStop();

			//now deal with code coverage
			if (codeCoverageTestRunning)
			{
				HoloScriptApplication *holoScriptApp = holoScriptRegistry->getHoloScriptAppForProcess(processID);
				if (!holoScriptApp)
					holoScriptApp = holoScriptRegistry->getHoloScriptAppForTerminatedProcess(processID);

				if (recording)
				{
					//if recording and process terminated then take care of all the test planning for the full run.
					LimitPaneDataNode *limitDataNode;
					TestGenerationDataNode *testGenNode;
				
					recording = false;

					if (allowDiskLimit)
					{
						limitDataNode = new LimitPaneDataNode("Max Disk", UIData::DiskLimit, 0, 0);
						if (maxDiskUsed != 0)
							limitDataNode->LimitValue = maxDiskUsed - 1;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						if (dataPoints > 0)
							avgDiskUsed = maxDiskUsed/dataPoints;
						limitDataNode = new LimitPaneDataNode("Avg Disk", UIData::DiskLimit, 0, 0);
						if (avgDiskUsed != 0)
							limitDataNode->LimitValue = avgDiskUsed - 1;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						limitDataNode = new LimitPaneDataNode("No Disk", UIData::DiskLimit, 0, 0);
						limitDataNode->LimitValue = 0;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);
					}
					if (allowMemoryLimit)
					{
						limitDataNode = new LimitPaneDataNode("Max Memory", UIData::MemoryLimit, 0, 0);
						if (maxMemoryUsed != 0)
							limitDataNode->LimitValue = maxMemoryUsed - 1;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						if (dataPoints > 0)
							avgMemoryUsed = maxMemoryUsed/dataPoints;
						limitDataNode = new LimitPaneDataNode("Avg Memory", UIData::MemoryLimit, 0, 0);
						if (avgMemoryUsed != 0)
							limitDataNode->LimitValue = avgMemoryUsed - 1;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						limitDataNode = new LimitPaneDataNode("No Memory", UIData::MemoryLimit, 0, 0);
						limitDataNode->LimitValue = 0;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);
					}
					if (allowNetworkLimit)
					{
						limitDataNode = new LimitPaneDataNode("28.8 Kb/sec", UIData::NetworkUpLimit, 0, 0);
						limitDataNode->LimitValue = 3600;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						limitDataNode = new LimitPaneDataNode("0 Kb/sec", UIData::NetworkUpLimit, 0, 0);
						limitDataNode->LimitValue = 0;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						limitDataNode = new LimitPaneDataNode("28.8 Kb/Sec", UIData::NetworkDownLimit, 0, 0);
						limitDataNode->LimitValue = 3600;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);

						limitDataNode = new LimitPaneDataNode("0 Kb/sec", UIData::NetworkDownLimit, 0, 0);
						limitDataNode->LimitValue = 0;
						testGenNode = new TestGenerationDataNode(limitDataNode);
						CreateDataNode(testGenNode);
					}

					if (allowTests)
					{
						for (int i=0; i<apiList->Count; i++)
						{
							LogItem *logItem = dynamic_cast <LogItem*> (apiList->GetByIndex(i));
							ScheduledTestDataNode *testDataNode = new ScheduledTestDataNode(logItem->Function, 0, 0);
							testDataNode->Function = logItem->Function;
							testDataNode->Category = logItem->Category;
							testDataNode->ReturnValue = GetReturnValue(logItem->Function);
							testDataNode->ErrorCode = GetErrorCode(logItem->Function);
							testDataNode->TestOwner = "Code Coverage Test Generator";
							if (testDataNode->ErrorCode == NULL)
								continue;

							testGenNode = new TestGenerationDataNode(testDataNode);
							CreateDataNode(testGenNode);
						}
					}

					if (allowResourceTests)
					{
						Fault *fault;
						String *keyString = String::Concat(__typeof(ResourcePaneDataArray)->Name, Convert::ToString(processID));
						keyString = String::Concat(keyString, "0");
						ResourcePaneDataArray *dataArray = dynamic_cast <ResourcePaneDataArray*> (dataArrayList->Item[keyString]);
						if (dataArray)
						{
							for (int resourceIndex=0; resourceIndex<dataArray->Count; resourceIndex++)
							{
								ResourcePaneDataNode *resourceDataNode = dynamic_cast <ResourcePaneDataNode*> (dataArray->GetDataNodeByIndex(resourceIndex));
								if (!resourceDataNode)
									continue;

								//pick a fault name
								switch (resourceDataNode->Type)
								{
								case File:
									fault = dynamic_cast <Fault*> (diskFaults->Item[random->Next(diskFaults->Count)]);
									break;
								case RegistryEntry:
									fault = dynamic_cast <Fault*> (registryFaults->Item[random->Next(registryFaults->Count)]);
									break;
								case Process:
									fault = dynamic_cast <Fault*> (processFaults->Item[random->Next(processFaults->Count)]);
									break;
								case ComObject:
									if (customFaults->Count > 0)
										fault = dynamic_cast <Fault*> (customFaults->Item[0]);
									break;
								}

								if (fault)
								{
									ResourceTestDataNode *testDataNode = new ResourceTestDataNode(resourceDataNode->Name, fault->Name, fault->Type, processID, 0);
									testGenNode = new TestGenerationDataNode(testDataNode);
									CreateDataNode(testGenNode);
								}
							}
						}
					}
					//call this to update nodes in the array with indexes between 0 and LogEntryCount
					UpdateArrayWithLogIndexes(holoScriptApp->LogEntryCount);
				}
				
				if (this->Count > 0)
				{
					//don't re-enter
					if (projectRestartStarted)
						return;

					projectRestartStarted = true;

					//Whether we are recording or not process termination means a restart if there are any tests left.
					testGenStatusBox->DescriptionText = "Restarting application";
					holoScriptApp->RemoveOnLogGeneratedHandler(logHandler);
					restartTimer->Change(Timeout::Infinite, Timeout::Infinite);
					restartMutex->WaitOne();
					dataEventRegistry->OnRestartProject->Invoke(true);
					restartMutex->ReleaseMutex();
				}
				else
				{
					//if no tests left then testing is complete
					this->CodeCoverageTestGenerationStop();
				}
			}
		}
		catch(...)
		{
		}
	}

	//*************************************************************************
	// Method:		GetCodeCoverageDescriptionText
	// Description: creates a string to send to the test gen status dlg box
	//
	// Parameters:
	//	dataNode - node representing the test we are about to create
	//
	// Return Value: none
	//*************************************************************************
	String *TestGenerationDataArray::GetCodeCoverageDescriptionText(DisplayableDataNode *dataNode)
	{
		switch (dataNode->Type)
		{
		case MemoryFault:
			return "Setting a Memory Fault that will last until process terminates";
		case DiskFault:
			return "Setting a Disk Fault that will last until process terminates";
		case NetworkFault:
			return "Setting a Network Fault that will last until process terminates";
		case RegistryFault:
			return "Setting a Registry Fault that will last until process terminates";
		case ProcessFault:
			return "Setting a Process Fault that will last until process terminates";
		case CustomFault:
			return "Setting a Custom Fault that will last until process terminates";
		case NetworkCorruptionFault:
			return "Setting a Network Fuzzing Fault that will last until process terminates";
		case FileCorruptionFault:
			return "Setting a File Fuzzing Fault that will last until process terminates";
		case DiskLimit:
			return "Setting a Disk Limit that will last until process terminates";
		case MemoryLimit:
			return "Setting a Memory Limit that will last until process terminates";
		case NetworkUpLimit:
		case NetworkDownLimit:
			return "Setting a Network Limit that will last until process terminates";
		case Test:
			return "Setting an API Test that will last until process terminates";
		case ResourceFault:
			return "Setting a Resource Based Test that will last until process terminates";
		default:
			return "Running a test until process terminates";
		}
	}
}