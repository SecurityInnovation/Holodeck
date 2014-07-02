namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFileStream
{
public class EndWrite_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_IAsyncResult
{
public static bool _EndWrite_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_IAsyncResult( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFileStream.EndWrite(asyncResult);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFileStream.EndWrite(asyncResult);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
