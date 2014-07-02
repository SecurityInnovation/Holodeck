namespace DotNetInterceptTester.My_System.Configuration.ConfigXmlDocument
{
public class CreateDocumentType_System_Configuration_ConfigXmlDocument_System_String_System_String_System_String_System_String
{
public static bool _CreateDocumentType_System_Configuration_ConfigXmlDocument_System_String_System_String_System_String_System_String( )
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
      returnValue_Real = System.Configuration.ConfigXmlDocument.CreateDocumentType(name,publicId,systemId,internalSubset);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Configuration.ConfigXmlDocument.CreateDocumentType(name,publicId,systemId,internalSubset);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
