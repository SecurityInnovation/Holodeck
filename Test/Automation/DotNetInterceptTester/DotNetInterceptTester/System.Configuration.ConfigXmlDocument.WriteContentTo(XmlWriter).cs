namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class WriteContentTo_System_Configuration_ConfigXmlDocument_System_Xml_XmlWriter
{
public static bool _WriteContentTo_System_Configuration_ConfigXmlDocument_System_Xml_XmlWriter( )
{
   //Parameters
   System.Xml.XmlWriter xw = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.WriteContentTo(xw);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.WriteContentTo(xw);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
