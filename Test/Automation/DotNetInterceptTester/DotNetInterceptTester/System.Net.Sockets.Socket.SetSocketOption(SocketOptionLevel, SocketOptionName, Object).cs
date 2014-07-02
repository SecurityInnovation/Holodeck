namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class SetSocketOption_System_Net_Sockets_Socket_System_Net_Sockets_SocketOptionLevel_System_Net_Sockets_SocketOptionName_System_Object
{
public static bool _SetSocketOption_System_Net_Sockets_Socket_System_Net_Sockets_SocketOptionLevel_System_Net_Sockets_SocketOptionName_System_Object( )
{
   //Parameters
   System.Net.Sockets.SocketOptionLevel optionLevel = null;
   System.Net.Sockets.SocketOptionName optionName = null;
   System.Object optionValue = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.SetSocketOption(optionLevel,optionName,optionValue);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.SetSocketOption(optionLevel,optionName,optionValue);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
