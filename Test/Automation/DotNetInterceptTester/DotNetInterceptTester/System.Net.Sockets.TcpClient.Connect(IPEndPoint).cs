namespace DotNetInterceptTester.My_System.Net.Sockets.TcpClient
{
public class Connect_System_Net_Sockets_TcpClient_System_Net_IPEndPoint
{
public static bool _Connect_System_Net_Sockets_TcpClient_System_Net_IPEndPoint( )
{
   //Parameters
   System.Net.IPEndPoint remoteEP = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpClient.Connect(remoteEP);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpClient.Connect(remoteEP);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
