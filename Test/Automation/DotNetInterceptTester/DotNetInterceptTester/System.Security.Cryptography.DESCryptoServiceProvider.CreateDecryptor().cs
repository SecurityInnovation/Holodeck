namespace DotNetInterceptTester.My_System.Security.Cryptography.DESCryptoServiceProvider
{
public class CreateDecryptor_System_Security_Cryptography_DESCryptoServiceProvider
{
public static bool _CreateDecryptor_System_Security_Cryptography_DESCryptoServiceProvider( )
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
      returnValue_Real = System.Security.Cryptography.DESCryptoServiceProvider.CreateDecryptor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DESCryptoServiceProvider.CreateDecryptor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
