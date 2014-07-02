namespace DotNetInterceptTester.My_System.IO.DirectoryInfo
{
public class ctor_System_IO_DirectoryInfo_System_String
{
public static bool _ctor_System_IO_DirectoryInfo_System_String( )
{
   //Parameters
   System.String path = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.DirectoryInfo.ctor(path);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.DirectoryInfo.ctor(path);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
