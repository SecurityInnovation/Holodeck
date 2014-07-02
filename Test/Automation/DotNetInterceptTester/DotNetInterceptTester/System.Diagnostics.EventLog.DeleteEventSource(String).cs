namespace DotNetInterceptTester.My_System.Diagnostics.EventLog
{
public class DeleteEventSource_System_String
{
public static bool _DeleteEventSource_System_String( )
{
   //Parameters
   System.String source = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.EventLog.DeleteEventSource(source);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.EventLog.DeleteEventSource(source);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
