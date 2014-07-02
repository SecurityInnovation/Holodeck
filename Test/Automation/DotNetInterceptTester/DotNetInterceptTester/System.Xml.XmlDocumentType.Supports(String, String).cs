namespace DotNetInterceptTester.My_System.Xml.XmlDocumentType
{
public class Supports_System_Xml_XmlDocumentType_System_String_System_String
{
public static bool _Supports_System_Xml_XmlDocumentType_System_String_System_String( )
{
   //Parameters
   System.String feature = null;
   System.String version = null;

   //ReturnType/Value
   System.Boolean returnVal_Real = null;
   System.Boolean returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocumentType.Supports(feature,version);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocumentType.Supports(feature,version);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
