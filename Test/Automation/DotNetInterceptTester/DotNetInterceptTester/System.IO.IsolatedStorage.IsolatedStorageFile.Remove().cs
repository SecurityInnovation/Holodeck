namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFile
{
public class Remove_System_IO_IsolatedStorage_IsolatedStorageFile
{
public static bool _Remove_System_IO_IsolatedStorage_IsolatedStorageFile( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFile.Remove();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFile.Remove();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
