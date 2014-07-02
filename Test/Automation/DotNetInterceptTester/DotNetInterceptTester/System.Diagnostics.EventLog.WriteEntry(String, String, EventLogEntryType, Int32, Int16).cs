namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class WriteEntry_System_String_System_String_System_Diagnostics_EventLogEntryType_System_Int32_System_Int16
{
public static bool _WriteEntry_System_String_System_String_System_Diagnostics_EventLogEntryType_System_Int32_System_Int16( )
{
   //Parameters
   System.String source = null;
   System.String message = null;
   System.Diagnostics.EventLogEntryType type = null;
   System.Int32 eventID = null;
   System.Int16 category = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.WriteEntry(source,message,type,eventID,category);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.WriteEntry(source,message,type,eventID,category);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
