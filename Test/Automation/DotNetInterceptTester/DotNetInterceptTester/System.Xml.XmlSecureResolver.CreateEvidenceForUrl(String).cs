namespace DotNetInterceptTester.My_System.Xml.XmlSecureResolver
{
public class CreateEvidenceForUrl_System_String
{
public static bool _CreateEvidenceForUrl_System_String( )
{
   //Parameters
   System.String securityUrl = null;

   //ReturnType/Value
   System.Security.Policy.Evidence returnVal_Real = null;
   System.Security.Policy.Evidence returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlSecureResolver.CreateEvidenceForUrl(securityUrl);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlSecureResolver.CreateEvidenceForUrl(securityUrl);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
