namespace DotNetInterceptTester.My_System.Security.Cryptography.RSACryptoServiceProvider
{
public class FromXmlString_System_Security_Cryptography_RSACryptoServiceProvider_System_String
{
public static bool _FromXmlString_System_Security_Cryptography_RSACryptoServiceProvider_System_String( )
{
   //Parameters
   System.String xmlString = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.RSACryptoServiceProvider.FromXmlString(xmlString);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.RSACryptoServiceProvider.FromXmlString(xmlString);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
