namespace DotNetInterceptTester.My_System.IO.FileStream
{
public class Lock_System_IO_FileStream_System_Int64_System_Int64
{
public static bool _Lock_System_IO_FileStream_System_Int64_System_Int64( )
{
   //Parameters
   System.Int64 position = null;
   System.Int64 length = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.IO.FileStream.Lock(position,length);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.IO.FileStream.Lock(position,length);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
