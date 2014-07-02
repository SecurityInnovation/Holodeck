namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterCategory
{
public class GetInstanceNames_System_Diagnostics_PerformanceCounterCategory
{
public static bool _GetInstanceNames_System_Diagnostics_PerformanceCounterCategory( )
{
   //Parameters

   //ReturnType/Value
   System.String[] returnVal_Real = null;
   System.String[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterCategory.GetInstanceNames();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterCategory.GetInstanceNames();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
