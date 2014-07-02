namespace DotNetInterceptTester.My_System.Security.Cryptography.CryptoConfig
{
public class ctor_System_Security_Cryptography_CryptoConfig
{
public static bool _ctor_System_Security_Cryptography_CryptoConfig( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.CryptoConfig.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.CryptoConfig.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
