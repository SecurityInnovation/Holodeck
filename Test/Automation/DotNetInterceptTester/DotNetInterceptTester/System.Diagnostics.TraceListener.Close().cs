namespace DotNetInterceptTester.My_System.Diagnostics.TraceListener
{
public class Close_System_Diagnostics_TraceListener
{
public static bool _Close_System_Diagnostics_TraceListener( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.TraceListener.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.TraceListener.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
