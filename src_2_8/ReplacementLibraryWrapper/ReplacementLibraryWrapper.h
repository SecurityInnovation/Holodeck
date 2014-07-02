#ifndef __REPLACEMENTLIBRARYWRAPPER_H__
#define __REPLACEMENTLIBRARYWRAPPER_H__

#include "ReplacementLibrary.h"
#include "SettingsManager.h"
#include "FunctionTestList.h"

using namespace Replacement;
using namespace SiUtils;

struct ParameterTypedArray
{
	void** paramValues;
	ParameterType* paramTypes;
};

extern "C" void __stdcall InitReplacementLibraryWrapper();
extern "C" int __stdcall GetSiStringLength(SiString* str);
extern "C" BOOL __stdcall GetSiStringContents(SiString* str, wchar_t* out, int len);
extern "C" void __stdcall FreeSiString(SiString* strPtr);
extern "C" FunctionTestList* __stdcall GetFunctionTests(wchar_t* category, wchar_t* functionName);
extern "C" void __stdcall FreeFunctionTests(FunctionTestList* testList);
extern "C" ParameterTypedArray* __stdcall CreateParamTypedArrays(int count);
extern "C" void __stdcall FreeParamTypedArrays(ParameterTypedArray* paramArrays, int count);
extern "C" void __stdcall SetParamString(ParameterTypedArray* paramArrays, int index, wchar_t* value);
extern "C" BOOL __stdcall DoesTestMatch(int paramCount, ParameterTypedArray* paramArrays, FunctionTestList* testList,
							  SiString** exceptionStr, SiString** returnValueStr, int* matchingTest);
extern "C" BOOL __stdcall IsCategoryLoggingEnabled(wchar_t* category);
extern "C" BOOL __stdcall IsFunctionLoggingEnabled(wchar_t* category, wchar_t* functionName);
extern "C" void __stdcall SendLog(wchar_t* originalDllName, wchar_t* category, wchar_t* functionName, int paramCount,
						ParameterTypedArray* paramArrays, wchar_t* exception, wchar_t* returnValue, LogEventList* events);

extern "C" int __stdcall CheckFiringSpecification (SiArray<SiString>* callStackList, FunctionTestList * functionTestList, int matchingTestIndex);
extern "C" int __stdcall GetFiringOptionForTest(FunctionTestList * functionTestList, int matchingTestIndex);
extern "C" BOOL __stdcall AddFunctionToCallStackList(SiArray<SiString>* callStackList, wchar_t* functionName);
extern "C" SiArray<SiString>* __stdcall InitCallStackList();
extern "C" BOOL __stdcall DeInitCallStackList(SiArray<SiString>* callStackList);

extern "C" LogEventList* __stdcall CreateNewEventsArray();
extern "C" LogEventList* __stdcall InitNewEventsArrayAndSavePrevious(LogEventList *events);
extern "C" void __stdcall RestorePreviousEventsArray(LogEventList* previousEventArray);
extern "C" void __stdcall DeleteEventArray(LogEventList* eventArray);

extern "C" void __stdcall SuspendProcess();
extern "C" void __stdcall FlushLog();
extern "C" void __stdcall IncrementTestExecutionCount(FunctionTestList * functionTestList, int matchingTestIndex);
extern "C" void __stdcall SetPauseEventInLogEntry();

extern "C" int __stdcall GetFunctionCallCount (FunctionTestList * functionTestList, int matchingTestIndex);

extern "C" BOOL __stdcall IsParentLogged();
extern "C" void __stdcall SetParentLogged(BOOL value);

#endif
