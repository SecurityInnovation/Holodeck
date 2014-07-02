namespace DotNetInterceptTester.My_System.Net.Sockets.UdpClient
{
public class JoinMulticastGroup_System_Net_Sockets_UdpClient_System_Int32_System_Net_IPAddress
{
public static bool _JoinMulticastGroup_System_Net_Sockets_UdpClient_System_Int32_System_Net_IPAddress( )
{
   //Parameters
   System.Int32 ifindex = null;
   System.Net.IPAddress multicastAddr = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.UdpClient.JoinMulticastGroup(ifindex,multicastAddr);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.UdpClient.JoinMulticastGroup(ifindex,multicastAddr);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
