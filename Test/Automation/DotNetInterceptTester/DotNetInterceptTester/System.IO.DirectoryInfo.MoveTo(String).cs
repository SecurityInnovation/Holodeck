namespace DotNetInterceptTester.My_System.IO.DirectoryInfo
{
public class MoveTo_System_IO_DirectoryInfo_System_String
{
public static bool _MoveTo_System_IO_DirectoryInfo_System_String( )
{
   //Parameters
   System.String destDirName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.DirectoryInfo.MoveTo(destDirName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.DirectoryInfo.MoveTo(destDirName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
