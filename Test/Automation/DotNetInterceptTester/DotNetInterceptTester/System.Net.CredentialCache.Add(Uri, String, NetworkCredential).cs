namespace DotNetInterceptTester.My_System.Net.CredentialCache
{
public class Add_System_Net_CredentialCache_System_Uri_System_String_System_Net_NetworkCredential
{
public static bool _Add_System_Net_CredentialCache_System_Uri_System_String_System_Net_NetworkCredential( )
{
   //Parameters
   System.Uri uriPrefix = null;
   System.String authType = null;
   System.Net.NetworkCredential cred = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CredentialCache.Add(uriPrefix,authType,cred);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CredentialCache.Add(uriPrefix,authType,cred);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
