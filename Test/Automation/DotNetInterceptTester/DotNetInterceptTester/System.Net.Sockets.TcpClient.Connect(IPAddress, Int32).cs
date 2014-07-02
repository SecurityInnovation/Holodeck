namespace DotNetInterceptTester.My_System.Net.Sockets.TcpClient
{
public class Connect_System_Net_Sockets_TcpClient_System_Net_IPAddress_System_Int32
{
public static bool _Connect_System_Net_Sockets_TcpClient_System_Net_IPAddress_System_Int32( )
{
   //Parameters
   System.Net.IPAddress address = null;
   System.Int32 port = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpClient.Connect(address,port);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpClient.Connect(address,port);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
