namespace DotNetInterceptTester.My_System.Diagnostics.TextWriterTraceListener
{
public class Close_System_Diagnostics_TextWriterTraceListener
{
public static bool _Close_System_Diagnostics_TextWriterTraceListener( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.TextWriterTraceListener.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.TextWriterTraceListener.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
