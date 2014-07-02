namespace DotNetInterceptTester.My_System.Net.GlobalProxySelection
{
public class ctor_System_Net_GlobalProxySelection
{
public static bool _ctor_System_Net_GlobalProxySelection( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.GlobalProxySelection.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.GlobalProxySelection.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
