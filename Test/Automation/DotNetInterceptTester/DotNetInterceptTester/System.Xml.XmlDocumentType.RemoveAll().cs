namespace DotNetInterceptTester.My_System.Xml.XmlDocumentType
{
public class RemoveAll_System_Xml_XmlDocumentType
{
public static bool _RemoveAll_System_Xml_XmlDocumentType( )
{
   //Parameters


   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocumentType.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocumentType.RemoveAll();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


}
}
}
