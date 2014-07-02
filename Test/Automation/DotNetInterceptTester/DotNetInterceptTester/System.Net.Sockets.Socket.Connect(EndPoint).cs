namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class Connect_System_Net_Sockets_Socket_System_Net_EndPoint
{
public static bool _Connect_System_Net_Sockets_Socket_System_Net_EndPoint( )
{
   //Parameters
   System.Net.EndPoint remoteEP = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.Connect(remoteEP);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.Connect(remoteEP);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
