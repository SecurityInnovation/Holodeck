namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class CreateElement_System_Configuration_ConfigXmlDocument_System_String_System_String_System_String
{
public static bool _CreateElement_System_Configuration_ConfigXmlDocument_System_String_System_String_System_String( )
{
   //Parameters
   System.String prefix = null;
   System.String localName = null;
   System.String namespaceUri = null;

   //ReturnType/Value
   System.Xml.XmlElement returnVal_Real = null;
   System.Xml.XmlElement returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Configuration.ConfigXmlDocument.CreateElement(prefix,localName,namespaceUri);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.CreateElement(prefix,localName,namespaceUri);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
