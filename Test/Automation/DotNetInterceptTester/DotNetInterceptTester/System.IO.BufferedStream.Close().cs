namespace DotNetInterceptTester.My_System.IO.BufferedStream
{
public class Close_System_IO_BufferedStream
{
public static bool _Close_System_IO_BufferedStream( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.BufferedStream.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.BufferedStream.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
