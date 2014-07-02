namespace DotNetInterceptTester.My_System.Security.Cryptography.SignatureDescription
{
public class CreateDeformatter_System_Security_Cryptography_SignatureDescription_System_Security_Cryptography_AsymmetricAlgorithm
{
public static bool _CreateDeformatter_System_Security_Cryptography_SignatureDescription_System_Security_Cryptography_AsymmetricAlgorithm( )
{
   //Parameters
   System.Security.Cryptography.AsymmetricAlgorithm key = null;

   //ReturnType/Value
   System.Security.Cryptography.AsymmetricSignatureDeformatter returnVal_Real = null;
   System.Security.Cryptography.AsymmetricSignatureDeformatter returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.SignatureDescription.CreateDeformatter(key);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.SignatureDescription.CreateDeformatter(key);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}