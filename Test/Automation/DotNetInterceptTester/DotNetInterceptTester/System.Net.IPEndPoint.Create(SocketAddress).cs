namespace DotNetInterceptTester.My_System.Net.IPEndPoint
{
public class Create_System_Net_IPEndPoint_System_Net_SocketAddress
{
public static bool _Create_System_Net_IPEndPoint_System_Net_SocketAddress( )
{
   //Parameters
   System.Net.SocketAddress socketAddress = null;

   //ReturnType/Value
   System.Net.EndPoint returnVal_Real = null;
   System.Net.EndPoint returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.IPEndPoint.Create(socketAddress);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.IPEndPoint.Create(socketAddress);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
