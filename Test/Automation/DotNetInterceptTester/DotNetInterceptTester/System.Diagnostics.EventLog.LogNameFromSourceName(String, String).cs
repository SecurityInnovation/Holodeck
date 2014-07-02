namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class LogNameFromSourceName_System_String_System_String
{
public static bool _LogNameFromSourceName_System_String_System_String( )
{
   //Parameters
   System.String source = null;
   System.String machineName = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.LogNameFromSourceName(source,machineName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.LogNameFromSourceName(source,machineName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
