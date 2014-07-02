namespace DotNetInterceptTester.My_System.Net.Sockets.TcpListener
{
public class Start_System_Net_Sockets_TcpListener
{
public static bool _Start_System_Net_Sockets_TcpListener( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.TcpListener.Start();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.TcpListener.Start();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
