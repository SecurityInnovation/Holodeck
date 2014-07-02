namespace DotNetInterceptTester.My_System.Net.CookieContainer
{
public class GetCookieHeader_System_Net_CookieContainer_System_Uri
{
public static bool _GetCookieHeader_System_Net_CookieContainer_System_Uri( )
{
   //Parameters
   System.Uri uri = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CookieContainer.GetCookieHeader(uri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CookieContainer.GetCookieHeader(uri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
