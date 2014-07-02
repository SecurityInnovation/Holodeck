namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class Close_System_IO_StringWriter
{
public static bool _Close_System_IO_StringWriter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
