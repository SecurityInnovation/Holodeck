namespace DotNetInterceptTester.My_System.IO.FileStream
{
public class ctor_System_IO_FileStream_System_String_System_IO_FileMode
{
public static bool _ctor_System_IO_FileStream_System_String_System_IO_FileMode( )
{
   //Parameters
   System.String path = null;
   System.IO.FileMode mode = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileStream.ctor(path,mode);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileStream.ctor(path,mode);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
