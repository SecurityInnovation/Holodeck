namespace DotNetInterceptTester.My_System.IO.File
{
public class Open_System_String_System_IO_FileMode_System_IO_FileAccess_System_IO_FileShare
{
public static bool _Open_System_String_System_IO_FileMode_System_IO_FileAccess_System_IO_FileShare( )
{
   //Parameters
   System.String path = null;
   System.IO.FileMode mode = null;
   System.IO.FileAccess access = null;
   System.IO.FileShare share = null;

   //ReturnType/Value
   System.IO.FileStream returnVal_Real = null;
   System.IO.FileStream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.File.Open(path,mode,access,share);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.File.Open(path,mode,access,share);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
