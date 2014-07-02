namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterPermission
{
public class Demand_System_Diagnostics_PerformanceCounterPermission
{
public static bool _Demand_System_Diagnostics_PerformanceCounterPermission( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterPermission.Demand();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterPermission.Demand();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
