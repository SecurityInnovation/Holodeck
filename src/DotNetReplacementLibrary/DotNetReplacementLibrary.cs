using System;
using System.Text;
using System.Runtime.InteropServices;

namespace Replacement
{
	public delegate bool OriginalMethodCaller(Object[] param, ref Object retValue, ref Exception exception);

	public enum ParameterType
	{
		NullType = 0,
		StringType,
		WideStringType,
		BooleanType,
		IntegerType,
		UnsignedLongType,
		RealType,
		PointerType,
		PointerPointerType,
		Integer64Type,
		IgnoreType
	};

	[System.Security.SuppressUnmanagedCodeSecurityAttribute]
	public class DotNetReplacementLibrary
	{
		[System.Security.SuppressUnmanagedCodeSecurityAttribute]
		private class ReplacementLibraryWrapper
		{
			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern void InitReplacementLibraryWrapper();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern int GetSiStringLength(IntPtr str);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern bool GetSiStringContents(IntPtr str, char[] output, int len);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern void FreeSiString(IntPtr strPtr);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern IntPtr GetFunctionTests(char[] category, char[] functionName);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern void FreeFunctionTests(IntPtr testList);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern IntPtr CreateParamTypedArrays(int count);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern void FreeParamTypedArrays(IntPtr paramArrays, int count);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void SetParamString(IntPtr paramArrays, int index, char[] val);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern bool DoesTestMatch(int paramCount, IntPtr paramArrays, IntPtr testList,
				out IntPtr exceptionStr, out IntPtr returnValueStr, ref int matchingTest);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern bool IsCategoryLoggingEnabled(char[] category);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern bool IsFunctionLoggingEnabled(char[] category, char[] functionName);
			
			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void SendLog(char[] originalDllName, char[] category, char[] functionName, int paramCount,
				IntPtr paramArrays, char[] exception, char[] returnValue, IntPtr events);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern int CheckFiringSpecification (IntPtr callStack, IntPtr functionTest, int matchingTest);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern int GetFiringOptionForTest (IntPtr functionTestList, int matchingTestIndex);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void SuspendProcess();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void FlushLog();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void IncrementTestExecutionCount(IntPtr functionTest, int matchingTest);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void SetPauseEventInLogEntry();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern IntPtr InitCallStackList();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern bool DeInitCallStackList(IntPtr callStackList);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern bool AddFunctionToCallStackList(IntPtr callStackList, char[] functionName);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern IntPtr CreateNewEventsArray();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern IntPtr InitNewEventsArrayAndSavePrevious(IntPtr events);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void RestorePreviousEventsArray(IntPtr previousEventArray);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi, CharSet=CharSet.Unicode)]
			public static extern void DeleteEventArray(IntPtr eventArray);

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern bool IsParentLogged();

			[DllImport("ReplacementLibraryWrapper.dll", CallingConvention=CallingConvention.Winapi)]
			public static extern bool SetParentLogged(bool value);
		}

		private String originalDllName;
		private static IntPtr settingsManager = IntPtr.Zero;

		public DotNetReplacementLibrary(String dllName)
		{
			originalDllName = dllName;
			ReplacementLibraryWrapper.InitReplacementLibraryWrapper();
		}

		[DllImport("Heat4Dll.dll", EntryPoint="enableInterceptionInCurrentThread", CallingConvention=CallingConvention.Winapi)]
		public static extern void EnableNativeInterception();

		[DllImport("Heat4Dll.dll", EntryPoint="disableInterceptionInCurrentThread", CallingConvention=CallingConvention.Winapi)]
		public static extern void DisableNativeInterception();

		[DllImport("Heat4Dll.dll", EntryPoint="incrementCallLevel", CallingConvention=CallingConvention.Winapi)]
		private static extern uint IncrementCallLevel();

		[DllImport("Heat4Dll.dll", EntryPoint="decrementCallLevel", CallingConvention=CallingConvention.Winapi)]
		private static extern uint DecrementCallLevel();

		private bool DoesTestMatch(int paramCount, IntPtr paramArrays, IntPtr testList, ref Exception matchingErrorCode,
			Type returnType, ref Object matchingReturnValue, ref int matchingTest)
		{
			matchingTest = -1;

			IntPtr exceptionSiString;
			IntPtr returnValueSiString;

			bool result = ReplacementLibraryWrapper.DoesTestMatch(paramCount, paramArrays, testList,
				out exceptionSiString, out returnValueSiString, ref matchingTest);

			if (result)
			{
				int exceptionStrLen = ReplacementLibraryWrapper.GetSiStringLength(exceptionSiString);
				char[] exceptionStr = new char[exceptionStrLen + 1];
				ReplacementLibraryWrapper.GetSiStringContents(exceptionSiString, exceptionStr, exceptionStr.Length);

				int returnValueStrLen = ReplacementLibraryWrapper.GetSiStringLength(returnValueSiString);
				char[] returnValueStr = new char[returnValueStrLen + 1];
				ReplacementLibraryWrapper.GetSiStringContents(returnValueSiString, returnValueStr, returnValueStr.Length);

				ReplacementLibraryWrapper.FreeSiString(exceptionSiString);
				ReplacementLibraryWrapper.FreeSiString(returnValueSiString);

				// Convert return value and exception strings to objects
				matchingErrorCode = ConvertExceptionStringToObject(new String(exceptionStr));
				matchingReturnValue = ConvertValueStringToObject(returnType, new String(returnValueStr));
			}
			return result;
		}

		private void ModifyChangeParameters(int paramCount, IntPtr paramArrays, IntPtr testList, int matchingTest)
		{
		}

		private int ConvertParamsToTypedArrays(Object[] param, ref IntPtr paramArrays)
		{
			int startIndex = 0;
			// First parameter is the MethodInterceptInfo object.  If ThisObject within
			// this is null, we have a static method and shouldn't include ThisObject
			// in the log entry
			if ((param.Length > 0) && param[0].GetType().IsSubclassOf(typeof(HeatDotNet.MethodInterceptInfo)) &&
				(((HeatDotNet.MethodInterceptInfo)param[0]).ThisObject == null))
				startIndex = 1;

			paramArrays = ReplacementLibraryWrapper.CreateParamTypedArrays(param.Length - startIndex);

			for (int i=startIndex; i<param.Length; i++)
			{
				try
				{
					Object obj = param[i];

					if (obj == null)
					{
						ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex, "null".ToCharArray());
						continue;
					}

					// If the parameter is a MethodInterceptInfo type, log the ThisObject part of the
					// MethodInterceptInfo object.
					if (obj.GetType().IsSubclassOf(typeof(HeatDotNet.MethodInterceptInfo)) &&
						((HeatDotNet.MethodInterceptInfo)obj != null))
						obj = ((HeatDotNet.MethodInterceptInfo)obj).ThisObject;

					if (obj == null)
						ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex, "null".ToCharArray());
					else
					{
						if (obj is Byte)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Byte)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is SByte)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((SByte)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Int16)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Int16)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is UInt16)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((UInt16)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Int32)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Int32)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is UInt32)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((UInt32)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Int64)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Int64)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is UInt64)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((UInt64)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Single)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Single)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Double)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Double)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Decimal)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Decimal)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Boolean)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Boolean)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Char)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Char)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is DateTime)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((DateTime)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Enum)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Enum)obj).ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is Guid)
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
								((Guid)obj).ToString("B", System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
						}
						else if (obj is IntPtr)
						{
							if (IntPtr.Size == 4)
							{
								ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
									((IntPtr)obj).ToInt32().ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
							}
							else
							{
								ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
									((IntPtr)obj).ToInt64().ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
							}
						}
						else if (obj is UIntPtr)
						{
							if (UIntPtr.Size == 4)
							{
								ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
									((UIntPtr)obj).ToUInt32().ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
							}
							else
							{
								ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex,
									((UIntPtr)obj).ToUInt64().ToString(System.Globalization.CultureInfo.InvariantCulture).ToCharArray());
							}
						}
						else if ((obj is Type) || (obj is String) || (obj is StringBuilder) ||
							(obj is Microsoft.Win32.RegistryKey))
						{
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex, obj.ToString().ToCharArray());
						}
						else
							ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex, obj.GetType().Name.ToCharArray());
					}
				}
				catch (Exception)
				{
					// Could not convert the parameter to a string.  Fill in the parameter with an "Unknown"
					ReplacementLibraryWrapper.SetParamString(paramArrays, i - startIndex, "Unknown".ToCharArray());
				}
			}

			return param.Length - startIndex; // Return actual number of parameters to log
		}

		private void FreeParamTypedArrays(int paramCount, IntPtr paramArrays)
		{
			ReplacementLibraryWrapper.FreeParamTypedArrays(paramArrays, paramCount);
		}

		private Exception ConvertExceptionStringToObject(String str)
		{
			Type exceptionType = Type.GetType(str);
			if (exceptionType == null)
				return null;

			System.Reflection.ConstructorInfo constructor = exceptionType.GetConstructor(new Type[0]);
			if (constructor == null)
				return null;

			return (Exception)constructor.Invoke(new Object[0]);
		}

		private Object ConvertValueStringToObject(Type returnType, String str)
		{
			if (returnType == typeof(String))
				return str;
			else if ((returnType == typeof(Boolean)) || (returnType == typeof(Byte)) ||
				(returnType == typeof(SByte)) || (returnType == typeof(Int16)) ||
				(returnType == typeof(UInt16)) || (returnType == typeof(Int32)) ||
				(returnType == typeof(UInt32)) || (returnType == typeof(Int64)) ||
				(returnType == typeof(UInt64)) || (returnType == typeof(Single)) ||
				(returnType == typeof(Double)))
			{
				// Numeric type, call the Parse method
				System.Reflection.MethodInfo method = returnType.GetMethod("Parse", new Type[]{typeof(String)});
				if (method != null)
				{
					try
					{
						return method.Invoke(null, new Object[]{str});
					}
					catch (Exception)
					{
						// Parse failed.  Return zero, as NULL won't work as it's a value type
						return 0;
					}
				}
				else
				{
					// No Parse method.  Return zero, as NULL won't work as it's a value type
					return 0;
				}
			}
			else if (returnType == typeof(Object))
			{
				if (str.ToLower().Equals("null"))
				{
					// User gave "null", return a NULL pointer
					return null;
				}

				if (str.ToLower().Equals("true"))
				{
					// Return boolean true
					return true;
				}

				if (str.ToLower().Equals("false"))
				{
					// Return boolean false
					return false;
				}

				// Try parsing the user's input as an integer
				try
				{
					int val = Int32.Parse(str);
					return val;
				}
				catch (Exception)
				{
					// Parse failed, fall through to next trial
				}

				// Try parsing the user's input as a floating point number
				try
				{
					double val = Double.Parse(str);
					return val;
				}
				catch (Exception)
				{
					// Parse failed, return it as a string
					return str;
				}
			}
			return null;
		}

		public bool RunStandardTestsAndGetResults(OriginalMethodCaller ofCaller, String category, String functionName,
			Object[] param, Type returnType, ref Object returnValue, ref Exception exception)
		{
			IncrementCallLevel();

			bool shouldPauseApp = false;
			bool runOriginalFunction = true;

			IntPtr newEvents = ReplacementLibraryWrapper.CreateNewEventsArray ();			
			IntPtr previousEvents = ReplacementLibraryWrapper.InitNewEventsArrayAndSavePrevious (newEvents);
			
			bool testMatches = false;
			int matchingTest = -1;
			IntPtr testList;

			IntPtr paramArrays = IntPtr.Zero;

			int typedArrayParams = ConvertParamsToTypedArrays(param, ref paramArrays);

			testList = ReplacementLibraryWrapper.GetFunctionTests(category.ToCharArray(), functionName.ToCharArray());
			if (testList != IntPtr.Zero)
			{
				testMatches = DoesTestMatch(typedArrayParams, paramArrays, testList,
					ref exception, returnType, ref returnValue, ref matchingTest);
			}

			//If matching test was found then Check Firing Specification
			if (testMatches)
			{
				int result = this.CheckFiringSpecification (testList, matchingTest);
				switch (result)
				{
					case -1:
						runOriginalFunction = true;
						shouldPauseApp  = false;
						break;
					case 0:
						runOriginalFunction = false;
						shouldPauseApp = false;
						break;
					case 1:
						runOriginalFunction = false;
						shouldPauseApp = true;
						break;
				}
			}

			bool loggingEnabled = ReplacementLibraryWrapper.IsFunctionLoggingEnabled(category.ToCharArray(), functionName.ToCharArray());
			bool parentLogged = ReplacementLibraryWrapper.IsParentLogged();

			if (!parentLogged)
			{
				// If parent is not logged, do not log this function either
				loggingEnabled = false;
			}

			ReplacementLibraryWrapper.SetParentLogged(loggingEnabled);
			
			if (runOriginalFunction)
				ofCaller(param, ref returnValue, ref exception);
			else
			{
				if (matchingTest != -1)
				{
					ReplacementLibraryWrapper.IncrementTestExecutionCount(testList, matchingTest);
				}
			}

			//If AUT should be paused after this test then set the pause event in the current LogEntry
			if (!runOriginalFunction && shouldPauseApp)
			{
				ReplacementLibraryWrapper.SetPauseEventInLogEntry();
			}

			if (matchingTest != -1)
				ModifyChangeParameters(typedArrayParams, paramArrays, testList, matchingTest);

			if (loggingEnabled)
				SendLog(category, functionName, typedArrayParams, paramArrays, exception, returnValue, newEvents);

			//If the app is going to be paused the flush the log now
			if (!runOriginalFunction && shouldPauseApp)
			{
				//Flush the log
				ReplacementLibraryWrapper.FlushLog();
			}

			// Restore original parent logging value
			ReplacementLibraryWrapper.SetParentLogged(parentLogged);

			ReplacementLibraryWrapper.RestorePreviousEventsArray (previousEvents);
			ReplacementLibraryWrapper.DeleteEventArray(newEvents);

			if (testList != IntPtr.Zero)
				ReplacementLibraryWrapper.FreeFunctionTests(testList);
			FreeParamTypedArrays(typedArrayParams, paramArrays);

			DecrementCallLevel();

			//Performing app pause here
			if (!runOriginalFunction && shouldPauseApp)
			{
				ReplacementLibraryWrapper.SuspendProcess();
			}

			return (!runOriginalFunction);
		}

		public void SendLog(String category, String functionName, int paramCount, IntPtr paramArrays,
			Exception exception, Object returnValue, IntPtr events)
		{
			String exceptionStr = "";
			String returnValueStr = "";

			if (returnValue != null)
				returnValueStr = returnValue.ToString();
			if (exception != null)
				exceptionStr = exception.GetType().Name;

			ReplacementLibraryWrapper.SendLog(originalDllName.ToCharArray(), category.ToCharArray(),
				functionName.ToCharArray(), paramCount, paramArrays, exceptionStr.ToCharArray(),
				returnValueStr.ToCharArray(), events);
		}

		public static void EnableInterception()
		{
			HeatDotNet.Interception.EnableInterceptionInCurrentThread();
		}

		public static void DisableInterception()
		{
			HeatDotNet.Interception.DisableInterceptionInCurrentThread();
		}

		/// <summary>
		/// Checks the Firing Specification for the specified function test and retuns a value
		/// indicating wheter the test should be run!
		/// </summary>
		/// <param name="functionTestList">The function test list</param>
		/// <param name="matchingTest">the index of the test which matched</param>
		/// <returns>(INT)  -1 = Run Original Function, Do not run test, do not pause app
		///					 0 = Dont run original function, run test, do not pause app
		///					 1 = Dont run original function, run test and pause app
		/// </returns>
		private int CheckFiringSpecification(IntPtr functionTestList, int matchingTest)
		{
			int retVal = 0;

			//Check the type of firing specification
			int firingOption = ReplacementLibraryWrapper.GetFiringOptionForTest (functionTestList, matchingTest);
			
			//Get stacktrace only if firing spec. requires a stackstrace
			if (firingOption != 1)
			{
				//Get the stack trace!
				System.Diagnostics.StackTrace st = new System.Diagnostics.StackTrace(System.Threading.Thread.CurrentThread, false);
			
				//Initialize a new stack list
				IntPtr stackList = ReplacementLibraryWrapper.InitCallStackList ();
				
				for(int i =0; i< st.FrameCount; i++ )
				{
					//Get the frame from the stack trace
					System.Diagnostics.StackFrame sf = st.GetFrame(i);
					if (sf == null)
						continue;
				
					System.Reflection.MethodBase mbase = sf.GetMethod();
					if (mbase == null)
						continue;
				
					String declTypeFullName = (mbase.DeclaringType == null) ? "" : mbase.DeclaringType.FullName + ".";
					String methodName = (mbase.Name == null) ? "[NULL]" : mbase.Name;
				
					//Cycle through the results and keep only the intercepted functions and internal module symbols
					//Filter rules:
					// 1. Filter out where fullname contains string "::ReplacementLibrary::"
					//	  This will take care of RunStandardTestsAndGetResults and other methods
					// 2. Filter out where full name contains string "heatintercepthandler"
				
					if ((declTypeFullName.ToLower().IndexOf (".dotnetreplacementlibrary") != -1) ||
						(declTypeFullName.ToLower().IndexOf ("heatintercepthandler") != -1))
						continue;

					ReplacementLibraryWrapper.AddFunctionToCallStackList (stackList, (declTypeFullName + methodName).ToCharArray());
				}

				//Check the firing specification
				retVal = ReplacementLibraryWrapper.CheckFiringSpecification (stackList, functionTestList, matchingTest);

				//Delete the stack list
				ReplacementLibraryWrapper.DeInitCallStackList (stackList);
			}
			else
			{
				//Check the firing specification
				retVal = ReplacementLibraryWrapper.CheckFiringSpecification ((IntPtr) null, functionTestList, matchingTest);
			}

			return retVal;
		}
	}
}
