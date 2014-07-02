namespace DotNetInterceptTester.My_System.Net.Sockets.NetworkStream
{
public class ctor_System_Net_Sockets_NetworkStream_System_Net_Sockets_Socket_System_IO_FileAccess_System_Boolean
{
public static bool _ctor_System_Net_Sockets_NetworkStream_System_Net_Sockets_Socket_System_IO_FileAccess_System_Boolean( )
{
   //Parameters
   System.Net.Sockets.Socket socket = null;
   System.IO.FileAccess access = null;
   System.Boolean ownsSocket = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.NetworkStream.ctor(socket,access,ownsSocket);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.NetworkStream.ctor(socket,access,ownsSocket);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
