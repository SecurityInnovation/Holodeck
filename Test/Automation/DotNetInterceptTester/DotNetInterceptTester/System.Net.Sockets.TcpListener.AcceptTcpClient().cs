namespace DotNetInterceptTester.My_System.Net.Sockets.TcpListener
{
public class AcceptTcpClient_System_Net_Sockets_TcpListener
{
public static bool _AcceptTcpClient_System_Net_Sockets_TcpListener( )
{
   //Parameters

   //ReturnType/Value
   System.Net.Sockets.TcpClient returnVal_Real = null;
   System.Net.Sockets.TcpClient returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpListener.AcceptTcpClient();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpListener.AcceptTcpClient();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
