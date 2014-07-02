namespace DotNetInterceptTester.My_System.Xml.XmlElement
{
public class SelectSingleNode_System_Xml_XmlElement_System_String_System_Xml_XmlNamespaceManager
{
public static bool _SelectSingleNode_System_Xml_XmlElement_System_String_System_Xml_XmlNamespaceManager( )
{
   //Parameters
   System.String xpath = null;
   System.Xml.XmlNamespaceManager nsmgr = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlElement.SelectSingleNode(xpath,nsmgr);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlElement.SelectSingleNode(xpath,nsmgr);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
