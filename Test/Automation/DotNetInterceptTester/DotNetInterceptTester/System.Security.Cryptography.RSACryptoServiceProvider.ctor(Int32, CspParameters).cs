namespace DotNetInterceptTester.My_System.Security.Cryptography.RSACryptoServiceProvider
{
public class ctor_System_Security_Cryptography_RSACryptoServiceProvider_System_Int32_System_Security_Cryptography_CspParameters
{
public static bool _ctor_System_Security_Cryptography_RSACryptoServiceProvider_System_Int32_System_Security_Cryptography_CspParameters( )
{
   //Parameters
   System.Int32 dwKeySize = null;
   System.Security.Cryptography.CspParameters parameters = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSACryptoServiceProvider.ctor(dwKeySize,parameters);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSACryptoServiceProvider.ctor(dwKeySize,parameters);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
