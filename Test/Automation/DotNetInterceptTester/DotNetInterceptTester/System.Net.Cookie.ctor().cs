namespace DotNetInterceptTester.My_System.Net.Cookie
{
public class ctor_System_Net_Cookie
{
public static bool _ctor_System_Net_Cookie( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.Cookie.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.Cookie.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
