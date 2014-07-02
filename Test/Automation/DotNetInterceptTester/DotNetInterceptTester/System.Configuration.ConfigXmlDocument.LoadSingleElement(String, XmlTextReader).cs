namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class LoadSingleElement_System_Configuration_ConfigXmlDocument_System_String_System_Xml_XmlTextReader
{
public static bool _LoadSingleElement_System_Configuration_ConfigXmlDocument_System_String_System_Xml_XmlTextReader( )
{
   //Parameters
   System.String filename = null;
   System.Xml.XmlTextReader sourceReader = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.LoadSingleElement(filename,sourceReader);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.LoadSingleElement(filename,sourceReader);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
