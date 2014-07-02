namespace DotNetInterceptTester.My_System.Net.CookieContainer
{
public class ctor_System_Net_CookieContainer
{
public static bool _ctor_System_Net_CookieContainer( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CookieContainer.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CookieContainer.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
