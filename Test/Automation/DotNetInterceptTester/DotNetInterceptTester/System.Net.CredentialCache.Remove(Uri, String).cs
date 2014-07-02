namespace DotNetInterceptTester.My_System.Net.CredentialCache
{
public class Remove_System_Net_CredentialCache_System_Uri_System_String
{
public static bool _Remove_System_Net_CredentialCache_System_Uri_System_String( )
{
   //Parameters
   System.Uri uriPrefix = null;
   System.String authType = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CredentialCache.Remove(uriPrefix,authType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CredentialCache.Remove(uriPrefix,authType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
