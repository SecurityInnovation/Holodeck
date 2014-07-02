namespace DotNetInterceptTester.My_System.Net.Sockets.TcpClient
{
public class ctor_System_Net_Sockets_TcpClient_System_Net_Sockets_AddressFamily
{
public static bool _ctor_System_Net_Sockets_TcpClient_System_Net_Sockets_AddressFamily( )
{
   //Parameters
   System.Net.Sockets.AddressFamily family = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpClient.ctor(family);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpClient.ctor(family);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
