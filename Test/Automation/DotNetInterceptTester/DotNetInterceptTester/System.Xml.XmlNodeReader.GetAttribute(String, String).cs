namespace DotNetInterceptTester.My_System.Xml.XmlNodeReader
{
public class GetAttribute_System_Xml_XmlNodeReader_System_String_System_String
{
public static bool _GetAttribute_System_Xml_XmlNodeReader_System_String_System_String( )
{
   //Parameters
   System.String name = null;
   System.String namespaceURI = null;

   //ReturnType/Value
   System.String returnVal_Real = null;
   System.String returnVal_Intercepted = null;

   //Exception
   Exception exception_Real = null;
   Exception exception_Intercepted = null;

   InterceptionMaintenance.disableInterception( );

   try
   {
      returnValue_Real = System.Xml.XmlNodeReader.GetAttribute(name,namespaceURI);
   }

   catch( Exception e )
   {
      exception_Real = e;
   }


   InterceptionMaintenance.enableInterception( );

   try
   {
      returnValue_Intercepted = System.Xml.XmlNodeReader.GetAttribute(name,namespaceURI);
   }

   catch( Exception e )
   {
      exception_Intercepted = e;
   }


   Return ( ( exception_Real.Messsage == exception_Intercepted.Message ) && ( returnValue_Real == returnValue_Intercepted ) );
}
}
}
