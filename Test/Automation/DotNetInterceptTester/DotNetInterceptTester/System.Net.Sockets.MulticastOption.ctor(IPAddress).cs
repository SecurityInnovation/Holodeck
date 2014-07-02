namespace DotNetInterceptTester.My_System.Net.Sockets.MulticastOption
{
public class ctor_System_Net_Sockets_MulticastOption_System_Net_IPAddress
{
public static bool _ctor_System_Net_Sockets_MulticastOption_System_Net_IPAddress( )
{
   //Parameters
   System.Net.IPAddress group = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Sockets.MulticastOption.ctor(group);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Sockets.MulticastOption.ctor(group);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
