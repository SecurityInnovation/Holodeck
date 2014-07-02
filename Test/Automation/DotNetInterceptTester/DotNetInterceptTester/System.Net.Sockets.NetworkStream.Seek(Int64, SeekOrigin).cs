namespace DotNetInterceptTester.My_System.Net.Sockets.NetworkStream
{
public class Seek_System_Net_Sockets_NetworkStream_System_Int64_System_IO_SeekOrigin
{
public static bool _Seek_System_Net_Sockets_NetworkStream_System_Int64_System_IO_SeekOrigin( )
{
   //Parameters
   System.Int64 offset = null;
   System.IO.SeekOrigin origin = null;

   //ReturnType/Value
   System.Int64 returnVal_Real = null;
   System.Int64 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.NetworkStream.Seek(offset,origin);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.NetworkStream.Seek(offset,origin);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
