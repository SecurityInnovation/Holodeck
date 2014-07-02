namespace DotNetInterceptTester.My_System.IO.IsolatedStorage.IsolatedStorageFile
{
public class CreateDirectory_System_IO_IsolatedStorage_IsolatedStorageFile_System_String
{
public static bool _CreateDirectory_System_IO_IsolatedStorage_IsolatedStorageFile_System_String( )
{
   //Parameters
   System.String dir = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.IsolatedStorage.IsolatedStorageFile.CreateDirectory(dir);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.IsolatedStorage.IsolatedStorageFile.CreateDirectory(dir);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
