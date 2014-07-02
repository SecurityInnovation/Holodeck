namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class MoveTo_System_IO_FileInfo_System_String
{
public static bool _MoveTo_System_IO_FileInfo_System_String( )
{
   //Parameters
   System.String destFileName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.MoveTo(destFileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.MoveTo(destFileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
