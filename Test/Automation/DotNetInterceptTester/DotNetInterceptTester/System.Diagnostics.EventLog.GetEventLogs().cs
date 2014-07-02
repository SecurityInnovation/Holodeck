namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class GetEventLogs
{
public static bool _GetEventLogs( )
{
   //Parameters

   //ReturnType/Value
   System.Diagnostics.EventLog[] returnVal_Real = null;
   System.Diagnostics.EventLog[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.GetEventLogs(;
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.GetEventLogs(;
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
