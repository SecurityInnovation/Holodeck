namespace DotNetInterceptTester.My_System.Diagnostics.DefaultTraceListener
{
public class ctor_System_Diagnostics_DefaultTraceListener
{
public static bool _ctor_System_Diagnostics_DefaultTraceListener( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.DefaultTraceListener.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.DefaultTraceListener.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
