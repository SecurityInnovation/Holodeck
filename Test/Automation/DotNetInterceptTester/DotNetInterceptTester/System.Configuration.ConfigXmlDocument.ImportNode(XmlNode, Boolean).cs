namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class ImportNode_System_Configuration_ConfigXmlDocument_System_Xml_XmlNode_System_Boolean
{
public static bool _ImportNode_System_Configuration_ConfigXmlDocument_System_Xml_XmlNode_System_Boolean( )
{
   //Parameters
   System.Xml.XmlNode node = null;
   System.Boolean deep = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.ImportNode(node,deep);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.ImportNode(node,deep);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
