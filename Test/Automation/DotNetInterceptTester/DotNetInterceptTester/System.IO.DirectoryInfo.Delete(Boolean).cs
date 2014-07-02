namespace DotNetInterceptTester.My_System.IO.DirectoryInfo
{
public class Delete_System_IO_DirectoryInfo_System_Boolean
{
public static bool _Delete_System_IO_DirectoryInfo_System_Boolean( )
{
   //Parameters
   System.Boolean recursive = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.DirectoryInfo.Delete(recursive);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.DirectoryInfo.Delete(recursive);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
