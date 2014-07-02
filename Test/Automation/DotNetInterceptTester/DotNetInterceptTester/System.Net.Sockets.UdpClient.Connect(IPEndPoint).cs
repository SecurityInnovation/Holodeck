namespace DotNetInterceptTester.My_System.Net.Sockets.UdpClient
{
public class Connect_System_Net_Sockets_UdpClient_System_Net_IPEndPoint
{
public static bool _Connect_System_Net_Sockets_UdpClient_System_Net_IPEndPoint( )
{
   //Parameters
   System.Net.IPEndPoint endPoint = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.UdpClient.Connect(endPoint);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.UdpClient.Connect(endPoint);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
