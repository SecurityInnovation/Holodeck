namespace DotNetInterceptTester.My_System.Net.Sockets.Socket
{
public class BeginConnect_System_Net_Sockets_Socket_System_Net_EndPoint_System_AsyncCallback_System_Object
{
public static bool _BeginConnect_System_Net_Sockets_Socket_System_Net_EndPoint_System_AsyncCallback_System_Object( )
{
   //Parameters
   System.Net.EndPoint remoteEP = null;
   System.AsyncCallback callback = null;
   System.Object state = null;

   //ReturnType/Value
   System.IAsyncResult returnVal_Real = null;
   System.IAsyncResult returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.Socket.BeginConnect(remoteEP,callback,state);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.Socket.BeginConnect(remoteEP,callback,state);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
