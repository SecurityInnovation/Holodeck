namespace DotNetInterceptTester.My_System.Security.Cryptography.MD5CryptoServiceProvider
{
public class Initialize_System_Security_Cryptography_MD5CryptoServiceProvider
{
public static bool _Initialize_System_Security_Cryptography_MD5CryptoServiceProvider( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.MD5CryptoServiceProvider.Initialize();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.MD5CryptoServiceProvider.Initialize();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
