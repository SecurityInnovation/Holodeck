namespace DotNetInterceptTester.My_System.Security.Cryptography.X509Certificates.X509Certificate
{
public class CreateFromCertFile_System_String
{
public static bool _CreateFromCertFile_System_String( )
{
   //Parameters
   System.String filename = null;

   //ReturnType/Value
   System.Security.Cryptography.X509Certificates.X509Certificate returnVal_Real = null;
   System.Security.Cryptography.X509Certificates.X509Certificate returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromCertFile(filename);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.X509Certificates.X509Certificate.CreateFromCertFile(filename);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
