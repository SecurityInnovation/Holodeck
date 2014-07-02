namespace DotNetInterceptTester.My_System.Net.WebProxy
{
public class ctor_System_Net_WebProxy_System_Uri
{
public static bool _ctor_System_Net_WebProxy_System_Uri( )
{
   //Parameters
   System.Uri Address = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.WebProxy.ctor(Address);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.WebProxy.ctor(Address);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
