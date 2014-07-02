namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class WriteEntry_System_Diagnostics_EventLog_System_String_System_Diagnostics_EventLogEntryType_System_Int32
{
public static bool _WriteEntry_System_Diagnostics_EventLog_System_String_System_Diagnostics_EventLogEntryType_System_Int32( )
{
   //Parameters
   System.String message = null;
   System.Diagnostics.EventLogEntryType type = null;
   System.Int32 eventID = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.WriteEntry(message,type,eventID);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.WriteEntry(message,type,eventID);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
