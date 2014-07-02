namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class Shutdown_System_Net_Sockets_Socket_System_Net_Sockets_SocketShutdown
{
public static bool _Shutdown_System_Net_Sockets_Socket_System_Net_Sockets_SocketShutdown( )
{
   //Parameters
   System.Net.Sockets.SocketShutdown how = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.Shutdown(how);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.Shutdown(how);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
