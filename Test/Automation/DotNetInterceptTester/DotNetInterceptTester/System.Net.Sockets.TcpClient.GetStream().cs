namespace DotNetInterceptTester.My_System.Net.Sockets.TcpClient
{
public class GetStream_System_Net_Sockets_TcpClient
{
public static bool _GetStream_System_Net_Sockets_TcpClient( )
{
   //Parameters

   //ReturnType/Value
   System.Net.Sockets.NetworkStream returnVal_Real = null;
   System.Net.Sockets.NetworkStream returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpClient.GetStream();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpClient.GetStream();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
