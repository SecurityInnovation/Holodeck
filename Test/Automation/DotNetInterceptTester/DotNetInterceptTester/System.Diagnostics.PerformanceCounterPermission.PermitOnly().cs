namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterPermission
{
public class PermitOnly_System_Diagnostics_PerformanceCounterPermission
{
public static bool _PermitOnly_System_Diagnostics_PerformanceCounterPermission( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterPermission.PermitOnly();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterPermission.PermitOnly();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
