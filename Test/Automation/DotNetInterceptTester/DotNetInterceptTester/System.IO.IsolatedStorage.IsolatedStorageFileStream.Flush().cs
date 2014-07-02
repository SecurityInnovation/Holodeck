namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFileStream
{
public class Flush_System_IO_IsolatedStorage_IsolatedStorageFileStream
{
public static bool _Flush_System_IO_IsolatedStorage_IsolatedStorageFileStream( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFileStream.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFileStream.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
