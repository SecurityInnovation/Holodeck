namespace DotNetInterceptTester.My_System.IO.FileStream
{
public class Flush_System_IO_FileStream
{
public static bool _Flush_System_IO_FileStream( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileStream.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileStream.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
