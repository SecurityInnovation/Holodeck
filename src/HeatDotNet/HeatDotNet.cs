using System;

namespace HeatDotNet
{
	[AttributeUsage(AttributeTargets.Class)]
	public class InterceptTypeName: Attribute
	{
		public String typeName;

		//*************************************************************************
		// Method:		HeatDotNet.InterceptTypeName.InterceptTypeName
		// Description: Constructor for InterceptTypeName attribute.  This
		//				attribute must be applied to an interception handler
		//				class, as it gives the name of the type to intercept.
		//
		// Parameters:
		//	name - Name of type to intercept, using the "Namespace.TypeName"
		//		format
		//
		// Return Value: None
		//*************************************************************************
		public InterceptTypeName(String name)
		{
			typeName = name;
		}
	}

	public class Interception
	{
		[ThreadStatic]
		public static int InInterceptionHandler = 0;

		[ThreadStatic]
		public static int ForceOriginalCode = 0;

		//*************************************************************************
		// Method:		HeatDotNet.Interception.DisableInterceptionInCurrentThread
		// Description: Forces all following calls to intercepted .NET functions
		//				in this thread to go to the real function without going
		//				through the handler.  Useful in logging threads, for
		//				example.
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		public static void DisableInterceptionInCurrentThread()
		{
			InInterceptionHandler++;
		}
        
		//*************************************************************************
		// Method:		HeatDotNet.Interception.EnableInterceptionInCurrentThread
		// Description: This method will undo the actions of
		//				DisableInterceptionInCurrentThread, allowing handler
		//				functions to be called for intercepted methods.
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		public static void EnableInterceptionInCurrentThread()
		{
			InInterceptionHandler--;
		}

		//*************************************************************************
		// Method:		HeatDotNet.Interception.BeginForceOriginalCode
		// Description: This method will force the next intercepted function to
		//		execute the original code, regardless of the value of
		//		InInterceptionHandler.  This flag will be automatically cleared.
		//
		// Parameters:
		//	None
		//
		// Return Value: None
		//*************************************************************************
		public static void BeginForceOriginalCode()
		{
			ForceOriginalCode = 1;
		}

		//*************************************************************************
		// Method:		HeatDotNet.Interception.ThrowException
		// Description: Throws an exception from a handler method.  Do not
		//				throw the exception explicitly, as this would cause
		//				interception to be disabled in that thread.  Use this
		//				function after processing an exception thrown by the
		//				real method.
		//
		// Parameters:
		//	e - Exception to throw
		//
		// Return Value: None, exception is thrown and interception is reenabled
		//*************************************************************************
		public static void ThrowException(Exception e)
		{
			InInterceptionHandler--;
			throw e;
		}
	}

	public abstract class MethodInterceptInfo
	{
		protected Object thisObj;

		//*************************************************************************
		// Method:		HeatDotNet.MethodInterceptInfo.MethodInterceptInfo
		// Description: Constructor for the MethodInterceptInfo class.  This
		//				is a protected member and can not be called directly.
		//				HeatDotNet will create a derived class for the specific
		//				method being intercepted.
		//
		// Parameters:
		//	thisObj - "this" object for this method call
		//
		// Return Value: None
		//*************************************************************************
		protected MethodInterceptInfo(Object obj)
		{
			thisObj = obj;
		}

		public Object ThisObject
		{
			get
			{
				return thisObj;
			}
		}

		public abstract Object Invoke(Object[] paramObjs);
	}
}
