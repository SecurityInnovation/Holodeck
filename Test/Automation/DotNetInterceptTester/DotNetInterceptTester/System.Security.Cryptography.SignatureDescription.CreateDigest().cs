namespace DotNetInterceptTester.My_System.Security.Cryptography.SignatureDescription
{
public class CreateDigest_System_Security_Cryptography_SignatureDescription
{
public static bool _CreateDigest_System_Security_Cryptography_SignatureDescription( )
{
   //Parameters

   //ReturnType/Value
   System.Security.Cryptography.HashAlgorithm returnVal_Real = null;
   System.Security.Cryptography.HashAlgorithm returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.SignatureDescription.CreateDigest();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.SignatureDescription.CreateDigest();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
