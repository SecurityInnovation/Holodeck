namespace DotNetInterceptTester.My_System.IO.BufferedStream
{
public class Flush_System_IO_BufferedStream
{
public static bool _Flush_System_IO_BufferedStream( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.BufferedStream.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.BufferedStream.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
