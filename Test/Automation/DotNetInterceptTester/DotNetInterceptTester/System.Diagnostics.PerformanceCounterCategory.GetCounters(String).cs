namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class GetCounters_System_Diagnostics_PerformanceCounterCategory_System_String
{
public static bool _GetCounters_System_Diagnostics_PerformanceCounterCategory_System_String( )
{
   //Parameters
   System.String instanceName = null;

   //ReturnType/Value
   System.Diagnostics.PerformanceCounter[] returnVal_Real = null;
   System.Diagnostics.PerformanceCounter[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.GetCounters(instanceName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.GetCounters(instanceName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
