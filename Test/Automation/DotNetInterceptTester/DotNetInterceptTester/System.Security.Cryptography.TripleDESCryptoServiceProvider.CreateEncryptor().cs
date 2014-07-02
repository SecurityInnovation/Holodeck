namespace DotNetInterceptTester.My_System.Security.Cryptography.TripleDESCryptoServiceProvider
{
public class CreateEncryptor_System_Security_Cryptography_TripleDESCryptoServiceProvider
{
public static bool _CreateEncryptor_System_Security_Cryptography_TripleDESCryptoServiceProvider( )
{
   //Parameters

   //ReturnType/Value
   System.Security.Cryptography.ICryptoTransform returnVal_Real = null;
   System.Security.Cryptography.ICryptoTransform returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.TripleDESCryptoServiceProvider.CreateEncryptor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.TripleDESCryptoServiceProvider.CreateEncryptor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
