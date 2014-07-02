namespace DotNetInterceptTester.My_System.IO.FileSystemInfo
{
public class Refresh_System_IO_FileSystemInfo
{
public static bool _Refresh_System_IO_FileSystemInfo( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileSystemInfo.Refresh();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileSystemInfo.Refresh();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
