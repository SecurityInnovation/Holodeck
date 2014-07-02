namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class GetSocketOption_System_Net_Sockets_Socket_System_Net_Sockets_SocketOptionLevel_System_Net_Sockets_SocketOptionName
{
public static bool _GetSocketOption_System_Net_Sockets_Socket_System_Net_Sockets_SocketOptionLevel_System_Net_Sockets_SocketOptionName( )
{
   //Parameters
   System.Net.Sockets.SocketOptionLevel optionLevel = null;
   System.Net.Sockets.SocketOptionName optionName = null;

   //ReturnType/Value
   System.Object returnVal_Real = null;
   System.Object returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.GetSocketOption(optionLevel,optionName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.GetSocketOption(optionLevel,optionName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
