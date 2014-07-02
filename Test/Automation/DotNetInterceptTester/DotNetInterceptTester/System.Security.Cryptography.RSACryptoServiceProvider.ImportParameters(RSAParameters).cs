namespace DotNetInterceptTester.My_System.Security.Cryptography.RSACryptoServiceProvider
{
public class ImportParameters_System_Security_Cryptography_RSACryptoServiceProvider_System_Security_Cryptography_RSAParameters
{
public static bool _ImportParameters_System_Security_Cryptography_RSACryptoServiceProvider_System_Security_Cryptography_RSAParameters( )
{
   //Parameters
   System.Security.Cryptography.RSAParameters parameters = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSACryptoServiceProvider.ImportParameters(parameters);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSACryptoServiceProvider.ImportParameters(parameters);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
