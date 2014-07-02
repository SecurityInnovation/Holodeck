namespace DotNetInterceptTester.My_System.Net.Sockets.NetworkStream
{
public class Close_System_Net_Sockets_NetworkStream
{
public static bool _Close_System_Net_Sockets_NetworkStream( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.NetworkStream.Close();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.NetworkStream.Close();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
