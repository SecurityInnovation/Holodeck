namespace DotNetInterceptTester.My_System.Security.Cryptography.DSACryptoServiceProvider
{
public class ImportParameters_System_Security_Cryptography_DSACryptoServiceProvider_System_Security_Cryptography_DSAParameters
{
public static bool _ImportParameters_System_Security_Cryptography_DSACryptoServiceProvider_System_Security_Cryptography_DSAParameters( )
{
   //Parameters
   System.Security.Cryptography.DSAParameters parameters = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DSACryptoServiceProvider.ImportParameters(parameters);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DSACryptoServiceProvider.ImportParameters(parameters);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
