namespace DotNetInterceptTester.My_System.Xml.XmlAttributeCollection
{
public class RemoveNamedItem_System_Xml_XmlAttributeCollection_System_String_System_String
{
public static bool _RemoveNamedItem_System_Xml_XmlAttributeCollection_System_String_System_String( )
{
   //Parameters
   System.String localName = null;
   System.String namespaceURI = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlAttributeCollection.RemoveNamedItem(localName,namespaceURI);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlAttributeCollection.RemoveNamedItem(localName,namespaceURI);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
