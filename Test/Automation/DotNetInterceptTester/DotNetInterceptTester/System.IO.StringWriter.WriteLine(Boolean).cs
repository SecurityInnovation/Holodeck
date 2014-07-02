namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class WriteLine_System_IO_StringWriter_System_Boolean
{
public static bool _WriteLine_System_IO_StringWriter_System_Boolean( )
{
   //Parameters
   System.Boolean _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.WriteLine(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.WriteLine(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
