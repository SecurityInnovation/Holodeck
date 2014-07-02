namespace DotNetInterceptTester.My_System.Diagnostics.EventLogTraceListener
{
public class ctor_System_Diagnostics_EventLogTraceListener
{
public static bool _ctor_System_Diagnostics_EventLogTraceListener( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLogTraceListener.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLogTraceListener.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
