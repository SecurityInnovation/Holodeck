namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class CreateXmlDeclaration_System_Xml_XmlDocument_System_String_System_String_System_String
{
public static bool _CreateXmlDeclaration_System_Xml_XmlDocument_System_String_System_String_System_String( )
{
   //Parameters
   System.String version = null;
   System.String encoding = null;
   System.String standalone = null;

   //ReturnType/Value
   System.Xml.XmlDeclaration returnVal_Real = null;
   System.Xml.XmlDeclaration returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.CreateXmlDeclaration(version,encoding,standalone);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.CreateXmlDeclaration(version,encoding,standalone);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
