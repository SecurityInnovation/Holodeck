namespace DotNetInterceptTester.My_System.Xml.XmlDocumentFragment
{
public class PrependChild_System_Xml_XmlDocumentFragment_System_Xml_XmlNode
{
public static bool _PrependChild_System_Xml_XmlDocumentFragment_System_Xml_XmlNode( )
{
   //Parameters
   System.Xml.XmlNode newChild = null;

   //ReturnType/Value
   System.Xml.XmlNode returnVal_Real = null;
   System.Xml.XmlNode returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocumentFragment.PrependChild(newChild);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocumentFragment.PrependChild(newChild);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
