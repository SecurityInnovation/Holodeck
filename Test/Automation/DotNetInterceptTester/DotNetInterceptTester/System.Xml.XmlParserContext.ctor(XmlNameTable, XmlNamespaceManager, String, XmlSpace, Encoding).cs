namespace DotNetInterceptTester.My_System.Xml.XmlParserContext
{
public class ctor_System_Xml_XmlParserContext_System_Xml_XmlNameTable_System_Xml_XmlNamespaceManager_System_String_System_Xml_XmlSpace_System_Text_Encoding
{
public static bool _ctor_System_Xml_XmlParserContext_System_Xml_XmlNameTable_System_Xml_XmlNamespaceManager_System_String_System_Xml_XmlSpace_System_Text_Encoding( )
{
   //Parameters
   System.Xml.XmlNameTable nt = null;
   System.Xml.XmlNamespaceManager nsMgr = null;
   System.String xmlLang = null;
   System.Xml.XmlSpace xmlSpace = null;
   System.Text.Encoding enc = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlParserContext.ctor(nt,nsMgr,xmlLang,xmlSpace,enc);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlParserContext.ctor(nt,nsMgr,xmlLang,xmlSpace,enc);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
