namespace DotNetInterceptTester.My_System.Net.CookieContainer
{
public class Add_System_Net_CookieContainer_System_Uri_System_Net_Cookie
{
public static bool _Add_System_Net_CookieContainer_System_Uri_System_Net_Cookie( )
{
   //Parameters
   System.Uri uri = null;
   System.Net.Cookie cookie = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CookieContainer.Add(uri,cookie);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CookieContainer.Add(uri,cookie);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
