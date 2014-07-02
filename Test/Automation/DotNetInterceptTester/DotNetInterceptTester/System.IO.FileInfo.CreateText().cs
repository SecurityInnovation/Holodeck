namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class CreateText_System_IO_FileInfo
{
public static bool _CreateText_System_IO_FileInfo( )
{
   //Parameters

   //ReturnType/Value
   System.IO.StreamWriter returnVal_Real = null;
   System.IO.StreamWriter returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.CreateText();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.CreateText();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
