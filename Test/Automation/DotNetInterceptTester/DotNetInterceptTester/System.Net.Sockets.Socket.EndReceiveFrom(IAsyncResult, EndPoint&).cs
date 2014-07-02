namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class EndReceiveFrom_System_Net_Sockets_Socket_System_IAsyncResult_System_Net_EndPoint_
{
public static bool _EndReceiveFrom_System_Net_Sockets_Socket_System_IAsyncResult_System_Net_EndPoint_( )
{
   //Parameters
   System.IAsyncResult asyncResult = null;
   System.Net.EndPoint& endPoint = null;

   //ReturnType/Value
   System.Int32 returnVal_Real = null;
   System.Int32 returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.EndReceiveFrom(asyncResult,endPoint);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.EndReceiveFrom(asyncResult,endPoint);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
