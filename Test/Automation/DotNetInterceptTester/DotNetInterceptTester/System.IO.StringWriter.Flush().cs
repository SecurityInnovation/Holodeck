namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class Flush_System_IO_StringWriter
{
public static bool _Flush_System_IO_StringWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
