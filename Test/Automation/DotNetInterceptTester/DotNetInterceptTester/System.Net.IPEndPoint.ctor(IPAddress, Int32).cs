namespace DotNetInterceptTester.My_System.Net.IPEndPoint
{
public class ctor_System_Net_IPEndPoint_System_Net_IPAddress_System_Int32
{
public static bool _ctor_System_Net_IPEndPoint_System_Net_IPAddress_System_Int32( )
{
   //Parameters
   System.Net.IPAddress address = null;
   System.Int32 port = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.IPEndPoint.ctor(address,port);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.IPEndPoint.ctor(address,port);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
