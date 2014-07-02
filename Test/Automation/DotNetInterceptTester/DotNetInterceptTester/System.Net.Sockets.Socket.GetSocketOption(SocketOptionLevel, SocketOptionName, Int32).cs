namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class GetSocketOption_System_Net_Sockets_Socket_System_Net_Sockets_SocketOptionLevel_System_Net_Sockets_SocketOptionName_System_Int32
{
public static bool _GetSocketOption_System_Net_Sockets_Socket_System_Net_Sockets_SocketOptionLevel_System_Net_Sockets_SocketOptionName_System_Int32( )
{
   //Parameters
   System.Net.Sockets.SocketOptionLevel optionLevel = null;
   System.Net.Sockets.SocketOptionName optionName = null;
   System.Int32 optionLength = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.GetSocketOption(optionLevel,optionName,optionLength);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.GetSocketOption(optionLevel,optionName,optionLength);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
