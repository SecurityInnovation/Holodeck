namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class ctor_System_Net_Sockets_Socket_System_Net_Sockets_AddressFamily_System_Net_Sockets_SocketType_System_Net_Sockets_ProtocolType
{
public static bool _ctor_System_Net_Sockets_Socket_System_Net_Sockets_AddressFamily_System_Net_Sockets_SocketType_System_Net_Sockets_ProtocolType( )
{
   //Parameters
   System.Net.Sockets.AddressFamily addressFamily = null;
   System.Net.Sockets.SocketType socketType = null;
   System.Net.Sockets.ProtocolType protocolType = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.ctor(addressFamily,socketType,protocolType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.ctor(addressFamily,socketType,protocolType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
