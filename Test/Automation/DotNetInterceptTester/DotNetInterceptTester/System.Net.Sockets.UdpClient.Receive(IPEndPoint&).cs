namespace DotNetInterceptTester.My_System.Net.Sockets.UdpClient
{
public class Receive_System_Net_Sockets_UdpClient_System_Net_IPEndPoint_
{
public static bool _Receive_System_Net_Sockets_UdpClient_System_Net_IPEndPoint_( )
{
   //Parameters
   System.Net.IPEndPoint& remoteEP = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.UdpClient.Receive(remoteEP);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.UdpClient.Receive(remoteEP);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
