namespace DotNetInterceptTester.My_System.Diagnostics.EventLogPermission
{
public class Assert_System_Diagnostics_EventLogPermission
{
public static bool _Assert_System_Diagnostics_EventLogPermission( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogPermission.Assert();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogPermission.Assert();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
