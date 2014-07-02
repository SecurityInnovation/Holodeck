namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class Bind_System_Net_Sockets_Socket_System_Net_EndPoint
{
public static bool _Bind_System_Net_Sockets_Socket_System_Net_EndPoint( )
{
   //Parameters
   System.Net.EndPoint localEP = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.Bind(localEP);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.Bind(localEP);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
