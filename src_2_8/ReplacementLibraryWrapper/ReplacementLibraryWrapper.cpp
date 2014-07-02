#include "ReplacementLibraryWrapper.h"


static SettingsManager* settingsManager = NULL;
static Mutex* testPauseMutex = NULL;


void __stdcall InitReplacementLibraryWrapper()
{
	if (settingsManager == NULL)
		settingsManager = new SettingsManager(GetCurrentProcessId(), false);
	if (!commonLogSender.GetIsSendLogThreadRunning())
		commonLogSender.StartSendLogThread();

	if (testPauseMutex == NULL)
	{
		char pidStr[16];
		_ultoa(GetCurrentProcessId(), pidStr, 10);
		SiString testPauseMutexName = "HolodeckTestPauseMutex_";
		testPauseMutexName += pidStr;
		testPauseMutex = new SiUtils::Mutex();
		testPauseMutex->Create(testPauseMutexName);
	}
}


int __stdcall GetSiStringLength(SiString* str)
{
	return str->GetLength();
}


BOOL __stdcall GetSiStringContents(SiString* str, wchar_t* out, int len)
{
	if (len < (int)(str->GetLength() + 1))
		return FALSE;

	wcscpy(out, (wchar_t*)*str);
	return TRUE;
}


void __stdcall FreeSiString(SiString* strPtr)
{
	delete strPtr;
}


FunctionTestList* __stdcall GetFunctionTests(wchar_t* category, wchar_t* functionName)
{
	FunctionTestList* testList = new FunctionTestList();

	SiString categoryStr(category);
	SiString functionNameStr(functionName);

	// first get the thread specific tests
	BOOL returnValue = settingsManager->GetFunctionTests(GetCurrentThreadId(), (char*)categoryStr,
		(char*)functionNameStr, *testList);
	// then get the common tests
	returnValue = returnValue || (settingsManager->GetFunctionTests(0, (char*)categoryStr,
		(char*)functionNameStr, *testList));

	if ((!returnValue) || (testList->GetTestCount() == 0))
	{
		delete testList;
		return NULL;
	}

	return testList;
}


void __stdcall FreeFunctionTests(FunctionTestList* testList)
{
	delete testList;
}


ParameterTypedArray* __stdcall CreateParamTypedArrays(int count)
{
	ParameterTypedArray* paramArrays = new ParameterTypedArray;
	paramArrays->paramValues = new void*[count];
	paramArrays->paramTypes = new ParameterType[count];
	memset(paramArrays->paramValues, 0, sizeof(void*) * count);
	memset(paramArrays->paramTypes, 0, sizeof(ParameterType) * count);
	return paramArrays;
}

void __stdcall FreeParamTypedArrays(ParameterTypedArray* paramArrays, int count)
{
	for (int i=0; i<count; i++)
	{
		if ((paramArrays->paramTypes[i] == WideStringType) && (paramArrays->paramValues[i]))
			delete (SiString*)paramArrays->paramValues[i];
	}
	delete[] paramArrays->paramValues;
	delete[] paramArrays->paramTypes;
	delete paramArrays;
}


void __stdcall SetParamString(ParameterTypedArray* paramArrays, int index, wchar_t* value)
{
	paramArrays->paramValues[index] = new SiString(value);
	paramArrays->paramTypes[index] = WideStringType;
}


BOOL __stdcall DoesTestMatch(int paramCount, ParameterTypedArray* paramArrays, FunctionTestList* testList,
				   SiString** exceptionRet, SiString** returnValueRet, int* matchingTest)
{
	*matchingTest = -1;
	SiString exceptionStr, returnValueStr;

	BOOL result = settingsManager->DoesTestMatch(paramArrays->paramValues, paramCount, false, NULL, *testList,
		exceptionStr, returnValueStr, 0, true, matchingTest);

	if (result)
	{
		*exceptionRet = new SiString(exceptionStr);
		*returnValueRet = new SiString(returnValueStr);
	}
	else
	{
		*exceptionRet = NULL;
		*returnValueRet = NULL;
	}

	return result;
}


BOOL __stdcall IsCategoryLoggingEnabled(wchar_t* category)
{
	return settingsManager->IsCategoryLoggingEnabled((char*)SiString(category));
}


BOOL __stdcall IsFunctionLoggingEnabled(wchar_t* category, wchar_t* functionName)
{
	return settingsManager->IsFunctionLoggingEnabled((char*)SiString(category), (char*)SiString(functionName));
}


void __stdcall SendLog(wchar_t* originalDllName, wchar_t* category, wchar_t* functionName, int paramCount,
			 ParameterTypedArray* paramArrays, wchar_t* exception, wchar_t* returnValue, LogEventList* events)
{
	// convert threadID to string
	char threadIDStr[16];
	_itoa(GetCurrentThreadId(), threadIDStr, 10);

	commonLogSender.SendLog(GetCurrentThreadId(), threadIDStr, category, functionName, originalDllName, paramCount,
		paramArrays->paramValues, paramArrays->paramTypes, -1, exception, returnValue, *events);
}

int __stdcall CheckFiringSpecification (SiArray<SiString>* callStackList, FunctionTestList * functionTestList, int matchingTestIndex)
{
	return settingsManager->CheckFiringSpecification (callStackList, functionTestList, matchingTestIndex);
}

int __stdcall GetFiringOptionForTest(FunctionTestList * functionTestList, int matchingTestIndex)
{
	FunctionTest * functionTest = functionTestList->GetTestAt (matchingTestIndex);
	if (functionTest == NULL)
		return -1;

	return functionTest->GetFiringOption();
}

LogEventList* __stdcall CreateNewEventsArray()
{
	LogEventList * events = new LogEventList();
	return events;
}

LogEventList* __stdcall InitNewEventsArrayAndSavePrevious(LogEventList *events)
{
	LogEventList* previousEventArray = LogEventList::SetListForCurrentThread(events);
	return previousEventArray;
}

void __stdcall RestorePreviousEventsArray(LogEventList* previousEventArray)
{
	LogEventList::SetListForCurrentThread(previousEventArray);
}

void __stdcall DeleteEventArray(LogEventList* eventArray)
{
	if (eventArray)
		delete eventArray;
}

SiArray<SiString>* __stdcall InitCallStackList()
{
	SiArray <SiString> * list = new SiArray<SiString>();
	return list;
}

BOOL __stdcall DeInitCallStackList(SiArray<SiString>* callStackList)
{
	if (callStackList != NULL)
	{
		//Delete the list
		delete callStackList;
		
		return true;
	}
	
	return false;
}

BOOL __stdcall AddFunctionToCallStackList(SiArray<SiString>* callStackList, wchar_t* functionName)
{
	if (callStackList != NULL)
	{
		const SiString * newStr = new SiString (functionName);
		callStackList->Add (*newStr);
		return true;
	}

	return false;
}

void __stdcall SuspendProcess()
{
	testPauseMutex->Acquire();
	settingsManager->SetTestPausedEvent();
	settingsManager->SuspendProcess();
	testPauseMutex->Release();
}

void __stdcall FlushLog()
{
	commonLogSender.ForceWrite();
}

void __stdcall IncrementTestExecutionCount(FunctionTestList * functionTestList, int matchingTestIndex)
{
		
	FunctionTest * test = functionTestList->GetTestAt (matchingTestIndex);
	settingsManager->IncrementTestExecutionCount (test->GetRegistryPath());
}

void __stdcall SetPauseEventInLogEntry()
{
	Log::LogEvent event;
	event.SetType(Log::PauseAUTEvent);		

	LogEventList* lel = LogEventList::GetListForCurrentThread();
	if (lel)
		lel->AddEvent(event);
}

int __stdcall GetFunctionCallCount (FunctionTestList * functionTestList, int matchingTestIndex)
{
	FunctionTest * test = functionTestList->GetTestAt (matchingTestIndex);
	return settingsManager->GetFunctionCallCount (test->GetRegistryPath());
}

BOOL __stdcall IsParentLogged()
{
	return LogSender::IsParentLogged();
}

void __stdcall SetParentLogged(BOOL value)
{
	LogSender::SetParentLogged(value);
}
