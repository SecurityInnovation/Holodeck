namespace DotNetInterceptTester.My_System.Net.Sockets.TcpListener
{
public class AcceptSocket_System_Net_Sockets_TcpListener
{
public static bool _AcceptSocket_System_Net_Sockets_TcpListener( )
{
   //Parameters

   //ReturnType/Value
   System.Net.Sockets.Socket returnVal_Real = null;
   System.Net.Sockets.Socket returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpListener.AcceptSocket();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpListener.AcceptSocket();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
