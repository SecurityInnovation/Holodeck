namespace DotNetInterceptTester.My_System.Net.Sockets.TcpClient
{
public class ctor_System_Net_Sockets_TcpClient_System_String_System_Int32
{
public static bool _ctor_System_Net_Sockets_TcpClient_System_String_System_Int32( )
{
   //Parameters
   System.String hostname = null;
   System.Int32 port = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpClient.ctor(hostname,port);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpClient.ctor(hostname,port);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
