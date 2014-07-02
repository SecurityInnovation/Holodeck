namespace DotNetInterceptTester.My_System.Security.Cryptography.DESCryptoServiceProvider
{
public class GenerateKey_System_Security_Cryptography_DESCryptoServiceProvider
{
public static bool _GenerateKey_System_Security_Cryptography_DESCryptoServiceProvider( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DESCryptoServiceProvider.GenerateKey();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DESCryptoServiceProvider.GenerateKey();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
