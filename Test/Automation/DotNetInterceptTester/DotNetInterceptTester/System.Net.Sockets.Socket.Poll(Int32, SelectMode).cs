namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class Poll_System_Net_Sockets_Socket_System_Int32_System_Net_Sockets_SelectMode
{
public static bool _Poll_System_Net_Sockets_Socket_System_Int32_System_Net_Sockets_SelectMode( )
{
   //Parameters
   System.Int32 microSeconds = null;
   System.Net.Sockets.SelectMode mode = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.Poll(microSeconds,mode);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.Poll(microSeconds,mode);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
