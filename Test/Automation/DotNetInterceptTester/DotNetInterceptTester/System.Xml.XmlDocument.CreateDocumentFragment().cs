namespace DotNetInterceptTester.My_System.Xml.XmlDocument
{
public class CreateDocumentFragment_System_Xml_XmlDocument
{
public static bool _CreateDocumentFragment_System_Xml_XmlDocument( )
{
   //Parameters

   //ReturnType/Value
   System.Xml.XmlDocumentFragment returnVal_Real = null;
   System.Xml.XmlDocumentFragment returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlDocument.CreateDocumentFragment();
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlDocument.CreateDocumentFragment();
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
