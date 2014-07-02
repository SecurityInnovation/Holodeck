namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class Refresh_System_IO_FileInfo
{
public static bool _Refresh_System_IO_FileInfo( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.Refresh();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.Refresh();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
