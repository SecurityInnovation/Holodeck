namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class CreateDocumentType_System_Xml_XmlDocument_System_String_System_String_System_String_System_String
{
public static bool _CreateDocumentType_System_Xml_XmlDocument_System_String_System_String_System_String_System_String( )
{
   //Parameters
   System.String name = null;
   System.String publicId = null;
   System.String systemId = null;
   System.String internalSubset = null;

   //ReturnType/Value
   System.Xml.XmlDocumentType returnVal_Real = null;
   System.Xml.XmlDocumentType returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.CreateDocumentType(name,publicId,systemId,internalSubset);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.CreateDocumentType(name,publicId,systemId,internalSubset);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
