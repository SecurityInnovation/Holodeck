namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFileStream
{
public class ctor_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_String_System_IO_FileMode_System_IO_IsolatedStorage_IsolatedStorageFile
{
public static bool _ctor_System_IO_IsolatedStorage_IsolatedStorageFileStream_System_String_System_IO_FileMode_System_IO_IsolatedStorage_IsolatedStorageFile( )
{
   //Parameters
   System.String path = null;
   System.IO.FileMode mode = null;
   System.IO.IsolatedStorage.IsolatedStorageFile isf = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFileStream.ctor(path,mode,isf);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFileStream.ctor(path,mode,isf);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
