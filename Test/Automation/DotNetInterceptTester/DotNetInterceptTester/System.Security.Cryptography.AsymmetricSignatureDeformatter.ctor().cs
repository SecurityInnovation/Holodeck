namespace DotNetInterceptTester.My_System.Security.Cryptography.AsymmetricSignatureDeformatter
{
public class ctor_System_Security_Cryptography_AsymmetricSignatureDeformatter
{
public static bool _ctor_System_Security_Cryptography_AsymmetricSignatureDeformatter( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.AsymmetricSignatureDeformatter.ctor();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.AsymmetricSignatureDeformatter.ctor();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
