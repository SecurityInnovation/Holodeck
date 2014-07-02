namespace DotNetInterceptTester.My_System.Security.Cryptography.SHA1CryptoServiceProvider
{
public class Initialize_System_Security_Cryptography_SHA1CryptoServiceProvider
{
public static bool _Initialize_System_Security_Cryptography_SHA1CryptoServiceProvider( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.SHA1CryptoServiceProvider.Initialize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.SHA1CryptoServiceProvider.Initialize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
