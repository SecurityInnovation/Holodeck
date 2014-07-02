namespace DotNetInterceptTester.My_System.IO.FileStream
{
public class ctor_System_IO_FileStream_System_IntPtr_System_IO_FileAccess_System_Boolean
{
public static bool _ctor_System_IO_FileStream_System_IntPtr_System_IO_FileAccess_System_Boolean( )
{
   //Parameters
   System.IntPtr handle = null;
   System.IO.FileAccess access = null;
   System.Boolean ownsHandle = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileStream.ctor(handle,access,ownsHandle);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileStream.ctor(handle,access,ownsHandle);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
