namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class Open_System_IO_FileInfo_System_IO_FileMode_System_IO_FileAccess
{
public static bool _Open_System_IO_FileInfo_System_IO_FileMode_System_IO_FileAccess( )
{
   //Parameters
   System.IO.FileMode mode = null;
   System.IO.FileAccess access = null;

   //ReturnType/Value
   System.IO.FileStream returnVal_Real = null;
   System.IO.FileStream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.Open(mode,access);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.Open(mode,access);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
