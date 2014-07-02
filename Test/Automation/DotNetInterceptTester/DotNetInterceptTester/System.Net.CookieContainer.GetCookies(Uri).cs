namespace DotNetInterceptTester.My_System.Net.CookieContainer
{
public class GetCookies_System_Net_CookieContainer_System_Uri
{
public static bool _GetCookies_System_Net_CookieContainer_System_Uri( )
{
   //Parameters
   System.Uri uri = null;

   //ReturnType/Value
   System.Net.CookieCollection returnVal_Real = null;
   System.Net.CookieCollection returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CookieContainer.GetCookies(uri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CookieContainer.GetCookies(uri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
