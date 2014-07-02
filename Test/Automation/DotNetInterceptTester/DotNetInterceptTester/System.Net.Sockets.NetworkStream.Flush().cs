namespace DotNetInterceptTester.My_System.Net.Sockets.NetworkStream
{
public class Flush_System_Net_Sockets_NetworkStream
{
public static bool _Flush_System_Net_Sockets_NetworkStream( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.NetworkStream.Flush();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.NetworkStream.Flush();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
