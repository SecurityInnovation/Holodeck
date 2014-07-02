namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class Delete_System_String_System_String
{
public static bool _Delete_System_String_System_String( )
{
   //Parameters
   System.String logName = null;
   System.String machineName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.Delete(logName,machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.Delete(logName,machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
