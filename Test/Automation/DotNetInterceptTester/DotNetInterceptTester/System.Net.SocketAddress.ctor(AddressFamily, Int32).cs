namespace DotNetInterceptTester.My_System.Net.SocketAddress
{
public class ctor_System_Net_SocketAddress_System_Net_Sockets_AddressFamily_System_Int32
{
public static bool _ctor_System_Net_SocketAddress_System_Net_Sockets_AddressFamily_System_Int32( )
{
   //Parameters
   System.Net.Sockets.AddressFamily family = null;
   System.Int32 size = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.SocketAddress.ctor(family,size);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.SocketAddress.ctor(family,size);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
