namespace DotNetInterceptTester.My_System.Net.Dns
{
public class GetHostByAddress_System_Net_IPAddress
{
public static bool _GetHostByAddress_System_Net_IPAddress( )
{
   //Parameters
   System.Net.IPAddress address = null;

   //ReturnType/Value
   System.Net.IPHostEntry returnVal_Real = null;
   System.Net.IPHostEntry returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Dns.GetHostByAddress(address);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Dns.GetHostByAddress(address);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
