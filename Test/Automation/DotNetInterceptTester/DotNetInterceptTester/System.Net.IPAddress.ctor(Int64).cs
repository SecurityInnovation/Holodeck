namespace DotNetInterceptTester.My_System.Net.IPAddress
{
public class ctor_System_Net_IPAddress_System_Int64
{
public static bool _ctor_System_Net_IPAddress_System_Int64( )
{
   //Parameters
   System.Int64 newAddress = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.IPAddress.ctor(newAddress);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.IPAddress.ctor(newAddress);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
