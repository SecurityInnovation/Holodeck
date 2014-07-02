namespace DotNetInterceptTester.My_System.Security.Cryptography.RC2CryptoServiceProvider
{
public class GenerateIV_System_Security_Cryptography_RC2CryptoServiceProvider
{
public static bool _GenerateIV_System_Security_Cryptography_RC2CryptoServiceProvider( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RC2CryptoServiceProvider.GenerateIV();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RC2CryptoServiceProvider.GenerateIV();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
