namespace DotNetInterceptTester.My_System.Xml.XmlWhitespace
{
public class SelectNodes_System_Xml_XmlWhitespace_System_String_System_Xml_XmlNamespaceManager
{
public static bool _SelectNodes_System_Xml_XmlWhitespace_System_String_System_Xml_XmlNamespaceManager( )
{
   //Parameters
   System.String xpath = null;
   System.Xml.XmlNamespaceManager nsmgr = null;

   //ReturnType/Value
   System.Xml.XmlNodeList returnVal_Real = null;
   System.Xml.XmlNodeList returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlWhitespace.SelectNodes(xpath,nsmgr);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlWhitespace.SelectNodes(xpath,nsmgr);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
