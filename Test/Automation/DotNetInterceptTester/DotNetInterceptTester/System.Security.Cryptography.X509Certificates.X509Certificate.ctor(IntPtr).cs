namespace DotNetInterceptTester.My_System.Security.Cryptography.X509Certificates.X509Certificate
{
public class ctor_System_Security_Cryptography_X509Certificates_X509Certificate_System_IntPtr
{
public static bool _ctor_System_Security_Cryptography_X509Certificates_X509Certificate_System_IntPtr( )
{
   //Parameters
   System.IntPtr handle = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.X509Certificates.X509Certificate.ctor(handle);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.X509Certificates.X509Certificate.ctor(handle);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
