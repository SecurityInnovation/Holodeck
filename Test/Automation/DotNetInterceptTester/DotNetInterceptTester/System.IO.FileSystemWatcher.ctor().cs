namespace DotNetInterceptTester.My_System.IO.FileSystemWatcher
{
public class ctor_System_IO_FileSystemWatcher
{
public static bool _ctor_System_IO_FileSystemWatcher( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileSystemWatcher.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileSystemWatcher.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
