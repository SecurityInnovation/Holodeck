//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		SettingsManager.cpp
//
// DESCRIPTION: Contains implementation for the class SettingsManager.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 16 DEC 2002		 B. Shirey	 File created.
//*************************************************************************

#include <time.h>
#include "settingsmanager.h"
#include "registry.h"
#include "FunctionTest.h"
#include "ReplacementLibrary.h"

namespace Replacement
{
	HANDLE SettingsManager::heap = NULL;
	DWORD64 SettingsManager::currentUploadUsage = 0;
	DWORD64 SettingsManager::currentDownloadUsage = 0;
	DWORD SettingsManager::sendUpdateStamp = 0;
	DWORD SettingsManager::recvUpdateStamp = 0;

	//*************************************************************************
	// Method:		SettingsManager
	// Description: Constructor for the SettingsManager class
	//
	// Parameters:
	//	processId - The id of the process this is running under
	//	deletePreviousKey - true to delete the previous registry key for the
	//		processID
	//
	// Return Value: None
	//*************************************************************************
	SettingsManager::SettingsManager(int processId, bool deletePreviousKey) : programRootPath(ROOT_PATH)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		srand(time(NULL));
		Initialize(processId, deletePreviousKey);
	}

	//*************************************************************************
	// Method:		~SettingsManager
	// Description: Destructor for the SettingsManager class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	SettingsManager::~SettingsManager()
	{
		if (registryMonitorThread)
		{
			registryMonitorRunning = false;
			WaitForSingleObject(registryMonitorThread, INFINITE);
		}

		DeleteCriticalSection(&functionTestLock);
		DeleteCriticalSection(&disabledLogLock);
		DeleteCriticalSection(&corruptLock);

		// delete registry entries for the current process id
		Registry registry(ROOT_KEY);
		registry.DeleteKeyRecursively(ROOT_KEY, programRootPath);
	}

	//*************************************************************************
	// Method:		operator new
	// Description: Allocates the memory for the object
	//
	// Parameters:
	//	size - The number of bytes to allocate
	//
	// Return Value: the pointer to the allocated memory
	//*************************************************************************
	void *SettingsManager::operator new(size_t size)
	{
		if (heap == NULL)
			heap = HeapCreate(0, 0, 0);

		void *p = HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
		return p;
	}

	//*************************************************************************
	// Method:		operator delete
	// Description: Deallocates the memory for the object
	//
	// Parameters:
	//	p - The pointer to memory to delete
	//
	// Return Value: None
	//*************************************************************************
	void SettingsManager::operator delete(void *p)
	{
		if (p != NULL)
		{
			HeapFree(heap, HEAP_ZERO_MEMORY, p);
			p = NULL;
		}
	}

	//*************************************************************************
	// Method:		SetProjectPath
	// Description: Sets the project path
	//
	// Parameters:
	//	path - Project path
	//
	// Return Value: None
	//*************************************************************************
	void SettingsManager::SetProjectPath(const SiString& path)
	{
		char fullPath[MAX_PATH];
		GetFullPathName((char*)path, MAX_PATH, fullPath, NULL);

		projectPath = fullPath;

		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;

		// make sure the key exists
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(regPath))
			registry.CreateKey(ROOT_KEY, regPath);

		registry.OpenKey(ROOT_KEY, regPath);
		registry.Write(PROJECT_PATH_VALUE_NAME, projectPath);
		registry.Close();

		int index = projectPath.FindLast('\\');
		SiString projectName;
		if (index > 0)
		{
			projectPath.SubString(projectFolder, 0, index);
			projectPath.SubString(projectName, index + 1);
		}
		else
		{
			projectFolder = "";
			projectName = projectFolder;
		}

		index = projectName.FindLastIgnoreCase(".hdp");
		if (index > 0)
			projectName.SubString(projectTitle, 0, index - 1);
		else
			projectTitle = projectName;
	}

	//*************************************************************************
	// Method:		GetProjectPath
	// Description: Gets the project path
	//
	// Parameters:
	//	path - Receives project path
	//
	// Return Value: true on success
	//*************************************************************************
	bool SettingsManager::GetProjectPath(SiString& path)
	{
		EnterCriticalSection(&corruptLock);
		path = projectPath;
		LeaveCriticalSection(&corruptLock);
		return true;
	}

	//*************************************************************************
	// Method:		GetProjectFolder
	// Description: Gets the project folder
	//
	// Parameters:
	//	path - Receives project folder
	//
	// Return Value: true on success
	//*************************************************************************
	bool SettingsManager::GetProjectFolder(SiString& folder)
	{
		EnterCriticalSection(&corruptLock);
		folder = projectFolder;
		LeaveCriticalSection(&corruptLock);
		return true;
	}

	//*************************************************************************
	// Method:		GetProjectTitle
	// Description: Gets the project title
	//
	// Parameters:
	//	path - Receives project title
	//
	// Return Value: true on success
	//*************************************************************************
	bool SettingsManager::GetProjectTitle(SiString& title)
	{
		EnterCriticalSection(&corruptLock);
		title = projectTitle;
		LeaveCriticalSection(&corruptLock);
		return true;
	}

	//*************************************************************************
	// Method:		GetDTDFolder
	// Description: Gets the DTD folder
	//
	// Parameters:
	//	path - Receives DTD folder
	//
	// Return Value: true on success
	//*************************************************************************
	bool SettingsManager::GetDTDFolder(SiString& folder)
	{
		Registry registry(ROOT_KEY);
		if (!registry.OpenKey(ROOT_KEY, ROOT_PATH))
			return false;

		if (!registry.Read(DTD_PATH_VALUE_NAME, folder))
			return false;

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		Initialize
	// Description: Initializes the data structures for the class
	//
	// Parameters:
	//	processId - The id of the process this is running under
	//	deletePreviousKey - true to delete the previous registry key for the
	//		processID
	//
	// Return Value: None
	//*************************************************************************
	void SettingsManager::Initialize(int processId, bool deletePreviousKey)
	{
		wchar_t *LogPipeName;
		wchar_t pidString[10];
		SiString pipeNameString;

		// get the process id in a string
		_itow (processId, pidString, 10);

		programRootPath = ROOT_PATH;
		programRootPath += "\\";
		programRootPath += pidString;

		Registry registry;

		// clear out the old registry key and prepare a new one
		if (deletePreviousKey)
			registry.DeleteKeyRecursively(ROOT_KEY, programRootPath);

		registry.CreateKey(ROOT_KEY, programRootPath);

		// create a unique pipe name
		UUID uuid;
		UuidCreate(&uuid);
		UuidToStringW(&uuid, (wchar_t **)&LogPipeName);

		pipeNameString = LogPipeName;
		registry.Write(LOG_PIPE_VALUE_NAME, pipeNameString);
		RpcStringFreeW((wchar_t **) &LogPipeName);

		InitializeCriticalSection(&functionTestLock);
		InitializeCriticalSection(&disabledLogLock);
		InitializeCriticalSection(&corruptLock);

		maxDiskUsageSet = false;
		maxMemUsageSet = false;
		netUploadBandwidthSet = false;
		netDownloadBandwidthSet = false;
		currentMemUsage = 0;
		currentDiskUsage = 0;

		SiString pauseEventName;
		pauseEventName = PAUSE_EVENT_NAME_PREFIX;
		pauseEventName += pidString;
		pauseEvent.Open(pauseEventName);

		// Read the settings from the registry if we didn't delete them
		if (!deletePreviousKey)
		{
			ReadFromRegistry();

			registryMonitorRunning = true;
			registryMonitorThread = ReplacementLibrary::CreateAutThread(WaitForRegistryChangeThread, this, NULL);
		}
		else
			registryMonitorRunning = false;
	}


	//*************************************************************************
	// Method:		GetFunctionTests
	// Description: Determines whether or not a given function has a test 
	//				associated with it, and if it does, gives the list to the
	//				caller.  If the only test that exists is a direct fault
	//				or there are no tests the function will return false
	//
	// Parameters:
	//	targetThreadID (IN) - The thread from which to get the tests
	//	category (IN) - A string representing the category of the function
	//  functionName (IN) - The name of the function to test for direct faults
	//  testList (OUT) - The list of tests associated with the function
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetFunctionTests(int targetThreadID, const char *category, const char *functionName, FunctionTestList &testList)
	{
		EnterCriticalSection(&functionTestLock);

		if (!functionTestTable.KeyExists(functionName))
		{
			LeaveCriticalSection(&functionTestLock);
			return false;
		}

		for (unsigned int i=0; i<functionTestTable[functionName].GetLength(); i++)
		{
			if (functionTestTable[functionName][i].threadId == targetThreadID)
			{
				for (int j=0; j<functionTestTable[functionName][i].tests.GetTestCount(); j++)
					testList.AddTest(functionTestTable[functionName][i].tests.GetTestAt(j));
				LeaveCriticalSection(&functionTestLock);
				return true;
			}
		}
		LeaveCriticalSection(&functionTestLock);
		return false;
	}

	//*************************************************************************
	// Method:		GetFunctionCallCount
	// Description: Gets the number of times the function has been called for the
	//				specified thread and the specified testID
	//
	// Parameters:
	//  testIDPath (IN) - The registry path of the testID
	//
	// Return Value: (int) the call count, 0 if reg key not found
	//*************************************************************************
	int SettingsManager::GetFunctionCallCount (const SiString &testIDPath)
	{
		// check the test key for existence
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(testIDPath))
			return -1;

		int callCount = 0;
		//Read the value
		if (registry.Read(FUNCTION_CALL_COUNT_VALUE_NAME, callCount))
			return callCount;
		else
			return 0;
	}

	//*************************************************************************
	// Method:		IncrementFunctionCallCount
	// Description: Gets the number of times the function has been called for the
	//				specified thread and the specified testID
	//
	// Parameters:
	//  testIDPath (IN) - The registry path of the testID
	//
	// Return Value: (int) the incremented call count, -1 if error
	//*************************************************************************
	int SettingsManager::IncrementFunctionCallCount (const SiString &testIDPath)
	{
		// check the test key for existence
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(testIDPath))
			return -1;

		//Get the old callcount
		int callCount = this->GetFunctionCallCount (testIDPath);
		//Increment the callcount
		callCount++;
		//Write the new value
		registry.Write(FUNCTION_CALL_COUNT_VALUE_NAME, callCount);
		return callCount;
	}

	//*************************************************************************
	// Method:		ReadFunctionTestsFromRegistry
	// Description: Determines whether or not a given function has a test 
	//				associated with it, and if it does, gives the list to the
	//				caller.  If the only test that exists is a direct fault
	//				or there are no tests the function will return false.
	//				This function reads the settings out of the registry
	//				rather than out of the internal hash tables.
	//
	// Parameters:
	//	targetThreadID (IN) - The thread from which to get the tests
	//	category (IN) - A string representing the category of the function
	//  functionName (IN) - The name of the function to test for direct faults
	//  testList (OUT) - The list of tests associated with the function
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::ReadFunctionTestsFromRegistry(const SiString &targetThreadID, const SiString &category, const SiString &functionName, FunctionTestList &testList)
	{
		// get the test root key
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += targetThreadID;
		path += "\\";
		path += FAULTS_KEY_NAME;
		path += "\\";
		path += category;
		path += "\\";
		path += functionName;

		// check the test key for existence
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(path))
			return false;

		int numKeys = registry.GetSubKeyCount(ROOT_KEY, path);

		// return false if there are no tests
		if (numKeys == 0)
			return false;

		for (int i = 0; i < numKeys; i++)
		{
			SiString testID;
			if (!registry.GetSubKeyNameAt(ROOT_KEY, path, i, testID))
				return false;

			// get the test path
			SiString testIdPath = path;
			testIdPath += "\\";
			testIdPath += testID;
			registry.OpenKey(ROOT_KEY, testIdPath);

			// get the size of the test's data
			int size, valueType;
			if (!registry.GetValueSizeAndType("", size, valueType))
				return false;

			BYTE *testBytes = (BYTE*)HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
			DWORD bytesRead = size;
			registry.Read("", testBytes, bytesRead);

			if ((bytesRead != size) || (bytesRead == 0))
			{
				// Could not read test
				HeapFree(heap, 0, testBytes);
				return false;
			}

			int curOffset = 0;

			// make a new test
			FunctionTest *test = new FunctionTest();

			// set the test string
			test->SetTestID(testID);

			// set the test registry path
			test->SetRegistryPath (testIdPath);

			// set the return value string
			int strLength;
			CopyMemory(&strLength, testBytes + curOffset, sizeof(int));
			curOffset += sizeof(int);
			test->SetReturnValue((wchar_t*)(testBytes + curOffset));
			curOffset += strLength;

			// set the error code string
			CopyMemory(&strLength, testBytes + curOffset, sizeof(int));
			curOffset += sizeof(int);
			test->SetErrorCode((wchar_t*)(testBytes + curOffset));
			curOffset += strLength;

			// set the exception string
			CopyMemory(&strLength, testBytes + curOffset, sizeof(int));
			curOffset += sizeof(int);
			test->SetException((wchar_t*)(testBytes + curOffset));
			curOffset += strLength;

			// set the allocation string
			test->SetAllocation(*(AllocationType*)(testBytes + curOffset));
			curOffset += sizeof(AllocationType);

			// set the passthrough string
			test->SetPassThrough(*(bool*)(testBytes + curOffset));
			curOffset += sizeof(bool);

			// set the passthrough return value string
			test->SetPassThroughReturnValue(*(bool*)(testBytes + curOffset));
			curOffset += sizeof(bool);

			// set the passthrough error code string
			test->SetPassThroughErrorCode(*(bool*)(testBytes + curOffset));
			curOffset += sizeof(bool);

			// set the check resource values
			CheckResource checkResource;
			checkResource.ParamIndex = *(int*)(testBytes + curOffset);
			curOffset += sizeof(int);
			checkResource.Exists = *(ExistsType*)(testBytes + curOffset);
			curOffset += sizeof(ExistsType);
			test->SetCheckResource(checkResource);

			// get the number of match parameters - each one is in a subkey of matchParamPath
			int numMatchParams = *(int*)(testBytes + curOffset);
			curOffset += sizeof(int);

			// get the match parameters
			for (int j = 0; j < numMatchParams; j++)
			{
				int position = *(int*)(testBytes + curOffset);
				curOffset += sizeof(int);

				int matchIndex = *(int*)(testBytes + curOffset);
				curOffset += sizeof(int);

				// get the param type
				ParameterType type = *(ParameterType*)(testBytes + curOffset);
				curOffset += sizeof(ParameterType);

				ParameterOperatorType operatorType = *(ParameterOperatorType*)(testBytes + curOffset);
				curOffset += sizeof(ParameterOperatorType);

				// get the size of the parameter's value
				int size = *(int*)(testBytes + curOffset);
				curOffset += sizeof(int);

				test->AddParameterToMatch(position, matchIndex, testBytes + curOffset, size, type, operatorType);
				curOffset += size;
			}

			// get the number of change parameters - each one is in a subkey of changeParamPath
			int numChangeParams = *(int*)(testBytes + curOffset);
			curOffset += sizeof(int);

			// get the change parameters
			for (int j = 0; j < numChangeParams; j++)
			{
				int position = *(int*)(testBytes + curOffset);
				curOffset += sizeof(int);

				// get the param type
				ParameterType type = *(ParameterType*)(testBytes + curOffset);
				curOffset += sizeof(ParameterType);

				// get the size of the parameter's value
				int size = *(int*)(testBytes + curOffset);
				curOffset += sizeof(int);

				test->AddParameterToChange(position, testBytes + curOffset, size, type);
				curOffset += size;
			}

			//get the firing details
			int firingOption = *(int*)(testBytes + curOffset);
			curOffset += sizeof(int);

			bool pauseAppAfterTestFire = *(bool*)(testBytes + curOffset);
			curOffset += sizeof(bool);

			SiArray<SiString> firingData;

			int fdCount = *(int*)(testBytes + curOffset);
			curOffset += sizeof(int);
			for (int j = 0; j < fdCount; j++)
			{
				CopyMemory(&strLength, testBytes + curOffset, sizeof(int));
				curOffset += sizeof(int);
				firingData.Add((wchar_t*)(testBytes + curOffset));
				curOffset += strLength;
			}

			//Set the firing option
			test->SetFiringOption (firingOption);
			//Set the pause on app option
			test->SetPauseAppOnTestFire (pauseAppAfterTestFire);
			//Set the firing data
			test->SetFiringDetails (firingData);

			// add the test to the list
			testList.AddTest(test);

			HeapFree(heap, 0, testBytes);
		}

		return true;
	}

	//*************************************************************************
	// Method:		AddFunctionTest
	// Description: Associates a test with a given function in a specific
	//	category
	//
	// Parameters:
	//	targetThreadID (IN) - The thread for which to add the test
	//	category (IN) - A string representing the category of the function
	//  functionName (IN) - The name of the function to test for direct faults
	//  test (IN) - The test to associate with this function and category
	//	testID (IN) - The unique id of this test
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::AddFunctionTest(const SiString &targetThreadID, const SiString &category, const SiString &functionName, FunctionTest *test, SiString *testID)
	{
		if ((test == NULL) || (testID == NULL))
			return false;

		// testID key
		SiString testIdPath = programRootPath;
		testIdPath += "\\";
		testIdPath += TEST_SETTINGS_KEY;
		testIdPath += "\\";
		testIdPath += targetThreadID;
		testIdPath += "\\";
		testIdPath += FAULTS_KEY_NAME;
		testIdPath += "\\";
		testIdPath += category;
		testIdPath += "\\";
		testIdPath += functionName;
		testIdPath += "\\";
		testIdPath += *testID;

		// Calcuate total size of the test data
		int totalTestBytes = 0;
		// add in return value string
		totalTestBytes += sizeof(int) + ((test->GetReturnValue().GetLength() + 1) * sizeof(wchar_t));
		// add in error code string
		totalTestBytes += sizeof(int) + ((test->GetErrorCode().GetLength() + 1) * sizeof(wchar_t));
		// add in exception string
		totalTestBytes += sizeof(int) + ((test->GetException().GetLength() + 1) * sizeof(wchar_t));
		// add in allocation
		totalTestBytes += sizeof(AllocationType);
		// add in pass through
		totalTestBytes += sizeof(bool);
		// add in pass through return value
		totalTestBytes += sizeof(bool);
		// add in pass through error code
		totalTestBytes += sizeof(bool);
		// add in the check resource values
		totalTestBytes += sizeof(int) + sizeof(ExistsType);
		// add in match parameters
		totalTestBytes += sizeof(int); // parmeter count
		for (int i = 0; i < test->GetMatchParameterCount(); i++)
		{
			// add in position, match index, and types
			totalTestBytes += sizeof(int) * 2 + sizeof(ParameterType) + sizeof(ParameterOperatorType);
			// add in data
			totalTestBytes += sizeof(int) + test->GetParameterToMatchAt(i).valueSize;
		}
		// add in the change parameters
		totalTestBytes += sizeof(int); // parmeter count
		for (int i = 0; i < test->GetChangeParameterCount(); i++)
		{
			// add in position and type
			totalTestBytes += sizeof(int) + sizeof(ParameterType);
			// add in data
			totalTestBytes += sizeof(int) + test->GetParameterToChangeAt(i).valueSize;
		}
		// add in firing details
		totalTestBytes += sizeof(int) + sizeof(bool);
		// add in firing data
		totalTestBytes += sizeof(int); // array length
		SiArray<SiString> firingData = test->GetFiringDetails();
		for (int i=0; i < (int) firingData.GetLength(); i++)
			totalTestBytes += sizeof(int) + ((firingData[i].GetLength() + 1) * sizeof(wchar_t));

		// Create a buffer to hold the output
		BYTE* testBytes = new BYTE[totalTestBytes];
		int curOffset = 0;

		// write the return value string
		int strLength = (test->GetReturnValue().GetLength() + 1) * sizeof(wchar_t);
		CopyMemory(testBytes + curOffset, &strLength, sizeof(int));
		curOffset += sizeof(int);
		CopyMemory(testBytes + curOffset, (wchar_t*)test->GetReturnValue(), strLength);
		curOffset += strLength;

		// write the error code string
		strLength = (test->GetErrorCode().GetLength() + 1) * sizeof(wchar_t);
		CopyMemory(testBytes + curOffset, &strLength, sizeof(int));
		curOffset += sizeof(int);
		CopyMemory(testBytes + curOffset, (wchar_t*)test->GetErrorCode(), strLength);
		curOffset += strLength;

		// write the exception string
		strLength = (test->GetException().GetLength() + 1) * sizeof(wchar_t);
		CopyMemory(testBytes + curOffset, &strLength, sizeof(int));
		curOffset += sizeof(int);
		CopyMemory(testBytes + curOffset, (wchar_t*)test->GetException(), strLength);
		curOffset += strLength;

		// write the allocation
		AllocationType allocation = test->GetAllocation();
		CopyMemory(testBytes + curOffset, &allocation, sizeof(AllocationType));
		curOffset += sizeof(AllocationType);

		// write the pass through
		bool passThrough = test->GetPassThrough();
		CopyMemory(testBytes + curOffset, &passThrough, sizeof(bool));
		curOffset += sizeof(bool);

		// write the pass through return value
		bool passThroughReturnValue = test->GetPassThroughReturnValue();
		CopyMemory(testBytes + curOffset, &passThroughReturnValue, sizeof(bool));
		curOffset += sizeof(bool);

		// write the pass through error code
		bool passThroughErrorCode = test->GetPassThroughErrorCode();
		CopyMemory(testBytes + curOffset, &passThroughErrorCode, sizeof(bool));
		curOffset += sizeof(bool);

		//write the check resource values
		CopyMemory(testBytes + curOffset, &test->GetCheckResource().ParamIndex, sizeof(int));
		curOffset += sizeof(int);
		CopyMemory(testBytes + curOffset, &test->GetCheckResource().Exists, sizeof(ExistsType));
		curOffset += sizeof(ExistsType);

		// write the match parameters
		int count = test->GetMatchParameterCount();
		CopyMemory(testBytes + curOffset, &count, sizeof(int));
		curOffset += sizeof(int);
		for (int i = 0; i < test->GetMatchParameterCount(); i++)
		{
			Parameter param = test->GetParameterToMatchAt(i);

			CopyMemory(testBytes + curOffset, &param.position, sizeof(int));
			curOffset += sizeof(int);

			CopyMemory(testBytes + curOffset, &param.matchIndex, sizeof(int));
			curOffset += sizeof(int);

			CopyMemory(testBytes + curOffset, &param.type, sizeof(ParameterType));
			curOffset += sizeof(ParameterType);

			CopyMemory(testBytes + curOffset, &param.operatorType, sizeof(ParameterOperatorType));
			curOffset += sizeof(ParameterOperatorType);

			CopyMemory(testBytes + curOffset, &param.valueSize, sizeof(int));
			curOffset += sizeof(int);

			CopyMemory(testBytes + curOffset, param.value, param.valueSize);
			curOffset += param.valueSize;
		}

		// write the change parameters
		count = test->GetChangeParameterCount();
		CopyMemory(testBytes + curOffset, &count, sizeof(int));
		curOffset += sizeof(int);
		for (int i = 0; i < test->GetChangeParameterCount(); i++)
		{
			Parameter param = test->GetParameterToChangeAt(i);

			CopyMemory(testBytes + curOffset, &param.position, sizeof(int));
			curOffset += sizeof(int);

			CopyMemory(testBytes + curOffset, &param.type, sizeof(ParameterType));
			curOffset += sizeof(ParameterType);

			CopyMemory(testBytes + curOffset, &param.valueSize, sizeof(int));
			curOffset += sizeof(int);

			CopyMemory(testBytes + curOffset, param.value, param.valueSize);
			curOffset += param.valueSize;
		}

		//write the firing details
		int firingOption = test->GetFiringOption();
		CopyMemory(testBytes + curOffset, &firingOption, sizeof(int));
		curOffset += sizeof(int);

		bool pauseAppOnTestFire = test->GetPauseAppOnTestFire();
		CopyMemory(testBytes + curOffset, &pauseAppOnTestFire, sizeof(bool));
		curOffset += sizeof(bool);

		count = firingData.GetLength();
		CopyMemory(testBytes + curOffset, &count, sizeof(int));
		curOffset += sizeof(int);
		for (int i=0; i < (int) firingData.GetLength(); i++)
		{
			strLength = (firingData[i].GetLength() + 1) * sizeof(wchar_t);
			CopyMemory(testBytes + curOffset, &strLength, sizeof(int));
			curOffset += sizeof(int);
			CopyMemory(testBytes + curOffset, (wchar_t*)firingData[i], strLength);
			curOffset += strLength;
		}

		Registry registry(ROOT_KEY);
		registry.CreateKey(ROOT_KEY, testIdPath);
		registry.Write("", testBytes, curOffset);
		registry.Close();

		functionTestKeys.Add(*testID, testIdPath);

		delete[] testBytes;
		return true;
	}

	//*************************************************************************
	// Method:		DoesBooleanParamMatch
	// Description: Determines whether or not a boolean param matches the test
	//	based on the operator specified
	//
	// Parameters:
	//	paramValue - the parameter value to test
	//	testValue - the test value to compare
	//  operatorType - the type of operator to use for the compare
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesBooleanParamMatch(bool paramValue, bool testValue, ParameterOperatorType operatorType)
	{
		// check for ==, !=.  All other operators have no meaning for boolean types
		switch (operatorType)
		{
		case EqualsType:
			return paramValue == testValue;
		case NotEqualToType:
			return paramValue != testValue;
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		DoesIntegerParamMatch
	// Description: Determines whether or not an integer param matches the test
	//	based on the operator specified
	//
	// Parameters:
	//	paramValue - the parameter value to test
	//	testValue - the test value to compare
	//  operatorType - the type of operator to use for the compare
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesIntegerParamMatch(int paramValue, int testValue, ParameterOperatorType operatorType)
	{
		// check for ==, !=, <, >, <=, >=.  All other operators have no meaning for integer types
		switch (operatorType)
		{
		case EqualsType:
			return paramValue == testValue;
		case NotEqualToType:
			return paramValue != testValue;
		case LessThanType:
			return paramValue < testValue;
		case GreaterThanType:
			return paramValue > testValue;
		case LessThanOrEqualType:
			return paramValue <= testValue;
		case GreaterThanOrEqualType:
			return paramValue >= testValue;
		case ContainsBinaryType:
			return ( (paramValue & testValue) == testValue);
		case NotContainsBinaryType:
			return !( (paramValue & testValue) == testValue);
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		DoesStringParamMatch
	// Description: Determines whether or not a string param matches the test
	//	based on the operator specified
	//
	// Parameters:
	//	pos - the param position
	//	paramValue - the parameter value to test
	//	testValue - the test value to compare
	//  operatorType - the type of operator to use for the compare
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesStringParamMatch(const SiString &paramValue, const SiString &testValue, ParameterOperatorType operatorType)
	{
		// check for ==, !=, <, >, <=, >=, starts with, ends with, substring.  All other operators have no meaning for string types
		switch (operatorType)
		{
		case EqualsResourceType:
		case EqualsType:
			return paramValue.CompareToIgnoreCase(testValue) == 0;
		case NotEqualToResourceType:
		case NotEqualToType:
			return paramValue.CompareToIgnoreCase(testValue) != 0;
		case LessThanType:
			return paramValue.CompareToIgnoreCase(testValue) < 0;
		case GreaterThanType:
			return paramValue.CompareToIgnoreCase(testValue) > 0;
		case LessThanOrEqualType:
			return paramValue.CompareToIgnoreCase(testValue) <= 0;
		case GreaterThanOrEqualType:
			return paramValue.CompareToIgnoreCase(testValue) >= 0;
		case StartsWithResourceType:
		case StartsWithType:
		{
			SiString subString;
			int endPos = testValue.GetLength();
			if (!((SiString)paramValue).SubString(subString, 0, endPos - 1))
				return false;

			return subString.CompareToIgnoreCase(testValue) == 0;
		}
		case EndsWithType:
		{
			SiString subString;
			int startPos = paramValue.GetLength() - testValue.GetLength();
			if (startPos < 0)
				return false;

			if (!((SiString)paramValue).SubString(subString, startPos))
				return false;

			return subString.CompareToIgnoreCase(testValue) == 0;
		}
		case ContainsResourceType:
		case ContainsSubstringType:
			return ((SiString)paramValue).FindFirstIgnoreCase(testValue) != -1;
		case NotContainsResourceType:
		case NotContainsSubstringType:
			return ((SiString)paramValue).FindFirstIgnoreCase(testValue) == -1;
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		DoesRealParamMatch
	// Description: Determines whether or not a real param matches the test
	//	based on the operator specified
	//
	// Parameters:
	//	paramValue - the parameter value to test
	//	testValue - the test value to compare
	//  operatorType - the type of operator to use for the compare
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesRealParamMatch(float paramValue, float testValue, ParameterOperatorType operatorType)
	{
		// check for ==, !=, <, >, <=, >=.  All other operators have no meaning for real types
		switch (operatorType)
		{
		case EqualsType:
			return paramValue == testValue;
		case NotEqualToType:
			return paramValue != testValue;
		case LessThanType:
			return paramValue < testValue;
		case GreaterThanType:
			return paramValue > testValue;
		case LessThanOrEqualType:
			return paramValue <= testValue;
		case GreaterThanOrEqualType:
			return paramValue >= testValue;
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		DoesPointerParamMatch
	// Description: Determines whether or not a pointer param matches the test
	//	based on the operator specified
	//
	// Parameters:
	//	paramValue - the parameter value to test
	//	testValue - the test value to compare
	//  operatorType - the type of operator to use for the compare
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesPointerParamMatch(unsigned long paramValue, unsigned long testValue, ParameterOperatorType operatorType)
	{
		// check for ==, !=, <, >, <=, >=.  All other operators have no meaning for pointer types
		switch (operatorType)
		{
		case EqualsType:
			return paramValue == testValue;
		case NotEqualToType:
			return paramValue != testValue;
		case LessThanType:
			return paramValue < testValue;
		case GreaterThanType:
			return paramValue > testValue;
		case LessThanOrEqualType:
			return paramValue <= testValue;
		case GreaterThanOrEqualType:
			return paramValue >= testValue;
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		DoesInteger64ParamMatch
	// Description: Determines whether or not a 64-bit int param matches the test
	//	based on the operator specified
	//
	// Parameters:
	//	paramValue - the parameter value to test
	//	testValue - the test value to compare
	//  operatorType - the type of operator to use for the compare
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesInteger64ParamMatch(__int64 paramValue, __int64 testValue, ParameterOperatorType operatorType)
	{
		// check for ==, !=, <, >, <=, >=.  All other operators have no meaning for 64-bit integer types
		switch (operatorType)
		{
		case EqualsType:
			return paramValue == testValue;
		case NotEqualToType:
			return paramValue != testValue;
		case LessThanType:
			return paramValue < testValue;
		case GreaterThanType:
			return paramValue > testValue;
		case LessThanOrEqualType:
			return paramValue <= testValue;
		case GreaterThanOrEqualType:
			return paramValue >= testValue;
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		DoesTestMatch
	// Description: Determines whether or not a list of parameters matches
	//				the tests for a function.
	//
	// Parameters:
	//	parameters - A list of pointers to the parameters of a function.  Note:
	//		when creating this list, all char* and wchar_t* parameters should
	//		be cast to SiUtils::String and added to the list as such.
	//  numParameters - The number of parameters
	//	considerOutParams - if false, ignore outparams as they could contain garbage
	//	resourcePathList - contains a list of full resource paths in case we are doing a resource test
	//  testList - The list of tests to try to match
	//	matchingErrorCode - The error code to apply if the test matches
	//	matchingReturnValue - The return value to apply if the test matches
	//  matchingTestIndex - the index in the test list that matched the parameters
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DoesTestMatch(void **parameters, int numParameters, bool considerOutParams, SiArray<SiString> *resourcePathList, FunctionTestList &testList, SiString &matchingErrorCode, SiString &matchingReturnValue, unsigned long allocInfo, bool positiveAllocation, int *matchingTest)
	{
		bool oneMatches = false;

		// loop through the tests
		for (int i = 0; (!oneMatches) && (i < testList.GetTestCount()); i++)
		{
			// only one test needs to match for the function to return true
			bool thisMatches = true;
			FunctionTest *test = (FunctionTest *)testList.GetTestAt(i);

			//check resource
			CheckResource checkResource = test->GetCheckResource();
			//bugbug - sometimes this structure is getting corrupted when applying a memory fault - why?
			if ( (checkResource.ParamIndex >= 0) && (checkResource.Exists != NAEXISTS) && (checkResource.Exists != CREATE) )
			{
				bool resourceFound = false;
				bool checkExists;
				SiString resourceName = *((SiString *) parameters[checkResource.ParamIndex]);

				if (GetFileAttributes(resourceName) != INVALID_FILE_ATTRIBUTES)
					resourceFound = true;

				if (checkResource.Exists == EXISTS)
					checkExists = true;
				else if (checkResource.Exists = NOTEXISTS)
					checkExists = false;

				thisMatches = !(resourceFound ^ checkExists);
			}

			//check allocation
			switch (test->GetAllocation())
			{
			case GT:
				if (!positiveAllocation || (allocInfo == 0))
					thisMatches = false;
				break;
			case GTE:
				if (!positiveAllocation && (allocInfo != 0))
					thisMatches = false;
				break;
			case LT:
				if (positiveAllocation || (allocInfo == 0))
					thisMatches = false;
				break;
			case LTE:
				if (positiveAllocation && (allocInfo != 0))
					thisMatches = false;
				break;
			case E:
				if (allocInfo != 0)
					thisMatches = false;
				break;
			case NA:
				break;
			}

			//Create an array of booleans and set them all to false
			const int maxParams = test->GetMatchParameterCount();
			bool * paramMatches = new bool[numParameters];
			for (int setTrue=0; setTrue < numParameters; setTrue++)
				paramMatches[setTrue] = false;

			// loop through the parameters
			for (int j = 0; (thisMatches) && (j < test->GetMatchParameterCount()); j++)
			{
				Parameter thisParam = test->GetParameterToMatchAt(j);

				// if any parameter does not match, the whole test does not match
				switch(thisParam.type)
				{
					case BooleanType:
					{
						bool paramBool = *((bool *)parameters[thisParam.position]);
						bool testBool = *((bool *)thisParam.value);
						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesBooleanParamMatch(paramBool, testBool, thisParam.operatorType);
						break;
					}
					case IntegerType:
					{
						int paramInt = *((int *)parameters[thisParam.position]);
						int testInt = *((int *)thisParam.value);
						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesIntegerParamMatch(paramInt, testInt, thisParam.operatorType);
						break;
					}
					case IIDRefType:
					{
						SiString paramString;
						IID paramIID = *((IID *)parameters[thisParam.position]);

						LPOLESTR paramIIDStr = NULL;
						LPOLESTR testIIDStr = (LPOLESTR) thisParam.value;
						StringFromIID(paramIID, &paramIIDStr);

						if ( resourcePathList && (resourcePathList->GetSize() > thisParam.position) && isResourceType(thisParam.operatorType) )
							paramString = (*resourcePathList)[thisParam.position];
						else
							paramString = paramIIDStr;

						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesStringParamMatch(paramString, testIIDStr, thisParam.operatorType);

						CoTaskMemFree(paramIIDStr);
						break;
					}
					case StringType:
					case OutStringType:
					{
						SiString paramString;
						SiString testString = (char*) thisParam.value;

						if ( resourcePathList && (resourcePathList->GetSize() > thisParam.position) && isResourceType(thisParam.operatorType) )
							paramString = (*resourcePathList)[thisParam.position];
						else if (thisParam.type == StringType)
							paramString = *((SiString *)parameters[thisParam.position]);
						else if (considerOutParams)
						{
							LPSTR paramValueString = *((LPSTR *)parameters[thisParam.position]);
							paramString = paramValueString;
						}

						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesStringParamMatch(paramString, testString, thisParam.operatorType);
						break;
					}
					case WideStringType:
					case OutWideStringType:
					{
						SiString paramString;
						SiString testString = (wchar_t*) thisParam.value;

						if (resourcePathList && (resourcePathList->GetSize() > thisParam.position) && isResourceType(thisParam.operatorType) )
							paramString = (*resourcePathList)[thisParam.position];
						else if (thisParam.type == WideStringType)
							paramString = *((SiString *)parameters[thisParam.position]);
						else if (considerOutParams)
						{
							LPWSTR paramValueString = *((LPWSTR *)parameters[thisParam.position]);
							paramString = paramValueString;
						}

						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesStringParamMatch(paramString, testString, thisParam.operatorType);
						break;
					}
					case RealType:
					{
						float paramFloat = *((float *)parameters[thisParam.position]);
						float testFloat = *((float *)thisParam.value);
						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesRealParamMatch(paramFloat, testFloat, thisParam.operatorType);
						break;
					}
					case PointerType:
					{
						unsigned long paramLong = *((unsigned long *)parameters[thisParam.position]);
						unsigned long testLong = *((unsigned long *)thisParam.value);
						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesPointerParamMatch(paramLong, testLong, thisParam.operatorType);
						break;
					}
					case Integer64Type:
					{
						__int64 paramInt = *((__int64 *)parameters[thisParam.position]);
						__int64 testInt = *((__int64 *)thisParam.value);
						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesInteger64ParamMatch(paramInt, testInt, thisParam.operatorType);
						break;
					}
					case IgnoreType:
					{
						// always match
						break;
					}
					default:
					{
						int paramInt = *((int *)parameters[thisParam.position]);
						int testInt = *((int *)thisParam.value);
						paramMatches[thisParam.position] = paramMatches[thisParam.position] || DoesIntegerParamMatch(paramInt, testInt, thisParam.operatorType);
						break;
					}
				}
			}

			//check to see if all the params matched at least one of the
			//conditions set by the user
			//and set the thisMatches flag accordingly
			for (int checkTrue = 0; checkTrue < test->GetMatchParameterCount(); checkTrue++)
			{
				Parameter checkParam = test->GetParameterToMatchAt(checkTrue);
				thisMatches = thisMatches && paramMatches[checkParam.position];
			}

			if (thisMatches)
			{
				oneMatches = true;

				// set the error code and return value
				matchingErrorCode = test->GetErrorCode();
				matchingReturnValue = test->GetReturnValue();
				*matchingTest = i;
			}
		}

		if (oneMatches)
			return true;

		return false;
	}

	//*************************************************************************
	// Method:		AddNetworkCorruptionFault
	// Description: Adds a new network corruption fault
	//
	// Parameters:
	//	send - true if sent data should be corrupted
	//	recv - true if received data should be corrupted
	//	portMask - bit array (65536 bits) of network ports to corrupt
	//	searchRegExpr - regular expression to search for
	//	replacementStr - replacement string to replace each regular expression
	//		occurence with
	//	testID - unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::AddNetworkCorruptionFault(bool send, bool recv, unsigned char* portMask, const SiString& searchRegExpr, const SiString& replacementStr, const SiString& testID)
	{
		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;
		regPath += "\\";
		regPath += NET_CORRUPT_KEY_NAME;
		regPath += "\\";
		regPath += testID;

		// check to make sure the key exists
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(regPath))
		{
			if (!registry.CreateKey(ROOT_KEY, regPath))
				return false;
		}
		else
		{
			if (!registry.OpenKey(ROOT_KEY, regPath))
				return false;
		}

		if (!registry.Write(NET_CORRUPT_SEND_VALUE, (int)send))
			return false;
		if (!registry.Write(NET_CORRUPT_RECV_VALUE, (int)recv))
			return false;

		if (!registry.Write(NET_CORRUPT_PORT_MASK, portMask, 8192)) // 65536 bits, one for each possible port
			return false;

		// Combine the search expression and the replacement string into one string, as the network corruption
		// backend expects
		SiString modifyExpr = searchRegExpr;
		modifyExpr += replacementStr;
		modifyExpr += "/";

		if (!registry.Write(NET_CORRUPT_MODIFY_EXPR, modifyExpr))
			return false;

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		DeleteNetworkCorruptionFault
	// Description: Removes a network corruption fault
	//
	// Parameters:
	//	testID - unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DeleteNetworkCorruptionFault(const SiString &testID)
	{
		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;
		regPath += "\\";
		regPath += NET_CORRUPT_KEY_NAME;
		regPath += "\\";
		regPath += testID;

		Registry registry(ROOT_KEY);
		if (registry.KeyExists(regPath))
			return registry.DeleteKeyRecursively(ROOT_KEY, regPath);

		return true;
	}

	//*************************************************************************
	// Method:		GetNetworkCorruptionFaults
	// Description: Gets the network corruption faults applied for a port
	//
	// Parameters:
	//	send - true if sending, false if receiving
	//  port - Port number
	//  modList - Receives list of modifications on this port
	//
	// Return Value: true if faults exist for this port
	//*************************************************************************
	bool SettingsManager::GetNetworkCorruptionFaults(bool send, int port, ModificationList &modList)
	{
		modList.Clear();

		EnterCriticalSection(&corruptLock);
		for (unsigned i=0; i<netCorruptFaults.GetLength(); i++)
		{
			if (send)
			{
				if (!netCorruptFaults[i].send)
					continue;
			}
			else
			{
				if (!netCorruptFaults[i].recv)
					continue;
			}

			// Check the bit in the mask.  The mask is a bit array of 65536 bits, one
			// for each port, or 8192 bytes.  "m_wPort >> 3" divides the bit index by
			// 8, selecting which byte contains the bit we want to check.  "m_wPort & 7"
			// selects which bit index within the byte needs to be checked.  The
			// "1 << (m_wPort & 7)" expression yields a value with only the selected
			// bit set.  We then AND this value with the existing value, which
			// results in zero if the bit is not set and nonzero if the bit is set.
			if (!netCorruptFaults[i].portMask[port >> 3] & (1 << (port & 7)))
				continue;

			modList.Add(netCorruptFaults[i].mod);
		}
		LeaveCriticalSection(&corruptLock);

		if (modList.GetLength() > 0)
			return true;
		return false;
	}

	//*************************************************************************
	// Method:		AddFileCorruptionFault
	// Description: Adds a new file corruption fault
	//
	// Parameters:
	//	fileName - original file name
	//  alwaysRegen - regenerate at each access
	//	searchRegExpr - regular expression to search for
	//	replacementStr - replacement string to replace each regular expression
	//		occurence with
	//	testID - unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::AddFileCorruptionFault(const SiString &fileName, bool alwaysRegen, const SiString& searchRegExpr, const SiString& replacementStr, const SiString& testID)
	{
		// Convert path to full long name and lowercase to ensure strcmp can be used for matching
		wchar_t fullPath[MAX_PATH];
		if (!GetLongPathNameW((wchar_t*)fileName, fullPath, MAX_PATH))
			return false;
		wcslwr(fullPath);

		// Combine the search expression and the replacement string into one string, as the network corruption
		// backend expects
		SiString modifyExpr = searchRegExpr;
		modifyExpr += replacementStr;
		modifyExpr += "/";

		EnterCriticalSection(&corruptLock);
		FileModification mod;
		mod.testId = testID;
		mod.regen = alwaysRegen;
		mod.mod.SetExpression(modifyExpr);
		fileCorruptFaults[fileName].Add(mod);
		LeaveCriticalSection(&corruptLock);

		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;
		regPath += "\\";
		regPath += FILE_CORRUPT_KEY_NAME;
		regPath += "\\";
		regPath += testID;

		// check to make sure the key exists
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(regPath))
		{
			if (!registry.CreateKey(ROOT_KEY, regPath))
				return false;
		}
		else
		{
			if (!registry.OpenKey(ROOT_KEY, regPath))
				return false;
		}

		if (!registry.Write(FILE_CORRUPT_ORIG_FILE, fullPath))
			return false;

		if (!registry.Write(FILE_CORRUPT_REGEN_VALUE, (int)alwaysRegen))
			return false;

		if (!registry.Write(FILE_CORRUPT_MODIFY_EXPR, modifyExpr))
			return false;

		registry.Close();

		ReadFileCorruptionFromRegistry();
		return true;
	}

	//*************************************************************************
	// Method:		DeleteFileCorruptionFault
	// Description: Removes a file corruption fault
	//
	// Parameters:
	//	testID - unique id that identifies this test.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DeleteFileCorruptionFault(const SiString &testID)
	{
		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;
		regPath += "\\";
		regPath += FILE_CORRUPT_KEY_NAME;
		regPath += "\\";
		regPath += testID;

		Registry registry(ROOT_KEY);
		if (registry.KeyExists(regPath))
			return registry.DeleteKeyRecursively(ROOT_KEY, regPath);

		ReadFileCorruptionFromRegistry();
		return true;
	}

	//*************************************************************************
	// Method:		GetFileCorruptionFaults
	// Description: Gets the file corruption faults applied to a file
	//
	// Parameters:
	//	fileName - Original file name
	//  modList - Receives list of modifications on this port
	//  alwaysRegen - Receives true if corruption should be regenerated at
	//		each access to the file
	//
	// Return Value: true if faults exist for this file
	//*************************************************************************
	bool SettingsManager::GetFileCorruptionFaults(const SiString &fileName, ModificationList &modList, bool& alwaysRegen)
	{
		// Convert path to full long name and lowercase to ensure strcmp can be used for matching
		wchar_t fullPath[MAX_PATH];
		if (!GetLongPathNameW((wchar_t*)fileName, fullPath, MAX_PATH))
			return false;
		wcslwr(fullPath);

		modList.Clear();
		alwaysRegen = false;

		EnterCriticalSection(&corruptLock);
		for (unsigned i=0; i<fileCorruptFaults[SiString(fullPath)].GetLength(); i++)
		{
			modList.Add(fileCorruptFaults[SiString(fullPath)][i].mod);
			if (fileCorruptFaults[SiString(fullPath)][i].regen)
				alwaysRegen = true;
		}
		LeaveCriticalSection(&corruptLock);

		if (modList.GetLength() > 0)
			return true;
		return false;
	}

	//*************************************************************************
	// Method:		SetLastFileCorruption
	// Description: Sets the last corrupted file for the specified
	//				original file
	//
	// Parameters:
	//	fileName - Original file name
	//  corruptedFile - Corrupted file name
	//
	// Return Value: true if successful
	//*************************************************************************
	bool SettingsManager::SetLastFileCorruption(const SiString &fileName, const SiString& corruptedFile, const SiString& changesXmlFile)
	{
		// Convert path to full long name and lowercase to ensure strcmp can be used for matching
		wchar_t fullPath[MAX_PATH];
		if (!GetLongPathNameW((wchar_t*)fileName, fullPath, MAX_PATH))
			return false;
		wcslwr(fullPath);

		EnterCriticalSection(&corruptLock);
		for (unsigned i=0; i<fileCorruptFaults[SiString(fullPath)].GetLength(); i++)
		{
			SiString regPath = programRootPath;
			regPath += "\\";
			regPath += TEST_SETTINGS_KEY;
			regPath += "\\";
			regPath += FILE_CORRUPT_KEY_NAME;
			regPath += "\\";
			regPath += fileCorruptFaults[SiString(fullPath)][i].testId;

			Registry registry(ROOT_KEY);
			if (registry.OpenKey(ROOT_KEY, regPath))
			{
				registry.Write(FILE_CORRUPT_LAST_FILE, corruptedFile);
				registry.Write(FILE_CORRUPT_LAST_CHANGES, changesXmlFile);
				registry.Close();
			}
		}
		LeaveCriticalSection(&corruptLock);
		ReadFileCorruptionFromRegistry();
		return true;
	}

	//*************************************************************************
	// Method:		ClearLastFileCorruption
	// Description: Clears the last corrupted file for the specified
	//				original file
	//
	// Parameters:
	//	fileName - Original file name
	//
	// Return Value: true if successful
	//*************************************************************************
	bool SettingsManager::ClearLastFileCorruption(const SiString &fileName)
	{
		// Convert path to full long name and lowercase to ensure strcmp can be used for matching
		wchar_t fullPath[MAX_PATH];
		if (!GetLongPathNameW((wchar_t*)fileName, fullPath, MAX_PATH))
			return false;
		wcslwr(fullPath);

		EnterCriticalSection(&corruptLock);
		for (unsigned i=0; i<fileCorruptFaults[SiString(fullPath)].GetLength(); i++)
		{
			SiString regPath = programRootPath;
			regPath += "\\";
			regPath += TEST_SETTINGS_KEY;
			regPath += "\\";
			regPath += FILE_CORRUPT_KEY_NAME;
			regPath += "\\";
			regPath += fileCorruptFaults[SiString(fullPath)][i].testId;

			Registry registry(ROOT_KEY);
			if (registry.OpenKey(ROOT_KEY, regPath))
			{
				registry.DeleteValue(FILE_CORRUPT_LAST_FILE);
				registry.DeleteValue(FILE_CORRUPT_LAST_CHANGES);
				registry.Close();
			}
		}
		LeaveCriticalSection(&corruptLock);
		ReadFileCorruptionFromRegistry();
		return true;
	}

	//*************************************************************************
	// Method:		GetLastFileCorruption
	// Description: Determines the last corrupted file for the specified
	//				original file
	//
	// Parameters:
	//	fileName - Original file name
	//  corruptedFile - Receives corrupted file name
	//
	// Return Value: true if successful, false if there were no corruptions
	//*************************************************************************
	bool SettingsManager::GetLastFileCorruption(const SiString &fileName, SiString& corruptedFile, SiString& changesXmlFile)
	{
		// Convert path to full long name and lowercase to ensure strcmp can be used for matching
		wchar_t fullPath[MAX_PATH];
		if (!GetLongPathNameW((wchar_t*)fileName, fullPath, MAX_PATH))
			return false;
		wcslwr(fullPath);

		EnterCriticalSection(&corruptLock);
		for (unsigned i=0; i<fileCorruptFaults[SiString(fullPath)].GetLength(); i++)
		{
			SiString regPath = programRootPath;
			regPath += "\\";
			regPath += TEST_SETTINGS_KEY;
			regPath += "\\";
			regPath += FILE_CORRUPT_KEY_NAME;
			regPath += "\\";
			regPath += fileCorruptFaults[SiString(fullPath)][i].testId;

			Registry registry(ROOT_KEY);
			if (registry.OpenKey(ROOT_KEY, regPath))
			{
				if (registry.Read(FILE_CORRUPT_LAST_FILE, corruptedFile) &&
					registry.Read(FILE_CORRUPT_LAST_CHANGES, changesXmlFile))
				{
					LeaveCriticalSection(&corruptLock);
					registry.Close();
					return true;
				}
				registry.Close();
			}
		}
		LeaveCriticalSection(&corruptLock);
		corruptedFile = "";
		return false;
	}

	//*************************************************************************
	// Method:		IsCategoryLoggingEnabled
	// Description: Determines whether or not logging is enabled for a given
	//	category of functions
	//
	// Parameters:
	//	category - The category to check for logging enabled
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::IsCategoryLoggingEnabled(const char *category)
	{
		EnterCriticalSection(&disabledLogLock);
		if (!disabledLogTable.KeyExists(category))
		{
			LeaveCriticalSection(&disabledLogLock);
			return true;
		}
		if (disabledLogTable[category])
		{
			LeaveCriticalSection(&disabledLogLock);
			return false;
		}
		LeaveCriticalSection(&disabledLogLock);
		return true;
	}

	//*************************************************************************
	// Method:		IsFunctionLoggingEnabled
	// Description: Determines whether or not logging is enabled for a given
	//	function in a given category
	//
	// Parameters:
	//	category - The category to check for logging enabled
	//	functionName - The function to check for logging enabled
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::IsFunctionLoggingEnabled(const char *category, const char *functionName)
	{
		EnterCriticalSection(&disabledLogLock);
		if (!disabledLogTable.KeyExists(functionName))
		{
			LeaveCriticalSection(&disabledLogLock);
			return true;
		}
		if (disabledLogTable[functionName])
		{
			LeaveCriticalSection(&disabledLogLock);
			return false;
		}
		LeaveCriticalSection(&disabledLogLock);
		return true;
	}

	//*************************************************************************
	// Method:		SetCategoryLoggingEnabled
	// Description: Sets whether or not logging is enabled for a given
	//	category
	//
	// Parameters:
	//	category - The category to set for logging enabled
	//	enabled - true for logging enabled, false to disable
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetCategoryLoggingEnabled(const SiString &category, bool enabled)
	{
		EnterCriticalSection(&disabledLogLock);

		if (enabled)
		{
			if (disabledLogTable.KeyExists(category))
				disabledLogTable.Remove(category);
		}
		else
			disabledLogTable.Add(category, true);

		LeaveCriticalSection(&disabledLogLock);
		return true;
	}

	//*************************************************************************
	// Method:		SetFunctionLoggingEnabled
	// Description: Sets whether or not logging is enabled for a given
	//	function in a given category
	//
	// Parameters:
	//	category - The category to set for logging enabled
	//	functionName - The function to set for logging enabled
	//	enabled - true for logging enabled, false to disable
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetFunctionLoggingEnabled(const SiString &category, const SiString &functionName, bool enabled)
	{
		try
		{
			EnterCriticalSection(&disabledLogLock);

			if (enabled)
			{
				// enable category logging if this is enabled
				if (!this->SetCategoryLoggingEnabled(category, enabled))
				{
					LeaveCriticalSection(&disabledLogLock);
					return false;
				}
				if (disabledLogTable.KeyExists(functionName))
					disabledLogTable.Remove(functionName);
			}
			else
				disabledLogTable[functionName] = true;

			LeaveCriticalSection(&disabledLogLock);
		}
		catch(...)
		{
			return false;
		}

		return true;
	}

	//*************************************************************************
	// Method:		CommitLoggingOptions
	// Description: Writes the current logging options to the registry
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::CommitLoggingOptions()
	{
		EnterCriticalSection(&disabledLogLock);

		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += DISABLE_LOGGING_KEY;

		// make sure the path exists
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(path))
			registry.CreateKey(ROOT_KEY, path);

		SiArray<SiString> disableList = disabledLogTable.GetKeyList();

		// Calculate total size of the disabled list entry
		int totalSize = sizeof(int);
		for (int i=0; i<disableList.GetLength(); i++)
		{
			if (disabledLogTable[disableList[i]])
				totalSize += sizeof(int) + (disableList[i].GetLength() + 1);
		}

		char* disabledBytes = new char[totalSize];
		int curOffset = 0;

		if (disabledBytes == NULL)
		{
			LeaveCriticalSection(&disabledLogLock);
			return false;
		}

		// Write number of disabled functions
		int disableCount = disableList.GetLength();
		CopyMemory(disabledBytes + curOffset, &disableCount, sizeof(int));
		curOffset += sizeof(int);

		// Write function names
		for (int i=0; i<disableList.GetLength(); i++)
		{
			if (disabledLogTable[disableList[i]])
			{
				int strLength = disableList[i].GetLength() + 1;
				CopyMemory(disabledBytes + curOffset, &strLength, sizeof(int));
				curOffset += sizeof(int);
				CopyMemory(disabledBytes + curOffset, (char*)disableList[i], strLength);
				curOffset += strLength;
			}
		}

		bool retVal = registry.Write("", disabledBytes, totalSize);
		delete[] disabledBytes;

		LeaveCriticalSection(&disabledLogLock);
		return true;
	}

	//*************************************************************************
	// Method:		GetCachedMaximumDiskUsage
	// Description: Gets the maximum allowable disk usage for an aut.
	//
	// Parameters:
	//	maxUsage - The maximum allowable disk usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCachedMaximumDiskUsage(DWORD64 &maxUsage)
	{
		if (!maxDiskUsageSet)
			return false;
		maxUsage = maxDiskUsage;
		return true;
	}

	//*************************************************************************
	// Method:		GetMaximumDiskUsage
	// Description: Gets the maximum allowable disk usage for an aut.
	//
	// Parameters:
	//	maxUsage - The maximum allowable disk usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetMaximumDiskUsage(DWORD64 &maxUsage)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, path))
		{
			maxDiskUsageSet = false;
			return false;
		}

		if (!registry.Read(MAX_DISK_USAGE_VALUE_NAME, maxUsage))
		{
			registry.Close();
			maxDiskUsageSet = false;
			return false;
		}

		registry.Close();
		maxDiskUsageSet = true;
		return true;
	}

	//*************************************************************************
	// Method:		GetCurrentDiskUsage
	// Description: Gets the current disk usage for an aut.
	//
	// Parameters:
	//	maxUsage - The current disk usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCurrentDiskUsage(DWORD64 &currentUsage)
	{
		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, path))
			return false;

		if (!registry.Read(CURR_DISK_USAGE_VALUE_NAME, currentUsage))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetCurrentCachedDiskUsage
	// Description: Gets the current disk usage for an aut.  Gets the cached
	//				value, which is updated more often than the registry value.
	//
	// Parameters:
	//	maxUsage - The current disk usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCurrentCachedDiskUsage(DWORD64 &currentUsage)
	{
		currentUsage = currentDiskUsage;
		return true;
	}

	//*************************************************************************
	// Method:		GetCachedMaximumMemoryUsage
	// Description: Gets the maximum allowable memory usage for an aut.
	//
	// Parameters:
	//	maxUsage - The maximum allowable memory usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCachedMaximumMemoryUsage(DWORD64 &maxUsage)
	{
		if (!maxMemUsageSet)
			return false;
		maxUsage = maxMemUsage;
		return true;
	}

	//*************************************************************************
	// Method:		GetMaximumMemoryUsage
	// Description: Gets the maximum allowable memory usage for an aut.
	//
	// Parameters:
	//	maxUsage - The maximum allowable memory usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetMaximumMemoryUsage(DWORD64 &maxUsage)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, path))
		{
			maxMemUsageSet = false;
			return false;
		}

		if (!registry.Read(MAX_MEM_USAGE_VALUE_NAME, maxUsage))
		{
			registry.Close();
			maxMemUsageSet = false;
			return false;
		}

		registry.Close();
		maxMemUsageSet = true;
		return true;
	}

	//*************************************************************************
	// Method:		GetCurrentMemoryUsage
	// Description: Gets the current memory usage for an aut.
	//
	// Parameters:
	//	maxUsage - The current memory usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCurrentMemoryUsage(DWORD64 &currentUsage)
	{
		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, path))
			return false;

		if (!registry.Read(CURR_MEM_USAGE_VALUE_NAME, currentUsage))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetCurrentCachedMemoryUsage
	// Description: Gets the current memory usage for an aut.  Gets the
	//				cached value, which is updated more often than the
	//				one in the registry.
	//
	// Parameters:
	//	maxUsage - The current memory usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCurrentCachedMemoryUsage(DWORD64 &currentUsage)
	{
		currentUsage = currentMemUsage;
		return true;
	}

	//*************************************************************************
	// Method:		SetMaximumDiskUsage
	// Description: Sets the maximum allowable disk usage for an aut.
	//
	// Parameters:
	//	maxUsage - The maximum allowable disk usage for an aut.
	//	isEnabled - If true, maxUsage is ignored and the registry value is deleted
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetMaximumDiskUsage(DWORD64 maxUsage, bool isEnabled = true)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey(ROOT_KEY, path))
			return false;

		if (!isEnabled)
		{
			registry.DeleteValue(MAX_DISK_USAGE_VALUE_NAME);
			registry.Close();
			return true;
		}

		if (!registry.Write(MAX_DISK_USAGE_VALUE_NAME, maxUsage))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetCurrentDiskUsage
	// Description: Sets the current disk usage for an aut.
	//
	// Parameters:
	//	maxUsage - The current disk usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetCurrentDiskUsage(DWORD64 currentUsage)
	{
		currentDiskUsage = currentUsage;
		// Will be written to registry in the registry monitor thread every second
		return true;
	}

	//*************************************************************************
	// Method:		SetCurrentDiskUsageInRegistry
	// Description: Sets the current disk usage for an aut.
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetCurrentDiskUsageInRegistry()
	{
		if (currentDiskUsage == 0)
		{
			// Don't write to the registry if this replacement library isn't using this value yet
			return false;
		}

		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey(ROOT_KEY, path))
			return false;

		if (!registry.Write(CURR_DISK_USAGE_VALUE_NAME, currentDiskUsage))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetMaximumMemoryUsage
	// Description: Sets the maximum allowable memory usage for an aut.
	//
	// Parameters:
	//	maxUsage - The maximum allowable memory usage for an aut.
	//	isEnabled - If true, maxUsage is ignored and the registry value is deleted
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetMaximumMemoryUsage(DWORD64 maxUsage, bool isEnabled)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey(ROOT_KEY, path))
			return false;

		if (!isEnabled)
		{
			registry.DeleteValue(MAX_MEM_USAGE_VALUE_NAME);
			registry.Close();
			return true;
		}

		if (!registry.Write(MAX_MEM_USAGE_VALUE_NAME, maxUsage))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetCurrentMemoryUsage
	// Description: Sets the current memory usage for an aut.
	//
	// Parameters:
	//	maxUsage - The current memory usage for an aut.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetCurrentMemoryUsage(DWORD64 currentUsage)
	{
		currentMemUsage = currentUsage;
		// Will be written to registry in the registry monitor thread every second
		return true;
	}

	//*************************************************************************
	// Method:		SetCurrentMemoryUsageInRegistry
	// Description: Sets the current memory usage for an aut.
	//
	// Parameters:
	//	NOne
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetCurrentMemoryUsageInRegistry()
	{
		if (currentMemUsage == 0)
		{
			// Don't write to the registry if this replacement library isn't using this value yet
			return false;
		}

		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey(ROOT_KEY, path))
			return false;

		if (!registry.Write(CURR_MEM_USAGE_VALUE_NAME, currentMemUsage))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetProcessChainingEnabled
	// Description: Sets whether or not to use process chaining in the aut.
	//
	// Parameters:
	//	enabled - true to enable process chaining in the aut, false to disable.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetProcessChainingEnabled(bool enabled)
	{
		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, programRootPath))
			return false;

		bool retVal = registry.Write(PROCESS_CHAINING_VALUE_NAME, (int)enabled);

		registry.Close();
		return retVal;
	}

	//*************************************************************************
	// Method:		GetProcessChainingEnabled
	// Description: Gets whether or not to use process chaining in the aut.
	//
	// Parameters:
	//	enabled - true if process chaining is enabled in the aut, false to disable.
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetProcessChainingEnabled(bool &enabled)
	{
		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, programRootPath))
			return false;

		bool retVal = registry.Read(PROCESS_CHAINING_VALUE_NAME, enabled);

		registry.Close();
		return retVal;
	}

	//*************************************************************************
	// Method:		GetLogPipeName
	// Description: Gets the log pipe name for this process
	//
	// Parameters:
	//	pipeName - on exiting the function holds the name of the pipe
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetLogPipeName(SiString &pipeName)
	{
		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, programRootPath))
			return false;

		if (!registry.Read(LOG_PIPE_VALUE_NAME, pipeName))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		DeleteFunctionTest
	// Description: Deletes a scheduled test for a function
	//
	// Parameters:
	//	targetThreadID - the thread for which to delete the test
	//	testID - a unique id for the test to be deleted
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::DeleteFunctionTest(const SiString &targetThreadID, const SiString &testID)
	{
		if (functionTestKeys.KeyExists(testID))
		{
			Registry registry(ROOT_KEY);
			if (!registry.DeleteKeyRecursively(ROOT_KEY, functionTestKeys[testID]))
				return false;
			functionTestKeys.Remove(testID);
			return true;
		}
		return true;
	}

	//*************************************************************************
	// Method:		GetTestExecutionCount
	// Description: Gets the test execution count from the registry
	//
	// Parameters:
	//	testIDPath - the registry path to the test
	//
	// Return Value: the current execution count, 0 if reg key does not exist
	//*************************************************************************
	int SettingsManager::GetTestExecutionCount(const SiString &testIDPath)
	{
		// check the test key for existence
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(testIDPath))
			return -1;

		int exeCount = 0;
		//Read the value
		if (registry.Read(TEST_EXECUTE_COUNT_VALUE_NAME, exeCount))
			return exeCount;
		else
			return 0;
	}

	//*************************************************************************
	// Method:		GetTestExecutionCount
	// Description: Gets the test execution count from the registry
	//
	// Parameters:
	//	testIDPath - the registry path to the test
	//
	// Return Value: the current execution count, 0 if reg key does not exist
	//*************************************************************************
	int SettingsManager::GetTestExecutionCount(const SiString &targetThreadID, const SiString &category, const SiString &functionName, const SiString &testID)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += targetThreadID;
		path += "\\";
		path += FAULTS_KEY_NAME;
		path += "\\";
		path += category;
		path += "\\";
		path += functionName;
		path += "\\";
		path += testID;

		// check the test key for existence
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(path))
			return 0;

		int exeCount = 0;
		//Read the value
		if (registry.Read(TEST_EXECUTE_COUNT_VALUE_NAME, exeCount))
			return exeCount;
		else
			return 0;
	}

	//*************************************************************************
	// Method:		IncrementTestExecutionCount
	// Description: Increments the test execution count in the registry
	//
	// Parameters:
	//	testIDPath - the registry path to the test
	//
	// Return Value: the current execution count, -1 if error
	//*************************************************************************
	int SettingsManager::IncrementTestExecutionCount(const SiString &testIDPath)
	{
		// check the test key for existence
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(testIDPath))
			return -1;

		//Get the old callcount
		int exeCount = this->GetTestExecutionCount (testIDPath);
		
		if (exeCount != -1)
		{
			//Increment the callcount
			exeCount++;
			//Write the new value
			if (registry.Write(TEST_EXECUTE_COUNT_VALUE_NAME, exeCount))
				return exeCount;
			else
				return -1;
		}

		return -1;
	}

	//*************************************************************************
	// Method:		GetCachedNetworkUploadBandwidth
	// Description: Gets the network upload limit
	//
	// Parameters:
	//	uploadBandwidth - [out] Receives the network upload bandwidth in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCachedNetworkUploadBandwidth(DWORD64 &uploadBandwidth)
	{
		if (!netUploadBandwidthSet)
			return false;
		uploadBandwidth = maxUploadBandwidth;
		return true;
	}

	//*************************************************************************
	// Method:		GetCachedNetworkDownloadBandwidth
	// Description: Gets the network download limit
	//
	// Parameters:
	//	uploadBandwidth - [out] Receives the network upload bandwidth in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCachedNetworkDownloadBandwidth(DWORD64 &downloadBandwidth)
	{
		if (!netDownloadBandwidthSet)
			return false;
		downloadBandwidth = maxDownloadBandwidth;
		return true;
	}

	//*************************************************************************
	// Method:		ReadNetworkCorruptionFromRegistry
	// Description: Reads the network corruption registry settings into the
	//				internal data structures
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SettingsManager::ReadNetworkCorruptionFromRegistry()
	{
		EnterCriticalSection(&corruptLock);

		netCorruptFaults.Clear();

		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;
		regPath += "\\";
		regPath += NET_CORRUPT_KEY_NAME;

		// Open the registry key containing the settings
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(regPath))
		{
			LeaveCriticalSection(&corruptLock);
			return;
		}
		registry.OpenKey(ROOT_KEY, regPath);

		// Read in the settings
		int numKeys = registry.GetSubKeyCount(ROOT_KEY, regPath);

		for (int i=0; i<numKeys; i++)
		{
			SiString testID;
			if (!registry.GetSubKeyNameAt(ROOT_KEY, regPath, i, testID))
				continue;

			// get the test path
			SiString testIdPath = regPath;
			testIdPath += "\\";
			testIdPath += testID;

			if (!registry.OpenKey(ROOT_KEY, testIdPath))
				continue;

			NetworkModification mod;
			unsigned long size;

			if (!registry.Read(NET_CORRUPT_SEND_VALUE, mod.send))
				continue;
			if (!registry.Read(NET_CORRUPT_RECV_VALUE, mod.recv))
				continue;

			size = (unsigned long)sizeof(mod.portMask);
			if (!registry.Read(NET_CORRUPT_PORT_MASK, mod.portMask, size))
				continue;

			SiString modifyExpr;
			if (!registry.Read(NET_CORRUPT_MODIFY_EXPR, modifyExpr))
				continue;
			mod.mod.SetExpression(modifyExpr);

			registry.Close();

			if (!mod.mod.IsValid())
				continue;

			netCorruptFaults.Add(mod);
		}

		registry.Close();
		LeaveCriticalSection(&corruptLock);
	}

	//*************************************************************************
	// Method:		ReadFileCorruptionFromRegistry
	// Description: Reads the file corruption registry settings into the
	//				internal data structures
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SettingsManager::ReadFileCorruptionFromRegistry()
	{
		EnterCriticalSection(&corruptLock);

		fileCorruptFaults.Clear();

		SiString regPath = programRootPath;
		regPath += "\\";
		regPath += TEST_SETTINGS_KEY;
		regPath += "\\";
		regPath += FILE_CORRUPT_KEY_NAME;

		// Open the registry key containing the settings
		Registry registry(ROOT_KEY);
		if (!registry.KeyExists(regPath))
		{
			LeaveCriticalSection(&corruptLock);
			return;
		}
		registry.OpenKey(ROOT_KEY, regPath);

		// Read in the settings
		int numKeys = registry.GetSubKeyCount(ROOT_KEY, regPath);

		for (int i=0; i<numKeys; i++)
		{
			SiString testID;
			if (!registry.GetSubKeyNameAt(ROOT_KEY, regPath, i, testID))
				continue;

			// get the test path
			SiString testIdPath = regPath;
			testIdPath += "\\";
			testIdPath += testID;

			if (!registry.OpenKey(ROOT_KEY, testIdPath))
				continue;

			FileModification mod;
			mod.testId = testID;

			SiString origFile;
			if (!registry.Read(FILE_CORRUPT_ORIG_FILE, origFile))
				continue;

			if (!registry.Read(FILE_CORRUPT_REGEN_VALUE, mod.regen))
				continue;

			SiString modifyExpr;
			if (!registry.Read(NET_CORRUPT_MODIFY_EXPR, modifyExpr))
				continue;
			mod.mod.SetExpression(modifyExpr);

			registry.Close();

			if (!mod.mod.IsValid())
				continue;

			if (fileCorruptFaults.KeyExists(origFile))
				fileCorruptFaults[origFile].Add(mod);
			else
			{
				SiArray<FileModification> list;
				list.Add(mod);
				fileCorruptFaults.Add(origFile, list);
			}
		}

		registry.Close();
		LeaveCriticalSection(&corruptLock);
	}

	//*************************************************************************
	// Method:		ReadFromRegistry
	// Description: Reads the registry settings into the internal hash tables
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
     void SettingsManager::ReadFromRegistry()
      {
		GetMaximumDiskUsage(maxDiskUsage);
		GetMaximumMemoryUsage(maxMemUsage);
		GetNetworkUploadBandwidth (maxUploadBandwidth);
		GetNetworkDownloadBandwidth (maxDownloadBandwidth);

		ReadNetworkCorruptionFromRegistry();
		ReadFileCorruptionFromRegistry();

		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		Registry registry(ROOT_KEY);
		registry.OpenKey(ROOT_KEY, path);

		EnterCriticalSection(&corruptLock);

		registry.Read(PROJECT_PATH_VALUE_NAME, projectPath);
		registry.Close();

		int index = projectPath.FindLast('\\');
		SiString projectName;
		if (index > 0)
		{
				projectPath.SubString(projectFolder, 0, index);
				projectPath.SubString(projectName, index + 1);
		}
		else
		{
				projectFolder = "";
				projectName = projectFolder;
		}

		index = projectName.FindLastIgnoreCase(".hdp");
		if (index > 0)
				projectName.SubString(projectTitle, 0, index - 1);
		else
				projectTitle = projectName;

		LeaveCriticalSection(&corruptLock);

		SiHashTable<bool> tempDisabledLogTable;

		// Read the disabled functions
		path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += DISABLE_LOGGING_KEY;

		if (registry.OpenKey(ROOT_KEY, path))
		{
				// get the size of the disabled function list data
				int size, valueType;
				if (registry.GetValueSizeAndType("", size, valueType))
				{
					BYTE *disabledBytes = (BYTE*)HeapAlloc(heap, HEAP_ZERO_MEMORY, size);
					DWORD bytesRead = size;
					registry.Read("", disabledBytes, bytesRead);

					if ((bytesRead == size) && (bytesRead != 0))
					{
							int curOffset = 0;

							int disabledCount;
							CopyMemory(&disabledCount, disabledBytes + curOffset, sizeof(int));
							curOffset += sizeof(int);
							for (int i=0; i<disabledCount; i++)
							{
								int strLen;
								CopyMemory(&strLen, disabledBytes + curOffset, sizeof(int));
								curOffset += sizeof(int);
								tempDisabledLogTable[SiString((char*)(disabledBytes + curOffset))] = true;
								curOffset += strLen;
							}
					}

					HeapFree(heap, 0, disabledBytes);
				}
		}

		EnterCriticalSection(&disabledLogLock);
		disabledLogTable.Clear();
		disabledLogTable = tempDisabledLogTable;
		LeaveCriticalSection(&disabledLogLock);

		SiHashTable<FunctionTests> tempFunctionTestTable;

		// Read the direct function faults and function tests
		path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;

		SiArray<SiString> threadSubkeys = registry.GetSubKeys(ROOT_KEY, path);
		for (unsigned int i=0; i<threadSubkeys.GetLength(); i++)
		{
				SiString targetThreadIDStr = threadSubkeys[i];
				SiString threadPath = path;
				threadPath += "\\";
				threadPath += targetThreadIDStr;
				threadPath += "\\";
				threadPath += FAULTS_KEY_NAME;

				int targetThreadID;
				if (targetThreadIDStr == GLOBAL_THREAD_NAME)
					targetThreadID = 0;
				else
					targetThreadID = atoi((char*)targetThreadIDStr);

				SiArray<SiString> categorySubkeys = registry.GetSubKeys(ROOT_KEY, threadPath);
				for (unsigned int j=0; j<categorySubkeys.GetLength(); j++)
				{
					SiString category = categorySubkeys[j];
					SiString categoryPath = threadPath;
					categoryPath += "\\";
					categoryPath += category;

					SiArray<SiString> funcSubkeys = registry.GetSubKeys(ROOT_KEY, categoryPath);
					for (unsigned int k=0; k<funcSubkeys.GetLength(); k++)
					{
							SiString func = funcSubkeys[k];
							SiString funcPath = categoryPath;
							funcPath += "\\";
							funcPath += func;

							if (registry.KeyExists(funcPath))
							{
								SingleFunctionTestList testList;
								testList.threadId = targetThreadID;
								ReadFunctionTestsFromRegistry(targetThreadIDStr, category, func, testList.tests);
								tempFunctionTestTable[func].Add(testList);
							}
					} // for each function
				} // for each category
		} // for each thread id

		EnterCriticalSection(&functionTestLock);
		functionTestTable.Clear();
		functionTestTable = tempFunctionTestTable;
		LeaveCriticalSection(&functionTestLock);
      }


	//*************************************************************************
	// Method:		WaitForRegistryChangeThread
	// Description: Thread that will wait for changes in the registry
	//				settings and update the internal hash tables on a change.
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	unsigned long WINAPI SettingsManager::WaitForRegistryChangeThread(LPVOID settingsManagerPtr)
	{
		ReplacementLibrary::DisableInterception();

		HKEY hKey;
		SettingsManager* settingsManager = (SettingsManager*)settingsManagerPtr;

		SiString path = settingsManager->programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;

		if (RegCreateKey(ROOT_KEY, (char*)path, &hKey) != ERROR_SUCCESS)
			return 0;

		HANDLE hUpdateEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (!hUpdateEvent)
			return 0;

		RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET, hUpdateEvent, TRUE);

		while (settingsManager->registryMonitorRunning)
		{
			DWORD result = WaitForSingleObject(hUpdateEvent, REGISTRY_CHANGE_WAIT_TIME);

			settingsManager->SetCurrentMemoryUsageInRegistry();
			settingsManager->SetCurrentDiskUsageInRegistry();
			settingsManager->SetUploadBandwidthUsageInRegistry();
			settingsManager->SetDownloadBandwidthUsageInRegistry();

			if (settingsManager->registryMonitorRunning)
			{
				if (result == WAIT_TIMEOUT)
					continue;
				else
				{
					ResetEvent(hUpdateEvent);
					RegNotifyChangeKeyValue(hKey, TRUE, REG_NOTIFY_CHANGE_NAME | REG_NOTIFY_CHANGE_LAST_SET, hUpdateEvent, TRUE);

					settingsManager->ReadFromRegistry();
				}
			}
		}

		CloseHandle(hUpdateEvent);
		RegCloseKey(hKey);

		TerminateThread(GetCurrentThread(), 0);
		return 0;
	}

	//*************************************************************************
	// Method:		SetNetworkUploadBandwidth 
	// Description: Sets the upload bandwidth for the network.
	//
	// Parameters:
	//	maxUploadSpeed - The maximum speed to set for upload in bytes/sec
	//	IsUploadControlEnabled - Boolean to specify whether to control upload traffic
	//
	// Return Value: true if successful, false if update fails
	//*************************************************************************	
	bool SettingsManager::SetNetworkUploadBandwidth (DWORD64 maxUploadSpeed, bool IsUploadControlEnabled)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey (ROOT_KEY, path))
			return false;

		if (IsUploadControlEnabled)
		{
			if (!registry.Write(NETWORK_ULOADBW_VALUE_NAME, maxUploadSpeed))
			{
				registry.Close();
				return false;
			}
		}
		else
		{
			registry.DeleteValue (NETWORK_ULOADBW_VALUE_NAME);
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetNetworkDownloadBandwidth 
	// Description: Sets the download bandwidth for the network
	//
	// Parameters:
	//	maxDownloadSpeed - The maximum speed to set for download in bytes/sec
	//	IsDownloadControlEnabled - Boolean to specify whether to control download traffic
	//
	// Return Value: true if successful, false if update fails
	//*************************************************************************	
	bool SettingsManager::SetNetworkDownloadBandwidth (DWORD64 maxDownloadSpeed, bool IsDownloadControlEnabled)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey (ROOT_KEY, path))
			return false;

		if (IsDownloadControlEnabled)
		{
			if (!registry.Write(NETWORK_DLOADBW_VALUE_NAME, maxDownloadSpeed))
			{
				registry.Close();
				return false;
			}
		}
		else
		{
			registry.DeleteValue (NETWORK_DLOADBW_VALUE_NAME);
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetNetworkUploadBandwidth 
	// Description: Gets the user specified upload network bandwidth
	//
	// Parameters:
	//	uploadBW - the bandwidth for upload in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetNetworkUploadBandwidth (DWORD64 &uploadBW)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey (ROOT_KEY, path))
		{
			netUploadBandwidthSet = false;
			return false;
		}

		if (!registry.Read(NETWORK_ULOADBW_VALUE_NAME, uploadBW))
		{
			registry.Close();
			netUploadBandwidthSet = false;
			return false;
		}
		else
			netUploadBandwidthSet = true;

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetNetworkDownloadBandwidth 
	// Description: Gets the user specified download network bandwidth
	//
	// Parameters:
	//	downloadBW - the bandwidth for download in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetNetworkDownloadBandwidth  (DWORD64 &downloadBW)
	{
		SiString path = programRootPath;
		path += "\\";
		path += TEST_SETTINGS_KEY;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey (ROOT_KEY, path))
		{
			netDownloadBandwidthSet = false;
			return false;
		}

		if (!registry.Read(NETWORK_DLOADBW_VALUE_NAME, downloadBW))
		{
			registry.Close();
			netDownloadBandwidthSet = false;
			return false;
		}
		else
			netDownloadBandwidthSet = true;

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetUploadBandwidthUsage
	// Description: Sets the current upload bandwidth usage
	//
	// Parameters:
	//	currentUploadRate - the current network upload rate in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetUploadBandwidthUsage(DWORD64 currentUploadRate, DWORD lastSendUpdateStamp)
	{
		currentUploadUsage = currentUploadRate;
		sendUpdateStamp = lastSendUpdateStamp;
		// Will be written to registry in the registry monitor thread every second
		return true;
	}

	//*************************************************************************
	// Method:		SetUploadBandwidthUsageInRegistry
	// Description: Sets the current upload bandwidth usage in the registry
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetUploadBandwidthUsageInRegistry()
	{
		// If this thread does not update the network monitoring info. then it shouldn't
		// write to the registry
		if (sendUpdateStamp == 0)
			return false;

		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey(ROOT_KEY, path))
			return false;

		if (!registry.Write(TOTAL_BYTES_SENT_VALUE_NAME, currentUploadUsage))
		{
			registry.Close();
			return false;
		}

		if (!registry.Write(NETWORK_SENDSTMP_VALUE_NAME, sendUpdateStamp))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SetDownloadBandwidthUsage
	// Description: Sets the current download bandwidth usage
	//
	// Parameters:
	//	currentDownloadRate - the current network download rate in bytes/sec
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetDownloadBandwidthUsage(DWORD64 currentDownloadRate, DWORD lastRecvUpdateStamp)
	{
		currentDownloadUsage = currentDownloadRate;
		recvUpdateStamp = lastRecvUpdateStamp;
		// Will be written to registry in the registry monitor thread every second
		return true;
	}

	//*************************************************************************
	// Method:		SetDownloadBandwidthUsageInRegistry
	// Description: Sets the current download bandwidth usage in the registry
	//
	// Parameters:
	//	None
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetDownloadBandwidthUsageInRegistry()
	{
		// If this thread does not update the network monitoring info. then it shouldn't
		// write to the registry
		if (recvUpdateStamp == 0)
			return false;

		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.CreateKey(ROOT_KEY, path))
			return false;

		if (!registry.Write(TOTAL_BYTES_RECV_VALUE_NAME, currentDownloadUsage))
		{
			registry.Close();
			return false;
		}
		
		if (!registry.Write(NETWORK_RECVSTMP_VALUE_NAME, recvUpdateStamp))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetUploadBandwidthUsage
	// Description: Gets the current upload bandwidth usage from the registry
	//
	// Parameters:
	//	bytesTransferred - The bytes uploaded by the application
	//	lastUpdateStamp - the last time when this counter was incremented
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetUploadBandwidthUsage(DWORD64 &bytesTransferred, DWORD &lastUpdateStamp)
	{
		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, path))
			return false;

		if (!registry.Read(TOTAL_BYTES_SENT_VALUE_NAME, bytesTransferred))
		{
			registry.Close();
			return false;
		}

		if (!registry.Read(NETWORK_SENDSTMP_VALUE_NAME, lastUpdateStamp))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetCachedUploadBandwidthUsage
	// Description: Gets the cached upload bandwidth usage
	//
	// Parameters:
	//	bytesTransferred - The bytes uploaded by the application
	//	lastUpdateStamp - the last time when this counter was incremented
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCachedUploadBandwidthUsage(DWORD64 &bytesTransferred, DWORD &lastUpdateStamp)
	{
		bytesTransferred = currentUploadUsage;
		lastUpdateStamp = sendUpdateStamp;
		return true;
	}

	//*************************************************************************
	// Method:		GetDownloadBandwidthUsage
	// Description: Gets the current download bandwidth usage from the registry
	//
	// Parameters:
	//	bytesTransferred - The bytes downloaded by the application
	//	lastUpdateStamp - The last time this value was updated
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetDownloadBandwidthUsage(DWORD64 &bytesTransferred, DWORD &lastUpdateStamp)
	{
		SiString path = programRootPath;
		path += "\\";
		path += USAGE_REG_KEY_NAME;

		Registry registry;
		if (!registry.OpenKey(ROOT_KEY, path))
			return false;

		if (!registry.Read(TOTAL_BYTES_RECV_VALUE_NAME, bytesTransferred))
		{
			registry.Close();
			return false;
		}

		if (!registry.Read(NETWORK_RECVSTMP_VALUE_NAME, lastUpdateStamp))
		{
			registry.Close();
			return false;
		}

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		GetCachedDownloadBandwidthUsage
	// Description: Gets the cached download bandwidth usage
	//
	// Parameters:
	//	bytesTransferred - The bytes downloaded by the application
	//	lastUpdateStamp - The last time this value was updated
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::GetCachedDownloadBandwidthUsage(DWORD64 &bytesTransferred, DWORD &lastUpdateStamp)
	{
		bytesTransferred = currentDownloadUsage;
		lastUpdateStamp = recvUpdateStamp;
		return true;
	}

	//*************************************************************************
	// Method:		SetExternalDotNetLibsForInit
	// Description: Writes the values from the list into the registry
	//				under PID\DotNetLibsInitList
	//
	// Parameters:
	//	bytesTransferred - The bytes downloaded by the application
	//	lastUpdateStamp - The last time this value was updated
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool SettingsManager::SetExternalDotNetLibsForInit(SiArray<SiString> * externalLibList)
	{
		// get the test root key
		SiString path = programRootPath;
		path += "\\";
		path += DOTNET_LIBS_INIT_LIST_KEY;

		Registry registry(ROOT_KEY);
		if (!registry.CreateKey (ROOT_KEY, path))
			return false;

		for (int i=0; i < externalLibList->GetLength(); i++)
			registry.Write ((*externalLibList)[i], 0);

		registry.Close();
		return true;
	}

	//*************************************************************************
	// Method:		SuspendProcess
	// Description: Suspends all the threads in the current process
	//
	// Parameters:
	//	bytesTransferred - The bytes downloaded by the application
	//	lastUpdateStamp - The last time this value was updated
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	void SettingsManager::SuspendProcess()
	{
		DWORD dwProcPriority = GetPriorityClass(GetCurrentProcess());
		DWORD dwThreadPriority = GetThreadPriority(GetCurrentThread());
		DWORD dwProcessId = GetCurrentProcessId();

		SetPriorityClass(GetCurrentProcess, REALTIME_PRIORITY_CLASS);
		SetThreadPriority(GetCurrentThread, THREAD_PRIORITY_TIME_CRITICAL);

		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, NULL);
		if (!hSnap)
			return;

		THREADENTRY32 first = {0};
		first.dwSize = sizeof(first);

		if (!Thread32First(hSnap, &first))
		{
			CloseHandle(hSnap);
			return;
		}

		do
		{
			if (first.th32OwnerProcessID == dwProcessId)
			{
				//Suspend all but the current thread, which will be paused later
				if (first.th32ThreadID != GetCurrentThreadId())
				{
					HANDLE hThread =
					OpenThread(THREAD_SUSPEND_RESUME, false, first.th32ThreadID);

					if (!hThread)
					{
						CloseHandle(hSnap);
						return;
					}

					SuspendThread(hThread);
					CloseHandle(hThread);
				}

			}
			ZeroMemory(&first, sizeof(first));
			first.dwSize = sizeof(first);
		} while (Thread32Next(hSnap, &first));

		CloseHandle(hSnap);

		//Suspend the current thread
		SuspendThread (GetCurrentThread());

		SetPriorityClass(GetCurrentProcess(), dwProcPriority);
		SetThreadPriority(GetCurrentThread(), dwThreadPriority);

		return;
	}


	//*************************************************************************
	// Method:		CheckFiringSpecification
	// Description: Checks, based on firing details, whether the test should be fired
	//				or not
	//
	// Parameters: 
	//	callStackList - the list with the callstack
	//	functionTestList - the function test list
	//	matchingTestIndex - the test which matched
	//
	// Return Value: (INT)
	//			-1 = Run Original Function, Do not run test, do not pause app
	//			 0 = Dont run original function, run test, do not pause app
	//			 1 = Dont run original function, run test and pause app
	//*************************************************************************
	int SettingsManager::CheckFiringSpecification (SiArray<SiString>* callStackList, FunctionTestList * functionTestList, int matchingTestIndex)
	{
		if (functionTestList == NULL)
			return -1;

		//get the function test from the list
		FunctionTest * functionTest = functionTestList->GetTestAt (matchingTestIndex);
		if (functionTest == NULL) return -1;

		return this->CheckFiringSpecification (callStackList, functionTest);
	}

	//*************************************************************************
	// Method:		CheckFiringSpecification
	// Description: Checks, based on firing details, whether the test should be fired
	//				or not
	//
	// Parameters: 
	//	callStackList - the list with the callstack
	//	funtionTest - the test to check
	//
	// Return Value: (INT)
	//			-1 = Run Original Function, Do not run test, do not pause app
	//			 0 = Dont run original function, run test, do not pause app
	//			 1 = Dont run original function, run test and pause app
	//*************************************************************************
	int SettingsManager::CheckFiringSpecification (SiArray<SiString>* callStackList, FunctionTest *functionTest)
	{
		int action = 0;

		//Increment function call count
		int functionCallCount = this->IncrementFunctionCallCount (functionTest->GetRegistryPath());

		//In case there was an error reading the registry key; then do not run the test at all
		if (functionCallCount == -1)
			return -1;

		//Get firing details
		SiArray<SiString> firingDetails = functionTest->GetFiringDetails();
		double firePercentage = 0;

		//get firing option
		int option = functionTest->GetFiringOption();

		//Perform action according to selected firing option
		if (option == 1)
		{
			firePercentage = (double) atoi ((const char*) firingDetails[0]);
			if (firePercentage < 1.00)
				action = -1;
			else
			{
				int randomNum = rand();
				if ( ((double) randomNum/RAND_MAX) > ((double)(firePercentage/100.0)) )
					action = -1;
			}
		}
		else if (callStackList != NULL && callStackList->GetLength() > 1)
		{
			//Remove the first item from the stackarray
			//which is the current intercepted function
			callStackList->Remove (0);

			//Check if the stack contains functions otherwise there 
			//is no use matching the firingDetails to the stackarray
			if (callStackList->GetLength() > 0)
			{
				SiString callStackMember;
				SiString callStackMemberWithoutModuleName;
				SiString directCaller;
				SiArray <SiString> tempArray;
				int tmpIndex = 0;

				switch (option)
				{
				case 2: //Fire when called directly by any of the specified functions
					//Set to run original function by default
					action = -1;
					directCaller = (SiString) (*callStackList)[0];
					tmpIndex = directCaller.FindFirst ('.');
					if (tmpIndex != -1 && directCaller.GetLength() > tmpIndex)
						directCaller.SubString (callStackMemberWithoutModuleName, tmpIndex + 1);

					for (unsigned int getsf=0; getsf < firingDetails.GetLength(); getsf++)
					{
						SiString specifiedFunction = firingDetails[getsf];
						if (specifiedFunction.CompareTo (directCaller) == 0 || specifiedFunction.CompareTo (callStackMemberWithoutModuleName) == 0)
						{
							action = 0;
							break;
						}
					}
					break;
				case 3: //Fire when callstack matches defined pattern
					
					//we must compare these arrays in reverse order
					//because the user specifies them in the reverse order
					tempArray.Clear();
					for (int xfer=firingDetails.GetLength() - 1; xfer >= 0; xfer--)
						tempArray.Add (firingDetails[xfer]);

					if (tempArray.GetLength() > callStackList->GetLength())
					{
						action  = -1;
						break;
					}
					
					action = 0;
					for (unsigned int c=0; c < tempArray.GetLength(); c++)
					{
						callStackMember = (*callStackList)[c];
						tmpIndex = callStackMember.FindFirst ('.');
						if (tmpIndex != -1 && callStackMember.GetLength() > tmpIndex)
							callStackMember.SubString (callStackMemberWithoutModuleName, tmpIndex + 1);

						if (tempArray[c].CompareTo (callStackMember) != 0 && tempArray[c].CompareTo (callStackMemberWithoutModuleName) != 0)
						{
							action = -1;
							break;
						}
					}
					break;
				case 4: //Fire when any one from defined list is in the callstack
					action = -1;

					for (unsigned int fd=0; fd < firingDetails.GetLength() && action == -1; fd++)
					{
						for (unsigned int cs=0; cs < callStackList->GetLength(); cs++)
						{
							callStackMember = (*callStackList)[cs];
							tmpIndex = callStackMember.FindFirst ('.');
							if ( (tmpIndex != -1) && (callStackMember.GetLength() > tmpIndex) )
								callStackMember.SubString (callStackMemberWithoutModuleName, tmpIndex + 1);

							if ( (callStackMember.CompareTo (firingDetails[fd]) == 0) || (callStackMemberWithoutModuleName.CompareTo (firingDetails[fd]) == 0) )
							{
								action = 0;
								break;
							}
						}
					}
					break;

				} //END OF switch (option)

			} //END OF if (callStackList->GetLength() > 0)
			else
				action = -1;

		} // if (option == 1) ELSE branch

		//If test should be run, then check and return pause status
		if (action == 0)
		{
			//If pause is enabled; then return 1; else 0
			if (functionTest->GetPauseAppOnTestFire ())
				return 1;
			else
				return 0;
		}
		else
			return action;
	}

	//*************************************************************************
	// Method:		isResourceType
	// Description: Checks if the operator type is a resource compare
	//
	// Parameters: 
	//	operatorType - operator type to check
	//
	// Return Value: true if a resource type, false otherwise
	//*************************************************************************
	bool SettingsManager::isResourceType (ParameterOperatorType operatorType)
	{
		switch (operatorType)
		{
		case EqualsResourceType:
		case NotEqualToResourceType:
		case StartsWithResourceType:
		case ContainsResourceType:
		case NotContainsResourceType:
			return true;
		default:
			return false;
		}
	}

	//*************************************************************************
	// Method:		SetTestPausedEvent
	// Description: Sets an event that will notify Holodeck that the AUT is
	//				being paused during a test
	//
	// Parameters: 
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void SettingsManager::SetTestPausedEvent()
	{
		pauseEvent.Signal();
	}

	//*************************************************************************
	// Method:		WaitForTestPausedEvent
	// Description: Waits for the event that is triggered when the AUT is
	//				being paused during a test
	//
	// Parameters: 
	//	timeout - Maximum time to wait in milliseconds
	//
	// Return Value: true if the event is triggered, false if not
	//*************************************************************************
	bool SettingsManager::WaitForTestPausedEvent(DWORD timeout)
	{
		if (pauseEvent.Acquire(timeout) == EVENT_SIGNALED)
			return true;
		return false;
	}
}