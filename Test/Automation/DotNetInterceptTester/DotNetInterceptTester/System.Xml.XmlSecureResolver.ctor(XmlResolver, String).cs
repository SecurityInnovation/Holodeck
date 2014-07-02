namespace DotNetInterceptTester.My_System.Xml.XmlSecureResolver
{
public class ctor_System_Xml_XmlSecureResolver_System_Xml_XmlResolver_System_String
{
public static bool _ctor_System_Xml_XmlSecureResolver_System_Xml_XmlResolver_System_String( )
{
   //Parameters
   System.Xml.XmlResolver resolver = null;
   System.String securityUrl = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlSecureResolver.ctor(resolver,securityUrl);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlSecureResolver.ctor(resolver,securityUrl);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
