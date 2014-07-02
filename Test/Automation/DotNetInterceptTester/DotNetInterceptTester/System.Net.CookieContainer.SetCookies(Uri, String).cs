namespace DotNetInterceptTester.My_System.Net.CookieContainer
{
public class SetCookies_System_Net_CookieContainer_System_Uri_System_String
{
public static bool _SetCookies_System_Net_CookieContainer_System_Uri_System_String( )
{
   //Parameters
   System.Uri uri = null;
   System.String cookieHeader = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CookieContainer.SetCookies(uri,cookieHeader);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CookieContainer.SetCookies(uri,cookieHeader);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
