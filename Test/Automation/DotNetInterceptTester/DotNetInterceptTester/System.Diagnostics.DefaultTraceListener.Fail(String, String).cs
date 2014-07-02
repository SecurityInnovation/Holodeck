namespace DotNetInterceptTester.My_System.Diagnostics.DefaultTraceListener
{
public class Fail_System_Diagnostics_DefaultTraceListener_System_String_System_String
{
public static bool _Fail_System_Diagnostics_DefaultTraceListener_System_String_System_String( )
{
   //Parameters
   System.String message = null;
   System.String detailMessage = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.DefaultTraceListener.Fail(message,detailMessage);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.DefaultTraceListener.Fail(message,detailMessage);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
