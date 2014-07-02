namespace DotNetInterceptTester.My_System.Diagnostics.PerformanceCounterPermission
{
public class Intersect_System_Diagnostics_PerformanceCounterPermission_System_Security_IPermission
{
public static bool _Intersect_System_Diagnostics_PerformanceCounterPermission_System_Security_IPermission( )
{
   //Parameters
   System.Security.IPermission target = null;

   //ReturnType/Value
   System.Security.IPermission returnVal_Real = null;
   System.Security.IPermission returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.PerformanceCounterPermission.Intersect(target);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.PerformanceCounterPermission.Intersect(target);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
