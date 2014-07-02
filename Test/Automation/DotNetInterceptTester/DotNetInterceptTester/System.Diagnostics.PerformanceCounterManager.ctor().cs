namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterManager
{
public class ctor_System_Diagnostics_PerformanceCounterManager
{
public static bool _ctor_System_Diagnostics_PerformanceCounterManager( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterManager.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterManager.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
