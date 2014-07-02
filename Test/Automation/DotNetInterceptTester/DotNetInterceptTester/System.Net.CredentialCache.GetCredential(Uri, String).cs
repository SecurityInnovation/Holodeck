namespace DotNetInterceptTester.My_System.Net.CredentialCache
{
public class GetCredential_System_Net_CredentialCache_System_Uri_System_String
{
public static bool _GetCredential_System_Net_CredentialCache_System_Uri_System_String( )
{
   //Parameters
   System.Uri uriPrefix = null;
   System.String authType = null;

   //ReturnType/Value
   System.Net.NetworkCredential returnVal_Real = null;
   System.Net.NetworkCredential returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CredentialCache.GetCredential(uriPrefix,authType);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CredentialCache.GetCredential(uriPrefix,authType);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
