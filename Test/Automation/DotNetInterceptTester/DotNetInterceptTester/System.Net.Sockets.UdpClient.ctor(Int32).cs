namespace DotNetInterceptTester.My_System.Net.Sockets.UdpClient
{
public class ctor_System_Net_Sockets_UdpClient_System_Int32
{
public static bool _ctor_System_Net_Sockets_UdpClient_System_Int32( )
{
   //Parameters
   System.Int32 port = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.UdpClient.ctor(port);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.UdpClient.ctor(port);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
