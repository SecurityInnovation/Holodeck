namespace DotNetInterceptTester.My_System.Security.Cryptography.DSASignatureDeformatter
{
public class SetKey_System_Security_Cryptography_DSASignatureDeformatter_System_Security_Cryptography_AsymmetricAlgorithm
{
public static bool _SetKey_System_Security_Cryptography_DSASignatureDeformatter_System_Security_Cryptography_AsymmetricAlgorithm( )
{
   //Parameters
   System.Security.Cryptography.AsymmetricAlgorithm key = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DSASignatureDeformatter.SetKey(key);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DSASignatureDeformatter.SetKey(key);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
