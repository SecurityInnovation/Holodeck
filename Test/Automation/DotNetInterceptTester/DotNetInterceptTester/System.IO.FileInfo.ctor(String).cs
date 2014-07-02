namespace DotNetInterceptTester.My_System.IO.FileInfo
{
public class ctor_System_IO_FileInfo_System_String
{
public static bool _ctor_System_IO_FileInfo_System_String( )
{
   //Parameters
   System.String fileName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileInfo.ctor(fileName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileInfo.ctor(fileName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
