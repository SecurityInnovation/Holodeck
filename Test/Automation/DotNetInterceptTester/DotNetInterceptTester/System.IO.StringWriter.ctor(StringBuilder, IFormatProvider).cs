namespace DotNetInterceptTester.My_System.IO.StringWriter
{
public class ctor_System_IO_StringWriter_System_Text_StringBuilder_System_IFormatProvider
{
public static bool _ctor_System_IO_StringWriter_System_Text_StringBuilder_System_IFormatProvider( )
{
   //Parameters
   System.Text.StringBuilder sb = null;
   System.IFormatProvider formatProvider = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringWriter.ctor(sb,formatProvider);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringWriter.ctor(sb,formatProvider);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
