namespace DotNetInterceptTester.My_System.IO.StringReader
{
public class ctor_System_IO_StringReader_System_String
{
public static bool _ctor_System_IO_StringReader_System_String( )
{
   //Parameters
   System.String s = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.StringReader.ctor(s);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.StringReader.ctor(s);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
