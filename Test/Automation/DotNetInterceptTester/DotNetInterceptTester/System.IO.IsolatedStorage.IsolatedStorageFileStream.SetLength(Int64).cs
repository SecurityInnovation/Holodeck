namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFileStream
{
public class SetLength_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_Int64
{
public static bool _SetLength_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_Int64( )
{
   //Parameters
   System.Int64 _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFileStream.SetLength(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFileStream.SetLength(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
