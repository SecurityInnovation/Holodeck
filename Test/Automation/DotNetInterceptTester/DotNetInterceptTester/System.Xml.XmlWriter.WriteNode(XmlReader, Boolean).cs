namespace DotNetInterceptTester.My_System.Xml.XmlWriter
{
public class WriteNode_System_Xml_XmlWriter_System_Xml_XmlReader_System_Boolean
{
public static bool _WriteNode_System_Xml_XmlWriter_System_Xml_XmlReader_System_Boolean( )
{
   //Parameters
   System.Xml.XmlReader reader = null;
   System.Boolean defattr = null;


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlWriter.WriteNode(reader,defattr);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlWriter.WriteNode(reader,defattr);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
