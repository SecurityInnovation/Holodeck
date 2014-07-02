namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class EndConnect_System_Net_Sockets_Socket_System_IAsyncResult
{
public static bool _EndConnect_System_Net_Sockets_Socket_System_IAsyncResult( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.EndConnect(asyncResult);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.EndConnect(asyncResult);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
