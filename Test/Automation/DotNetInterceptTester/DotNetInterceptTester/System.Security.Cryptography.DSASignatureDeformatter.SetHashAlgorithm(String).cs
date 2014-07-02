namespace DotNetInterceptTester.My_System.Security.Cryptography.DSASignatureDeformatter
{
public class SetHashAlgorithm_System_Security_Cryptography_DSASignatureDeformatter_System_String
{
public static bool _SetHashAlgorithm_System_Security_Cryptography_DSASignatureDeformatter_System_String( )
{
   //Parameters
   System.String strName = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DSASignatureDeformatter.SetHashAlgorithm(strName);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DSASignatureDeformatter.SetHashAlgorithm(strName);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
