namespace DotNetInterceptTester.My_System.Net.CredentialCache
{
public class ctor_System_Net_CredentialCache
{
public static bool _ctor_System_Net_CredentialCache( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Net.CredentialCache.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Net.CredentialCache.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
