#include <vcclr.h>
#using <mscorlib.dll>
#using <HeatDotNet.dll>
#include "DotNetReplacementLibrary.h"
#include "ReplacementLibrary.h"
#include "SettingsManager.h"


namespace Replacement
{
	static SettingsManager* settingsManager = NULL;


	DotNetReplacementLibrary::DotNetReplacementLibrary(System::String* dllName)
	{
		originalDllName = dllName;

		if (settingsManager == NULL)
			settingsManager = new SettingsManager(GetCurrentProcessId(), false);
		if (!commonLogSender.GetIsSendLogThreadRunning())
			commonLogSender.StartSendLogThread();
	}


	int DotNetReplacementLibrary::ConvertParamsToTypedArrays(Object* params __gc[],
		void** &paramValues, ParameterType* &paramTypes)
	{
		int startIndex = 0;
		// First parameter is the MethodInterceptInfo object.  If ThisObject within
		// this is null, we have a static method and shouldn't include ThisObject
		// in the log entry
		if (params[0]->GetType()->IsSubclassOf(__typeof(HeatDotNet::MethodInterceptInfo)) &&
			((dynamic_cast<HeatDotNet::MethodInterceptInfo*>(params[0]))->ThisObject == NULL))
			startIndex = 1;

		paramValues = new void*[params->Count - startIndex];
		paramTypes = new ParameterType[params->Count - startIndex];

		for (int i=startIndex; i<params->Count; i++)
		{
			try
			{
				Object* obj = params[i];

				if (obj == NULL)
				{
					paramValues[i - startIndex] = new SiString("null");
					paramTypes[i - startIndex] = WideStringType;
					continue;
				}

				// If the parameter is a MethodInterceptInfo type, log the ThisObject part of the
				// MethodInterceptInfo object.
				if (obj->GetType()->IsSubclassOf(__typeof(HeatDotNet::MethodInterceptInfo)))
					obj = (dynamic_cast<HeatDotNet::MethodInterceptInfo*>(obj))->ThisObject;

				const wchar_t __pin* wszParamStr;
				if (obj == NULL)
					wszParamStr = PtrToStringChars(S"null");
				else
					wszParamStr = PtrToStringChars(obj->ToString());
				paramValues[i - startIndex] = new SiString(wszParamStr);
				paramTypes[i - startIndex] = WideStringType;
			}
			catch (...)
			{
				// Could not convert the parameter to a string.  Fill in the parameter with an "Unknown"
				paramValues[i - startIndex] = new SiString("Unknown");
				paramTypes[i - startIndex] = WideStringType;
			}
		}

		return params->Count - startIndex; // Return actual number of parameters to log
	}


	void DotNetReplacementLibrary::FreeParamTypedArrays(int paramCount, void** paramValues,
		ParameterType* paramTypes)
	{
		for (int i=0; i<paramCount; i++)
		{
			if (paramTypes[i] == WideStringType)
				delete (SiString*)paramValues[i];
		}
		delete[] paramValues;
		delete[] paramTypes;
	}


	bool DotNetReplacementLibrary::RunStandardTestsAndGetResults(OriginalMethodCaller* ofCaller, String* category,
		String* functionName, Object* params __gc[], Type* returnType, Object* &returnValue, Exception* &exception)
	{
		IncrementCallLevel();

		bool testMatches = false;
		int matchingTest = -1;
		FunctionTestList testList;

		void** paramValues;
		ParameterType* paramTypes;

		int typedArrayParams = ConvertParamsToTypedArrays(params, paramValues, paramTypes);

		if (GetFunctionTests(category, functionName, testList))
		{
			testMatches = DoesTestMatch(typedArrayParams, paramValues, paramTypes, testList,
				exception, returnType, returnValue, &matchingTest);
		}

		if (!testMatches)
			ofCaller(params, returnValue, exception);

		bool loggingEnabled = IsFunctionLoggingEnabled(category, functionName);

		if (matchingTest != -1)
			ModifyChangeParameters(typedArrayParams, paramValues, paramTypes, &testList, matchingTest);

		if (loggingEnabled)
			SendLog(category, functionName, typedArrayParams, paramValues, paramTypes, exception, returnValue);

        FreeParamTypedArrays(typedArrayParams, paramValues, paramTypes);

		DecrementCallLevel();
		return testMatches;
	}


	Exception* DotNetReplacementLibrary::ConvertExceptionStringToObject(String* str)
	{
		Type* exceptionType = Type::GetType(str);
		if (!exceptionType)
			return NULL;

		System::Reflection::ConstructorInfo* constructor = exceptionType->GetConstructor(new Type* __gc[0]);
		if (!constructor)
			return NULL;

		return dynamic_cast<Exception*>(constructor->Invoke(new Object* __gc[0]));
	}


	Object* DotNetReplacementLibrary::ConvertValueStringToObject(Type* returnType, String* str)
	{
		if (returnType == __typeof(String))
			return str;
		else if ((returnType == __typeof(Boolean)) || (returnType == __typeof(Byte)) ||
			(returnType == __typeof(SByte)) || (returnType == __typeof(Int16)) ||
			(returnType == __typeof(UInt16)) || (returnType == __typeof(Int32)) ||
			(returnType == __typeof(UInt32)) || (returnType == __typeof(Int64)) ||
			(returnType == __typeof(UInt64)) || (returnType == __typeof(Single)) ||
			(returnType == __typeof(Double)))
		{
			// Numeric type, call the Parse method
			Type* methodParams __gc[] = new Type* __gc[1];
			methodParams[0] = __typeof(String);
			System::Reflection::MethodInfo* method = returnType->GetMethod("Parse", methodParams);
			if (method)
			{
				try
				{
					Object* invokeParams __gc[] = new Object* __gc[1];
					invokeParams[0] = str;
					return method->Invoke(NULL, invokeParams);
				}
				catch (Exception*)
				{
					// Parse failed.  Return zero, as NULL won't work as it's a value type
					return __box(0);
				}
			}
			else
			{
				// No Parse method.  Return zero, as NULL won't work as it's a value type
				return __box(0);
			}
		}
		else if (returnType == __typeof(Object))
		{
			// Returning an Object, could be any type.
			if (str->ToLower()->Equals("null"))
			{
				// User gave "null", return a NULL pointer
				return NULL;
			}

			if (str->ToLower()->Equals("true"))
			{
				// Return boolean true
				return __box(true);
			}

			if (str->ToLower()->Equals("false"))
			{
				// Return boolean false
				return __box(false);
			}

			// Try parsing the user's input as an integer
			try
			{
				int value = Int32::Parse(str);
				return __box(value);
			}
			catch (Exception*)
			{
				// Parse failed, fall through to next trial
			}

			// Try parsing the user's input as a floating point number
			try
			{
				double value = Double::Parse(str);
				return __box(value);
			}
			catch (Exception*)
			{
				// Parse failed, return it as a string
				return str;
			}
		}
		return NULL;
	}

	bool DotNetReplacementLibrary::GetFunctionTests(String* category, String* functionName, FunctionTestList &testList)
	{
		const wchar_t __pin* wszCategory = PtrToStringChars(category);
		const wchar_t __pin* wszFunctionName = PtrToStringChars(functionName);

		// first get the thread specific tests
		bool returnValue = settingsManager->GetFunctionTests(GetCurrentThreadId(), (char*)SiString(wszCategory),
			(char*)SiString(wszFunctionName), testList);
		// then get the common tests
		returnValue = returnValue || (settingsManager->GetFunctionTests(0, (char*)SiString(wszCategory),
			(char*)SiString(wszFunctionName), testList));

		if ((!returnValue) || (testList.GetTestCount() == 0))
			return false;

		return true;
	}


	bool DotNetReplacementLibrary::DoesTestMatch(int paramCount, void** paramValues,
		ParameterType* paramTypes, FunctionTestList &testList, Exception* &matchingErrorCode,
		Type* returnType, Object* &matchingReturnValue, int *matchingTest)
	{
		*matchingTest = -1;
		SiString exceptionStr, returnValueStr;

		bool result = settingsManager->DoesTestMatch(paramValues, paramCount, NULL, testList,
			exceptionStr, returnValueStr, 0, true, matchingTest);

		if (result)
		{
			// Convert return value and exception strings to objects
			matchingErrorCode = ConvertExceptionStringToObject(new String((wchar_t*)exceptionStr));
			matchingReturnValue = ConvertValueStringToObject(returnType, new String((wchar_t*)returnValueStr));
		}
		return result;
	}


	void DotNetReplacementLibrary::ModifyChangeParameters(int paramCount, void** paramValues,
		ParameterType* paramTypes, FunctionTestList* testList, int matchingTest)
	{
	}


	bool DotNetReplacementLibrary::IsCategoryLoggingEnabled(String* category)
	{
		const wchar_t __pin* wszCategory = PtrToStringChars(category);
		return settingsManager->IsCategoryLoggingEnabled((char*)SiString(wszCategory));
	}


	bool DotNetReplacementLibrary::IsFunctionLoggingEnabled(String* category, String* functionName)
	{
		const wchar_t __pin* wszCategory = PtrToStringChars(category);
		const wchar_t __pin* wszFunctionName = PtrToStringChars(functionName);
		return settingsManager->IsFunctionLoggingEnabled((char*)SiString(wszCategory), (char*)SiString(wszFunctionName));
	}


	void DotNetReplacementLibrary::SendLog(String* category, String* functionName, int paramCount,
		void** paramValues, ParameterType* paramTypes, Exception* exception, Object* returnValue)
	{
		const wchar_t __pin* wszCategory = PtrToStringChars(category);
		const wchar_t __pin* wszFunctionName = PtrToStringChars(functionName);

		SiString sReturnValue = "";
		if (returnValue != NULL)
		{
			const wchar_t __pin* wszReturnValue = PtrToStringChars(returnValue->ToString());
			sReturnValue = wszReturnValue;
		}

		SiString sException = "";
		if (exception != NULL)
		{
			const wchar_t __pin* wszException = PtrToStringChars(exception->GetType()->Name);
			sException = wszException;
		}

		const wchar_t __pin* wszOriginalDLLName = PtrToStringChars(originalDllName);

		// convert threadID to string
		char threadIDStr[16];
		_itoa(GetCurrentThreadId(), threadIDStr, 10);

		LogEventList events;

		commonLogSender.SendLog(threadIDStr, wszCategory, wszFunctionName, wszOriginalDLLName, paramCount,
			paramValues, paramTypes, -1, sException, sReturnValue, events);
	}


	void DotNetReplacementLibrary::EnableInterception()
	{
		HeatDotNet::Interception::EnableInterceptionInCurrentThread();
	}


	void DotNetReplacementLibrary::DisableInterception()
	{
		HeatDotNet::Interception::DisableInterceptionInCurrentThread();
	}

	void DotNetReplacementLibrary::EnableNativeInterception()
	{
		ReplacementLibrary::EnableInterception();
	}

	void DotNetReplacementLibrary::DisableNativeInterception()
	{
		ReplacementLibrary::DisableInterception();
	}

	//*************************************************************************
	// Method:		IncrementCallLevel
	// Description: Increments the current thread's call level
	//
	// Parameters:
	//	None
	//
	// Return Value: the thread's current call level
	//*************************************************************************
	unsigned int DotNetReplacementLibrary::IncrementCallLevel()
	{
		HMODULE module;
		static DWORD (WINAPI *incrementCallLevelFunc)() = NULL;
		
		if (incrementCallLevelFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return 0;

			incrementCallLevelFunc = (DWORD (WINAPI *)())::GetProcAddress(module, INC_CALL_LEVEL_FUNC_NAME);
		}

		return incrementCallLevelFunc();
	}

	//*************************************************************************
	// Method:		DecrementCallLevel
	// Description: Decrements the current thread's call level
	//
	// Parameters:
	//	None
	//
	// Return Value: the thread's current call level
	//*************************************************************************
	unsigned int DotNetReplacementLibrary::DecrementCallLevel()
	{
		HMODULE module;
		static DWORD (WINAPI *decrementCallLevelFunc)() = NULL;
		
		if (decrementCallLevelFunc == NULL)
		{
			module = ::GetModuleHandle(HEAT_DLL_FILE_NAME);
			if (module == NULL)
				module = ::LoadLibraryEx(HEAT_DLL_FILE_NAME, NULL, 0);

			if (module == NULL)
				return 0;

			decrementCallLevelFunc = (DWORD (WINAPI *)())::GetProcAddress(module, DEC_CALL_LEVEL_FUNC_NAME);
		}

		return decrementCallLevelFunc();
	}
}
