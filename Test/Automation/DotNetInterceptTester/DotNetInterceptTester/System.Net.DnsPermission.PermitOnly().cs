namespace DotNetInterceptTester.My_System.Net.DnsPermission
{
public class PermitOnly_System_Net_DnsPermission
{
public static bool _PermitOnly_System_Net_DnsPermission( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.DnsPermission.PermitOnly();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.DnsPermission.PermitOnly();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
