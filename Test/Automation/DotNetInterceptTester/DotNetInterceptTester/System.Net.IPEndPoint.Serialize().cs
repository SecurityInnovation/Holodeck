namespace DotNetInterceptTester.My_System.Net.IPEndPoint
{
public class Serialize_System_Net_IPEndPoint
{
public static bool _Serialize_System_Net_IPEndPoint( )
{
   //Parameters

   //ReturnType/Value
   System.Net.SocketAddress returnVal_Real = null;
   System.Net.SocketAddress returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.IPEndPoint.Serialize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.IPEndPoint.Serialize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
