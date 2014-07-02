namespace DotNetInterceptTester.My_System.Security.Cryptography.RSACryptoServiceProvider
{
public class ctor_System_Security_Cryptography_RSACryptoServiceProvider_System_Int32
{
public static bool _ctor_System_Security_Cryptography_RSACryptoServiceProvider_System_Int32( )
{
   //Parameters
   System.Int32 dwKeySize = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSACryptoServiceProvider.ctor(dwKeySize);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSACryptoServiceProvider.ctor(dwKeySize);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
