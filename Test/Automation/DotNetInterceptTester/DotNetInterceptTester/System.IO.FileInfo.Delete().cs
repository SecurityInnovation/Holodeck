namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class Delete_System_IO_FileInfo
{
public static bool _Delete_System_IO_FileInfo( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.Delete();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.Delete();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
