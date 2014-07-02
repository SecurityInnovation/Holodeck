namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class GetPrefixOfNamespace_System_Xml_XmlDocument_System_String
{
public static bool _GetPrefixOfNamespace_System_Xml_XmlDocument_System_String( )
{
   //Parameters
   System.String namespaceURI = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.GetPrefixOfNamespace(namespaceURI);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.GetPrefixOfNamespace(namespaceURI);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
