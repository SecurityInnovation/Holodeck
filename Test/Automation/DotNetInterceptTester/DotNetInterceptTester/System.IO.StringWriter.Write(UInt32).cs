namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class Write_System_IO_StringWriter_System_UInt32
{
public static bool _Write_System_IO_StringWriter_System_UInt32( )
{
   //Parameters
   System.UInt32 _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.Write(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.Write(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
