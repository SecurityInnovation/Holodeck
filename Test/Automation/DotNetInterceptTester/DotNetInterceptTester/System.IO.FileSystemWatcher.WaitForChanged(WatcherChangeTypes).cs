namespace DotNetInterceptTester.My_System.IO.FileSystemWatcher
{
public class WaitForChanged_System_IO_FileSystemWatcher_System_IO_WatcherChangeTypes
{
public static bool _WaitForChanged_System_IO_FileSystemWatcher_System_IO_WatcherChangeTypes( )
{
   //Parameters
   System.IO.WatcherChangeTypes changeType = null;

   //ReturnType/Value
   System.IO.WaitForChangedResult returnVal_Real = null;
   System.IO.WaitForChangedResult returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileSystemWatcher.WaitForChanged(changeType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileSystemWatcher.WaitForChanged(changeType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}