namespace DotNetInterceptTester.My_System.Security.Cryptography.X509Certificates.X509Certificate
{
public class GetRawCertData_System_Security_Cryptography_X509Certificates_X509Certificate
{
public static bool _GetRawCertData_System_Security_Cryptography_X509Certificates_X509Certificate( )
{
   //Parameters

   //ReturnType/Value
   System.Byte[] returnVal_Real = null;
   System.Byte[] returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.X509Certificates.X509Certificate.GetRawCertData();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.X509Certificates.X509Certificate.GetRawCertData();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
