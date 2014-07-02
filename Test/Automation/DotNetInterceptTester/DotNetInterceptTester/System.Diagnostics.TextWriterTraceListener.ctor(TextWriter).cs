namespace DotNetInterceptTester.My_System.Diagnostics.TextWriterTraceListener
{
public class ctor_System_Diagnostics_TextWriterTraceListener_System_IO_TextWriter
{
public static bool _ctor_System_Diagnostics_TextWriterTraceListener_System_IO_TextWriter( )
{
   //Parameters
   System.IO.TextWriter writer = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Diagnostics.TextWriterTraceListener.ctor(writer);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Diagnostics.TextWriterTraceListener.ctor(writer);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
