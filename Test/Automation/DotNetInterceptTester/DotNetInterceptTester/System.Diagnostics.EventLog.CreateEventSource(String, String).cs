namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class CreateEventSource_System_String_System_String
{
public static bool _CreateEventSource_System_String_System_String( )
{
   //Parameters
   System.String source = null;
   System.String logName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.CreateEventSource(source,logName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.CreateEventSource(source,logName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
