namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFileStream
{
public class WriteByte_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_Byte
{
public static bool _WriteByte_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_Byte( )
{
   //Parameters
   System.Byte _value = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFileStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFileStream.WriteByte(_value);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
