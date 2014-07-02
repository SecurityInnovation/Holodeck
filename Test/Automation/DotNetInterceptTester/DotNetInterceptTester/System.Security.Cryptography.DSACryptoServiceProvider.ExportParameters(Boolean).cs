namespace DotNetInterceptTester.My_System.Security.Cryptography.DSACryptoServiceProvider
{
public class ExportParameters_System_Security_Cryptography_DSACryptoServiceProvider_System_Boolean
{
public static bool _ExportParameters_System_Security_Cryptography_DSACryptoServiceProvider_System_Boolean( )
{
   //Parameters
   System.Boolean includePrivateParameters = null;

   //ReturnType/Value
   System.Security.Cryptography.DSAParameters returnVal_Real = null;
   System.Security.Cryptography.DSAParameters returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DSACryptoServiceProvider.ExportParameters(includePrivateParameters);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DSACryptoServiceProvider.ExportParameters(includePrivateParameters);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
