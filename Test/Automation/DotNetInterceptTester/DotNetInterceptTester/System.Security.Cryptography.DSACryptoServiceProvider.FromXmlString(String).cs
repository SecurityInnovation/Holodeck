namespace DotNetInterceptTester.My_System.Security.Cryptography.DSACryptoServiceProvider
{
public class FromXmlString_System_Security_Cryptography_DSACryptoServiceProvider_System_String
{
public static bool _FromXmlString_System_Security_Cryptography_DSACryptoServiceProvider_System_String( )
{
   //Parameters
   System.String xmlString = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Security.Cryptography.DSACryptoServiceProvider.FromXmlString(xmlString);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Security.Cryptography.DSACryptoServiceProvider.FromXmlString(xmlString);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
