namespace DotNetInterceptTester.My_System.Diagnostics.DefaultTraceListener
{
public class WriteLine_System_Diagnostics_DefaultTraceListener_System_String
{
public static bool _WriteLine_System_Diagnostics_DefaultTraceListener_System_String( )
{
   //Parameters
   System.String message = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.DefaultTraceListener.WriteLine(message);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.DefaultTraceListener.WriteLine(message);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
