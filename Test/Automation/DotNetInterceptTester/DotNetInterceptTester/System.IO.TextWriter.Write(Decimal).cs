namespace DotNetInterceptTester.My_System.IO.TextWriter
{
public class Write_System_IO_TextWriter_System_Decimal
{
public static bool _Write_System_IO_TextWriter_System_Decimal( )
{
   //Parameters
   System.Decimal _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.TextWriter.Write(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.TextWriter.Write(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
