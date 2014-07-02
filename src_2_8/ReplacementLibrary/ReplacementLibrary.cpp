//*************************************************************************
// Copyright (C) Security Innovation, LLC, 2002-2003 – All Rights Reserved.
//
// FILE:		ReplacementLibrary.cpp
//
// DESCRIPTION: Contains implementation for the class ReplacementLibrary.
//
//=========================================================================
// Modification History
//
// Date         SCR  Name        Purpose
// -----------  ---  ----------- ------------------------------------------
// 13 DEC 2002		 B. Shirey	 File created.
//*************************************************************************
#include <stdio.h>
#include "ReplacementLibrary.h"
#include "LogEventList.h"

namespace Replacement
{
	//*************************************************************************
	// Method:		ReplacementLibrary
	// Description: Constructor for the ReplacementLibrary class
	//
	// Parameters:
	//	dllName - The filename of the dll this replacement library replaces
	//		Assumptions – Will be a valid file.
	//
	// Return Value: None
	//*************************************************************************
	ReplacementLibrary::ReplacementLibrary(char *dllName)
	{
		originalDllName = dllName;

		//add all standard reg handles
		char handleString[36];
		_ultoa((unsigned long)HKEY_CLASSES_ROOT, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_CLASSES_ROOT");
		_ultoa((unsigned long)HKEY_CURRENT_USER, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_CURRENT_USER");
		_ultoa((unsigned long)HKEY_LOCAL_MACHINE, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_LOCAL_MACHINE");
		_ultoa((unsigned long)HKEY_USERS, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_USERS");
		_ultoa((unsigned long)HKEY_PERFORMANCE_DATA, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_PERFORMANCE_DATA");
		_ultoa((unsigned long)HKEY_PERFORMANCE_TEXT, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_PERFORMANCE_TEXT");
		_ultoa((unsigned long)HKEY_PERFORMANCE_NLSTEXT, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_PERFORMANCE_NLSTEXT");
		_ultoa((unsigned long)HKEY_CURRENT_CONFIG, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_CURRENT_CONFIG");
		_ultoa((unsigned long)HKEY_DYN_DATA, handleString, 10);
		resourceMappings.Add(handleString, "HKEY_DYN_DATA");

		settingsMgr = new SettingsManager(GetCurrentProcessId(), false);
		
		currentTlsSlot = TlsAlloc();

		resourceMappingMutex = new SiUtils::Mutex();
		resourceMappingMutex->Create("resourceMapping");

		char pidStr[16];
		_ultoa(GetCurrentProcessId(), pidStr, 10);
		SiString testPauseMutexName = "HolodeckTestPauseMutex_";
		testPauseMutexName += pidStr;
		testPauseMutex = new SiUtils::Mutex();
		testPauseMutex->Create(testPauseMutexName);
	}

	//*************************************************************************
	// Method:		~ReplacementLibrary
	// Description: Destructor for the ReplacementLibrary class
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	ReplacementLibrary::~ReplacementLibrary()
	{
		//delete any files we've created
		//bugbug - will only work if app terminates normally.
		for (unsigned int i=0; i<createdFileList.GetLength(); i++)
		{
			DeleteFile(createdFileList[i]);
		}

		if (settingsMgr != NULL)
		{
			delete settingsMgr;
			settingsMgr = NULL;
		}

		resourceMappingMutex->Close();
		testPauseMutex->Close();
	}

	//*************************************************************************
	// Method:		GetOriginalFunction
	// Description: Gets the address of the original function from the original
	//	library that this is replacing.  ie gets the real RegCloseKey function
	//	address for the RegCloseKeyReplacement function.
	//
	// Parameters:
	//	functionName - the name of the original function to get.
	//
	// Return Value: the address of the original function, null if not found
	//*************************************************************************
	void *ReplacementLibrary::GetOriginalFunction(const SiString &functionName)
	{
		HMODULE module;
		static void* (WINAPI *getOriginalCallerFunc)(char*, char*) = NULL;
		
		if (getOriginalCallerFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return NULL;

			getOriginalCallerFunc = (void* (WINAPI *)(char*, char*))::GetProcAddress(module, ORIGINAL_CALLER_FUNC_NAME);
		}

		return getOriginalCallerFunc(functionName, originalDllName);
	}

	//*************************************************************************
	// Method:		RunLimitTests
	// Description: Performs Disk,Memory and Network Limit tests.  Gives
	//	back as a result the error code and return value to return from the
	//	replacement function
	//
	// Parameters:
	//	sender - The LogSender object to use to send this log entry
	//	ofCaller - The function to call the original function given by functionName
	//	category - The category the function belongs to
	//	functionName - the name of the original function to get.
	//	numParams - The number of parameters in the paramValues array
	//	paramValues - The values of the parameters passed to the replacement
	//		function
	//	paramTypes - The types of the parameterValues
	//	returnValue - The value to return from the replacement function
	//	errorCode - The error code to set on return from the replacement function
	//  limitInfo - size the current function is trying to allocate, ignored when category = "NETWORK"
	//
	// Return Value: true if successful, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::RunLimitTests(LogSender *sender, OriginalFunctionCaller ofCaller, char *category, char *functionName, int numParams, void **paramValues, ParameterType *paramTypes, DWORD *returnValue, char *returnType, DWORD *errorCode, unsigned long limitInfo)
	{
		LogEventList events;
		bool limitRestricted = false;
		DWORD64 currentUsage = 0;
		DWORD64 maximumAllowed = 0;

		if (strcmp (category, "FILE") == 0)
		{
			if (settingsMgr->GetCachedMaximumDiskUsage(maximumAllowed))
			{
				if (!settingsMgr->GetCurrentCachedDiskUsage(currentUsage))
					currentUsage = 0;

				if (currentUsage + limitInfo > maximumAllowed)
				{
					*errorCode = 112;
					*returnValue = false;
					limitRestricted = true;
				}
			}
		}
		else if (strcmp (category, "MEMORY") == 0)
		{
			if (settingsMgr->GetCachedMaximumMemoryUsage(maximumAllowed))
			{
				if (!settingsMgr->GetCurrentCachedMemoryUsage (currentUsage))
					currentUsage = 0;

				if (currentUsage + limitInfo > maximumAllowed)
				{
					*errorCode = 8;
					*returnValue = NULL;
					limitRestricted = true;
				}
			}
		}
		
		if (limitRestricted)
		{
			if (this->IsFunctionLoggingEnabled(category, functionName))
				this->SendLog(sender, GetCurrentThreadId(), category, functionName, numParams, paramValues, paramTypes, errorCode, returnValue, returnType, events);
			return true;
		}

		return false;
	}

	//*************************************************************************
	// Method:		RunStandardTestsAndGetResults
	// Description: Performs standard replacement function instructions such
	//	as checking for tests, calling the original function, and logging.  Gives
	//	back as a result the error code and return value to return from the
	//	replacement function
	//
	// Parameters:
	//	sender - The LogSender object to use to send this log entry
	//	ofCaller - The function to call the original function given by functionName
	//	category - The category the function belongs to
	//	functionName - the name of the original function to get.
	//	numParams - The number of parameters in the paramValues array
	//	paramValues - The values of the parameters passed to the replacement
	//		function
	//	paramTypes - The types of the parameterValues
	//	paramSizes - The sizes in bytes of each parameter
	//	returnValue - The value to return from the replacement function
	//	errorCode - The error code to set on return from the replacement function
	//
	// Return Value: FALSE if the original function was called, TRUE otherwise
	//*************************************************************************
	bool ReplacementLibrary::RunStandardTestsAndGetResults(LogSender *sender, OriginalFunctionCaller ofCaller, char *category, char *functionName, int numParams, void **paramValues, ParameterType *paramTypes, DWORD *returnValue, char *returnType, DWORD *errorCode, DWORD allocationInfo, bool positiveAllocation)
	{
		bool shouldPauseApp = false;
		bool runOriginalFunction = false;
		// Set up an event array to hold special events for this log entry (i.e. file corruptions or network packets)
		LogEventList events;
		LogEventList* previousEventArray = LogEventList::SetListForCurrentThread(&events);

		bool testMatches = false;
		int matchingTest = -1;
		FunctionTestList testList;
		FunctionTest *functionTest = NULL;

		//first check to see if we should run a limit
		if ((allocationInfo != 0) && positiveAllocation)
		{
			if (RunLimitTests(sender, ofCaller, category, functionName, numParams, paramValues, paramTypes, returnValue, returnType, errorCode, allocationInfo))
			{
				LogEventList::SetListForCurrentThread(previousEventArray);
				return true;
			}
		}

		if (this->GetFunctionTests(category, functionName, testList))
		{
			testMatches = this->DoesTestMatch(paramValues, numParams, false, GetResourceListForCurrentThread(), testList, errorCode, returnValue, allocationInfo, positiveAllocation, &matchingTest);
		}

		//If none of the tests matched then call the real function and skip testing
		if (testMatches)
		{
			//get the function test which matched
			functionTest = testList.GetTestAt(matchingTest);

			//check the firing details of this test
			if (this->CheckFiringSpecification (functionTest))
			{
				//Set the bit for pausing the AUT after this test if requested
				shouldPauseApp = functionTest->GetPauseAppOnTestFire();

				//honor passthrough flags on test
				if (functionTest->GetPassThrough())
				{
					DWORD oldRetVal = *returnValue;
					DWORD oldErrorCode = *errorCode;

					//create file if necessary
					SiString resourceName;
					CheckResource checkResource = functionTest->GetCheckResource();
					if ( (checkResource.Exists == CREATE) && (checkResource.ParamIndex >=0) )
					{
         				resourceName = *((SiString *) paramValues[checkResource.ParamIndex]);
						if (GetFileAttributes(resourceName) == INVALID_FILE_ATTRIBUTES)
						{
							HANDLE fileHandle = CreateFile(resourceName, GENERIC_WRITE, FILE_SHARE_DELETE|FILE_SHARE_WRITE|FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
							CloseHandle(fileHandle);
							createdFileList.Add(resourceName);
						}
					}

					ofCaller(numParams, paramValues, returnValue, errorCode);
					
					if (!functionTest->GetPassThroughReturnValue())
						*returnValue = oldRetVal;

					if (!functionTest->GetPassThroughErrorCode())
						*errorCode = oldErrorCode;
				}
			}
			else
				runOriginalFunction = true;
		}
		else
			runOriginalFunction = true;

		bool loggingEnabled = this->IsFunctionLoggingEnabled(category, functionName);
		bool parentLogged = LogSender::IsParentLogged();

		if (!parentLogged)
		{
			// If parent is not logged, do not log this function either
			loggingEnabled = false;
		}

		LogSender::SetParentLogged(loggingEnabled);

		//If original function should be executed; then do it
		//else increment the execution count for the test
		if (runOriginalFunction)
		{
			//we may need to match on an outparam
			TestSpecialFunctionsAndCallOriginal(ofCaller, category, functionName, paramValues, numParams, returnValue, errorCode, allocationInfo, positiveAllocation);
		}

		else
		{
			if (functionTest != NULL)
			{
				this->settingsMgr->IncrementTestExecutionCount (functionTest->GetRegistryPath());
			}
		}
	
		if (loggingEnabled)
		{
			//dereference all registry phkey types so that registry pane can put reg paths together in the ui.
			if (strcmp(category, "REGISTRY") == 0)
			{
				for (int i=0; i < numParams; i++)
				{
					if (paramTypes[i] == PointerPointerType)
					{
						if (paramValues[i] != NULL)
						{
							PHKEY phKey = (PHKEY) paramValues[i];
							paramValues[i] = *phKey;
						}
						paramTypes[i] = PointerType;
					}
				}
			}
		}

		//If AUT should be paused after this test then set the pause event in the current LogEntry
		if (!runOriginalFunction && shouldPauseApp)
		{
			Log::LogEvent event;
			event.SetType(Log::PauseAUTEvent);
			ReplacementLibrary::AddEventToCurrentEntry(event);
		}

		// change the change params
		if (matchingTest != -1)
			this->ModifyChangeParameters(numParams, paramValues, paramTypes, &testList, matchingTest);

		if (loggingEnabled)
		{
			this->TestFunctionForOutStringLogging(functionName, paramValues, paramTypes, numParams, returnValue, errorCode);
			this->SendLog(sender, GetCurrentThreadId(), category, functionName, numParams, paramValues, paramTypes, errorCode, returnValue, returnType, events);
		}

		//If the app is going to be paused the flush the log now
		if (!runOriginalFunction && shouldPauseApp)
		{
			//Flush the log
			this->FlushLog(sender);
		}

		// Restore original parent logging value
		LogSender::SetParentLogged(parentLogged);

		// Restore previous event array
		LogEventList::SetListForCurrentThread(previousEventArray);

		//throw exceptions if necessary
		if (testMatches)
		{
			SiString exception = functionTest->GetException();
			if (exception.GetLength() != 0)
			{
				unsigned long exceptionValue = strtoul(exception, NULL, 10);
				throw (exceptionValue);
			}
		}

		bool ret = testMatches;
		//return false if we passed through and made the original function call
		if (testMatches)
			ret = testMatches && !functionTest->GetPassThrough();

		//Performing app pause here
		if (!runOriginalFunction && shouldPauseApp)
		{
			testPauseMutex->Acquire();
			settingsMgr->SetTestPausedEvent();
			this->SuspendProcess();
			testPauseMutex->Release();
		}

		return ret;
	}

	//*************************************************************************
	// Method:		TestSpecialFunctions
	// Description: Handles special cases, functions we need to test on out params
	//
	// Parameters:
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ReplacementLibrary::TestSpecialFunctionsAndCallOriginal(OriginalFunctionCaller ofCaller, char *category, char *functionName, void **paramValues, int numParams, DWORD *returnValue, DWORD *errorCode, DWORD allocationInfo, bool positiveAllocation)
	{		
		int matchingTest = -1;
		FunctionTestList testList;
		SiString resourcePath;

		if ( (strcmp("RegEnumKeyW", functionName) == 0) || (strcmp("RegEnumKeyA", functionName) == 0) )
		{
			ofCaller(numParams, paramValues, returnValue, errorCode);
			if (this->GetFunctionTests(category, functionName, testList))
			{
				//stitch together resource path
				if (strcmp("RegEnumKeyW", functionName) == 0)
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), *((LPWSTR *)paramValues[2]));
				else
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), *((LPSTR *)paramValues[2]));

				//add to pathlist for use by doestestmatch
				AddToPathList(2, resourcePath);
				if (this->DoesTestMatch(paramValues, numParams, true, GetResourceListForCurrentThread(), testList, errorCode, returnValue, allocationInfo, positiveAllocation, &matchingTest))
				{
					//if error code == 259 then call ofcaller again - we pretend the key queried for doesn't exist and move to the next one
					if (*returnValue == 259)
					{
						(*(DWORD*)paramValues[1])++; //increment dwIndex;
						ofCaller(numParams, paramValues, returnValue, errorCode);
					}
				}
				//clear pathlist so later tests don't get confused
				ClearPathList();
			}
		}
		else if ( (strcmp("RegEnumKeyExW", functionName) == 0) || (strcmp("RegEnumKeyExA", functionName) == 0) )
		{
			//save in/out buffer sizes
			DWORD lpcName; 
			DWORD lpcClass; 
			
			if (*((LPDWORD *)paramValues[3]))
				lpcName = *(*((LPDWORD *)paramValues[3]));
			if (*((LPDWORD *)paramValues[6]))
				lpcClass = *(*((LPDWORD *)paramValues[6]));

			ofCaller(numParams, paramValues, returnValue, errorCode);
			if (this->GetFunctionTests(category, functionName, testList))
			{
				//stitch together resource path
				if (strcmp("RegEnumKeyExW", functionName) == 0)
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), *((LPWSTR *)paramValues[2]));
				else
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), *((LPSTR *)paramValues[2]));

				//add to pathlist for use by doestestmatch
				AddToPathList(2, resourcePath);
				if (this->DoesTestMatch(paramValues, numParams, true, GetResourceListForCurrentThread(), testList, errorCode, returnValue, allocationInfo, positiveAllocation, &matchingTest))
				{
					//if error code == 259 then call ofcaller again - we pretend the key queried for doesn't exist and move to the next one
					if (*returnValue == 259)
					{
						//increment dwIndex
						(*(DWORD*)paramValues[1])++; 
						//reset in/out buffer size vars
						(*((LPDWORD *)paramValues[3])) = &lpcName;
						(*((LPDWORD *)paramValues[6])) = &lpcClass;
						ofCaller(numParams, paramValues, returnValue, errorCode);
					}
				}
				//clear pathlist so later tests don't get confused
				ClearPathList();
			}
		}
		else if ( (strcmp("RegEnumValueW", functionName) == 0) || (strcmp("RegEnumValueA", functionName) == 0) )
		{
			//save in/out buffer sizes
			DWORD lpcValueName; 
			DWORD lpcbData; 
			
			if (*((LPDWORD *)paramValues[3]))
				lpcValueName = *(*((LPDWORD *)paramValues[3]));
			if (*((LPDWORD *)paramValues[7]))
				lpcbData = *(*((LPDWORD *)paramValues[7]));

			ofCaller(numParams, paramValues, returnValue, errorCode);
			if (this->GetFunctionTests(category, functionName, testList))
			{
				//stitch together resource path
				if (strcmp("RegEnumValueW", functionName) == 0)
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), *((LPWSTR *)paramValues[2]));
				else
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), *((LPSTR *)paramValues[2]));

				//add to pathlist for use by doestestmatch
				AddToPathList(2, resourcePath);
				if (this->DoesTestMatch(paramValues, numParams, true, GetResourceListForCurrentThread(), testList, errorCode, returnValue, allocationInfo, positiveAllocation, &matchingTest))
				{
					//if error code == 259 then call ofcaller again - we pretend the key queried for doesn't exist and move to the next one
					if (*returnValue == 259)
					{
						//increment dwIndex
						(*(DWORD*)paramValues[1])++; 
						//reset in/out buffer size vars
						(*((LPDWORD *)paramValues[3])) = &lpcValueName;
						(*((LPDWORD *)paramValues[7])) = &lpcbData;
						ofCaller(numParams, paramValues, returnValue, errorCode);
					}
				}
				//clear pathlist so later tests don't get confused
				ClearPathList();
			}
		}
		else if ( (strcmp("RegQueryMultipleValuesA", functionName) == 0) || (strcmp("RegQueryMultipleValuesW", functionName) == 0) )
		{
			SiString value;
			PVALENT pvalEnt;
			DWORD numVals;

			ofCaller(numParams, paramValues, returnValue, errorCode);

			if (this->GetFunctionTests(category, functionName, testList))
			{
				pvalEnt = *((PVALENTA *)paramValues[1]);
				numVals = *((DWORD *)paramValues[2]);

				for (DWORD i=0; (pvalEnt) && i<numVals; i++)
				{
					VALENT valEnt = pvalEnt[i];
					if ( (valEnt.ve_type == REG_SZ) && (strcmp("RegQueryMultipleValuesA", functionName) == 0) )
					{
						value = (char*)valEnt.ve_valuename;
						value += "\\";
						value += (char*)valEnt.ve_valueptr;
					}
					else if ( (valEnt.ve_type == REG_SZ) && (strcmp("RegQueryMultipleValuesW", functionName) == 0) )
					{
						value = (wchar_t*)valEnt.ve_valuename;
						value += "\\";
						value += value = (wchar_t*)valEnt.ve_valueptr;
					}
					else
						continue;
					
					resourcePath = ConcatResourcePath(*((HANDLE *)paramValues[0]), value);
					//add to pathlist for use by doestestmatch
					AddToPathList(1, resourcePath);
					//only need one match and then we're done
					if (this->DoesTestMatch(paramValues, numParams, true, GetResourceListForCurrentThread(), testList, errorCode, returnValue, allocationInfo, positiveAllocation, &matchingTest))
					{
						ClearPathList();
						ZeroMemory(pvalEnt, sizeof(VALENT)*numVals);
						break;
					}

					ClearPathList();
				}
			}
		}
		else
			ofCaller(numParams, paramValues, returnValue, errorCode);

	}

	//*************************************************************************
	// Method:		TestFunctionForOutStringLogging
	// Description: Determines whether or not a given function has string out
	//	parameters and if the function call was not successful, logs an empty
	//	string
	//
	// Parameters:
	//	functionName - the name of the original function to get.
	//	paramValues - The values of the parameters passed to the replacement
	//		function
	//	paramTypes - The types of the parameterValues
	//	numParams - The number of parameters in the paramValues array
	//	returnValue - The value to return from the replacement function
	//	errorCode - The error code to set on return from the replacement function
	//
	// Return Value: true if there were tests, false otherwise
	//*************************************************************************
	void ReplacementLibrary::TestFunctionForOutStringLogging(char *functionName, void **paramValues, ParameterType *paramTypes, int numParams, DWORD *returnValue, DWORD *errorCode)
	{
		bool foundFunction = false;
		int paramIndexOfOutString = 0;
		bool ascii = false;

		if ((strcmp(functionName, "RegEnumKeyA") == 0) ||
			(strcmp(functionName, "RegEnumKeyExA") == 0) ||
			(strcmp(functionName, "RegEnumValueA") == 0) ||
			(strcmp(functionName, "RegQueryValueA") == 0))
		{
			foundFunction = true;
			ascii = true;
			paramIndexOfOutString = 2;
		}
		else if ((strcmp(functionName, "RegEnumKeyW") == 0) ||
			(strcmp(functionName, "RegEnumKeyExW") == 0) ||
			(strcmp(functionName, "RegEnumValueW") == 0) ||
			(strcmp(functionName, "RegQueryValueW") == 0))
		{
			foundFunction = true;
			ascii = false;
			paramIndexOfOutString = 2;
		}
		else if (strcmp(functionName, "RegQueryInfoKeyA") == 0)
		{
			foundFunction = true;
			ascii = true;
			paramIndexOfOutString = 1;
		}
		else if (strcmp(functionName, "RegQueryInfoKeyW") == 0)
		{
			foundFunction = true;
			ascii = false;
			paramIndexOfOutString = 1;
		}
		else if (strcmp(functionName, "RegQueryMultipleValuesA") == 0)
		{
			foundFunction = true;
			ascii = true;
			paramIndexOfOutString = 3;
		}
		else if (strcmp(functionName, "RegQueryMultipleValuesW") == 0)
		{
			foundFunction = true;
			ascii = false;
			paramIndexOfOutString = 3;
		}

		if (foundFunction)
		{
			if (*returnValue != ERROR_SUCCESS)
			{
				if (ascii)
					*((LPSTR *)paramValues[paramIndexOfOutString]) = " ";
				else
					*((LPWSTR *)paramValues[paramIndexOfOutString]) = L" ";
			}
		}
		// check the functions that don't return error_success
		else
		{
			if ((strcmp(functionName, "lstrcpyA") == 0) ||
					 (strcmp(functionName, "lstrcatA") == 0))
			{
				if (*returnValue == NULL)
				{
					*((LPSTR *)paramValues[0]) = " ";
				}
			}
			else if ((strcmp(functionName, "lstrcpyW") == 0) ||
					 (strcmp(functionName, "lstrcatW") == 0))
			{
				if (*returnValue == NULL)
				{
					*((LPWSTR *)paramValues[0]) = L" ";
				}
			}
			else if (strcmp(functionName, "gets") == 0)
			{
				if (*returnValue == NULL)
				{
					*((char **)paramValues[0]) = " ";
				}
			}
		}
	}

	//*************************************************************************
	// Method:		GetFunctionTests
	// Description: Determines whether or not a given function has a test 
	//				associated with it, and if it does, gives the list to the
	//				caller
	//
	// Parameters:
	//	category (IN) - A string representing the category of the function
	//  functionName (IN) - The name of the function to test for direct faults
	//  testList (OUT) - The list of tests associated with the function
	//
	// Return Value: true if there were tests, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::GetFunctionTests(const char *category, const char *functionName, FunctionTestList &testList)
	{
		// first get the thread specific tests
		bool returnValue = settingsMgr->GetFunctionTests(GetCurrentThreadId(), category, functionName, testList);
		// then get the common tests
		returnValue = returnValue || (settingsMgr->GetFunctionTests(0, category, functionName, testList));

		if ((!returnValue) || (testList.GetTestCount() == 0))
			return false;

		return true;
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
	//	considerOutParams - if false ignore out params, they may contain garbage
	//	resourcePathList - contains a list of full resource paths in case we are doing a resource test
	//  testList - The list of tests to try to match
	//
	// Return Value: true if one or more do match, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::DoesTestMatch(void **parameters, int numParameters, bool considerOutParams, SiArray<SiString> *ResourcePathList, FunctionTestList &testList, DWORD *matchingErrorCode, DWORD *matchingReturnValue, unsigned long allocInfo, bool positiveAllocation, int *matchingTest)
	{
		*matchingTest = -1;
		SiString errorCodeStr, returnValueStr;

		bool result = settingsMgr->DoesTestMatch(parameters, numParameters, considerOutParams, ResourcePathList, testList, errorCodeStr, returnValueStr, allocInfo, positiveAllocation, matchingTest);

		if (result)
		{
			*matchingErrorCode = atoi((char*)errorCodeStr);
			*matchingReturnValue = atoi((char*)returnValueStr);
		}
		return result;
	}

	//*************************************************************************
	// Method:		IsCategoryLoggingEnabled
	// Description: Determines whether or not logging is enabled for a given
	//	category of functions
	//
	// Parameters:
	//	category - The category to check for logging enabled
	//
	// Return Value: true if enabled, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::IsCategoryLoggingEnabled(const char *category)
	{
		return settingsMgr->IsCategoryLoggingEnabled(category);
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
	// Return Value: true if enabled, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::IsFunctionLoggingEnabled(const char *category, const char *functionName)
	{
		return settingsMgr->IsFunctionLoggingEnabled(category, functionName);
	}

	//*************************************************************************
	// Method:		CallLoggingDisabledFunction
	// Description: Calls the original function for a function that has logging
	//	disabled.
	//
	// Parameters:
	//	ofCaller - The function that calls the original function
	//	numParams - The number of parameters in <parameters>
	//	parameters - The values to pass as parameters to the original function
	//
	// Return Value: A void * that can be casted to the return type of the 
	//	original function and contains the value returned from the original
	//	function
	//*************************************************************************
	void *ReplacementLibrary::CallLoggingDisabledFunction(OriginalFunctionCaller ofCaller, unsigned int numParams, void **parameters)
	{
		DWORD tempReturnValue, errorCode;
		DWORD placeHolder = 0;
		void *returnValue = &placeHolder;

		ofCaller(numParams, parameters, &tempReturnValue, &errorCode);
		CopyMemory(returnValue, &tempReturnValue, sizeof(DWORD));

		return returnValue;
	}

	//*************************************************************************
	// Method:		SendLog
	// Description: Sends a log of a function to the HoloScript object listening
	//	for them
	//
	// Parameters:
	//	sender - The LogSender object to use to send this log entry
	//	threadID - The ID of the thread sending the log
	//	category - The category to check for logging enabled
	//	functionName - The function to check for logging enabled
	//	numParams - The number of parameters in the parameters array
	//	parameters - A list of parameter values to a function.  Note all strings
	//		must be converted from char*, wchar_t*, etc to an SiString object
	//		to be correctly interpreted.
	//	paramTypes - A list of parameter types to a function (must be same #
	//		elements as parameters has)
	//	errorCode - The error code from the function
	//	returnValue - The return value from the function
	//  eventList - Events attached to the log entry
	//
	// Return Value: true if enabled, false otherwise
	//*************************************************************************
	void ReplacementLibrary::SendLog(LogSender *sender, long threadID, const SiString &category, const SiString &functionName, unsigned int numParams, void **parameters, ParameterType *paramTypes, DWORD *errorCode, DWORD *returnValue, char *returnType, const LogEventList& eventList)
	{
		if (sender != NULL)
		{
			// convert threadID to string
			char threadIDStr[16];
			ZeroMemory(threadIDStr, 16);
			_ultoa(threadID, threadIDStr, 10);

			char returnValueStr[16];
			ZeroMemory(returnValueStr, 16);
			//convert the return value appropriately
			if (StrCmpI (returnType, "int") == 0)
				itoa ((int) *returnValue, returnValueStr, 10);
			else if (StrCmpI (returnType, "long") == 0)
				ltoa ((long) *returnValue, returnValueStr, 10);
			else if (StrCmpI (returnType, "bool") == 0)
			{
				//convert bool to friendly string
				if (*returnValue == 0)
					StrCpy (returnValueStr, "False");
				else
					StrCpy (returnValueStr, "True");
			}
			else if ( (StrCmpI (returnType, "dword") == 0) || (StrCmpI (returnType, "size_t") == 0) || (StrCmpI (returnType, "socket") == 0) )
			{			
				_ultoa(*returnValue, returnValueStr, 10);
			}
			else
			{
				//all other types are pointer types and can be NULL
				if (*returnValue == 0)
					StrCpy(returnValueStr, "NULL");
				else
					_ultoa(*returnValue, returnValueStr, 10);
			}

			//send log
			sender->SendLog(threadID, threadIDStr, category, functionName, originalDllName, numParams, parameters, paramTypes, *errorCode, "", returnValueStr, eventList);
		}
	}

	//*************************************************************************
	// Method:		FlushLog
	// Description: Flushes the Log buffer and send the entries to Holodeck
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::FlushLog(LogSender *sender)
	{
		if (sender != NULL)
		{
			sender->ForceWrite();
		}
	}

	//*************************************************************************
	// Method:		SuspendProcess
	// Description: Suspends all the threads in the current process
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::SuspendProcess()
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
	// Method:		DisableInterception
	// Description: Disables HEAT's interception capabilities until restored by
	//	calling EnableInterception
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::DisableInterception()
	{
		HMODULE module;
		static DWORD (WINAPI *disableFunc)() = NULL;
		
		if (disableFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return;

			disableFunc = (DWORD (WINAPI *)())::GetProcAddress(module, DISABLE_INTERCEPT_FUNC_NAME);
		}

		disableFunc();
	}

	//*************************************************************************
	// Method:		EnableInterception
	// Description: Enables HEAT's interception capabilities that was disabled by
	//	calling DisableInterception
	//
	// Parameters:
	//	None
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::EnableInterception()
	{
		HMODULE module;
		static DWORD (WINAPI *enableFunc)() = NULL;
		
		if (enableFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return;

			enableFunc = (DWORD (WINAPI *)())::GetProcAddress(module, ENABLE_INTERCEPT_FUNC_NAME);
		}

		enableFunc();
	}

	//*************************************************************************
	// Method:		CreateAutThread
	// Description: Creates an aut thread through Heat
	//
	// Parameters:
	//	routine - the thread routine to execute
	//	param - the parameter to pass to the thread function
	//	pdwThreadID (out) - the thread id of the new thread
	//
	// Return Value: the handle to the created thread
	//*************************************************************************
	HANDLE ReplacementLibrary::CreateAutThread(LPTHREAD_START_ROUTINE routine, LPVOID param, DWORD *pdwThreadID)
	{
		return LogSender::CreateAutThread(routine, param, pdwThreadID);
	}
	
	//*************************************************************************
	// Method:		IsAutCreatedThread
	// Description: determines whether or not a thread was created by holodeck
	//	in the aut
	//
	// Parameters:
	//	dwThreadID - the thread id of the thread to check
	//
	// Return Value: true if it was created by us, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::IsAutCreatedThread(DWORD dwThreadID)
	{
		return LogSender::IsAutCreatedThread(dwThreadID);
	}

	//*************************************************************************
	// Method:		UpdateDiskUsage
	// Description: Updates the disk usage information by adding or subtracting
	//	            from the previous value.
	//
	// Parameters: 
	//	allocInfo - Allocation size information in bytes
	//	positiveAllocation - Tells the function whether allocInfo is to be added or sub.
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::UpdateDiskUsage (unsigned long allocInfo, bool positiveAllocation)
	{
		DWORD64 currentlyUsed = 0;

		if (!settingsMgr->GetCurrentCachedDiskUsage (currentlyUsed))
			currentlyUsed = 0;

		if (!positiveAllocation)
		{
			if (currentlyUsed > allocInfo)
				settingsMgr->SetCurrentDiskUsage (currentlyUsed - allocInfo);
			else
				settingsMgr->SetCurrentDiskUsage (0);
		}
		else
			settingsMgr->SetCurrentDiskUsage (currentlyUsed + allocInfo);

	}
	//*************************************************************************
	// Method:		UpdateMemoryUsage
	// Description: Updates the memory usage info
	//
	// Parameters: 
	//	totalMemUsed - The updated memory usage value 
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::UpdateMemoryUsage (unsigned long totalMemUsed)
	{
		settingsMgr->SetCurrentMemoryUsage (totalMemUsed);
	}
	//*************************************************************************
	// Method:		GetMaxDiskInfo
	// Description: Gets the maximum disk limit information
	//
	// Parameters: 
	//	maxDiskUsage - The maximum disk limit info
	//
	// Return Value: False if no limit set, true if limit is set
	//*************************************************************************
	bool ReplacementLibrary::GetMaxDiskInfo(DWORD64 &maxDiskUsage)
	{
		if (settingsMgr->GetCachedMaximumDiskUsage (maxDiskUsage))
			return true;
		else
			return false;
	}

	//*************************************************************************
	// Method:		GetMaxMemInfo
	// Description: Gets the maximum memory limit information
	//
	// Parameters: 
	//	maxMemUsage - The maximum memory limit info
	//
	// Return Value: False if no limit set, true if limit is set
	//*************************************************************************
	bool ReplacementLibrary::GetMaxMemInfo(DWORD64 &maxMemUsage)
	{
		if (settingsMgr->GetCachedMaximumMemoryUsage (maxMemUsage))
			return true;
		else
			return false;
	}

	//*************************************************************************
	// Method:		GetCurrentMemUsage
	// Description: Gets the amount of memory currently used by the application
	//
	// Parameters: 
	//	memUsed - The amount of memory currently used by the application
	//
	// Return Value: False if no limit set, true if limit is set
	//*************************************************************************
	bool ReplacementLibrary::GetCurrentMemUsage(DWORD64 &memUsed)
	{
		if (settingsMgr->GetCurrentCachedMemoryUsage (memUsed))
			return true;
		else
			return false;
	}
	//*************************************************************************
	// Method:		GetCurrentDiskUsage
	// Description: Gets the amount of disk space currently used by the application
	//
	// Parameters: 
	//	memUsed - The amount of disk space currently used by the application
	//
	// Return Value: False if no limit set, true if limit is set
	//*************************************************************************
	bool ReplacementLibrary::GetCurrentDiskUsage(DWORD64 &diskUsed)
	{
		if (settingsMgr->GetCurrentCachedDiskUsage (diskUsed))
			return true;
		else
			return false;
	}
	//*************************************************************************
	// Method:		GetFileSizeInfoA
	// Description: (ANSI version) Gets the size of the specified file in bytes
	//
	// Parameters: 
	//	lpFileName - The complete path and name of the file 
	//
	// Return Value: size of the file, 0 if file not found
	//*************************************************************************
	unsigned int ReplacementLibrary::GetFileSizeInfoA (LPCTSTR lpFileName)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.bInheritHandle = FALSE;
		sa.lpSecurityDescriptor = NULL;
		HANDLE	hFile;

		hFile = CreateFileA (lpFileName, 0, NULL, &sa, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return (0);

		LARGE_INTEGER lpFileSize ;
		GetFileSizeEx( hFile, &lpFileSize);
		CloseHandle (hFile);
		return (unsigned int) (lpFileSize.QuadPart);
	}

	//*************************************************************************
	// Method:		GetFileSizeInfoW
	// Description: (UNICODE version) Gets the size of the specified file in bytes
	//
	// Parameters: 
	//	lpFileName - The complete path and name of the file 
	//
	// Return Value: size of the file, 0 if file not found
	//*************************************************************************	
	unsigned int ReplacementLibrary::GetFileSizeInfoW (LPCWSTR lpFileName)
	{
		SECURITY_ATTRIBUTES sa;
		sa.nLength = sizeof (SECURITY_ATTRIBUTES);
		sa.bInheritHandle = FALSE;
		sa.lpSecurityDescriptor = NULL;
		HANDLE	hFile;

		hFile = CreateFileW (lpFileName, 0, NULL, &sa, OPEN_EXISTING, NULL, NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return (0);

		LARGE_INTEGER lpFileSize ;
		GetFileSizeEx( hFile, &lpFileSize);
		CloseHandle (hFile);
		return (unsigned int) (lpFileSize.QuadPart);
	}

	//*************************************************************************
	// Method:		GetProcessMemory
	// Description: Gets the total memory used by the current process
	//
	// Parameters: 
	//	None
	//
	// Return Value: Total memory used by the process, 0 if could not get
	// handle to process
	//*************************************************************************
	unsigned long ReplacementLibrary::GetProcessMemory()
	{
		PROCESS_MEMORY_COUNTERS ppsmemCounters;
		GetProcessMemoryInfo(GetCurrentProcess(), &ppsmemCounters, sizeof(PROCESS_MEMORY_COUNTERS));
		return (unsigned long)(ppsmemCounters.WorkingSetSize);
	}

	//*************************************************************************
	// Method:		ModifyChangeParameters
	// Description: Changes the values of the change parameters to the values
	//	specified in the test list
	//
	// Parameters: 
	//	numParams - the number of params in the function
	//	paramValues - the values of each parameters
	//	paramTypes - the type of each parameters
	//	paramSizes - the size in bytes of each parameter
	//	testList - the list of tests for this function
	//	matchingTest - the index of the matching test in the list
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::ModifyChangeParameters(int numParams, void **paramValues, ParameterType *paramTypes, FunctionTestList *testList, int matchingTest)
	{
		if (testList == NULL)
			return;

		if ((matchingTest < 0) || (matchingTest >= testList->GetTestCount()))
			return;

		FunctionTest *test = (FunctionTest *)testList->GetTestAt(matchingTest);
		for (int i = 0; i < test->GetChangeParameterCount(); i++)
		{
			Parameter param = test->GetParameterToChangeAt(i);

			CopyMemory(paramValues[param.position], param.value, param.valueSize);
		}
	}

	//*************************************************************************
	// Method:		AddEventToCurrentEntry
	// Description: Adds an event to the log entry for the current thread
	//
	// Parameters:
	//	event - Event to add
	//
	// Return Value: None
	//*************************************************************************
	void ReplacementLibrary::AddEventToCurrentEntry(const LogEvent& event)
	{
		LogEventList::GetListForCurrentThread()->AddEvent(event);
	}

	//*************************************************************************
	// Method:		ConcatResourcePath
	// Description: Concatenates path variables
	//
	// Parameters: 
	// str0 - first part of path
	// str1 - second part of path
	//
	// Return Value: concatenated string
	//*************************************************************************
	SiString ReplacementLibrary::ConcatResourcePath(SiString str0, SiString str1)
	{
		str0 += "\\";
		str0 += str1;

		return str0;
	}

	//*************************************************************************
	// Method:		ConcatResourcePath
	// Description: Concatenates path variables
	//
	// Parameters: 
	//	handle - handle that represents an already opened resource
	//	string - path to concat
	//
	// Return Value: concatenated string
	//*************************************************************************
	SiString ReplacementLibrary::ConcatResourcePath(HANDLE handle, SiString string)
	{
		SiString resourcePath = GetResourceMapping(handle);

		if (resourcePath.GetLength() > 0)
		{
			resourcePath += "\\";
			resourcePath += string;
			return resourcePath;
		}
		else
			return string;
	}

	//*************************************************************************
	// Method:		CreateResourceMapping
	// Description: Adds a mapping between a handle value and a resource path
	//
	// Parameters: 
	//	handle - handle that represents an open resource
	//	resourcePath - resource path to map to the handle
	//
	// Return Value: none
	//*************************************************************************
	void ReplacementLibrary::CreateResourceMapping(HANDLE handle, SiString resourcePath)
	{
		char handleString[36];
		_ultoa((unsigned long) handle, handleString, 10);

		resourceMappingMutex->Acquire();
		resourceMappings.Add(handleString, resourcePath);
		resourceMappingMutex->Release();
	}

	//*************************************************************************
	// Method:		GetResourceMapping
	// Description: Returns a mapping between a handle value and a resource path
	//
	// Parameters: 
	//	handle - handle that represents an open resource
	//
	// Return Value: resourcePath represented by the handle
	//*************************************************************************
	SiString ReplacementLibrary::GetResourceMapping(HANDLE handle)
	{
		char handleString[36];
		HANDLE mappedHandle = NULL;
		SiString resourceName;

		resourceMappingMutex->Acquire();
		_ultoa((unsigned long) handle, handleString, 10);
		if (handleMappings.KeyExists(handleString))
		{
			mappedHandle = handleMappings[handleString];
			_ultoa((unsigned long) mappedHandle, handleString, 10);
		}

		resourceName = resourceMappings[handleString];
		resourceMappingMutex->Release();

		return resourceName;
	}

	//*************************************************************************
	// Method:		CreateHandleMapping
	// Description: Adds a mapping between two handle values
	//
	// Parameters: 
	//	mapToHandle - the value that will be mapped to
	//	mapFromHandle - the value that will be mapped from
	//
	// Return Value: none
	//*************************************************************************
	void ReplacementLibrary::CreateHandleMapping(HANDLE mapFromHandle, HANDLE mapToHandle)
	{
		char handleString[36];
		_ultoa((unsigned long) mapFromHandle, handleString, 10);

		resourceMappingMutex->Acquire();
		handleMappings.Add(handleString, mapToHandle);
		resourceMappingMutex->Release();
	}

	//*************************************************************************
	// Method:		GetHandleMapping
	// Description: Adds a mapping between a handle value and a resource path
	//
	// Parameters: 
	//	handle - handle to look for
	//
	// Return Value: handle this maps to
	//*************************************************************************
	HANDLE ReplacementLibrary::GetHandleMapping(HANDLE handle)
	{
		char handleString[36];
		HANDLE mappedHandle;

		resourceMappingMutex->Acquire();
		_ultoa((unsigned long) handle, handleString, 10);
		mappedHandle = handleMappings[handleString];
		resourceMappingMutex->Release();

		return mappedHandle;
	}

	//*************************************************************************
	// Method:		AddToPathList
	// Description: Adds a resource path to the pathlist
	//
	// Parameters: 
	//	pos - param index in the original function for this resource path
	//	resourcePath - resource path to add to the list
	//
	// Return Value: none
	//*************************************************************************
	void ReplacementLibrary::AddToPathList(unsigned int pos, SiString resourcePath)
	{
		SiArray<SiString> *resourcePathList = GetResourceListForCurrentThread();
		if (resourcePathList)
			resourcePathList->SetAtIndex(pos, resourcePath);
	}

	//*************************************************************************
	// Method:		GetPathFromList
	// Description: Returns a path from the list if it exists
	//
	// Parameters: 
	//	pos - param index in the original function for this resource path
	//
	// Return Value: resource path if it exists or else and empty string
	//*************************************************************************
	SiString ReplacementLibrary::GetPathFromList(unsigned int pos)
	{
		SiString resourcePath;
		SiArray<SiString> *resourcePathList = GetResourceListForCurrentThread();

		if (resourcePathList && (resourcePathList->GetLength() > pos))
			resourcePath = (*resourcePathList)[pos];

		return resourcePath;
	}

	//*************************************************************************
	// Method:		ClearPathList
	// Description: Clears the pathlist
	//
	// Parameters: 
	//	none
	//
	// Return Value: none
	//*************************************************************************
	void ReplacementLibrary::ClearPathList()
	{
		SiArray<SiString> *resourcePathList = GetResourceListForCurrentThread();
		if (resourcePathList)
			resourcePathList->Clear();
	}

	//*************************************************************************
	// Method:		GetResourceListForCurrentThread
	// Description: gets the path list for the current thread
	//
	// Parameters: 
	//	none
	//
	// Return Value: none
	//*************************************************************************
	SiArray<SiString> *ReplacementLibrary::GetResourceListForCurrentThread()
	{
		SiArray<SiString> *resourcePathList;

		if (currentTlsSlot != TLS_OUT_OF_INDEXES)
		{
			resourcePathList = (SiArray<SiString>*)TlsGetValue(currentTlsSlot);
			if (!resourcePathList)
			{
				resourcePathList = new SiArray<SiString>();
				TlsSetValue(currentTlsSlot, resourcePathList);
			}

			return resourcePathList;
		}
		else
			return NULL;
	}

	//*************************************************************************
	// Method:		CheckFiringSpecification
	// Description: Checks, based on firing details, whether the test should be fired
	//				or not
	//
	// Parameters: 
	//	functionTest - The test which is being tested :p
	//
	// Return Value: true if test should run, false otherwise
	//*************************************************************************
	bool ReplacementLibrary::CheckFiringSpecification(FunctionTest *functionTest)
	{
		//Set default to true
		bool testShouldFire = true;

		//Increment function call count
		int functionCallCount = settingsMgr->IncrementFunctionCallCount (functionTest->GetRegistryPath());
		
		//In case there was an error reading the registry key; then do not run the test at all
		if (functionCallCount == -1)
			return false;

		//Get firing details
		SiArray<SiString> firingDetails = functionTest->GetFiringDetails();
		double firePercentage = 0;
		
		int option = functionTest->GetFiringOption();
		
		int action = 0;
		//Perform action according to selected firing option
		if (option == 1)
		{
			action = settingsMgr->CheckFiringSpecification (NULL, functionTest);
		}
		else
		{
			//Store regiter values
			DWORD dwEip = (DWORD) MessageBoxA;					//Point Eip to any valid function
			DWORD dwEsp;										//stackpointer
			DWORD dwEbp;										//basepointer

			__asm mov dword ptr[dwEsp], esp;					//Save esp
			__asm mov dword ptr[dwEbp], ebp;					//Save ebp
			
			//Store values in CONTEXT struct
			CONTEXT c;
			memset( &c, '\0', sizeof c );
			c.ContextFlags = CONTEXT_FULL;
			c.Eip = dwEip;
			c.Esp = dwEsp;
			c.Ebp = dwEbp;

			//Get the stacktrace
			SiArray <FrameInfo *> frameArray = StackWalker::getInstance()->GetStackInfo (GetCurrentThread(), c);
			SiArray <SiString> stackArray;

			//Cycle through the results and keep only the intercepted functions and internal module symbols
			//Filter rules:
			// 1. Filter out where fullname contains string "::ReplacementLibrary::"
			//	  This will take care of RunStandardTestsAndGetResults and other methods
			// 2. Filter out where ImageName contains string "Heat4Dll"
			
			for (int i=1; i < frameArray.GetLength(); i++)			//start with i=1 to skip current eip method
			{
				FrameInfo * fi = frameArray[i];
				
				SiString subString = "";
				SiString checkString = "";
				SiString testName = "";

				//Skip, if image name contains string "::ReplacementLibrary::"
				testName = fi->undecoratedFullName;
				checkString = "::ReplacementLibrary::";
				if (testName.FindFirstIgnoreCase (checkString) != -1)
					continue;

				//Skip, if image name contains string "Heat4Dll"
				testName = fi->ImageName;
				checkString = "Heat4Dll";
				if (testName.FindFirstIgnoreCase (checkString) != -1)
					continue;

				//if method is from a replacement library, then remove
				//the word "Replacement" from the function name
				testName = fi->ImageName;
				checkString = "replacementlibrary";
				if (testName.FindFirstIgnoreCase (checkString) != -1)
				{
					testName = fi->undecoratedFullName;
					checkString = "Replacement";
					subString = "";
					testName.SubString (subString, testName.GetLength()- checkString.GetLength());
					testName.SubString (subString, 0, testName.GetLength() - checkString.GetLength() - 1);
					stackArray.Add (subString);
				}
				else
					stackArray.Add (fi->undecoratedFullName);
				
			}

			action = settingsMgr->CheckFiringSpecification (&stackArray, functionTest);
		
		} // if (option == 1)

		if (action >= 0)
			testShouldFire = true;
		else
			testShouldFire = false;
		
		return testShouldFire;
	}
}