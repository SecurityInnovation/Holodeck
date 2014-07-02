namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class ctor_System_IO_StringWriter
{
public static bool _ctor_System_IO_StringWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
