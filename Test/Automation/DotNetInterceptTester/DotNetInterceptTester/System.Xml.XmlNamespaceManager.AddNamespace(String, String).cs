namespace DotNetInterceptTester.My_System.Xml.XmlNamespaceManager
{
public class AddNamespace_System_Xml_XmlNamespaceManager_System_String_System_String
{
public static bool _AddNamespace_System_Xml_XmlNamespaceManager_System_String_System_String( )
{
   //Parameters
   System.String prefix = null;
   System.String uri = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNamespaceManager.AddNamespace(prefix,uri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNamespaceManager.AddNamespace(prefix,uri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
