namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class Load_System_Configuration_ConfigXmlDocument_System_Xml_XmlReader
{
public static bool _Load_System_Configuration_ConfigXmlDocument_System_Xml_XmlReader( )
{
   //Parameters
   System.Xml.XmlReader reader = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.Load(reader);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.Load(reader);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
