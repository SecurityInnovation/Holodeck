namespace DotNetInterceptTester.My_System.Xml.XmlSecureResolver
{
public class ctor_System_Xml_XmlSecureResolver_System_Xml_XmlResolver_System_Security_PermissionSet
{
public static bool _ctor_System_Xml_XmlSecureResolver_System_Xml_XmlResolver_System_Security_PermissionSet( )
{
   //Parameters
   System.Xml.XmlResolver resolver = null;
   System.Security.PermissionSet permissionSet = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlSecureResolver.ctor(resolver,permissionSet);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlSecureResolver.ctor(resolver,permissionSet);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
