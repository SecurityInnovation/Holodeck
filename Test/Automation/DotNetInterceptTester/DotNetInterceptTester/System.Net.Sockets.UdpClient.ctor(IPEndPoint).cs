namespace DotNetInterceptTester.My_System.Net.Sockets.UdpClient
{
public class ctor_System_Net_Sockets_UdpClient_System_Net_IPEndPoint
{
public static bool _ctor_System_Net_Sockets_UdpClient_System_Net_IPEndPoint( )
{
   //Parameters
   System.Net.IPEndPoint localEP = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.UdpClient.ctor(localEP);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.UdpClient.ctor(localEP);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
