namespace DotNetInterceptTester.My_System.Security.Cryptography.RSAPKCS1SignatureFormatter
{
public class CreateSignature_System_Security_Cryptography_RSAPKCS1SignatureFormatter_System_Security_Cryptography_HashAlgorithm
{
public static bool _CreateSignature_System_Security_Cryptography_RSAPKCS1SignatureFormatter_System_Security_Cryptography_HashAlgorithm( )
{
   //Parameters
   System.Security.Cryptography.HashAlgorithm hash = null;

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSAPKCS1SignatureFormatter.CreateSignature(hash);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSAPKCS1SignatureFormatter.CreateSignature(hash);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
