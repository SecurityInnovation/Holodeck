namespace DotNetInterceptTester.My_System.Diagnostics.EventLogEntry
{
public class Dispose_System_Diagnostics_EventLogEntry
{
public static bool _Dispose_System_Diagnostics_EventLogEntry( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogEntry.Dispose();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogEntry.Dispose();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
