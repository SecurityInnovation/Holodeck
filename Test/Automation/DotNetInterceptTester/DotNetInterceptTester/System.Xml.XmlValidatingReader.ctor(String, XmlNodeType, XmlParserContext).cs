namespace DotNetInterceptTester.My_System.Xml.XmlValidatingReader
{
public class ctor_System_Xml_XmlValidatingReader_System_String_System_Xml_XmlNodeType_System_Xml_XmlParserContext
{
public static bool _ctor_System_Xml_XmlValidatingReader_System_String_System_Xml_XmlNodeType_System_Xml_XmlParserContext( )
{
   //Parameters
   System.String xmlFragment = null;
   System.Xml.XmlNodeType fragType = null;
   System.Xml.XmlParserContext context = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlValidatingReader.ctor(xmlFragment,fragType,context);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlValidatingReader.ctor(xmlFragment,fragType,context);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
