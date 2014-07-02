namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class OpenText_System_IO_FileInfo
{
public static bool _OpenText_System_IO_FileInfo( )
{
   //Parameters

   //ReturnType/Value
   System.IO.StreamReader returnVal_Real = null;
   System.IO.StreamReader returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.OpenText();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.OpenText();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
