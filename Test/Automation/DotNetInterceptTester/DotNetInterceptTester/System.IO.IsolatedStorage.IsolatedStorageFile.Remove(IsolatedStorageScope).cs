namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFile
{
public class Remove_System_IO_IsolatedStorage_IsolatedStorageScope
{
public static bool _Remove_System_IO_IsolatedStorage_IsolatedStorageScope( )
{
   //Parameters
   System.IO.IsolatedStorage.IsolatedStorageScope scope = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFile.Remove(scope);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFile.Remove(scope);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
